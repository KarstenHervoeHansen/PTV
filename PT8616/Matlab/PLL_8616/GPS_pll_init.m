clear;

Ga = 2;
Gb = 2;
Kv=4e-6;
Ts=1/100e6; %Clockcycle for phasedetector

loopfilter = ss(0,1,Ga,Gb);

Gdac = 2^-16*5;

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
figure(4)
w=logspace(-3,1,1000);
semilogx(w,180/pi*angle(exp(-i*w/2)))
title('Delay')
grid on

