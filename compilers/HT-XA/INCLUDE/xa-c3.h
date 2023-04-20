/*
 *	XA-C3 SFR definitions
 *	Extracted directly from Philips documentation
 */

#ifndef	__xa_h__
#define	__xa_h__

extern unsigned int	__trap(unsigned char, ...);
#pragma inline(__trap)

static unsigned char	M0MIDH	@ 0x000;		/* message 0 match ID high */
static unsigned char	M0MIDL	@ 0x002;		/* message 0 match ID low */
static unsigned char	M0MSKH	@ 0x004;		/* message 0 mask high */
static unsigned char	M0MSKL	@ 0x006;		/* message 0 mask low */
static unsigned char	M0CTRL	@ 0x008;		/* message 0 control */
static unsigned char	M0BLR	@ 0x00A;		/* message 0 buffer location */
static unsigned char	M0BSZ	@ 0x00C;		/* message 0 buffer size */
static unsigned char	M0FCR	@ 0x00E;		/* message 0 fragmentation count */
static unsigned char	M1MIDH	@ 0x010;		/* message 1 match ID high */
static unsigned char	M1MIDL	@ 0x012;		/* message 1 match ID low */
static unsigned char	M1MSKH	@ 0x014;		/* message 1 mask high */
static unsigned char	M1MSKL	@ 0x016;		/* message 1 mask low */
static unsigned char	M1CTRL	@ 0x018;		/* message 1 control */
static unsigned char	M1BLR	@ 0x01A;		/* message 1 buffer location */
static unsigned char	M1BSZ	@ 0x01C;		/* message 1 buffer size */
static unsigned char	M1FCR	@ 0x01E;		/* message 1 fragmentation count */
static unsigned char	M2MIDH	@ 0x020;		/* message 2 match ID high */
static unsigned char	M2MIDL	@ 0x022;		/* message 2 match ID low */
static unsigned char	M2MSKH	@ 0x024;		/* message 2 mask high */
static unsigned char	M2MSKL	@ 0x026;		/* message 2 mask low */
static unsigned char	M2CTRL	@ 0x028;		/* message 2 control */
static unsigned char	M2BLR	@ 0x02A;		/* message 2 buffer location */
static unsigned char	M2BSZ	@ 0x02C;		/* message 2 buffer size */
static unsigned char	M2FCR	@ 0x02E;		/* message 2 fragmentation count */
static unsigned char	M3MIDH	@ 0x030;		/* message 3 match ID high */
static unsigned char	M3MIDL	@ 0x032;		/* message 3 match ID low */
static unsigned char	M3MSKH	@ 0x034;		/* message 3 mask high */
static unsigned char	M3MSKL	@ 0x036;		/* message 3 mask low */
static unsigned char	M3CTRL	@ 0x038;		/* message 3 control */
static unsigned char	M3BLR	@ 0x03A;		/* message 3 buffer location */
static unsigned char	M3BSZ	@ 0x03C;		/* message 3 buffer size */
static unsigned char	M3FCR	@ 0x03E;		/* message 3 fragmentation count */
static unsigned char	M4MIDH	@ 0x040;		/* message 4 match ID high */
static unsigned char	M4MIDL	@ 0x042;		/* message 4 match ID low */
static unsigned char	M4MSKH	@ 0x044;		/* message 4 mask high */
static unsigned char	M4MSKL	@ 0x046;		/* message 4 mask low */
static unsigned char	M4CTRL	@ 0x048;		/* message 4 control */
static unsigned char	M4BLR	@ 0x04A;		/* message 4 buffer location */
static unsigned char	M4BSZ	@ 0x04C;		/* message 4 buffer size */
static unsigned char	M4FCR	@ 0x04E;		/* message 4 fragmentation count */
static unsigned char	M5MIDH	@ 0x050;		/* message 5 match ID high */
static unsigned char	M5MIDL	@ 0x052;		/* message 5 match ID low */
static unsigned char	M5MSKH	@ 0x054;		/* message 5 mask high */
static unsigned char	M5MSKL	@ 0x056;		/* message 5 mask low */
static unsigned char	M5CTRL	@ 0x058;		/* message 5 control */
static unsigned char	M5BLR	@ 0x05A;		/* message 5 buffer location */
static unsigned char	M5BSZ	@ 0x05C;		/* message 5 buffer size */
static unsigned char	M5FCR	@ 0x05E;		/* message 5 fragmentation count */
static unsigned char	M6MIDH	@ 0x060;		/* message 6 match ID high */
static unsigned char	M6MIDL	@ 0x062;		/* message 6 match ID low */
static unsigned char	M6MSKH	@ 0x064;		/* message 6 mask high */
static unsigned char	M6MSKL	@ 0x066;		/* message 6 mask low */
static unsigned char	M6CTRL	@ 0x068;		/* message 6 control */
static unsigned char	M6BLR	@ 0x06A;		/* message 6 buffer location */
static unsigned char	M6BSZ	@ 0x06C;		/* message 6 buffer size */
static unsigned char	M6FCR	@ 0x06E;		/* message 6 fragmentation count */
static unsigned char	M7MIDH	@ 0x070;		/* message 7 match ID high */
static unsigned char	M7MIDL	@ 0x072;		/* message 7 match ID low */
static unsigned char	M7MSKH	@ 0x074;		/* message 7 mask high */
static unsigned char	M7MSKL	@ 0x076;		/* message 7 mask low */
static unsigned char	M7CTRL	@ 0x078;		/* message 7 control */
static unsigned char	M7BLR	@ 0x07A;		/* message 7 buffer location */
static unsigned char	M7BSZ	@ 0x07C;		/* message 7 buffer size */
static unsigned char	M7FCR	@ 0x07E;		/* message 7 fragmentation count */
static unsigned char	M8MIDH	@ 0x080;		/* message 8 match ID high */
static unsigned char	M8MIDL	@ 0x082;		/* message 8 match ID low */
static unsigned char	M8MSKH	@ 0x084;		/* message 8 mask high */
static unsigned char	M8MSKL	@ 0x086;		/* message 8 mask low */
static unsigned char	M8CTRL	@ 0x088;		/* message 8 control */
static unsigned char	M8BLR	@ 0x08A;		/* message 8 buffer location */
static unsigned char	M8BSZ	@ 0x08C;		/* message 8 buffer size */
static unsigned char	M8FCR	@ 0x08E;		/* message 8 fragmentation count */
static unsigned char	M9MIDH	@ 0x090;		/* message 9 match ID high */
static unsigned char	M9MIDL	@ 0x092;		/* message 9 match ID low */
static unsigned char	M9MSKH	@ 0x094;		/* message 9 mask high */
static unsigned char	M9MSKL	@ 0x096;		/* message 9 mask low */
static unsigned char	M9CTRL	@ 0x098;		/* message 9 control */
static unsigned char	M9BLR	@ 0x09A;		/* message 9 buffer location */
static unsigned char	M9BSZ	@ 0x09C;		/* message 9 buffer size */
static unsigned char	M9FCR	@ 0x09E;		/* message 9 fragmentation count */
static unsigned char	M10MIDH	@ 0x0A0;		/* message 10 match ID high */
static unsigned char	M10MIDL	@ 0x0A2;		/* message 10 match ID low */
static unsigned char	M10MSKH	@ 0x0A4;		/* message 10 mask high */
static unsigned char	M10MSKL	@ 0x0A6;		/* message 10 mask low */
static unsigned char	M10CTRL	@ 0x0A8;		/* message 10 control */
static unsigned char	M10BLR	@ 0x0AA;		/* message 10 buffer location */
static unsigned char	M10BSZ	@ 0x0AC;		/* message 10 buffer size */
static unsigned char	M10FCR	@ 0x0AE;		/* message 10 fragmentation count */
static unsigned char	M11MIDH	@ 0x0B0;		/* message 11 match ID high */
static unsigned char	M11MIDL	@ 0x0B2;		/* message 11 match ID low */
static unsigned char	M11MSKH	@ 0x0B4;		/* message 11 mask high */
static unsigned char	M11MSKL	@ 0x0B6;		/* message 11 mask low */
static unsigned char	M11CTRL	@ 0x0B8;		/* message 11 control */
static unsigned char	M11BLR	@ 0x0BA;		/* message 11 buffer location */
static unsigned char	M11BSZ	@ 0x0BC;		/* message 11 buffer size */
static unsigned char	M11FCR	@ 0x0BE;		/* message 11 fragmentation count */
static unsigned char	M12MIDH	@ 0x0C0;		/* message 12 match ID high */
static unsigned char	M12MIDL	@ 0x0C2;		/* message 12 match ID low */
static unsigned char	M12MSKH	@ 0x0C4;		/* message 12 mask high */
static unsigned char	M12MSKL	@ 0x0C6;		/* message 12 mask low */
static unsigned char	M12CTRL	@ 0x0C8;		/* message 12 control */
static unsigned char	M12BLR	@ 0x0CA;		/* message 12 buffer location */
static unsigned char	M12BSZ	@ 0x0CC;		/* message 12 buffer size */
static unsigned char	M12FCR	@ 0x0CE;		/* message 12 fragmentation count */
static unsigned char	M13MIDH	@ 0x0D0;		/* message 13 match ID high */
static unsigned char	M13MIDL	@ 0x0D2;		/* message 13 match ID low */
static unsigned char	M13MSKH	@ 0x0D4;		/* message 13 mask high */
static unsigned char	M13MSKL	@ 0x0D6;		/* message 13 mask low */
static unsigned char	M13CTRL	@ 0x0D8;		/* message 13 control */
static unsigned char	M13BLR	@ 0x0DA;		/* message 13 buffer location */
static unsigned char	M13BSZ	@ 0x0DC;		/* message 13 buffer size */
static unsigned char	M13FCR	@ 0x0DE;		/* message 13 fragmentation count */
static unsigned char	M14MIDH	@ 0x0E0;		/* message 14 match ID high */
static unsigned char	M14MIDL	@ 0x0E2;		/* message 14 match ID low */
static unsigned char	M14MSKH	@ 0x0E4;		/* message 14 mask high */
static unsigned char	M14MSKL	@ 0x0E6;		/* message 14 mask low */
static unsigned char	M14CTRL	@ 0x0E8;		/* message 14 control */
static unsigned char	M14BLR	@ 0x0EA;		/* message 14 buffer location */
static unsigned char	M14BSZ	@ 0x0EC;		/* message 14 buffer size */
static unsigned char	M14FCR	@ 0x0EE;		/* message 14 fragmentation count */
static unsigned char	M15MIDH	@ 0x0F0;		/* message 15 match ID high */
static unsigned char	M15MIDL	@ 0x0F2;		/* message 15 match ID low */
static unsigned char	M15MSKH	@ 0x0F4;		/* message 15 mask high */
static unsigned char	M15MSKL	@ 0x0F6;		/* message 15 mask low */
static unsigned char	M15CTRL	@ 0x0F8;		/* message 15 control */
static unsigned char	M15BLR	@ 0x0FA;		/* message 15 buffer location */
static unsigned char	M15BSZ	@ 0x0FC;		/* message 15 buffer size */
static unsigned char	M15FCR	@ 0x0FE;		/* message 15 fragmentation count */
static unsigned char	M16MIDH	@ 0x100;		/* message 16 match ID high */
static unsigned char	M16MIDL	@ 0x102;		/* message 16 match ID low */
static unsigned char	M16MSKH	@ 0x104;		/* message 16 mask high */
static unsigned char	M16MSKL	@ 0x106;		/* message 16 mask low */
static unsigned char	M16CTRL	@ 0x108;		/* message 16 control */
static unsigned char	M16BLR	@ 0x10A;		/* message 16 buffer location */
static unsigned char	M16BSZ	@ 0x10C;		/* message 16 buffer size */
static unsigned char	M16FCR	@ 0x10E;		/* message 16 fragmentation count */
static unsigned char	M17MIDH	@ 0x110;		/* message 17 match ID high */
static unsigned char	M17MIDL	@ 0x112;		/* message 17 match ID low */
static unsigned char	M17MSKH	@ 0x114;		/* message 17 mask high */
static unsigned char	M17MSKL	@ 0x116;		/* message 17 mask low */
static unsigned char	M17CTRL	@ 0x118;		/* message 17 control */
static unsigned char	M17BLR	@ 0x11A;		/* message 17 buffer location */
static unsigned char	M17BSZ	@ 0x11C;		/* message 17 buffer size */
static unsigned char	M17FCR	@ 0x11E;		/* message 17 fragmentation count */
static unsigned char	M18MIDH	@ 0x120;		/* message 18 match ID high */
static unsigned char	M18MIDL	@ 0x122;		/* message 18 match ID low */
static unsigned char	M18MSKH	@ 0x124;		/* message 18 mask high */
static unsigned char	M18MSKL	@ 0x126;		/* message 18 mask low */
static unsigned char	M18CTRL	@ 0x128;		/* message 18 control */
static unsigned char	M18BLR	@ 0x12A;		/* message 18 buffer location */
static unsigned char	M18BSZ	@ 0x12C;		/* message 18 buffer size */
static unsigned char	M18FCR	@ 0x12E;		/* message 18 fragmentation count */
static unsigned char	M19MIDH	@ 0x130;		/* message 19 match ID high */
static unsigned char	M19MIDL	@ 0x132;		/* message 19 match ID low */
static unsigned char	M19MSKH	@ 0x134;		/* message 19 mask high */
static unsigned char	M19MSKL	@ 0x136;		/* message 19 mask low */
static unsigned char	M19CTRL	@ 0x138;		/* message 19 control */
static unsigned char	M19BLR	@ 0x13A;		/* message 19 buffer location */
static unsigned char	M19BSZ	@ 0x13C;		/* message 19 buffer size */
static unsigned char	M19FCR	@ 0x13E;		/* message 19 fragmentation count */
static unsigned char	M20MIDH	@ 0x140;		/* message 20 match ID high */
static unsigned char	M20MIDL	@ 0x142;		/* message 20 match ID low */
static unsigned char	M20MSKH	@ 0x144;		/* message 20 mask high */
static unsigned char	M20MSKL	@ 0x146;		/* message 20 mask low */
static unsigned char	M20CTRL	@ 0x148;		/* message 20 control */
static unsigned char	M20BLR	@ 0x14A;		/* message 20 buffer location */
static unsigned char	M20BSZ	@ 0x14C;		/* message 20 buffer size */
static unsigned char	M20FCR	@ 0x14E;		/* message 20 fragmentation count */
static unsigned char	M21MIDH	@ 0x150;		/* message 21 match ID high */
static unsigned char	M21MIDL	@ 0x152;		/* message 21 match ID low */
static unsigned char	M21MSKH	@ 0x154;		/* message 21 mask high */
static unsigned char	M21MSKL	@ 0x156;		/* message 21 mask low */
static unsigned char	M21CTRL	@ 0x158;		/* message 21 control */
static unsigned char	M21BLR	@ 0x15A;		/* message 21 buffer location */
static unsigned char	M21BSZ	@ 0x15C;		/* message 21 buffer size */
static unsigned char	M21FCR	@ 0x15E;		/* message 21 fragmentation count */
static unsigned char	M22MIDH	@ 0x160;		/* message 22 match ID high */
static unsigned char	M22MIDL	@ 0x162;		/* message 22 match ID low */
static unsigned char	M22MSKH	@ 0x164;		/* message 22 mask high */
static unsigned char	M22MSKL	@ 0x166;		/* message 22 mask low */
static unsigned char	M22CTRL	@ 0x168;		/* message 22 control */
static unsigned char	M22BLR	@ 0x16A;		/* message 22 buffer location */
static unsigned char	M22BSZ	@ 0x16C;		/* message 22 buffer size */
static unsigned char	M22FCR	@ 0x16E;		/* message 22 fragmentation count */
static unsigned char	M23MIDH	@ 0x170;		/* message 23 match ID high */
static unsigned char	M23MIDL	@ 0x172;		/* message 23 match ID low */
static unsigned char	M23MSKH	@ 0x174;		/* message 23 mask high */
static unsigned char	M23MSKL	@ 0x176;		/* message 23 mask low */
static unsigned char	M23CTRL	@ 0x178;		/* message 23 control */
static unsigned char	M23BLR	@ 0x17A;		/* message 23 buffer location */
static unsigned char	M23BSZ	@ 0x17C;		/* message 23 buffer size */
static unsigned char	M23FCR	@ 0x17E;		/* message 23 fragmentation count */
static unsigned char	M24MIDH	@ 0x180;		/* message 24 match ID high */
static unsigned char	M24MIDL	@ 0x182;		/* message 24 match ID low */
static unsigned char	M24MSKH	@ 0x184;		/* message 24 mask high */
static unsigned char	M24MSKL	@ 0x186;		/* message 24 mask low */
static unsigned char	M24CTRL	@ 0x188;		/* message 24 control */
static unsigned char	M24BLR	@ 0x18A;		/* message 24 buffer location */
static unsigned char	M24BSZ	@ 0x18C;		/* message 24 buffer size */
static unsigned char	M24FCR	@ 0x18E;		/* message 24 fragmentation count */
static unsigned char	M25MIDH	@ 0x190;		/* message 25 match ID high */
static unsigned char	M25MIDL	@ 0x192;		/* message 25 match ID low */
static unsigned char	M25MSKH	@ 0x194;		/* message 25 mask high */
static unsigned char	M25MSKL	@ 0x196;		/* message 25 mask low */
static unsigned char	M25CTRL	@ 0x198;		/* message 25 control */
static unsigned char	M25BLR	@ 0x19A;		/* message 25 buffer location */
static unsigned char	M25BSZ	@ 0x19C;		/* message 25 buffer size */
static unsigned char	M25FCR	@ 0x19E;		/* message 25 fragmentation count */
static unsigned char	M26MIDH	@ 0x1A0;		/* message 26 match ID high */
static unsigned char	M26MIDL	@ 0x1A2;		/* message 26 match ID low */
static unsigned char	M26MSKH	@ 0x1A4;		/* message 26 mask high */
static unsigned char	M26MSKL	@ 0x1A6;		/* message 26 mask low */
static unsigned char	M26CTRL	@ 0x1A8;		/* message 26 control */
static unsigned char	M26BLR	@ 0x1AA;		/* message 26 buffer location */
static unsigned char	M26BSZ	@ 0x1AC;		/* message 26 buffer size */
static unsigned char	M26FCR	@ 0x1AE;		/* message 26 fragmentation count */
static unsigned char	M27MIDH	@ 0x1B0;		/* message 27 match ID high */
static unsigned char	M27MIDL	@ 0x1B2;		/* message 27 match ID low */
static unsigned char	M27MSKH	@ 0x1B4;		/* message 27 mask high */
static unsigned char	M27MSKL	@ 0x1B6;		/* message 27 mask low */
static unsigned char	M27CTRL	@ 0x1B8;		/* message 27 control */
static unsigned char	M27BLR	@ 0x1BA;		/* message 27 buffer location */
static unsigned char	M27BSZ	@ 0x1BC;		/* message 27 buffer size */
static unsigned char	M27FCR	@ 0x1BE;		/* message 27 fragmentation count */
static unsigned char	M28MIDH	@ 0x1C0;		/* message 28 match ID high */
static unsigned char	M28MIDL	@ 0x1C2;		/* message 28 match ID low */
static unsigned char	M28MSKH	@ 0x1C4;		/* message 28 mask high */
static unsigned char	M28MSKL	@ 0x1C6;		/* message 28 mask low */
static unsigned char	M28CTRL	@ 0x1C8;		/* message 28 control */
static unsigned char	M28BLR	@ 0x1CA;		/* message 28 buffer location */
static unsigned char	M28BSZ	@ 0x1CC;		/* message 28 buffer size */
static unsigned char	M28FCR	@ 0x1CE;		/* message 28 fragmentation count */
static unsigned char	M29MIDH	@ 0x1D0;		/* message 29 match ID high */
static unsigned char	M29MIDL	@ 0x1D2;		/* message 29 match ID low */
static unsigned char	M29MSKH	@ 0x1D4;		/* message 29 mask high */
static unsigned char	M29MSKL	@ 0x1D6;		/* message 29 mask low */
static unsigned char	M29CTRL	@ 0x1D8;		/* message 29 control */
static unsigned char	M29BLR	@ 0x1DA;		/* message 29 buffer location */
static unsigned char	M29BSZ	@ 0x1DC;		/* message 29 buffer size */
static unsigned char	M29FCR	@ 0x1DE;		/* message 29 fragmentation count */
static unsigned char	M30MIDH	@ 0x1E0;		/* message 30 match ID high */
static unsigned char	M30MIDL	@ 0x1E2;		/* message 30 match ID low */
static unsigned char	M30MSKH	@ 0x1E4;		/* message 30 mask high */
static unsigned char	M30MSKL	@ 0x1E6;		/* message 30 mask low */
static unsigned char	M30CTRL	@ 0x1E8;		/* message 30 control */
static unsigned char	M30BLR	@ 0x1EA;		/* message 30 buffer location */
static unsigned char	M30BSZ	@ 0x1EC;		/* message 30 buffer size */
static unsigned char	M30FCR	@ 0x1EE;		/* message 30 fragmentation count */
static unsigned char	M31MIDH	@ 0x1F0;		/* message 31 match ID high */
static unsigned char	M31MIDL	@ 0x1F2;		/* message 31 match ID low */
static unsigned char	M31MSKH	@ 0x1F4;		/* message 31 mask high */
static unsigned char	M31MSKL	@ 0x1F6;		/* message 31 mask low */
static unsigned char	M31CTRL	@ 0x1F8;		/* message 31 control */
static unsigned char	M31BLR	@ 0x1FA;		/* message 31 buffer location */
static unsigned char	M31BSZ	@ 0x1FC;		/* message 31 buffer size */
static unsigned char	M31FCR	@ 0x1FE;		/* message 31 fragmentation count */
static unsigned char	MCPLL	@ 0x224;		/* message complete status flags low */
static unsigned char	MCPLH	@ 0x226;		/* message complete status flags high */
static unsigned char	CANINTFLG	@ 0x228;		/* CAN interrupt flag register */
static unsigned char	MCIR	@ 0x229;		/* message complete information */
static unsigned char	MEIR	@ 0x22A;		/* message error information */
static unsigned char	FESTR	@ 0x22C;		/* frame error status */
static unsigned char	FEENR	@ 0x22E;		/* frame error enable */
static unsigned char	SPICFG	@ 0x260;		/* SPI configuration */
static unsigned char	SPIDATA	@ 0x262;		/* SPI data */
static unsigned char	SPICS	@ 0x263;		/* SPI control and status */
static unsigned char	CANCMR	@ 0x270;		/* CAN core command */
static unsigned char	CANSTR	@ 0x271;		/* CAN core status */
static unsigned char	CANBTR	@ 0x272;		/* CAN core bus timing */
static unsigned char	TXERC	@ 0x274;		/* tx error counter */
static unsigned char	RXERC	@ 0x275;		/* rx error counter */
static unsigned char	EWLR	@ 0x276;		/* error warning limit */
static unsigned char	ECCR	@ 0x278;		/* error code capture */
static unsigned char	ALCR	@ 0x27A;		/* arbitration lost capture */
static unsigned char	GCTL	@ 0x27E;		/* global control */
static unsigned char	XRAMB	@ 0x290;		/* xram base address */
static unsigned char	MBXSR	@ 0x291;		/* message buffer and XRAM segment register */
static unsigned char	MIFBTRL	@ 0x292;		/* MIF bus timing register low */
static unsigned char	MIFBTRH	@ 0x293;		/* MIF bus timing register high */
static unsigned char	PSWL	@ 0x400;		/* program status word low */
static unsigned char	PSWH	@ 0x401;		/* program status word high */
static unsigned char	PSW51	@ 0x402;		/* 80c51-compatible PSW */
static unsigned char	SSEL	@ 0x403;		/* segment selection register */
static unsigned char	PCON	@ 0x404;		/* power control register */
static unsigned char	TCON	@ 0x410;		/* timer 0 and 1 control register */
static unsigned char	TSTAT	@ 0x411;		/* timer 0 and 1 extended status */
static unsigned char	T2CON	@ 0x418;		/* timer 2 control register */
static unsigned char	T2MOD	@ 0x419;		/* timer 2 mode control */
static unsigned char	WDCON	@ 0x41F;		/* watchdog control register */
static unsigned char	S0CON	@ 0x420;		/* serial port 0 control register */
static unsigned char	S0STAT	@ 0x421;		/* serial port 0 extended status */
static unsigned char	IEL	@ 0x426;		/* interrupt enable low */
static unsigned char	IEH	@ 0x427;		/* interrupt enable high */
static unsigned char	SWR	@ 0x42A;		/* software interrupt request */
static unsigned char	P0	@ 0x430;		/* port 0 */
static unsigned char	P1	@ 0x431;		/* port 1 */
static unsigned char	P2	@ 0x432;		/* port 2 */
static unsigned char	P3	@ 0x433;		/* port 3 */
static unsigned char	SCR	@ 0x440;		/* system configuration register */
static unsigned char	DS	@ 0x441;		/* data segment */
static unsigned char	ES	@ 0x442;		/* extra segment */
static unsigned char	CS	@ 0x443;		/* code segment */
static unsigned char	TL0	@ 0x450;		/* timer 0 low byte */
static unsigned char	TH0	@ 0x451;		/* timer 0 high byte */
static unsigned char	TL1	@ 0x452;		/* timer 1 low byte */
static unsigned char	TH1	@ 0x453;		/* timer 1 high byte */
static unsigned char	RTL0	@ 0x454;		/* timer 0 extended reload low byte */
static unsigned char	RTH0	@ 0x455;		/* timer 0 extended reload high byte */
static unsigned char	RTL1	@ 0x456;		/* timer 1 extended reload low byte */
static unsigned char	RTH1	@ 0x457;		/* timer 1 extended reload high byte */
static unsigned char	TL2	@ 0x458;		/* timer 2 low byte */
static unsigned char	TH2	@ 0x459;		/* timer 2 high byte */
static unsigned char	T2CAPL	@ 0x45A;		/* timer 2 capture register, low byte */
static unsigned char	T2CAPH	@ 0x45B;		/* timer 2 capture register, high byte */
static unsigned char	TMOD	@ 0x45C;		/* timer 0 and 1 mode control */
static unsigned char	WFEED1	@ 0x45D;		/* watchdog feed 1 */
static unsigned char	WFEED2	@ 0x45E;		/* watchdog feed 2 */
static unsigned char	WDL	@ 0x45F;		/* watchdog timer reload */
static unsigned char	S0BUF	@ 0x460;		/* serial port 0 buffer register */
static unsigned char	S0ADDR	@ 0x461;		/* serial port 0 address register */
static unsigned char	S0ADEN	@ 0x462;		/* serial port 0 address enable register */
static unsigned char	BTRL	@ 0x468;		/* bus timing register low */
static unsigned char	BTRH	@ 0x469;		/* bus timing register high */
static unsigned char	BCR	@ 0x46A;		/* bus configuration register */
static unsigned char	P0CFGA	@ 0x470;		/* port 0 configuration a */
static unsigned char	P1CFGA	@ 0x471;		/* port 1 configuration a */
static unsigned char	P2CFGA	@ 0x472;		/* port 2 configuration a */
static unsigned char	P3CFGA	@ 0x473;		/* port 3 configuration a */
static unsigned char	SWE	@ 0x47A;		/* software enable */
static unsigned char	MIFCNTL	@ 0x495;		/* MIF control register */
static unsigned char	MRBL	@ 0x496;		/* MMR base address low */
static unsigned char	MRBH	@ 0x497;		/* MMR base address high */
static unsigned char	IPA0	@ 0x4A0;		/* interrupt priotity assignment 0 */
static unsigned char	IPA1	@ 0x4A1;		/* interrupt priotity assignment 1 */
static unsigned char	IPA2	@ 0x4A2;		/* interrupt priotity assignment 2 */
static unsigned char	IPA3	@ 0x4A3;		/* interrupt priotity assignment 3 */
static unsigned char	IPA4	@ 0x4A4;		/* interrupt priotity assignment 4 */
static unsigned char	IPA5	@ 0x4A5;		/* interrupt priotity assignment 5 */
static unsigned char	IPA6	@ 0x4A6;		/* interrupt priotity assignment 6 */
static unsigned char	IPA7	@ 0x4A7;		/* interrupt priotity assignment 7 */
static unsigned char	P0CFGB	@ 0x4F0;		/* port 0 configuration b */
static unsigned char	P1CFGB	@ 0x4F1;		/* port 1 configuration b */
static unsigned char	P2CFGB	@ 0x4F2;		/* port 2 configuration b */
static unsigned char	P3CFGB	@ 0x4F3;		/* port 3 configuration b */

 /* Addressable Bits */
