/**
 * benchmark.cpp
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

#include <cstdlib>
#include <biquad.h>
#include <cascade.h>
#include <benchmark/benchmark.h>

/**
 * This benchmark uses Google Benchmark
 *
 * In GNU/Linux we install the library with
 * 
 * > sudo apt install libbenchmark-dev
 *
 */

static void BM_Biquad_Process(benchmark::State& state) {
  biquad dut;
  dut.setCoefficients(std::vector<float>{
      1, -1.9962282681026606, 1.0000000006886136,
      1,-1.9902746344470237,0.99111829673050256
    });
  int size = state.range(0);

  float input[size];
  float output[size];

  for (auto _ : state) {
    dut.process(size, input, output);
  }
  
  state.SetItemsProcessed(size);  // Optional: Report number of items processed
  state.SetComplexityN(size);     // Optional: Analyze time complexity
}

// Vary array size from 8 to 8192
BENCHMARK(BM_Biquad_Process)->RangeMultiplier(2)->Range(256, 8<<10);


static void BM_Cascade_Process(benchmark::State& state) {
  cascade cascade_filter(std::vector< std::vector<float> >{
      {0.88489099304085195,-1.7647259369167299,0.88489099279944872,
       1,-1.9447696737414277,0.96118976527688038},
      {1, -1.9962282681026606, 1.0000000006886136,
       1,-1.9902746344470237,0.99111829673050256},
      {1,-1.9975106328926273,0.99999999958419417,
       1,-1.8137023593689499,0.81712922359906082}
    });
  int size = state.range(0);

  float input[size];
  float output[size];

  for (auto _ : state) {
    cascade_filter.process(size, input, output);
  }
  
  state.SetItemsProcessed(size);  // Optional: Report number of items processed
  state.SetComplexityN(size);     // Optional: Analyze time complexity
}

// Vary array size from 8 to 8192
BENCHMARK(BM_Cascade_Process)->RangeMultiplier(2)->Range(256, 8<<10); 

BENCHMARK_MAIN(); 
