#ifndef BIQUAD_H
#define BIQUAD_H

#include <vector>
#include <jack_client.h>
#include <cassert>
#include <cmath>  // Para std::abs



class biquad : public jack::client {
public:
    // Coeficientes del filtro
    sample_t b0_, b1_, b2_, a1_, a2_;
    // Variables de estado
    sample_t z1_, z2_;
    char set_mode_;

//public:
    // Constructor por defecto
    biquad();

    // Destructor
    ~biquad();

    // Método para establecer el modo de operación
    void setMode(char mode);

    // Método para inicializar los coeficientes
    void setCoefficients(const std::vector<sample_t>& coeffs);


    inline bool process(jack_nframes_t nframes, const sample_t* in, sample_t* out){
    *out = *in*b0_+z1_;
    z1_=*in*b1_-a1_**out+z2_;
    z2_=*in*b2_-a2_**out;
    return true;
};

    inline sample_t processSample(sample_t input){
        sample_t output = input*b0_+z1_;
        z1_=input*b1_-a1_*output+z2_;
        z2_=input*b2_-a2_*output;
        return output;
};   

    
};

#endif  // BIQUAD_H
