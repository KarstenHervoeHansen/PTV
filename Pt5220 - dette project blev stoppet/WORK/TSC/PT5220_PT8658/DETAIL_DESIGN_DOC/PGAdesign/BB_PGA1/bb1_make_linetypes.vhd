-- Navn			: BB1_MAKE_LINETYPES
-- Dato			: 99-06-
-- Rev.nr.		: 0.1
-- Funktion		: Makes the the different signaltypes vx,ux,sx,xv,xu to the
--				: control unit and p1-4 for the pedistal i 'M'.
-- Input		:  
-- Output		: 
-- Forfatter	: Torben Schorlemmer

LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_arith.all;




ENTITY BB1_MAKE_LINETYPES IS
	PORT(
		clk								:	IN	STD_LOGIC;
		pga1_bb1_line_type				:	IN	STD_LOGIC_VECTOR(3 downto 0);

		pga1_bb1_vx						:	OUT	STD_LOGIC;
		pga1_bb1_ux						:	OUT	STD_LOGIC;
		pga1_bb1_sx						:	OUT	STD_LOGIC;
		pga1_bb1_xv						:	OUT	STD_LOGIC;
		pga1_bb1_xu						:	OUT	STD_LOGIC;
		pga1_bb1_p1						:	OUT	STD_LOGIC;
		pga1_bb1_p2						:	OUT	STD_LOGIC;
		pga1_bb1_p3						:	OUT	STD_LOGIC;
		pga1_bb1_p4						:	OUT	STD_LOGIC;
		pga1_bb1_plin7					:	OUT	STD_LOGIC
		); 
END BB1_MAKE_LINETYPES;

ARCHITECTURE CONTROL_SIG_U_S_V OF BB1_MAKE_LINETYPES IS
	TYPE STATE_TYPE IS (s0_make_linetypes);
	SIGNAL state: STATE_TYPE;
BEGIN
	PROCESS (clk)
	BEGIN
		IF (clk'EVENT AND clk = '1') THEN
			CASE state IS
				WHEN s0_make_linetypes =>
					IF (pga1_bb1_line_type(3)='0') and ( pga1_bb1_line_type(1)='0')
						THEN pga1_bb1_vx <= '1';
						ELSE pga1_bb1_vx <= '0';
					END IF; 
					IF (pga1_bb1_line_type(3)='0') and ( pga1_bb1_line_type(1)='1')
						THEN pga1_bb1_ux <= '1';
						ELSE pga1_bb1_ux <= '0';
					END IF; 
					IF (pga1_bb1_line_type(3)='1')
						THEN pga1_bb1_sx <= '1';
						ELSE pga1_bb1_sx <= '0';
					END IF; 
					IF (pga1_bb1_line_type(3)='0') and 
						( pga1_bb1_line_type(2)='0') and
						( pga1_bb1_line_type(0)='0')
						THEN pga1_bb1_xv <= '1';
						ELSE pga1_bb1_xv <= '0';
					END IF; 
					IF (pga1_bb1_line_type(2)='0') and ( pga1_bb1_line_type(0)='1')
						THEN pga1_bb1_xu <= '1';
						ELSE pga1_bb1_xu <= '0';
					END IF; 
					IF (pga1_bb1_line_type(3)='1') and ( pga1_bb1_line_type(0)='1')
						THEN pga1_bb1_p1 <= '1';
						ELSE pga1_bb1_p1 <= '0';
					END IF; 
					IF (pga1_bb1_line_type(3)='1') and ( pga1_bb1_line_type(2)='0')
						 and ( pga1_bb1_line_type(0)='1')
						THEN pga1_bb1_p2 <= '1';
						ELSE pga1_bb1_p2 <= '0';
					END IF; 
					IF (pga1_bb1_line_type(3)='1') and ( pga1_bb1_line_type(2)='1')
						 and ( pga1_bb1_line_type(0)='0')
						THEN pga1_bb1_p3 <= '1';
						ELSE pga1_bb1_p3 <= '0';
					END IF; 
					IF (pga1_bb1_line_type(3)='1') and ( pga1_bb1_line_type(2)='1')
						THEN pga1_bb1_p4 <= '1';
						ELSE pga1_bb1_p4 <= '0';
					END IF; 
					IF (pga1_bb1_line_type(0)='1') and ( pga1_bb1_line_type(1)='1')
						and (pga1_bb1_line_type(2)='1') and ( pga1_bb1_line_type(3)='1')
					THEN pga1_bb1_plin7 <= '1';
						ELSE pga1_bb1_plin7 <= '0';
					END IF; 
			END CASE;
		END IF;
	END PROCESS;

END CONTROL_SIG_U_S_V;





