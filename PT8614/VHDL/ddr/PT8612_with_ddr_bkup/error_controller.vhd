library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

--  Uncomment the following lines to use the declarations that are
--  provided for instantiating Xilinx primitive components.
--  library UNISIM;
--  use UNISIM.VComponents.all;

entity error_controller is
   port (
      clk_i                : in std_logic;                        -- 27MHz clock
      parameter_change_i   : in std_logic;                        -- connect to cs_i
      error_i              : in std_logic;
      error_o              : out std_logic;
      tick_10ms_o          : out std_logic
   );
end error_controller;


architecture Behavioral of error_controller is

type statetype is (no_errors, wait_for_tick, wait1, wait2, observe1, observe2, observe3, observe4, observe5, report_error);
signal state                     : statetype;

signal tick_10ms_count           : std_logic_vector(18 downto 0);
signal tick_10ms                 : std_logic;
signal tick_2s_count             : std_logic_vector(7 downto 0);
signal tick_2s                   : std_logic;


begin


-------------------------------------------------------------
-- Generates 10ms tick :
-------------------------------------------------------------
tick_10ms_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if tick_10ms_count = 0 then
         tick_10ms_count <= conv_std_logic_vector(270000-1,19);
         tick_10ms <= '1';
      else
         tick_10ms_count <= tick_10ms_count-1;
         tick_10ms <= '0';
      end if;
   end if;
end process;

tick_2s_generation : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if tick_10ms = '1' then
         if tick_2s_count = 0 then
            tick_2s_count <= conv_std_logic_vector(199,8);
            tick_2s <= '1';
         else
            tick_2s_count <= tick_2s_count-1;
            tick_2s <= '0';
         end if;
      else
         tick_2s <= '0';
      end if;
   end if;
end process;

tick_10ms_o <= tick_10ms;


--------------------------------------------------------------
-- error sm :
--------------------------------------------------------------
error_sm : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if parameter_change_i = '1' then
         state    <= no_errors;
         error_o  <= '0';
      else
         case state is
         when no_errors =>
            if error_i = '1' then
               state <= wait_for_tick;
            end if;
            error_o  <= '0';
         when wait_for_tick =>
            if tick_2s = '1' then
               state <= wait1;
            end if;
            error_o  <= '0';
         when wait1 =>
            if tick_2s = '1' then
               state <= wait2;
            end if;
            error_o  <= '0';
         when wait2 =>
            if tick_2s = '1' then
               state <= observe1;
            end if;
            error_o  <= '0';
         when observe1 =>
            if error_i = '1' then
               state <= report_error;
            elsif tick_2s = '1' then
               state <= observe2;
            end if;
         when observe2 =>
            if error_i = '1' then
               state <= report_error;
            elsif tick_2s = '1' then
               state <= observe3;
            end if;
         when observe3 =>
            if error_i = '1' then
               state <= report_error;
            elsif tick_2s = '1' then
               state <= observe4;
            end if;
            error_o  <= '0';
         when observe4 =>
            if error_i = '1' then
               state <= report_error;
            elsif tick_2s = '1' then
               state <= observe5;
            end if;
            error_o  <= '0';
         when observe5 =>
            if error_i = '1' then
               state <= report_error;
            elsif tick_2s = '1' then
               state <= no_errors;
            end if;
            error_o  <= '0';
         when others => -- report_error
            state    <= observe1;
            error_o  <= '1';
         end case;
      end if;
   end if;
end process;


end Behavioral;