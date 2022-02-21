/***************************************************************************/
/*	Copyright PHILIPS TV TEST EQUIPMENT A/S, BRONDBY 1997							*/
/*	Project:		PM5639 Display Unit														*/
/*	Module:		RUN.C																			*/
/*	Author:		Preben Christiansen, DEV												*/
/*	Org. date:	910212																		*/
/*	Rev. date:	970822, KEn	DEV															*/
/*	Status:		Version 2.3																	*/
/*																									*/
/*	This module contains the following functions:									*/
/*																									*/
/*				char *alloc( int n);															*/
/*				void memalloc( void);														*/
/*				void initCRT( void);															*/
/*				void initPRJ( void);															*/
/*				void initall( char Poweron_Button);										*/
/*				void common_calc( void);													*/
/*				float CalculateTempKor( void); 											*/
/*				void outxyy( void);															*/
/*				void outrgb( void);															*/
/*																									*/
/* Changes 																						*/
/*	970822 Released as version 2.3														*/
/*	951207 Released as version 2.2														*/
/* 951128 Added MasterResetLock in initiall(), intiCRT() and initPRJ()		*/
/*	951109 Added parameter in call of function: initall()							*/
/* 920520 Now supports projector head 													*/
/* 920520 For earlier versions of history see u'v'-version 						*/
/***************************************************************************/
#pragma large

#include <reg51.h>				/* Define 8051 registers */
#include <stdio.h>				/* Define I/O functions	*/
#include <stdlib.h>				/* Standard library	*/
#include <absacc.h>				/* Absolute addresses	*/
#include <math.h>					/* Math (abs,...)	*/
#include <sub.h>					/* Own definitions	*/
#include <defs.h>					/* Own definitions	*/
#include <tables.h>				/* Own definitions	*/


/***************************************************************************/
/*	alloc																							*/
/*																									*/
/* Written by:	Preben Christiansen, DEV     											*/
/*	Revised by:	Kim Engedahl, DEV															*/
/*	Date:			910212																		*/
/*	Revised:		951005																		*/
/*																									*/
/* Module:		RUN.C																			*/
/*	Function:	Reserve space for n bytes in RAM										*/
/*	Syntax:		char* alloc( int n);														*/
/*	Remarks:		ALLOCATION OF MEMORY 													*/
/*	Returns:		Pointer to allocated n bytes											*/
/*	Updates:		----																			*/
/***************************************************************************/
char *alloc( int n) {
	allocp += n;
	return(allocp - n);
}

/***************************************************************************/
/*	memalloc																						*/
/*																									*/
/* Written by:	Preben Christiansen, DEV     											*/
/*	Revised by:	Kim Engedahl, DEV															*/
/*	Date:			910212																		*/
/*	Revised:		951128																		*/
/*																									*/
/* Module:		RUN.C																			*/
/*	Function:	Reserve space for resident variables								*/
/*	Syntax:		void memalloc( void);													*/
/*	Remarks:		ALLOCATION OF MEMORY 													*/
/* 				CRT version:	Allocates 2500 (2265) bytes						*/
/*										Includes 10 white refs.								*/
/*										Includes 10 setup										*/
/* 				PRJ version:	Allocates 2000 (1467) bytes						*/
/*										Includes 10 white refs.								*/
/*										Includes 10 setups									*/
/*										Includes 10 conversion factors					*/
/*					allocbuf[0] reserved for head version								*/
/*					allocbuf[size_of_allocbuf - 1] is used for MasterResetLock	*/
/*	Returns:		Nothing																		*/
/*	Updates:		----																			*/
/***************************************************************************/
void memalloc( void) {
	int n;
	data char i;

	switch(head_version) {
		case CRThead:
			allocp = &allocbuf[1];
							
			n = sizeof( *phos[0]);
			for ( i=0; i<phosabsmax+2; i++)
				phos[i] = (struct phosp *) alloc(n);
			break;

		case PRJhead:
			allocp = &allocbuf[2500];

			n = sizeof( *cfactor[0]);
			for ( i=0; i<factormax+2; i++)
				cfactor[i] = (struct unit *) alloc(n);

			n = sizeof( *phos[0]);
			for ( i=0; i<2; i++)
				phos[i] = (struct phosp *) alloc(n);
			break;
	}

	n = sizeof( *whref[0]);
	for ( i=0; i<refmax+2; i++)
		whref[i] = (struct whr *) alloc(n);

	n = sizeof( *sup[0]);
	for ( i=0; i<supmax+3; i++)
		sup[i] = (struct status *) alloc(n);

	n = sizeof( *msm[0]);
	for ( i=0; i<measmax+1; i++)
		msm[i] = (struct meas *) alloc(n);

	temp = (struct tmp *) alloc( sizeof( *temp));

	sample = (struct avg *) alloc( sizeof( *sample));

	menuptr = (struct menus *) alloc( sizeof( *menuptr));
	testmenuptr = (struct menus *) alloc( sizeof( *testmenuptr));

	for ( i=0; i<42; i++)
		barptr[i] = alloc(1);				/* Reserve space for 42 menu-pointers */

	checkptr = alloc(1);
	contrast = alloc(1);
	freeze = alloc(1);
	menuno = alloc(1);
	menuon = alloc(1);
	mother = alloc(1);
	sampleno = alloc(1);
	samples = alloc(1);
	number = alloc(6);
	kuno = alloc(7);
	new_number = alloc(6);
	new_kuno = alloc(7);
	laphr = alloc(1);
	lapmin = alloc(1);
	lapsec = alloc(1);
}

