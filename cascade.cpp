#include "cascade.h"
#include <cstring>  // Para memcpy

// Constructor
cascade::cascade(const std::vector<std::vector<sample_t>>& filter_coeffs) {
    // Crear e inicializar cada biquad con los coeficientes correspondientes
    for (const auto& coeffs : filter_coeffs) {
        auto filter = std::make_unique<biquad>();  // Crear un nuevo biquad
        filter->setCoefficients(coeffs);           // Inicializar los coeficientes
        filters_.push_back(std::move(filter));     // Mover el puntero único al vector
    }
}

// Procesar el bloque de datos
bool cascade::process(jack_nframes_t nframes, const sample_t* in, sample_t* out) {
    // Buffer intermedio para almacenar los datos procesados por cada filtro
    std::vector<sample_t> intermediate_buffer_1(nframes); //buffer de processamiento
    std::vector<sample_t> intermediate_buffer_2(nframes); //buffer de salida
        //necesitamos un buffer intermedio:
        
        //<<buffer_in>> -->>> <<intermediate_buffer>> |cascade_filter| --->>  <<buffer_out>>

    // Copiar la entrada al buffer intermedio para el primer filtro
    memcpy(intermediate_buffer_1.data(), in, sizeof(sample_t) * nframes);

    // Procesar a través de cada biquad
    for (auto& filter : filters_) {
        // El filtro procesa los datos en el buffer intermedio y sobrescribe el mismo buffer
        filter->process(nframes, intermediate_buffer_1.data(), intermediate_buffer_2.data()); // Para cada uno de los filtros 'filter' dentro de filters_, usa el método process (definido en biquad).
        std::swap(intermediate_buffer_1,intermediate_buffer_2);
    }

    // Copiar el resultado final al buffer de salida
    memcpy(out, intermediate_buffer_1.data(), sizeof(sample_t) * nframes);  // Finalizamos copiando el buffer filtrado hacia la salida.

    return true; 
}

