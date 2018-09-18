
function apartment
clear set
close all


%% simulation

% target set
lb=[9 0];
ub=lb+0.5;
% initial state
x0=[0.6 0.6 0];


% load controller from file
%controller=StaticController('controller');

% simulate closed loop system
% y=x0;
% v=[];
% loop=3000;
% 
% % tau
% tau=0.3;
% 
% while(loop>0)
%     loop=loop-1;
%   
%   if (lb(1) <= y(end,1) & y(end,1) <= ub(1) &&...
%       lb(2) <= y(end,2) & y(end,2) <= ub(2))
%     break;
%   end 
% 
%   u=controller.control(y(end,:));
%   v=[v; u];
% 
%   [t x]=ode45(@unicycle_ode,[0 tau], y(end,:), odeset('abstol',1e-12,'reltol',1e-12),u);
%   
%   y=[y; x(end,:)];
% 
% end

%% plot the vehicle domain
% colors
colors=get(groot,'DefaultAxesColorOrder');

% load the symbolic set containig obstacles
%obs=GridPoints('obstacles');
%obs=unique(obs(:,[1 2]),'rows');
%plot(obs(:,1),obs(:,2),'.');
%hold on

target=GridPoints('target');
target=unique(target(:,[1 2]),'rows');
plot(target(:,1),target(:,2),'.','color',colors(2,:));
hold on


re_lazy=GridPoints('recomputation_lazy');
re_lazy=unique(re_lazy(:,[1,2]),'rows');
plot(re_lazy(:,1),re_lazy(:,2),'.','color',colors(2,:));

hold on



% plot the real obstacles and target set
plot_domain

% plot initial state  and trajectory
%plot(y(:,1),y(:,2),'k.-')
%plot(y(1,1),y(1,2),'.','color',colors(5,:),'markersize',20)

box on
axis([0 10 0 6])

%set(gcf,'paperunits','centimeters','paperposition',[0 0 16 10],'papersize',[16 6])

end

function dxdt = unicycle_ode(t,x,u)

  dxdt = zeros(3,1);
  c=atan(tan(u(2))/2);

  dxdt(1)=u(1)*cos(c+x(3))/cos(c);
  dxdt(2)=u(1)*sin(c+x(3))/cos(c);
  dxdt(3)=u(1)*tan(u(2));

end

function plot_domain

colors=get(groot,'DefaultAxesColorOrder');

%disturbance_type w2_lb={{7.5,0}};
%  disturbance_type w2_ub={{10,2}};


alpha=0.2;

v=[4.4 1;7 1; 4.4 3.3; 7 3.3];
patch('vertices',v,'faces',[1 2 4 3],'facea', alpha, 'edgec',colors(2,:));


v=[9 0; 9.5  0; 9 0.5; 9.5 .5];
patch('vertices',v,'faces',[1 2 4 3],'facea',alpha,'facec',colors(2,:),'edgec',colors(2,:));


v=[4.0     0  ;4.2  0 ; 4.0  2 ; 4.2  2  ];
patch('vertices',v,'faces',[1 2 4 3],'facea',alpha,'facec',colors(1,:),'edgec',colors(1,:));
v=[4.7     1.8;7.5  1.8   ; 4.7   2.0    ; 7.5 2.0   ];
patch('vertices',v,'faces',[1 2 4 3],'facea',alpha,'facec',colors(1,:),'edgec',colors(1,:));
v=[4.0 3.4; 4.2 3.4; 4.0 6; 4.2 6];
patch('vertices',v,'faces',[1 2 4 3],'facea',alpha,'facec',colors(1,:),'edgec',colors(1,:));
v=[4.7 3.4; 7.5 3.4; 4.7 3.6; 7.5 3.6];
patch('vertices',v,'faces',[1 2 4 3],'facea',alpha,'facec',colors(1,:),'edgec',colors(1,:));
v=[7.3   0  ;7.5  0   ; 7.3   2.0   ; 7.5 2.0  ];
patch('vertices',v,'faces',[1 2 4 3],'facea',alpha,'facec',colors(1,:),'edgec',colors(1,:));
v=[7.3   3.4  ;7.5  3.4   ; 7.3   6.0    ; 7.5  6.0   ];
patch('vertices',v,'faces',[1 2 4 3],'facea',alpha,'facec',colors(1,:),'edgec',colors(1,:));
v=[8.3   1.8  ;10.0  1.8   ;8.3   2.0   ; 10.0  2.0  ];
patch('vertices',v,'faces',[1 2 4 3],'facea',alpha,'facec',colors(1,:),'edgec',colors(1,:));
v=[8.3   3.4  ;10.0    3.4   ; 8.3   3.6    ; 10.0   3.6   ];
patch('vertices',v,'faces',[1 2 4 3],'facea',alpha,'facec',colors(1,:),'edgec',colors(1,:));


end
