#include "biquad.h"
#include <cstring>
#include <iostream>

// Constructor por defecto
biquad::biquad() : b0_(0.0), b1_(0.0), b2_(0.0), a1_(0.0), a2_(0.0), z1_(0.0), z2_(0.0), set_mode_('p') {
}

// Destructor
biquad::~biquad() {
}

// Constructor de copia
biquad::biquad(const biquad& other) {
    // Copiar los coeficientes y las variables de estado
    b0_ = other.b0_;
    b1_ = other.b1_;
    b2_ = other.b2_;
    a1_ = other.a1_;
    a2_ = other.a2_;
    z1_ = other.z1_;
    z2_ = other.z2_;
    set_mode_ = other.set_mode_;
}

// Operador de asignación por copia
biquad& biquad::operator=(const biquad& other) {
    if (this != &other) {
        // Evitar auto-asignación
        // Copiar los coeficientes y las variables de estado
        b0_ = other.b0_;
        b1_ = other.b1_;
        b2_ = other.b2_;
        a1_ = other.a1_;
        a2_ = other.a2_;
        z1_ = other.z1_;
        z2_ = other.z2_;
        set_mode_ = other.set_mode_;
    }
    return *this;
}

// Constructor de movimiento
biquad::biquad(biquad&& other) noexcept {
    // Mover los coeficientes y las variables de estado
    b0_ = other.b0_;
    b1_ = other.b1_;
    b2_ = other.b2_;
    a1_ = other.a1_;
    a2_ = other.a2_;
    z1_ = other.z1_;
    z2_ = other.z2_;
    set_mode_ = other.set_mode_;

    // Limpiar el objeto fuente
    other.b0_ = 0.0;
    other.b1_ = 0.0;
    other.b2_ = 0.0;
    other.a1_ = 0.0;
    other.a2_ = 0.0;
    other.z1_ = 0.0;
    other.z2_ = 0.0;
    other.set_mode_ = 'p';
}

// Operador de asignación por movimiento
biquad& biquad::operator=(biquad&& other) noexcept {
    if (this != &other) {
        // Mover los coeficientes y las variables de estado
        b0_ = other.b0_;
        b1_ = other.b1_;
        b2_ = other.b2_;
        a1_ = other.a1_;
        a2_ = other.a2_;
        z1_ = other.z1_;
        z2_ = other.z2_;
        set_mode_ = other.set_mode_;

        // Limpiar el objeto fuente
        other.b0_ = 0.0;
        other.b1_ = 0.0;
        other.b2_ = 0.0;
        other.a1_ = 0.0;
        other.a2_ = 0.0;
        other.z1_ = 0.0;
        other.z2_ = 0.0;
        other.set_mode_ = 'p';
    }
    return *this;
}

// Método para procesar el bloque de datos
bool biquad::process(jack_nframes_t nframes, const sample_t* const in, sample_t* out) {
    const sample_t *inptr = in;
    const sample_t *const end_ptr = in + nframes;
    sample_t *outptr = out;

    // Procesar bloque de datos
    while (inptr != end_ptr) {
        *outptr = b0_ * (*inptr) + z1_;
        z1_ = b1_ * (*inptr) - a1_ * (*outptr) + z2_;
        z2_ = b2_ * (*inptr) - a2_ * (*outptr);
        ++inptr;
        ++outptr;
    }
    return true;
}

// Método para inicializar los coeficientes
void biquad::setCoefficients(const std::vector<sample_t>& coeffs) {
    // Comprobamos que el tamaño de los coeficientes es correcto
    //assert(coeffs.size() == 6u); //borrar al final de verificar

    // Inicializamos los coeficientes
    b0_ = coeffs[0];
    b1_ = coeffs[1];
    b2_ = coeffs[2];

    a1_ = coeffs[4];
    a2_ = coeffs[5];

    // Reiniciamos el estado del filtro
    z1_ = 0.0f;
    z2_ = 0.0f;
}