/***************************************************************************/
/*	initCRT																						*/
/*																									*/
/* Written by:	Preben Christiansen, DEV     											*/
/*	Revised by:	Kim Engedahl, DEV															*/
/*	Date:			910212																		*/
/*	Revised:		951128																		*/
/*																									*/
/* Module:		RUN.C																			*/
/*	Function:	Initialize RAM for the CRT-sensor									*/
/*	Syntax:		void initCRT( void);														*/
/*	Remarks:		ALLOCATION OF MEMORY 													*/
/*	Returns:		Nothing																		*/
/*	Updates:		----																			*/
/***************************************************************************/
void initCRT( void) {
	data char i;

	if (*checkptr != 0x55) {		/* if RAM not OK then initiate user memory */

		MasterResetLock = LockOFF;					/* Master Reset Lock OFF */
															/* Used for virgin apparatus */

		for ( i=3; i<phosmax+1; i++) {
			phos[i]->name[0] = '\0';				/* Erase stored phosphor name */
			phos[i]->key = 0;							/* Lock phosphor selections */
		}
		phos[1]->key = 0x55;							/* Unlock phosphor 1 and 2 */
		phos[2]->key = 0x55;

		for ( i=4; i<refmax+1; i++) {
			whref[i]->name[0] = '\0';				/* Erase stored wh.ref names */
			whref[i]->key = 0;						/* Lock wh.ref selections */
		}
		whref[1]->x = whx6500;						/* set default wh.ref D6500 */
		whref[1]->y = why6500;						/* xy, uv & u'v' */
		whref[1]->u = whu6500;
		whref[1]->v = whv6500;
		whref[1]->v_ = whv_6500;
		strput( "D6500", whref[1]->name);

		whref[2]->x = whx3200;						/* set default wh.ref 3200K Cor */
		whref[2]->y = why3200;						/* xy, uv & u'v' */
		whref[2]->u = whu3200;
		whref[2]->v = whv3200;
		whref[2]->v_ = whv_3200;
		strput( "3200K cor.", whref[2]->name);

		whref[3]->x = whx9300;						/* set default wh.ref 9300K Cor */
		whref[3]->y = why9300;						/* xy, uv & u'v' */
		whref[3]->u = whu9300;
		whref[3]->v = whv9300;
		whref[3]->v_ = whv_9300;
		strput( "9300K cor.", whref[3]->name);

		for ( i=1; i<4; i++)					/* Unlock factory programmed wh.refs */
			whref[i]->key = 0x55;

		for ( i=0; i<42; i++)				/* Initiate menu select bars */
			*barptr[i] = menu[i].maxline;

		*barptr[2] = 1;

		sup[0]->phos_no = *barptr[3] = 1;		/* Initiate set-up */
		sup[0]->whref_no = *barptr[4] = 1;
		sup[0]->munit_no = *barptr[5] = 1;
		sup[0]->abs_rgb = 100;
		sup[0]->rgbmode_no = *barptr[1] = 1;
		sup[0]->mem_no = *barptr[9] = 1;
		sup[0]->zoom = 25;
		sup[0]->mode_no = xyY_mode;						/* Set standard mode: xyY */
		sup[0]->xymode_no = *barptr[29] = 1;
		sup[0]->toggle_setup = 0;				/* Toggle setup 1,2:start in 1 */
	}
													/* The next is done in every start-up */
	strput(phosname1,phos[1]->name);		/* default phosphor names */
	strput(phosname2,phos[2]->name);

	unitfact = measunit[sup[0]->munit_no].unitfact;		  /* Luminance factor */

	convfact = 1.0;				/* Conversion factor eq. 1. Only used in PRJ */
}

