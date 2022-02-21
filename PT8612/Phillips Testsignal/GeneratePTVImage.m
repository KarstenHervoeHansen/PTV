function GeneratePTVImage()

Filename='PTV';

YUV=LoadPTVTestFrame;

%Dimensions:
D1=size(YUV,1)
D2=size(YUV,2)
D3=size(YUV,3)

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

%imwrite(RGBs,Filename,'bmp');

h=ones(11)/11^2;
h=fspecial('gaussian',21,5);
h=1;
plotRGB(imfilter(RGBs,h))

function plotRGB(RGB)
image(RGB);
set(gca, ...
    'Visible', 'off', ...
    'Units', 'pixels', ...
    'Position', [50 50 size(RGB,2) size(RGB,1)]);
set(gcf, 'Units', 'pixels');
%position = get(gcf, 'Position');
set(gcf, 'Position', [100 100 size(RGB,2)+100 size(RGB,1)+100]);  



function plotRGB(RGB)
image(RGB);
set(gca, ...
    'Visible', 'off', ...
    'Units', 'pixels', ...
    'Position', [50 50 size(RGB,2) size(RGB,1)]);
set(gcf, 'Units', 'pixels');
%position = get(gcf, 'Position');
set(gcf, 'Position', [100 100 size(RGB,2)+100 size(RGB,1)+100]);  

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