#include "cascade.h"
#include <cstring>  // Para memcpy

// Constructor
cascade::cascade(const std::vector<std::vector<sample_t>>& filter_coeffs) {
    for (const auto& coeffs : filter_coeffs) {
        auto filter = std::make_unique<biquad>();  // Crear un nuevo biquad
        filter->setCoefficients(coeffs);           // Inicializar los coeficientes
        filters_.push_back(std::move(filter));     // Mover el puntero único al vector
    }
}

// Procesar el bloque de datos utilizando la función `process` con SIMD optimizado
bool cascade::process(jack_nframes_t nframes, const sample_t* in, sample_t* out) {
    std::vector<sample_t> intermediate_buffer_1(nframes); // Buffer intermedio para las muestras procesadas
    std::vector<sample_t> intermediate_buffer_2(nframes); // Otro buffer para la salida temporal

    // Copiar la entrada al buffer intermedio para el primer filtro
    std::memcpy(intermediate_buffer_1.data(), in, sizeof(sample_t) * nframes);

    // Procesar a través de cada biquad en cascada
    for (auto& filter : filters_) {
        filter->process(nframes, intermediate_buffer_1.data(), intermediate_buffer_2.data());  // Usar SIMD para procesar todo el bloque

        // Intercambiar los buffers para la siguiente etapa del filtro
        std::swap(intermediate_buffer_1, intermediate_buffer_2);
    }

    // Copiar el resultado final al buffer de salida
    std::memcpy(out, intermediate_buffer_1.data(), sizeof(sample_t) * nframes);

    return true;
}

/*
// Procesar el bloque de datos utilizando loop unrolling (8 muestras por iteración)
bool cascade::process(jack_nframes_t nframes, const sample_t* in, sample_t* out) {
   const sample_t *inptr=in;
   sample_t *outptr=out;
   const sample_t *const end_ptr=in+nframes;

    // Copiar la entrada al buffer intermedio
    while (inptr + 8 <= end_ptr) {
        for (size_t i = 0; i < 8; ++i) {
            sample_t sample = *inptr++;

            // Aplicamos cada filtro biquad en cascada
            for (auto& filter : filters_) {
                sample = filter->processSample(sample);
            }

            *outptr++ = sample;
        }
    }

    // Procesamos las muestras restantes
    while (inptr != end_ptr) {
        sample_t sample = *inptr++;

        // Aplicamos cada filtro biquad en cascada
        for (auto& filter : filters_) {
            sample = filter->processSample(sample);
        }

        *outptr++ = sample;
    }

    return true;
}

*/
