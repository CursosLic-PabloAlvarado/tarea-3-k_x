#include "cascade.h"
#include <xsimd/xsimd.hpp>
#include <cstring>  // Para memcpy

// Constructor
cascade::cascade(const std::vector<std::vector<sample_t>>& filter_coeffs) {
    for (const auto& coeffs : filter_coeffs) {
        auto filter = std::make_unique<biquad>();  // Crear un nuevo biquad
        filter->setCoefficients(coeffs);           // Inicializar los coeficientes
        filters_.push_back(std::move(filter));     // Mover el puntero único al vector
    }
}

// Procesar el bloque de datos utilizando xsimd
bool cascade::process(jack_nframes_t nframes, const sample_t* in, sample_t* out) {
    size_t simd_size = xsimd::batch<sample_t>::size;  // Tamaño del batch SIMD
    size_t i = 0;

    std::vector<sample_t> intermediate_buffer_1(nframes); // buffer de entrada
    std::vector<sample_t> intermediate_buffer_2(nframes); // buffer de salida

    // Copiar la entrada al buffer intermedio
    memcpy(intermediate_buffer_1.data(), in, sizeof(sample_t) * nframes);

    // Procesar cada etapa biquad en cascada
    for (auto& filter : filters_) {
        filter->process(nframes, intermediate_buffer_1.data(), intermediate_buffer_2.data());  // Aplicar el filtro

        // Intercambiar los buffers para la siguiente etapa
        std::swap(intermediate_buffer_1, intermediate_buffer_2);
    }

    // Copiar el resultado final al buffer de salida
    memcpy(out, intermediate_buffer_1.data(), sizeof(sample_t) * nframes);

    return true;
}
