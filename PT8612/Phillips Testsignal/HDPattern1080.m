function HDPattern1080()

filename='HDTestPattern(1920x1080)'

NsampH=1920
NsampV=1080

NgridV=13;
Rcircle=6;
Rcorner=1.5;
MixzoneCenter=5;

%Generate grid:
TargetMargin=20;
gridsize=round(((NsampV-2*TargetMargin)/NgridV)/2)*2;
NgridH=floor((((NsampH-2*TargetMargin)/gridsize)+1)/2)*2-1;

marginV=(NsampV-NgridV*gridsize)/2;
marginH=(NsampH-NgridH*gridsize)/2;

Msquare=[ones(1,gridsize);[ones(gridsize-2,1) 0.48*ones(gridsize-2) ones(gridsize-2,1)];ones(1,gridsize)];

Mgrid=repmat(Msquare,(NgridV+1)/2,(NgridH+1)/2);
Mgrid=Mgrid(1:end-gridsize/2,gridsize/2+1:end);

sampV=size(Mgrid,1)*2;
sampH=size(Mgrid,2)*2;

%Corner circles:
LineWidth=3;
Rcornercenterrings=0.3*gridsize; %Determined by centerrings

%Resolution wedges:

%300-600 Lines:
Wmin=300/NsampV*pi; %Min freq rad/sample (Nyquist at 300 lines)
Wmax=600/NsampV*pi; %Min freq rad/sample (Nyquist at 600 lines)

d1=1/Wmax*2*pi;
d2=1/Wmin*2*pi;
x2=Rcorner*gridsize-LineWidth/2;
x1=Rcornercenterrings;
x0=x1-d1*(x2-x1)/(d2-d1);
k=2*pi*(x1-x0)/d1;

