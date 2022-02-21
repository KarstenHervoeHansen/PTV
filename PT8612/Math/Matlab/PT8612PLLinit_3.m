w=logspace(-1,5,10000);

%VCXO:
Kvcxo=90e-6;
fvcxo=148.5e6/1.001;
Gvcxo=Kvcxo*fvcxo;

%Phase compartor:
Nfodiv=500*4;
Tsphase=1/fvcxo*Nfodiv;
Hf2t=1./(i*w);

%Loop Filter:
%Digital Approximation:
Tlfp=Tsphase;
Gpd=0.0002
Tlfi=10000*500/fvcxo
Gid=50e-5*Tlfi
Tlfd=100*500/fvcxo;
Gdd=0%1/8;
Hlfd=Gpd+Gid*(1./(exp(i*w*Tlfi)-1))+Gdd*(1-exp(-i*w*Tlfd));
Hlf=Hlfd;

%DeltaSigma DAC:
Vcc=3.3;
Nbit=12;
Tds=8/fvcxo;
Mdac=1;
Gdac=Vcc*Mdac;
Hntf=(exp(i*w*Tds)-1)./exp(i*w*Tds).*Gdac;

%RC Filter:
R=82e3;
C=680e-9;
Hrc=1./(R*C*i*w+1);

%Loop Gain:
Hloop=Hf2t.*Hlf.*Gdac.*Hrc.*Kvcxo.*fvcxo;
%Phase margin:
PhaseMargin=45;
GainMargin=-20*log10(abs(Hloop(max(find(180/pi*angle(Hloop)>-180+PhaseMargin)))))
PhaseMargin=180+180/pi*angle(Hloop(min(find(abs(Hloop)<1))))

%From VCXO voltage noise to jitter:
HVc2jit=Kvcxo.*fvcxo./(1+Hloop).*(1./(i*w)).*10;

%From DS quantization noise to jitter:
Hq2jit=Hntf.*HVc2jit;

%From DAC noise to jitter:
Hdac2jit=Hrc.*HVc2jit;

%From fin to fout:
Hfi2fo=((1./(i*w)).*Hlf.*Gdac.*Hrc.*Gvcxo)./(1+Hloop);

%From phase detector quantization to jitter:
Hpq2jit=Hlf.*Gdac.*Hrc.*HVc2jit;


figure(1)
subplot(2,1,1)
semilogx(w,20*log10(abs(Hq2jit)));
grid on
title('From Delta-Sigma quantization noise to output jitter (UI)')
subplot(2,1,2)
semilogx(w,180./pi*angle(Hq2jit));
grid on

figure(2)
subplot(2,1,1)
semilogx(w,20*log10(abs(Hlf)));
title('Loop Filter')
grid on
subplot(2,1,2)
semilogx(w,180./pi*angle(Hlf));
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

figure(6)
subplot(2,1,1)
semilogx(w,20*log10(abs(Hdac2jit)));
grid on
title('From DAC output noise (voltage) to output jitter (UI)')
subplot(2,1,2)
semilogx(w,180./pi*angle(Hdac2jit));
grid on

figure(7)
subplot(2,1,1)
semilogx(w,20*log10(abs(Hfi2fo)));
grid on
title('From fin to fout')
subplot(2,1,2)
semilogx(w,180./pi*angle(Hfi2fo));
grid on

figure(8)
subplot(2,1,1)
semilogx(w,20*log10(abs(Hpq2jit)));
grid on
title('From phase detector quantization to jitter (UI)')
subplot(2,1,2)
semilogx(w,180./pi*angle(Hpq2jit));
grid on