-------------------------------------------------------------------------------
-- dsocm_bram_elaborate.vhd
-------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity dsocm_bram_elaborate is
  generic (
    C_MEMSIZE : integer;
    C_PORT_DWIDTH : integer;
    C_PORT_AWIDTH : integer;
    C_NUM_WE : integer;
    C_FAMILY : string
    );
  port (
    BRAM_Rst_A : in std_logic;
    BRAM_Clk_A : in std_logic;
    BRAM_EN_A : in std_logic;
    BRAM_WEN_A : in std_logic_vector(0 to C_NUM_WE-1);
    BRAM_Addr_A : in std_logic_vector(0 to C_PORT_AWIDTH-1);
    BRAM_Din_A : out std_logic_vector(0 to C_PORT_DWIDTH-1);
    BRAM_Dout_A : in std_logic_vector(0 to C_PORT_DWIDTH-1);
    BRAM_Rst_B : in std_logic;
    BRAM_Clk_B : in std_logic;
    BRAM_EN_B : in std_logic;
    BRAM_WEN_B : in std_logic_vector(0 to C_NUM_WE-1);
    BRAM_Addr_B : in std_logic_vector(0 to C_PORT_AWIDTH-1);
    BRAM_Din_B : out std_logic_vector(0 to C_PORT_DWIDTH-1);
    BRAM_Dout_B : in std_logic_vector(0 to C_PORT_DWIDTH-1)
  );
end dsocm_bram_elaborate;

architecture STRUCTURE of dsocm_bram_elaborate is

  attribute WRITE_MODE_A : string;
  attribute WRITE_MODE_B : string;

  component RAMB16_S2_S2 is
    generic (
      WRITE_MODE_A : string;
      WRITE_MODE_B : string
    );
    port (
      ADDRA : in std_logic_vector(12 downto 0);
      CLKA : in std_logic;
      DIA : in std_logic_vector(1 downto 0);
      DOA : out std_logic_vector(1 downto 0);
      ENA : in std_logic;
      SSRA : in std_logic;
      WEA : in std_logic;
      ADDRB : in std_logic_vector(12 downto 0);
      CLKB : in std_logic;
      DIB : in std_logic_vector(1 downto 0);
      DOB : out std_logic_vector(1 downto 0);
      ENB : in std_logic;
      SSRB : in std_logic;
      WEB : in std_logic
    );
  end component;

  attribute WRITE_MODE_A of RAMB16_S2_S2: component is "WRITE_FIRST";
  attribute WRITE_MODE_B of RAMB16_S2_S2: component is "WRITE_FIRST";

  -- Internal signals

  signal dina : std_logic_vector(31 downto 0);
  signal dinb : std_logic_vector(31 downto 0);
  signal douta : std_logic_vector(31 downto 0);
  signal doutb : std_logic_vector(31 downto 0);

