-- ****************************************************************************
-- Copyright (c) 2004 Texas Instruments, Inc. All Rights Reserved
-- TI Proprietary Information - Internal Data
-- ----------------------------------------------------------------------------
-- File Name: PwmFpga.vhd
-- 16 November 2004
--
-- Author(s): Anker Bjørn Josefsen & Thomas H. Hansen
-- 
-- Description: FPGA for PWM testplatform.
-- PcData00_i to PcData08_i determine duration (0x000 is 512 clock duration)
-- PcData09_i to PcData30 defines the 22 PWM's/Valid signals
-- Req1_o clock data out of the PC
-- zFifoUnderrun turn on the LED on FIFO underrun.
-------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned."+";
use ieee.std_logic_unsigned."-";
use ieee.std_logic_arith.all;
use ieee.std_logic_signed.all;
use ieee.numeric_std.all;

LIBRARY std;
USE   std.standard.ALL;
USE   std.textio.ALL;


entity PwmFpga is
  port (
    zReset_i             : in std_logic;
    Clk_i                : in std_logic;

    Ack1_i               : in std_logic;
    Ack2_i               : in std_logic;

    Sw1_i				: in std_logic;
    Sw2_i				: in std_logic;
    Sw3_i				: in std_logic;
    Sw4_i				: in std_logic;    
 		      
    PcData00_i           : in std_logic;
    PcData01_i           : in std_logic;
    PcData02_i           : in std_logic;
    PcData03_i           : in std_logic;
    PcData04_i           : in std_logic;
    PcData05_i           : in std_logic;
    PcData06_i           : in std_logic;
    PcData07_i           : in std_logic;
    PcData08_i           : in std_logic;
    
    PcData09_i           : in std_logic;
    PcData10_i           : in std_logic;
    PcData11_i           : in std_logic;
    PcData12_i           : in std_logic;
    PcData13_i           : in std_logic;
    PcData14_i           : in std_logic;
    PcData15_i           : in std_logic;
    PcData16_i           : in std_logic;
    
    PcData17_i           : in std_logic;
    PcData18_i           : in std_logic;
    PcData19_i           : in std_logic;
    PcData20_i           : in std_logic;
    PcData21_i           : in std_logic;
    PcData22_i           : in std_logic;
    PcData23_i           : in std_logic;
    PcData24_i           : in std_logic;

    PcData25_i           : in std_logic;
    PcData26_i           : in std_logic;
    PcData27_i           : in std_logic;
    PcData28_i           : in std_logic;
    PcData29_i           : in std_logic;
    PcData30_i           : in std_logic;
   
       
    Req1_o               : out std_logic;
    Req2_o               : out std_logic;
    
    zLed1_o              : out std_logic;
    zLed2_o              : out std_logic;
    zLed3_o              : out std_logic;
    zLed4_o              : out std_logic;

    Dir_a_o              : out std_logic;
    Dir_b_o              : out std_logic;
    Dir_c_o              : out std_logic;
    Dir_d_o              : out std_logic;
    Dir_req_o            : out std_logic;
    Dir_ack_o            : out std_logic;

    zOe_a_o              : out std_logic;
    zOe_b_o              : out std_logic;
    zOe_c_o              : out std_logic;
    zOe_d_o              : out std_logic;
    zOe_req_o            : out std_logic;
    zOe_ack_o            : out std_logic;
    Oe_osc1_o            : out std_logic;
    Oe_osc2_o            : out std_logic;
    Oe_osc3_o            : out std_logic;
    zOe_ReClk_o          : out std_logic;

    Pwm00_o              : out std_logic;
    Pwm01_o              : out std_logic;
    Pwm02_o              : out std_logic;
    Pwm03_o              : out std_logic;
    Pwm04_o              : out std_logic;
    Pwm05_o              : out std_logic;
    Pwm06_o              : out std_logic;
    Pwm07_o              : out std_logic;
    
    Pwm08_o              : out std_logic;
    Pwm09_o              : out std_logic;
    Pwm10_o              : out std_logic;
    Pwm11_o              : out std_logic;
    Pwm12_o              : out std_logic;
    Pwm13_o              : out std_logic;
    Pwm14_o              : out std_logic;
    Pwm15_o              : out std_logic;

    Pwm16_o              : out std_logic;
    Pwm17_o              : out std_logic;
    Pwm18_o              : out std_logic;
    Pwm19_o              : out std_logic;
    Pwm20_o              : out std_logic;
    Pwm21_o              : out std_logic;
    Pwm22_o              : out std_logic;
    Pwm23_o              : out std_logic;
    
    Pwm24_o              : out std_logic;
    Pwm25_o              : out std_logic;
    Pwm26_o              : out std_logic;
    Pwm27_o              : out std_logic;
    Pwm28_o              : out std_logic;
    Pwm29_o              : out std_logic;
    Pwm30_o              : out std_logic;
    Pwm31_o              : out std_logic
       );
