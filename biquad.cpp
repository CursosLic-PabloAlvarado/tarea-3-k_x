
#include "biquad.h"
#include <cstring>
#include <iostream>

// Constructor por defecto
biquad::biquad() : b0_(0.0), b1_(0.0), b2_(0.0), a1_(0.0), a2_(0.0), z1_(0.0), z2_(0.0), set_mode_('p') {
}

// Destructor
biquad::~biquad() {
}

// Método para procesar el bloque de datos con loop unrolling
bool biquad::process(jack_nframes_t nframes, const sample_t* const in, sample_t* out) {
    size_t i = 0;

    // Desenrollar el bucle en bloques de 4 muestras por iteración
    for (; i + 3 < nframes; i += 4) {
        // Primera muestra
        out[i] = b0_ * in[i] + z1_;
        z1_ = b1_ * in[i] - a1_ * out[i] + z2_;
        z2_ = b2_ * in[i] - a2_ * out[i];

        // Segunda muestra
        out[i + 1] = b0_ * in[i + 1] + z1_;
        z1_ = b1_ * in[i + 1] - a1_ * out[i + 1] + z2_;
        z2_ = b2_ * in[i + 1] - a2_ * out[i + 1];

        // Tercera muestra
        out[i + 2] = b0_ * in[i + 2] + z1_;
        z1_ = b1_ * in[i + 2] - a1_ * out[i + 2] + z2_;
        z2_ = b2_ * in[i + 2] - a2_ * out[i + 2];

        // Cuarta muestra
        out[i + 3] = b0_ * in[i + 3] + z1_;
        z1_ = b1_ * in[i + 3] - a1_ * out[i + 3] + z2_;
        z2_ = b2_ * in[i + 3] - a2_ * out[i + 3];
    }

    // Procesar cualquier muestra restante (esto no debería ocurrir ya que nframes es 1024)
    for (; i < nframes; ++i) {
        out[i] = b0_ * in[i] + z1_;
        z1_ = b1_ * in[i] - a1_ * out[i] + z2_;
        z2_ = b2_ * in[i] - a2_ * out[i];
    }

    return true;
}

// Método para inicializar los coeficientes
void biquad::setCoefficients(const std::vector<sample_t>& coeffs) {
    b0_ = coeffs[0];
    b1_ = coeffs[1];
    b2_ = coeffs[2];
    a1_ = coeffs[4];
    a2_ = coeffs[5];

    // Reiniciamos el estado del filtro
    z1_ = 0.0f;
    z2_ = 0.0f;
}
