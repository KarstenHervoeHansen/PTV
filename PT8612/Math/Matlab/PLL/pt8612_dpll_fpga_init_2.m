clear

wmax = 1e4;
wmin = 1e0;

%Frequency parameters:
Tlogic=1/148.5e6;
fvcxo=148.5e6

% Phase comparator :
time_diff_max=250/148.5e6;
hphasecomparator = ss(0,1,2/Tlogic,0);
figure(1)
bode(hphasecomparator,{wmin,wmax})
title('Phase Comparator')
grid on

% Loop filter:
%fast%slow%normal
T=4*495/fvcxo;
alpha = -5%-5%-9%-7
beta  = 6%6%4%5
% dx/dt = A*x+B*u
% Vo    = C*x+D*u
% x = [Vc1 Vcp Vcs]'
% u = [ii Vth]' 
hloopfilter = ss(0,1/T,2^alpha,2^beta);
wkneeloop = 2^alpha/2^beta/T

%DAC:
Nbits=16
Vcc= 2.5
Mdac=1
hdac = 2^-Nbits*Vcc*Mdac;

% RC filter : 
C=680e-9
R=1/(wkneeloop*20)/C %100000
R=3900
frc=1/2/pi/R/C
hrclowpass = ss(-1/C/R,1/C/R,1,0);

%VCXO characteristics:
Kvcxo=90e-6
Vvcxomin=0;
Vvcxomax=2.5;
Vvcxo0 = 1.25;  %Linearization voltage
hvcxo = Kvcxo;

% Open loop gain:
hopenloop = hphasecomparator*hloopfilter*hdac*hrclowpass*hvcxo;
figure(2)
bode(hopenloop,{wmin,wmax})
title('Open loop')
grid on

% loop filter:
figure(3)
bode(hloopfilter,{wmin,wmax})
title('Loop filter')
grid on

% RC filter:
figure(4)
bode(hrclowpass,{wmin,wmax})
title('RC filter')
grid on

% Vc to out:
hvc2fout = hvcxo / (1 + hopenloop);
figure(5)
bode(hvc2fout,{wmin,wmax})
title('Vc to f_o_u_t')
grid on

% From fout to jitter :
hfout2ui = 10*fvcxo*ss(0,1,1/fvcxo,0);

% Vc to UI:
hvc2ui = hvcxo / (1 + hopenloop) * hfout2ui;
figure(6)
bode(hvc2ui,{wmin,wmax})
title('Vc to UI')
grid on

% f_in to f_out:
hfin2fout = hopenloop / (1 + hopenloop);
figure(7)
bode(hfin2fout,{wmin,wmax})
title('f_i_n to f_o_u_t')
grid on
