#include "cascade.h"
#include <cstring>  // Para memcpy

// Constructor
cascade::cascade(const std::vector<std::vector<sample_t>>& filter_coeffs) {
    // Crear e inicializar cada biquad con los coeficientes correspondientes
    for (const auto& coeffs : filter_coeffs) {
        biquad filter;
        filter.setCoefficients(coeffs);  //para cada conjunto de parametros, se crea una instancia de biquad 'filter'
                                         // y llama al metodo setCoefficients definido en biquad.
        filters_.push_back(filter);  //filters_ es nuestro vector de biquad, entonces cada que crea uno nuevo lo añade al vector
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
        filter.process(nframes, intermediate_buffer.data(), intermediate_buffer.data()); //para cada uno de los filtros 'filter dentro de filters_' debe hacer uso del método process (definiod en biquad, por supuesto)
    }

    // Copiar el resultado final al buffer de salida
    memcpy(out, intermediate_buffer.data(), sizeof(sample_t) * nframes);  //finalizamos haciendo la copia del buffer filtrado hacia la salida

    return true; 
}
