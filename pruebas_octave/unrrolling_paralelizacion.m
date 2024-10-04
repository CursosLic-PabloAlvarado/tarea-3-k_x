pkg load symbolic

syms b0 b1 b2 a0 a1 a2;  # coeficientes del filtro

syms x_n x_n1 x_n2 x_n3; # valores de entrada en diferentes tiempos (n, n+1, n+2, n+3)
syms y_n y_n1 y_n2 y_n3;       # valores de salida en diferentes tiempos (n, n+1, n+2, n+3)
syms z1 z2;               # iniciales del filtro

# Ecuación de salida y[n]
y_n = b0 * x_n + z1;

# Estados intermedios
w1 = b1 * x_n - a1 * y_n + z2;


# Ecuacion de salida y[n+1]
y_n1 = b0 * x_n1 + w1;

# Estado intermedios
w2 = b2 * x_n - a2 * y_n;

#Estados intermedios

w1=b1*x_n1-a1*y_n1+w2;


#Ecuacion de salida y[n+2]
y_n2=b0*x_n2+w1;

# Estado intermedios
w2=b2*x_n1-a2*y_n1;

#Estados intermedios

w1=b1*x_n2-a1*y_n2+w2;
#w2=b2*x_n2-a2*y_n2;

#Ecuacion de salida y[n+3]

y_n3=b0*x_n3+w1;

y_n

y_n1 = collect(expand(y_n1),x_n);
y_n1=collect(y_n1,x_n1)

y_n2 = collect(expand(y_n2),x_n);
y_n2 = collect(y_n2,x_n1);
y_n2 = collect(y_n2,x_n2);
y_n2 = collect(y_n2,z1)

y_n3 = collect(expand(y_n3),x_n);
y_n3 = collect(y_n3,x_n1);
y_n3 = collect(y_n3,x_n2);
y_n3 = collect(y_n3,x_n3);
y_n3 = collect(y_n3,z1);
y_n3 = collect(y_n3,z2)
