library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


entity output_level_monitor is
   Port (
      mreset_i          : in std_logic;
      clk_i             : in std_logic;
      tick_10ms_i       : in std_logic;
      mute_i            : in std_logic;
      spl_is_zero_i     : in std_logic;
      tsg_level_i       : in std_logic;
      output_ok_o       : out std_logic
   );
end output_level_monitor;


architecture Behavioral of output_level_monitor is

type statetype is (lvl_ok, lvl_not_ok);
signal state            : statetype;
signal state_count      : std_logic_vector(4 downto 0);
signal tsg_level        : std_logic;


begin


------------------------------------------------------------
-- Reclocking:
------------------------------------------------------------
level_reclocking : process(mreset_i, clk_i)
begin
   if mreset_i = '0' then
      tsg_level <= '1';
   elsif clk_i'event and clk_i = '1' then
      tsg_level <= tsg_level_i;
   end if;
end process;


level_sm : process (mreset_i, clk_i) begin
   if mreset_i = '0' then
      state <= lvl_ok;
      state_count <= conv_std_logic_vector(31,5);
      output_ok_o <= '1';
   elsif clk_i'event and clk_i ='1' then
      case state is
         when lvl_not_ok =>
            if tsg_level = '1' or spl_is_zero_i = '1' then
               state <= lvl_ok;
               state_count <= conv_std_logic_vector(31,5);
            end if;
            output_ok_o <= '0';
         when lvl_ok =>
            if tsg_level = '1' or mute_i = '1' or spl_is_zero_i = '1' then
               state_count <= conv_std_logic_vector(31,5);
            elsif tick_10ms_i = '1' then
               if state_count = conv_std_logic_vector(0,5) then
                  state <= lvl_not_ok;
               else
                  state_count <= state_count - 1;
               end if;
            end if;
            output_ok_o <= '1';
         when others =>
            null;
      end case;
   end if;
end process;


end Behavioral;