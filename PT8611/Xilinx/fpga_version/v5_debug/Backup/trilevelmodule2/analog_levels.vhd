library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


entity analog_levels is
   Port (
      mreset_i          : in std_logic;
      frame_reset_i     : in std_logic;
      clk_i             : in std_logic;
      sync_zero_i       : in std_logic;
      sync_polarity_i   : in std_logic;
      samplerate_x2_i   : in std_logic;
      DAC_o             : out std_logic_vector(3 downto 0)
   );
end analog_levels;


architecture Behavioral of analog_levels is


type state_type is ( negativ, half_negativ, nulniveau, half_positiv, positiv );

signal half_clk_tick : std_logic;
signal state         : state_type;
signal DAC           : std_logic_vector(3 downto 0);

-- trilevelsync_statemaskine benytter sync_zero_i og sync_polarity_i fra trileveltimer til,
-- at generere de korrekte niveauer ( ligesom THS8135 video converter ) :

-- sync_zero_i = 1 and sync_polarity_i = x  => nulniveau
-- sync_zero_i = 0 and sync_polarity_i = 0  => negativt niveau
-- sync_zero_i = 0 and sync_polarity_i = 1  => positivt niveau

-- fra positiv til negativ ( -300mV til 300mV ) er der 5 states ( DAC_o )
-- -300mV, -150mV, 0V, 150mV, 300mV


begin


------------------------------------------------------------------
-- half clk frequency tick
------------------------------------------------------------------
make_half_clk_tick : process( mreset_i, clk_i )
begin
   if mreset_i = '0' then
      half_clk_tick <= '0';
   elsif clk_i'event and clk_i = '1' then
      if frame_reset_i = '1' or samplerate_x2_i = '1' then
         half_clk_tick <= '1';
      else
         half_clk_tick <= not half_clk_tick;
      end if;
   end if;
end process;


------------------------------------------------------------------
-- Output Level SM
------------------------------------------------------------------
trilevelsync_statemaskine : process(mreset_i, clk_i)
begin
   if mreset_i = '0' then
      state <= nulniveau;
   elsif clk_i'event and clk_i = '1' then
      if frame_reset_i = '1' then
         state <= nulniveau;
      elsif half_clk_tick = '1' then
         case state is
         when positiv =>
            if sync_zero_i = '1' then
               state <= half_positiv;
            else
               null;
            end if;
         when half_positiv =>
            if sync_polarity_i = '1' and sync_zero_i = '0' then
               state <= positiv;
            else
               state <= nulniveau;
            end if;
         when nulniveau =>
            if sync_zero_i = '0' and sync_polarity_i = '0' then
               state <= half_negativ;
            elsif sync_zero_i = '0' and sync_polarity_i = '1' then
               state <= half_positiv;
            else
               null;
            end if;
         when half_negativ =>
            if sync_zero_i = '0' and sync_polarity_i = '0' then
               state <= negativ;
            else
               state <= nulniveau;
            end if;
         when negativ =>
            if sync_polarity_i = '1' or sync_zero_i = '1' then
               state <= half_negativ;
            else
               null;
            end if;
         end case;
      end if;
   end if;
end process;

-- state-vaerdier :
-- positiv          =  600 mV
-- half_positiv     =  300 mV
-- nulniveau        =    0 mV
-- half_neg         = -300 mV
-- neg              = -600 mV


make_DAC_output : process(mreset_i, clk_i)
begin
   if mreset_i = '0' then
      DAC(3 downto 0)  <= "0011";
   elsif clk_i'event and clk_i = '1' then
      case state is
      when positiv =>
         DAC(3 downto 0)  <= "0000";
      when half_positiv =>
         DAC(3 downto 0)  <= "0001";
      when half_negativ =>
         DAC(3 downto 0)  <= "0111";
      when negativ =>
         DAC(3 downto 0)  <= "1111";
      when others =>
         DAC(3 downto 0)  <= "0011";
      end case;
   end if;
end process;


--------------------------------------------------------
-- Output Reclocking:
--------------------------------------------------------
output_reclocking : process( mreset_i, clk_i )
begin
   if mreset_i = '0' then
      DAC_o <= "0011";
   elsif clk_i'event and clk_i = '1' then
      DAC_o <= DAC;
   end if;
end process;


end Behavioral;