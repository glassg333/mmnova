#pragma once
// 1.8.5: DELAY is an independent final track-effect stage. Included only after
// TrackChain is complete and while namespace nova is open.
inline void TrackChain::stageDELAY(float* l,float* r,int n){
    // DELAY follows SRR.  l/r already contain the VOL/PAN-scaled, rate-reduced
    // dry signal; the delay core returns wet only and the dry path is added
    // exactly once here.
    const float dsend=std::clamp(params[28],0.0f,127.0f)-64.0f;
    const float send=std::abs(dsend)/(dsend<0?64.0f:63.0f);
    if(dsend!=0)pingPong=dsend<0;
    const float feedback=norm(params[29])*0.95f;
    for(int i=0;i<n;++i){
        const float dryL=l[i],dryR=r[i];
        if(delayMode==monomachine::dspModeMnm){
            float wetL=0.0f,wetR=0.0f;
            mnmDelay.process(params[27],feedback,send,pingPong,ppMode,
                             std::max(repitchSlew,declickSlew),dryL,dryR,wetL,wetR);
            l[i]=dryL+wetL;r[i]=dryR+wetR;continue;
        }
        if(delayMode==monomachine::dspModeNew){
            float wetL=0.0f,wetR=0.0f;
            newDelay.process(params[27],feedback,send,pingPong,ppMode,
                             std::max(repitchSlew,declickSlew),dryL,dryR,wetL,wetR);
            l[i]=dryL+wetL;r[i]=dryR+wetR;continue;
        }
        // OLD retains its established line/time law.  DBAS/DWID have always
        // belonged inside this recurrence; make that routing explicit and feed
        // the optional Q values instead of leaving its two resonators fixed.
        time+=static_cast<float>(1-std::exp(-1/(sr*0.03)))*(targetDelay-time);
        float pos=static_cast<float>(write)-std::clamp(time,1.0f,static_cast<float>(delayL.size()-2));
        if(pos<0)pos+=static_cast<float>(delayL.size());
        size_t i0=static_cast<size_t>(pos)%delayL.size(),i1=(i0+1)%delayL.size();
        float frac=pos-std::floor(pos);
        float a=delayL[i0]+frac*(delayL[i1]-delayL[i0]),b=delayR[i0]+frac*(delayR[i1]-delayR[i0]);
        float filteredL=0.0f,filteredR=0.0f;
        delayFilter.processStereo(&a,&b,&filteredL,&filteredR,1);
        a=filteredL;b=filteredR;
        // OLD is byte-compatible at Q=0; only optional resonance requests use
        // a final finite-feedback safeguard.
        if(delayBaseQ>0.5f||delayWidthQ>0.5f){
            a=monomachine::mnm::DelayFeedbackFilter::stabilise(a);
            b=monomachine::mnm::DelayFeedbackFilter::stabilise(b);
        }
        delayWrite(dryL,dryR,a,b,send,feedback,pingPong,delayL[write],delayR[write]);
        l[i]=dryL+a;r[i]=dryR+b;write=(write+1)%delayL.size();
    }
}
