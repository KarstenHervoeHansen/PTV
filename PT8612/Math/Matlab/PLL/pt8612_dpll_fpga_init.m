clear

wmax = 1e4;
wmin = 1e0;

%Frequency parameters:
fin=27e6;
Ndec1=4*91;
Ndec2=4*500;
fpll=fin/Ndec1;
hdecimator1 = 1/Ndec1;
hdecimator2 = 1/Ndec2;

% Phase comparator : 
Tpll=1/fpll;
hphasecomparator = ss(0,1,2*Ndec2,0);

% Loop filter:
%fast%slow%normal
alpha = -7%-3%-15%-9
beta  = 5%8%2%5
% dx/dt = A*x+B*u
% Vo    = C*x+D*u
% x = [Vc1 Vcp Vcs]'
% u = [ii Vth]' 
hloopfilter = ss(0,1/Tpll,2^alpha,2^beta);
wkneeloop = 2^alpha/2^beta/Tpll

%DAC:
Tdac = 4/148.5e6
Nbits=16
Vcc= 2.5
Mdac=1
hdac = 2^-Nbits*Vcc*Mdac;

% RC filter : 
C=680e-9
R=1/(wkneeloop*20)/C %100000
frc=1/2/pi/R/C
hrclowpass = ss(-1/C/R,1/C/R,1,0);

%VCXO characteristics:
Kvcxo=90e-6
fvcxo=148.5e6/1.001
Vvcxomin=0;
Vvcxomax=2.5;
Vvcxo0 = 1.25;  %Linearization voltage
hvcxo = Kvcxo*fvcxo;

% Open loop gain:
hopenloop = hphasecomparator*hloopfilter*hdac*hrclowpass*hvcxo*hdecimator2;
figure(1)
bode(hopenloop,{wmin,wmax})
title('open loop')
grid on

% loop filter:
figure(2)
bode(hloopfilter,{wmin,wmax})
title('loop filter')
grid on

% RC filter:
figure(3)
bode(hrclowpass,{wmin,wmax})
title('RC filter')
grid on