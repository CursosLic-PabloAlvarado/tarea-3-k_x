#ifndef BIQUAD_H
#define BIQUAD_H

#include <vector>
#include <jack/jack.h> 

class biquad {
private:
    //coeficientes:
    sample_t b0_, b1_, b2_, a1_, a2_;

    sample_t z1_, z2_;
public:
    //constructor, podemos en la siguiente linea inicializar los estados y los coeficientes en 0.
    biquad();
    ~biquad();
    void setCoefficients(sample_t b0, sample_t b1, sample_t b2, sample_t a1, sample_t a2) {
        z1_= 0;
        z2_ = 0;

    }


    void biquad::process(jack_nframes_t nframes, const sample_t* const in, sample_t* out){

        //posible inline... to be continue

    }

    //inline para un sola muestra


}; #endif 