static bit		ZERO	@ 0x200;		/* */
static bit		NEGATIVE	@ 0x201;		/* */
static bit		OVERFLOW	@ 0x202;		/* */
static bit		AC	@ 0x206;		/* */
static bit		CARRY	@ 0x207;		/* */
static bit		IM0	@ 0x208;		/* */
static bit		IM1	@ 0x209;		/* */
static bit		IM2	@ 0x20A;		/* */
static bit		IM3	@ 0x20B;		/* */
static bit		RS0	@ 0x20C;		/* */
static bit		RS1	@ 0x20D;		/* */
static bit		TM	@ 0x20E;		/* */
static bit		SM	@ 0x20F;		/* */
static bit		P_51	@ 0x210;		/* */
static bit		F1_51	@ 0x211;		/* */
static bit		V_51	@ 0x212;		/* */
static bit		RS0_51	@ 0x213;		/* */
static bit		RS1_51	@ 0x214;		/* */
static bit		F0_51	@ 0x215;		/* */
static bit		AC_51	@ 0x216;		/* */
static bit		C_51	@ 0x217;		/* */
static bit		R0SEG	@ 0x218;		/* */
static bit		R1SEG	@ 0x219;		/* */
static bit		R2SEG	@ 0x21A;		/* */
static bit		R3SEG	@ 0x21B;		/* */
static bit		R4SEG	@ 0x21C;		/* */
static bit		R5SEG	@ 0x21D;		/* */
static bit		R6SEG	@ 0x21E;		/* */
static bit		ESWEN	@ 0x21F;		/* */
static bit		IDL	@ 0x220;		/* */
static bit		PD	@ 0x221;		/* */
static bit		IT0	@ 0x280;		/* */
static bit		IE0	@ 0x281;		/* */
static bit		IT1	@ 0x282;		/* */
static bit		IE1	@ 0x283;		/* */
static bit		TR0	@ 0x284;		/* */
static bit		TF0	@ 0x285;		/* */
static bit		TR1	@ 0x286;		/* */
static bit		TF1	@ 0x287;		/* */
static bit		T0OE	@ 0x288;		/* */
static bit		T1OE	@ 0x28A;		/* */
static bit		DCEN	@ 0x2C8;		/* */
static bit		T2OE	@ 0x2C9;		/* */
static bit		WDTOF	@ 0x2F9;		/* */
static bit		WDRUN	@ 0x2FA;		/* */
static bit		PRE0	@ 0x2FD;		/* */
static bit		PRE1	@ 0x2FE;		/* */
static bit		PRE2	@ 0x2FF;		/* */
static bit		RI_0	@ 0x300;		/* */
static bit		TI_0	@ 0x301;		/* */
static bit		RB8_0	@ 0x302;		/* */
static bit		TB8_0	@ 0x303;		/* */
static bit		REN_0	@ 0x304;		/* */
static bit		SM2_0	@ 0x305;		/* */
static bit		SM1_0	@ 0x306;		/* */
static bit		SM0_0	@ 0x307;		/* */
static bit		STINT0	@ 0x308;		/* */
static bit		OE0	@ 0x309;		/* */
static bit		BR0	@ 0x30A;		/* */
static bit		FE0	@ 0x30B;		/* */
static bit		EX0	@ 0x330;		/* */
static bit		ET0	@ 0x331;		/* */
static bit		EX1	@ 0x332;		/* */
static bit		ET1	@ 0x333;		/* */
static bit		ET2	@ 0x334;		/* */
static bit		EBUFF	@ 0x335;		/* */
static bit		EA	@ 0x337;		/* */
static bit		ERIO	@ 0x338;		/* */
static bit		ETIO	@ 0x339;		/* */
static bit		ESPI	@ 0x33B;		/* */
static bit		ECER	@ 0x33C;		/* */
static bit		EMER	@ 0x33D;		/* */
static bit		EMTI	@ 0x33E;		/* */
static bit		EMRI	@ 0x33F;		/* */
static bit		SWR1	@ 0x350;		/* */
static bit		SWR2	@ 0x351;		/* */
static bit		SWR3	@ 0x352;		/* */
static bit		SWR4	@ 0x353;		/* */
static bit		SWR5	@ 0x354;		/* */
static bit		SWR6	@ 0x355;		/* */
static bit		SWR7	@ 0x356;		/* */
static bit		A4D0	@ 0x380;		/* */
static bit		A5D1	@ 0x381;		/* */
static bit		A6D2	@ 0x382;		/* */
static bit		A7D3	@ 0x383;		/* */
static bit		A8D4	@ 0x384;		/* */
static bit		A9D5	@ 0x385;		/* */
static bit		A10D6	@ 0x386;		/* */
static bit		A11D7	@ 0x387;		/* */
static bit		WRH	@ 0x388;		/* */
static bit		A1	@ 0x389;		/* */
static bit		A2	@ 0x38A;		/* */
static bit		A3	@ 0x38B;		/* */
static bit		SPIRX	@ 0x38C;		/* */
static bit		SPITX	@ 0x38D;		/* */
static bit		SPICLK	@ 0x38E;		/* */
/* alternate function */ 
static bit		T2	@ 0x38E;		/* */
static bit		T2EX	@ 0x38F;		/* */
static bit		A12D8	@ 0x390;		/* */
static bit		A13D9	@ 0x391;		/* */
static bit		A14D10	@ 0x392;		/* */
static bit		A15D11	@ 0x393;		/* */
static bit		A16D12	@ 0x394;		/* */
static bit		A17D13	@ 0x395;		/* */
static bit		A18D14	@ 0x396;		/* */
static bit		A19D15	@ 0x397;		/* */
static bit		RXD0	@ 0x398;		/* */
static bit		TXD0	@ 0x399;		/* */
static bit		INT0	@ 0x39A;		/* */
static bit		INT1	@ 0x39B;		/* */
static bit		T0	@ 0x39C;		/* */
static bit		T1	@ 0x39D;		/* */
static bit		WRL	@ 0x39E;		/* */
static bit		RD	@ 0x39F;		/* */

