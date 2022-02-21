library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

library UNISIM;
use UNISIM.VComponents.all;


entity parallel_to_pc_backend is
port(
   sync_reset_i         : in std_logic;
   clk_i                : in std_logic;
   enable_o             : out std_logic;
   
   -- video input:
   data_a_i             : in std_logic_vector(19 downto 0);
   data_b_i             : in std_logic_vector(19 downto 0);
   
   -- pc interface:
   pc_ibf_i             : in std_logic;
   pc_data_o            : out std_logic_vector(9 downto 0);
   pc_zstb_o            : out std_logic
);
end parallel_to_pc_backend;


architecture Behavioral of parallel_to_pc_backend is

type state_type is (s1,s2,s3,s4);
signal state                  : state_type;
type pci_state_type is (wait_for_ibf_high, wait_for_ibf_low, new_data);
signal pci_state              : pci_state_type;


signal rio_fifo_err_a         : std_logic;
signal rio_fifo_err_b         : std_logic;
signal pc_ibf                 : std_logic;
signal data_tick              : std_logic;
signal os_data_a              : std_logic_vector(19 downto 0);
signal os_data_b              : std_logic_vector(19 downto 0);
     
constant single_zdual         : std_logic := '0';


component glitch_remover
   port (
      clk_i            : in std_logic;
      signal_i         : in std_logic;
      clean_signal_o   : out std_logic
   );
end component;


begin


--------------------------------------------------------------
-- Remove glitches on pc_ibf_i:
--------------------------------------------------------------
glitch_removing_ibf : glitch_remover
   port map(
      clk_i             => clk_i,                               
      signal_i          => pc_ibf_i,
      clean_signal_o    => pc_ibf
   );


data_multiplex_sm : process (clk_i)
begin
   if clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         state <= s1;
      elsif data_tick = '1' then
         case state is
         when s1 =>
            state <= s2;
         when s2 =>
            if single_zdual = '1' then
               state <= s1;
            else
               state <= s3;
            end if;
         when s3 =>
            state <= s4;
         when others => -- s4
            state <= s1;
         end case;
      end if;
   end if;
end process;


enable_generation : process (clk_i)
begin
   if clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         enable_o <= '0';
      elsif data_tick = '1' and (state = s4 or (state = s2 and single_zdual = '1')) then
         enable_o <= '1';
      else
         enable_o <= '0';
      end if;
   end if;
end process;


data_multiplex_output : process (state)
begin
   case state is
   when s1     =>  pc_data_o <= data_a_i( 9 downto  0);
   when s2     =>  pc_data_o <= data_a_i(19 downto 10);
   when s3     =>  pc_data_o <= data_b_i( 9 downto  0);
   when others =>  pc_data_o <= data_b_i(19 downto 10);
   end case;
end process;


--------------------------------------------
-- PC interface SM :
--------------------------------------------
pc_interface_sm : process (clk_i) begin
   if clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         pci_state   <= wait_for_ibf_high;
         pc_zstb_o   <= '1';
         data_tick   <= '0';
      else
         case pci_state is 
         when wait_for_ibf_low =>
            pc_zstb_o   <= '1';
            data_tick   <= '0';
            if pc_ibf = '0' then
               pci_state <= new_data;
            end if;
         when new_data =>
            pc_zstb_o   <= '1';
            data_tick   <= '1';
            pci_state   <= wait_for_ibf_high;
         when others =>    -- wait_for_ibf_high
            pc_zstb_o   <= '0';
            data_tick   <= '0';
            if pc_ibf = '1' then
               pci_state <= wait_for_ibf_low;
            end if;
         end case;
      end if;
   end if;
end process;

   
end Behavioral;