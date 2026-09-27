#pragma once
// Native SRR stage.
inline void TrackChain::stageSRR(float* l,float* r,int n){
        // SRR follows AMP ENV and VOL/PAN, immediately before DELAY.
        const int hold=1+static_cast<int>(norm(params[26])*norm(params[26])*63*sr/44100);
        for(int i=0;i<n;++i){
            if(srrCounter==0){heldL=l[i];heldR=r[i];}
            srrCounter=(srrCounter+1)%hold;
            l[i]=heldL; r[i]=heldR;
        }
    }
