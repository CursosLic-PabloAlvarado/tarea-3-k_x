
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

// Procesar el bloque de datos usando punteros directos para evitar memcpy y swap
bool cascade::process(jack_nframes_t nframes, const sample_t* in, sample_t* out) {
    const sample_t* input_ptr = in;
    sample_t* output_ptr = out;
    
    // Si solo hay un filtro, procesamos directamente de 'in' a 'out'
    if (filters_.size() == 1) {
        filters_[0]->process(nframes, input_ptr, output_ptr);
        return true;
    }

    // Si hay más de un filtro, usamos un buffer intermedio
    std::vector<sample_t> intermediate_buffer(nframes);

    // Procesar la primera etapa, escribir en el buffer intermedio
    filters_[0]->process(nframes, input_ptr, intermediate_buffer.data());

    // Si solo hay dos filtros, procesamos de buffer intermedio a 'out'
    if (filters_.size() == 2) {
        filters_[1]->process(nframes, intermediate_buffer.data(), output_ptr);
        return true;
    }

    // Si hay tres filtros, procesamos con dos pasos intermedios
    filters_[1]->process(nframes, intermediate_buffer.data(), output_ptr);  // Filtro 2
    filters_[2]->process(nframes, output_ptr, intermediate_buffer.data());  // Filtro 3

    // Copiamos el resultado final a 'out'
    std::memcpy(out, intermediate_buffer.data(), sizeof(sample_t) * nframes);
    return true;
}



// // Procesar el bloque de datos utilizando loop unrolling (8 muestras por iteración)
// bool cascade::process(jack_nframes_t nframes, const sample_t* in, sample_t* out) {
//    const sample_t *inptr=in;
//    sample_t *outptr=out;
//    const sample_t *const end_ptr=in+nframes;

//     // Copiar la entrada al buffer intermedio
//     while (inptr + 8 <= end_ptr) {
//         for (size_t i = 0; i < 8; ++i) {
//             sample_t sample = *inptr++;

//             // Aplicamos cada filtro biquad en cascada
//             for (auto& filter : filters_) {
//                 sample = filter->processSample(sample);
//             }

//             *outptr++ = sample;
//         }
//     }

//     // Procesamos las muestras restantes
//     while (inptr != end_ptr) {
//         sample_t sample = *inptr++;

//         // Aplicamos cada filtro biquad en cascada
//         for (auto& filter : filters_) {
//             sample = filter->processSample(sample);
//         }

//         *outptr++ = sample;
//     }

//     return true;
// }


