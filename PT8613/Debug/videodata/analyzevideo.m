% Load and analyze video data:


filename='data.bin';
datasize=100000;




fid = fopen(filename,'r');

data = fread(fid,datasize,'uint16');

fclose(fid);

data=reshape(data,4,length(data)/4);
%data=reshape(data,2,length(data)/2);

chromaA = data(1,1:end)';
lumaA   = data(2,1:end)';
%chromaB = data(3,1:end)';
%lumaB   = data(4,1:end)';

clear data


% Find system:
subdata=chromaA(1:min([1000000 length(chromaA)]));

%in XYZ words:
h = dec2bin(subdata,10);
h = h(:,4);

%  Find EAV:
eav  = (subdata(1:end-3) == 1023) & (subdata(2:end-2) == 0) & (subdata(3:end-1) == 0);% & h(4:end);
ieav = find(eav);
%clear eav;

%  Find SAV:
sav  = (subdata(1:end-3) == 1023) & (subdata(2:end-2) == 0) & (subdata(3:end-1) == 0) & (h(4:end) == 0);
isav = find(sav);
%clear sav;

ieav
isav



