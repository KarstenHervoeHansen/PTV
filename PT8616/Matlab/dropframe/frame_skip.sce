function[skipped_out]= frame_skip(secs_since_sync)
  skipped_frames=0;
  
  //skipped frames uger
	skipped_frames = floor(secs_since_sync / 86400) * 2592;  //2592 = 24 timer x 108 frames
	secs_since_sync = secs_since_sync - floor(secs_since_sync / 86400)*86400;
			
	//skipped frames timer
	skipped_frames = skipped_frames + floor(secs_since_sync / 3600) * 108;
	secs_since_sync = secs_since_sync - floor(secs_since_sync / 3600)*3600;
	
	//skipped frames minutter
	skipped_frames = skipped_frames + floor(secs_since_sync / 60) * 2;
	skipped_frames = skipped_frames - floor(secs_since_sync / 600) * 2; //spring 10+20+30+40+50 over
	secs_since_sync = secs_since_sync - floor(secs_since_sync / 60)*60;

  skipped_out = skipped_frames
endfunction
