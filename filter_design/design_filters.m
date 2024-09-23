# Filter design

# para ellip, butter, cheby1 y cheby2 la frecuencia normalizada va [0 1]

#define the normalizing frequency
function resultado = normalizer(f)
  Fs = 48000;

  if isscalar(f)  % Verifica si la entrada es un escalar
    resultado = 2 * f / Fs;
  elseif ismatrix(f) && isequal(size(f), [1, 2])  % Verifica si la entrada es una matriz de 1x2
    resultado = 2 * f / Fs;
  else
    error('La entrada debe ser un escalar o una matriz de 1x2.');
  end
end


##########Definicion de variables###############
n=3; #orden de los filtros
Rp=1; #rizado pico a pico en la banda de paso en db
Rs=50; #atenuacion en db de la banda de rechazo

Lp=440; #frecuency lowpass
Hp=600; #frequency highpass
Bp=[220 1000]; #band pass frequency same frquencies for the stop band


################################################
########## Filtros pasa bajas

########## Filtros pasa bajas (Lowpass)
[b_ellip_lp, a_ellip_lp] = ellip(n, Rp, Rs, normalizer(Lp));
[b_butter_lp, a_butter_lp] = butter(n, normalizer(Lp));
[b_cheby1_lp, a_cheby1_lp] = cheby1(n, Rp, normalizer(Lp));
[b_cheby2_lp, a_cheby2_lp] = cheby2(n, Rs, normalizer(Lp));

########## Filtros pasa altas (Highpass)
[b_ellip_hp, a_ellip_hp] = ellip(n, Rp, Rs, normalizer(Hp), 'high');
[b_butter_hp, a_butter_hp] = butter(n, normalizer(Hp), 'high');
[b_cheby1_hp, a_cheby1_hp] = cheby1(n, Rp, normalizer(Hp), 'high');
[b_cheby2_hp, a_cheby2_hp] = cheby2(n, Rs, normalizer(Hp), 'high');

########## Filtros paso banda (Bandpass)
[b_ellip_bp, a_ellip_bp] = ellip(n, Rp, Rs, normalizer(Bp));
[b_butter_bp, a_butter_bp] = butter(n, normalizer(Bp));
[b_cheby1_bp, a_cheby1_bp] = cheby1(n, Rp, normalizer(Bp));
[b_cheby2_bp, a_cheby2_bp] = cheby2(n, Rs, normalizer(Bp));

########## Filtros rechaza banda (Bandstop)
[b_ellip_bs, a_ellip_bs] = ellip(n, Rp, Rs, normalizer(Bp), 'stop');
[b_butter_bs, a_butter_bs] = butter(n, normalizer(Bp), 'stop');
[b_cheby1_bs, a_cheby1_bs] = cheby1(n, Rp, normalizer(Bp), 'stop');
[b_cheby2_bs, a_cheby2_bs] = cheby2(n, Rs, normalizer(Bp), 'stop');