/***************************************************************************/
/*	initPRJ																						*/
/*																									*/
/* Written by:	Preben Christiansen, DEV     											*/
/*	Revised by:	Kim Engedahl, DEV															*/
/*	Date:			910212																		*/
/*	Revised:		951128																		*/
/*																									*/
/* Module:		RUN.C																			*/
/*	Function:	Initialize RAM for the projector-sensor							*/
/*	Syntax:		void initPRJ( void);														*/
/*	Remarks:		ALLOCATION OF MEMORY 													*/
/*	Returns:		Nothing																		*/
/*	Updates:		----																			*/
/***************************************************************************/
void initPRJ( void) {
	data char i;

	if (*checkptr != 0x55) {		/* if RAM not OK then initiate user memory */

		MasterResetLock = LockOFF;					/* Master Reset Lock OFF */
															/* Used for virgin apparatus */

		phos[1]->key = 0x55;							/* Unlock phosphor 1 */

		for ( i=2; i<refmax+1; i++) {
			whref[i]->name[0] = '\0';				  /* erase stored wh.ref names */
			whref[i]->key = 0;							  /* lock wh.ref selections */
		}

		whref[1]->x = whx6500;								  /* set default wh.ref */
		whref[1]->y = why6500;
		strput( "fty.ref", whref[1]->name);	 /* ONLY x,y is used in PROJECTOR */

		whref[1]->key = 0x55;				 /* unlock factory programmed wh.ref */

		for ( i=1; i<factormax+1; i++) {
			cfactor[i]->name[0] = '\0';    /* erase stored con fac. names */
			cfactor[i]->key = 0;				 /* lock con fac. selections */
		}

		cfactor[1]->cfactor = default_cfactor;
		strput( "Unit", cfactor[1]->name);

		cfactor[1]->key = 0x55;	 		 	/* unlock factory programmed con. fac */
									
		for ( i=0; i<42; i++)			   /* initiate menu select bars */
			*barptr[i] = menu[i].maxline;

		*barptr[30] = 1;

		sup[0]->phos_no = *barptr[3] = 1; 		/* initiate set-up */
		sup[0]->whref_no = *barptr[4] = 1;
		sup[0]->cfactor_no = *barptr[32] = 1;
		sup[0]->munit_no = *barptr[5] = 1;
		sup[0]->abs_rgb = 7;
		sup[0]->rgbmode_no = *barptr[30] = 1;
		sup[0]->mem_no = *barptr[9] = 1;
		sup[0]->zoom = 25;
		sup[0]->mode_no = RGB_mode;						/* set standard mode: RGB */
		sup[0]->xymode_no = *barptr[29] = 1;
		sup[0]->toggle_setup = 0;					/* Toggle setup 1,2:start in 1 */
	}
												  /* The next is done in every start-up */
	convfact = cfactor[sup[0]->cfactor_no]->cfactor;

	unitfact = measunit[1].unitfact;
}

