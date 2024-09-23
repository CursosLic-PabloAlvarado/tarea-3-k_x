########## Definición de variables ###############
n = 3;  % Orden del filtro
Rp = 1;  % Rizado en banda de paso (dB)
Rs = 50; % Atenuación en banda de rechazo (dB)

Lp = 440;  % Filtro pasa bajas (frecuencia de corte)
Hp = 600;  % Filtro pasa altas (frecuencia de corte)
Bp = [220 1000];  % Filtro paso banda (frecuencias de corte)

% Función para guardar filtros
function savefilter(B, A, name)
    SOS = tf2sos(B, A);
    save(name, "SOS");
end

################################################
########## Filtros pasa bajas (Lowpass)
[b_ellip_lp, a_ellip_lp] = ellip(n, Rp, Rs, normalizer(Lp));
savefilter(b_ellip_lp, a_ellip_lp, "ellip_lowpass.mat");

[b_butter_lp, a_butter_lp] = butter(n, normalizer(Lp));
savefilter(b_butter_lp, a_butter_lp, "butter_lowpass.mat");

[b_cheby1_lp, a_cheby1_lp] = cheby1(n, Rp, normalizer(Lp));
savefilter(b_cheby1_lp, a_cheby1_lp, "cheby1_lowpass.mat");

[b_cheby2_lp, a_cheby2_lp] = cheby2(n, Rs, normalizer(Lp));
savefilter(b_cheby2_lp, a_cheby2_lp, "cheby2_lowpass.mat");

########## Filtros pasa altas (Highpass)
[b_ellip_hp, a_ellip_hp] = ellip(n, Rp, Rs, normalizer(Hp), 'high');
savefilter(b_ellip_hp, a_ellip_hp, "ellip_highpass.mat");

[b_butter_hp, a_butter_hp] = butter(n, normalizer(Hp), 'high');
savefilter(b_butter_hp, a_butter_hp, "butter_highpass.mat");

[b_cheby1_hp, a_cheby1_hp] = cheby1(n, Rp, normalizer(Hp), 'high');
savefilter(b_cheby1_hp, a_cheby1_hp, "cheby1_highpass.mat");

[b_cheby2_hp, a_cheby2_hp] = cheby2(n, Rs, normalizer(Hp), 'high');
savefilter(b_cheby2_hp, a_cheby2_hp, "cheby2_highpass.mat");

########## Filtros paso banda (Bandpass)
[b_ellip_bp, a_ellip_bp] = ellip(n, Rp, Rs, normalizer(Bp));
savefilter(b_ellip_bp, a_ellip_bp, "ellip_bandpass.mat");

[b_butter_bp, a_butter_bp] = butter(n, normalizer(Bp));
savefilter(b_butter_bp, a_butter_bp, "butter_bandpass.mat");

[b_cheby1_bp, a_cheby1_bp] = cheby1(n, Rp, normalizer(Bp));
savefilter(b_cheby1_bp, a_cheby1_bp, "cheby1_bandpass.mat");

[b_cheby2_bp, a_cheby2_bp] = cheby2(n, Rs, normalizer(Bp));
savefilter(b_cheby2_bp, a_cheby2_bp, "cheby2_bandpass.mat");

########## Filtros rechaza banda (Bandstop)
[b_ellip_bs, a_ellip_bs] = ellip(n, Rp, Rs, normalizer(Bp), 'stop');
savefilter(b_ellip_bs, a_ellip_bs, "ellip_stopband.mat");

[b_butter_bs, a_butter_bs] = butter(n, normalizer(Bp), 'stop');
savefilter(b_butter_bs, a_butter_bs, "butter_stopband.mat");

[b_cheby1_bs, a_cheby1_bs] = cheby1(n, Rp, normalizer(Bp), 'stop');
savefilter(b_cheby1_bs, a_cheby1_bs, "cheby1_stopband.mat");

[b_cheby2_bs, a_cheby2_bs] = cheby2(n, Rs, normalizer(Bp), 'stop');
savefilter(b_cheby2_bs, a_cheby2_bs, "cheby2_stopband.mat");
