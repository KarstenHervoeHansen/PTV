y=[];
cb=[bin2dec('011000111010')*ones(12,1); 4*512*ones(12,1)];

% 75% red:
% HD: 001100101111 011011001100 110101000000
% SD: 010000010010 011000111010 110101000000

hcb=[34 185 492 839 996 839 492 185 34];

cbresp=conv(cb,hcb);
cbresp=round(cbresp/2^14);
reshape(cbresp,2,length(cbresp)/2)'