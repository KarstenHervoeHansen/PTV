m_s_1560 <= '0';      -- ( master mode = '1' , slave mode = '0' )

-- DVBASI, SD/HD, RCbyp and SMPTEbyp is configured as outputs in master mode
-- da1560_sig <= d_a1560;
-- sd_hd_1560_sig <= sd_hd_1560;
-- rcbyp_sig <= rcbyp;
-- smptebp1560_sig <= smptebp1560;
    
-- DVBASI, SD/HD, RCbyp and SMPTEbyp is configured as inputs in slave mode
d_a1560 <= '0';     -- (DVB/ASI pin) no decodong or word alignment of recieved DVB/ASI data if LOW
sd_hd_1560 <= '0';  -- (SD/HD pin)( standard def = 1 , high def = 0 )
rcbyp <= '1';       -- reclocker bypass - active LOW
smptebp1560 <= '1'; -- (SMPTEbyp pin) carry out descrambling and word alignment if high ( DVB/ASI must be low )


iop1560 <= '1';     -- IO processing enable - active HIGH
fwen_dis <= '1';    -- Flywheel disable - active LOW
busw_1560 <= '1';   -- ( 10 bit buswidth = 0 , 20 bit buswidth = 1 )
cs_1560 <= '1';     -- chip select - active LOW