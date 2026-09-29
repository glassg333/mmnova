#pragma once
// Native/reconstructed and retained legacy FILT implementations. Explicit
// VEL/KT, FIL ENV and SAT controls are opt-in additions; their default values
// leave the former MNM and OLD paths unchanged.
inline void TrackChain::stageFILT(float* l,float* r,int n){
        // FILT follows EQ and precedes the amplifier envelope.
        const float lowerSemitones=lowerFilterSemitones();
        const float upperSemitones=upperFilterSemitones();
        const bool filEnvAudible=filterEnvMix>0.5f
            &&(std::abs(filterEnvBase)>0.5f||std::abs(filterEnvWidth)>0.5f);
        const float filEnvScale=filterEnvMix/127.0f;
        const auto nextFilEnvOffsets=[this,filEnvAudible,filEnvScale](){
            // Tick even when MIX is zero once FILT is actively running, so
            // enabling ENV FIL during a held note observes the real envelope
            // phase rather than restarting it.
            const float level=filterModEnv.tick();
            if(!filEnvAudible)return std::pair<float,float>{0.0f,0.0f};
            return std::pair<float,float>{level*filEnvScale*filterEnvBase,
                                          level*filEnvScale*filterEnvWidth};
        };

        // The native MNM core and the independent physical-family core own
        // separate state.  A K35/Moog/ladder/R selection therefore never
        // enters mnmFilter just because FILT DSP happens to say MNM, and it
        // never enters the OLD renderer when FILT DSP says OLD.
        const auto renderPhysicalCore=[this,l,r,n,lowerSemitones,upperSemitones,
                                       &nextFilEnvOffsets](auto& physicalCore){
            // P2's documented neutral filter values are an insertion THRU.
            // This can only apply when both physical choices are NATIVE. A
            // selected K35/ladder/R family always remains audible.
            if(neutralFilterThru&&hybridLowerMode==hybrid_private::hybridNative
               &&hybridUpperMode==hybrid_private::hybridNative&&hasNeutralMnmFilter()){
                if(!neutralFilterThruActive){physicalCore.reset();neutralFilterThruActive=true;}
                return;
            }
            neutralFilterThruActive=false;
            const float fltBase=params[16],fltWidth=params[17],fltHpq=params[18],fltLpq=params[19];
            const float fltAtk=params[20],fltDec=params[21];
            const float fltBofs=params[22]-64.0f,fltWofs=params[23]-64.0f;
            physicalCore.setParameters(fltBase,fltWidth,fltHpq,fltLpq,fltAtk,fltDec,fltBofs,fltWofs);
            for(int i=0;i<n;++i){
                const auto offsets=nextFilEnvOffsets();
                physicalCore.setExternalFilterModifiers(lowerSemitones,upperSemitones,offsets.first,offsets.second);
                l[i]=physicalCore.process(0,l[i]);r[i]=physicalCore.process(1,r[i]);
            }
        };

        hybrid_private::dispatchFilterRenderRoute(
            filterMode==monomachine::dspModeMnm,hybridLowerMode,hybridUpperMode,
            // Both MODE selectors are NATIVE: retain the legacy OLD renderer.
            [this,l,r,n,&nextFilEnvOffsets,lowerSemitones,upperSemitones]{
                neutralFilterThruActive=false;
                for(int i=0;i<n;++i){
                    const auto offsets=nextFilEnvOffsets();
                    filter.setExternalFilterModifiers(lowerSemitones,upperSemitones,offsets.first,offsets.second);
                    filter.processStereo(l+i,r+i,l+i,r+i,1);
                }
            },
            // Both MODE selectors are NATIVE and FILT DSP explicitly says MNM.
            [&]{renderPhysicalCore(mnmFilter);},
            // Any non-NATIVE MODE L/H selection: dedicated independent route.
            // Its own state is not the MNM native renderer's state.
            [&]{renderPhysicalCore(independentPhysicalFilter);});

        if(filterSaturation.active()){
            for(int i=0;i<n;++i){
                l[i]=filterSaturation.process(0,l[i]);
                r[i]=filterSaturation.process(1,r[i]);
            }
        }
    }