end PwmFpga;
 
 
architecture rtl of PwmFpga is

  signal zReset1         : std_logic;
  signal zReset          : std_logic;
  
  signal PcData          : std_logic_vector(30 downto 0);
  
  signal IfWrite         : std_logic;
  signal IfCounter       : std_logic_vector(3 downto 0);

  type IfStateType is (
    WaitForSpace, 
    WaitHigh,
    WaitLow     
  );
  signal IfState         : IfStateType;
  
  constant FifoN         : integer :=16;
  constant FifoW         : integer :=31;
  signal zFifoUnderrun   : std_logic;
  signal FifoReady       : std_logic;
  signal FifoData        : std_logic_vector(FifoW-1 downto 0);
  signal FifoPnt         : std_logic_vector(FifoN-1 downto 0);
  type FifoType is array (0 to FifoN-1) of std_logic_vector(FifoW-1 downto 0);
  signal Fifo            : FifoType;
  
  signal PwmRead         : std_logic;
  signal Pwm             : std_logic_vector(21 downto 0);
  signal Pwm1            : std_logic_vector(21 downto 0);
  signal PwmCnt          : std_logic_vector(8 downto 0);
  
begin


--------------------------------------------------------------------------------
--  Control and Status Output Signals in Combinatoric Logic :
--------------------------------------------------------------------------------

  Req2_o      <= '0';

  zLed1_o     <= zFifoUnderrun;
  zLed2_o     <= '1';
  zLed3_o     <= '1';
  zLed4_o     <= '1';

  Dir_a_o     <= '1';
  Dir_b_o     <= '1';
  Dir_c_o     <= '1';
  Dir_d_o     <= '1';
  Dir_req_o   <= '0';
  Dir_ack_o   <= '1';

  zOe_a_o     <= '0';
  zOe_b_o     <= '0';
  zOe_c_o     <= '0';
  zOe_d_o     <= '0';
  zOe_req_o   <= '0';
  zOe_ack_o   <= '0';
  Oe_osc1_o   <= '1';
  Oe_osc2_o   <= '1';
  Oe_osc3_o   <= '1';
  zOe_ReClk_o <= '0';


