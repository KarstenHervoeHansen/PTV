library IEEE;                   
use IEEE.STD_LOGIC_1164.ALL;    
use IEEE.STD_LOGIC_ARITH.ALL;   
use IEEE.STD_LOGIC_UNSIGNED.ALL;

---- Uncomment the following library declaration if instantiating
---- any Xilinx primitives in this code.
library UNISIM;
use UNISIM.VComponents.all;


entity HD_Gen_Module is
   Port (
      --------- from Mainframe -----------------------------------------------------------------
      mreset_i       : in std_logic;      -- Master Reset fra mainboard
      brefclk_p_i    : in std_logic;
      brefclk_n_i    : in std_logic;
      fvh_i		 			    : in std_logic_vector(2 downto 0);
      enable_i         : in std_logic;
      sync_reset_i   : in std_logic;
      --------- LED's --------------------------------------------------------------------------
      led_o          : out std_logic_vector(3 downto 0);
      
      --------  test signals for RocketIO output ------------------------------------------------		
                
		luma_a_o				: out std_logic_vector(9 downto 0);
		chroma_a_o				: out std_logic_vector(9 downto 0);
   
      --------- DDR RAM--------------------------------------------------------------------------
      ddr2_casb      : out std_logic;
      ddr2_cke       : out std_logic;
      ddr2_clk0      : out std_logic;
      ddr2_clk0b     : out std_logic;
      ddr2_csb       : out std_logic;
      ddr2_rasb      : out std_logic;
      ddr2_web       : out std_logic;
      ddr2_ODT0      : out std_logic;
      ddr2_address   : out std_logic_VECTOR(12 downto 0);
      ddr2_ba        : out std_logic_VECTOR(1 downto 0);
      ddr2_dm        : out std_logic_VECTOR(1 downto 0);
      ddr2_dqs       : inout std_logic_VECTOR(1 downto 0);
      ddr2_dqs_n     : inout std_logic_VECTOR(1 downto 0);
      ddr2_dq        : inout std_logic_VECTOR(15 downto 0)
   );
end HD_Gen_Module;


architecture Behavioral of HD_Gen_Module is



component BUFG
   Port (
      I : in  std_logic;
      O : out std_logic
   );
end component;

