/****************************************************************************/
/* MODULE:                                                                  */
/*   rtcl_str.h - Controller & device independent types & structure         */
/*                declarations                                              */
/****************************************************************************/
/*
 *   RTXCio    Version 1.4
 *   Copyright (c) 1993-1997.
 *   Embedded System Products, Inc.
 *   ALL RIGHTS RESERVED
*/
/****************************************************************************/
#ifndef _RTCL_STR_H_  /* { _RTCL_STR_H_ */
#define  _RTCL_STR_H_

#ifdef __cplusplus /* { */
extern "C" {
#endif /* } __cplusplus */

#define NFUNCS 5      /* constant number of RTXCio functions per controller */

typedef IORC (*DEV_FUNC)(IOPACK *);     /* pointer to a controller function */

typedef struct cntlr_unit
{
	 CNTLRID   cntlr_number;                     /* Controller's number */
	 INT   cntlr_address;                        /* Controllers address */
	 INT   number_of_units;          /* number of physical device units */
	 size_t  size_of_units;        /* size of physical device unit(PDU) */
  struct dev_unit_header * ptr_to_pdus;         /* pointer to the first PDU */
	 DEV_FUNC  io_funcs[NFUNCS];       /* table of Controller functions */
}CNTLR_UNIT;

/* =============================== */
/* FIXED!! device attribute status */
/* =============================== */
typedef enum
{
	ATTB_READ = 8,
	ATTB_WRITE,
	ATTB_RDWR
}ATTB_TYPE;

typedef struct dev_unit_header
{
	RESOURCE   dev_res;                     /* physical device resource */
	    SEMA   dev_sema;                   /* physical device semaphore */
	   DEVID   dev_number;                    /* physical device number */
       ATTB_TYPE   dev_attribute;              /* physical device attribute */
	 IOFNCID   last_io_oper;         /* device's last req i/o operation */
	    CHAR   lost_data;             /* flag to mark lost data occured */
	     INT   dev_unitno;           /* device's controller unit number */
      CNTLR_UNIT * dev_cntlr;          /* pointer to the Controller's block */
	 BYTECNT   bytes_transfered;    /* total number of bytes transfered */
}PDU_HEADER;

typedef struct  dev_init_pack
{
	 CNTLRID   dev_cntlrno;                       /* Controllers number */
	   DEVID   dev_number;                    /* physical device number */
	     INT   dev_unitno;                  /* Controller's unit number */
						    /* for physical device. */
 void (*devinit_func)(struct dev_init_pack *); /* Controller initialization function */
	RESOURCE   dev_res;                     /* physical device resource */
	    SEMA   dev_sema;                   /* physical device semaphore */
       ATTB_TYPE   dev_attribute;              /* physical device attribute */
}DEV_INIT_PACK;

typedef void (*DEVINIT_FUNC)(struct dev_init_pack *);      /* pointer to an */
					  /* device initialization function */

#ifdef __cplusplus /* { */
}
#endif /* } __cplusplus */

#endif    /* } _RTCL_STR_H_ */

/* End of the RTCL_STR.H module */