/* Interrupt Vectors */

/* Exception interrupt vectors */ 
#define IV_RESET   	0x0000	/* Reset */
#define IV_BKPNT   	0x0004	/* Breakpoint */
#define IV_TRACE   	0x0008	/* Trace */
#define IV_STKOVER   	0x000C	/* Stack overflow */
#define IV_DIVZERO   	0x0010	/* Divide by zero */
#define IV_RETI   	0x0014	/* User RETI */

/* Trap interrupt vectors */ 
#define IV_TRI0   	0x40	/* Software trap 0 */
#define IV_TRI1   	0x44	/* Software trap 1 */
#define IV_TRI2   	0x48	/* Software trap 2 */
#define IV_TRI3   	0x4C	/* Software trap 3 */
#define IV_TRI4   	0x50	/* Software trap 4 */
#define IV_TRI5   	0x54	/* Software trap 5 */
#define IV_TRI6   	0x58	/* Software trap 6 */
#define IV_TRI7   	0x5C	/* Software trap 7 */
#define IV_TRI8   	0x60	/* Software trap 8 */
#define IV_TRI9   	0x64	/* Software trap 9 */
#define IV_TRI10   	0x68	/* Software trap 10 */
#define IV_TRI11   	0x6C	/* Software trap 11 */
#define IV_TRI12   	0x70	/* Software trap 12 */
#define IV_TRI13   	0x74	/* Software trap 13 */
#define IV_TRI14   	0x78	/* Software trap 14 */
#define IV_TRI15   	0x7C	/* Software trap 15 */

