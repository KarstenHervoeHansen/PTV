library ieee;
use ieee.std_logic_1164.all;
--library synplify;
--use synplify.attributes.all;
-- pragma translate_off
library UNISIM; 
 use UNISIM.VCOMPONENTS.ALL;
 -- pragma translate_on
package parameter_16bit_00 is
 constant data_width : INTEGER  := 16;
 constant row_address_p   : INTEGER   := 13;
constant column_address_p: INTEGER   :=  10;
constant bank_address_p  : INTEGER   :=  2;
constant mask_width      : INTEGER   :=  4   ;
constant RFC_COUNTER      : std_logic_vector(5 downto 0)   := "100011";                     -- Depends on the memory selected 
constant REF_FREQ_CNT      : std_logic_vector(11 downto 0)    := "010100001110";   -- REF_FREQ_CNT is frequency*7.7 
constant WRITE_RECOVERY_VAL  : std_logic_vector(2 downto 0)    := "100";   -- Write Recovery Value 
end parameter_16bit_00 ;
