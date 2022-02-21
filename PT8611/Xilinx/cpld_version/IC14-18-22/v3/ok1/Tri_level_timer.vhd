library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--------------------------------------------------------------------------
-- v1 :     Original kode til Prototype lavet af studerende Jesper Christoffersen 
--          for DK-Audio, som afgangsprojekt november 2002 til marts 2003
-- v2:      19/6 2003 - PS      tilfoejet clamping- og enablesignaler til rekonstruktionsfiltre
-- v3:      23/5 2003 - PS      nyt hieraki => toplevel = Tri_level_sync_generator.vhd
--------------------------------------------------------------------------

entity Tri_level_timer is
   Port ( 
        reset : in std_logic;       -- Master Reset fra mainboard
        clk   : in std_logic;       -- Output fra clock multiplier ( 148 MHz )
        clk_out : out std_logic; -- clocksignal til konverteren ( clk inverteret )
            
        IC5_S0  : out std_logic; -- styresignaler til clock synthesizerne
        IC5_S1  : out std_logic;
        IC5_S2  : out std_logic;
        IC5_S3  : out std_logic;

        IC6_S0  : out std_logic;
        IC6_S1  : out std_logic;
        IC6_S2  : out std_logic;
        IC6_S3  : out std_logic;
          
        sync           : out std_logic; -- styresignal til konverteren
        sync_t         : out std_logic; -- styresignal til konverteren
        blank          : out std_logic; -- styresignal til konverteren (altid nul)

--------- rekonstruktionsfiltre -------------------------------------------

        sync_in_1 : out std_logic;      -- aktiverer clamping paa filter output
        sync_in_2 : out std_logic;      -- aktiverer clamping paa filter output
        disable_1 : inout std_logic;    -- filter 1 disable ( aktiv hoej )
        disable_2 : out std_logic;       -- filter 2 disable ( aktiv hoej )

--------- test pins ----------------------------------------------

        frame : out std_logic;          -- frame clock (til test formaal)
        line_freq : out std_logic       -- line clock  (til test formaal)
          
        );
end Tri_level_timer;


architecture Behavioral of Tri_level_timer is

component Tilstandsmaskine
    Port (  reset          : in std_logic;
            clk            : in std_logic;
            command        : in integer range 0 to 2; -- starter tilstandsmaskinen
            cnt_setup      : in integer range 0 to 1023; -- signal til 

            sync       : out std_logic; -- styresignal til konverteren
            sync_t     : out std_logic; -- styresignal til konverteren
            blank      : out std_logic  -- styresignal til konverteren
         );
end component;


signal cnt_setup        : integer range 0 to 1023; -- signal that loads the 'loadable down counter'
signal command          : integer range 0 to 2; -- Signal that triggers the state machine

signal pixel      : integer range 0 to 8191 :=  0;  -- interne signaler
signal line       : integer range 0 to 4095 :=  0;  
signal l          : std_logic               := '0'; 
signal div_11     : std_logic; -- 
signal div        : integer range 0 to 15;
signal pixel_4399 : std_logic;
signal f          : std_logic;

signal no_of_pulses     : std_logic;            -- hjaelpesignal 1
signal pulse_type       : integer range 1 to 2; -- hjaelpesignal 2
signal line_begin_pulse : std_logic;            -- hjaelpesignal 3
signal line_mid_pulse   : std_logic;            -- hjaelpesignal 4

signal filter_1_valgt : std_logic := '1'; -- lav for outputfilter 1 aktiv, hoej for outputfilter 2


begin

statemachine : Tilstandsmaskine port map ( reset     => reset,
                                           clk       => clk,
                                           command   => command,
                                           sync      => sync,
                                           sync_t    => sync_t,
                                           blank     => blank,
                                           cnt_setup => cnt_setup );
                                       
