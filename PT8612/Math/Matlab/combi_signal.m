spl=1920;
lpf=1080;


% Colorbar:
bar_width=spl/8;
bar_heigth=lpf/2;

rgb_bar=[[1 1 1];[1 1 0];[0 1 1];[0 1 0];[1 0 1];[1 0 0];[0 0 1];[0 0 0]];

R=[];
G=[];
B=[];
for j=1:size(rgb_bar,1),
    R=[R repmat(rgb_bar(j,1),bar_heigth,bar_width)];
    G=[G repmat(rgb_bar(j,2),bar_heigth,bar_width)];
    B=[B repmat(rgb_bar(j,3),bar_heigth,bar_width)];
end

% Luma Ramp:

ramp_heigth=lpf/6;
black_width=60;

luma=[zeros(1,black_width) linspace(0,1,spl-2*black_width) ones(1,black_width)];
size(repmat(luma,ramp_heigth,1))
R=[R;repmat(luma,ramp_heigth,1)];
G=[G;repmat(luma,ramp_heigth,1)];
B=[B;repmat(luma,ramp_heigth,1)];

% Clapper Board:

red_heigth=lpf/45;
board_heigth=lpf/45*13;
width_red=spl/3;

R=[R;ones(red_heigth,spl)];
G=[G;zeros(red_heigth,spl)];
B=[B;zeros(red_heigth,spl)];

R=[R;[ones(board_heigth,24) zeros(board_heigth,(spl-2*24-width_red)/2) ones(board_heigth,width_red) zeros(board_heigth,(spl-2*24-width_red)/2) ones(board_heigth,24)]];
G=[G;zeros(board_heigth,spl)];
B=[B;zeros(board_heigth,spl)];



R=[R;ones(red_heigth,spl)];
G=[G;zeros(red_heigth,spl)];
B=[B;zeros(red_heigth,spl)];

RGB=[];
RGB(:,:,1)=R;
RGB(:,:,2)=G;
RGB(:,:,3)=B;

size(RGB)
image(RGB)
imwrite(RGB,'combi_signal.bmp','BMP')