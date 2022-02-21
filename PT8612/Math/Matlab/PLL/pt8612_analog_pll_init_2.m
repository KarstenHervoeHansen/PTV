% pt8612_analog_pll_init:

%       Vcc
%        | 
%     s1  /
%        |                              
% 	     R1    ii                       
% 	     |----->-------- Ri -----------------
% 	     R2        |          |     |         +
% 	     |         |         Cs     |         Vo
% 	  s2  /        C1         |     Cp        -
% 	     |         |          Rs    |       |
% 	     /         /          |     |       /
% 	                          /     /


%        -------         -------         --
% Ref   |       |       |       |       |
%     --         -------         -------
%           -------         -------
% Vco      |       |       |       |
%     -----         -------         -------
%        ---             ---             --
% s1    |   |           |   |           |
%     --     -----------     -----------
%           -               -              
% s2       ||              || t_both_on              
%     -----  --------------  --------------
%                         
%        --              --              --
% s1-s2 |  |            |  | tdiff      |
%     --    ------------    ------------


wmax = 1e6;

%VCXO characteristics:
Kvcxo=90e-6
fvcxo=148.5e6/1.001
Vvcxomin=0;
Vvcxomax=2.5;
Vvcxo0 = 1.25;  %Nominal control voltage at fvcxo

%Frequency parameters:
fin=27e6;
Ndec1=91;
Ndec2=500;
fpll=fin/Ndec1;

%Pulse hight out of phase comparator:
Vcc= 3.3

%Loop filter components:
Ri = 100e3
Rs = 22000
Rp = 1e6
Cs = 680e-9
Cp = Cs/20

% Phase comparator : 
R2 = 4700
R1 = R2*(Vcc-Vvcxo0)/Vvcxo0    % no change!
C1 = 10e-9
Gt2i = Vvcxo0/R2;              % From tdiff/T to ii
phasecomparator = ss(0,1,Gt2i,0);   % From feedback frequency to ii
fknee = 1/(2*pi*R2/2*C1)


% Loop filter:
% dx/dt = A*x+B*i
% Vo    = C*x+D*i
% x = [Vc1 Vcp Vcs]'
%
% 	           ii                       
% 	       ---->-------- Ri -----------------
% 	               |          |     |         +
% 	               |         Cs     |         Vo
% 	               C1         |     Cp        -
% 	               |          Rs    |       |
% 	               /          |     |       /
% 	                          /     /

A = [[-1/C1/Ri     1/C1/Ri          0   ]
     [ 1/Cp/Ri  -(1/Ri+1/Rs)/Cp  1/Cp/Rs]
     [    0        1/Cs/Rs      -1/Cs/Rs]];
B = [1/C1 0 0].';
C = [0 1 0];
D = 0;
Cvc1 = [1 0 0];

loopfilter = ss(A,B,C,D);      % From ii to Vo

figure(1)
bode(loopfilter,{1e-1,wmax})
grid on;
title('loop filter')

% VCXO :
Gvcxo = Kvcxo * fvcxo;

% Decimator 2 :
Gdec2 = 1/Ndec2;

% Open loop gain :
openloop = phasecomparator*loopfilter*Gvcxo*Gdec2;
figure(2)
bode(openloop,{1e-1,wmax})
title('open loop')
grid on

% From fout to jitter :
freq2jitter = ss(0,1,1/fvcxo,0);

% From time to UI :
Gt2ui = 10*fvcxo;

% From phase detector ripple current to output jitter (UI):
ripple2jitter=phasecomparator*loopfilter*Gvcxo*freq2jitter*Gt2ui/(1-openloop);
figure(3)
bode(ripple2jitter,{1e-1,10e3})
grid on
title('ripple current from phase detector to jitter output (UI)')

% From input jitter (time) to output jitter (time):
% jitter2jitter=loopfilter*Gvcxo*freq2jitter*fin/Ndec1/(1-openloop);
% figure(4)
% bode(jitter2jitter,{1e-1,wmax})
% grid on
% title('jitter input (time) to jitter output (time)')

