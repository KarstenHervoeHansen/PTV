////////////////////////////////////////////////////////////////////////////////
//   ____  ____  
//  /   /\/   /  
// /___/  \  /   
// \   \   \/    
//  \   \        Copyright (c) 2003-2004 Xilinx, Inc.
//  /   /        All Right Reserved. 
// /___/   /\   
// \   \  /  \  
//  \___\/\___\ 
////////////////////////////////////////////////////////////////////////////////

#ifndef H_workMtest__digital__loop_H
#define H_workMtest__digital__loop_H

#ifdef _MSC_VER
#pragma warning(disable: 4355)
#endif

#ifdef __MINGW32__
#include "xsimMinGW.h"
#else
#include "xsim.h"
#endif

class workMtest__digital__loop : public HSim__s5{
public: 
    workMtest__digital__loop(const char *instname);
    ~workMtest__digital__loop();
    void setDefparam();
    void constructObject();
    void moduleInstantiate(HSimConfigDecl *cfg);
    void connectSigs();
    void reset();
    virtual void archImplement();
    HSim::ValueS* up1Func(HSim::VlogVarType& outVarType, int& outNumScalars, int inNumScalars);
    HSim::ValueS* up4Func(HSim::VlogVarType& outVarType, int& outNumScalars, int inNumScalars);
    HSim__s1 us[1];
    HSim__s3 uv[3];
    HSimVlogParam up[6];
};

#endif