make_pixel_counter : process(reset,clk)
begin
   if reset = '1' then
      pixel <= 0;
      l     <= '1'; -- det er dette signal der driver linjetaelleren
   elsif (clk'event and clk = '1') then
		-- if pixel /= pix then
		if pixel /= 4399 then -- saa laenge der ikke er talt til 2199 saa fortsaet
			pixel <= (pixel + 1);
         if (pixel >= 2048) and (pixel <= 4095) then -- paa denne maade behves der kun at blive laest paa en bit. Dette reducerer produkt termerne for signalet 'pixel' drastisk!
			    l <= '0'; -- signalet l saettes lav naar pixel overstiger vaerdien 1024
         end if;
		else  
			l <= '1'; -- signalet l saettes hoejt naar pixel har vaerdien 2199
			pixel <= 0; -- taelleren starter forfra
		end if;
	end if;		
	  
end process;


make_line_counter : process(reset,l)
	begin
      if reset = '1' then
          line <= 0;
      elsif l'event and l = '1' then
			if line /= 1124 then
				line <= (line+1);
            if (line >= 512) and (line <= 1023) then
               f <= '0'; -- lav frame clock
            end if;
         else
				line <= 0;
            f <= '1'; -- lav frame clock
			end if;
		end if;
end process;


make_clk_11 : process(reset,clk) -- neddeler clocken til 1/11, fordi 11 gaar op i 
begin

   if reset = '1' then
       pixel_4399 <= '0';
   elsif clk'event and clk = '1' then 
       if pixel = 4398 then   -- sikrer at 'pixel' og 'div_11' er synkrone
           pixel_4399 <= '1';
       else
           pixel_4399 <= '0';
       end if;
   end if;

   if reset = '1' then
      div <= 0;
      div_11 <= '0';
   elsif clk'event and clk = '1' then 
      if (div /= 10) and (pixel_4399 = '0') then
         div <= div + 1;
         if div = 5 then
             div_11 <= '1';
         end if;
      else
         div <= 0;
         div_11 <= '0';
      end if;
   end if;
end process;

      

make_state_command : process(reset, div_11) -- starter statemaskinen
begin
   if reset = '1' then
      command <= 0;
   elsif div_11'event and div_11 = '0' then
       if line_begin_pulse = '1' then -- laver synkpuls i starten af en linje
           command <= Pulse_type;
       elsif (line_mid_pulse = '1') and (no_of_pulses = '1') then
           command <= Pulse_type; -- laver synkpuls midt paa en linje (field synk)
       else
           command <= 0;
       end if;
   end if;
end process;


make_no_of_pulses : process(reset, div_11) -- hjaelpesignal 1: fortaeller om der er 1 eller 2 pulser/linje
begin
   if reset = '1' then
      no_of_pulses <= '1';
   elsif div_11'event and div_11 = '0' then
      case line is
      when 0 =>
         no_of_pulses <= '1'; -- 2 pulser/linje
      when 6 =>
         no_of_pulses <= '0'; -- 1 puls/linje
      when 562 =>
         no_of_pulses <= '1'; -- 2 pulser/linje
      when 568 =>
         no_of_pulses <= '0'; -- 1 puls/linje
      when others =>
         null;
      end case;
   end if;
end process;


make_Pulse_type : process(reset, div_11) -- hjaelpesignal 2: fortaeller hvilken pulstype der skal laves
begin
   if reset = '1' then
      Pulse_type <= 1;
   elsif div_11'event and div_11 = '0' then
      case line is
         when 1124 => 
            Pulse_type <= 1; -- pulstype 1 fra linje 1125 til linje 5
         when    4 => 
            if (pixel >= 1024) and (pixel <= 2047) then -- sidste puls paa linje 5 er en type 2
               Pulse_type <= 2;
            end if;
         when  562 => -- type 2 pulser hele vejen til linje 562
            Pulse_type <= 1; 
         when  567 => -- type 1 pulser fra linje 563 til 568
            Pulse_type <= 2;
         when others => 
            null;
       end case;
   end if;
end process;


make_line_begin_pulse : process(reset, clk) -- hjaelpesignal 3: identificerer tidspunktet, hvor foerste puls startes (sker paa alle linjer)
begin
   if reset = '1' then
      line_begin_pulse <= '0';
   elsif clk'event and clk = '1' then
      case pixel is
         when 4223 =>  -- 2*g-h = 2*1100-1-88 (forsinket 88 clockcycles pga statemaskinen foerst skal lave pos & neg puls foer den naar til nul-gennemgang)
            line_begin_pulse <= '1';
         when 4245 => -- signalet holdes i 11 clock cycles pga. den neddelte clock i "make_state_command" processen
            line_begin_pulse <= '0';
         when others =>
            null;
      end case;
   end if;
end process;


make_line_mid_pulse : process(reset, clk) -- hjaelpesignal 4: identificerer tidspunktet, hvor midterste puls paa en linje startes (sker kun paa nogle linjer, defineret af signalet no_of_pulses)
begin
   if reset = '1' then
      line_mid_pulse <= '0';
   elsif clk'event and clk = '1' then
      case pixel is
         when 2023 => -- g-h = 1099-88 = 1011
            line_mid_pulse <= '1';
         when 2045 => -- signalet holdes i 11 clock cycles pga. den neddelte clock i "make_state_command" processen
            line_mid_pulse <= '0';
         when others =>
            null;
      end case;
   end if;
end process;

make_converter_clock : process(clk)
begin
   clk_out <= not clk; -- soerger for at data til konverteren clockes ind 1/2 clock cycle senere end det clockes ud af CPLD'en
   frame <= f; -- frame clock
   line_freq <= l; -- line clock
   cnt_setup <= 173; -- saetter formatet til 1980X1080/60/2:1

   IC5_S0 <= '0'; -- saetter IC5 
   IC5_S1 <= '1'; -- til at lave 
   IC5_S2 <= '0'; -- 74,25 MHz
   IC5_S3 <= '0';

   IC6_S0 <= '1'; -- saetter IC6
   IC6_S1 <= '1'; -- til at gange
   IC6_S2 <= '1'; -- med faktor 2
   IC6_S3 <= '1';
end process;   


Sync_In_1 <= no_of_pulses;  -- Ved field sync og frame sync clampes filtrenes udgange for 
Sync_In_2 <= no_of_pulses;  -- at modvirke DC paa Syncsignalet
                            -- Denne metode er kun brugbar ved interlaced video, da der
                            -- ved progressiv scanning kun er een tri-level-puls pr linie

vaelg_filter :  Disable_1 <= filter_1_valgt;
                Disable_2 <= not Disable_1;


end Behavioral;
