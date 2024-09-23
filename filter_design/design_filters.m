
########## Definición de variables ###############
n = 3;  % Orden del filtro
Rp = 1;  % Rizado en banda de paso (dB)
Rs = 50; % Atenuación en banda de rechazo (dB)

Lp = 440;  % Filtro pasa bajas (frecuencia de corte)
Hp = 600;  % Filtro pasa altas (frecuencia de corte)
Bp = [220 1000];  % Filtro paso banda (frecuencias de corte)

################################################
########## Filtros pasa bajas (Lowpass)
[b_ellip_lp, a_ellip_lp] = ellip(n, Rp, Rs, normalizer(Lp));
SOS_ellip_lp = tf2sos(b_ellip_lp, a_ellip_lp);
save("ellip_lowpass.mat", "SOS_ellip_lp");

[b_butter_lp, a_butter_lp] = butter(n, normalizer(Lp));
SOS_butter_lp=tf2sos(b_butter_lp, a_butter_lp);
save("butter_lowpass.mat","SOS_butter_lp");

[b_cheby1_lp, a_cheby1_lp] = cheby1(n, Rp, normalizer(Lp));
SOS_cheby1_lp=tf2sos(b_cheby1_lp, a_cheby1_lp);
save("cheby1_lowpass.mat","SOS_cheby1_lp");

[b_cheby2_lp, a_cheby2_lp] = cheby2(n, Rs, normalizer(Lp));
SOS_cheby2_lp=tf2sos(b_cheby2_lp, a_cheby2_lp);
save("cheby2_lowpass.mat","SOS_cheby2_lp");

########## Filtros pasa altas (Highpass)
[b_ellip_hp, a_ellip_hp] = ellip(n, Rp, Rs, normalizer(Hp), 'high');
SOS_ellip_hp = tf2sos(b_ellip_hp, a_ellip_hp);
save("ellip_highpass.mat", "SOS_ellip_hp");

[b_butter_hp, a_butter_hp] = butter(n, normalizer(Hp), 'high');
SOS_butter_hp = tf2sos(b_butter_hp, a_butter_hp);
save("butter_highpass.mat", "SOS_butter_hp");

[b_cheby1_hp, a_cheby1_hp] = cheby1(n, Rp, normalizer(Hp), 'high');
SOS_cheby1_hp = tf2sos(b_cheby1_hp, a_cheby1_hp);
save("cheby1_highpass.mat", "SOS_cheby1_hp");

[b_cheby2_hp, a_cheby2_hp] = cheby2(n, Rs, normalizer(Hp), 'high');
SOS_cheby2_hp = tf2sos(b_cheby2_hp, a_cheby2_hp);
save("cheby2_highpass.mat", "SOS_cheby2_hp");


########## Filtros paso banda (Bandpass)
[b_ellip_bp, a_ellip_bp] = ellip(n, Rp, Rs, normalizer(Bp));
SOS_ellip_bp = tf2sos(b_ellip_bp, a_ellip_bp);
save("ellip_bandpass.mat", "SOS_ellip_bp");

[b_butter_bp, a_butter_bp] = butter(n, normalizer(Bp));
SOS_butter_bp = tf2sos(b_butter_bp, a_butter_bp);
save("butter_bandpass.mat", "SOS_butter_bp");

[b_cheby1_bp, a_cheby1_bp] = cheby1(n, Rp, normalizer(Bp));
SOS_cheby1_bp = tf2sos(b_cheby1_bp, a_cheby1_bp);
save("cheby1_bandpass.mat", "SOS_cheby1_bp");

[b_cheby2_bp, a_cheby2_bp] = cheby2(n, Rs, normalizer(Bp));
SOS_cheby2_bp = tf2sos(b_cheby2_bp, a_cheby2_bp);
save("cheby2_bandpass.mat", "SOS_cheby2_bp");

########## Filtros rechaza banda (Bandstop)
[b_ellip_bs, a_ellip_bs] = ellip(n, Rp, Rs, normalizer(Bp), 'stop');
SOS_ellip_bs = tf2sos(b_ellip_bs, a_ellip_bs);
save("ellip_stopband.mat", "SOS_ellip_bs");

[b_butter_bs, a_butter_bs] = butter(n, normalizer(Bp), 'stop');
SOS_butter_bs = tf2sos(b_butter_bs, a_butter_bs);
save("butter_stopband.mat", "SOS_butter_bs");

[b_cheby1_bs, a_cheby1_bs] = cheby1(n, Rp, normalizer(Bp), 'stop');
SOS_cheby1_bs = tf2sos(b_cheby1_bs, a_cheby1_bs);
save("cheby1_stopband.mat", "SOS_cheby1_bs");

[b_cheby2_bs, a_cheby2_bs] = cheby2(n, Rs, normalizer(Bp), 'stop');
SOS_cheby2_bs = tf2sos(b_cheby2_bs, a_cheby2_bs);
save("cheby2_stopband.mat", "SOS_cheby2_bs");