component  mem_interface_top
port(
		dip1                    	: in STD_LOGIC;                              
		dip2                    	: in std_logic;                              
		dip3                    	: in std_logic;                              
		reset_in                	: in STD_LOGIC;                              
		SYS_CLK            			: in STD_LOGIC;                                 
		                          
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
end component;

component mem_controller
port(         
		reset	            		: in std_logic;
		clk	                		: in std_logic;
		clk90   					: in std_logic;
		clk180						: in std_logic;
		burst_done   				: out std_logic;
		enable_i				: in std_logic;
		sync_reset_i           	: in std_logic;		             
		         	
		user_input_address  		: out std_logic_vector(22 downto 0);
		user_bank_address   		: out std_logic_vector(1 downto 0);
		user_input_data  			: out std_logic_vector(31 downto 0);
		user_data_mask   			: out std_logic_vector(3 downto 0);
		                        	
		auto_ref_req				: in std_logic;
		ar_done          			: in std_logic;
		user_cmd_ack     			: in std_logic;
		user_data_valid  			: in std_logic;
		user_output_data 			: in std_logic_vector(31 downto 0);
		init_val         			: in std_logic;
      		   
		fvh_i		 			: in  std_logic_vector(2 downto 0);
		luma_a_o				: out std_logic_vector(9 downto 0);
		chroma_a_o				: out std_logic_vector(9 downto 0);
		luma_b_o				: out std_logic_vector(9 downto 0);  
		chroma_b_o				: out std_logic_vector(9 downto 0);    
		                	
		user_config_register1		: out std_logic_vector(14 downto 0);
		user_config_register2		: out std_logic_vector(12 downto 0); 
		user_command_register		: out std_logic_vector(3 downto 0);
		TP1							: out std_logic;
		TP2							: out std_logic
	);
end component;

signal new_clk: std_logic;
signal clk1,clk2,clk90,clk180,brefclk,brefclk2: std_logic;
signal burst_done: std_logic;                                     
signal user_input_address: std_logic_vector(22 downto 0);    
signal user_bank_address: std_logic_vector(1 downto 0);      
signal user_input_data: std_logic_vector(31 downto 0);	     
signal user_data_mask: std_logic_vector(3 downto 0);	     
signal auto_ref_req: std_logic; 	          
signal ar_done: std_logic;                 
signal user_cmd_ack: std_logic;           
signal user_data_valid: std_logic;        
signal user_output_data: std_logic_vector(31 downto 0);      
signal init_val: std_logic;                          
signal user_config_register1: std_logic_vector(14 downto 0);   
signal user_config_register2: std_logic_vector(12 downto 0);   
signal user_command_register: std_logic_vector(3 downto 0);    
signal rst_dqs_div_i, rst_dqs_div_o: std_logic;   
signal o_enable: std_logic; 
signal fvh_reg: std_logic_vector(2 downto 0);
signal sync_reset: std_logic;
--signal ddr2_casb   : std_logic;                     
--signal ddr2_cke    : std_logic;                     
--signal ddr2_clk0   : std_logic;                     
--signal ddr2_clk0b  : std_logic;                     
--signal ddr2_csb    : std_logic;                     
--signal ddr2_rasb   : std_logic;                     
--signal ddr2_web    : std_logic;                     
--signal ddr2_ODT0   : std_logic;                     
--signal ddr2_address: std_logic_VECTOR(12 downto 0); 
--signal ddr2_ba     : std_logic_VECTOR(1 downto 0);  
--signal ddr2_dm     : std_logic_VECTOR(1 downto 0);  
--signal ddr2_dqs    : std_logic_VECTOR(1 downto 0);
--signal ddr2_dqs_n  : std_logic_VECTOR(1 downto 0);
--signal ddr2_dq     : std_logic_VECTOR(15 downto 0);

begin
--------------------------------------------------------------
-- Differntial clock inputs:
--------------------------------------------------------------
diff_buf_brefclk : IBUFGDS
generic map (IOSTANDARD => "LVDS_25_DT") -- generic map (IOSTANDARD => "LVDS_25")
port map (
  		O  => brefclk,
  		I  => brefclk_p_i,
  		IB => brefclk_n_i
);


clk1_for_logic : BUFG
port map(
   		I => brefclk,
   		O => clk1
);
         

mem_interface_top_inst: mem_interface_top 
port map(
		dip1                    	 => '1',
		dip2                    	 => '1',
		dip3                    	 => '1',
		reset_in                	 => mreset_i,
		SYS_CLK            			 => clk1,
		clk_int_1          			 => new_clk,--led_o(3),   --open,
		clk90_int_1        			 => clk90,
		clk180_1           			 => clk180,
		clk270_1           			 => open,
		sys_rst_1       			 => open,      
		sys_rst90_1     			 => open,      
		sys_rst180_1    			 => open,      
		sys_rst270_1    			 => open,      
		cntrl0_rst_dqs_div_in    	 => rst_dqs_div_i,
		cntrl0_rst_dqs_div_out   	 => rst_dqs_div_i,
		
		cntrl0_ddr2_casb         	 => ddr2_casb,  
		cntrl0_ddr2_cke          	 => ddr2_cke,   
		cntrl0_ddr2_clk0  			 => ddr2_clk0,     
		cntrl0_ddr2_clk0b   		 => ddr2_clk0b,    
		cntrl0_ddr2_csb          	 => ddr2_csb,  
		cntrl0_ddr2_rasb         	 => ddr2_rasb, 
		cntrl0_ddr2_web          	 => ddr2_web,  
		cntrl0_ddr2_ODT0         	 => ddr2_ODT0, 
		cntrl0_ddr2_address      	 => ddr2_address, 
		cntrl0_ddr2_ba           	 => ddr2_ba,      
		cntrl0_ddr2_dm           	 => ddr2_dm,    
		cntrl0_ddr2_dqs          	 => ddr2_dqs,   
		cntrl0_ddr2_dqs_n          	 => ddr2_dqs_n, 
		cntrl0_ddr2_dq           	 => ddr2_dq,
				                             
		cntrl0_burst_done   		 => burst_done,   		                             
		cntrl0_auto_ref_req     	 => auto_ref_req,
		cntrl0_ar_done          	 => ar_done,
		cntrl0_init_val         	 => init_val,	

		cntrl0_user_cmd_ack     	 => user_cmd_ack,     	   
		cntrl0_user_data_valid  	 => user_data_valid,  	   
		cntrl0_user_output_data 	 => user_output_data, 	   
		cntrl0_user_input_address  	 => user_input_address,  	
		cntrl0_user_bank_address   	 => user_bank_address,   	
		cntrl0_user_input_data   	 => user_input_data,   	  
		cntrl0_user_data_mask   	 => user_data_mask,   	   
		cntrl0_user_config_register1 => user_config_register1,
		cntrl0_user_config_register2 => user_config_register2,
		cntrl0_user_command_register => user_command_register
); 
		
system_controller_inst: mem_controller     		   
port map(                           
		reset	        			 => mreset_i,    	           
		clk	                	     => new_clk,--clk1, 
		clk90						 => clk90,   
		clk180					     => clk180,                
		burst_done   			     => burst_done,         
		enable_i				         => o_enable,
		sync_reset_i         => sync_reset,                        
		user_input_address  	     => user_input_address,      
		user_bank_address   	     => user_bank_address,      
		user_input_data  		     => user_input_data,  	        
		user_data_mask   		     => user_data_mask,   	        
		                                
		auto_ref_req			     => auto_ref_req,	              
		ar_done          		     => ar_done,                
		user_cmd_ack     		     => user_cmd_ack,           
		user_data_valid  		     => user_data_valid,        
		user_output_data 		     => user_output_data,       
		init_val         		     => init_val,      
		fvh_i                  => fvh_reg,       
      luma_a_o			             => luma_a_o,
		chroma_a_o	             => chroma_a_o,                  
		user_config_register1	     => user_config_register1,    
		user_config_register2	     => user_config_register2,   
		user_command_register	     => user_command_register, 
		TP1 						 => led_o(0),  
		TP2						     => led_o(1)                
	);      

--led_o(3) <= cntrl0_ddr2_clk0;
--led_o(2) <= clk1;
--led_o(1) <= 

sim_oe: process(new_clk, mreset_i)
begin
    if mreset_i = '0' then
        o_enable <= '0';
        fvh_reg  <= "111";
        sync_reset <= '0';
    elsif new_clk'event and new_clk = '1' then
        fvh_reg <= fvh_i;
        sync_reset <= sync_reset_i;
       if enable_i = '1' then
           o_enable <=not o_enable;
        else
           o_enable <= '0';
    end if;
   end if;
end process;	
end Behavioral;                       