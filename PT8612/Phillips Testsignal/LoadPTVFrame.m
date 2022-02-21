function mainfunction()

YUV=LoadPTVTestFrame;

%Dimensions:
D1=size(YUV,1);
D2=size(YUV,2);
D3=size(YUV,3);

%Make YUV columns:
YUVCol=reshape(reshape(shiftdim(YUV,2),[D1*D2*D3 1])',[D3 D1*D2])';

%Scale YUV:%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
YUVCols(:,1)=(YUVCol(:,1)-64)/876;
YUVCols(:,2)=YUVCol(:,2)/(2^10-1);
YUVCols(:,3)=YUVCol(:,3)/(2^10-1);

%Transform to RGB:
RGBCols=ycbcr2rgb(YUVCols);

%Make RGB Matrix:
RGBs=reshape(RGBCols',[D3 D1 D2]);
RGBs=shiftdim(RGBs,1);

%Plot Frame:
figure(1)
image(RGBs)
set(gca,'PlotBoxAspectRatio',[4 3 1]);

%Plot Horizontal Waveforms:
WFLines=830:830;
figure(2)
subplot(3,1,1);
plot(YUV(WFLines,:,1)','-*b')
title('Horizontal Waveforms')
ylabel('Y')
subplot(3,1,2);
plot(YUV(WFLines,:,2)','-*b')
ylabel('Cb')
subplot(3,1,3);
plot(YUV(WFLines,:,3)','-*b')
ylabel('Cr')

%Plot Vertical Waveforms:
WFColumns=50:50;
figure(3)
subplot(3,1,1);
plot(YUV(:,WFColumns,1)','-*b')
title('Vertical Waveforms')
ylabel('Y')
subplot(3,1,2);
plot(YUV(:,WFColumns,2)','-*b')
ylabel('Cb')
subplot(3,1,3);
plot(YUV(:,WFColumns,3)','-*b')
ylabel('Cr')

%Plot Histograms:
HistRange=[1 size(YUV,2) 1 size(YUV,1)];
YUVHist=YUV(HistRange(3):HistRange(4),HistRange(1):HistRange(2));
YUVCol=reshape(reshape(shiftdim(YUV,2),[D1*D2*D3 1])',[D3 D1*D2])'; %Make YUV columns:
Bins=1:2^10-1;

figure(4)
subplot(3,1,1);
Yhist = hist(YUVCol(:,1),Bins);
bar(Bins,Yhist)
subplot(3,1,2);
Yhist = hist(YUVCol(:,2),Bins);
bar(Bins,Yhist)
subplot(3,1,3);
Yhist = hist(YUVCol(:,3),Bins);
bar(Bins,Yhist)


function YUV=LoadPTVTestFrame()

FileNameStart='CHANNEL';

NSampPerLine=1024;     %Sampels per line
StartLine=230;         %First line number
NLines=1152;           %Number of lines in frame
ISAV=17;               %Index for start of active video in line
IEAV=ISAV+720-1;       %Index for end of active video in line

%Load Y data:
filename=[FileNameStart,'1.dat'];
fid=fopen(filename);
[Y,Nsamples]=fread(fid,inf,'int16');
fclose(fid);
Y=reshape(Y,NSampPerLine,Nsamples/NSampPerLine)';
Y=Y(StartLine:StartLine+NLines-1,ISAV:IEAV);

%Load U data:
filename=[FileNameStart,'2.dat'];
fid=fopen(filename);
[U,Nsamples]=fread(fid,inf,'int16');
fclose(fid);
U=reshape(U,NSampPerLine,Nsamples/NSampPerLine)';
U=U(StartLine:StartLine+NLines-1,ISAV:IEAV);

%Load V data:
filename=[FileNameStart,'3.dat'];
fid=fopen(filename);
[V,Nsamples]=fread(fid,inf,'int16');
fclose(fid);
V=reshape(V,NSampPerLine,Nsamples/NSampPerLine)';
V=V(StartLine:StartLine+NLines-1,ISAV:IEAV);

%Collect Y,U and V:
YUV(:,:,1)=Y;
YUV(:,:,2)=U;
YUV(:,:,3)=V;

%Combine odd and even lines:
YUVComb(1:2:NLines,:,:)=YUV(1:NLines/2,:,:);
YUVComb(2:2:NLines,:,:)=YUV(NLines/2+1:end,:,:);
YUV=YUVComb;