/***************************************************************************/
/*	initall																						*/
/*																									*/
/* Written by:	Preben Christiansen, DEV     											*/
/*	Revised by:	Kim Engedahl, DEV															*/
/*	Date:			910212																		*/
/*	Revised:		951128																		*/
/*																									*/
/* Module:		RUN.C																			*/
/*	Function:	Initialize RAM-memory and variables									*/
/*	Syntax:		void initall( char Poweron_Button);									*/
/*	Remarks:		ALLOCATION OF MEMORY 													*/
/*	Returns:		Nothing																		*/
/*	Updates:		----																			*/
/***************************************************************************/
void initall( unsigned char Poweron_Button) {
	data char i;

	memalloc();						/* Allocate memory */

	MasterResetLock = allocbuf[size_of_allocbuf - 1];

	Poweron_Button &= 0x7f;		/* Check for special start-up commands */

	if ( !Poweron_Button) {		/* RAM reset: ENTER + xyY + FUNCT.+ ON/OFF buttons */
		if ( MasterResetLock != LockON)
			*checkptr = 0x00;
	}
	else
		if ( Poweron_Button == ON_OFF_FUNCT_Buttons)
			test_on = 1;

	switch ( head_version) {
		case CRThead:
			initCRT();
			break;

		case PRJhead:
			initPRJ();
			break;
	}

	if (*checkptr != 0x55) {		/* if RAM not OK then initiate user memory */

		*checkptr = 0x55;							/* set RAM OK flag */

		recall_whiteref(1);						/* select wh.ref no. 1 */

		for ( i=1; i<supmax+2; i++)			/* set default recalls */
			store_setup(i);

		for ( i=1; i<measmax+1; i++)			/* erase stored measurements */
			msm[i]->key = 0;

		*menuon = *freeze = 0;
		*menuno = 2;
		*contrast = 50;							/* standard contrast */
		temp->charno = 0;							/* char no. pointer for spelling */

		*laphr = *lapmin = *lapsec = 0;  /* reset non shut-down counter (test) */

		if ( head_version == PRJhead)
			recall_cfactor(1);					/* select con factor no. 1 */
	}

	butenable = 0;									/* inhibit ON button */
	idle_cnt = idle_delay;						/* arm auto shut-off */

	if (*menuon) 
		menu_cnt = menu_delay;					/* armmenu fall-out */

	rep_cnt = rep_delay = non_shdwn = 0;	/* initiate button repetition */
	sample->x = sample->y = sample->z = 0.0;
	contrast_port = *contrast | 0x80;		/* contrast and power-on */
}


char decimals;
float lum, lumlow, yy, xm, ym, zm, xval, yval, uval, vval, v_val;


