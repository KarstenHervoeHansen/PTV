% pt8612_analog_pll_init:

%    -----                            -------
% 	     |                          |    
% 	     |------ Ri ----------------|
% 	FPGA |            |     |       | VCXO
% 	     |            Cs    |       |
% 	     |            |     Cp      |
% 	-----             Rs    |        -------
% 	                  |     |
% 	                  /     /

%Pulse hight out of phase comparator:
Vcc= 3.3

%Loop filter components:
Ri = 100e3
Rs = 100e3
Rp = 1e6
Cs = 680e-9
Cp = Cs/20

%VCXO characteristics:
Kvcxo=90e-6     
fvcxo=148.5e6/1.001
Vvcxo0 = 1.25;  %Nominal control voltage at fvcxo
Vvcxomin=0;
Vvcxomax=2.5;

%Frequency parameters:
fin=27e6;
Ndec1=91;
Ndec2=500;

fpll=fin/Ndec1;


% Loop filter:
% dx/dt = A*x+B*i
% Vo    = C*x+D*i
A = [[-1/Cp*(1/Rs+1/Rp) 1/Cp/Rs]
     [1/Rs/Cs          -1/Cs/Rs]]
B = [1/Cp 0].'
C = [1 0]
D = 0

loopfilter = ss(A,B*(Vcc-Vvcxo0)/Ri,C,D)       % From ton/T to Vo
lfmax      = ss(A,B*(Vcc-Vvcxomax)/Ri,C,D)     % Max Vo
lfmin      = ss(A,B*(Vcc-Vvcxomin)/Ri,C,D)     % Min Vo

figure(1)
bode(loopfilter,{1e-1,1e3})
grid on;
title('loop filter')

% VCXO :
Gvcxo = Kvcxo * fvcxo;

% Decimator 2 :
Gdec2 = 1/Ndec2;

% Phase detector : 
phasedetector = ss(0,1,1,0);

% Open loop gain :
openloop = phasedetector*loopfilter*Gvcxo*Gdec2;
olmax    = phasedetector*lfmax*Gvcxo*Gdec2;
olmin    = phasedetector*lfmin*Gvcxo*Gdec2;
figure(2)
bode(openloop,olmax,olmin,{1e-1,1e3})
title('open loop')
grid on


% From fout to jitter :
freq2jitter = ss(0,1,1/fvcxo,0);

% From time to UI :
Gt2ui = 10*fvcxo;

% From phase detector ripple current to output jitter (UI):
ripple2jitter=(loopfilter/((Vcc-Vvcxo0)/Ri))*Gvcxo*freq2jitter*Gt2ui/(1-openloop);
figure(3)
bode(ripple2jitter,{1e-1,10e3})
grid on
title('ripple current from phase detector to jitter output (UI)')

% From input jitter (time) to output jitter (time):
jitter2jitter=loopfilter*Gvcxo*freq2jitter*fin/Ndec1/(1-openloop);
figure(4)
bode(jitter2jitter,{1e-1,1e3})
grid on
title('jitter input (time) to jitter output (time)')