--------------------------------------------------------------------------------
--  ReClock asyn Reset
--------------------------------------------------------------------------------
  
  ResetProcess:  process ( Clk_i )
  begin
    if (Clk_i 'event and Clk_i  = '1') then
      zReset1 <= zReset_i;
      zReset <= zReset1;
    end if;
  end process;

--------------------------------------------------------------------------------
--  Vector from pin input
--------------------------------------------------------------------------------
  
  PcData <= PcData30_i & PcData29_i & PcData28_i & PcData27_i & PcData26_i & PcData25_i &PcData24_i & PcData23_i & PcData22_i & PcData21_i & PcData20_i & PcData19_i & PcData18_i & PcData17_i & PcData16_i & PcData15_i & PcData14_i & PcData13_i & PcData12_i & PcData11_i & PcData10_i & PcData09_i & PcData08_i & PcData07_i & PcData06_i & PcData05_i &PcData04_i & PcData03_i & PcData02_i & PcData01_i & PcData00_i;
  
--------------------------------------------------------------------------------
--  IfProcess controls PC interface signals (Req1_o) and writing 
--  into the Fifo (IfWrite)
--------------------------------------------------------------------------------
  
  IfProcess: process (zReset ,Clk_i ) 
  begin
    if ( zReset = '0') then
      IfState <= WaitForSpace;
      Req1_o <= '0';
      IfWrite <= '0';
      IfCounter <= (others => '0');
    elsif (Clk_i 'event and Clk_i  = '1') then
      case IfState is
        when WaitForSpace => 
          if (FifoReady='1') then
            IfState <= WaitHigh;
            IfWrite <= '1';
            IfCounter <= (others => '0');
            Req1_o <= '1';         
          else
            IfState <= WaitForSpace;
          end if;
        when WaitHigh =>
          if (IfCounter="1111") then
            IfState <= WaitLow;
            IfCounter <= (others => '0');
          else
            IfCounter <= IfCounter+"1";
          end if;
          IfWrite <= '0';
        when WaitLow =>
          if (IfCounter="1111") then
            IfState <= WaitForSpace;
          else
            IfCounter <= IfCounter+"1";
          end if;
          Req1_o <= '0';
        when others =>
          IfState <= WaitForSpace;
        end case;
    end if;
  end process;  
  
--------------------------------------------------------------------------------
--  FifoProcess implements a buffer. FifoReady signals room for new data,
--  FifoData is output from buffer, PcData is data going into the Fifo
--------------------------------------------------------------------------------
  
  FifoProcess:  process (zReset ,Clk_i ) 
  begin
    if ( zReset = '0') then
      FifoPnt(FifoN-1 downto 1) <= (others => '0');
      FifoPnt(0) <= '1';
      zFifoUnderrun <= '1';
      for i in 0 to FifoN-1 loop
        Fifo(i) <= (others => '0');
      end loop;
    elsif (Clk_i 'event and Clk_i  = '1') then
      for i in 0 to FifoN-1 loop
          if (PwmRead='1' and IfWrite='0') then
            Fifo(i) <= Fifo((i+1) mod FifoN);
            FifoPnt(i) <= FifoPnt((i+1) mod FifoN);          
          elsif (PwmRead='0' and IfWrite='1') then
            if (FifoPnt(i)='1') then
              Fifo(i) <= PcData;
            end if;
            FifoPnt(i) <= FifoPnt(((i-1)+FifoN) mod FifoN);
          elsif (PwmRead='1' and IfWrite='1') then
             if (FifoPnt((i+1) mod FifoN)='1') then
               Fifo(i) <= PcData;
             else
               Fifo(i) <= Fifo((i+1) mod FifoN);
             end if;  
          end if;
      end loop;
      if PwmRead='1' and IfWrite='0' and FifoPnt(1)='1' then
        zFifoUnderrun <= '0';
      end if;
    end if;
  end process;  
  
  FifoData <= Fifo(0);
  FifoReady <= not FifoPnt(0);
  
--------------------------------------------------------------------------------
--  PwmProcess clock PWM signals to pinoutput after defined duration.
--  Prediction is done by reading data out of Fifo when counter is equal to 2.
--  When Fifo data is equal to 1 there is no time manipulate the counter and the
--  PwmRead is asserted immediatly.
--------------------------------------------------------------------------------
  
  PwmProcess:  process (zReset ,Clk_i  )
  begin
    if ( zReset = '0') then
      Pwm <= (others => '0');
      Pwm1 <= (others => '0');
      PwmCnt <= (others => '0');
      PwmRead <= '0';
    elsif (Clk_i 'event and Clk_i  = '1') then
      if ((PwmCnt="000000010") or ((FifoData(8 downto 0)="000000001") and (PwmCnt="000000001"))) then
        PwmRead<='1';
      else        
        PwmRead<='0';
      end if;  
      if (PwmRead='1') then 
        PwmCnt <= FifoData(8 downto 0);
        Pwm1 <= FifoData(30 downto 9);
      else
        PwmCnt<=PwmCnt-"1";
      end if;
      Pwm <= Pwm1;
    end if;
  end process;
  
  Pwm00_o <= Pwm(00);
  Pwm01_o <= Pwm(01);
  Pwm02_o <= Pwm(02);
  Pwm03_o <= Pwm(03);
  Pwm04_o <= Pwm(04);
  Pwm05_o <= Pwm(05);
  Pwm06_o <= Pwm(06);
  Pwm07_o <= Pwm(07);

  Pwm08_o <= Pwm(08);
  Pwm09_o <= Pwm(09);
  Pwm10_o <= Pwm(10);
  Pwm11_o <= Pwm(11);
  Pwm12_o <= Pwm(12);
  Pwm13_o <= Pwm(13);
  Pwm14_o <= Pwm(14);
  Pwm15_o <= Pwm(15);

  Pwm16_o <= Pwm(16);
  Pwm17_o <= Pwm(17);
  Pwm18_o <= Pwm(18);
  Pwm19_o <= Pwm(19);
  Pwm20_o <= Pwm(20);
  Pwm21_o <= Pwm(21);
  
  Pwm22_o <= '0';
  Pwm23_o <= '0';

  Pwm24_o <= '0';
  Pwm25_o <= '0';
  Pwm26_o <= '0';
  Pwm27_o <= '0';
  Pwm28_o <= '0';
  Pwm29_o <= '0';
  Pwm30_o <= '0';
  Pwm31_o <= '0';
 
end rtl;
