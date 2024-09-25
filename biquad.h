#ifndef BIQUAD_H
#define BIQUAD_H

#include <vector>
#include <jack/jack.h>
#include <cassert>
#include <cmath>  // Para std::abs

//typedef float sample_t; 

class biquad {
private:
    // Coeficientes del filtro
    sample_t b0_, b1_, b2_, a1_, a2_;
    // Variables de estado
    sample_t z1_, z2_;
    char set_mode_;

public:
     biquad();
    ~biquad();
    void setMode(char mode);

    // Método para inicializar los coeficientes
    void setCoefficients(const std::vector<sample_t>& coeffs);

    // Método para procesar un bloque de muestras
    void process(jack_nframes_t nframes, const sample_t* const in, sample_t* out);
};

#endif  // BIQUAD_H