/***************************************************************************/
/*	common_calc																					*/
/*																									*/
/* Written by:	Preben Christiansen, DEV     											*/
/*	Revised by:	Kim Engedahl, DEV															*/
/*	Date:			910212																		*/
/*	Revised:		951005																		*/
/*																									*/
/* Module:		RUN.C																			*/
/*	Function:	Calculations from  sensor-output which is common to both		*/
/*					xyY- and RGB-mode															*/
/*	Syntax:		void common_calc( void);												*/
/*	Remarks:		----						 													*/
/*	Returns:		Nothing																		*/
/*	Updates:		----																			*/
/***************************************************************************/
void common_calc( void) {
	float x, y, z, sum, xx, zz, divisor, utmp, vtmp, v_tmp;

	x = sample->x;										  /* save Sensor values */
	y = sample->y;
	z = sample->z;

	if (x < -0.5) overload = 1;

	/* NOTE: Correction matrix for PROJECTOR should only contain the values: */
	/*		 k[0] = 0.71*L  k[3] = 0       k[6] = 0 */
	/*		 k[1] = 0.22*L  k[4] = 0.71*L  k[7] = 0.07*L */
	/*		 k[2] = 0       k[5] = 0       k[8] = 0.71*L */
	/* AND phosphor matrix EBU should contain ALL zero's */

		  /* multiply with correction matrix to make standard observer values */
	xx = x * k[0] + y * k[3] + z * k[6];							/* = true X */
	zz = x * k[2] + y * k[5] + z * k[8];							/* = true Z */

	msm[0]->yy = yy = x * k[1] + y * k[4] + z * k[7];			/* = true Y */

	if (xx < 0.) 
		xx = 0.;												/* correct negative values */

	sum = xx + yy + zz;									/* calculate x,y */
	msm[0]->xval = xval = xx/sum;
	msm[0]->yval = yval = yy/sum;

							/* Calculate pre-gain corrected values for RGB display */
							/* and store values of current measurement */
	msm[0]->xm = xm = x * xfact;
	msm[0]->ym = ym = y;  										/* yfact is always 1.0 */
	msm[0]->zm = zm = z * zfact; 	

	lumlow = 0;

	switch ( head_version) {
		case CRThead:
			if ( yy < ( 0.1 / measunit[1].unitfact) ) 
				lumlow++;
			break;
		case PRJhead:
			if ( yy < ( 0.01 / (measunit[1].unitfact*default_cfactor)))
				lumlow++;
			break;
	}

	lum = (lumlow) ? 0.0 : yy * unitfact * convfact;		/* calc. luminance */

	switch ( head_version) {
		case PRJhead:									/* Examine display out of range */
			errorno = 0;
			if ((( lum < 0.01) || ( lum > 99999)) && !lumlow)
				errorno = 4;
			break;
	} 
										/* determine number format for CRT luminance */
	decimals = 1;
	if ( sup[0]->munit_no == 3) 
		decimals++; 

	divisor = xx + 15*yy + 3*zz;					/* calc. deviation in CIELUV */

	msm[0]->uval = uval = 4 * xx / divisor;
	msm[0]->vval = vval = 6 * yy / divisor;
	msm[0]->v_val = v_val = 9 * yy / divisor;

	utmp = uval - whref[0]->u;
	vtmp = vval - whref[0]->v;
	v_tmp = v_val - whref[0]->v_;

	msm[0]->dev = 1300. * sqrt( utmp*utmp + v_tmp*v_tmp);

					/* x,y,z & sum used as temporary store, ( not used anymore) */
	switch ( sup[0]->xymode_no) {
		case 1:
		case 2:
			x = xval;
			y = whref[0]->x;
			z = yval;
			sum = whref[0]->y;
			break;
		case 3:
		case 4:
			x = uval;
			y = whref[0]->u;
			z = v_val;
			sum = whref[0]->v_;
			break;
		case 5:
		case 6:
			x = uval;
			y = whref[0]->u;
			z = vval;
			sum = whref[0]->v;
			break;
	}
	xerr = (int)( ( x + 0.0005)*1000) - (int)( ( y + 0.0005)*1000);
	yerr = (int)( ( z + 0.0005)*1000) - (int)( ( sum + 0.0005)*1000);
}

/***************************************************************************/
/*	CalculateTempKor																			*/
/*																									*/
/* Written by:	Kim Engedahl, DEV			     											*/
/*	Revised by:	Kim Engedahl, DEV															*/
/*	Date:			921001																		*/
/*	Revised:		951005																		*/
/*																									*/
/* Module:		RUN.C																			*/
/*	Function:	Calculate the correlated color temperature						*/
/*	Syntax:		float CalculateTempKor( void);										*/
/*	Remarks:		----						 													*/
/*	Returns:		A float containing the correlated color temperature			*/
/*	Updates:		----																			*/
/***************************************************************************/
float CalculateTempKor( void) {
	float d1, d2, tmp1, tmp2;
	bit TempFound = 0;
	data char KLine = 12;

	do {
		d1 = (( vval-TT[KLine+1][2])-TT[KLine+1][3]*(uval-TT[KLine+1][1]))/
					TT[KLine+1][4];

		tmp1 = vval-TT[KLine][2];
		tmp2 = uval-TT[KLine][1];

		d2 = ( tmp1-TT[KLine][3]*tmp2)/TT[KLine][4];

		if ((d1 > 0) && (d2 > 0)) 
			KLine += 1;
		else 
			if ((d1 < 0) && (d2 < 0)) 
				KLine -= 1;
		else												/* if ((d1 < 0) || (d2 < 0)) */
			TempFound = 1;

	} while ((KLine >= 0) && (KLine <= (MaxTable -1)) && !TempFound);

	if ( TempFound) {
		d1 = fabs( d1);
		d2 = fabs( d2);

		if ( sqrt( tmp1*tmp1 + tmp2*tmp2) > 0.0385)
			return ( 0.);

		return( 1/(TT[KLine+1][0] + 
						( d1* (TT[KLine][0] - TT[KLine+1][0])/(d1+d2))));
	}
	else 
		return (0.);
}

