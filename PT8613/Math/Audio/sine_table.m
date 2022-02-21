clear

% Sine table:
nbits=18;       % nbit 2's complements representation
a=sin((0:47)/48*2*pi)';
abin=dec2bin(mod(a*(2^(nbits-1)-1),2^nbits))

%Attenuator parameters:
gain=10.^(flipud((-60:0)')/20);
bit_shifts=-floor(log2(gain))
multiply=gain.*2.^bit_shifts;
multiplier_gain=round(multiply*2^(nbits-2))
