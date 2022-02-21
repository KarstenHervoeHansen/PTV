library ieee; 
use ieee.std_logic_1164.all;

 --library synplify; 
--use synplify.attributes.all;
--
-- pragma translate_off
library UNISIM;use UNISIM.VCOMPONENTS.ALL;
-- pragma translate_on
--

entity  mem_interface_top is
port(
		dip1                    	: in STD_LOGIC;
		dip2                    	: in std_logic;
		dip3                    	: in std_logic;
		reset_in                	: in STD_LOGIC;
		SYS_CLK            			: in STD_LOGIC;
--		SYS_CLKb           			: in STD_LOGIC;
		clk_int_1          			: out STD_LOGIC;
		clk90_int_1        			: out STD_LOGIC;
		clk180_1           			: out STD_LOGIC;
		clk270_1           			: out STD_LOGIC;
       	sys_rst_1       			: out STD_LOGIC;
       	sys_rst90_1     			: out STD_LOGIC;
       	sys_rst180_1    			: out STD_LOGIC;
       	sys_rst270_1    			: out STD_LOGIC; 
		cntrl0_rst_dqs_div_in    	: in std_logic;
		cntrl0_rst_dqs_div_out   	: out std_logic;
		cntrl0_ddr2_casb         	: out STD_LOGIC;
		cntrl0_ddr2_cke          	: out STD_LOGIC; 
		cntrl0_ddr2_clk0  			: out STD_LOGIC; 
		cntrl0_ddr2_clk0b   		: out STD_LOGIC; 

		cntrl0_burst_done   		: in std_logic;
		cntrl0_user_input_address  	: in std_logic_vector(22 downto 0);
		cntrl0_user_bank_address   	: in std_logic_vector(1 downto 0);
		cntrl0_user_input_data   	: in std_logic_vector(31 downto 0);
		cntrl0_user_data_mask   	: in std_logic_vector(3 downto 0);

		cntrl0_auto_ref_req     	: out STD_LOGIC;
		cntrl0_ar_done          	: out STD_LOGIC;
		cntrl0_user_cmd_ack     	: out STD_LOGIC;
		cntrl0_user_data_valid  	: out STD_LOGIC;
		cntrl0_user_output_data 	: out STD_LOGIC_vector(31 downto 0);
		cntrl0_init_val         	: out STD_LOGIC;
		cntrl0_ddr2_csb          	: out STD_LOGIC;
		cntrl0_ddr2_rasb         	: out STD_LOGIC;
		cntrl0_ddr2_web          	: out STD_LOGIC; 
		cntrl0_ddr2_ODT0         	: out STD_LOGIC; 
		cntrl0_user_config_register1: in STD_LOGIC_VECTOR (14 downto 0);
		cntrl0_user_config_register2: in STD_LOGIC_VECTOR (12 downto 0); 
		cntrl0_ddr2_address      	: out STD_LOGIC_VECTOR(12 downto 0);
		cntrl0_ddr2_ba           	: out STD_LOGIC_VECTOR(1 downto 0); 
		cntrl0_user_command_register: in std_logic_vector(3 downto 0);
		cntrl0_ddr2_dm           	: out STD_LOGIC_VECTOR(1 downto 0);
		cntrl0_ddr2_dqs          	: inout STD_LOGIC_VECTOR(1 downto 0); 
     	cntrl0_ddr2_dqs_n          	: inout STD_LOGIC_VECTOR(1 downto 0); 
      	cntrl0_ddr2_dq           	: inout STD_LOGIC_VECTOR(15 downto 0) 
      ); 
end   mem_interface_top; 
 architecture   arc_mem_interface_top of    mem_interface_top    is 

---- Component declaration
component   ddr2_top_16bit_00
port(
		dip1              		: in STD_LOGIC;
		dip3              		: in std_logic;
		wait_200us        		: in std_logic;
		delay_sel_val     		: in STD_LOGIC_VECTOR(4 downto 0);
		clk_int           		: in std_logic;
		clk90_int         		: in std_logic;
		sys_rst      			: in std_logic;
		sys_rst90     			: in std_logic;
		sys_rst180    			: in std_logic;
		sys_rst270    			: in std_logic;
		reset_in          		: in STD_LOGIC;
		rst_dqs_div_in    		: in std_logic;
		rst_dqs_div_out   		: out std_logic; 
		clk180   				: in std_logic;
		clk270   				: in std_logic;
		ddr_casb         		: out STD_LOGIC;
		ddr_cke          		: out STD_LOGIC;
		ddr2_clk0  				: out STD_LOGIC; 
		ddr2_clk0b   			: out STD_LOGIC; 
		                    	
		ddr_csb          		: out STD_LOGIC;
		ddr_rasb         		: out STD_LOGIC;
		ddr_web          		: out STD_LOGIC; 
		burst_done       		: in std_logic;
		user_input_address  	: in std_logic_vector(22 downto 0);
		user_bank_address   	: in std_logic_vector(1 downto 0);
		user_command_register	: in std_logic_vector(3 downto 0);
		user_input_data     	: in std_logic_vector(31 downto 0);
		user_data_mask      	: in std_logic_vector(3 downto 0);
		auto_ref_req     		: out STD_LOGIC;
		ar_done          		: out STD_LOGIC;
		user_cmd_ack     		: out STD_LOGIC;
		user_data_valid  		: out STD_LOGIC;
		user_output_data 		: out STD_LOGIC_vector(31 downto 0);
		init_val         		: out STD_LOGIC; 
		ddr_ODT0         		: out STD_LOGIC; 
		user_config_register1   :  in STD_LOGIC_vector (14 downto 0);
		user_config_register2   :  in STD_LOGIC_vector (12 downto 0); 
		ddr_address      		: out STD_LOGIC_VECTOR(12 downto 0);
		ddr_ba           		: out STD_LOGIC_VECTOR(1 downto 0);
		ddr_dm           		: out STD_LOGIC_VECTOR(1 downto 0);
		ddr_dqs          		: inout STD_LOGIC_VECTOR(1 downto 0);
		ddr_dqs_n        		: inout STD_LOGIC_VECTOR(1 downto 0);
		ddr_dq           		: inout STD_LOGIC_VECTOR(15 downto 0) 
);
end   component;  


