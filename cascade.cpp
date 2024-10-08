
#include <cstring>  // Para memcpy
#include "cascade.h"

// Constructor
cascade::cascade(const std::vector<std::vector<sample_t>>& filter_coeffs) {
    for (const auto& coeffs : filter_coeffs) {
        auto filter = std::make_unique<biquad>();  // Crear un nuevo biquad
        filter->setCoefficients(coeffs);           // Inicializar los coeficientes
        filters_.push_back(std::move(filter));     // Mover el puntero único al vector
    }
}

bool cascade::process(jack_nframes_t nframes, const sample_t* in, sample_t* out) {
    const sample_t* inptr = in;
    sample_t* outptr = out;
    const sample_t* const end_ptr = in + nframes;

    // Procesar las muestras en bloques de 4
    while (inptr + 4 <= end_ptr) {
        // Si solo hay un filtro, procesamos directamente de 'in' a 'out'
        if (filters_.size() == 1) {
            filters_[0]->processSample(inptr, outptr);
        }
        // Si hay más de un filtro, usamos un buffer intermedio
        else if (filters_.size() == 2) {
            std::array<sample_t, 4> intermediate_buffer;

            // Procesar la primera etapa, escribir en el buffer intermedio
            filters_[0]->processSample(inptr, intermediate_buffer.data());

            // Procesar de buffer intermedio a 'out'
            filters_[1]->processSample(intermediate_buffer.data(), outptr);
        }
        // Si hay tres filtros, procesamos con dos pasos intermedios
        else if (filters_.size() == 3) {
            std::array<sample_t, 4> intermediate_buffer_1;
            std::array<sample_t, 4> intermediate_buffer_2;

            // Filtro 1 -> buffer intermedio 1
            filters_[0]->processSample(inptr, intermediate_buffer_1.data());

            // Filtro 2 -> buffer intermedio 2
            filters_[1]->processSample(intermediate_buffer_1.data(), intermediate_buffer_2.data());

            // Filtro 3 -> salida final
            filters_[2]->processSample(intermediate_buffer_2.data(), outptr);
        }

        // Avanzar punteros de entrada y salida en bloques de 4
        inptr += 4;
        outptr += 4;
    }

    return true;
}







