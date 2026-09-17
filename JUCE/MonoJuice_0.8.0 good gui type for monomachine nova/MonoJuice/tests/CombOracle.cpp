// Research/test bridge only; not linked into the plugin.
// Upstream DSPCore compiled unchanged with its own TEST_DSP parameter facade.
#include "../Reference/Uhhyou/ParallelComb/source/dsp/dspcore.hpp"
extern "C" void* combOracleCreate(double sr){auto* p=new DSPCore;p->setup(sr);return p;}
extern "C" void combOracleDestroy(void* p){delete static_cast<DSPCore*>(p);}
extern "C" void combOracleConfigure(void* object,const double* values,bool reset){auto& p=*static_cast<DSPCore*>(object);for(size_t i=0;i<p.param.value.size();i++)p.param.value[i]->setFromFloat(values[i]);if(reset)p.reset();p.setParameters();}
extern "C" void combOracleProcess(void* object,int n,const float* l,const float* r,float* a,float* b){static_cast<DSPCore*>(object)->process(size_t(n),l,r,a,b);}
