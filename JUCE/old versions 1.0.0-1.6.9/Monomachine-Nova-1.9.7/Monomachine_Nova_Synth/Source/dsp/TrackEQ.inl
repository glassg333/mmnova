#pragma once
// 1.8.4: TrackChain stage implementation extracted from NovaDSP.h.
// Included only after TrackChain is complete and while namespace nova is open.
// Keep this body bit-identical unless the stage itself is intentionally changed.
inline void TrackChain::stageEQ(float* l,float* r,int n){
        // EQ precedes FILT in the documented track path.
        for(int i=0;i<n;++i){
            l[i]=eqL.tick(l[i]);
            r[i]=eqR.tick(r[i]);
        }
    }
