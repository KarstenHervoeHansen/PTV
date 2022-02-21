static const char * HSimCopyRightNotice = "Copyright 2004-2005, Xilinx Inc. All rights reserved.";
#ifdef __MINGW32__
#include "xsimMinGW.h"
#else
#include "xsim.h"
#endif
#include "C:/Xilinx91i/vhdl/hdp/nt/ieee/std_logic_1164/std_logic_1164.h"
#include "C:/Xilinx91i/vhdl/hdp/nt/ieee/std_logic_arith/std_logic_arith.h"
#include "C:/Xilinx91i/vhdl/hdp/nt/ieee/std_logic_signed/std_logic_signed.h"


static HSim__s6* IF0(HSim__s6 *Arch,const char* label,int nGenerics, 
va_list vap)
{
    extern HSim__s6 * createworkMtest__digital__loop(const char*);
    HSim__s6 *blk = createworkMtest__digital__loop(label); 
    return blk;
}


static HSim__s6* IF1(HSim__s6 *Arch,const char* label,int nGenerics, 
va_list vap)
{
    extern HSim__s6 * createworkMglbl(const char*);
    HSim__s6 *blk = createworkMglbl(label); 
    return blk;
}


static HSim__s6* IF2(HSim__s6 *Arch,const char* label,int nGenerics,va_list vap)
{
    extern HSim__s6* createWork_digital_phase_loop_behavioral(const char*);
    HSim__s6 *blk = createWork_digital_phase_loop_behavioral(label);
    return blk;
}


static HSim__s6* IF3(HSim__s6 *Arch,const char* label,int nGenerics,va_list vap)
{
    extern HSim__s6* createWork_digital_phase_loop_behavioral(const char*);
    HSim__s6 *blk = createWork_digital_phase_loop_behavioral(label);
    return blk;
}

class _top : public HSim__s6 {
public:
    _top() : HSim__s6(false, "_top", "_top", 0, 0, HSim::VerilogModule) {}
    HSimConfigDecl * topModuleInstantiate() {
        HSimConfigDecl * cfgvh = 0;
        cfgvh = new HSimConfigDecl("default");
        (*cfgvh).registerFuseLibList("unisims_ver;xilinxcorelib_ver");

        (*cfgvh).addVlogModule("work","test_digital_loop", (HSimInstFactoryPtr)IF0);
        (*cfgvh).addVlogModule("work","glbl", (HSimInstFactoryPtr)IF1);
        (*cfgvh).addVlogModule("digital_phase_loop/behavioral", (HSimInstFactoryPtr)IF2, true);
        (*cfgvh).addVlogModule("digital_phase_loop", (HSimInstFactoryPtr)IF3, true);
        HSim__s5 * topvl = 0;
        extern HSim__s6 * createworkMtest__digital__loop(const char*);
        topvl = (HSim__s5*)createworkMtest__digital__loop("test_digital_loop");
        topvl->moduleInstantiate(cfgvh);
        addChild(topvl);
        extern HSim__s6 * createworkMglbl(const char*);
        topvl = (HSim__s5*)createworkMglbl("glbl");
        topvl->moduleInstantiate(cfgvh);
        addChild(topvl);
        return cfgvh;
}
};

main(int argc, char **argv) {
  HSimDesign::initDesign();
  globalKernel->getOptions(argc,argv);
  HSim__s6 * _top_i = 0;
  try {
    IeeeStd_logic_1164=new Ieee_std_logic_1164("std_logic_1164");
    IeeeStd_logic_arith=new Ieee_std_logic_arith("std_logic_arith");
    IeeeStd_logic_signed=new Ieee_std_logic_signed("STD_LOGIC_SIGNED");
    HSimConfigDecl *cfg;
 _top_i = new _top();
  cfg =  _top_i->topModuleInstantiate();
    return globalKernel->runTcl(cfg, _top_i, "_top", argc, argv);
  }
  catch (HSimError& msg){
    try {
      globalKernel->error(msg.ErrMsg);
      return 1;
    }
    catch(...) {}
      return 1;
  }
  catch (...){
    globalKernel->fatalError();
    return 1;
  }
}
