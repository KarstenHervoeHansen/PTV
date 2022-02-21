clear;

Ga = 1/32;
Gb = 2;
Kv=4e-6;
Ts=1/200e6; %Clockcycle for phasedetector

loopfilter = ss(0,1,Ga,Gb);

Gdac = 2^-16*3.3;

ppsgenerator = Kv;

phasedifference = ss(0,1,1/Ts,0);

loopgain = phasedifference*loopfilter*Gdac*ppsgenerator;

figure(1)
bode(loopfilter)
title('Loopfilter')
grid on
figure(2)
bode(phasedifference)
title('Phase difference')
grid on
figure(3)
bode(loopgain)
title('Loopgain')
grid on

