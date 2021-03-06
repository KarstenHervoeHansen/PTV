% pt8612_analog_pll_init:

%       Vcc
%        | 
%     s1  /
%        |                              
% 	     R1    ii                       
% 	     |----->-------- Ri -------------------
% 	     R2        |          |     |     |     +
% 	     |         |         Cs     |     |     Vo
% 	  s2  /        C1         |     Cp    Rp    -
% 	     |         |          Rs    |     |    |
% 	     /         /          |     |     /    /
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


wmax = 1e4;
wmin = 1e0;

%VCXO characteristics:
Kvcxo=90e-6
fvcxo=148.5e6/1.001
Vvcxomin=0;
Vvcxomax=2.5;
Vvcxo0 = 1.25;  %Linearization voltage

%Frequency parameters:
fin=27e6;
Ndec1=91;
Ndec2=500;
fpll=fin/Ndec1;

%Pulse hight out of phase comparator:
Vcc= 3.3

%Loop filter components:
Ri = 500e3
Rs = 22000
Rp = 1e6
Cs = 680e-9
Cp = Cs/20

% Phase comparator : 
Tpll=1/fpll;
t_both=2e-9
R2 = 220
R1 = 220
C1 = 20e-9

Cfpga = 20e-12;                     % parasitic capacitance on fpga output
Rhc = Tpll/Cfpga                    % Equivalent resistance due to parasitic capacitance
Rlc = Tpll/Cfpga                    % Equivalent resistance due to parasitic capacitance
Rhr = R1*Tpll/t_both                % Equivalent resistance due to pulsed resistance 
Rlr = R2*Tpll/t_both                % Equivalent resistance due to pulsed resistance 
Rheq= 1/(1/Rhc+1/Rhr)               % Equivalent resistance to Vcc
Rleq= 1/(1/Rlc+1/Rlr)               % Equivalent resistance to ground
Rth = 1/(1/Rheq+1/Rleq)             % Equivalent thevenin resistance due to on-pulses in both switches. Contribution from both capacitance and resistance
Vth = Vcc*Rleq/(Rleq+Rheq)          % Equivalent thevenin voltage

Glt2i = Vvcxo0/R2;                  % From tdiff/T to ii when vcxo is before ref (low pulses)
Ght2i = (Vcc-Vvcxo0)/R1;            % From tdiff/T to ii when vcxo is after ref (high pulses)

phasecomplow = ss(0,1,Glt2i,0);     % From feedback frequency to ii
phasecomphigh = ss(0,1,Ght2i,0);    % From feedback frequency to ii

%Steady state error:
ith   = (Vth-Vvcxo0)/Rth;
% tdiff is positive if vcxo is after ref
if ith >= 0,    % tdiff 
   Gt2i0 = Glt2i;
else
   Gt2i0 = Ght2i;
end
tdiff0 = -ith/Gt2i0*Tpll

phasecomparator = ss(0,1,Gt2i0,0);    % From feedback frequency to ii at linearization point
fknee = 1/(2*pi*R2/2*C1)


% Loop filter:
% dx/dt = A*x+B*u
% Vo    = C*x+D*u
% x = [Vc1 Vcp Vcs]'
% u = [ii Vth]' 
%    ii                       
% ---->----------------- Ri ---------------------
% 	      |        |          |     |     |     +
% 	     Rth       |         Cs     |     |     Vo
% 	      |        C1         |     Cp    Rp    -
% 	     Vth       |          Rs    |     |     |
% 	      |        /          |     /     /     /
% 	      /                   /     

A = [[-1/C1*(1/Ri+1/Rth)      1/C1/Ri           0   ]
     [ 1/Cp/Ri         -(1/Ri+1/Rs+1/Rp)/Cp  1/Cp/Rs]
     [    0                   1/Cs/Rs       -1/Cs/Rs]];
B = [[1/C1 0 0].' [1/C1/Rth 0 0].'];
C = [0 1 0];
D = [0 0];
Cvc1 = [1 0 0];

loopfilter = ss(A,B(:,1),C,D(1));      % From ii to Vo

figure(1)
bode(loopfilter,{wmin,wmax})
grid on;
title('loop filter')

% VCXO :
Gvcxo = Kvcxo * fvcxo;

% Decimator 2 :
Gdec2 = 1/Ndec2;

% Open loop gain :
openlooplow  = phasecomplow*loopfilter*Gvcxo*Gdec2;
openloophigh = phasecomphigh*loopfilter*Gvcxo*Gdec2;
openloop     = phasecomparator*loopfilter*Gvcxo*Gdec2;
figure(2)
hold off
bode(openlooplow,{wmin,wmax})
hold on
bode(openloophigh,{wmin,wmax})
title('open loop')
grid on

% From fout to jitter :
freq2jitter = ss(0,1,1/fvcxo,0);

% From time to UI :
Gt2ui = 10*fvcxo;

% From phase detector ripple current to output jitter (UI):
ripple2jitter=phasecomparator*loopfilter*Gvcxo*freq2jitter*Gt2ui/(1-openloop);
figure(3)
bode(ripple2jitter,{wmin,wmax})
grid on
title('ripple current from phase detector to jitter output (UI)')

% From input jitter (time) to output jitter (time):
% jitter2jitter=loopfilter*Gvcxo*freq2jitter*fin/Ndec1/(1-openloop);
% figure(4)
% bode(jitter2jitter,{1e-1,wmax})
% grid on
% title('jitter input (time) to jitter output (time)')

figure(2)