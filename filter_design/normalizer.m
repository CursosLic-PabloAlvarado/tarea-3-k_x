% Archivo: normalizer.m
function resultado = normalizer(f)
  Fs = 48000;  % Frecuencia de muestreo

  if isscalar(f)  % Verifica si es un valor único
    resultado = 2 * f / Fs;
  elseif ismatrix(f) && isequal(size(f), [1, 2])  % Verifica si es un vector de 1x2
    resultado = 2 * f / Fs;
  else
    error('La entrada debe ser un escalar o un vector de 1x2.');
  end
end
