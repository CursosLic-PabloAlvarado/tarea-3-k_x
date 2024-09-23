function viewfreqresp(filename, fs=48000)
    pkg load signal;
    close all;
    % Cargar la matriz SOS desde el archivo
    Data = load(filename, "SOS");

    % Recuperar la matriz SOS
    SOS = Data.SOS;

    % Obtener los coeficientes del filtro a partir de SOS
    [b, a] = sos2tf(SOS);
    % Mostrar el diagrama de polos y ceros
    figure;
    zplane(b, a); %basta con usar esto
    title(sprintf('Diagrama de Polos y Ceros para %s', strrep(filename, '_', '\_')));

    %trabjar en la funcion que muestra la magnitud, fase, etc... (no usar bode)
    f = logspace(0,5);
    z=e.^(i*2*pi*f/fs);
    n=polyval(b,z);
    d=polyval(a,z);
    ft = n./d;
    absolute=20*log10(abs(ft));
    argument=arg(ft)*180/pi;



    figure;
    semilogx(f,absolute);
    title(sprintf('Respuesta en magnitud para %s', strrep(filename, '_', '\_')));
    xlabel('Frecuencia [Hz]');
    ylabel('|H(F)| [dB]')
    axis('normal')
    xlim([1 fs/2]);
    ylim([-50 3]);
    grid on;

    figure;
    semilogx(f,argument);
    title(sprintf('Respuesta en fase para %s', strrep(filename, '_', '\_')));
    xlabel('Frecuencia [Hz]');
    ylabel('\angleH(F) [°]')
    axis('normal')
    xlim([1 fs/2]);
    ylim([-360 360]);
    grid on;
end
