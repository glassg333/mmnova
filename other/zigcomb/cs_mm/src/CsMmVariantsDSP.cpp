#include "csmm/CsMmVariantsDSP.h"

#include <algorithm>
#include <cmath>

namespace csmm
{
namespace
{
constexpr float pi = 3.14159265358979323846f;
constexpr float twoPi = 2.0f * pi;
constexpr std::array<const char*, CsMmVariantsDSP::variantCount> names {
    "01 Harmonic Chorus Clean", "02 Harmonic Chorus Wide", "03 Inharmonic Glass Flange",
    "04 Inharmonic Phase Teeth", "05 Stretched Slow Bloom", "06 Stretched Fast Motion",
    "07 Cluster Dark Swarm", "08 Cluster Bright Swarm", "09 Broken Tape Drift",
    "10 Percussive Metallic", "11 Prime Ring", "12 Prime Wide", "13 Octave Chorus",
    "14 Octave Phase", "15 Shimmer Comb", "16 Shimmer Dark", "17 Rubber Resonator",
    "18 Rubber Stereo", "19 Frozen Metal", "20 Liquid Metal"
};
}

const char* CsMmVariantsDSP::getVariantName(int index) noexcept
{
    const auto safe = static_cast<std::size_t>(clamp(static_cast<float>(index), 0.0f,
                                                      static_cast<float>(variantCount - 1)));
    return names[safe];
}

CsMmVariantsDSP::Config CsMmVariantsDSP::configFor(int variant) noexcept
{
    switch (variant)
    {
        case 1: return Config{{.25f,.50f,1,1.5f,2,3,4,6},.52f,.42f,1.1f,.03f,.05f,.75f,.1f,1.2f,.88f,2.2f,4.2f,1.3f,.62f,2.5f,false,false};
        case 2: return Config{{.37f,.59f,.83f,1.17f,1.41f,1.73f,2.11f,2.63f},.50f,.44f,1,.04f,.35f,.90f,.15f,1.15f,.88f,3.2f,3.8f,.75f,.70f,2.8f,false,false};
        case 3: return Config{{.31f,.47f,.71f,1.09f,1.37f,1.91f,2.47f,3.19f},.55f,.40f,.70f,.09f,.55f,.85f,.25f,1.25f,.88f,1.6f,5.0f,2.4f,.54f,3.0f,false,false};
        case 4: return Config{{.20f,.32f,.51f,.79f,1.27f,1.93f,2.91f,4.37f},.57f,.38f,.80f,.045f,.35f,.75f,.10f,1.10f,.90f,4.5f,2.2f,.35f,.58f,2.2f,false,false};
        case 5: return Config{{.16f,.29f,.46f,.73f,1.31f,2.08f,3.11f,4.70f},.59f,.36f,1.25f,.10f,.25f,.82f,.20f,1.25f,.90f,2.0f,5.0f,3.8f,.64f,2.9f,false,false};
        case 6: return Config{{.82f,.91f,.97f,1,1.04f,1.11f,1.19f,1.31f},.62f,.33f,1,.025f,.70f,.50f,.35f,.75f,.90f,5.0f,1.4f,.22f,.72f,1.8f,false,false};
        case 7: return Config{{.68f,.82f,.94f,1,1.08f,1.18f,1.34f,1.52f},.50f,.45f,1.15f,.055f,.08f,.95f,.40f,1.15f,.86f,1.8f,4.5f,1.8f,.67f,3.1f,false,false};
        case 8: return Config{{.27f,.43f,.64f,.98f,1.44f,2.01f,2.79f,3.77f},.58f,.39f,.90f,.18f,.42f,.75f,.25f,1.30f,.90f,2.8f,4.8f,2.6f,.58f,2.4f,false,true};
        case 9: return Config{{.12f,.24f,.36f,.50f,.75f,1,1.5f,2},.47f,.43f,.85f,.02f,.18f,.65f,.05f,1,.84f,1.2f,2.2f,.45f,.52f,1.4f,false,false};
        case 10: return Config{{.17f,.29f,.43f,.71f,1.13f,1.61f,2.37f,3.41f},.54f,.42f,.95f,.06f,.22f,.72f,.12f,1.10f,.88f,3.8f,2.6f,.55f,.62f,2.0f,false,false};
        case 11: return Config{{.23f,.37f,.53f,.79f,1.27f,1.67f,2.29f,3.17f},.52f,.43f,1.25f,.045f,.48f,.84f,.30f,1.30f,.88f,2.6f,3.6f,1.1f,.66f,2.9f,false,false};
        case 12: return Config{{.23f,.37f,.53f,.79f,1.27f,1.67f,2.29f,3.17f},.55f,.40f,.75f,.08f,.60f,.90f,.48f,1.45f,.90f,1.5f,4.5f,2.1f,.60f,3.0f,false,true};
        case 13: return Config{{.50f,1,2,3,4,5,6,8},.51f,.43f,1,.025f,.12f,.70f,.10f,1.1f,.86f,2.0f,3.0f,.65f,.60f,2.2f,false,false};
        case 14: return Config{{.50f,1,2,3,4,5,6,8},.56f,.38f,1.2f,.07f,.55f,.90f,.30f,1.3f,.90f,3.4f,4.4f,2.7f,.63f,3.1f,false,true};
        case 15: return Config{{.28f,.42f,.70f,1,1.42f,2.12f,2.84f,4.26f},.50f,.44f,.92f,.05f,.20f,.82f,.18f,1.2f,.88f,1.4f,5.0f,1.9f,.72f,2.7f,false,false};
        case 16: return Config{{.28f,.42f,.70f,1,1.42f,2.12f,2.84f,4.26f},.61f,.31f,1.15f,.03f,.75f,.48f,.42f,.82f,.90f,4.8f,1.5f,.28f,.56f,1.7f,false,false};
        case 17: return Config{{.36f,.48f,.64f,.86f,1.16f,1.56f,2.16f,2.96f},.58f,.38f,.82f,.12f,.38f,.76f,.28f,1.1f,.90f,2.3f,4.6f,3.3f,.60f,2.5f,false,false};
        case 18: return Config{{.36f,.48f,.64f,.86f,1.16f,1.56f,2.16f,2.96f},.54f,.42f,.98f,.09f,.30f,.88f,.38f,1.5f,.88f,1.9f,3.9f,1.6f,.70f,3.14f,false,true};
        case 19: return Config{{.41f,.61f,.83f,1.07f,1.39f,1.83f,2.41f,3.07f},.63f,.31f,1.05f,.012f,.62f,.58f,.32f,.86f,.90f,5.0f,.9f,.12f,.78f,1.5f,false,false};
        default: return Config{{.50f,1,1.5f,2,2.5f,3,4,5},.48f,.46f,.90f,.015f,.10f,.65f,.05f,1,.86f,2.5f,2.5f,.8f,.58f,2.2f,false,false};
    }
}

void CsMmVariantsDSP::DelayLine::prepare(double rate, double maximumDelayMs)
{
    const auto size = static_cast<std::size_t>(std::ceil(std::max(1.0, rate) * maximumDelayMs / 1000.0)) + 4u;
    buffer.assign(std::max<std::size_t>(size, 8u), 0.0f);
    writeIndex = 0;
}

void CsMmVariantsDSP::DelayLine::clear() noexcept
{
    std::fill(buffer.begin(), buffer.end(), 0.0f);
    writeIndex = 0;
}

float CsMmVariantsDSP::DelayLine::read(float delaySamples) const noexcept
{
    if (buffer.empty()) return 0.0f;
    delaySamples = std::clamp(delaySamples, 1.0f, static_cast<float>(buffer.size() - 2u));
    double position = static_cast<double>(writeIndex) - delaySamples;
    while (position < 0.0) position += static_cast<double>(buffer.size());
    const auto i0 = static_cast<std::size_t>(position) % buffer.size();
    const auto i1 = (i0 + 1u) % buffer.size();
    const float fraction = static_cast<float>(position - std::floor(position));
    return buffer[i0] + fraction * (buffer[i1] - buffer[i0]);
}

void CsMmVariantsDSP::DelayLine::write(float value) noexcept
{
    if (!buffer.empty()) { buffer[writeIndex] = value; writeIndex = (writeIndex + 1u) % buffer.size(); }
}

float CsMmVariantsDSP::clamp(float value, float low, float high) noexcept { return std::clamp(value, low, high); }

float CsMmVariantsDSP::dcBlock(Voice& voice, float input) noexcept
{
    const float output = input - voice.dcInput + 0.995f * voice.dcOutput;
    voice.dcInput = input; voice.dcOutput = output; return output;
}

void CsMmVariantsDSP::prepare(double newRate, int blockSize)
{
    sampleRate = std::max(1.0, newRate); maximumBlockSize = std::max(1, blockSize); (void) maximumBlockSize;
    for (auto& voice : voices) { voice.delay1.prepare(sampleRate, 7000.0); voice.delay2.prepare(sampleRate, 7000.0); voice.chorus.left.prepare(sampleRate, 40.0); voice.chorus.right.prepare(sampleRate, 40.0); }
    reset();
}

void CsMmVariantsDSP::reset() noexcept
{
    for (std::size_t i = 0; i < voices.size(); ++i) { auto& v=voices[i]; v.delay1.clear();v.delay2.clear();v.chorus.left.clear();v.chorus.right.clear();v.dampingState=0;v.allpassState=0;v.dcInput=0;v.dcOutput=0;v.phase=static_cast<float>(i)*.73f;v.chorus.phase=static_cast<float>(i)*.41f; }
    wetLeft=0;wetRight=0;smoothedScan=0;smoothedMono=0;smoothedWetLeft=0;smoothedWetRight=0;
}

void CsMmVariantsDSP::setParameters(Parameters p)
{
    parameters.feedback=clamp(p.feedback,0,.999f);parameters.damp=clamp(p.damp,0,.999f);parameters.phase=clamp(p.phase,0,1);parameters.diffusion=clamp(p.diffusion,0,1);parameters.crossMix=clamp(p.crossMix,0,1);parameters.motion=clamp(p.motion,0,1);parameters.chorusMix=clamp(p.chorusMix,0,1);parameters.chorusDepth=clamp(p.chorusDepth,0,1);parameters.chorusRate=clamp(p.chorusRate,0,1);parameters.delay1Ms=clamp(p.delay1Ms,0,2000);parameters.delay2Ms=clamp(p.delay2Ms,0,2000);parameters.scan=clamp(p.scan,0,1);parameters.variant=static_cast<int>(clamp(static_cast<float>(p.variant),0,static_cast<float>(variantCount-1)));config=configFor(parameters.variant);
}

float CsMmVariantsDSP::processMono(float input) noexcept
{
    const float oneMs=1000.0f/static_cast<float>(sampleRate), feedback=config.feedbackFloor+config.feedbackRange*parameters.feedback; std::array<float,8> outL{},outR{};float previous=0;
    for(std::size_t i=0;i<voices.size();++i){auto&v=voices[i];v.phase+=(.012f+.028f*parameters.phase)*(1+.08f*i)/static_cast<float>(sampleRate);if(v.phase>=twoPi)v.phase-=twoPi;const float motion=std::sin(v.phase)*config.modulationDepth*(.2f+1.8f*parameters.motion);const float ratio=config.ratios[i]*(1+motion);const float tap=.5f*(v.delay1.read(std::max(oneMs,parameters.delay1Ms*ratio)*static_cast<float>(sampleRate)/1000)+v.delay2.read(std::max(oneMs,parameters.delay2Ms*ratio)*static_cast<float>(sampleRate)/1000));const float cross=clamp(config.crossMix+.75f*(parameters.crossMix-.5f),0,.92f);const float comb=tap*(1-cross)+previous*cross;v.dampingState+=(.08f+.2f*(1-parameters.damp)+.03f*parameters.diffusion)*(comb-v.dampingState);const float reson=.7f*comb+.3f*v.dampingState;const float coeff=clamp(config.allpassBase*(.2f+.8f*parameters.diffusion)+config.allpassPhaseDepth*parameters.phase*(.25f+.75f*parameters.diffusion)+.04f*std::sin(v.phase*1.7f),-.92f,.92f);const float ap=-coeff*reson+v.allpassState;v.allpassState=reson+coeff*ap;v.delay1.write(clamp(input*.42f+feedback*(.72f*reson+.28f*ap),-1.5f,1.5f));v.delay2.write(clamp(input*.42f+feedback*(.52f*reson+.48f*ap),-1.5f,1.5f));const float raw=dcBlock(v,.72f*comb+.28f*ap);const float base=config.chorusBaseMs*(.7f+.6f*parameters.chorusMix);const float depth=config.chorusDepthMs*(.25f+.75f*parameters.chorusDepth);const float rate=config.chorusRateHz*(.35f+1.65f*parameters.chorusRate);v.chorus.phase+=twoPi*rate/static_cast<float>(sampleRate);if(v.chorus.phase>=twoPi)v.chorus.phase-=twoPi;v.chorus.left.write(raw);v.chorus.right.write(raw);const float spread=config.chorusSpread*(.35f+.65f*parameters.phase);const float cL=v.chorus.left.read((base+depth*std::sin(v.chorus.phase))*static_cast<float>(sampleRate)/1000);const float cR=v.chorus.right.read((base+depth*std::sin(v.chorus.phase+spread))*static_cast<float>(sampleRate)/1000);const float mix=clamp(config.chorusMix*parameters.chorusMix*1.55f,0,1);outL[i]=(1-mix)*raw+mix*cL;outR[i]=(1-mix)*raw+mix*cR;previous=.5f*(outL[i]+outR[i]);}
    const float scanTime=.05f-.035f*parameters.motion,scanAlpha=1-std::exp(-1/(scanTime*static_cast<float>(sampleRate)));smoothedScan+=scanAlpha*(parameters.scan-smoothedScan);float pos=std::pow(smoothedScan,config.scanCurve)*7;if(config.reverseScan)pos=(1-smoothedScan)*7;const auto first=static_cast<std::size_t>(std::min(7.0f,pos)),second=(first+1u)%8;const float f=pos-static_cast<float>(first);auto pan=[this](std::size_t i){float p=(config.alternatePan&&(i%2))?1-static_cast<float>(i)/7:static_cast<float>(i)/7;return clamp(.5f+(p-.5f)*config.panSpread,0,1);};const float p0=pan(first),p1=pan(second),w0=std::cos(f*pi*.5f),w1=std::sin(f*pi*.5f);wetLeft=config.wet*(w0*outL[first]*std::cos(p0*pi*.5f)+w1*outL[second]*std::cos(p1*pi*.5f));wetRight=config.wet*(w0*outR[first]*std::sin(p0*pi*.5f)+w1*outR[second]*std::sin(p1*pi*.5f));return .5f*(wetLeft+wetRight);
}

void CsMmVariantsDSP::processBlock(float* left,float* right,int numSamples) noexcept
{
    if(!left||numSamples<=0)return;const float alpha=1-std::exp(-1/(.00008f*static_cast<float>(sampleRate)));constexpr float dry=.12f,makeup=2.8f;if(!right){for(int i=0;i<numSamples;++i){const float input=left[i],wet=processMono(input);smoothedMono+=alpha*(wet-smoothedMono);left[i]=std::tanh(makeup*(dry*input+(1-dry)*smoothedMono));}return;}for(int i=0;i<numSamples;++i){const float il=left[i],ir=right[i];processMono(.5f*(il+ir));smoothedWetLeft+=alpha*(wetLeft-smoothedWetLeft);smoothedWetRight+=alpha*(wetRight-smoothedWetRight);left[i]=std::tanh(makeup*(dry*il+(1-dry)*smoothedWetLeft));right[i]=std::tanh(makeup*(dry*ir+(1-dry)*smoothedWetRight));}
}

void CsMmVariantsDSP::processBlock(float* const* channels,int numChannels,int numSamples) noexcept
{ if(channels&&numChannels>0)processBlock(channels[0],numChannels>1?channels[1]:nullptr,numSamples); }
}
