/**
 * passthrough_client.h
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

#ifndef FILTER_CLIENT_H
#define FILTER_CLIENT_H

#include "jack_client.h"
#include "cascade.h"  // Incluir la clase Cascade

class filter_client : public jack::client {
public:
    filter_client();
    ~filter_client();

    // Método para establecer el modo de funcionamiento (passthrough o filtrado)
    void setMode(char mode);

    // Método para establecer los coeficientes de filtro (ya sea uno o varios filtros biquad)
    void setFilterCoefficients(const std::vector<std::vector<sample_t>>& coeffs);

    // Procesa un bloque de muestras (override del cliente JACK)
    virtual bool process(jack_nframes_t nframes, const sample_t* const in, sample_t* out) override;

private:
    char set_mode_;  // 'p' para passthrough, 'f' para filtrado
    cascade* cascade_filter_;  // Puntero a cascada de filtros (uno o varios biquads)
    //aquí usamos un puntero a cascade para que cuando se llame a filter_client y no queramos filtrar no cree un objeto 
  
};

#endif  // FILTER_CLIENT_H

