function[out_array]= IIR_fil(in_array)
  temp_array=zeros(size(in_array,2),1);
  
  IIR_val=in_array(1);
  index=2;
  
  for index=2:size(in_array,2);
    IIR_val=(IIR_val-IIR_val/8 + in_array(index-1)/8 );
    temp_array(index)=IIR_val;
  end;
  
  out_array=temp_array;
endfunction
