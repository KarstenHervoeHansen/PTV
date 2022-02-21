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
        reset : in std_logic;       -- start generator pulse - aktiv lav
        f148   : in std_logic;       -- Output fra clock multiplier ( 148 MHz )
        f148_inv : out std_logic; -- clocksignal til konverteren ( f148 inverteret )

        scan_method :       in std_logic;                 -- ( 0=interlaced, 1=progressiv )
        cnt_setup : in integer range 0 to 1023;   -- vaerdi til valg af Video standard        
        samples_pr_line : in integer range 0 to 8191;                  
        lines_pr_frame :    in integer range 0 to 2047;                  
            
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
        line_freq : out std_logic;       -- line clock  (til test formaal)
        State :     out integer range 0 to 15   -- for debug only
  
        );
end Tri_level_timer;


architecture Behavioral of Tri_level_timer is

component Tilstandsmaskine
    Port (  reset :         in std_logic;
            f148 :          in std_logic;
            command :       in integer range 0 to 2; -- starter tilstandsmaskinen

            scan_method :   in std_logic;            -- ( 0=interlaced, 1=progressiv )
            cnt_setup :     in integer range 0 to 1023; -- signal til 

            sync :          out std_logic; -- styresignal til konverteren
            sync_t :        out std_logic; -- styresignal til konverteren
            blank :         out std_logic;  -- styresignal til konverteren

            state :         out integer range 0 to 15 -- for debug only

         );
end component;


signal command          : integer range 0 to 2; -- Signal that triggers the state machine

signal pixel      : integer range 0 to 8191 :=  0;  -- interne signaler
signal last_pixel : std_logic;
signal line       : integer range 0 to 4095 :=  0;  
signal new_line   : std_logic               := '0'; -- hoejtgaaende signalerer ny linie
signal div_11     : std_logic; --           
signal div        : integer range 0 to 15;
signal new_frame  : std_logic;                  -- hoejtgaaende flanke signalerer ny frame

signal no_of_pulses     : std_logic;            -- hjaelpesignal 1
signal pulse_type       : integer range 1 to 2; -- hjaelpesignal 2
signal line_begin_pulse : std_logic;            -- hjaelpesignal 3
signal line_mid_pulse   : std_logic;            -- hjaelpesignal 4

signal filter_1_valgt   : std_logic := '1'; -- lav for outputfilter 1 aktiv, hoej for outputfilter 2


begin

statemachine : Tilstandsmaskine port map ( 
    reset       => reset,
    f148        => f148,
    command     => command,
    sync        => sync,
    sync_t      => sync_t,
    blank       => blank,
    scan_method => scan_method,
    cnt_setup   => cnt_setup, 

    state       => state -- for debug only

    );
                                       
make_pixel_counter : process(reset,f148)
begin
    if reset = '0' then
        pixel <= 0;
        new_line     <= '1'; -- det er dette signal der driver linjetaelleren
    elsif (f148'event and f148 = '1') then
		-- if pixel /= pix then
		if pixel /= ( samples_pr_line - 1 ) then
        --		if pixel /= 4399 then               -- original
	        pixel <= (pixel + 1);
            if (pixel >= 2048) and (pixel <= 4095) then -- paa denne maade behves der kun at blive laest paa en bit. Dette reducerer produkt termerne for signalet 'pixel' drastisk!
		       new_line <= '0'; -- signalet new_line saettes lav naar pixel overstiger vaerdien 1024
            end if;
		else  
		    new_line <= '1'; -- signalet new_line saettes hoejt naar pixel har vaerdien ( samples_pr_line - 1 )
		    pixel <= 0; -- taelleren starter forfra
		end if;
	end if;		
	  
end process;


make_line_counter : process(reset,new_line)
begin
    if reset = '0' then
        line <= 0;
    elsif new_line'event and new_line = '1' then
        if line /= (lines_pr_frame - 1) then
		    line <= (line+1);
            if (line >= 512) and (line <= 1023) then
                new_frame <= '0';
            end if;
        else
		    line <= 0;
            new_frame <= '1';       -- indikerer ny frame
		end if;
	end if;
end process;


make_f148_div11 : process(reset,f148) -- taeller 11 samples ad gangen, da 11 gaar op i antal samples/linier
begin

   if reset = '0' then
       last_pixel <= '0';
   elsif f148'event and f148 = '1' then 
--       if pixel = 4398 then   -- sikrer at 'pixel' og 'div_11' er synkrone ( original )
       if pixel = ( samples_pr_line - 2 ) then   -- sikrer at 'pixel' og 'div_11' er synkrone
           last_pixel <= '1';
       else
           last_pixel <= '0';
       end if;
   end if;

   if reset = '0' then
      div <= 0;
      div_11 <= '0';
   elsif f148'event and f148 = '1' then 
      if (div /= 10) and (last_pixel = '0') then
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
    if reset = '0' then
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
    if reset = '0' then
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


-- fejl i original kode mht, paa hvilke linier, der skal laves hvilke pulsetyper
-- => ny make_Pulse_type
make_Pulse_type : process(reset, div_11) -- hjaelpesignal 2: fortaeller hvilken pulstype der skal laves
begin
    if reset = '0' then
        Pulse_type <= 1;
    elsif div_11'event and div_11 = '0' then
        case line is
            when 1124 => 
                if (pixel >= 2047) then
                    Pulse_type <= 1; 
                end if;
            when 4 => 
                if (pixel >= 2047) then
                    Pulse_type <= 2;
                end if;
            when 562 => -- type 1 pulser fra midt paa linie 563 til midt paa linie 568
                if (pixel >= 1024) and (pixel <= 2047) then
                    Pulse_type <= 1; 
                end if;
            when 567 => -- type 2 pulser fra linje 563 til 568
                if (pixel >= 1024) and (pixel <= 2047) then
                    Pulse_type <= 2; 
                end if;
            when others => 
                null;
        end case;
    end if;
end process;


make_line_begin_pulse : process(reset, f148) -- hjaelpesignal 3: identificerer tidspunktet, hvor foerste puls startes (sker paa alle linjer)
begin
    if reset = '0' then
        line_begin_pulse <= '0';
    elsif f148'event and f148 = '1' then
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


make_line_mid_pulse : process(reset, f148) -- hjaelpesignal 4: identificerer tidspunktet, hvor midterste puls paa en linje startes (sker kun paa nogle linjer, defineret af signalet no_of_pulses)
begin
    if reset = '0' then
        line_mid_pulse <= '0';
    elsif f148'event and f148 = '1' then
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

f148_inv <= not f148;             -- soerger for at data til konverteren clockes ind 1/2 clock cycle senere end det clockes ud af CPLD'en
frame <= new_frame;             -- frame clock til testpin frame
line_freq <= new_line;          -- line clock til testpin line_freq

Sync_In_1 <= not no_of_pulses;  -- Ved field sync og frame sync clampes filtrenes udgange for 
Sync_In_2 <= not no_of_pulses;  -- at modvirke DC paa Syncsignalet
                                -- Denne metode er kun brugbar ved interlaced video, da der
                                -- ved progressiv scanning kun er een tri-level-puls pr linie
                                -- Sync_in signalet kan derfor ikke foelge no_of_pulses

vaelg_filter :  Disable_1 <= filter_1_valgt;
                Disable_2 <= not Disable_1;

end Behavioral;
