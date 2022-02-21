%    R G B
% Y
% cb
% cr

Ggrey=[-5 0 5 10 15 20 25 30 35 40 45 50 55 60 65 70 75 80 85 90 95 100 105 -2 2 450/700*100 200/700*100 110/700*100]'./100;
rgbgrey=(Ggrey*[1 1 1])'
rgb=[[0 0 1];[0 1 0];[0 1 1];[1 0 0];[1 0 1];[1 1 0]]';
rgb=[rgb 0.75*rgb]


m=12;

ROMval=[];

%Calculated from method in RP177:

% disp('SMPTE 240M:')
% % RGB Reference Primaries from Smpte240M
% Xr = 0.630; Yr = 0.340;
% Xg = 0.310; Yg = 0.595;
% Xb = 0.155; Yb = 0.070;
% % Reference White primaries
% Xw = 0.3127; Yw = 0.3290;
% % Calculations
% Zr = 1 -(Xr+Yr);
% Zg = 1 -(Xg+Yg);
% Zb = 1 -(Xb+Yb);
% Zw = 1-(Xw+Yw);
% P=[[Xr Xg Xb];[Yr Yg Yb];[Zr Zg Zb]];
% W= [Xw/Yw; 1; Zw/Yw];
% P_inv = eye(3)/P;
% CrCgCb = P_inv*W;
% C= [[CrCgCb(1) 0 0];[0 CrCgCb(2) 0];[0 0 CrCgCb(3)]];
% NPM = P*C;
% % Luminance Equation
% My = [NPM(2) NPM(5) NPM(8)]
% %My=  [0.212 0.701 0.087]
% Mbmy=[0 0 1]-My;
% Kb=0.5/sum(Mbmy(find(Mbmy>0)));
% Mbmy=Kb*Mbmy;
% Mrmy=[1 0 0]-My;
% Kr=0.5/sum(Mrmy(find(Mrmy>0)));
% Mrmy=Kr*Mrmy;
% Msmpte240=[My;Mbmy;Mrmy]
% ycbcr=Msmpte240*rgb;
% ycbcr(1,:)=(ycbcr(1,:)*(235-16)+16)*2^(m-8);
% ycbcr(2:3,:)=(ycbcr(2:3,:)*(240-16)+128)*2^(m-8);
% ycbcr=round(ycbcr)
% ROMval=[ROMval;dec2bin(ycbcr'*2.^[2*m m 0]',m)];


disp('SMPTE 274M:')
% RGB Reference Primaries from Smpte274M
Xr = 0.640; Yr = 0.330;
Xg = 0.300; Yg = 0.600;
Xb = 0.150; Yb = 0.060;
% Reference White primaries
Xw = 0.3127; Yw = 0.3290;
% Calculations
Zr = 1 -(Xr+Yr);
Zg = 1 -(Xg+Yg);
Zb = 1 -(Xb+Yb);
Zw = 1-(Xw+Yw);
P=[[Xr Xg Xb];[Yr Yg Yb];[Zr Zg Zb]];
W= [Xw/Yw; 1; Zw/Yw];
P_inv = eye(3)/P;
CrCgCb = P_inv*W;
C= [[CrCgCb(1) 0 0];[0 CrCgCb(2) 0];[0 0 CrCgCb(3)]];
NPM = P*C;
% Luminance Equation
My = [NPM(2) NPM(5) NPM(8)]
%My=[0.2126390059 0.7151686788 0.0721923154]
Mbmy=[0 0 1]-My;
Kb=0.5/sum(Mbmy(find(Mbmy>0)));
Mbmy=Kb*Mbmy;
Mrmy=[1 0 0]-My;
Kr=0.5/sum(Mrmy(find(Mrmy>0)));
Mrmy=Kr*Mrmy;
Msmpte274=[My;Mbmy;Mrmy]
ycbcr=Msmpte274*rgb;
ycbcr(1,:)=(ycbcr(1,:)*(235-16)+16)*2^(m-8);
ycbcr(2:3,:)=(ycbcr(2:3,:)*(240-16)+128)*2^(m-8);
ycbcr=round(ycbcr);
ROMval=[ROMval;dec2bin(ycbcr'*2.^[2*m m 0]',m)];
ycbcr'

disp('ITU-R BT.601:')
My=  [0.299 0.587 0.114]
Mbmy=[0 0 1]-My;
Kb=0.5/sum(Mbmy(find(Mbmy>0)));
Mbmy=Kb*Mbmy;
Mrmy=[1 0 0]-My;
Kr=0.5/sum(Mrmy(find(Mrmy>0)));
Mrmy=Kr*Mrmy;
Mitu601=[My;Mbmy;Mrmy]
ycbcr=Mitu601*[rgb rgbgrey];
ycbcr(1,:)=(ycbcr(1,:)*(235-16)+16)*2^(m-8);
ycbcr(2:3,:)=(ycbcr(2:3,:)*(240-16)+128)*2^(m-8);
ycbcr=round(ycbcr);
ROMval=[ROMval;dec2bin(ycbcr'*2.^[2*m m 0]',m)];
ycbcr'

ROMval

size(ROMval)

