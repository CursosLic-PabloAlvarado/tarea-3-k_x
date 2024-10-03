
#include "biquad.h"
#include <xsimd/xsimd.hpp>
#include <iostream>
using sample_t = jack::client::sample_t;
// Constructor por defecto
biquad::biquad() : b0_(0.0), b1_(0.0), b2_(0.0), a1_(0.0), a2_(0.0), z1_(0.0), z2_(0.0), set_mode_('p'), last_input_(0.0), last_output_(0.0) {
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

    // Inicializamos los estados del filtro en 0 al principio
    z1_ = 0.0f;
    z2_ = 0.0f;
}

// Método para procesar un bloque de datos con SIMD (4 muestras simultáneas)
bool biquad::process(jack_nframes_t nframes, const sample_t* in, sample_t* out) {
    size_t simd_size = 4;  // Procesamos 4 muestras a la vez
    size_t i = 0;

    // Procesar en bloques de 4 muestras
    while (i + simd_size <= nframes) {
        // Cargar las entradas en un vector SIMD (4 muestras)
        xsimd::batch<sample_t> x_batch0 = xsimd::load_unaligned(&in[i]);
        xsimd::batch<sample_t> x_batch1 = xsimd::load_unaligned(&in[i+1]);
        xsimd::batch<sample_t> x_batch2 = xsimd::load_unaligned(&in[i+2]);
        xsimd::batch<sample_t> x_batch3 = xsimd::load_unaligned(&in[i+3]);

        // Calcular las salidas y[n], y[n+1], y[n+2], y[n+3] en términos de las entradas y las condiciones iniciales del bloque anterior
        xsimd::batch<sample_t> y_batch0 = b0_ * x_batch0 + z1_;
        xsimd::batch<sample_t> y_batch1 = b0_ * x_batch1 + (b1_ * x_batch0 - a1_ * z1_ + z2_);
        xsimd::batch<sample_t> y_batch2 = b0_ * x_batch2 + (b1_ * x_batch1 + b2_ * x_batch0 - a1_ * z2_ - a2_ * z1_);
        xsimd::batch<sample_t> y_batch3 = b0_ * x_batch3 + (b1_ * x_batch2 + b2_ * x_batch1 - a1_ * z2_ - a2_ * z1_);

        // Almacenar las salidas en el buffer de salida
        y_batch0.store_unaligned(&out[i]);
        y_batch1.store_unaligned(&out[i+1]);
        y_batch2.store_unaligned(&out[i+2]);
        y_batch3.store_unaligned(&out[i+3]);

        // Actualizar los estados intermedios
        z1_ = b1_ * x_batch3.get(0) - a1_ * y_batch3.get(0) + b2_ * x_batch2.get(0) - a2_ * y_batch2.get(0);
        z2_ = b2_ * x_batch3.get(0) - a2_ * y_batch3.get(0);

        i += simd_size;
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