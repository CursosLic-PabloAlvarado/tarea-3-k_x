#include "biquad.h"
#include <immintrin.h>  // Para AVX
#include <iostream>
#include <cmath>
using sample_t = jack::client::sample_t;

// Constructor por defecto
biquad::biquad() : b0_(0.0), b1_(0.0), b2_(0.0), a1_(0.0), a2_(0.0), z1_(0.0), z2_(0.0), set_mode_('p') {
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
    a_ = -a1_*b0_ + b1_;
    b_ = a1_*a1_ *b0_-a1_*b1_-a2_*b0_+b2_; //x[n] en y[n+2]
    c_ = -a1_*b0_+b1_;          //x[n+1] en y[n+2]
    d_ = a1_*a1_-a2_;    //z1 en y[n+2]

    e_ = -a1_*a1_*a1_*b0_+a1_*a1_*b1_+2*a1_*a2_*b0_-a1_*b2_-a2_*b1_;   //x[n] en y[n+3]
    f_ = a1_*a1_*b0_-a1_*b1_-a2_*b0_+b2_; //x[n+1]
    h_ = -a1_*b0_+b1_;   //x[n+2]
    i_ = -a1_*a1_*a1_+2*a1_*a2_; //z1
    j_ = a1_*a1_-a2_; // z2

    // Pre-cargar la matriz A en AVX: 4 filas para representar las operaciones sobre 4 muestras
    A_row_1 = _mm256_set_ps(b0_, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
    A_row_2 = _mm256_set_ps(a_, b0_, 0.0f, 0.0f, -a1_, 1.0f, 0.0f, 0.0f);
    A_row_3 = _mm256_set_ps(b_, c_, b0_, 0.0f, d_, -a1_, 0.0f, 0.0f);
    A_row_4 = _mm256_set_ps(e_, f_, h_, b0_, i_, j_, 0.0f, 0.0f);

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
        X_avx = _mm256_set_ps(in[i], in[i+2], in[i+3],in[i+4],z1_,z2_,0.0f,0.0f);

        // Multiplicar la primera fila de A por X
        __m256 result1 = _mm256_mul_ps(A_row_1, X_avx);
        __m256 sum1 = _mm256_hadd_ps(result1, result1);
        sum1 = _mm256_hadd_ps(sum1, sum1);
        float sum1_high = _mm_cvtss_f32(_mm256_extractf128_ps(sum1, 1));
        float sum1_low = _mm_cvtss_f32(_mm256_castps256_ps128(sum1));
        out[i] = sum1_high + sum1_low;
        //
        __m256 result2 = _mm256_mul_ps(A_row_2, X_avx);
        __m256 sum2 = _mm256_hadd_ps(result2, result2);
        sum2 = _mm256_hadd_ps(sum1, sum1);
        float sum2_high = _mm_cvtss_f32(_mm256_extractf128_ps(sum2, 1));
        float sum2_low = _mm_cvtss_f32(_mm256_castps256_ps128(sum2));
        out[i+1] = sum2_high + sum2_low;
        //
        __m256 result3 = _mm256_mul_ps(A_row_3, X_avx);
        __m256 sum3 = _mm256_hadd_ps(result3, result3);
        sum3 = _mm256_hadd_ps(sum3, sum3);
        float sum3_high = _mm_cvtss_f32(_mm256_extractf128_ps(sum3, 1));
        float sum3_low = _mm_cvtss_f32(_mm256_castps256_ps128(sum3));
        out[i+2] = sum3_high + sum3_low;
        //
        __m256 result4 = _mm256_mul_ps(A_row_4, X_avx);
        __m256 sum4 = _mm256_hadd_ps(result4, result4);
        sum4 = _mm256_hadd_ps(sum4, sum4);
        float sum4_high = _mm_cvtss_f32(_mm256_extractf128_ps(sum4, 1));
        float sum4_low = _mm_cvtss_f32(_mm256_castps256_ps128(sum4));
        out[i+3] = sum4_high + sum4_low;
        

        // Actualizar los estados intermedios (z1_, z2_)
        z1_ = in[i+3];  // último valor de la entrada
        z2_ = out[i];   // último valor de la salida

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