clc, clear all;
%TLC_generator

frame=0;
second=55;
minute=58;
hour=23;

drop_frame_flag=0;
color_frame_flag=0;
BGF0=0;
BGF1=0;
BGF2=0;
sync_word=[0 0 1 1 1 1 1 1 1 1 1 1 1 1 0 1];
polarity_correction=0;
BG=[0 0 0 0];

fase=0;
run_time=2000;

sample_rate=44100;

wave_count=1;
wave=zeros(run_time*80*500/1000000*sample_rate, 1);

for frame_nr = 1:run_time
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % generer tid for 25 FPS
    frame=frame+1;
    if(frame==25)
        second=second+1;
        frame=0;
    end;
        
    if(second==60)
        minute=minute+1;
        second=0;
    end;
        
    if(minute==60)
        hour=hour+1;
        minute=0;
    end;
       
    if(hour==24)
        hour=0;
    end;
    
    %display tid
    [hour minute second frame];
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % generer frame ord
    frame_units=de2bi(mod(frame,10),4);
    frame_tens=de2bi( (frame-mod(frame,10))/10, 2);
    
    second_units=de2bi(mod(second,10),4);
    second_tens=de2bi( (second-mod(second,10))/10, 3);
    
    minute_units=de2bi(mod(minute,10),4);
    minute_tens=de2bi( (minute-mod(minute,10))/10, 3);
    
    hour_units=de2bi(mod(hour,10),4);
    hour_tens=de2bi( (hour-mod(hour,10))/10, 2);
    
    LTC_word=[frame_units BG frame_tens 0 color_frame_flag BG second_units BG second_tens BGF0 BG minute_units BG minute_tens BGF2 BG hour_units BG hour_tens BGF1 0 BG sync_word];
   
    [hour_tens hour_units]
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % generer LTC wave
    % polarity bit
    LTC_word(60)=~mod(sum(LTC_word(1:64)),2);
    
    for bit_count=1:80
        fase=~fase;
        for period_count=1:22
            if (LTC_word(bit_count)==1) && (period_count==round(250/1000000*sample_rate))
                fase=~fase;
            end;
            
            wave(wave_count)=fase;
            wave_count=wave_count+1;
        end;
    end;
end;

wave=wave-0.5;
filter_size=4;
filtered_wave=filter(ones(1,filter_size)/filter_size,1,wave);

wavwrite(filtered_wave, sample_rate, 8, 'out_wave1.wav');
%plot(filtered_wave(1:1000));
sound(filtered_wave, sample_rate);