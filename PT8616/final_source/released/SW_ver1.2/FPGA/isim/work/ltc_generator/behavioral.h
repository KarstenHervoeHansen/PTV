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

#ifndef H_Work_ltc_generator_behavioral_H
#define H_Work_ltc_generator_behavioral_H
#ifdef __MINGW32__
#include "xsimMinGW.h"
#else
#include "xsim.h"
#endif


class Work_ltc_generator_behavioral: public HSim__s6 {
public:

    HSim__s1 SE[11];

HSim__s4 CR;
HSim__s4 CY;
    HSim__s1 SA[19];
HSimConstraints *c42;
  char *t43;
HSimConstraints *c44;
  char *t45;
HSimConstraints *c46;
HSimConstraints *c47;
  char *t48;
HSimConstraints *c49;
  char *t50;
HSimConstraints *c51;
  char *t52;
HSimConstraints *c53;
  char *t54;
HSimConstraints *c55;
  char *t56;
HSimConstraints *c57;
HSimConstraints *c58;
  char *t59;
HSimConstraints *c60;
  char *t61;
HSimConstraints *c62;
  char *t63;
HSimConstraints *c64;
  char *t65;
HSimConstraints *c66;
  char *t67;
HSimConstraints *c68;
HSimConstraints *c69;
  char *t70;
HSimConstraints *c71;
  char *t72;
HSimConstraints *c73;
  char *t74;
HSimConstraints *c75;
  char *t76;
HSimConstraints *c77;
  char *t78;
HSimConstraints *c79;
  char *t80;
HSimConstraints *c81;
  char *t82;
HSimConstraints *c83;
  char *t84;
HSimConstraints *c85;
  char *t86;
    Work_ltc_generator_behavioral(const char * name);
    ~Work_ltc_generator_behavioral();
    void constructObject();
    void constructPorts();
    void reset();
    void architectureInstantiate(HSimConfigDecl* cfg);
    virtual void vhdlArchImplement();
};



HSim__s6 *createWork_ltc_generator_behavioral(const char *name);

#endif
