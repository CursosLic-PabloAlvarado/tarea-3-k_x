function viewfreqresp(filename, fs=48000)
    % Cargar la matriz SOS desde el archivo
    Data = load(filename, "SOS");
    
    % Recuperar la matriz SOS
    SOS = Data.SOS;
    
    % Obtener los coeficientes del filtro a partir de SOS
    [b, a] = sos2tf(SOS);
    % Mostrar el diagrama de polos y ceros
    figure;
    zplane(b, a); %basta con usar esto
    title('Diagrama de Polos y Ceros');
    
    %trabjar en la funcion que muestra la magnitud, fase, etc... (no usar bode)
    
 
end
