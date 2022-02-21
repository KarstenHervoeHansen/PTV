function Imagescan()

scan_dir_path = uigetdir('Select scan directory...');
%scan_dir_path ='\\vmware-host\Shared Folders\Temp\GPSDEBUG\Week16';
%ref_file=     '\\vmware-host\Shared Folders\Temp\GPSDEBUG\Week16\23\130419_144723.png'

ref_frame=int8(zeros(480,640));

scan_dir=dir(scan_dir_path);
l=length(scan_dir);

%Go through sub-folders:

file_id = fopen([scan_dir_path,'\','image_changes.txt'],'w');
fprintf(file_id,'Scan directory: "%s"\n',scan_dir_path);
fprintf(file_id,'Scan start: %s\n\n',datestr(clock));

hwait = waitbar(0,'Scanning images...');
Nsubfolders=length(scan_dir)-2;
Nfile=1;
prev_difference=0;
for i=1:Nsubfolders,
   if scan_dir(i+2).isdir == 1
      scan_sub_dir_path=[scan_dir_path,'\',scan_dir(i+2).name];
      scan_sub_dir=dir(scan_sub_dir_path);
      %Go through fiiles in sub-folders:
      Nfilesinsub=length(scan_sub_dir)-2;
      for j=1:Nfilesinsub,
         image_filename=[scan_sub_dir_path,'\',scan_sub_dir(j+2).name];
         if sum(image_filename(end-3:end)=='.PNG')==4,
            test_frame=int8(grey_scale_index(image_filename));
            diff_frame=test_frame-ref_frame;
            difference=sum(sum(abs(diff_frame)));
            image_diff(Nfile) = difference;
            if difference>max([1.30*prev_difference 100]),
               text_string=sprintf('%7.0d: "%s"',Nfile,image_filename);
               fprintf(file_id,'%s\n',text_string);
            end    
            ref_frame=test_frame;
            Nfile=Nfile+1;
            prev_difference=difference;
            waitbar(((i-1)+(j-1)/Nfilesinsub)/Nsubfolders,hwait);
         end
      end
   end
end
close(hwait);

fclose(file_id);

%Display difference image:

%Plots:
figure(1)
plot(image_diff);

save([scan_dir_path,'\','image_diff.mat'],'image_diff')

% ctable=[ones(32,3);zeros(256-32,3)];
% ctable(17,:)=0;
% 
% figure(4)
% image(test_frame);
% colormap(ctable);