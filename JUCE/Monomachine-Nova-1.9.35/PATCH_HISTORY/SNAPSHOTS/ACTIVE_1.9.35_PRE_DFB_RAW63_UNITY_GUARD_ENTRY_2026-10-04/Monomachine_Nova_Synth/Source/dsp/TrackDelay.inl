#pragma once
// DELAY is the final per-track effect stage.  The hardware track-delay is not
// a separate FX machine: DSND/DFB are host-side mixer controls around the DSP
// core.  The Pack-8 voice-frame audit keeps native DSP-tail stage-2 controls
// separate from this host bridge.
inline void TrackChain::stageDELAY(float* l,float* r,int n){
    // DELAY follows SRR. l/r already contain the VOL/PAN-scaled, rate-reduced
    // dry signal; the delay core returns wet only and the dry path is added
    // exactly once here.
    // DSND is centred bipolar: positive side is one mono positive comb;
    // negative side splits the pre-delay L/R mono paths and inverts feedback.
    // Centre zero writes no new input but leaves the currently running tail intact.
    const float signedSend=track_delay_routing::signedSend(params[track_delay_routing::kDelaySend]);
    const float send=track_delay_routing::bipolarSendGain(signedSend);
    if(signedSend!=0.0f)negativeDelayComb=signedSend<0.0f;

    // MnmVoiceFrame's P:$0A22/$0AB7 reads the independent native stage-2
    // words Y:$414..$417. P1 FILT ATK/DEC are Y:$40C/$40D and control the
    // cascade-1 filter envelope; the host has no verified mirror for $414..17.
    // Therefore this compatibility delay must not reinterpret FILT ATK/DEC as
    // a return-bank selector or a wet-depth control. The observed DSND-sign
    // comb route below is a Nova host behaviour, not an alias of those stage-2
    // words and not a claim of original-firmware exactness.
    // DFB DYNAMICS is shared by OLD/MNM/NEW. When it is ON, BASE CURVE keeps
    // RAW=63 as reference, lifts RAW 0..63 toward it, lightly trims RAW=64,
    // and returns to RAW/64 at 65+. With FB CLIP enabled, RAW 0..63 bypasses
    // GUARD, RAW 63..64 arms it, and RAW 64..127 selects its user-adjustable
    // four-anchor level window. It remains a safety governor, not a firmware claim.
    const float feedbackRaw=std::clamp(params[track_delay_routing::kDelayFeedback],0.0f,127.0f);
    // The RMB graph reads this post-modulation/raw value through the processor
    // UI echo. It is observation only and never routes DFB into custom slots.
    lastDelayFeedbackRaw=feedbackRaw;
    const float returnGain=delayReturnGainForFeedback(feedbackRaw);
    const bool invertFeedback=negativeDelayComb?delayFeedbackInvertNegative:delayFeedbackInvertPositive;
    const float delaySeconds=delaySecondsForRaw(params[track_delay_routing::kDelayTime]);
    for(int i=0;i<n;++i){
        const float dryL=l[i],dryR=r[i];
        const float feedback=delayFeedbackDynamics.process(feedbackRaw);
        if(delayMode==monomachine::dspModeMnm){
            float wetL=0.0f,wetR=0.0f;
            mnmDelay.processSeconds(delaySeconds,feedback,send,negativeDelayComb,invertFeedback,
                                    std::max(repitchSlew,declickSlew),dryL,dryR,wetL,wetR);
            delayFeedbackDynamics.observeLoop(wetL,wetR);
            l[i]=dryL+wetL*returnGain;r[i]=dryR+wetR*returnGain;continue;
        }
        if(delayMode==monomachine::dspModeNew){
            float wetL=0.0f,wetR=0.0f;
            newDelay.processSeconds(delaySeconds,feedback,send,negativeDelayComb,invertFeedback,
                                    std::max(repitchSlew,declickSlew),dryL,dryR,wetL,wetR);
            delayFeedbackDynamics.observeLoop(wetL,wetR);
            l[i]=dryL+wetL*returnGain;r[i]=dryR+wetR*returnGain;continue;
        }
        // DBAS/DWID are the feedback-loop bandpass here: DBAS is its HP cutoff
        // and DWID expands the LP cutoff above DBAS. Their filtered tap is
        // multiplied by the positive/negative DSND comb polarity on write.
        time+=static_cast<float>(1-std::exp(-1/(sr*0.03)))*(targetDelay-time);
        float pos=static_cast<float>(write)-std::clamp(time,1.0f,static_cast<float>(delayL.size()-2));
        if(pos<0)pos+=static_cast<float>(delayL.size());
        size_t i0=static_cast<size_t>(pos)%delayL.size(),i1=(i0+1)%delayL.size();
        float frac=pos-std::floor(pos);
        float a=delayL[i0]+frac*(delayL[i1]-delayL[i0]),b=delayR[i0]+frac*(delayR[i1]-delayR[i0]);
        float filteredL=0.0f,filteredR=0.0f;
        delayFilter.processStereo(&a,&b,&filteredL,&filteredR,1);
        a=filteredL;b=filteredR;
        if(delayBaseQ>0.5f||delayWidthQ>0.5f){
            a=monomachine::mnm::DelayFeedbackFilter::stabilise(a);
            b=monomachine::mnm::DelayFeedbackFilter::stabilise(b);
        }
        delayFeedbackDynamics.observeLoop(a,b);
        float nextL=0.0f,nextR=0.0f;
        delayWrite(dryL,dryR,a,b,send,feedback,negativeDelayComb,invertFeedback,nextL,nextR);
        if(delayFeedbackLoopClip){
            nextL=monomachine::mnm::DelayFeedbackFilter::stabilise(nextL);
            nextR=monomachine::mnm::DelayFeedbackFilter::stabilise(nextR);
        }else{
            if(!std::isfinite(nextL))nextL=0.0f;
            if(!std::isfinite(nextR))nextR=0.0f;
        }
        delayL[write]=nextL;delayR[write]=nextR;
        l[i]=dryL+a*returnGain;r[i]=dryR+b*returnGain;write=(write+1)%delayL.size();
    }
}
