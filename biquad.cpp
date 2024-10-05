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

float horizontal_sum_avx(__m256 vec) {
    // Suma los elementos adyacentes dentro de cada mitad de 128 bits
    __m128 low = _mm256_castps256_ps128(vec); // Baja parte de vec (4 floats)
    __m128 high = _mm256_extractf128_ps(vec, 1); // Alta parte de vec (4 floats)
    
    // Suma las dos mitades
    __m128 sum128 = _mm_add_ps(low, high);

    // Realiza la suma horizontal en los 4 elementos de sum128
    sum128 = _mm_hadd_ps(sum128, sum128); // Suma pares de elementos
    sum128 = _mm_hadd_ps(sum128, sum128); // Repite la suma

    // Devuelve el valor final como float
    return _mm_cvtss_f32(sum128);
}

// Método para procesar un bloque de datos con AVX
bool biquad::process(jack_nframes_t nframes, const sample_t* in, sample_t* out) {
    size_t simd_size = 4;  // Procesamos 4 muestras a la vez
    size_t i = 0;

    // Vector para almacenar las 4 entradas (x[n], x[n+1], x[n+2], x[n+3]) y los estados (z1_, z2_)
    __m256 X_avx;

    while (i + simd_size <= nframes) {
        // Cargar las 4 entradas en el vector X (junto con z1_ y z2_)
        X_avx = _mm256_set_ps(in[i], in[i+1], in[i+2], in[i+3],z1_,z2_, 0.0f, 0.0f);

        // Multiplicar la primera fila de A por X
        __m256 result1 = _mm256_mul_ps(A_row_1, X_avx);
        float sum1 = horizontal_sum_avx(result1);
        out[i] = sum1;
        
        __m256 result2 = _mm256_mul_ps(A_row_2, X_avx);
        float sum2 = horizontal_sum_avx(result2);
        out[i+1] = sum2;
        std::cout<<out[i+1]<<std::endl;
        __m256 result3 = _mm256_mul_ps(A_row_3, X_avx);
        out[i+2] = horizontal_sum_avx(result3);
        
        __m256 result4 = _mm256_mul_ps(A_row_4, X_avx);
        float sum3=horizontal_sum_avx(result4);
        out[i+3] = sum3;
        
        // Actualizar los estados intermedios (z1_, z2_)
        
        z1_ = b1_ * in[i+3] - a1_ * sum3 + b2_ * in[i+2] - a2_ * out[i+2];
        z2_ = b2_ * in[i+3] - a2_ * out[i+3];
        

        std::cout<<"z1 "<<out[i+3]<<" z2 "<<z2_<<std::endl;

        i += simd_size;  // de 4 en 4
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