close all;
clear;
## coeficientes del filtro
b0=0.2929
b1=0.5858
b2=0.2929

a0=1
a1=0
a2=0.1716


###Valores de la matriz de coeficientes:
a = -a1*b0 + b1;
b = a1^2 *b0-a1*b1-a2*b0+b2; #x[n] en y[n+2]
c = -a1*b0+b1; #x[n+1] en y[n+2]
d = a1^2-a2; #z1 en y[n+2]

e = -a1^3*b0+a1^2*b1+2*a1*a2*b0-a1*b2-a2*b1; #x[n] en y[n+3]
f = a1^2*b0-a1*b1-a2*b0+b2; #x[n+1]
h = -a1*b0+b1; #x[n+2]
i = -a1^3+2*a1*a2; #z1
j = a1^2-a2; # z2

##filtro implementado de forma vectorial
C=[b0 0 0 0 1 0;
  a b0 0 0 -a1 1;
  b c b0 0 d -a1;
  e f h b0 i j];

z1=0;
z2=0;

x=[1 1 1 0 zeros(1,36)];


y=[];
n=1;

while n <=40
  bv=[x(n:n+3) z1 z2].';
  y=[y ;C*bv];

  %Estados siguientes
  z1=b1*x(n+3)-a1*y(n+3)+b2*x(n+2)-a2*y(n+2);
  z2=b2*x(n+3)-a2*y(n+3);



  n=n+4;


end


stem(y);

##filtro normal
w1=0;
w2=0;
tic
x = [x zeros(1, 40 - length(x))];

for n = 1:40  % Comienza en n=2 para evitar ¡ndice negativo en n-1

    % Implementaci¢n de las ecuaciones
    w(n) = b0 * x(n) + w1;               % C lculo de y[n]
    w1 = b1 * x(n) - a1 * w(n) + w2;  % C lculo de w1[n]
    w2 = b2 * x(n) - a2 * w(n);              % C lculo de w2[n]

end

w=w'
x=x';
figure;
stem(w)


