function[]= df_count(run_frames)
  hours=0; mins=0; secs=0; frames=0;
  secs_since_sync=0;
  for count=0:run_frames; 
    frames=frames+1;
    
    if (frames==30)
      mprintf('Time: %02d:%02d:%02d:%02d %d\n\t', hours, mins, secs, frames, frame_skip(secs_since_sync));
   
      frames=0;
      secs=secs+1;
      secs_since_sync=secs_since_sync+1;
      if(secs==60)
        secs=0;
        mins=mins+1;
        
        if(mins==60)
          mins=0;
          hours=hours+1;
          
          if(hours==24)
            hours=0;
          end;
        end;
      end;
      
      if(secs==0)
        if(modulo(mins, 10))
          frames=2;
          mprintf('---------------------------------Frame dropped !------------------------------------ \n\t');
        end;
      end;
    end;
  end;
endfunction
