/**
 * filter_client.cpp
 *
 * Copyright (C) 2023-2024  Pablo Alvarado
 * EL5805 Procesamiento Digital de Señales
 * Escuela de Ingeniería Electrónica
 * Tecnológico de Costa Rica
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the authors nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "filter_client.h"
#include <cstring>

filter_client::filter_client() : jack::client(), set_mode_('p'), cascade_filter_(nullptr), biquad_filter_(nullptr) {
}

filter_client::~filter_client() {
    // Liberar memoria de cascade_filter_
    if (cascade_filter_ != nullptr) {
        delete cascade_filter_;
        cascade_filter_ = nullptr;
    }

    // Liberar memoria de biquad_filter_
    if (biquad_filter_ != nullptr) {
        delete biquad_filter_;
        biquad_filter_ = nullptr;
    }
}

// Establece el modo de funcionamiento: passthrough ('p'), filtrado en cascada ('c') o biquad test ('t')
void filter_client::setMode(char mode) {
    set_mode_ = mode;
}

// Establece los coeficientes de filtro (uno o varios filtros biquad)
void filter_client::setFilterCoefficients(const std::vector<std::vector<sample_t>>& coeffs) {
    // Si ya hay una cascada configurada, liberarla primero
    if (cascade_filter_ != nullptr) {
        delete cascade_filter_;
        cascade_filter_ = nullptr;
    }

    // Si ya hay un filtro biquad, liberarlo primero
    // if (biquad_filter_ != nullptr) {
    //     delete biquad_filter_;
    //     biquad_filter_ = nullptr;
    // }

    // Crear una nueva instancia de Cascade con los coeficientes
    cascade_filter_ = new cascade(coeffs);

    // Crear un filtro biquad para el modo de prueba con coeficientes predeterminados (puedes ajustarlos según tu necesidad)
    biquad_filter_ = new biquad;
    biquad_filter_->setCoefficients({0.2929, 0.5858, 0.2929,1.0, -0.0000, 0.1716});
}

// Procesa un bloque de muestras (override del cliente JACK)
bool filter_client::process(jack_nframes_t nframes, const sample_t* const in, sample_t* out) {
    switch (set_mode_) {
        case 'p':  // Passthrough: copiar los datos de entrada a la salida
            std::memcpy(out, in, sizeof(sample_t) * nframes);
            break;

        case 'c':  // Filtro: aplicar la cascada de filtros
            if (cascade_filter_ != nullptr) {
                cascade_filter_->process(nframes, in, out);  // Procesar con la cascada de filtros
            }
            break;

        case 't':  // Test de biquad con filtro predeterminado
            if (biquad_filter_ != nullptr) {
                const sample_t* inptr = in;
                const sample_t* const end_ptr = in + nframes;
                sample_t* outptr = out;

                while (inptr != end_ptr) {
                    // Procesar cada muestra individualmente con el biquad
                    sample_t sample = biquad_filter_->processSample(*inptr);
                    *outptr = sample;
                    ++inptr;
                    ++outptr;
                }
            }
            break;

        default:  // Si el modo es desconocido, usar passthrough como protección
            std::memcpy(out, in, sizeof(sample_t) * nframes);
            break;
    }

    return true;  // Indicar que el procesamiento fue exitoso
}
