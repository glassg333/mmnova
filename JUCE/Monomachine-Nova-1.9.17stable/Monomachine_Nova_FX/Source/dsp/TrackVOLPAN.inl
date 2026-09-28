#pragma once
// 1.8.5: documented Monomachine track path keeps VOL/PAN independent from
// DELAY. Included only after TrackChain is complete and while namespace nova is
// open.
inline void TrackChain::stageVOLPAN(float* l,float* r,int n){
    // VOL/PAN follows the amplifier envelope and precedes SRR. Their targets
    // arrive on the control cadence, so keep the established audio-rate 3 ms
    // de-zipper rather than stepping the stereo gain.
    const float panTarget=(params[14]-64)/64;
    const float volumeTarget=std::clamp(params[13],0.0f,127.0f)/volumeReference;
    if(!panSmoothingReady){smoothedPan=panTarget;smoothedVolume=volumeTarget;panSmoothingReady=true;}
    const float panSlew=static_cast<float>(1.0-std::exp(-1.0/(sr*0.003)));
    for(int i=0;i<n;++i){
        smoothedPan+=panSlew*(panTarget-smoothedPan);
        smoothedVolume+=panSlew*(volumeTarget-smoothedVolume);
        const float pan=smoothedPan,volume=smoothedVolume;
        l[i]=l[i]*volume*(1-std::max(0.0f,pan));
        r[i]=r[i]*volume*(1+std::min(0.0f,pan));
    }
}
