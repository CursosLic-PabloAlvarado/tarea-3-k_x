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

// Procesar el bloque de datos utilizando loop unrolling (8 muestras por iteración)
bool cascade::process(jack_nframes_t nframes, const sample_t* in, sample_t* out) {
    std::vector<sample_t> intermediate_buffer_1(nframes); // buffer de entrada
    std::vector<sample_t> intermediate_buffer_2(nframes); // buffer de salida

    // Copiar la entrada al buffer intermedio
    memcpy(intermediate_buffer_1.data(), in, sizeof(sample_t) * nframes);

    // Procesar cada etapa biquad en cascada
    for (auto& filter : filters_) {
        size_t i = 0;

        // Aplicar loop unrolling para procesar 8 muestras por iteración
        for (; i + 7 < nframes; i += 8) {
            // Muestra 1
            intermediate_buffer_2[i] = filter->b0_ * intermediate_buffer_1[i] + filter->z1_;
            filter->z1_ = filter->b1_ * intermediate_buffer_1[i] - filter->a1_ * intermediate_buffer_2[i] + filter->z2_;
            filter->z2_ = filter->b2_ * intermediate_buffer_1[i] - filter->a2_ * intermediate_buffer_2[i];

            // Muestra 2
            intermediate_buffer_2[i + 1] = filter->b0_ * intermediate_buffer_1[i + 1] + filter->z1_;
            filter->z1_ = filter->b1_ * intermediate_buffer_1[i + 1] - filter->a1_ * intermediate_buffer_2[i + 1] + filter->z2_;
            filter->z2_ = filter->b2_ * intermediate_buffer_1[i + 1] - filter->a2_ * intermediate_buffer_2[i + 1];

            // Muestra 3
            intermediate_buffer_2[i + 2] = filter->b0_ * intermediate_buffer_1[i + 2] + filter->z1_;
            filter->z1_ = filter->b1_ * intermediate_buffer_1[i + 2] - filter->a1_ * intermediate_buffer_2[i + 2] + filter->z2_;
            filter->z2_ = filter->b2_ * intermediate_buffer_1[i + 2] - filter->a2_ * intermediate_buffer_2[i + 2];

            // Muestra 4
            intermediate_buffer_2[i + 3] = filter->b0_ * intermediate_buffer_1[i + 3] + filter->z1_;
            filter->z1_ = filter->b1_ * intermediate_buffer_1[i + 3] - filter->a1_ * intermediate_buffer_2[i + 3] + filter->z2_;
            filter->z2_ = filter->b2_ * intermediate_buffer_1[i + 3] - filter->a2_ * intermediate_buffer_2[i + 3];

            // Muestra 5
            intermediate_buffer_2[i + 4] = filter->b0_ * intermediate_buffer_1[i + 4] + filter->z1_;
            filter->z1_ = filter->b1_ * intermediate_buffer_1[i + 4] - filter->a1_ * intermediate_buffer_2[i + 4] + filter->z2_;
            filter->z2_ = filter->b2_ * intermediate_buffer_1[i + 4] - filter->a2_ * intermediate_buffer_2[i + 4];

            // Muestra 6
            intermediate_buffer_2[i + 5] = filter->b0_ * intermediate_buffer_1[i + 5] + filter->z1_;
            filter->z1_ = filter->b1_ * intermediate_buffer_1[i + 5] - filter->a1_ * intermediate_buffer_2[i + 5] + filter->z2_;
            filter->z2_ = filter->b2_ * intermediate_buffer_1[i + 5] - filter->a2_ * intermediate_buffer_2[i + 5];

            // Muestra 7
            intermediate_buffer_2[i + 6] = filter->b0_ * intermediate_buffer_1[i + 6] + filter->z1_;
            filter->z1_ = filter->b1_ * intermediate_buffer_1[i + 6] - filter->a1_ * intermediate_buffer_2[i + 6] + filter->z2_;
            filter->z2_ = filter->b2_ * intermediate_buffer_1[i + 6] - filter->a2_ * intermediate_buffer_2[i + 6];

            // Muestra 8
            intermediate_buffer_2[i + 7] = filter->b0_ * intermediate_buffer_1[i + 7] + filter->z1_;
            filter->z1_ = filter->b1_ * intermediate_buffer_1[i + 7] - filter->a1_ * intermediate_buffer_2[i + 7] + filter->z2_;
            filter->z2_ = filter->b2_ * intermediate_buffer_1[i + 7] - filter->a2_ * intermediate_buffer_2[i + 7];
        }

        // Procesar cualquier muestra restante
        for (; i < nframes; ++i) {
            intermediate_buffer_2[i] = filter->b0_ * intermediate_buffer_1[i] + filter->z1_;
            filter->z1_ = filter->b1_ * intermediate_buffer_1[i] - filter->a1_ * intermediate_buffer_2[i] + filter->z2_;
            filter->z2_ = filter->b2_ * intermediate_buffer_1[i] - filter->a2_ * intermediate_buffer_2[i];
        }

        // Intercambiar los buffers para la siguiente etapa
        std::swap(intermediate_buffer_1, intermediate_buffer_2);
    }

    // Copiar el resultado final al buffer de salida
    memcpy(out, intermediate_buffer_1.data(), sizeof(sample_t) * nframes);

    return true;
}