x=repmat((1:2*gridsize)-0.5,2*gridsize,1);
y=repmat((1:2*gridsize)'-0.5,1,2*gridsize);
phase=k*y./(x-x0);

%limit phase:
phase=reshape(phase,(2*gridsize)^2,1);
phase(find(phase>5*pi))=5*pi;
phase=reshape(phase,2*gridsize,2*gridsize);
Wedges36=(cos(phase)+1)/2;
Wedges36=[flipud(Wedges36);Wedges36];
Wedges36=[zeros(4*gridsize,2*gridsize) Wedges36];
Wedges36=Wedges36+Wedges36';

%Dots:
Rdot=4;
dist=sqrt((x-(x1+(x2-x1)/3)).^2+y.^2);
phase=dist*pi/Rdot;

%limit phase:
phase=reshape(phase,(2*gridsize)^2,1);
phase(find(phase>pi))=pi;
phase=reshape(phase,2*gridsize,2*gridsize);
ResDots1=(1-cos(phase))/2;

dist=sqrt((x-(x1+(x2-x1)*2/3)).^2+y.^2);
phase=dist*pi/Rdot;
%limit phase:
phase=reshape(phase,(2*gridsize)^2,1);
phase(find(phase>pi))=pi;
phase=reshape(phase,2*gridsize,2*gridsize);
ResDots2=(1-cos(phase))/2;

ResDots=ResDots1.*ResDots2;
ResDots=[flipud(ResDots); ResDots];
ResDots=[fliplr(ResDots) ResDots];
ResDots=ResDots.*ResDots';

%600-900 Lines:
Wmin=600/NsampV*pi; %Min freq rad/sample (Nyquist at 600 lines)
Wmax=900/NsampV*pi; %Min freq rad/sample (Nyquist at 900 lines)

d1=1/Wmax*2*pi;
d2=1/Wmin*2*pi;
x2=Rcorner*gridsize-LineWidth/2;
x1=Rcornercenterrings;
x0=x1-d1*(x2-x1)/(d2-d1);
k=2*pi*(x1-x0)/d1;

x=repmat((1:2*gridsize)-0.5,2*gridsize,1);
y=repmat((1:2*gridsize)'-0.5,1,2*gridsize);
phase=k*y./(x-x0);

%limit phase:
phase=reshape(phase,(2*gridsize)^2,1);
phase(find(phase>7*pi))=7*pi;
phase=reshape(phase,2*gridsize,2*gridsize);

Wedges58=(cos(phase)+1)/2;
Wedges58=[flipud(Wedges58);Wedges58];
Wedges58=[zeros(4*gridsize,2*gridsize) Wedges58];
Wedges58=Wedges58+Wedges58';
Wedges58=flipud(fliplr(Wedges58));

Wedges=Wedges36+Wedges58;
Wedges=flipud(Wedges);
Wedges=Wedges.*ResDots;

%limit value:
Wedges=reshape(Wedges,(4*gridsize)^2,1);
Wedges(find(Wedges>1))=1;
Wedges=reshape(Wedges,4*gridsize,4*gridsize);

%Resolution circles:
Nrings=4;
Rrescirc=1/3;
W=600/NsampV*pi; %Min freq rad/sample (Nyquist at 600 lines)
dist=sqrt(x.^2+y.^2)-Rrescirc*gridsize;
phase=dist*W;

%limit phase:
phase=reshape(phase,(2*gridsize)^2,1);
phase(find(phase>-pi))=-pi;
phase(find(phase<-2*pi*Nrings))=0;
phase=reshape(phase,2*gridsize,2*gridsize);

ResCirc=(1-cos(phase))/2;
ResCirc=[fliplr(ResCirc) ResCirc];
ResCirc=[flipud(ResCirc); ResCirc];

%mix resolution rings and wedges:
phase=dist*W;

%limit phase:
phase=reshape(phase,(2*gridsize)^2,1);
phase(find(phase>0))=0;
phase(find(phase<-pi))=-pi;
phase=reshape(phase,2*gridsize,2*gridsize);
MixResCirc=(1-cos(phase))/2;
MixResCirc=[fliplr(MixResCirc) MixResCirc];
MixResCirc=[flipud(MixResCirc); MixResCirc];

Mcornercontent=Wedges.*(1-MixResCirc)+ResCirc.*MixResCirc;

%Corner Grid:
MCgrid=repmat(Msquare,[4 4 1]);

%Mix for corner circles:
MixzoneCorner=4;
Mdist=sqrt(repmat(((1:2*gridsize)'-0.5).^2,1,2*gridsize)+repmat(((1:2*gridsize)-0.5).^2,2*gridsize,1));

%Inner Mix:
Mdist=Mdist-Rcorner*gridsize+LineWidth/2;
MmixInner=(sin(Mdist*pi/MixzoneCorner)+1)/2;
Moutside=Mdist>MixzoneCorner/2;
Minside=Mdist<-MixzoneCorner/2;
MmixInner=MmixInner.*(~Minside);
MmixInner=MmixInner.*(~Moutside)+Moutside;
MmixInner=[fliplr(MmixInner) MmixInner];
MmixInner=[flipud(MmixInner); MmixInner];

%Outer mix:
Mdist=Mdist-LineWidth;
MmixOuter=(sin(Mdist*pi/MixzoneCorner)+1)/2;
Moutside=Mdist>MixzoneCorner/2;
Minside=Mdist<-MixzoneCorner/2;
MmixOuter=MmixOuter.*(~Minside);
MmixOuter=MmixOuter.*(~Moutside)+Moutside;
MmixOuter=[fliplr(MmixOuter) MmixOuter];
MmixOuter=[flipud(MmixOuter); MmixOuter];

WhiteCorner=ones(size(MmixOuter));

%Mix Corner circles:
MCorner=WhiteCorner.*(1-MmixOuter)+MCgrid.*MmixOuter;
MCorner=Mcornercontent.*(1-MmixInner)+MCorner.*MmixInner;

Mgrid(1:4*gridsize,end-4*gridsize+1:end)=MCorner;

Mgrid=[fliplr(Mgrid) Mgrid];
Mgrid=[Mgrid; flipud(Mgrid)];
Mgrid=repmat(Mgrid,[1 1 3]);

%Vertical Sweep:
Wmin=300/NsampV*pi; %Min freq rad/sample (Nyquist at 300 lines)
Wmax=800/NsampV*pi; %Min freq rad/sample (Nyquist at 800 lines)
b=Wmin;
a=(Wmax-b)/2/(5*gridsize-3);
x=(0:5*gridsize-3)';
Phase=a*x.^2+b*x;

VerSweep=(1-cos(Phase))/2;
VerSweepud=flipud(VerSweep);
VerSweep=repmat(VerSweep,[1 gridsize-2 3]);
VerSweepud=repmat(VerSweepud,[1 gridsize-2 3]);

Mgrid(4*gridsize+2:9*gridsize-1,gridsize+2:2*gridsize-1,:)=VerSweep;
Mgrid(4*gridsize+2:9*gridsize-1,end-2*gridsize+2:end-gridsize-1,:)=VerSweepud;

%Diagonal resolution:
%400 Lines
Wlines=400/NsampV*pi;
MDist=1/sqrt(2)*toeplitz(flipud((0:2*gridsize-3)'),(2*gridsize-3:4*gridsize-6));

MdiagRes=(1-cos(Wlines*MDist))/2;
MdiagReslr=fliplr(MdiagRes);
MdiagRes=repmat(MdiagRes,[1 1 3]);
MdiagReslr=repmat(MdiagReslr,[1 1 3]);

Mgrid(7*gridsize+2:9*gridsize-1,2*gridsize+2:4*gridsize-1,:)=MdiagReslr;
Mgrid(7*gridsize+2:9*gridsize-1,end-4*gridsize+2:end-2*gridsize-1,:)=MdiagRes;

%600 Lines
Wlines=600/NsampV*pi;
MDist=1/sqrt(2)*toeplitz(flipud((0:2*gridsize-3)'),(2*gridsize-3:4*gridsize-6));

MdiagRes=(1-cos(Wlines*MDist))/2;
MdiagReslr=fliplr(MdiagRes);
MdiagRes=repmat(MdiagRes,[1 1 3]);
MdiagReslr=repmat(MdiagReslr,[1 1 3]);

Mgrid(4*gridsize+2:6*gridsize-1,2*gridsize+2:4*gridsize-1,:)=MdiagRes;
Mgrid(4*gridsize+2:6*gridsize-1,end-4*gridsize+2:end-2*gridsize-1,:)=MdiagReslr;

%Generate mix matrix:
Mdist=sqrt(repmat(((1:sampV/2)'-0.5).^2,1,sampH/2)+repmat(((1:sampH/2)-0.5).^2,sampV/2,1));
Mdist=Mdist-Rcircle*gridsize;
Mmix=(sin(Mdist*pi/MixzoneCenter)+1)/2;
Moutside=Mdist>MixzoneCenter/2;
Minside=Mdist<-MixzoneCenter/2;
Mmix=Mmix.*(~Minside);
Mmix=Mmix.*(~Moutside)+Moutside;
Mmix=[fliplr(Mmix) Mmix];
Mmix=[flipud(Mmix); Mmix];
Mmix=repmat(Mmix,[1 1 3]);

%Center content:

% Top White:
TopWhite=ones(gridsize,gridsize*2*Rcircle);
TopWhite=repmat(TopWhite,[1 1 3]);

%White Black White:
WhiteBlackWhite=[ones(gridsize,gridsize*(Rcircle-2)) zeros(gridsize,gridsize*4) ones(gridsize,gridsize*(Rcircle-2))];
WhiteBlackWhite=repmat(WhiteBlackWhite,[1 1 3]);

%Black White Black with reflection test:
BWBTrans=[zeros(gridsize,gridsize*(Rcircle-3)) ones(gridsize,gridsize/2-1) zeros(gridsize,2) ones(gridsize,gridsize*5.5-1) zeros(gridsize,gridsize*(Rcircle-3))];
BWBTrans=repmat(BWBTrans,[1 1 3]);

%Squarewave bar:
Nblock=round(11/12*gridsize);
Squarewave=repmat([zeros(gridsize,Nblock) 0.75*ones(gridsize,Nblock)],[1 6 3]);
TempMargin=Rcircle*gridsize-Nblock*6;
Squarewave=[zeros(gridsize,TempMargin,3) Squarewave zeros(gridsize,TempMargin,3)];

%Colorbar:
YcolorBar=0.75*[[1 1 0];[0 1 1];[0 1 0];[1 0 1];[1 0 0];[0 0 1]];
Colorbar=[];
for j=1:size(YcolorBar,1),
   Colorbar=[Colorbar repmat(reshape(YcolorBar(j,:),[1 1 3]),2*gridsize,2*gridsize)];
end

MidVerLine=[zeros(gridsize,gridsize/2-1,3) ones(gridsize,2,3) zeros(gridsize,gridsize/2-1,3)];
Colorbar(gridsize+1:end,end/2-gridsize/2+1:end/2+gridsize/2,:)=MidVerLine;

%Centerbar:
MCross=zeros(gridsize/2);
MCross(1,:)=1;
MCross(:,1)=1;
MCross=[fliplr(MCross) MCross];
MCross=[flipud(MCross);MCross];
CenterBar=[];
for j=1:2*Rcircle,
   CenterBar=[CenterBar MCross];
end
CenterBar(:,end/2:end/2+1)=1;
CenterBar=Repmat(CenterBar,[1 1 3]);

%Sweep:
pludge=-0.04;
Wsweep=linspace(5/74.25*2*pi,30/74.25*2*pi,5);
Sweep=zeros(1,gridsize);
StartPhase=0;
for j=1:length(Wsweep),
   Sweep=[Sweep (1-cos((0:2*gridsize-1)*Wsweep(j)+StartPhase))/2];
   StartPhase=StartPhase+2*gridsize*Wsweep(j);
end
Sweep=[Sweep zeros(1,gridsize)];
Sweep=repmat(Sweep,[2*gridsize 1 3]);
Sweep(1:gridsize,end/2-gridsize/2+1:end/2+gridsize/2,:)=MidVerLine*(1-pludge)+pludge;

%GreyStep:
Ygrey=linspace(0,1,6);
GreyStep=[];
for j=1:length(Ygrey),
   GreyStep=[GreyStep repmat(Ygrey(j),[gridsize 2*gridsize 3])];
end

%WhiteBlackWhite Trans:
WBWTrans=1-BWBTrans;

%YellowRedYellow:
Yellow=0.75*[1 1 0];
Red=0.75*[1 0 0];
YRY=[repmat(reshape(Yellow,[1 1 3]),2*gridsize,5.5*gridsize) repmat(reshape(Red,[1 1 3]),2*gridsize,gridsize) repmat(reshape(Yellow,[1 1 3]),2*gridsize,5.5*gridsize)];

MCenter=[TopWhite;WhiteBlackWhite;BWBTrans;Squarewave;Colorbar;CenterBar;Sweep;GreyStep;WBWTrans;YRY];

%Expand center by dublicating edges:
CenterMargin=(sampH-2*Rcircle*gridsize)/2;

MCenter=[repmat(MCenter(:,1,:),1,CenterMargin) MCenter repmat(MCenter(:,end,:),1,CenterMargin)];

Mres=MCenter.*(1-Mmix)+Mgrid.*Mmix;

%chess border:
TopBar=[ones(marginV-1,1) repmat([zeros(marginV-1,gridsize-2) ones(marginV-1,gridsize+2)],1,(NgridH-1)/2) zeros(marginV-1,gridsize-2) ones(marginV-1,1)];
TopBar=[TopBar;ones(1,size(TopBar,2))];
BottomBar=flipud(TopBar);
TopBar=repmat(TopBar,[1 1 3]);
BottomBar=repmat(BottomBar,[1 1 3]);

Mres=[TopBar;Mres;BottomBar];

RightBar=[ones(marginV+1,marginH-1); repmat([zeros(gridsize-2,marginH-1); ones(gridsize+2,marginH-1)],(NgridV-1)/2,1); zeros(gridsize-2,marginH-1); ones(marginV+1,marginH-1)];
RightBar=[ones(NsampV,1) RightBar];
LeftBar=fliplr(RightBar);
RightBar=repmat(RightBar,[1 1 3]);
LeftBar=repmat(LeftBar,[1 1 3]);

Mres=[LeftBar Mres RightBar];

%Overscan Marks: (Must be done after adding the border)
Wlines=500/NsampV*pi;
Marks=[0.04 0.05 0.06]';

%Horizontal marks:
grey=0.48;
x=(1:gridsize+marginH-7)-0.5;
dist=min(abs(repmat(x,3,1)-repmat(Marks*NsampH/2,1,length(x))));
phase=dist*Wlines;
phase(find(phase>pi))=pi;
OverScan=(1+cos(phase))/2*(1-grey)+grey;
OverScanL=repmat(OverScan,[gridsize-2,1,3]);
OverScanR=repmat(fliplr(OverScan),[gridsize-2,1,3]);

Mres(end/2-gridsize/2+2:end/2+gridsize/2-1,1:size(OverScan,2),:)=OverScanL;
Mres(end/2-gridsize/2+2:end/2+gridsize/2-1,end-size(OverScan,2)+1:end,:)=OverScanR;

%Vertical marks:
x=(1:gridsize/2-MixzoneCenter+marginV-7)-0.5;
dist=min(abs(repmat(x,3,1)-repmat(Marks*NsampV/2,1,length(x))));
phase=dist*Wlines;
phase(find(phase>pi))=pi;
OverScan=(1+cos(phase))/2*(1-grey)+grey;
OverScanU=repmat(OverScan',[1,gridsize-2,3]);
OverScanL=repmat(flipud(OverScan'),[1,gridsize-2,3]);

Mres(1:size(OverScanU,1),end/2-gridsize/2+2:end/2+gridsize/2-1,:)=OverScanU;
Mres(end-size(OverScanL,1)+1:end,end/2-gridsize/2+2:end/2+gridsize/2-1,:)=OverScanL;

%image(Mres)
imwrite(Mres,[filename,'.bmp'],'bmp')

%Color space conversion SMPTE 274M-1995:
Yres=0.2126*Mres(:,:,1)+0.7152*Mres(:,:,2)+0.0722*Mres(:,:,3);
PBres=0.5/(1-0.0722)*(Mres(:,:,3)-Yres);
PRres=0.5/(1-0.2126)*(Mres(:,:,1)-Yres);

%Convert to binary words:
D=2^(10-8);
Yres=floor(219*D*Yres+16*D+0.5);
PBres=floor(224*D*PBres+128*D+0.5);
PRres=floor(224*D*PRres+128*D+0.5);

%Write binary data for ASTRO:
fileid=fopen([filename,'.bin'],'w','b');
fwrite(fileid,Yres','ubit16');
fwrite(fileid,PBres','ubit16');
fwrite(fileid,PRres','ubit16');

fclose(fileid);