function ploR2cartS(r,theta)

%input the four polar coordinates rectangle
%array r=[r1, r2, r3, r4]; %radius
%array theta=[theta1, theta2, theta3, theta4]; %theta

[x,y]=pol2cart(r,theta);

%small circle arc
syms x0 y0
[x0,y0]=solve((x0-x(1))^2+(y0-y(1))^2==r(1,1)^2,(x0-x(2))^2+(y0-y(2))^2==r(1,2)^2,x0,y0);
%plot arc
syms X Y
ezplot((X-x0(1))^2+(Y-y0(1))^2==r(1,1)^2,[min(x(1),x(2)),max(x(1),x(2)), ...
    min(y(1),y(2)),max(y(1),y(2))])
axis equal
figure
ezplot((X-x0(2))^2+(Y-y0(2))^2==r(1,2)^2,[min(x(1),x(2)),max(x(1),x(2)), ...
    min(y(1),y(2)),max(y(1),y(2))])
axis equal

%big out arc
syms x0 y0
[x0,y0]=solve((x0-x(3))^2+(y0-y(3))^2==r(1,3)^2,(x0-x(4))^2+(y0-y(4))^2==r(1,4)^2,x0,y0);
%plot arc
syms X Y
ezplot((X-x0(3))^2+(Y-y0(3))^2==r(1,3)^2,[min(x(3),x(3)),max(x(3),x(3)), ...
    min(y(3),y(4)),max(y(3),y(4))])
axis equal
figure
ezplot((X-x0(4))^2+(Y-y0(4))^2==r(1,4)^2,[min(x(3),x(3)),max(x(3),x(3)), ...
    min(y(3),y(4)),max(y(3),y(4))])
axis equal

%plot radius line between two endpoints
plot([x(1) x(3)], [y(1) y(3)]);
plot([x(2) x(4)], [y(2) y(4)]);

end