/***************************************************************************/
/*	outxyy																						*/
/*																									*/
/* Written by:	Preben Christiansen, DEV												*/
/*	Revised by:	Kim Engedahl, DEV															*/
/*	Date:			921001																		*/
/*	Revised:		951005																		*/
/*																									*/
/* Module:		RUN.C																			*/
/*	Function:	Output the sensor-measurement in xyY format.						*/
/*	Syntax:		void outxyy( void);														*/
/*	Remarks:		----						 													*/
/*	Returns:		Nothing																		*/
/*	Updates:		----																			*/
/***************************************************************************/
void outxyy( void) {
	char res;
	float out1, out2;
	char begin_line;

	common_calc();						/* Calculations common to xyY- and RGB-mode */
	clrcross();							/* Erase old measure dot */

	msm[0]->dx = xerr/(sup[0]->zoom);		/* Calc. deviations from wh.ref */
	msm[0]->dy = yerr/(sup[0]->zoom);

	if (!lumlow && !overload) {									/* write to screen */
		putcross( msm[0]->dx, msm[0]->dy);
										 					/* Correlated colourtemperatur */
		if ( (msm[0]->colortemp = CalculateTempKor()) == 0) {
			outtextxy( 11, 14, " ");
			outtext( "----");
		}
		else
			outtextxy( 11, 14, ftoa( msm[0]->colortemp , 0));
		outtext("  ");

		switch ( sup[0]->xymode_no) { 
			case 1:
				begin_line = 12;
				out1 = xval;
				out2 = yval;
				break;
			case 3:
				begin_line = 15;
				out1 = uval;
				out2 = v_val;
				break;
			case 5:
				begin_line = 12;
				out1 = uval;
				out2 = vval;
				break;
			case 2:
			case 6:
				begin_line = 15;
				break;
			case 4:
				begin_line = 18;
				break;
		}

		switch ( sup[0]->xymode_no) { 
			case 1:
			case 3:
			case 5:
				outtextxy( begin_line, 10, ftoa( out1, 3));
				outtext("  ");
				outtextxy( begin_line, 11, ftoa( out2, 3));
				outtext("  ");
				break;
			case 2:
			case 4:
			case 6:
				if ( xerr > 0 )
					outtextxy( begin_line, 10, "+");
				else
					outtextxy( begin_line, 10, "-");

				if ( yerr > 0 )
					outtextxy( begin_line, 11, "+");
				else
					outtextxy( begin_line, 11, "-");

				outtextxy( begin_line+5, 10, ftoa( fabs( 
												msm[0]->deltax = (float) xerr/1000.), 3));
				outtext("  ");
				outtextxy( begin_line+5, 11, ftoa( fabs( 
												msm[0]->deltay = (float) yerr/1000.), 3));
				outtext("  ");
				break;
		}

		if (msm[0]->dev < 999.) {
			outtextxy( 14, 13, ftoa( msm[0]->dev, 0));
	 		outtext(" ");											/* erase old figures */
		}
	}
	else {
		outtextxy( 18, 13, "         ");
		outtextxy( 11, 14, "         ");

		switch ( sup[0]->xymode_no) { 
			case 1:
			case 5:
				outtextxy( 12, 10, "           ");
				outtextxy( 12, 11, "           ");
				break;
			case 2:
			case 3:
			case 6:
				outtextxy( 15, 10, "            ");
				outtextxy( 15, 11, "            ");
				break;
			case 4:
				outtextxy( 18, 10, "            ");
				outtextxy( 18, 11, "            ");
				break;
		}
		if ( overload)
			outtextxy( 14, 12, "          ");
	}

	if ( !overload) {
		outtextxy( 14, 12, ftoa( lum, decimals));
		outtext("    ");											/* erase old figures */
	}
	outtextxy( 36, 12, measunit[sup[0]->munit_no].name);
}

