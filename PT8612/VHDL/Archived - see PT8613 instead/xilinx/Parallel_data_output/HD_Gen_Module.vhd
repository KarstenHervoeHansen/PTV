library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

library UNISIM;
use UNISIM.VComponents.all;

------------------------------------------------------------------------------------------------
-- Toplevel for HD generator module PT8612
-- 2005-09-26: Created                                                        Thomas Holm Hansen
-- 2005-02-20: crc_registering process changed                                THH
--
------------------------------------------------------------------------------------------------

entity HD_Gen_Module is
   Port (
      --------- clocks -------------------------------------------------------------------------
      brefclk2_p_i   : in std_logic;
      brefclk2_n_i   : in std_logic;

      --------- to PLL's -----------------------------------------------------------------------
      pll1_S0_o      : out std_logic;
      pll1_S1_o      : out std_logic;
      pll1_S2_o      : out std_logic;
      pll1_S3_o      : out std_logic;
      pll2_S0_o      : out std_logic;
      pll2_S1_o      : out std_logic;
      pll2_S2_o      : out std_logic;
      pll2_S3_o      : out std_logic;

      --------- Debug --------------------------------------------------------------------------
      
      ibf_i          : in std_logic;
      debug_o        : out std_logic_vector(10 downto 0)
   );
end HD_Gen_Module;


architecture Behavioral of HD_Gen_Module is

signal clk                       : std_logic;
signal ibf                       : std_logic;
signal zstb                      : std_logic;
signal brefclk2                  : std_logic;
signal data_tick                 : std_logic;
signal debug_data                : std_logic_vector(9 downto 0);
signal debug_count               : std_logic_vector(9 downto 0);
signal debug_clk                 : std_logic;

type state_type is (wait_for_ibf_high, wait_for_ibf_low, new_data);
signal state                     : state_type;


component glitch_remover
   port (
      clk_i           : in std_logic;
      signal_i       : in std_logic;
      clean_signal_o : out std_logic
   );
end component;



begin


glitch_removing_ibf : glitch_remover
   port map(
      clk_i             => clk,                               
      signal_i          => ibf_i,
      clean_signal_o    => ibf
   );
   
   
--------------------------------------------
-- PC interface SM :
--------------------------------------------
pc_interface_sm : process (clk) begin
   if clk'event and clk = '1' then
      case state is 
      when wait_for_ibf_low =>
         zstb        <= '1';
         data_tick   <= '0';
         if ibf = '0' then
            state <= new_data;
         end if;
      when new_data =>
         zstb        <= '1';
         data_tick   <= '1';
         state       <= wait_for_ibf_high;
      when others =>    -- wait_for_ibf_high
         zstb        <= '0';
         data_tick   <= '0';
         if ibf = '1' then
            state <= wait_for_ibf_low;
         end if;
      end case;
   end if;
end process;


--------------------------------------------
-- Counter for debug data :
--------------------------------------------
debug_counter : process (clk) begin
   if clk'event and clk = '1' then
      if data_tick = '1' then
         debug_count <= debug_count + 1;
      end if;
   end if;
end process;


--------------------------------------------------------------
-- Differntial clock inputs:
--------------------------------------------------------------
diff_buf_clk : IBUFGDS
generic map (IOSTANDARD => "LVDS_25")
port map (
   O  => clk,
   I  => brefclk2_p_i,
   IB => brefclk2_n_i
);


debug_data           <= debug_count;
debug_o(9 downto 0)  <= debug_data;
debug_o(10)          <= zstb;


--PLL configuration:
pll1_S0_o <= '0';    --brefclk2 148.5
pll1_S1_o <= '1';
pll1_S2_o <= '1';
pll1_S3_o <= '0';

pll2_S0_o <= '1';    --brefclk 148.35
pll2_S1_o <= '1';
pll2_S2_o <= '1';
pll2_S3_o <= '0';


end Behavioral;