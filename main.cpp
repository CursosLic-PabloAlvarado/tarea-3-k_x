/** @file base.cpp
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

/** @file main.cpp
 *
 * @brief This is a C++ version of a simple jack client, to serve as
 * a simple framework to test basic digital signal processing algorithms,
 * applied to audio.
 */

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <vector>
#include <csignal>
#include <boost/program_options.hpp>
#include "waitkey.h"
#include "filter_client.h"
#include "parse_filter.h"

namespace po = boost::program_options;

/**
 * Handler for the SIGINT (interrupt signal)
 */
void signal_handler(int signal) {
    if (signal == SIGINT) {
        std::cout << "Ctrl-C caught, cleaning up and exiting" << std::endl;
        // Let RAII do the clean-up
        exit(EXIT_SUCCESS);
    }
}

int main(int argc, char *argv[]) {
    std::signal(SIGINT, signal_handler);

    try {
        static filter_client client;  // Instancia de cliente JACK

        typedef jack::client::sample_t sample_t;

        // Filter coefficients
        std::string filter_file;
        std::vector<std::vector<sample_t>> filter_coefs;

        // Parse options from the command line
        po::options_description desc("Allowed options");

        desc.add_options()
            ("help,h", "show usage information")
            ("files,f", po::value<std::vector<std::filesystem::path>>()->multitoken(), "List of audio files to be played")
            ("coeffs,c", po::value<std::string>(&filter_file), "File with filter coefficients (from GNU/Octave)");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << std::endl;
            return EXIT_SUCCESS;
        }

        if (vm.count("files")) {
            const std::vector<std::filesystem::path>& audio_files = vm["files"].as<std::vector<std::filesystem::path>>();

            for (const auto& f : audio_files) {
                bool ok = client.add_file(f);
                std::cout << "Adding file '" << f.c_str() << "' " << (ok ? "succeeded" : "failed") << std::endl;
            }
        }
        if (vm.count("coeffs")) {
            filter_coefs = parse_filter<sample_t>(filter_file);
        if (filter_coefs.empty() || filter_coefs[0].size() != 6) {
            throw std::runtime_error("Invalid filter coefficients");
         }

        std::cout << "Loaded " << filter_coefs.size() << " 2nd order filters from " << filter_file << std::endl;

        // Pasar los coeficientes a filter_client (pueden ser uno o varios conjuntos)
        client.setFilterCoefficients(filter_coefs);
        }



        if (client.init() != jack::client_state::Running) {
            throw std::runtime_error("Could not initialize the JACK client");
        }

        // Ciclo de procesamiento
        char set_mode = 'q';
        int key = -1;
        bool go_away = false;

        std::cout << "Press x key to exit" << std::endl;

        while (!go_away) {
            key = waitkey(100);
            if (key > 0) {
                switch (key) {
                    case 'x': {
                        go_away = true;
                        std::cout << "Finishing..." << std::endl;
                    } break;

                    case 'p': {
                        set_mode = 'p';  // Passthrough
                        client.setMode(set_mode);  // Actualiza el modo en el cliente
                    } break;

                    case 'c': {
                        set_mode = 'c';  // Filtro
                        client.setMode(set_mode);  // Actualiza el modo en el cliente
                        std::cout << "Filtrando audio" << std::endl;
                    } break;
                    
                    case 't': { //test para filtro de segundo orden con biquad
                        set_mode='t';
                        client.setMode(set_mode);
                        std::cout << "Probando clase biquad con filtro predeterminado" << std:: endl;
                    }break;

                    case 'r': {
                        if (vm.count("files")) {
                            const std::vector<std::filesystem::path>& audio_files = vm["files"].as<std::vector<std::filesystem::path>>();

                            for (const auto& f : audio_files) {
                                bool ok = client.add_file(f);
                                std::cout << "Re-adding file '" << f.c_str() << "' " << (ok ? "succeeded" : "failed") << std::endl;
                            }
                        }

                        std::cout << "Repeat playing files" << std::endl;
                    } break;

                    default: {
                        if (key > 32) {
                            std::cout << "Key " << char(key) << " pressed" << std::endl;
                        } else {
                            std::cout << "Key " << key << " pressed" << std::endl;
                        }
                        key = -1;
                    }
                }  // switch key
            }  // if (key > 0)
        }  // end while

        client.stop();
    } catch (std::exception& exc) {
        std::cout << argv[0] << ": Error: " << exc.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
