#ifndef BIQUAD_H
#define BIQUAD_H

#include <vector>
#include <jack/jack.h> 

class biquad {
private:
    //coeficientes:
    sample_t sample_t b0_, b1_, b2_, a1_, a2_;

    sample_t z1_, z2_;
public:
    //constructor, podemos en la siguiente linea inicializar los estados y los coeficientes en 0.
    biquad();
    void setCoefficients(sample_t b0, sample_t b1, sample_t b2, sample_t a1, sample_t a2) {
    //esto no se puede realizar en tiempo de compilacion así como está acá (setcoefficients)
    b0_ = b0;
    b1_ = b1;
    b2_ = b2;
    a1_ = a1;
    a2_ = a2;
    }
    //metodo para reseatear?

    void biquad::process(jack_nframes_t nframes, const sample_t* const in, sample_t* out){
    //..ciertos punteros a utilizar..
    //implementar de forma optima, por ahora es solo una idea:
    const sample_t *inptr = in; //const acá significa que no modifica el contenido de lo que apunta
    const sample_t *const end_ptr = in+nframes; //puntero de contenido fijo
    sample_t *const oend_ptr = out+nframes;
    
    while(inptr!=end_ptr){

        //...aca montamos nuestra ec de diferencias...
        //usar forma transpuesta II por ejemplo
    }
    }


}; #endif 