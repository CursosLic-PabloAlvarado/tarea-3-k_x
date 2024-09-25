#ifndef BIQUAD_H
#define BIQUAD_H

#include <vector>
#include <jack_client.h>
#include <cassert>
#include <cmath>  // Para std::abs

class biquad : public jack::client {
private:
    // Coeficientes del filtro
    sample_t b0_, b1_, b2_, a1_, a2_;
    // Variables de estado
    sample_t z1_, z2_;
    char set_mode_;

public:
    // Constructor por defecto
    biquad();

    // Destructor
    ~biquad();

    // Constructor de copia
    biquad(const biquad& other);

    // Operador de asignación por copia
    biquad& operator=(const biquad& other);

    // Constructor de movimiento
    biquad(biquad&& other) noexcept;

    // Operador de asignación por movimiento
    biquad& operator=(biquad&& other) noexcept;

    // Método para establecer el modo de operación
    void setMode(char mode);

    // Método para inicializar los coeficientes
    void setCoefficients(const std::vector<sample_t>& coeffs);

    // Método para procesar un bloque de muestras
    bool process(jack_nframes_t nframes, const sample_t* const in, sample_t* out);
};

#endif  // BIQUAD_H
