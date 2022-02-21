%    R G B
% Y
% cb
% cr

rgb=[[0 0 0];[0 0 1];[0 1 0];[0 1 1];[1 0 0];[1 0 1];[1 1 0];[1 1 1]]';
rgb=[rgb 0.75*rgb];

m=8;

disp('SMPTE 240M:')     
My=  [0.212 0.701 0.087];
Mbmy=[0 0 1]-My;
Kb=0.5/sum(Mbmy(find(Mbmy>0)));
Mbmy=Kb*Mbmy;
Mrmy=[1 0 0]-My;
Kr=0.5/sum(Mrmy(find(Mrmy>0)));
Mrmy=Kr*Mrmy;
Msmpte240=[My;Mbmy;Mrmy]
ycbcr=Msmpte240*rgb;
ycbcr(1,:)=(ycbcr(1,:)*(235-16)+16)*2^(m-8);
ycbcr(2:3,:)=(ycbcr(2:3,:)*(240-16)+128)*2^(m-8);
ycbcr=round(ycbcr)


disp('SMPTE RP177:')
% Mcie=[[0.64 0.3 0.15];
%       [0.33 0.6 0.06]];
% Wref=[0.3127 0.3290];
My=[0.2126390059 0.7151686788 0.0721923154];
Mbmy=[0 0 1]-My;
Kb=0.5/sum(Mbmy(find(Mbmy>0)));
Mbmy=Kb*Mbmy;
Mrmy=[1 0 0]-My;
Kr=0.5/sum(Mrmy(find(Mrmy>0)));
Mrmy=Kr*Mrmy;
Msmpte177=[My;Mbmy;Mrmy]
ycbcr=Msmpte177*rgb;
ycbcr(1,:)=(ycbcr(1,:)*(235-16)+16)*2^(m-8);
ycbcr(2:3,:)=(ycbcr(2:3,:)*(240-16)+128)*2^(m-8);
ycbcr=round(ycbcr)

disp('ITU-R BT.601:')     
My=  [0.299 0.587 0.114];
Mbmy=[0 0 1]-My;
Kb=0.5/sum(Mbmy(find(Mbmy>0)));
Mbmy=Kb*Mbmy;
Mrmy=[1 0 0]-My;
Kr=0.5/sum(Mrmy(find(Mrmy>0)));
Mrmy=Kr*Mrmy;
Mitu601=[My;Mbmy;Mrmy]
ycbcr=Mitu601*rgb;
ycbcr(1,:)=(ycbcr(1,:)*(235-16)+16)*2^(m-8);
ycbcr(2:3,:)=(ycbcr(2:3,:)*(240-16)+128)*2^(m-8);
ycbcr=round(ycbcr)

