Ts=1/(27e6/91);
w=logspace(-1,3,1000);

%Phase Detector:
Hf2p=2*pi*1./(i*w);

%Loop Filter:
Gp=1/4;
Gi=1/32;
Hl=1./(i*w)*Gi+Gp;

%RC Filter:
R=100e3;
C=680e-9;
Hrc=1./(R*C*i*w+1);

%VCXO:
Kvcxo=90e-6;
fvcxo=148.5e6/1.001;

%Decimator:
Gdec=1/500;

%Loop Gain:
Hloop=Hf2p.*Hl.*Hrc.*Kvcxo.*fvcxo.*Gdec;

%From VCXO voltage noise to jitter:
HVc2jit=Kvcxo.*fvcxo.*(1./(i*w)).*10./(1+Hloop);


figure(1)
subplot(2,1,1)
semilogx(w,20*log10(abs(Hf2p)));
grid on
subplot(2,1,2)
semilogx(w,180./pi*angle(Hf2p));
grid on

figure(2)
subplot(2,1,1)
semilogx(w,20*log10(abs(Hl)));
title('Loop Filter')
grid on
subplot(2,1,2)
semilogx(w,180./pi*angle(Hl));
grid on

figure(3)
subplot(2,1,1)
semilogx(w,20*log10(abs(Hrc)));
title('RC Filter')
grid on
subplot(2,1,2)
semilogx(w,180./pi*angle(Hrc));
grid on

figure(4)
subplot(2,1,1)
semilogx(w,20*log10(abs(Hloop)));
grid on
title('Loop Gain')
subplot(2,1,2)
semilogx(w,180./pi*angle(Hloop));
grid on

figure(5)
subplot(2,1,1)
semilogx(w,20*log10(abs(HVc2jit)));
grid on
title('From VCXO voltage (Vc) to output jitter (UI)')
subplot(2,1,2)
semilogx(w,180./pi*angle(HVc2jit));
grid on
