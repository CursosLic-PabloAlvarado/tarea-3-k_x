
#include "biquad.h"
#include <xsimd/xsimd.hpp>
#include <iostream>
using sample_t = jack::client::sample_t;
// Constructor por defecto
biquad::biquad() : b0_(0.0), b1_(0.0), b2_(0.0), a1_(0.0), a2_(0.0), z1_(0.0), z2_(0.0), set_mode_('p'){
}

// Destructor
biquad::~biquad() {
}

// Método para inicializar los coeficientes
void biquad::setCoefficients(const std::vector<sample_t>& coeffs) {
    b0_ = coeffs[0];
    b1_ = coeffs[1];
    b2_ = coeffs[2];
    a1_ = coeffs[4];
    a2_ = coeffs[5];
    a_= -a1_*b1_+a1_*a1_*b1_+b2_-a2_*b1_;
    b_= -a1_*a1_*b1_-a1_*a1_*a1_*b1_-a1_*b2_+a1_*a2_*b1_-a2_*b1_+a2_*a1_*b1_;
    c_=-a1_*a1_*a1_+a1_*a2_+a2_*a1_;
    // Precargar los coeficientes en los registros AVX
    b0_avx = _mm256_set1_ps(b0_);
    b1_avx = _mm256_set1_ps(b1_);
    b2_avx = _mm256_set1_ps(b2_);
    a1_avx = _mm256_set1_ps(a1_);
    a2_avx = _mm256_set1_ps(a2_);
    // Precargar a_, b_, y c_ en registros AVX
    a_avx = _mm256_set1_ps(a_);
    b_avx = _mm256_set1_ps(b_);
    c_avx = _mm256_set1_ps(c_);
    // Pre-cargar la matriz A en AVX: 4 filas para representar las operaciones sobre 4 muestras
    A_avx[0] = _mm256_set_ps(b0_, b0_, b0_, b0_, 0.0f, 0.0f, 0.0f, 0.0f);  // Fila 1 para b0
    A_avx[1] = _mm256_set_ps(b1_, b1_, b1_, b1_, z1_, z2_, 0.0f, 0.0f);  // Fila 2 para b1 y estados
    A_avx[2] = _mm256_set_ps(b2_, b2_, b2_, b2_, 0.0f, 0.0f, z1_, z2_);  // Fila 3 para b2 y estados
    A_avx[3] = _mm256_set_ps(-a1_, -a1_, -a1_, -a1_, 0.0f, 0.0f, z1_, z2_);  // Fila 4 para a1

    // Inicializamos los estados del filtro en 0 al principio
    z1_ = 0.0f;
    z2_ = 0.0f;
}

// Método para procesar un bloque de datos con AVX
bool biquad::process(jack_nframes_t nframes, const sample_t* in, sample_t* out) {
    size_t simd_size = 4;  // Procesamos 4 muestras a la vez
    size_t i = 0;

    // Vector para almacenar las 4 entradas (x[n], x[n+1], x[n+2], x[n+3]) y los estados (z1_, z2_)
    __m256 X_avx;

    while (i + simd_size <= nframes) {
        // Cargar las 4 entradas en el vector X (junto con z1_ y z2_)
      __m256 X_avx = _mm256_set_ps(0.0f, 0.0f, z2_, z1_, in[i+3], in[i+2], in[i+1], in[i]);

        // Calcular las salidas y[n], y[n+1], y[n+2], y[n+3] como producto de A * X
        __m256 y_avx = _mm256_add_ps(
            _mm256_mul_ps(A_avx[0], X_avx),  // b0_*x[n], b0_*x[n+1] ...
            _mm256_add_ps(
                _mm256_mul_ps(A_avx[1], X_avx),  // b1_*x[n], b1_*x[n+1] ...
                _mm256_add_ps(
                    _mm256_mul_ps(A_avx[2], X_avx),  // b2_*x[n]...
                    _mm256_mul_ps(A_avx[3], X_avx)   // -a1_*y[n]...
                )
            )
        );

        // Guardar las salidas en el buffer
        _mm256_storeu_ps(&out[i], y_avx);

        // Actualizar los estados intermedios (z1_, z2_)
        z1_ = in[i+3];  // último valor de la entrada
        z2_ = out[i+3];  // último valor de la salida

        i += simd_size;  // de 4 en 4
    }

    // Procesar cualquier muestra restante de forma escalar
    for (; i < nframes; ++i) {
        out[i] = b0_ * in[i] + z1_;
        z1_ = b1_ * in[i] - a1_ * out[i] + z2_;
        z2_ = b2_ * in[i] - a2_ * out[i];
    }

    return true;
}


// Método para procesar una sola muestra
sample_t biquad::processSample(sample_t input) {
    sample_t output = input * b0_ + z1_;
    z1_ = input * b1_ - a1_ * output + z2_;
    z2_ = input * b2_ - a2_ * output;
    return output;
}