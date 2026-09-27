#pragma once
// 1.8.4: TrackChain stage implementation extracted from NovaDSP.h.
// Included only after TrackChain is complete and while namespace nova is open.
// Keep this body bit-identical unless the stage itself is intentionally changed.
inline void TrackChain::stageENV(float* l,float* r,const float* amp,int n){
        // AMP ENV follows EQ/FILT and precedes VOL/PAN.
        for(int i=0;i<n;++i){
            l[i]*=amp[i];
            r[i]*=amp[i];
        }
    }
