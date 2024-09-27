#include "biquad.h"
#include <xsimd/xsimd.hpp>
#include <iostream>

// Constructor por defecto
biquad::biquad() : b0_(0.0), b1_(0.0), b2_(0.0), a1_(0.0), a2_(0.0), z1_(0.0), z2_(0.0), set_mode_('p') {
}

// Destructor
biquad::~biquad() {
}

// Método para procesar el bloque de datos usando xsimd
bool biquad::process(jack_nframes_t nframes, const sample_t* const in, sample_t* out) {
    size_t simd_size = xsimd::batch<sample_t>::size;  // Tamaño del batch SIMD
    size_t i = 0;

    // Cargar los coeficientes escalares en batches SIMD
    xsimd::batch<sample_t> b0_batch = xsimd::batch<sample_t>(b0_);
    xsimd::batch<sample_t> b1_batch = xsimd::batch<sample_t>(b1_);
    xsimd::batch<sample_t> b2_batch = xsimd::batch<sample_t>(b2_);
    xsimd::batch<sample_t> a1_batch = xsimd::batch<sample_t>(a1_);
    xsimd::batch<sample_t> a2_batch = xsimd::batch<sample_t>(a2_);

    // Variables de estado temporales SIMD
    xsimd::batch<sample_t> z1_batch = xsimd::batch<sample_t>(z1_);
    xsimd::batch<sample_t> z2_batch = xsimd::batch<sample_t>(z2_);

    // Procesar en bloques de tamaño SIMD
    for (; i + simd_size <= nframes; i += simd_size) {
        // Cargar el bloque de datos de entrada en un batch SIMD
        xsimd::batch<sample_t> input_batch = xsimd::load_unaligned(&in[i]);

        // Aplicar el filtro biquad a las muestras usando SIMD
        xsimd::batch<sample_t> output_batch = b0_batch * input_batch + z1_batch;
        z1_batch = b1_batch * input_batch - a1_batch * output_batch + z2_batch;
        z2_batch = b2_batch * input_batch - a2_batch * output_batch;

        // Almacenar el resultado procesado en el bloque de salida
        output_batch.store_unaligned(&out[i]);
    }

    // Actualiza los estados con el último valor procesado
    z1_ = z1_batch.get(simd_size - 1);  // Usar el último valor del bloque SIMD
    z2_ = z2_batch.get(simd_size - 1);

    // Procesar las muestras restantes de una en una
    for (; i < nframes; ++i) {
        out[i] = b0_ * in[i] + z1_;
        z1_ = b1_ * in[i] - a1_ * out[i] + z2_;
        z2_ = b2_ * in[i] - a2_ * out[i];
    }

    return true;
}


// Método para inicializar los coeficientes
void biquad::setCoefficients(const std::vector<sample_t>& coeffs) {
    b0_ = coeffs[0];
    b1_ = coeffs[1];
    b2_ = coeffs[2];
    a1_ = coeffs[4];
    a2_ = coeffs[5];

    // Reiniciamos el estado del filtro
    z1_ = 0.0f;
    z2_ = 0.0f;
}
