/***************************************************************************/
/*  Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997             */
/*  Project:    PM5639 Display Unit                           */
/*  Module:   SUB.H                                     */
/*  Author:   Kim Engedahl, DEV                             */
/*  Org. date:  951017                                    */
/*  Rev. date:  970822 KEn  DEV                             */
/*  Status:   Version 2.3                                 */
/*                                                  */
/*  Changes:                                            */
/*  970822 Released as version 2.3                            */
/*  951207 Released as version 2.2                            */
/***************************************************************************/

extern void putcmd( char);
extern void putdat( unsigned char);
extern unsigned int readcuradr();
extern void setcuradr( unsigned int adr);
extern bit putpixel( char x, char y, char plan, char mode);
extern void clrcross();
extern void putcross( int dx, int dy);
extern void outchar( char c);
extern void outtext( char *s);
extern void outtextxy( char x, char y, char *s);
extern void textbar( char y, char mode);
extern void clrtext( char y);
extern void vline( char x, char ymin, char ymax, char mode);
extern void hline( char xmin, char xmax, char y, char mode);
extern void cls();
extern void drawbar( char barposx, char zoom, float val);
extern void strput( char *txt, char *target);
extern char* ftoa( float f, char d);
extern void settestmenu();
extern void batt_warn( char x, char y);
extern void recall_whiteref( char no);
extern void store_whiteref( char no);
extern void recall_cfactor( char no);
extern void store_cfactor( char no);
extern void set_measunit( char no);
extern void store_setup( char no);
extern void recall_setup( char no);
extern void store_msm( char no);
extern void recall_msm( char no);
extern void invert_k();
extern void rgbref( char no);
extern void draw_refline( unsigned char zoom); 
extern void initmode();
extern void scopy( char *s, char *t, char no);
extern void show_status();  /* but.c */
extern void out_phos_name();
extern void out_ref_name();
extern void out_cfactor_name();
extern void bardown();
extern void barup();
extern void out_laptime( char line);
extern void shut_dwn();
extern char no_of_decimals( float value);
extern void setmenu( char no);