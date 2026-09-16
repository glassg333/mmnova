#pragma once
#include <array>
#include <cstddef>
#include <algorithm>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <cstdint>

namespace monomachine {
// Allocation-free, note-synchronised native arp. Not firmware emulation.
class MonomachineArpeggiator {
public:
    enum class Mode : uint8_t { Off, Key, Sid, Add };
    enum class Play : uint8_t { True, Up, Down, Cycl, Rnd };
    struct ArpSettings { Mode mode=Mode::Off; Play play=Play::Up; uint8_t speed=6, range=1, noteLength=64; };
    struct ArpNoteEvent { bool triggered=false; uint8_t note=60, velocity=127; bool isNoteOff=false; };
    void reset(double rate=44100, double bpm=120) {
        sampleRate=rate; tempo=bpm; down.fill(false); orders.fill(0); latched.fill(0);
        serial=0; step=0; clock=gate=0; sounding=false; rng=1234567;
    }
    void setTempo(double bpm) { tempo=std::clamp(bpm,30.0,300.0); }
    void setSettings(const ArpSettings& s) { settings=s; settings.range=std::clamp<uint8_t>(s.range,1,4); }
    void noteOn(uint8_t note,uint8_t vel=127) {
        if (note>127) return;
        down[note]=true; orders[note]=++serial; latched[note]=serial; velocity[note]=vel;
    }
    void noteOff(uint8_t note) {
        if(note>127) return;
        down[note]=false; orders[note]=0;
        if(settings.mode==Mode::Add && std::none_of(down.begin(),down.end(),[](bool x){return x;})) latched.fill(0);
    }
    int samplesUntilEvent() const {
        if(settings.mode==Mode::Off) return std::numeric_limits<int>::max();
        if(poolSize()==0) return sounding ? 0 : std::numeric_limits<int>::max();
        return std::max(0,static_cast<int>(std::ceil(sounding ? std::min(clock,gate) : clock)));
    }
    void advance(int samples) { clock-=samples; if(sounding) gate-=samples; }
    ArpNoteEvent poll() {
        ArpNoteEvent ev;
        if(settings.mode==Mode::Off) return ev;
        auto count=makePool();
        if(sounding && (gate<=0 || count==0)) {
            sounding=false; ev.triggered=true; ev.isNoteOff=true;
            if(count==0) {clock=0;step=0;}
            return ev;
        }
        if(count==0 || clock>0) return ev;
        const double duration=sampleRate*60.0/tempo/24.0*std::max<int>(1,settings.speed);
        clock+=std::max(2.0,duration);
        gate=std::max(1.0,std::min(duration-1.0,duration*settings.noteLength/128.0));
        size_t index=0;
        const size_t total=count*settings.range;
        if(settings.play==Play::Rnd) { rng^=rng<<13;rng^=rng>>17;rng^=rng<<5;index=rng%total; }
        else if(settings.play==Play::Cycl && total>1) {size_t x=step%(2*total-2);index=x<total?x:2*total-2-x;}
        else index=step%total;
        ++step;
        auto note=pool[index%count];
        ev.note=static_cast<uint8_t>(std::min(127,static_cast<int>(note+12*(index/count))));
        ev.velocity=velocity[note];ev.triggered=true;sounding=true;
        return ev;
    }
    // Compatibility helper; the plugin uses poll/advance to preserve multiple events per block.
    ArpNoteEvent processBlock(size_t samples) {auto ev=poll();advance(static_cast<int>(samples));return ev;}
    static std::vector<std::string> getArpDropdownHierarchy() {return {"OFF","KEY","SID (latch)","ADD","TRUE","UP","DOWN","CYCL","RND"};}
private:
    bool useLatch() const {return settings.mode==Mode::Sid || settings.mode==Mode::Add;}
    size_t poolSize() const {size_t n=0;for(size_t i=0;i<128;++i) if((useLatch()?latched[i]:orders[i])!=0) ++n;return n;}
    size_t makePool() {
        size_t n=0;for(size_t i=0;i<128;++i) if((useLatch()?latched[i]:orders[i])!=0) pool[n++]=static_cast<uint8_t>(i);
        if(settings.play==Play::Down) std::reverse(pool.begin(),pool.begin()+static_cast<std::ptrdiff_t>(n));
        if(settings.play==Play::True) std::sort(pool.begin(),pool.begin()+static_cast<std::ptrdiff_t>(n),[this](uint8_t a,uint8_t b){return (useLatch()?latched[a]:orders[a])<(useLatch()?latched[b]:orders[b]);});
        return n;
    }
    ArpSettings settings;
    std::array<bool,128> down{};
    std::array<uint64_t,128> orders{},latched{};
    std::array<uint8_t,128> velocity{},pool{};
    uint64_t serial=0,step=0;uint32_t rng=1234567;
    double sampleRate=44100,tempo=120,clock=0,gate=0;
    bool sounding=false;
};
}
