////////////////////////////////////////////////////////////////////////////////
//   ____  ____   
//  /   /\/   /  
// /___/  \  /   
// \   \   \/  
//  \   \        Copyright (c) 2003-2004 Xilinx, Inc.
//  /   /        All Right Reserved. 
// /---/   /\     
// \   \  /  \  
//  \___\/\___\
////////////////////////////////////////////////////////////////////////////////

#ifndef H_Work_ltc_clockgen_behavioral_H
#define H_Work_ltc_clockgen_behavioral_H
#ifdef __MINGW32__
#include "xsimMinGW.h"
#else
#include "xsim.h"
#endif


class Work_ltc_clockgen_behavioral: public HSim__s6 {
public:

    HSim__s1 SE[7];

HSim__s4 Cd;
HSim__s4 Cg;
HSim__s4 Cj;
HSim__s4 Cn;
    HSim__s1 SA[2];
    Work_ltc_clockgen_behavioral(const char * name);
    ~Work_ltc_clockgen_behavioral();
    void constructObject();
    void constructPorts();
    void reset();
    void architectureInstantiate(HSimConfigDecl* cfg);
    virtual void vhdlArchImplement();
};



HSim__s6 *createWork_ltc_clockgen_behavioral(const char *name);

#endif
