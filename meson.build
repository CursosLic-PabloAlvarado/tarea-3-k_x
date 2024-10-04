project('tarea3', 'cpp',
        default_options : ['cpp_std=c++20'],
        version : '0.0.2')

# Dependencias
jack_dep = dependency('jack')
sndfile_dep = dependency('sndfile')
boost_dep = dependency('boost', modules : ['program_options', 'system'])

# Incluir la carpeta 'externo' directamente
xsimd_inc = include_directories('externo')  # Apunta a 'externo', ya que 'xsimd.hpp' está en 'externo/xsimd/'

all_deps = [jack_dep, sndfile_dep, boost_dep]
sources = files('main.cpp', 'jack_client.cpp', 'filter_client.cpp',
                'sndfile_thread.cpp', 'waitkey.cpp', 'biquad.cpp', 'biquad.h', 'cascade.h', 'cascade.cpp')

# Añadir la opción -mavx para habilitar las instrucciones AVX
executable('tarea3', 
           sources, 
           dependencies: all_deps, 
           include_directories: [xsimd_inc], 
           cpp_args: ['-std=c++20', '-mavx'])  # Habilitar AVX

google_benchmark_dep = dependency('benchmark', required: true)
jack_dep = dependency('jack')
sndfile_dep = dependency('sndfile')
xsimd_inc = include_directories('externo')  # Apunta a 'externo', ya que 'xsimd.hpp' está en 'externo/xsimd/'

# Añadir la opción -mavx para el benchmark también
benchmark_tarea3 = executable('benchmark_tarea3',                                 
                               files('benchmark.cpp', 'biquad.cpp', 'cascade.cpp', 'jack_client.cpp','sndfile_thread.cpp'),                
                               dependencies: [boost_dep, google_benchmark_dep, jack_dep, sndfile_dep], 
                               include_directories: [xsimd_inc],     
                               cpp_args: ['-std=c++20', '-mavx'],  # Habilitar AVX                          
                               build_by_default: false  
)
