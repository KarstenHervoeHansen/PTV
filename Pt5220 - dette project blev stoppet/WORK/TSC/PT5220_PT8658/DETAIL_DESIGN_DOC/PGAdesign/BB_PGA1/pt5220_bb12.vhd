-- Navn			: combuf.vhd
-- Dato			: 26-11-96
-- Rev.nr.		: 1.0
-- Funktion		: Buffer udgange til comport og dspstrbsig..
-- Input		: interncomport,interndspstrb,startud
-- Output		: comport,dspstrbsig
-- Forfatter	: Torben Schorlemmer


LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;


ENTITY Pt5220_BB12 IS
	PORT(
		startud			:	IN		STD_LOGIC;
		interncomport	:	IN		STD_LOGIC_VECTOR(7 DOWNTO 0);
		interndspstrb	:	IN		STD_LOGIC;
		dspstrbsig		:	INOUT	STD_LOGIC;
		comport7		:	INOUT	STD_LOGIC;
		comport6		:	INOUT	STD_LOGIC;
		comport5		:	INOUT	STD_LOGIC;
		comport4		:	INOUT	STD_LOGIC;
		comport3		:	INOUT	STD_LOGIC;
		comport2		:	INOUT	STD_LOGIC;
		comport1		:	INOUT	STD_LOGIC;
		comport0		:	INOUT	STD_LOGIC
		);
END Pt5220_BB12;

ARCHITECTURE comhigh OF dspcomb IS

	TYPE regcounttype IS (s0,s1,s2,s3) ;
	SIGNAL strbcount		: regcounttype;

BEGIN
combuffer:PROCESS
	BEGIN
		IF startud = '0' THEN
								-- output buffer bli'r 3-state
			dspstrbsig <= 'Z';
			comport7 <= 'Z';
			comport6 <= 'Z';
			comport5 <= 'Z';
			comport4 <= 'Z';
			comport3 <= 'Z';
			comport2 <= 'Z';
			comport1 <= 'Z';
			comport0 <= 'Z';
		ELSE
			comport7 <= interncomport(7);
			comport6 <= interncomport(6);
			comport5 <= interncomport(5);
			comport4 <= interncomport(4);
			comport3 <= interncomport(3);
			comport2 <= interncomport(2);
			comport1 <= interncomport(1);
			comport0 <= interncomport(0);
			dspstrbsig <= interndspstrb;
		END IF;
END PROCESS combuffer;

END comhigh;
