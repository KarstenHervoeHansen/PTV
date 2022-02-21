library ieee;
USE ieee.STD_LOGIC_1164.ALL;
USE ieee.std_logic_signed.all;
USE ieee.std_logic_unsigned.all;
USE ieee.std_logic_arith.all;


entity audio_lut is
   port (
      clk_i                : in std_logic;
      zreset_i             : in std_logic;                        -- async. master reset
      en_i                 : in std_logic;
      sync_reset_i         : in std_logic;

      audio_sample_tick_i  : in std_logic;
      audio_click_i        : in std_logic;
      signal_index_i       : in std_logic_vector(7 downto 0);
      level_index_i        : in std_logic_vector(7 downto 0);
      audio_data_o         : out std_logic_vector(23 downto 0)
   );
end audio_lut;


architecture behavioral of audio_lut is


signal a_rom         : std_logic_vector(5 downto 0);
signal rom_data      : std_logic_vector(23 downto 0);
signal sample_count  : std_logic_vector(5 downto 0);


component sine_rom
	port(
	   addr  : IN std_logic_VECTOR(5 downto 0);   
	   clk   : IN std_logic;                      
	   dout  : OUT std_logic_VECTOR(23 downto 0);
	   en    : IN std_logic
	);                                     
end component;


begin


---------------------------------------------------------------
-- ROM containing 24-bit sine table with 48 samples:
---------------------------------------------------------------
sine_table_rom : sine_rom
   port map(
		addr  => a_rom,
		clk   => clk_i,
		dout  => rom_data,
		en    => en_i
	);


---------------------------------------------------------------
-- sample_counter:
---------------------------------------------------------------
sample_counting : process (clk_i, zreset_i) begin
   if clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         sample_count <= (others => '0');
      elsif en_i = '1' and audio_sample_tick_i = '1' then
         if sample_count = conv_std_logic_vector(47,6) then
            sample_count <= (others => '0');
         else
            sample_count <= sample_count + 1;
         end if;
      end if;
   end if;
end process;


---------------------------------------------------------------
-- audio signal selection:
---------------------------------------------------------------
audio_signal_selecting : process (clk_i, zreset_i) begin
   if clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         a_rom <= (others => '0');
      elsif en_i = '1' and audio_sample_tick_i = '1' then
         case signal_index_i is                                                      
            when X"01"  =>                         -- sine
               a_rom <= sample_count;
            when X"02"  =>                         -- click
               if audio_click_i = '1' then
                  a_rom <= sample_count;
               else
                  a_rom <= (others => '0');
               end if;
            when others =>                         -- silence
               a_rom <= (others => '0');
         end case;
         
      end if;
   end if;
end process;

     
---------------------------------------------------------------
-- Audio level:
---------------------------------------------------------------
audio_level_shifting : process (clk_i, zreset_i) begin
   if clk_i'event and clk_i = '1' then
      if sync_reset_i = '1' then
         audio_data_o <= (others => '0');
      elsif en_i = '1' then
         case level_index_i is                                                      
            when X"01"  => audio_data_o <= to_stdlogicvector(to_bitvector(rom_data) sra 1);   --  -6 dBFS  
            when X"02"  => audio_data_o <= to_stdlogicvector(to_bitvector(rom_data) sra 2);   -- -12 dBFS  
            when X"03"  => audio_data_o <= to_stdlogicvector(to_bitvector(rom_data) sra 3);   -- -18 dBFS  
            when X"04"  => audio_data_o <= to_stdlogicvector(to_bitvector(rom_data) sra 4);   -- -24 dBFS  
            when others => audio_data_o <= rom_data;           --   0 dBFS
         end case;
      end if;
   end if;
end process;


end behavioral;