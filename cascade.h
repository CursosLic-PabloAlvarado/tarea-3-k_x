#ifndef CASCADE_H
#define CASCADE_H
#include "biquad.h"
#include <vector>
#include <memory>  // Para std::unique_ptr
#include <jack_client.h>

class cascade : public jack::client {
public:
    // Constructor que recibe uno o varios conjuntos de coeficientes
    cascade(const std::vector<std::vector<sample_t>>& filter_coeffs);
    
    // Método para procesar un bloque de muestras
    bool process(jack_nframes_t nframes, const sample_t* in, sample_t* out);

private:
     // Vector que contiene punteros únicos a biquad 
     //esto reduce la necesidad de copiar los elementos biquad, en cambio así solo manejos los punteros
    std::vector<std::unique_ptr<biquad>> filters_;
};

#endif // CASCADE_H
