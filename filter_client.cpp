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

filter_client::filter_client() : jack::client(), set_mode_('p') {  // Modo por defecto es passthrough
}

filter_client::~filter_client() {
}

void filter_client::setMode(char mode) {
    set_mode_ = mode;
}

void filter_client::setFilterCoefficients(const std::vector<sample_t>& coeffs) {
    filter_.setCoefficients(coeffs);  // Establece los coeficientes en la instancia de biquad
}

bool filter_client::process(jack_nframes_t nframes,
                            const sample_t *const in,
                            sample_t *const out) {
    switch (set_mode_) {
        case 'p':  // Passthrough: copia la entrada a la salida sin modificaciones
            std::memcpy(out, in, sizeof(sample_t) * nframes);
            break;

        case 'f':  // Filtrado: usa el biquad para procesar las muestras
            filter_.process(nframes, in, out);  // Llama al método process del filtro biquad
            break;

        default:  // Modo desconocido, por seguridad, hacemos passthrough
            set_mode_='p';
            //std::memcpy(out, in, sizeof(sample_t) * nframes);
            break;
    }

    return true;  // Retorna true para indicar que el procesamiento fue exitoso
}

