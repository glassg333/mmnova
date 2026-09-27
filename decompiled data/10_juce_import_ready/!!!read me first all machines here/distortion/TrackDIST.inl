#pragma once
// MODE S owns the existing post-filter DIST block. The retained DSP registry
// still keeps mnm|old for state compatibility, but it is not a competing sound
// selector once a project is loaded through the MODE S schema.
inline void TrackChain::stageDIST(float* l,float* r,int n){
        // DIST is the one post-filter track block. params[12] is refreshed on
        // the control cadence; render through the existing audio-rate de-zipper.
        const float target=std::clamp(params[12],0.0f,127.0f);
        if(!distSmoothingReady){smoothedDist=target;distSmoothingReady=true;}
        for(int i=0;i<n;++i){
            smoothedDist+=distSlew*(target-smoothedDist);
            const float nativeL=monomachine::mnm::Saturator::process(l[i],smoothedDist);
            const float nativeR=monomachine::mnm::Saturator::process(r[i],smoothedDist);
            switch(hybridSaturationMode){
                case hybrid_private::hybridDistOld:
                    // OLD remains an isolated, untouched reference law.
                    l[i]=bipolarDist(l[i],smoothedDist);
                    r[i]=bipolarDist(r[i],smoothedDist);
                    break;
                case hybrid_private::hybridDistMnmFix:
                    // Experimental A/B correction for the reported 3.5 kHz
                    // MNM-DIST level notch; native MNM remains untouched.
                    l[i]=mnmFixNotch.process(0,nativeL,smoothedDist);
                    r[i]=mnmFixNotch.process(1,nativeR,smoothedDist);
                    break;
                case hybrid_private::hybridDistFold:
                case hybrid_private::hybridDistZero:
                case hybrid_private::hybridDistClamp:
                    // Imported characters inherit the retained MNM control
                    // progression: native/attenuated through 64, then matched
                    // pre-drive, output control, and a continuous colour blend.
                    l[i]=hybridSaturation.processMnmControlled(0,l[i],smoothedDist,nativeL);
                    r[i]=hybridSaturation.processMnmControlled(1,r[i],smoothedDist,nativeR);
                    break;
                case hybrid_private::hybridDistMnm:
                default:
                    l[i]=nativeL;
                    r[i]=nativeR;
                    break;
            }
        }
    }