begin

  -- Internal assignments

  dina(31 downto 0) <= BRAM_Dout_A(0 to 31);
  BRAM_Din_A(0 to 31) <= douta(31 downto 0);
  dinb(31 downto 0) <= BRAM_Dout_B(0 to 31);
  BRAM_Din_B(0 to 31) <= doutb(31 downto 0);

  ramb16_s2_s2_0 : RAMB16_S2_S2
    generic map (
      WRITE_MODE_A => "WRITE_FIRST",
      WRITE_MODE_B => "WRITE_FIRST"
    )
    port map (
      ADDRA => BRAM_Addr_A(17 to 29),
      CLKA => BRAM_Clk_A,
      DIA => dina(31 downto 30),
      DOA => douta(31 downto 30),
      ENA => BRAM_EN_A,
      SSRA => BRAM_Rst_A,
      WEA => BRAM_WEN_A(0),
      ADDRB => BRAM_Addr_B(17 to 29),
      CLKB => BRAM_Clk_B,
      DIB => dinb(31 downto 30),
      DOB => doutb(31 downto 30),
      ENB => BRAM_EN_B,
      SSRB => BRAM_Rst_B,
      WEB => BRAM_WEN_B(0)
    );

  ramb16_s2_s2_1 : RAMB16_S2_S2
    generic map (
      WRITE_MODE_A => "WRITE_FIRST",
      WRITE_MODE_B => "WRITE_FIRST"
    )
    port map (
      ADDRA => BRAM_Addr_A(17 to 29),
      CLKA => BRAM_Clk_A,
      DIA => dina(29 downto 28),
      DOA => douta(29 downto 28),
      ENA => BRAM_EN_A,
      SSRA => BRAM_Rst_A,
      WEA => BRAM_WEN_A(0),
      ADDRB => BRAM_Addr_B(17 to 29),
      CLKB => BRAM_Clk_B,
      DIB => dinb(29 downto 28),
      DOB => doutb(29 downto 28),
      ENB => BRAM_EN_B,
      SSRB => BRAM_Rst_B,
      WEB => BRAM_WEN_B(0)
    );

  ramb16_s2_s2_2 : RAMB16_S2_S2
    generic map (
      WRITE_MODE_A => "WRITE_FIRST",
      WRITE_MODE_B => "WRITE_FIRST"
    )
    port map (
      ADDRA => BRAM_Addr_A(17 to 29),
      CLKA => BRAM_Clk_A,
      DIA => dina(27 downto 26),
      DOA => douta(27 downto 26),
      ENA => BRAM_EN_A,
      SSRA => BRAM_Rst_A,
      WEA => BRAM_WEN_A(0),
      ADDRB => BRAM_Addr_B(17 to 29),
      CLKB => BRAM_Clk_B,
      DIB => dinb(27 downto 26),
      DOB => doutb(27 downto 26),
      ENB => BRAM_EN_B,
      SSRB => BRAM_Rst_B,
      WEB => BRAM_WEN_B(0)
    );

  ramb16_s2_s2_3 : RAMB16_S2_S2
    generic map (
      WRITE_MODE_A => "WRITE_FIRST",
      WRITE_MODE_B => "WRITE_FIRST"
    )
    port map (
      ADDRA => BRAM_Addr_A(17 to 29),
      CLKA => BRAM_Clk_A,
      DIA => dina(25 downto 24),
      DOA => douta(25 downto 24),
      ENA => BRAM_EN_A,
      SSRA => BRAM_Rst_A,
      WEA => BRAM_WEN_A(0),
      ADDRB => BRAM_Addr_B(17 to 29),
      CLKB => BRAM_Clk_B,
      DIB => dinb(25 downto 24),
      DOB => doutb(25 downto 24),
      ENB => BRAM_EN_B,
      SSRB => BRAM_Rst_B,
      WEB => BRAM_WEN_B(0)
    );

  ramb16_s2_s2_4 : RAMB16_S2_S2
    generic map (
      WRITE_MODE_A => "WRITE_FIRST",
      WRITE_MODE_B => "WRITE_FIRST"
    )
    port map (
      ADDRA => BRAM_Addr_A(17 to 29),
      CLKA => BRAM_Clk_A,
      DIA => dina(23 downto 22),
      DOA => douta(23 downto 22),
      ENA => BRAM_EN_A,
      SSRA => BRAM_Rst_A,
      WEA => BRAM_WEN_A(1),
      ADDRB => BRAM_Addr_B(17 to 29),
      CLKB => BRAM_Clk_B,
      DIB => dinb(23 downto 22),
      DOB => doutb(23 downto 22),
      ENB => BRAM_EN_B,
      SSRB => BRAM_Rst_B,
      WEB => BRAM_WEN_B(1)
    );

  ramb16_s2_s2_5 : RAMB16_S2_S2
    generic map (
      WRITE_MODE_A => "WRITE_FIRST",
      WRITE_MODE_B => "WRITE_FIRST"
    )
    port map (
      ADDRA => BRAM_Addr_A(17 to 29),
      CLKA => BRAM_Clk_A,
      DIA => dina(21 downto 20),
      DOA => douta(21 downto 20),
      ENA => BRAM_EN_A,
      SSRA => BRAM_Rst_A,
      WEA => BRAM_WEN_A(1),
      ADDRB => BRAM_Addr_B(17 to 29),
      CLKB => BRAM_Clk_B,
      DIB => dinb(21 downto 20),
      DOB => doutb(21 downto 20),
      ENB => BRAM_EN_B,
      SSRB => BRAM_Rst_B,
      WEB => BRAM_WEN_B(1)
    );

  ramb16_s2_s2_6 : RAMB16_S2_S2
    generic map (
      WRITE_MODE_A => "WRITE_FIRST",
      WRITE_MODE_B => "WRITE_FIRST"
    )
    port map (
      ADDRA => BRAM_Addr_A(17 to 29),
      CLKA => BRAM_Clk_A,
      DIA => dina(19 downto 18),
      DOA => douta(19 downto 18),
      ENA => BRAM_EN_A,
      SSRA => BRAM_Rst_A,
      WEA => BRAM_WEN_A(1),
      ADDRB => BRAM_Addr_B(17 to 29),
      CLKB => BRAM_Clk_B,
      DIB => dinb(19 downto 18),
      DOB => doutb(19 downto 18),
      ENB => BRAM_EN_B,
      SSRB => BRAM_Rst_B,
      WEB => BRAM_WEN_B(1)
    );

  ramb16_s2_s2_7 : RAMB16_S2_S2
    generic map (
      WRITE_MODE_A => "WRITE_FIRST",
      WRITE_MODE_B => "WRITE_FIRST"
    )
    port map (
      ADDRA => BRAM_Addr_A(17 to 29),
      CLKA => BRAM_Clk_A,
      DIA => dina(17 downto 16),
      DOA => douta(17 downto 16),
      ENA => BRAM_EN_A,
      SSRA => BRAM_Rst_A,
      WEA => BRAM_WEN_A(1),
      ADDRB => BRAM_Addr_B(17 to 29),
      CLKB => BRAM_Clk_B,
      DIB => dinb(17 downto 16),
      DOB => doutb(17 downto 16),
      ENB => BRAM_EN_B,
      SSRB => BRAM_Rst_B,
      WEB => BRAM_WEN_B(1)
    );

  ramb16_s2_s2_8 : RAMB16_S2_S2
    generic map (
      WRITE_MODE_A => "WRITE_FIRST",
      WRITE_MODE_B => "WRITE_FIRST"
    )
    port map (
      ADDRA => BRAM_Addr_A(17 to 29),
      CLKA => BRAM_Clk_A,
      DIA => dina(15 downto 14),
      DOA => douta(15 downto 14),
      ENA => BRAM_EN_A,
      SSRA => BRAM_Rst_A,
      WEA => BRAM_WEN_A(2),
      ADDRB => BRAM_Addr_B(17 to 29),
      CLKB => BRAM_Clk_B,
      DIB => dinb(15 downto 14),
      DOB => doutb(15 downto 14),
      ENB => BRAM_EN_B,
      SSRB => BRAM_Rst_B,
      WEB => BRAM_WEN_B(2)
    );

  ramb16_s2_s2_9 : RAMB16_S2_S2
    generic map (
      WRITE_MODE_A => "WRITE_FIRST",
      WRITE_MODE_B => "WRITE_FIRST"
    )
    port map (
      ADDRA => BRAM_Addr_A(17 to 29),
      CLKA => BRAM_Clk_A,
      DIA => dina(13 downto 12),
      DOA => douta(13 downto 12),
      ENA => BRAM_EN_A,
      SSRA => BRAM_Rst_A,
      WEA => BRAM_WEN_A(2),
      ADDRB => BRAM_Addr_B(17 to 29),
      CLKB => BRAM_Clk_B,
      DIB => dinb(13 downto 12),
      DOB => doutb(13 downto 12),
      ENB => BRAM_EN_B,
      SSRB => BRAM_Rst_B,
      WEB => BRAM_WEN_B(2)
    );

  ramb16_s2_s2_10 : RAMB16_S2_S2
    generic map (
      WRITE_MODE_A => "WRITE_FIRST",
      WRITE_MODE_B => "WRITE_FIRST"
    )
    port map (
      ADDRA => BRAM_Addr_A(17 to 29),
      CLKA => BRAM_Clk_A,
      DIA => dina(11 downto 10),
      DOA => douta(11 downto 10),
      ENA => BRAM_EN_A,
      SSRA => BRAM_Rst_A,
      WEA => BRAM_WEN_A(2),
      ADDRB => BRAM_Addr_B(17 to 29),
      CLKB => BRAM_Clk_B,
      DIB => dinb(11 downto 10),
      DOB => doutb(11 downto 10),
      ENB => BRAM_EN_B,
      SSRB => BRAM_Rst_B,
      WEB => BRAM_WEN_B(2)
    );

  ramb16_s2_s2_11 : RAMB16_S2_S2
    generic map (
      WRITE_MODE_A => "WRITE_FIRST",
      WRITE_MODE_B => "WRITE_FIRST"
    )
    port map (
      ADDRA => BRAM_Addr_A(17 to 29),
      CLKA => BRAM_Clk_A,
      DIA => dina(9 downto 8),
      DOA => douta(9 downto 8),
      ENA => BRAM_EN_A,
      SSRA => BRAM_Rst_A,
      WEA => BRAM_WEN_A(2),
      ADDRB => BRAM_Addr_B(17 to 29),
      CLKB => BRAM_Clk_B,
      DIB => dinb(9 downto 8),
      DOB => doutb(9 downto 8),
      ENB => BRAM_EN_B,
      SSRB => BRAM_Rst_B,
      WEB => BRAM_WEN_B(2)
    );

  ramb16_s2_s2_12 : RAMB16_S2_S2
    generic map (
      WRITE_MODE_A => "WRITE_FIRST",
      WRITE_MODE_B => "WRITE_FIRST"
    )
    port map (
      ADDRA => BRAM_Addr_A(17 to 29),
      CLKA => BRAM_Clk_A,
      DIA => dina(7 downto 6),
      DOA => douta(7 downto 6),
      ENA => BRAM_EN_A,
      SSRA => BRAM_Rst_A,
      WEA => BRAM_WEN_A(3),
      ADDRB => BRAM_Addr_B(17 to 29),
      CLKB => BRAM_Clk_B,
      DIB => dinb(7 downto 6),
      DOB => doutb(7 downto 6),
      ENB => BRAM_EN_B,
      SSRB => BRAM_Rst_B,
      WEB => BRAM_WEN_B(3)
    );

  ramb16_s2_s2_13 : RAMB16_S2_S2
    generic map (
      WRITE_MODE_A => "WRITE_FIRST",
      WRITE_MODE_B => "WRITE_FIRST"
    )
    port map (
      ADDRA => BRAM_Addr_A(17 to 29),
      CLKA => BRAM_Clk_A,
      DIA => dina(5 downto 4),
      DOA => douta(5 downto 4),
      ENA => BRAM_EN_A,
      SSRA => BRAM_Rst_A,
      WEA => BRAM_WEN_A(3),
      ADDRB => BRAM_Addr_B(17 to 29),
      CLKB => BRAM_Clk_B,
      DIB => dinb(5 downto 4),
      DOB => doutb(5 downto 4),
      ENB => BRAM_EN_B,
      SSRB => BRAM_Rst_B,
      WEB => BRAM_WEN_B(3)
    );

  ramb16_s2_s2_14 : RAMB16_S2_S2
    generic map (
      WRITE_MODE_A => "WRITE_FIRST",
      WRITE_MODE_B => "WRITE_FIRST"
    )
    port map (
      ADDRA => BRAM_Addr_A(17 to 29),
      CLKA => BRAM_Clk_A,
      DIA => dina(3 downto 2),
      DOA => douta(3 downto 2),
      ENA => BRAM_EN_A,
      SSRA => BRAM_Rst_A,
      WEA => BRAM_WEN_A(3),
      ADDRB => BRAM_Addr_B(17 to 29),
      CLKB => BRAM_Clk_B,
      DIB => dinb(3 downto 2),
      DOB => doutb(3 downto 2),
      ENB => BRAM_EN_B,
      SSRB => BRAM_Rst_B,
      WEB => BRAM_WEN_B(3)
    );

  ramb16_s2_s2_15 : RAMB16_S2_S2
    generic map (
      WRITE_MODE_A => "WRITE_FIRST",
      WRITE_MODE_B => "WRITE_FIRST"
    )
    port map (
      ADDRA => BRAM_Addr_A(17 to 29),
      CLKA => BRAM_Clk_A,
      DIA => dina(1 downto 0),
      DOA => douta(1 downto 0),
      ENA => BRAM_EN_A,
      SSRA => BRAM_Rst_A,
      WEA => BRAM_WEN_A(3),
      ADDRB => BRAM_Addr_B(17 to 29),
      CLKB => BRAM_Clk_B,
      DIB => dinb(1 downto 0),
      DOB => doutb(1 downto 0),
      ENB => BRAM_EN_B,
      SSRB => BRAM_Rst_B,
      WEB => BRAM_WEN_B(3)
    );

end architecture STRUCTURE;

