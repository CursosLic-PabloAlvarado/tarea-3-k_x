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
   const sample_t *inptr=in;
   sample_t *outptr=out;

   const sample_t *const end_ptr=in+nframes;
    // Copiar la entrada al buffer intermedio
    while (inptr  != end_ptr) {
            sample_t sample = *inptr++;

            // Aplicamos cada filtro biquad en cascada
            for (auto& filter : filters_) {
                sample=filter->processSample(sample);
            }

            *outptr++ = sample;

            sample = *inptr++;
            for (auto& filter : filters_) {
                sample=filter->processSample(sample);
            }
            *outptr++ = sample;

            sample = *inptr++;
            for (auto& filter : filters_) {
                sample=filter->processSample(sample);
            }
            *outptr++ = sample;

            sample = *inptr++;
            for (auto& filter : filters_) {
                sample=filter->processSample(sample);
            }
            *outptr++ = sample;
        }
    

    // Procesamos las muestras restantes

    return true;
}
