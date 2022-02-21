

{ get_light_box_data:
  This procedure reads light box DAC values from file LIGHT_BOX_FILE_NAME
  into CalLightLevel
}  
procedure get_light_box_data;
  var
     lbfile     : text;
     lbbuffer   : str_70;
     n, k       : byte;
     code       : integer;
     
  begin
   Assign(lbfile, LIGHT_BOX_FILE_NAME);
 {$I-}
   Reset(lbfile);
 {$I+}
   if IOResult <> 0 then
     begin
     Writeln(Bell, LIGHT_BOX_FILE_NAME,' not found');
     Writeln('Program terminated - press a key');
     WaitForAnyKey;
     AfslutProgram := true;
     Halt(1);
     end;
  for k := 1 to 2 do
    begin
    for n := 1 to 5 do
      begin
      Readln(lbfile, lbbuffer);
      if lbbuffer[1] = ';' then
        begin
        n := n - 1;
        continue;
        end;
      Val(lbbuffer, CalLightLevel[k, n], code);
      if code <> 0 then
        begin
        Writeln(Bell, LIGHT_BOX_FILE_NAME,' :format error.');
        Writeln('Program terminated - press a key');
        WaitForAnyKey;
        AfslutProgram := true;
        Halt(1);
        end;
      end;
    end;
  end;
