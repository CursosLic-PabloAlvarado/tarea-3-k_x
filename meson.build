project('tarea3', 'cpp',
        default_options : ['cpp_std=c++20'],
        version : '0.0.2')

# Dependencias
jack_dep = dependency('jack')
sndfile_dep = dependency('sndfile')
boost_dep = dependency('boost', modules : ['program_options', 'system'])



all_deps = [jack_dep, sndfile_dep, boost_dep]
sources = files('main.cpp', 'jack_client.cpp', 'filter_client.cpp',
                'sndfile_thread.cpp', 'waitkey.cpp', 'biquad.cpp', 'biquad.h', 'cascade.h', 'cascade.cpp')

# Incluir las dependencias y la ruta corregida
executable('tarea3', sources, dependencies: all_deps)


google_benchmark_dep = dependency('benchmark', required: true)
jack_dep = dependency('jack')
sndfile_dep = dependency('sndfile')

benchmark_tarea3 = executable('benchmark_tarea3',                                 
                               files('benchmark.cpp', 'biquad.cpp', 'cascade.cpp', 'jack_client.cpp','sndfile_thread.cpp'),                
                               dependencies: [boost_dep, google_benchmark_dep, jack_dep, sndfile_dep],     
                               cpp_args: ['-std=c++20'],                          
                               build_by_default: false  
)