component IBUFGDS_LVDS_25
  port ( I  : in std_logic;
         IB : in std_logic;
         O  : out std_logic);
 end component;

 

component infrastructure_top
port (
		reset_in           : in std_logic;
		wait_200us         : out std_logic;
		delay_sel_val1_val : out std_logic_vector(4 downto 0); 
		sys_rst_val        : out std_logic;
		sys_rst90_val      : out std_logic;
		clk_int_val        : out std_logic;
		clk90_int_val      : out std_logic; 
		sys_clk_ibuf       : in std_logic;
		sys_rst180_val     : out std_logic;
		sys_rst270_val     : out std_logic

);

end component; 
--signal sys_clk_ibuf :  std_logic;
signal sys_rst :  std_logic;
signal wait_200us :std_logic;
signal sys_rst90 :  std_logic;
signal sys_rst180 :  std_logic;
signal sys_rst270 :  std_logic;
signal clk_int :  std_logic;
signal clk90_int :  std_logic;

signal clk180 : std_logic;
signal clk270 : std_logic;
signal delay_sel :  std_logic_vector(4 downto 0); 
begin

clk180 					<= not clk_int;
clk270 					<= not clk90_int;

clk_int_1 				<= clk_int;
clk90_int_1 			<= clk90_int;
clk180_1				<= not clk_int;
clk270_1				<= not clk90_int;
sys_rst_1     			<= sys_rst;
sys_rst90_1   			<= sys_rst90;
sys_rst180_1  			<= sys_rst180;
sys_rst270_1  			<= sys_rst270;

ddr2_top0 : ddr2_top_16bit_00
port map(
		dip1               		=> dip1,
		dip3               		=> dip3,
		wait_200us         		=> wait_200us, 
		clk180 					=> clk180,
		clk270 					=> clk270,
		                    	
		delay_sel_val     		=> delay_sel,
		clk_int            		=> clk_int,
		clk90_int          		=> clk90_int,
		sys_rst        			=> sys_rst,
		sys_rst90      			=> sys_rst90,
		sys_rst180     			=>  sys_rst180,
		sys_rst270     			=> sys_rst270,
		reset_in          		=> reset_in,
		rst_dqs_div_in    		=> cntrl0_rst_dqs_div_in,
		rst_dqs_div_out   		=> cntrl0_rst_dqs_div_out, 
		ddr_casb          		=> cntrl0_ddr2_casb,
		ddr_cke           		=> cntrl0_ddr2_cke, 
		ddr2_clk0  				=> cntrl0_ddr2_clk0, 
		ddr2_clk0b  			=> cntrl0_ddr2_clk0b, 
		burst_done           	=> cntrl0_burst_done,
		user_input_address   	=> cntrl0_user_input_address,
		user_bank_address    	=> cntrl0_user_bank_address,
		user_command_register	=> cntrl0_user_command_register,
		user_input_data      	=> cntrl0_user_input_data,
		user_data_mask       	=> cntrl0_user_data_mask,
		auto_ref_req         	=> cntrl0_auto_ref_req,
		ar_done          		=> cntrl0_ar_done,
		user_cmd_ack     		=> cntrl0_user_cmd_ack,
		user_data_valid  		=> cntrl0_user_data_valid,
		user_output_data 		=> cntrl0_user_output_data,
		init_val				=> cntrl0_init_val,  
		
		ddr_csb           		=> cntrl0_ddr2_csb,
		ddr_rasb          		=> cntrl0_ddr2_rasb,
		ddr_web           		=> cntrl0_ddr2_web, 
		ddr_ODT0          		=> cntrl0_ddr2_ODT0,
		user_config_register1 	=> cntrl0_user_config_register1,
		user_config_register2 	=> cntrl0_user_config_register2,        
		ddr_address       		=> cntrl0_ddr2_address,
		ddr_ba            		=> cntrl0_ddr2_ba,
		ddr_dm            		=> cntrl0_ddr2_dm,
		ddr_dqs           		=> cntrl0_ddr2_dqs, 
		ddr_dqs_n         		=> cntrl0_ddr2_dqs_n, 
		ddr_dq            		=> cntrl0_ddr2_dq
		
); 

-- lvds_clk_input : IBUFGDS_LVDS_25 port map(
--  I => SYS_CLK,
--  IB => SYS_CLKb,
--  O => sys_clk_ibuf
--); 

infrastructure_top0 : infrastructure_top port map
(
		reset_in 				=> reset_in,
		wait_200us 				=> wait_200us,
		delay_sel_val1_val 		=> delay_sel, 
		clk_int_val    			=> clk_int,
		clk90_int_val  			=> clk90_int,
		               			
		sys_clk_ibuf   			=> SYS_CLK,
		sys_rst_val    			=> sys_rst,
		sys_rst90_val  			=> sys_rst90,
		sys_rst180_val 			=> sys_rst180,
		sys_rst270_val 			=> sys_rst270
);


end arc_mem_interface_top; 