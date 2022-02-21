Nbits=13;
x=(0:2^Nbits-1)';
y=sin(x/2^Nbits*pi/2);

%Basis vectors:
M=[x.^3 x.^2 x x.^0];
c = M\y

ypoly=M*c;
err=y-ypoly;

plot([y ypoly]);

thd=10*log10(sum(y.^2)/sum(err.^2))