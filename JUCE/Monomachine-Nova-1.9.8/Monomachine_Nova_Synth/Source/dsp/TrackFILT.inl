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

        if(filterMode==monomachine::dspModeMnm){
            // P2's documented neutral filter values are an insertion THRU.
            // This shortcut is retained only while no explicitly enabled
            // VEL/KT/SAT/FIL ENV control can make the stage audible.
            if(neutralFilterThru&&hybridLowerMode==0&&hybridUpperMode==0&&hasNeutralMnmFilter()){
                if(!neutralFilterThruActive){mnmFilter.reset();neutralFilterThruActive=true;}
                return;
            }
            neutralFilterThruActive=false;
            const float fltBase=params[16],fltWidth=params[17],fltHpq=params[18],fltLpq=params[19];
            const float fltAtk=params[20],fltDec=params[21];
            const float fltBofs=params[22]-64.0f,fltWofs=params[23]-64.0f;
            mnmFilter.setParameters(fltBase,fltWidth,fltHpq,fltLpq,fltAtk,fltDec,fltBofs,fltWofs);
            for(int i=0;i<n;++i){
                const auto offsets=nextFilEnvOffsets();
                mnmFilter.setExternalFilterModifiers(lowerSemitones,upperSemitones,offsets.first,offsets.second);
                l[i]=mnmFilter.process(0,l[i]);r[i]=mnmFilter.process(1,r[i]);
            }
        }
        else {
            neutralFilterThruActive=false;
            for(int i=0;i<n;++i){
                const auto offsets=nextFilEnvOffsets();
                filter.setExternalFilterModifiers(lowerSemitones,upperSemitones,offsets.first,offsets.second);
                filter.processStereo(l+i,r+i,l+i,r+i,1);
            }
        }
        if(filterSaturation.active()){
            for(int i=0;i<n;++i){
                l[i]=filterSaturation.process(0,l[i]);
                r[i]=filterSaturation.process(1,r[i]);
            }
        }
    }
