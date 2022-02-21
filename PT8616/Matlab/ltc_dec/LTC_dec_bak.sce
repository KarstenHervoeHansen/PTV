close;
clear all;
clc

stacksize(10000000);

sync_word = [0 0 1 1 1 1 1 1 1 1 1 1 1 1 0 1];
lower_tresh = 5;
period = 12;
"load"
wav = wavread("short.wav");
wav = wav>0;

"decode biphase"
num_samps=size(wav,2)-100;
bitstream=zeros(1, num_samps/(160*9*30));

count=0; index=1; streamindex=1;

//fortolk skift
while(index<num_samps) do

  count = 0;
  
  //find næste transition
  while((wav(index)==wav(index+1) & index<num_samps) | (count<lower_tresh))
   index=index+1;
   count=count+1;
  end;
  
  index=index+1;
  //hvis 0
  if(count>period)
    bitstream(streamindex)=0;
  //hvis 1
  else 
    count=0;
    //find næste transition
    while((wav(index)==wav(index+1) & index<num_samps) | (count<lower_tresh))
       index=index+1;
       count=count+1;
    end;
    index=index+1;
    bitstream(streamindex)=1;
  end;
    
  streamindex=streamindex+1;
end;


"scan bitstream"

index = 1;
count = 1;
index2 = 1;
LTC_frame = zeros(1:80);
num_samps=size(bitstream,2);

//åben fil til skrivning
fil=file('open','fil.txt','unknown')
frame=0;
hour=0;
minut=0;
sec=0;
prev_frame=0;

while(index<(num_samps-80)) do
  
  //hvis start på sync ord, test
  if(bitstream(index)==0)
    index2=index;
    count=1;
    
    while (bitstream(index2)==sync_word(count) & count<16) do
      count=count+1;
      index2=index2+1;
    end;
    
     //hvis fundet syncword
    if(count==16)
      count = 1;
      index=index2+1;
      LTC_frame = bitstream(index:index+80);
      frame=LTC_frame(1)+LTC_frame(2)*2+LTC_frame(3)*4+LTC_frame(4)*8+LTC_frame(9)*10+LTC_frame(10)*20;
      sec=LTC_frame(17)+LTC_frame(18)*2+LTC_frame(19)*4+LTC_frame(20)*8+LTC_frame(25)*10+LTC_frame(26)*20+LTC_frame(27)*40;
      minut=LTC_frame(33)+LTC_frame(34)*2+LTC_frame(35)*4+LTC_frame(36)*8+LTC_frame(41)*10+LTC_frame(42)*20+LTC_frame(43)*40;
      hour=LTC_frame(49)+LTC_frame(50)*2+LTC_frame(51)*4+LTC_frame(52)*8+LTC_frame(57)*10+LTC_frame(58)*20;
     
           
      
      if (frame ~= (prev_frame+1))
        fprintf(fil, 'Time: %02d:%02d:%02d:%02d*\n', hour, minut, sec, frame);
      else
        fprintf(fil, 'Time: %02d:%02d:%02d:%02d\n', hour, minut, sec, frame);
      end;
      
      prev_frame=frame;
      
    end;
  end;
  
  index = index + 1;
end;
file('close',fil )
"done"

