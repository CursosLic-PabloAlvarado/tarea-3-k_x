function listen_filter(filter_filename, filename_sound, fs=48000)
    % Cargar la matriz SOS desde el archivo
    Data = load(filter_filename, "SOS"); % Cargar solo la matriz SOS

    % Recuperar la matriz SOS
    SOS = Data.SOS;

    % Convertir la matriz SOS a los coeficientes del filtro b y a
    [b, a] = sos2tf(SOS);  % Convierte la matriz SOS a coeficientes b (numerador) y a (denominador)

    % Cargar el archivo de audio .wav
    [audio_data, fs_audio] = audioread(filename_sound);

    % Verificar si la frecuencia de muestreo del archivo .wav es distinta a la frecuencia deseada
    if fs_audio ~= fs
        fprintf('La frecuencia de muestreo del archivo es distinta. Resampleando de %d Hz a %d Hz...\n', fs_wav, fs_desired);
        audio_data = resample(audio_data, fs, fs_audio); % Reescalar la frecuencia de muestreo
    else
        fprintf('La frecuencia de muestreo coincide: %d Hz.\n', fs);
    end

    % Aplicar el filtro al audio
    filtered_audio = filter(b, a, audio_data);

    % Reproducir el audio filtrado
    fprintf('Reproduciendo el audio filtrado...\n');
    sound(filtered_audio, fs);

    % Guardar el archivo de audio filtrado si se desea
    audiowrite(strcat('filtered_', filename_sound), filtered_audio, fs);

    fprintf('Archivo filtrado guardado como: filtered_%s\n', filename_sound);
end

