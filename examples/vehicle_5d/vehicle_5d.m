syms x1 x2 x3 x4 x5 u1 u2 r 

r=0.2;
a=1;
d1=1;
d2=1;
b11=1;
b22=1;
b12=0;
b21=0;
b=1;


a11=0.5*r*x4*(cos(x3)+a*sin(x3));
a12=0.5*r*x4*(cos(x3)-a*sin(x3));
a21=0.5*r*x4*(sin(x3)-a*cos(x3));
a22=0.5*r*x4*(sin(x3)+a*cos(x3));

dtdx1=x4*a11+x5*a12;
dtdx2=x4*a21+x5*a22;
dtdx3=r/(2*b) *(x5-x4);
dtdx4=-d1*x4+b11*u1+b12*u2;
dtdx5=-d2*x5+b21*u1+b22*u2;

l_13=diff(dtdx1,x3)
l_14=diff(dtdx1,x4)
l_15=diff(dtdx1,x5)
l_23=diff(dtdx2,x3)
l_24=diff(dtdx2,x4)
l_25=diff(dtdx2,x5)
l_34=diff(dtdx3,x4)
l_35=diff(dtdx3,x5)
l_44=diff(dtdx4,x4)
l_55=diff(dtdx5,x5)

 
% l_13 =(x4^2*(cos(x3) - sin(x3)))/10 - (x4*x5*(cos(x3) + sin(x3)))/10
%  
%  
% l_14 =(x5*(cos(x3) - sin(x3)))/10 + (x4*(cos(x3) + sin(x3)))/5
%  
%  
% l_15 =(x4*(cos(x3) - sin(x3)))/10
%  
%  
% l_23 =(x4^2*(cos(x3) + sin(x3)))/10 + (x4*x5*(cos(x3) - sin(x3)))/10
%  
%  
% l_24 =(x5*(cos(x3) + sin(x3)))/10 - (x4*(cos(x3) - sin(x3)))/5
%  
%  
% l_25 =(x4*(cos(x3) + sin(x3)))/10
 

ub=[10,10,pi,10,10];
lb=[0,0,-pi,0,0];
initial=[0,0,-pi,0,0];
[x1,mf]=fmincon(@func,initial,[],[],[],[],lb,ub);
max=-mf

x1


function f=func(x)
f=-((x(5)*(cos(x(3)) + sin(x(3))))/10 - (x(4)*(cos(x(3)) - sin(x(3))))/5);
end