/* Event interrupt vectors */ 
#define IV_EX0   	0x80	/* External interrupt 0 */
#define IV_T0   	0x84	/* Timer 0 interrupt */
#define IV_EX1   	0x88	/* External interrupt 1 */
#define IV_T1   	0x8C	/* timer 1 interrupt */
#define IV_T2   	0x90	/* timer 2 interrupt */
#define IV_RXFULL   	0x94	/* (CAN) Rx buffer full */
#define IV_RI0   	0xA0	/* Serial port 0 Rx */
#define IV_TI0   	0xA4	/* Serial port 0 Tx */
#define IV_SPI   	0xAC	/* SPI interrupt */
#define IV_FERR   	0xB0	/* (CAN) Frame error */
#define IV_MERR   	0xB4	/* (CAN) Message error */
#define IV_TXDONE   	0xB8	/* Tx message complete */
#define IV_RXDONE   	0xBC	/* Rx message complete */

/* Software interrupt vectors */ 
#define IV_SWI1   	0x100	/* Software interrupt 1 */
#define IV_SWI2   	0x104	/* Software interrupt 2 */
#define IV_SWI3   	0x108	/* Software interrupt 3 */
#define IV_SWI4   	0x10C	/* Software interrupt 4 */
#define IV_SWI5   	0x110	/* Software interrupt 5 */
#define IV_SWI6   	0x114	/* Software interrupt 6 */
#define IV_SWI7   	0x118	/* Software interrupt 7 */

/* PSW Values for interrupt vectors */

#define	IV_PSW		0x8F00	/* System mode, high priority, bank 0 */

#define	IV_SYSTEM	0x8000

#define	IV_PRI00	0x0000	/* priorities 0 - 15 */
#define	IV_PRI01	0x0100
#define	IV_PRI02	0x0200
#define	IV_PRI03	0x0300
#define	IV_PRI04	0x0400
#define	IV_PRI05	0x0500
#define	IV_PRI06	0x0600
#define	IV_PRI07	0x0700
#define	IV_PRI08	0x0800
#define	IV_PRI09	0x0900
#define	IV_PRI10	0x0A00
#define	IV_PRI11	0x0B00
#define	IV_PRI12	0x0C00
#define	IV_PRI13	0x0D00
#define	IV_PRI14	0x0E00
#define	IV_PRI15	0x0F00

#define	IV_BANK0	0x0000
#define	IV_BANK1	0x1000
#define	IV_BANK2	0x2000
#define	IV_BANK3	0x3000

#endif	/* __xa_h__ */

