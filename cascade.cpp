#include "cascade.h"
#include <cstring>  // Para memcpy

// Constructor
cascade::cascade(const std::vector<std::vector<sample_t>>& filter_coeffs) {
    // Crear e inicializar cada biquad con los coeficientes correspondientes
    for (const auto& coeffs : filter_coeffs) {
        biquad filter;
        filter.setCoefficients(coeffs);
        filters_.push_back(filter);
    }
}

// Procesar el bloque de datos
bool cascade::process(jack_nframes_t nframes, const sample_t* in, sample_t* out) {
    // Buffer intermedio para almacenar los datos procesados por cada filtro
    std::vector<sample_t> intermediate_buffer(nframes);

    // Copiar la entrada al buffer intermedio para el primer filtro
    memcpy(intermediate_buffer.data(), in, sizeof(sample_t) * nframes);

    // Procesar a través de cada biquad
    for (auto& filter : filters_) {
        // El filtro procesa los datos en el buffer intermedio y sobrescribe el mismo buffer
        filter.process(nframes, intermediate_buffer.data(), intermediate_buffer.data());
    }

    // Copiar el resultado final al buffer de salida
    memcpy(out, intermediate_buffer.data(), sizeof(sample_t) * nframes);

    return true;  // Indica que el proceso fue exitoso
}
