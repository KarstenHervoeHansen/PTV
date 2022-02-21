function [frame] = grey_scale_index(filename)

xrange=(24:522)';
yrange=(1:480)';

frame=imread(filename);
%frame=frame(yrange,xrange);

% keep specific index value range:
frame(find(frame>=64))=0;

%grayscale colormap:
frame=mod(frame,16);