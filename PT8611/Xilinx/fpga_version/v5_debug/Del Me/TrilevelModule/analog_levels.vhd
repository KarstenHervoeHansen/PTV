library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity analog_levels is
   Port (
      mreset_i      : in std_logic;    -- active low
      frame_reset_i : in std_logic;    -- active high
      clk_i         : in std_logic;
      sync_i        : in std_logic;
      sync_t_i      : in std_logic;
      tsg_out_o     : out std_logic_vector(3 downto 0)
   );
end analog_levels;

architecture Behavioral of analog_levels is

signal f74          : std_logic;
type state_type is ( negativ, half_negativ, nulniveau, half_positiv, positiv );
signal state :       state_type;

-- trilevelsync_statemaskine benytter sync_i og sync_t_i fra trileveltimer til,
-- at generere de korrekte niveauer ( ligesom THS8135 video converter ) :
-- sync_i = sync_t_i = 00           => negativ niveau
-- sync_i = 1, sync_t_i = dont care => nulniveau
-- sync_i = 0, sync_t_i = 1         => trilevelsync flanke ( positiv niveau )

-- fra positiv til negativ ( -300mV til 300mV ) er der 5 states ( tsg_out_o )
-- -300mV, -150mV, 0V, 150mV, 300mV

begin


-- f74 = intern neddeling af clk_i
make_f74 : process(mreset_i, clk_i)
begin
   if mreset_i = '0' then
      f74 <= '1';
   elsif clk_i'event and clk_i = '1' then
      if frame_reset_i = '1' then
         f74 <= '1';
      else
         f74 <= not f74;
      end if;
   end if;
end process;


trilevelsync_statemaskine : process(mreset_i, clk_i)
begin
   if mreset_i = '0' then
      state <= nulniveau;
   elsif clk_i'event and clk_i = '1' then
      if frame_reset_i = '1' then
         state <= nulniveau;
      elsif f74 = '1' then
         case state is
            when positiv =>
               if sync_i = '1' then
                  state <= half_positiv;
               else
                  null;
               end if;
            when half_positiv =>
               if sync_t_i = '1' and sync_i = '0' then
                  state <= positiv;
               else
                  state <= nulniveau;
               end if;
            when nulniveau =>
               if sync_i = '0' and sync_t_i = '0' then
                  state <= half_negativ;
               elsif sync_i = '0' and sync_t_i = '1' then
                  state <= half_positiv;
               else
                  null;
               end if;
            when half_negativ =>
               if sync_i = '0' and sync_t_i = '0' then
                  state <= negativ;
               else
                  state <= nulniveau;
               end if;
            when negativ => 
               if sync_t_i = '1' or sync_i = '1' then
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

make_tsg_output : process(mreset_i, clk_i)
begin
   if mreset_i = '0' then
      tsg_out_o(3 downto 0)  <= "0011";
   elsif clk_i'event and clk_i = '1' then
      if f74 = '1' then
         case state is
            when positiv =>
               tsg_out_o(3 downto 0)  <= "0000";
            when half_positiv =>
               tsg_out_o(3 downto 0)  <= "0001";
            when half_negativ =>
               tsg_out_o(3 downto 0)  <= "0111";
            when negativ =>
               tsg_out_o(3 downto 0)  <= "1111";
            when others =>
               tsg_out_o(3 downto 0)  <= "0011";
         end case;
      end if;
   end if;     
end process;

end Behavioral;