% Filter step response for fast filter:

Nbits=12;

level=0.75;

step=[4*64*ones(12,1); 4*(64+level*(940-64))*ones(12,1)];

% 75% red:
% HD: 001100101111 011011001100 110101000000
% SD: 010000010010 011000111010 110101000000

hslow=[34 185 492 839  996 839 492 185 34];
sum(hslow)
hfast=[ 0   0  98 996 1908 996  98   0  0];
sum(hfast)

disp('Fast response:')
disp('  msb     lsb')
resp=round(conv(step,hfast)/2^(24-Nbits))*2^(24-Nbits);
msb=floor(resp/2^14);
lsb=(resp-msb*2^14)/2^12;
[msb lsb]

%disp('Slow response:')
%disp('  msb     lsb')
%resp=round(conv(step,hslow)/2^(24-Nbits))*2^(24-Nbits);
%msb=floor(resp/2^14);
%lsb=(resp-msb*2^14)/2^12;
%[msb lsb]