/***************************************************************************/
/*	outrgb																						*/
/*																									*/
/* Written by:	Preben Christiansen, DEV												*/
/*	Revised by:	Kim Engedahl, DEV															*/
/*	Date:			921001																		*/
/*	Revised:		951005																		*/
/*																									*/
/* Module:		RUN.C																			*/
/*	Function:	Output the sensor-measurement in RGB format.						*/
/*	Syntax:		void outrgb( void);														*/
/*	Remarks:		----						 													*/
/*	Returns:		Nothing																		*/
/*	Updates:		----																			*/
/***************************************************************************/
void outrgb( void) {
	char *s, barptr_tmp;
	float basis, rbar, gbar, bbar, rval, gval, bval, barmax, barmin;

	common_calc();						/* Calculations common to xyY- and RGB-mode */

	switch ( head_version) {
		case CRThead:
			barptr_tmp = *barptr[1];

			if ( !overload) {
				outtextxy( 16, 15, ftoa( lum, decimals));
				outtext("    ");
			}
			else 
				outtextxy( 16, 15, "            " );
				
			outtextxy( 40, 15, measunit[sup[0]->munit_no].name);
			break;

		case PRJhead:
			barptr_tmp = *barptr[30];

			if ( overload) {
				outtextxy( 13, 15, "          ");
			}
			else {
				if ( errorno != 4) {
					outtextxy( 13, 15, ftoa( lum, no_of_decimals( lum)));
					outtext("     ");
				}
				else {
					if ( lum < 0.01) 
						s = "< 0.01 ";
					else
						s =  "> 99999 ";
					outtextxy( 13, 15, s);
				}
			}
			outtextxy( 40, 15, cfactor[sup[0]->cfactor_no]->name);
			break;
	}
																	/* and wh.ref name */
	if (( barptr_tmp < 5) && ( errorno != 4) && !overload) {
		outtextxy( 4, 16, "Ref: ");						/* if not "Disp out of.." */
		outtext( whref[sup[0]->whref_no]->name);
	}

		 /* multiply with phosphor matrix to make r, g, b sensors independant */
	rval = int_scale * xm + nn_gr * ym + nn_br * zm;
	gval = nn_rg * xm + int_scale * ym + nn_bg * zm;
	bval = nn_rb * xm + nn_gb * ym + int_scale * zm;

	if ( lumlow || overload) 
		rbar = gbar = bbar = 0;
	else {
		switch ( barptr_tmp) {	 /* calculate bar size relative to references */
			case 1:
				basis = gref/gval; 
				gbar = 1.0;
				rbar = rval/rref * basis;
				bbar = bval/bref * basis;
				break;
			case 2:
				basis = rref/rval;
				rbar = 1.0;
				gbar = gval/gref * basis;
				bbar = bval/bref * basis;
				break;
			case 3:
				basis = bref/bval;
				bbar = 1.0;
				rbar = rval/rref * basis;
				gbar = gval/gref * basis;
				break;
			default:
				switch ( barptr_tmp) {		/* basis in these modes means barfact */
					case 4:
						basis = 90. / (unitfact*convfact) / yy;
						break;
					case 5:
						basis = (90./ sup[0]->abs_rgb)*
									(measunit[1].unitfact/(unitfact*convfact));
						break;
					case 6:
					case 7:
					case 8:
					case 9:
					case 10:	
						basis = msm[barptr_tmp-5]->yy / yy;
						break;
				}

				rbar = rval/rref * basis;
				gbar = gval/gref * basis;
				bbar = bval/bref * basis;
				break;
		}
	}

	msm[0]->rbar = rbar;												/* save bar values */
	msm[0]->gbar = gbar;
	msm[0]->bbar = bbar;

	drawbar( 8, sup[0]->zoom, rbar);						 /* display RGB bars */
	drawbar( 18, sup[0]->zoom, gbar);
	drawbar( 28, sup[0]->zoom, bbar);
}
