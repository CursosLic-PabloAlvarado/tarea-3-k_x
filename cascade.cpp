#include "cascade.h"
#include <cstring>  // Para memcpy si es necesario

// Constructor
cascade::cascade(const std::vector<std::vector<sample_t>>& filter_coeffs) {
    for (const auto& coeffs : filter_coeffs) {
        auto filter = std::make_unique<biquad>();  // Crear un nuevo biquad
        filter->setCoefficients(coeffs);           // Inicializar los coeficientes
        filters_.push_back(std::move(filter));     // Guardar el filtro en la cascada
    }
}

// Procesar el bloque de datos sin copias innecesarias
bool cascade::process(jack_nframes_t nframes, const sample_t* in, sample_t* out) {
    const sample_t* current_input = in;    // Comienza con el buffer de entrada
    sample_t* current_output = out;        // Buffer de salida final

    // Si solo tenemos un filtro, procesar directamente en el buffer de salida
    if (filters_.size() == 1) {
        filters_[0]->process(nframes, current_input, current_output);
        return true;
    }

    // Buffer intermedio para almacenar los resultados entre los filtros
    std::vector<sample_t> intermediate_buffer(nframes);

    // Procesar el primer filtro y escribir en el buffer intermedio
    filters_[0]->process(nframes, current_input, intermediate_buffer.data());

    // Alternar buffers entre el intermedio y el buffer final
    for (size_t i = 1; i < filters_.size() - 1; ++i) {
        filters_[i]->process(nframes, intermediate_buffer.data(), intermediate_buffer.data());
    }

    // Procesar el último filtro directamente hacia el buffer de salida
    filters_.back()->process(nframes, intermediate_buffer.data(), current_output);

    return true;  // Procesamiento exitoso
}

