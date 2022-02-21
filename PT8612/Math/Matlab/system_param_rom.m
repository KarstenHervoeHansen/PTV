% system label  system  index

%
%
%
%
%
%
%
%
%
%
%
%
%
%
%
%
%
delay_adjust = -7;

hb1s =      [  82-delay_adjust         %  1080i/30        
               82-delay_adjust         %  1080i/29.97     
               522-delay_adjust        %  1080i/25
               82-delay_adjust         %  1080p/30        
               82-delay_adjust         %  1080p/29.97     
               522-delay_adjust        %  1080p/25        
               632-delay_adjust        %  1080p/24        
               632-delay_adjust        %  1080p/23.98     
               104-delay_adjust        %  720p/60         
               104-delay_adjust        %  720p/59.94      
               434-delay_adjust        %  720p/50         
               1754-delay_adjust       %  720p/30         
               1754-delay_adjust       %  720p/29.97      
               2414-delay_adjust       %  720p/25         
               2579-delay_adjust       %  720p/24         
               2579-delay_adjust       %  720p/23.98      
               14-delay_adjust         %  NTSC 525i/29.97 
               10-delay_adjust ];      %  PAL 625i/25     

            
interlaced = [ 1                       %  1080i/30       
               1                       %  1080i/29.97    
               1                       %  1080i/25       
               0                       %  1080p/30       
               0                       %  1080p/29.97    
               0                       %  1080p/25       
               0                       %  1080p/24       
               0                       %  1080p/23.98    
               0                       %  720p/60        
               0                       %  720p/59.94     
               0                       %  720p/50        
               0                       %  720p/30        
               0                       %  720p/29.97     
               0                       %  720p/25        
               0                       %  720p/24        
               0                       %  720p/23.98     
               1                       %  NTSC 525i/29.97
               1 ];                    %  PAL 625i/25    
               
               
lpf =        [ 1125                    %  1080i/30       
               1125                    %  1080i/29.97    
               1125                    %  1080i/25       
               1125                    %  1080p/30       
               1125                    %  1080p/29.97    
               1125                    %  1080p/25       
               1125                    %  1080p/24       
               1125                    %  1080p/23.98    
               750                     %  720p/60        
               750                     %  720p/59.94     
               750                     %  720p/50        
               750                     %  720p/30        
               750                     %  720p/29.97     
               750                     %  720p/25        
               750                     %  720p/24        
               750                     %  720p/23.98     
               525                     %  NTSC 525i/29.97
               625 ];                  %  PAL 625i/25    
               

line_start = [ 82-delay_adjust         %  1080i/30        
               82-delay_adjust         %  1080i/29.97     
               522-delay_adjust        %  1080i/25
               82-delay_adjust         %  1080p/30        
               82-delay_adjust         %  1080p/29.97     
               522-delay_adjust        %  1080p/25        
               632-delay_adjust        %  1080p/24        
               632-delay_adjust        %  1080p/23.98     
               104-delay_adjust        %  720p/60         
               104-delay_adjust        %  720p/59.94      
               434-delay_adjust        %  720p/50         
               1754-delay_adjust       %  720p/30         
               1754-delay_adjust       %  720p/29.97      
               2414-delay_adjust       %  720p/25         
               2579-delay_adjust       %  720p/24         
               2579-delay_adjust       %  720p/23.98      
               14-delay_adjust         %  NTSC 525i/29.97 
               10-delay_adjust ];      %  PAL 625i/25     
                              
               
               
               
               