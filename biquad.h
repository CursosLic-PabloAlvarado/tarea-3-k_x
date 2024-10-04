#ifndef BIQUAD_H
#define BIQUAD_H

#include <vector>
#include <jack_client.h>
#include <cassert>
#include <cmath>  // Para std::abs
#include <immintrin.h>  // Para usar las intrinsics AVX

class biquad : public jack::client {
public:
    // Coeficientes del filtro
    sample_t b0_, b1_, b2_, a1_, a2_; //parametros del filtro
    sample_t a_, b_, c_; //parametros para la implementación optimizada
    sample_t z1_, z2_;
    __m256 b0_avx, b1_avx, b2_avx, a1_avx, a2_avx;
    __m256 a_avx, b_avx, c_avx;  // Nuevos coeficientes precargados en AVX
    __m256 A_avx[4];
    // Variables de estado
    char set_mode_;
    
    // Constructor por defecto
    biquad();

    // Destructor
    ~biquad();

    // Método para establecer el modo de operación
    void setMode(char mode);

    // Método para inicializar los coeficientes
    void setCoefficients(const std::vector<sample_t>& coeffs);

    // Método para procesar un bloque de muestras
    bool process(jack_nframes_t nframes, const sample_t* in, sample_t* out);

    // Método para procesar una sola muestra
    sample_t processSample(sample_t input);
};

#endif  // BIQUAD_H
