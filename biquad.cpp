#include "biquad.h"
#include <cstring>

biquad:: biquad() {}
biquad::~biquad() {}

bool biquad::process(jack_nframes_t nframes, const sample_t* const in, sample_t* out) {
    const sample_t *inptr = in;
    const sample_t *const end_ptr = in + nframes;
    sample_t *outptr = out;

    // Procesar bloque de datos de forma ineficiente
    while (inptr != end_ptr) {
        *outptr = b0_ * (*inptr) + z1_;
        z1_ = b1_ * (*inptr) - a1_ * (*outptr) + z2_;
        z2_ = b2_ * (*inptr) - a2_ * (*outptr);

        ++inptr;
        ++outptr;
    }
}

void biquad::setCoefficients(const std::vector<sample_t>& coeffs) {
    // Comprobamos que el tamaño de los coeficientes es correcto
    //assert(coeffs.size() == 6u); //borrar al final de verificar

    // Inicializamos los coeficientes
    b0_ = coeffs[0];
    b1_ = coeffs[1];
    b2_ = coeffs[2];

    // Comprobamos que el coeficiente a0 es aproximadamente 1.0
    
    a1_ = coeffs[4];
    a2_ = coeffs[5];

    // Reiniciamos el estado del filtro
    z1_ = 0.0f;
    z2_ = 0.0f;
}
