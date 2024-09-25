#include "cascade.h"
#include <cstring>  // Para memcpy

// Constructor
cascade::cascade(const std::vector<std::vector<sample_t>>& filter_coeffs) {
    // Crear e inicializar cada biquad con los coeficientes correspondientes
    for (const auto& coeffs : filter_coeffs) {
        biquad filter;
        filter.setCoefficients(coeffs);  //para cada conjunto de parámetros, se crea una instancia de biquad 'filter'
                                         // y llama al método setCoefficients definido en biquad.
        filters_.push_back(std::move(filter));  // Usamos std::move para mover el objeto en lugar de copiarlo
    }
}

// Procesar el bloque de datos
bool cascade::process(jack_nframes_t nframes, const sample_t* in, sample_t* out) {
    // Buffer intermedio para almacenar los datos procesados por cada filtro
    std::vector<sample_t> intermediate_buffer(nframes); 
        //necesitamos un buffer intermedio:
        
        //<<buffer_in>> -->>> <<intermediate_buffer>> |cascade_filter| --->>  <<buffer_out>>

    // Copiar la entrada al buffer intermedio para el primer filtro
    memcpy(intermediate_buffer.data(), in, sizeof(sample_t) * nframes);

    // Procesar a través de cada biquad
    for (auto& filter : filters_) {
        // El filtro procesa los datos en el buffer intermedio y sobrescribe el mismo buffer
        filter.process(nframes, intermediate_buffer.data(), intermediate_buffer.data()); // Para cada uno de los filtros 'filter' dentro de filters_, usa el método process (definido en biquad).
    }

    // Copiar el resultado final al buffer de salida
    memcpy(out, intermediate_buffer.data(), sizeof(sample_t) * nframes);  // Finalizamos copiando el buffer filtrado hacia la salida.

    return true; 
}

