#include "biquad.h"
#include <cstring>

biquad::biquad(){}
biquad::~biquad(){}

    void biquad::process(jack_nframes_t nframes, const sample_t* const in, sample_t* out){
    //..ciertos punteros a utilizar..
    const sample_t *inptr = in; //const acá significa que no modifica el contenido de lo que apunta
    const sample_t *const end_ptr = in+nframes; //puntero de contenido fijo
    sample_t *outptr = out;
    sample_t *const oend_ptr = out+nframes;
   
    while(inptr!=end_ptr){
        *outptr = b0_ * (*inptr) +  z1_;
        z1_= b1_*(*inptr)  -  a1_ * (*outptr)  + z_2;
        z2_= b2_*(*inptr)  -  a2_ * (*outptr);
        ++inptr; ++outptr;
        //...aca montamos nuestra ec de diferencias...
    }
    }

void setCoefficients(sample_t b0, sample_t b1, sample_t b2, sample_t a1, sample_t a2) {
    //esto no se puede realizar en tiempo de compilacion así como está acá (setcoefficients)
    b0_ = b0;
    b1_ = b1;
    b2_ = b2;
    a1_ = a1;
    a2_ = a2;
    }