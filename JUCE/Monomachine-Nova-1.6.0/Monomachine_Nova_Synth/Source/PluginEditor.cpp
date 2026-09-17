#include "PluginEditor.h"
#include "PixelFont.h"

// v1.3 UI pass, all per the black reference screen: white ink on black, outlined boxes,
// list knobs change by press-and-drag (the popup stays as a bonus), BPM drags instead of
// opening settings, the matrix gets its own pixel page, and the FX build carries a
// MIDI/FREE gate toggle right on the main screen.
namespace {
const juce::Colour ink=juce::Colours::white;      // foreground on black, like the LCD
const juce::Colour knockout=juce::Colours::black; // ink knocked out of filled bars
const juce::Colour dim=juce::Colour(0xff737373);
const juce::Colour panel=juce::Colour(0xff181818);

class PixelLabel final : public juce::Label {
public:
    void paint(juce::Graphics& g) override { g.fillAll(juce::Colours::black); if (!isBeingEdited()) { g.setColour(isEnabled() ? ink : dim); pixel::text(g, getText(), getLocalBounds().reduced(2, 0), 14, false); } }
};

class PixelTheme final : public juce::LookAndFeel_V4 {
public:
    PixelTheme(){setColour(juce::PopupMenu::backgroundColourId,juce::Colours::black);setColour(juce::PopupMenu::textColourId,ink);
        setColour(juce::PopupMenu::highlightedBackgroundColourId,ink);setColour(juce::PopupMenu::highlightedTextColourId,knockout);
        setColour(juce::ComboBox::backgroundColourId,juce::Colours::black);setColour(juce::ComboBox::textColourId,ink);
        setColour(juce::ComboBox::outlineColourId,ink);setColour(juce::ComboBox::buttonColourId,ink);
        setColour(juce::Slider::textBoxTextColourId,ink);setColour(juce::Slider::textBoxBackgroundColourId,juce::Colours::black);
        setColour(juce::Slider::textBoxOutlineColourId,juce::Colours::black);setColour(juce::Slider::textBoxHighlightColourId,juce::Colour(0xff404040));
        setColour(juce::TextEditor::backgroundColourId,juce::Colours::black);setColour(juce::TextEditor::textColourId,ink);
        setColour(juce::TextEditor::highlightColourId,ink);setColour(juce::TextEditor::highlightedTextColourId,knockout);
        setColour(juce::TextButton::buttonColourId,juce::Colours::black);setColour(juce::TextButton::textColourOffId,ink);
        setColour(juce::TextButton::textColourOnId,ink);
        setColour(juce::Label::textColourId,ink);setColour(juce::TooltipWindow::backgroundColourId,juce::Colours::black);
    }
    juce::Label* createSliderTextBox(juce::Slider&) override { auto* l = new PixelLabel(); l->setJustificationType(juce::Justification::centred); l->setColour(juce::TextEditor::textColourId, ink); l->setColour(juce::TextEditor::backgroundColourId, juce::Colours::black); return l; }
    juce::Label* createComboBoxTextBox(juce::ComboBox&) override { auto* l = new PixelLabel(); l->setJustificationType(juce::Justification::centred); return l; }
    void positionComboBoxText(juce::ComboBox& box, juce::Label& label) override { label.setBounds(5, 2, box.getWidth() - 28, box.getHeight() - 4); }
    void drawButtonBackground(juce::Graphics& g, juce::Button& b, const juce::Colour&, bool hover, bool down) override { const bool filled = b.getToggleState() || down; g.fillAll(filled ? ink : juce::Colours::black); g.setColour(b.isEnabled() ? ink : dim); g.drawRect(b.getLocalBounds().reduced(1), hover ? 2 : 1); }
    void drawButtonText(juce::Graphics& g, juce::TextButton& b, bool, bool down) override { g.setColour(!b.isEnabled() ? dim : (b.getToggleState() || down ? knockout : ink)); pixel::text(g, b.getButtonText(), b.getLocalBounds().reduced(4, 0), 14, true); }
    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& b, bool, bool) override { g.fillAll(juce::Colours::black); g.setColour(ink); g.drawRect(2, 5, 18, 18, 1); if (b.getToggleState()) { g.drawLine(5, 14, 9, 18, 2); g.drawLine(9, 18, 16, 9, 2); } pixel::text(g, b.getButtonText(), {26, 0, b.getWidth() - 28, b.getHeight()}, 13, false); }
    void drawTickBox(juce::Graphics& g,juce::Component&,float x,float y,float w,float h,bool ticked,bool enabled,bool,bool)override{juce::Rectangle<float> box(x+2,y+(h-14)*0.5f,std::min(14.0f,w-4),14);g.setColour(juce::Colours::black);g.fillRect(box);g.setColour(ink.withAlpha(enabled?0.85f:0.3f));g.drawRect(box,1);if(ticked)g.fillRect(box.reduced(3));}
    void drawLinearSlider(juce::Graphics& g,int x,int y,int width,int height,float pos,float min,float max,const juce::Slider::SliderStyle style,juce::Slider& slider)override{
        if(style==juce::Slider::LinearVertical){g.setColour(ink);g.fillRect(x+width/2-6,juce::roundToInt(pos),12,std::max(1,y+height-juce::roundToInt(pos)));}
        else { g.setColour(ink.withAlpha(0.22f)); g.fillRect(x + 2, y + height / 2 - 2, width - 4, 4); g.setColour(ink); g.fillRect(x + 2, y + height / 2 - 2, std::max(2, juce::roundToInt(pos - x)), 4); g.fillEllipse(juce::roundToInt(pos) - 6.0f, y + height / 2 - 6.0f, 12.0f, 12.0f); }
        juce::ignoreUnused(min, max, slider);
    }
    void drawRotarySlider(juce::Graphics& g,int x,int y,int width,int height,float pos,float start,float end,juce::Slider&)override{
        int cx=x+width/2,cy=y+height/2;g.setColour(ink);
        for(int i=0;i<16;++i){float a=2*nova::pi*i/16;int px=cx+juce::roundToInt(std::sin(a)*14),py=cy+juce::roundToInt(std::cos(a)*14);g.fillRect(px-1,py-1,3,3);}
        const float angle=start+pos*(end-start);
        for(int i=3;i<=11;++i)g.fillRect(cx+juce::roundToInt(std::sin(angle)*i)-1,cy-juce::roundToInt(std::cos(angle)*i)-1,3,3);
    }
    void drawComboBox(juce::Graphics& g,int width,int height,bool isDown,int buttonX,int,int,int,juce::ComboBox&)override{
        g.fillAll(juce::Colours::black);g.setColour(ink);g.drawRect(0,0,width-1,height-1);
        int ax=buttonX+width/2-4-2*(isDown?1:-1)+2;
        for(int i=0;i<5;++i)g.fillRect(ax-3+i,height/2+(isDown?i:-i)-(isDown?0:4),2,2);
    }
};

// Outlined pixel button. `dragHandler(dx)` fires while the pointer moves horizontally,
// so the same control supports click (popup / toggle) and press-drag (cycle values).
class PixelButton : public juce::TextButton {
public:
    explicit PixelButton(const juce::String& s):juce::TextButton(s){}
    int textHeight=28;
    int textAlign=0; // 0 centred, 1 left
    bool outlined=true;
    std::function<void(int)> dragHandler;std::function<void()> doubleClickHandler;int dragPixelsPerStep=6;
    void mouseDoubleClick(const juce::MouseEvent& e)override{if(doubleClickHandler)doubleClickHandler();else juce::TextButton::mouseDoubleClick(e);}
    void mouseWheelMove(const juce::MouseEvent& e,const juce::MouseWheelDetails& w)override{if(dragHandler&&std::abs(w.deltaY)>0.0001f)dragHandler(w.deltaY>0?1:-1);else juce::TextButton::mouseWheelMove(e,w);}
    void paintButton(juce::Graphics& g,bool /*over*/,bool down)override{
        g.fillAll(down&&outlined?ink:juce::Colours::black);
        if(outlined){g.setColour(ink);g.drawRect(getLocalBounds().reduced(1),1);}
        g.setColour(down&&outlined?knockout:ink);
        pixel::text(g,getButtonText(),getLocalBounds().reduced(5,0),textHeight,textAlign==0);
    }
    void mouseDown(const juce::MouseEvent&e)override{dragStartX=e.x;dragAccum=0;wasDragged=false;juce::TextButton::mouseDown(e);}
    void mouseDrag(const juce::MouseEvent&e)override{
        if(!dragHandler)return;const int dx=e.x-dragStartX;const int pixels=std::max(1,dragPixelsPerStep);
        if(std::abs(dx)>=pixels){wasDragged=true;const int steps=(dx-dragAccum)/pixels;if(steps!=0){dragAccum+=steps*pixels;dragHandler(steps);}}
    }
    void mouseUp(const juce::MouseEvent&e)override{juce::TextButton::mouseUp(e);wasDragged=false;}
    void clicked()override{if(wasDragged)return;juce::TextButton::clicked();}
private:
    int dragStartX=0,dragAccum=0;bool wasDragged=false;
};
class MsegOutputButton final : public PixelButton {
public:
    explicit MsegOutputButton(const juce::String& s):PixelButton(s){dragPixelsPerStep=1000000;}
    void mouseDown(const juce::MouseEvent& e) override { PixelButton::mouseDown(e); if (e.mods.isLeftButtonDown()) if (auto* d = juce::DragAndDropContainer::findParentDragContainerFor(this)) d->startDragging("MSEG", this, juce::ScaledImage(), true); }
};

// Host-value helpers shared by the list cells: every parameter in this plugin is an
// AudioParameterChoice or integer-scaled AudioParameterFloat, so stepping by
// convertTo0to1/convertFrom0to1 pairs lands exactly on legal values without touching
// version-specific range APIs.
inline int choiceCount(juce::RangedAudioParameter* p){
    if(!p)return 0;const auto names=p->getAllValueStrings();if(names.size()>0)return names.size();
    return juce::roundToInt(p->convertFrom0to1(1.0f))+1; // integer 0..N parameter
}
inline float hostFor(juce::RangedAudioParameter& p,int index){
    return std::clamp(p.convertTo0to1(static_cast<float>(index)),0.0f,1.0f);
}
inline void stepBy(juce::RangedAudioParameter& p,int steps){
    const int raw=juce::roundToInt(p.convertFrom0to1(p.getValue()));
    const int next=std::clamp(raw+steps,0,choiceCount(&p)-1);
    p.setValueNotifyingHost(hostFor(p,next));
}

static juce::String targetName(MonomachineNovaAudioProcessor& p,int target){
    if(target<8)return "SYNT "+juce::String(nova::machines()[static_cast<size_t>(p.machineIndex())].synthParams[static_cast<size_t>(juce::jlimit(0,7,target))].name);
    const char* names[]{"AMP","FILT","EFFX","LFO1","LFO2","LFO3"};
    const int page=juce::jlimit(0,5,(target-8)/8);return juce::String(names[page])+" "+nova::pageLabel(page,(target-8)%8);
}
class CurveSlider final : public juce::Slider {
public:
    // Right mouse button is reserved for alternative actions (curve editor, DSP mode list,
    // modulation pickers) and must never rotate the parameter - user rule from the 1.5.2 prompt.
    std::function<void()> curveMenu;std::function<void()> modeMenu;bool popup=false;
    void mouseDown(const juce::MouseEvent& e)override{
        if(e.mods.isPopupMenu()){popup=true;if(curveMenu)curveMenu();else if(modeMenu)modeMenu();return;}
        popup=false;juce::Slider::mouseDown(e);}
    void mouseDrag(const juce::MouseEvent& e)override{if(popup||e.mods.isPopupMenu())return;juce::Slider::mouseDrag(e);}
    void mouseUp(const juce::MouseEvent& e)override{if(popup||e.mods.isPopupMenu()){popup=false;return;}juce::Slider::mouseUp(e);}
};
class Cell final : public juce::Component, public juce::FileDragAndDropTarget, public juce::DragAndDropTarget, private juce::Timer {
public:
    explicit Cell(MonomachineNovaAudioProcessor& p):processor(p){
        slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);slider.setTextBoxStyle(juce::Slider::NoTextBox,false,0,0);
        slider.setScrollWheelEnabled(true);addAndMakeVisible(slider);startTimerHz(20);
    }
    std::function<void()> openSamples;std::function<void(int)> openEnvelope;std::function<void(int)> openModes;std::function<void(uint8_t)> addMsegModulation;std::function<void(uint8_t)> pickTarget;
    void setPickMode(bool enabled){pickMode=enabled;slider.setVisible(parameter&&!isChoice&&!enabled);repaint();}
    bool isTarget(uint8_t target) const noexcept { return modulationTarget() == target; }
    void flashPick(){flashTicks=18;repaint();}
    std::function<void(uint8_t)> pickHover;
    bool isInterestedInDragSource(const juce::DragAndDropTarget::SourceDetails& details) override { return details.description.toString() == "MSEG" && parameter != nullptr && !isChoice; }
    void itemDropped(const juce::DragAndDropTarget::SourceDetails& details) override { if (details.description.toString() == "MSEG" && addMsegModulation) addMsegModulation(modulationTarget()); }
    bool isInterestedInFileDrag(const juce::StringArray& files)override{return machine==7&&knob==2&&!files.isEmpty();}
    void filesDropped(const juce::StringArray& files,int,int)override{
        int slot=juce::roundToInt(processor.parameters.getRawParameterValue("m7_2")->load());
        juce::String errors;for(const auto& name:files){if(slot>=24)break;auto error=processor.loadSample(slot++,juce::File(name));if(error.isNotEmpty())errors+=error+"\n";}
        if(errors.isNotEmpty())juce::AlertWindow::showMessageBoxAsync(juce::MessageBoxIconType::WarningIcon,"Sample import",errors);
    }
    juce::String choiceName(int value)const{
        if(pageIndex>=3&&knob==1){const int page=juce::jlimit(0,7,juce::roundToInt(processor.parameters.getRawParameterValue(nova::pageParam(pageIndex,0))->load()));
            if(page==0){const char* ranges[]{"1ST","2ST","3ST","5ST","7ST","1OCT","2OCT","3OCT"};return ranges[juce::jlimit(0,7,value)];}
            return targetName(processor,(page-1)*8+juce::jlimit(0,7,value));}
        const auto names=parameter->getAllValueStrings();return names.size()>value?names[value]:parameter->getText(hostFor(*parameter,value),32);
    }
    void bind(const juce::String& parameterId,const juce::String& name,int page=-1,int index=-1,int engine=-1){
        attachment.reset();id=parameterId;label=name;pageIndex=page;knob=index;machine=engine;
        parameter=id.isEmpty()?nullptr:processor.parameters.getParameter(id);
        slider.curveMenu={};if(pageIndex==0&&(knob==0||knob==2||knob==3))slider.curveMenu=[this]{if(openEnvelope)openEnvelope(knob);};
        // RMB elsewhere opens the DSP mode list of the section this knob belongs to
        // (prompt: "по пкм крутилка можно сделать переключение режима").
        slider.modeMenu={};
        {
            int section=-1;
            if(machine>=0&&pageIndex<0)section=monomachine::DspSynt;
            else if(pageIndex==0)section=monomachine::DspAmp;
            else if(pageIndex==1)section=monomachine::DspFilter;
            else if(pageIndex==2)section=(knob>=3?monomachine::DspDelay:monomachine::DspDist);
            if(section>=0&&!slider.curveMenu)slider.modeMenu=[this,section]{if(openModes)openModes(section);};
        }
        isChoice=parameter&&(parameter->getAllValueStrings().size()>0||(pageIndex>=3&&knob==1));
        slider.setVisible(parameter&&!isChoice);setMouseCursor(parameter?juce::MouseCursor::PointingHandCursor:juce::MouseCursor::NormalCursor);
        if(parameter&&!isChoice){attachment=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.parameters,id,slider);slider.setDoubleClickReturnValue(true,parameter->convertFrom0to1(parameter->getDefaultValue()));} // double-click restores the stored default
        repaint();
    }
    juce::String displayedValue()const{
        if(!parameter)return "---";float v=parameter->convertFrom0to1(parameter->getValue());int raw=juce::roundToInt(v);
        if(pageIndex>=3&&knob==1){int page=juce::roundToInt(processor.parameters.getRawParameterValue(nova::pageParam(pageIndex,0))->load());
            if(page==0){const char* ranges[]{"1ST","2ST","3ST","5ST","7ST","1OCT","2OCT","3OCT"};return ranges[juce::jlimit(0,7,raw)];}
            if(page==1)return juce::String(processor.machineIndex()>=0?nova::machines()[static_cast<size_t>(processor.machineIndex())].synthParams[static_cast<size_t>(juce::jlimit(0,7,raw))].name:"---");
            return nova::pageLabel(page-2,juce::jlimit(0,7,raw));}
        if(isChoice)return parameter->getText(parameter->getValue(),32);
        if(machine==10&&knob==0)return juce::String(v/16,1);
        if(machine==10&&knob==4)return juce::String(std::pow(2.0f,(v-32)/24),2);
        if((machine==8||machine==9)&&(label=="1FRQ"||label=="2FRQ"||label=="3FRQ"))return juce::String(monomachine::getFmListedRatio(static_cast<uint8_t>(raw/4)),2);
        if(machine==2&&knob==3)return juce::String(raw-64);
        if(label=="TUNE"&&machine!=17)return juce::String(raw-64);
        // DIST (AMP page) and DSND (FX page) are bipolar on hardware: centre 0, -64..+63.
        if((pageIndex==0&&knob==4)||(pageIndex==2&&knob==4))return juce::String(raw-64);
        if((machine==10&&knob==1)||(machine==8&&knob==1)||(pageIndex==0&&knob==6)||(pageIndex==1&&knob>=6)||(pageIndex==2&&knob==1)||(pageIndex>=3&&knob==7))return juce::String(raw-64);
        if(pageIndex==0&&knob==1&&raw==127)return "INF";
        return juce::String(raw);
    }
    void paint(juce::Graphics& g)override{
        if(pickMode || flashTicks > 0){g.setColour(flashTicks > 0 ? ink : ink.withAlpha(0.28f));g.drawRect(getLocalBounds().reduced(2),flashTicks > 0 ? 3 : 2);g.drawLine(getWidth()/2-8,getHeight()/2,getWidth()/2+8,getHeight()/2,1);g.drawLine(getWidth()/2,getHeight()/2-8,getWidth()/2,getHeight()/2+8,1);}
        g.setColour(parameter?ink:dim);pixel::text(g,label,{3,3,getWidth()-6,23},21,true);
        if(isChoice){int cx=getWidth()/2,cy=49;
            if(pageIndex>=3&&knob==3){int type=juce::roundToInt(parameter->convertFrom0to1(parameter->getValue()));
                int lastY=cy;
                for(int i=0;i<24;++i){float t=i/23.0f;float v=0;
                    if(type==10){const float held[]{0.6f,-0.3f,0.85f,-0.7f};v=held[std::min(3,i/6)];}
                    else v=nova::lfoShape(type,t);
                    int xx=cx-23+i*2,yy=cy-juce::roundToInt(v*13);g.fillRect(xx,yy,3,3);
                    if(i>0)g.fillRect(xx,std::min(yy,lastY),2,std::max(2,std::abs(yy-lastY)));lastY=yy;}
            }else if(pageIndex>=3&&(knob==2||knob==4)){
                for(int i=0;i<16;++i){float a=2*nova::pi*i/16;g.fillRect(cx+juce::roundToInt(std::sin(a)*14)-1,cy+juce::roundToInt(std::cos(a)*14)-1,3,3);}
                float a=3.9f+parameter->getValue()*4.7f;for(int i=3;i<12;++i)g.fillRect(cx+juce::roundToInt(std::sin(a)*i)-1,cy-juce::roundToInt(std::cos(a)*i)-1,3,3);
            }else {g.drawRect(cx-17,cy-16,34,30,3);g.fillRect(cx-11,cy-10,5,8);g.fillRect(cx-4,cy-8,15,3);g.fillRect(cx-4,cy-1,15,3);g.fillRect(cx-4,cy+6,15,3);}
        }
        pixel::text(g,displayedValue(),{2,getHeight()-26,getWidth()-4,23},21,true);
    }
    void resized()override{slider.setBounds(getWidth()/2-26,27,52,43);}
    void mouseWheelMove(const juce::MouseEvent& e,const juce::MouseWheelDetails& w)override{if(parameter&&isChoice&&std::abs(w.deltaY)>0.0001f){parameter->beginChangeGesture();stepBy(*parameter,w.deltaY>0?1:-1);parameter->endChangeGesture();repaint();}else juce::Component::mouseWheelMove(e,w);}
    void mouseEnter(const juce::MouseEvent&) override { if(pickMode && pickHover) pickHover(modulationTarget()); }

    // Press-drag is the primary way to swap lists (8 px per entry); a plain click
    // still opens the popup list as a bonus.
    void mouseDown(const juce::MouseEvent&e)override{
        if(pickMode&&parameter){if(pickTarget)pickTarget(modulationTarget());return;}
        if(pageIndex==0&&(knob==0||knob==2||knob==3)&&e.mods.isPopupMenu()){popupHandled=true;if(openEnvelope)openEnvelope(knob);return;}
        if(machine==7&&knob==2&&e.mods.isPopupMenu()){popupHandled=true;if(openSamples)openSamples();return;}
        if(!parameter)return;
        if(isChoice){dragOriginY=static_cast<int>(e.getScreenPosition().y);dragBaseIndex=juce::roundToInt(parameter->convertFrom0to1(parameter->getValue()));dragMoved=false;gestureOpen=true;parameter->beginChangeGesture();}
    }
    void mouseDrag(const juce::MouseEvent&e)override{
        if(!parameter||!isChoice)return;
        const int dy=dragOriginY-static_cast<int>(e.getScreenPosition().y);
        if(std::abs(dy)>3)dragMoved=true;
        const int value=std::clamp(dragBaseIndex+dy/8,0,choiceCount(parameter)-1);
        parameter->setValueNotifyingHost(hostFor(*parameter,value));
    }
    void mouseUp(const juce::MouseEvent&)override{
        if(popupHandled){popupHandled=false;return;}
        if(gestureOpen){gestureOpen=false;parameter->endChangeGesture();}
        if(!parameter||!isChoice||dragMoved){dragMoved=false;return;}
        juce::PopupMenu menu;const int n=choiceCount(parameter);
        const auto names=parameter->getAllValueStrings();
        for(int i=0;i<n;++i)menu.addItem(i+1,choiceName(i),true,i==juce::roundToInt(parameter->convertFrom0to1(parameter->getValue())));
        const juce::Component::SafePointer<Cell> safe(this);
        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(this),[safe](int result){if(safe&&result>0&&safe->parameter){safe->parameter->beginChangeGesture();safe->parameter->setValueNotifyingHost(hostFor(*safe->parameter,result-1));safe->parameter->endChangeGesture();}});
    }
private:
    uint8_t modulationTarget() const noexcept {
        if (machine >= 0 && pageIndex < 0) return static_cast<uint8_t>(juce::jlimit(0, 7, knob));
        if (pageIndex >= 0) return static_cast<uint8_t>(juce::jlimit(0, 55, pageIndex * 8 + knob + (pageIndex >= 0 ? 8 : 0)));
        return 0;
    }
    void timerCallback()override{if(flashTicks>0)--flashTicks;repaint();}
    MonomachineNovaAudioProcessor& processor;juce::String id,label;int pageIndex=-1,knob=-1,machine=-1,flashTicks=0;bool isChoice=false;
    int dragOriginY=0,dragBaseIndex=0;bool dragMoved=false,gestureOpen=false,popupHandled=false,pickMode=false;
    juce::RangedAudioParameter* parameter=nullptr;CurveSlider slider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;
};

class DragChoice final : public juce::ComboBox {
public:
    DragChoice(){setScrollWheelEnabled(true);}
    void mouseDown(const juce::MouseEvent& e)override{origin=e.getPosition();initial=getSelectedItemIndex();dragged=false;}
    void mouseDrag(const juce::MouseEvent& e)override{int d=(e.x-origin.x)+(origin.y-e.y);if(std::abs(d)>4){dragged=true;setSelectedItemIndex(juce::jlimit(0,std::max(0,getNumItems()-1),initial+d/6),juce::sendNotificationSync);}}
    void mouseUp(const juce::MouseEvent&)override{if(!dragged)showPopup();}
private:juce::Point<int> origin;int initial=0;bool dragged=false;
};
// Global/tempo/arp list kept for the MENU page, restyled for the black screen.
class SettingsPage final : public juce::Component {
public:
    SettingsPage(MonomachineNovaAudioProcessor& p,bool matrix):processor(p){
        if(matrix)return; // the matrix has its own page below
        std::vector<juce::String> ids={"arp_on","arp_hold","host_sync","arp_range","arp_sync","arp_play","arp_grid","arp_length","arp_wrap","arp_velocity_mode","arp_step","arp_step_velocity","arp_step_transpose","arp_step_hold","arp_time","bpm","mseg_sync","mseg_loop","mseg_rate","macro_x","macro_y"};
        if(!p.isSynthVersion){ids.push_back("fx_mix");ids.push_back("gate");}
        for(const auto& id:ids){auto* parameter=p.parameters.getParameter(id);if(!parameter)continue;
            auto row=std::make_unique<Row>();row->label.setText(parameter->getName(128),juce::dontSendNotification);row->label.setColour(juce::Label::textColourId,ink);
            addAndMakeVisible(row->label);auto choices=parameter->getAllValueStrings();
            if(id=="arp_on"||id=="arp_hold"||id=="host_sync"||id=="mseg_sync"||id=="mseg_loop"||id=="arp_step_hold"){addAndMakeVisible(row->toggle);row->buttonLink=std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(p.parameters,id,row->toggle);}
            else if(!choices.isEmpty()){row->combo.addItemList(choices,1);addAndMakeVisible(row->combo);row->comboLink=std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(p.parameters,id,row->combo);}
            else {row->slider.setSliderStyle(juce::Slider::LinearHorizontal);row->slider.setTextBoxStyle(juce::Slider::TextBoxRight,false,70,24);
                row->slider.setColour(juce::Slider::trackColourId,ink);row->slider.setColour(juce::Slider::thumbColourId,ink);row->slider.setColour(juce::Slider::rotarySliderFillColourId,ink);addAndMakeVisible(row->slider);
                row->sliderLink=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters,id,row->slider);}
            rows.push_back(std::move(row));}
        setSize(1120,static_cast<int>((rows.size()+1)/2)*70+55);
    }
    void paint(juce::Graphics& g)override{g.fillAll(juce::Colours::black);g.setColour(ink);pixel::text(g,"TEMPO / ARPEGGIATOR / GLOBAL",{10,4,700,22},18);}
    void resized()override{for(size_t i=0;i<rows.size();++i){int x=static_cast<int>(i%2)*550+10,y=static_cast<int>(i/2)*70+32;rows[i]->label.setBounds(x,y,515,25);rows[i]->combo.setBounds(x,y+26,500,27);rows[i]->slider.setBounds(x,y+26,500,27);rows[i]->toggle.setBounds(x,y+26,30,27);}}
private:
    struct Row{juce::ToggleButton toggle;std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> buttonLink;juce::Label label;DragChoice combo;juce::Slider slider;std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> comboLink;std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderLink;};
    MonomachineNovaAudioProcessor& processor;std::vector<std::unique_ptr<Row>> rows;
};


// Dedicated ARP page. Rate is one compact control row: clock mode and the
// active time/grid value sit together, while the 8-step bank is always visible.
class ArpPage final : public juce::Component, private juce::Timer {
public:
    explicit ArpPage(MonomachineNovaAudioProcessor& p) : processor(p) {
        addToggle(arpEnable, "ARP ENABLE", "arp_on");
        addToggle(arpHold, "ARP HOLD", "arp_hold");
        addToggle(hostTempo, "HOST TEMPO", "host_sync");
        addAndMakeVisible(clock); clock.addItemList(processor.parameters.getParameter("arp_sync")->getAllValueStrings(), 1); clockLink = std::make_unique<ComboAttachment>(processor.parameters, "arp_sync", clock);
        addAndMakeVisible(mode); mode.addItemList(processor.parameters.getParameter("arp_play")->getAllValueStrings(), 1); modeLink = std::make_unique<ComboAttachment>(processor.parameters, "arp_play", mode);
        addAndMakeVisible(rateGrid); rateGrid.addItemList(processor.parameters.getParameter("arp_grid")->getAllValueStrings(), 1); rateGridLink = std::make_unique<ComboAttachment>(processor.parameters, "arp_grid", rateGrid);
        addAndMakeVisible(timeRate); timeRate.setSliderStyle(juce::Slider::LinearHorizontal); timeRate.setTextBoxStyle(juce::Slider::TextBoxRight, false, 72, 24); timeRate.setRange(5.0, 2000.0, 0.1); timeRateLink = std::make_unique<SliderAttachment>(processor.parameters, "arp_time", timeRate);
        addAndMakeVisible(octaves); octaves.setSliderStyle(juce::Slider::LinearHorizontal); octaves.setTextBoxStyle(juce::Slider::TextBoxRight, false, 54, 24); octaves.setRange(1, 4, 1); octavesLink = std::make_unique<SliderAttachment>(processor.parameters, "arp_range", octaves);
        addAndMakeVisible(gate); gate.setSliderStyle(juce::Slider::LinearHorizontal); gate.setTextBoxStyle(juce::Slider::TextBoxRight, false, 54, 24); gate.setRange(1, 127, 1); gateLink = std::make_unique<SliderAttachment>(processor.parameters, "arp_length", gate);
        addAndMakeVisible(wrap); wrap.setSliderStyle(juce::Slider::LinearHorizontal); wrap.setTextBoxStyle(juce::Slider::TextBoxRight, false, 54, 24); wrap.setRange(1, 16, 1); wrapLink = std::make_unique<SliderAttachment>(processor.parameters, "arp_wrap", wrap);
        addAndMakeVisible(velocityMode); velocityMode.addItemList(processor.parameters.getParameter("arp_velocity_mode")->getAllValueStrings(), 1); velocityModeLink = std::make_unique<ComboAttachment>(processor.parameters, "arp_velocity_mode", velocityMode);
        addAndMakeVisible(page); for (int i = 0; i < 16; ++i) page.addItem("PAGE " + juce::String(i + 1), i + 1); page.onChange = [this] { if (auto* p = processor.parameters.getParameter("arp_step_page")) { p->beginChangeGesture(); p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(page.getSelectedItemIndex()))); p->endChangeGesture(); } rebindSteps(); };
        addAndMakeVisible(pageLimit); pageLimit.setSliderStyle(juce::Slider::LinearHorizontal); pageLimit.setTextBoxStyle(juce::Slider::TextBoxRight, false, 54, 24); pageLimit.setRange(1, 16, 1); pageLimitLink = std::make_unique<SliderAttachment>(processor.parameters, "arp_step_page_limit", pageLimit);
        addAndMakeVisible(randomButton); randomButton.setButtonText("RANDOM"); randomButton.onClick = [this] { showRandomMenu(); };
        addAndMakeVisible(random); random.setSliderStyle(juce::Slider::LinearHorizontal); random.setTextBoxStyle(juce::Slider::TextBoxRight, false, 54, 24); random.setRange(0, 100, 1); randomLink = std::make_unique<SliderAttachment>(processor.parameters, "arp_step_random", random);
        for (int i = 0; i < 8; ++i) { addAndMakeVisible(stepRandom[static_cast<size_t>(i)]); stepRandom[static_cast<size_t>(i)].setButtonText("RND"); stepRandom[static_cast<size_t>(i)].onClick = [this, i] { selectedStep = i; randomizeStep(page.getSelectedItemIndex(), i); }; addAndMakeVisible(stepHold[static_cast<size_t>(i)]); stepHold[static_cast<size_t>(i)].setButtonText(""); stepHold[static_cast<size_t>(i)].setClickingTogglesState(true); addAndMakeVisible(stepTranspose[static_cast<size_t>(i)]); addAndMakeVisible(stepVelocity[static_cast<size_t>(i)]); stepTranspose[static_cast<size_t>(i)].setSliderStyle(juce::Slider::LinearHorizontal); stepTranspose[static_cast<size_t>(i)].setTextBoxStyle(juce::Slider::TextBoxRight, false, 48, 22); stepTranspose[static_cast<size_t>(i)].setRange(-24, 24, 1); stepVelocity[static_cast<size_t>(i)].setSliderStyle(juce::Slider::LinearHorizontal); stepVelocity[static_cast<size_t>(i)].setTextBoxStyle(juce::Slider::TextBoxRight, false, 48, 22); stepVelocity[static_cast<size_t>(i)].setRange(0, 127, 1); }
        rebindSteps(); setSize(1160, 760); startTimerHz(15);
    }
    ~ArpPage() override { stopTimer(); }

    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::black); g.setColour(ink);
        pixel::text(g, "ARP / STEP SEQUENCER", {12, 4, 600, 26}, 21);
        pixel::text(g, "ONE RATE CONTROL / 16 STEP PAGES / VELOCITY AND HOLD", {625, 8, 520, 18}, 12, false);
        g.setColour(ink.withAlpha(0.35f)); g.drawRect(10, 38, 1140, 108, 1); g.drawRect(10, 158, 1140, 116, 1); g.drawRect(10, 286, 1140, 390, 1);
        pixel::text(g, "PLAY", {24, 48, 80, 20}, 14, true); pixel::text(g, "MODE", {328, 48, 80, 20}, 14, true); pixel::text(g, "RATE", {518, 48, 80, 20}, 14, true); pixel::text(g, "OCT", {24, 166, 80, 20}, 14, true); pixel::text(g, "GATE", {210, 166, 80, 20}, 14, true); pixel::text(g, "WRAP", {396, 166, 80, 20}, 14, true); pixel::text(g, "VELOCITY", {582, 166, 130, 20}, 14, true);
        pixel::text(g, "STEP BANK", {24, 294, 100, 20}, 15, true); pixel::text(g, "RND", {118, 294, 54, 20}, 14, true); pixel::text(g, "HOLD", {190, 294, 100, 20}, 14, true); pixel::text(g, "TRANSPOSE", {315, 294, 250, 20}, 14, true); pixel::text(g, "VELOCITY", {665, 294, 250, 20}, 14, true);
        for (int i = 0; i < 8; ++i) { const int y = 332 + i * 42; g.setColour(ink.withAlpha(0.18f)); g.drawHorizontalLine(y + 33, 20, 1135); pixel::text(g, juce::String(i + 1).paddedLeft('0', 2), {28, y, 50, 26}, 16, true); pixel::text(g, i == 0 ? "FIRST" : i == 7 ? "LAST" : "", {82, y, 90, 26}, 11, false); }
        pixel::text(g, "PAGE LIMIT", {760, 178, 120, 18}, 12, true); pixel::text(g, "RANDOM ORDER", {945, 178, 170, 18}, 12, true);
        pixel::text(g, "HOLD = EXTEND NOTE / VELOCITY 0 = NO NOTE / TRANSPOSE -24..+24", {24, 695, 780, 20}, 13, false);
    }
    void resized() override {
        arpEnable.setBounds(20, 76, 135, 28); arpHold.setBounds(170, 76, 125, 28); hostTempo.setBounds(20, 110, 145, 28); clock.setBounds(180, 110, 120, 28); mode.setBounds(328, 76, 170, 28);
        rateGrid.setBounds(518, 76, 120, 28); timeRate.setBounds(650, 76, 250, 28);
        octaves.setBounds(24, 195, 160, 28); gate.setBounds(210, 195, 160, 28); wrap.setBounds(396, 195, 160, 28); velocityMode.setBounds(582, 195, 160, 28);
        page.setBounds(24, 238, 180, 28); pageLimit.setBounds(760, 202, 150, 28); random.setBounds(945, 202, 170, 28); randomButton.setBounds(1000, 290, 135, 28);
        for (int i = 0; i < 8; ++i) { const int y = 327 + i * 42; stepRandom[static_cast<size_t>(i)].setBounds(115, y + 2, 58, 26); stepHold[static_cast<size_t>(i)].setBounds(220, y + 2, 26, 26); stepTranspose[static_cast<size_t>(i)].setBounds(315, y, 310, 28); stepVelocity[static_cast<size_t>(i)].setBounds(665, y, 310, 28); }
    }
private:
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    void addToggle(juce::ToggleButton& button, const juce::String& text, const juce::String& id) { button.setButtonText(text); button.setClickingTogglesState(true); addAndMakeVisible(button); toggleLinks.push_back(std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processor.parameters, id, button)); }
    void setStepParameter(int pg, int st, const char* field, float value) { const auto id = "arp_s" + juce::String(pg) + "_" + juce::String(st) + "_" + field; if (auto* p = processor.parameters.getParameter(id)) { p->beginChangeGesture(); p->setValueNotifyingHost(p->convertTo0to1(value)); p->endChangeGesture(); } }
    void randomizeStep(int pg, int st) { pg = juce::jlimit(0, 15, pg); st = juce::jlimit(0, 7, st); auto& r = juce::Random::getSystemRandom(); setStepParameter(pg, st, "hold", r.nextInt(2)); setStepParameter(pg, st, "transpose", r.nextInt(49) - 24); setStepParameter(pg, st, "velocity", r.nextInt(128)); repaint(); }
    void showRandomMenu() { juce::PopupMenu menu; menu.addItem(1, "RANDOM THIS PAGE (8 STEPS)"); menu.addItem(2, "RANDOM ALL PAGES (128 STEPS)"); menu.addItem(3, "RANDOM SELECTED STEP"); const juce::Component::SafePointer<ArpPage> safe(this); menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(&randomButton), [safe](int result) { if (!safe || result <= 0) return; const int pg = safe->page.getSelectedItemIndex(); if (result == 1) { for (int i = 0; i < 8; ++i) safe->randomizeStep(pg, i); } else if (result == 2) { for (int allPg = 0; allPg < 16; ++allPg) for (int st = 0; st < 8; ++st) safe->randomizeStep(allPg, st); } else safe->randomizeStep(pg, safe->selectedStep); }); }
    void rebindSteps() { const int pg = juce::jlimit(0, 15, page.getSelectedItemIndex()); for (auto& x : stepHoldLinks) x.reset(); for (auto& x : stepTransposeLinks) x.reset(); for (auto& x : stepVelocityLinks) x.reset(); for (int i = 0; i < 8; ++i) { const auto prefix = "arp_s" + juce::String(pg) + "_" + juce::String(i) + "_"; stepHoldLinks[static_cast<size_t>(i)] = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processor.parameters, prefix + "hold", stepHold[static_cast<size_t>(i)]); stepTransposeLinks[static_cast<size_t>(i)] = std::make_unique<SliderAttachment>(processor.parameters, prefix + "transpose", stepTranspose[static_cast<size_t>(i)]); stepVelocityLinks[static_cast<size_t>(i)] = std::make_unique<SliderAttachment>(processor.parameters, prefix + "velocity", stepVelocity[static_cast<size_t>(i)]); } repaint(); }
    void timerCallback() override { const bool syncMode = clock.getSelectedId() == 2; rateGrid.setVisible(syncMode); timeRate.setVisible(!syncMode); if (auto* p = processor.parameters.getRawParameterValue("arp_step_page")) { const int wanted = juce::jlimit(1, 16, juce::roundToInt(p->load()) + 1); if (page.getSelectedId() != wanted) { page.setSelectedId(wanted, juce::dontSendNotification); rebindSteps(); } } repaint(); }
    MonomachineNovaAudioProcessor& processor; juce::ToggleButton arpEnable, arpHold, hostTempo; juce::TextButton randomButton; juce::ComboBox clock, mode, rateGrid, velocityMode, page; int selectedStep = 0; juce::Slider timeRate, octaves, gate, wrap, pageLimit, random; std::array<juce::TextButton, 8> stepRandom; std::array<juce::ToggleButton, 8> stepHold; std::array<juce::Slider, 8> stepTranspose, stepVelocity;
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>> toggleLinks; std::unique_ptr<ComboAttachment> clockLink, modeLink, rateGridLink, velocityModeLink; std::unique_ptr<SliderAttachment> timeRateLink, octavesLink, gateLink, wrapLink, pageLimitLink, randomLink; std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>, 8> stepHoldLinks; std::array<std::unique_ptr<SliderAttachment>, 8> stepTransposeLinks, stepVelocityLinks;
};

// The modulation matrix as a real screen page (8 routes x on/src/dest/depth),
// drawn in the same pixel language; cells change by drag, popup on click.
class MatrixPage final : public juce::Component, private juce::Timer {
public:
    explicit MatrixPage(MonomachineNovaAudioProcessor& p) : processor(p) {
        const char* fields[]{"on", "src", "dest", "depth", "mode"};
        for (int r = 0; r < 16; ++r) for (int c = 0; c < 5; ++c) cells[static_cast<size_t>(r * 5 + c)] = p.parameters.getParameter("r" + juce::String(r) + "_" + fields[c]);
        setSize(1145, 650); startTimerHz(15);
    }
    ~MatrixPage() override { stopTimer(); }
    std::function<void(int)> onTargetPick;
    static int percent(int raw) { return juce::jlimit(0, 100, juce::roundToInt(std::abs(raw) * 100.0f / (raw < 0 ? 64.0f : 63.0f))); }
    static juce::String signedPercent(int raw) { return raw == 0 ? "0%" : juce::String(raw < 0 ? "-" : "+") + juce::String(percent(raw)) + "%"; }

    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::black); g.setColour(ink);
        pixel::text(g, "MODULATION MATRIX", {10, 3, 400, 24}, 20);
        pixel::text(g, "CLICK HEADER TO SORT / DRAG MSEG OUTPUT TO TARGET", {470, 7, 650, 17}, 11, false);
        const char* headers[]{"", "", "SOURCE", "DESTINATION", "DEPTH", "MODE"};
        for (int c = 0; c < 6; ++c) pixel::text(g, headers[c], {colX(c), 30, colWidth(c), 19}, 13, true);
        if (sortColumn >= 2 && sortColumn <= 5) { g.setColour(ink.withAlpha(0.5f)); pixel::text(g, sortDescending ? "▼" : "▲", {colX(sortColumn) + colWidth(sortColumn) - 19, 30, 16, 18}, 12, true); }
        for (int r = 0; r < 16; ++r) {
            const int y = rowY(r); g.setColour(ink.withAlpha(0.18f)); g.drawHorizontalLine(y + 31, 8, 1128);
            for (int c = 1; c <= 5; ++c) {
                auto* par = cell(r, c); if (!par) continue; const int value = juce::roundToInt(par->convertFrom0to1(par->getValue()));
                if (c == 1) { drawCheck(g, colX(c) + 10, y + 7, value > 0); continue; }
                juce::String text = c == 2 ? sourceName(value) : c == 3 ? targetName(processor, juce::jlimit(0, 55, value)) : c == 4 ? signedPercent(value) : polarityName(value);
                pixel::text(g, text, {colX(c) + 5, y + 2, colWidth(c) - (c == 3 ? 32 : 8), 25}, c == 3 ? 14 : 16, c != 3);
                if (c == 3) drawCrosshair(g, colX(c) + colWidth(c) - 18, y + 15);
            }
            g.setColour(ink.withAlpha(0.22f)); for (int c = 1; c <= 5; ++c) g.drawVerticalLine(colX(c) - 4, y, y + 31);
        }
        g.setColour(ink.withAlpha(0.35f)); g.drawRect(8, 26, 1120, 16 * 34 + 36, 1);
        g.setColour(dim); pixel::text(g, "DEPTH 0..100%   UNIPOLAR = ONE SIDE   BIPOLAR = TWO SIDES   GRAB EMPTY LEFT EDGE TO REORDER", {10, getHeight() - 24, 1115, 17}, 11, false);
    }

    void mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& w) override { const int r = rowAt(e.y), c = colAt(e.x); if (r < 0 || c < 1 || std::abs(w.deltaY) < 0.0001f) return; auto* p = cell(r, c); if (!p) return; const int old = juce::roundToInt(p->convertFrom0to1(p->getValue())); const int lo = c == 4 ? -64 : 0, hi = c == 4 ? 63 : (c == 5 ? 1 : choiceCount(p) - 1); p->beginChangeGesture(); p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(juce::jlimit(lo, hi, old + (w.deltaY > 0 ? 1 : -1))))); p->endChangeGesture(); repaint(); }
    void mouseDown(const juce::MouseEvent& e) override {
        if (e.y < 56) { const int c = colAt(e.x); if (c >= 2 && c <= 5) sortBy(c); return; }
        const int r = rowAt(e.y), c = colAt(e.x); if (r < 0) return;
        if (c == 0) { dragRow = r; dragTarget = r; return; }
        if (c == 1) { if (auto* p = cell(r, 1)) toggle(*p); return; }
        if (c == 3 && e.x >= colX(c) + colWidth(c) - 28) { if (onTargetPick) onTargetPick(r); return; }
        hitRow = r; hitCol = c; hitY = static_cast<int>(e.getScreenPosition().y); hitBase = cell(r, c) ? juce::roundToInt(cell(r, c)->convertFrom0to1(cell(r, c)->getValue())) : 0; dragged = false; if (auto* p = cell(r, c)) { gesture = true; p->beginChangeGesture(); }
    }
    void mouseDrag(const juce::MouseEvent& e) override {
        if (dragRow >= 0) { dragTarget = juce::jlimit(0, 15, rowAt(e.y)); repaint(); return; }
        if (hitRow < 0) return; auto* p = cell(hitRow, hitCol); if (!p) return; const int dy = hitY - static_cast<int>(e.getScreenPosition().y); if (std::abs(dy) > 2) dragged = true;
        if (hitCol == 4) { const int amount = e.mods.isShiftDown() ? dy / 10 : dy; p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(juce::jlimit(-64, 63, hitBase + amount)))); }
        else { const int hi = hitCol == 5 ? 1 : choiceCount(p) - 1; p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(juce::jlimit(0, std::max(0, hi), hitBase + dy / 8)))); }
        repaint();
    }
    void mouseUp(const juce::MouseEvent& e) override {
        if (dragRow >= 0) { if (dragTarget >= 0 && dragTarget != dragRow) processor.moveModRoute(dragRow, dragTarget); dragRow = dragTarget = -1; repaint(); return; }
        if (gesture && cell(hitRow, hitCol)) { cell(hitRow, hitCol)->endChangeGesture(); gesture = false; }
        const bool wasDrag = dragged; const int r = hitRow, c = hitCol; hitRow = hitCol = -1; dragged = false; if (wasDrag || r < 0 || c < 2) return;
        auto* par = cell(r, c); if (!par) return; juce::PopupMenu menu;
        if (c == 4) { const int values[]{0, 16, 32, 48, 63, -16, -32, -48, -64}; for (int i = 0; i < 9; ++i) menu.addItem(i + 1, signedPercent(values[i])); }
        else { const int n = c == 5 ? 2 : choiceCount(par); const int selected = juce::roundToInt(par->convertFrom0to1(par->getValue())); const auto names = par->getAllValueStrings(); for (int i = 0; i < n; ++i) menu.addItem(i + 1, c == 2 ? sourceName(i) : c == 3 ? targetName(processor, i) : (names.size() > i ? names[i] : polarityName(i)), true, i == selected); }
        const auto screen = e.getScreenPosition(); const juce::Component::SafePointer<MatrixPage> safe(this);
        menu.showMenuAsync(juce::PopupMenu::Options().withTargetScreenArea(juce::Rectangle<int>(screen.x, screen.y, 1, 1)), [safe, r, c](int result) { if (!safe || result <= 0) return; auto* p = safe->cell(r, c); if (!p) return; if (c == 4) { const int values[]{0, 16, 32, 48, 63, -16, -32, -48, -64}; p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(values[result - 1]))); } else p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(result - 1))); });
    }
private:
    static int choiceCount(juce::RangedAudioParameter* p) { if (!p) return 0; const auto n = p->getAllValueStrings(); return n.size() > 0 ? n.size() : juce::roundToInt(p->convertFrom0to1(1.0f)) + 1; }
    static juce::String sourceName(int v) { const char* n[]{"KEY", "VEL", "MACRO X", "MACRO Y", "LFO1", "LFO2", "LFO3", "PITCH WHL", "MOD WHL", "AFTERTOUCH", "MSEG", "STEP VEL", "STEP TRANS", "ARP GATE", "ARP RATE", "RANDOM"}; return n[juce::jlimit(0, 15, v)]; }
    static juce::String polarityName(int v) { return v == 1 ? "BIPOLAR" : "UNIPOLAR"; }
    static void drawCheck(juce::Graphics& g, int x, int y, bool checked) { g.setColour(ink); g.drawRect(x, y, 18, 18, 1); if (checked) { g.drawLine(x + 4, y + 9, x + 8, y + 14, 2); g.drawLine(x + 8, y + 14, x + 15, y + 4, 2); } }
    static void drawCrosshair(juce::Graphics& g, int x, int y) { g.setColour(ink); g.drawEllipse(x - 6, y - 6, 12, 12, 1); g.drawLine(x - 9, y, x + 9, y, 1); g.drawLine(x, y - 9, x, y + 9, 1); }
    int colX(int c) const { const int x[]{8, 28, 78, 245, 580, 735}; return x[juce::jlimit(0, 5, c)]; }
    int colWidth(int c) const { const int w[]{18, 42, 165, 330, 150, 190}; return w[juce::jlimit(0, 5, c)]; }
    int rowY(int r) const { return 58 + r * 34; }
    int colAt(int x) const { for (int c = 0; c < 6; ++c) if (x >= colX(c) && x < colX(c) + colWidth(c)) return c; return -1; }
    int rowAt(int y) const { if (y < 58) return -1; const int r = (y - 58) / 34; return r >= 0 && r < 16 ? r : -1; }
    juce::RangedAudioParameter* cell(int r, int c) { return cells[static_cast<size_t>(r * 5 + (c - 1))]; }
    void toggle(juce::RangedAudioParameter& p) { p.beginChangeGesture(); p.setValueNotifyingHost(p.convertTo0to1(p.convertFrom0to1(p.getValue()) > 0.5f ? 0.0f : 1.0f)); p.endChangeGesture(); }
    void sortBy(int c) { if (sortColumn == c) sortDescending = !sortDescending; else { sortColumn = c; sortDescending = c == 4; } processor.sortModRoutes(c, sortDescending); repaint(); }
    void timerCallback() override { repaint(); }
    MonomachineNovaAudioProcessor& processor; std::array<juce::RangedAudioParameter*, 80> cells{}; int hitRow = -1, hitCol = -1, hitY = 0, hitBase = 0, dragRow = -1, dragTarget = -1, sortColumn = -1; bool gesture = false, dragged = false, sortDescending = false;
};

class MsegPage final : public juce::Component, public juce::DragAndDropTarget, private juce::Timer {
public:
    explicit MsegPage(MonomachineNovaAudioProcessor& p) : processor(p) {
        loop.setButtonText("LOOP"); sync.setButtonText("SYNC");
        loopLink = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(p.parameters, "mseg_loop", loop);
        syncLink = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(p.parameters, "mseg_sync", sync);
        addAndMakeVisible(loop); addAndMakeVisible(sync);
        rate.setSliderStyle(juce::Slider::LinearHorizontal); rate.setTextBoxStyle(juce::Slider::TextBoxRight, false, 70, 22); rate.setRange(0.05, 20.0, 0.01);
        rateLink = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters, "mseg_rate", rate); addAndMakeVisible(rate);
        reset.setButtonText("RESET SHAPE"); reset.onClick = [this] { processor.resetMseg(); }; addAndMakeVisible(reset);
        setSize(1170, 820); startTimerHz(20);
    }
    ~MsegPage() override { stopTimer(); }

    bool isInterestedInDragSource(const SourceDetails& details) override { return details.description.toString() == "MSEG"; }
    void itemDropped(const SourceDetails& details) override {
        if (details.description.toString() != "MSEG") return;
        const auto at = details.localPosition;
        if (at.x < 780 || at.y < 64) return;
        const int col = at.x >= 970 ? 1 : 0, row = (static_cast<int>(at.y) - 64) / 22;
        if (row >= 0 && row < 28) { processor.addMsegRoute(static_cast<uint8_t>(col * 28 + row)); flash = col * 28 + row; repaint(); }
    }
    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::black); g.setColour(ink);
        pixel::text(g, "MSEG / CURVE EDITOR", {12, 4, 600, 28}, 22);
        pixel::text(g, juce::String(processor.msegPointCount()) + " POINTS", {620, 9, 140, 20}, 14, false);
        g.setColour(ink.withAlpha(0.22f)); g.drawRect(graphX, graphY, graphW, graphH, 1);
        for (int i = 1; i < 4; ++i) g.drawHorizontalLine(graphY + graphH * i / 4, graphX, graphX + graphW);
        for (int i = 1; i < 8; ++i) g.drawVerticalLine(graphX + graphW * i / 8, graphY, graphY + graphH);
        juce::Path path;
        for (int i = 0; i < processor.msegPointCount(); ++i) { const float px = pointToX(processor.msegPointX(i)), py = pointToY(processor.msegPointY(i)); if (i == 0) path.startNewSubPath(px, py); else path.lineTo(px, py); }
        g.setColour(ink); g.strokePath(path, juce::PathStrokeType(2.0f));
        for (int i = 0; i < processor.msegPointCount(); ++i) { g.setColour(i == selected ? juce::Colours::white : juce::Colours::black); g.fillEllipse(pointToX(processor.msegPointX(i)) - 6.0f, pointToY(processor.msegPointY(i)) - 6.0f, 12.0f, 12.0f); g.setColour(ink); g.drawEllipse(pointToX(processor.msegPointX(i)) - 6.0f, pointToY(processor.msegPointY(i)) - 6.0f, 12.0f, 12.0f, 2.0f); }
        pixel::text(g, "LEFT CLICK: ADD OR GRAB   RIGHT CLICK: ERASE   HOLD RIGHT: ERASE CONTINUOUSLY   DRAG: EDIT", {20, 358, 720, 20}, 13, false);
        pixel::text(g, "MSEG ROUTE TARGETS / DROP OUTPUT HERE", {790, 40, 360, 20}, 14, true);
        const auto& names = targetNames();
        for (int i = 0; i < 56; ++i) { const int x = i < 28 ? 790 : 970, y = 64 + (i % 28) * 22; const bool active = hasMsegRoute(i); g.setColour(active ? ink.withAlpha(0.35f) : juce::Colours::black); g.fillRect(x, y, 178, 20); g.setColour(active ? ink : dim); pixel::text(g, active ? "[MSEG] " + names[i] : names[i], {x + 4, y, 170, 20}, 12, false); }
        if (flash >= 0) { g.setColour(ink); g.drawRect(flash < 28 ? 790 : 970, 64 + (flash % 28) * 22, 178, 20, 1); }
    }
    void resized() override { loop.setBounds(20, 385, 92, 27); sync.setBounds(120, 385, 92, 27); rate.setBounds(225, 385, 360, 27); reset.setBounds(600, 385, 150, 27); }
    void mouseDown(const juce::MouseEvent& e) override {
        if (e.x < graphX || e.x > graphX + graphW || e.y < graphY || e.y > graphY + graphH) return;
        if (e.mods.isPopupMenu()) { erasing = true; eraseAt(e.position); return; }
        selected = nearestPoint(e.position);
        if (selected < 0) { processor.addMsegPoint(toX(e.x), toY(e.y)); selected = nearestPoint(e.position); }
        draggingPoint = selected >= 0; repaint();
    }
    void mouseDrag(const juce::MouseEvent& e) override {
        if (erasing) { eraseAt(e.position); return; }
        if (!draggingPoint || selected < 0) return;
        const float x = toX(e.x), y = toY(e.y); float lo = selected == 0 ? 0.0f : processor.msegPointX(selected - 1) + 0.005f; float hi = selected == processor.msegPointCount() - 1 ? 1.0f : processor.msegPointX(selected + 1) - 0.005f; processor.setMsegPoint(selected, juce::jlimit(lo, hi, x), y); repaint();
    }
    void mouseUp(const juce::MouseEvent&) override { draggingPoint = false; erasing = false; }
private:
    static const std::array<juce::String, 56>& targetNames() { static const auto names = [] { std::array<juce::String, 56> a{}; for (int i = 0; i < 56; ++i) a[static_cast<size_t>(i)] = juce::String(monomachine::ModulationMatrix::getDestinationName(static_cast<uint8_t>(i))); return a; }(); return names; }
    bool hasMsegRoute(int target) const { for (int r = 0; r < 16; ++r) { auto* src = processor.parameters.getRawParameterValue("r" + juce::String(r) + "_src"); auto* dest = processor.parameters.getRawParameterValue("r" + juce::String(r) + "_dest"); auto* on = processor.parameters.getRawParameterValue("r" + juce::String(r) + "_on"); if (src && dest && on && on->load() > 0.5f && juce::roundToInt(src->load()) == static_cast<int>(monomachine::ModSource::MSEG) && juce::roundToInt(dest->load()) == target) return true; } return false; }
    void eraseAt(juce::Point<float> p) { const int point = nearestPoint(p); if (point > 0 && point < processor.msegPointCount() - 1) { processor.removeMsegPoint(point); selected = -1; repaint(); } }
    int nearestPoint(juce::Point<float> p) const { int best = -1; float distance = 15.0f; for (int i = 0; i < processor.msegPointCount(); ++i) { const float d = p.getDistanceFrom({pointToX(processor.msegPointX(i)), pointToY(processor.msegPointY(i))}); if (d < distance) { distance = d; best = i; } } return best; }
    float pointToX(float x) const { return graphX + x * graphW; }
    float pointToY(float y) const { return graphY + (1.0f - y) * graphH; }
    float toX(float x) const { return juce::jlimit(0.0f, 1.0f, (x - graphX) / graphW); }
    float toY(float y) const { return juce::jlimit(0.0f, 1.0f, 1.0f - (y - graphY) / graphH); }
    void timerCallback() override { repaint(); }
    MonomachineNovaAudioProcessor& processor; juce::ToggleButton loop, sync; juce::Slider rate; juce::TextButton reset; std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> loopLink, syncLink; std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rateLink; int selected = -1, flash = -1; bool draggingPoint = false, erasing = false;
    static constexpr int graphX = 20, graphY = 60, graphW = 730, graphH = 285;
};

class EnvelopePage final : public juce::Component,private juce::Timer {
public:
    explicit EnvelopePage(MonomachineNovaAudioProcessor& p):processor(p){
        mode.setScrollWheelEnabled(true);mode.addItem("LEGACY 1.4",1);mode.addItem("REFERENCE FIT",2);addAndMakeVisible(mode);
        modeLink=std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(p.parameters,"amp_mode",mode);
        const char* ids[]{"p0_0","p0_1","p0_2","p0_3","amp_curve_a","amp_curve_d","amp_curve_r"};
        const char* names[]{"ATK","HOLD","DEC","REL","ATK CURVE","DEC CURVE","REL CURVE"};
        for(size_t i=0;i<7;++i){auto& sl=sliders[i];sl.setColour(juce::Slider::thumbColourId,ink);sl.setColour(juce::Slider::trackColourId,ink.withAlpha(0.8f));sl.setColour(juce::Slider::backgroundColourId,ink.withAlpha(0.2f));sl.setSliderStyle(juce::Slider::LinearHorizontal);sl.setTextBoxStyle(juce::Slider::TextBoxRight,false,62,22);sl.setName(names[i]);sl.setDoubleClickReturnValue(true,i<4?(i<2?0:64):0);addAndMakeVisible(sl);links[i]=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters,ids[i],sl);labels[i].setText(names[i],juce::dontSendNotification);addAndMakeVisible(labels[i]);}
        gate.setColour(juce::Slider::thumbColourId,ink);gate.setColour(juce::Slider::trackColourId,ink.withAlpha(0.8f));gate.setColour(juce::Slider::backgroundColourId,ink.withAlpha(0.2f));gate.setRange(10,10000,1);gate.setValue(1000);gate.setSliderStyle(juce::Slider::LinearHorizontal);gate.setTextBoxStyle(juce::Slider::TextBoxRight,false,62,22);addAndMakeVisible(gate);
        gateLabel.setText("PREVIEW NOTE (ms)",juce::dontSendNotification);addAndMakeVisible(gateLabel);
        setSize(1170,465);startTimerHz(15);refresh();
    }
    void focusCurve(int knob){selected=knob==0?4:knob==2?5:6;if(sliders[static_cast<size_t>(selected)].isEnabled())sliders[static_cast<size_t>(selected)].grabKeyboardFocus();repaint();}
    void resized()override{mode.setBounds(850,8,300,26);for(int i=0;i<8;++i){const int x=20+(i%4)*285,y=330+(i/4)*58;if(i<7){labels[static_cast<size_t>(i)].setBounds(x,y,260,20);sliders[static_cast<size_t>(i)].setBounds(x,y+20,265,28);}else{gateLabel.setBounds(x,y,265,20);gate.setBounds(x,y+20,265,28);}}}
    void paint(juce::Graphics& g)override{
        g.fillAll(juce::Colours::black);g.setColour(ink);pixel::text(g,"AMP / ENVELOPE",{20,8,650,26},22);
        const juce::Rectangle<float> box(30,60,1110,225);g.setColour(ink.withAlpha(0.2f));g.drawRect(box);
        for(int i=1;i<4;++i)g.drawHorizontalLine(60+i*56,30,1140);
        for(int i=0;i<=4;++i){float x=30.0f+1110.0f*static_cast<float>(i)/4.0f;g.drawVerticalLine(juce::roundToInt(x),60,285);g.setColour(ink);g.drawText(juce::String(duration*i/4,2)+" s",juce::roundToInt(x)-25,286,70,20,juce::Justification::centred);g.setColour(ink.withAlpha(0.2f));}
        juce::Path path;for(size_t i=0;i<trace.size();++i){float x=30+1110*static_cast<float>(i)/std::max(1.0f,static_cast<float>(trace.size()-1));float y=285-225*trace[i];if(i==0)path.startNewSubPath(x,y);else path.lineTo(x,y);}g.setColour(ink);g.strokePath(path,juce::PathStrokeType(1.8f));
        const float off=30+1110*static_cast<float>(gate.getValue()/1000/duration);if(off<1140){g.setColour(ink.withAlpha(0.5f));g.drawVerticalLine(juce::roundToInt(off),60,285);g.drawText("NOTE OFF",juce::roundToInt(off)+4,62,85,18,juce::Justification::left);}
        g.setColour(ink);g.drawText(mode.getSelectedId()==1?"Legacy: original 1.4 DSP. Curves apply only in REFERENCE FIT.":"Drag graph vertically: decay before NOTE OFF, release after. Preview uses the DSP envelope; display limited to 20 s.",20,307,1130,20,juce::Justification::left);
        if(selected>=4){auto r=sliders[static_cast<size_t>(selected)].getBounds().expanded(2);g.drawRect(r);}
    }
    void mouseDown(const juce::MouseEvent& e)override{if(e.y<60||e.y>285||mode.getSelectedId()!=2)return;dragId=(e.x-30)/1110.0*duration<gate.getValue()/1000?"amp_curve_d":"amp_curve_r";dragParameter=processor.parameters.getParameter(dragId);origin=e.y;start=dragParameter->convertFrom0to1(dragParameter->getValue());dragParameter->beginChangeGesture();}
    void mouseDrag(const juce::MouseEvent& e)override{if(dragParameter)dragParameter->setValueNotifyingHost(dragParameter->convertTo0to1(juce::jlimit(-100.0f,100.0f,start+static_cast<float>(origin-e.y))));}
    void mouseUp(const juce::MouseEvent&)override{endGesture();}
    ~EnvelopePage()override{endGesture();}
private:
    void endGesture(){if(dragParameter){dragParameter->endChangeGesture();dragParameter=nullptr;}}
    void timerCallback()override{refresh();}
    void refresh(){std::array<double,9> values{};for(size_t i=0;i<7;++i)values[i]=sliders[i].getValue();values[7]=mode.getSelectedId();values[8]=gate.getValue();if(values==previous)return;previous=values;for(size_t i=4;i<7;++i)sliders[i].setEnabled(mode.getSelectedId()==2);
        nova::AmpEnvelope env;env.configure(mode.getSelectedId()-1,{static_cast<float>(values[4]),static_cast<float>(values[5]),static_cast<float>(values[6])});env.reset(2000);for(size_t i=0;i<4;++i)env.p[i]=static_cast<float>(values[i]);env.on();std::vector<float> samples; samples.reserve(40000);
        for(int i=0;i<40000;++i){if(i==juce::roundToInt(gate.getValue()*2))env.off();samples.push_back(env.tick());if(i>2&&env.stage==0)break;}
        duration=std::max(0.002,static_cast<double>(samples.size())/2000);trace.clear();for(size_t i=0;i<1000;++i)trace.push_back(samples[std::min(samples.size()-1,i*samples.size()/1000)]);repaint();}
    MonomachineNovaAudioProcessor& processor;juce::ComboBox mode;std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> modeLink;
    std::array<juce::Slider,7> sliders;std::array<juce::Label,7> labels;std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>,7> links;
    juce::Slider gate;juce::Label gateLabel;std::array<double,9> previous{};std::vector<float> trace;double duration=1;int selected=-1,origin=0;float start=0;juce::String dragId;juce::RangedAudioParameter* dragParameter=nullptr;
};

class SamplePage final : public juce::Component,public juce::ListBoxModel,public juce::FileDragAndDropTarget,private juce::Timer {
public:
    explicit SamplePage(MonomachineNovaAudioProcessor& p):processor(p),list("BBOX slots",this){
                addAndMakeVisible(list);list.setRowHeight(29);list.setColour(juce::ListBox::backgroundColourId,juce::Colours::black);
        list.setColour(juce::ListBox::outlineColourId,juce::Colours::white.withAlpha(0.5f));list.setOutlineThickness(1);
        addAndMakeVisible(load);load.setButtonText("LOAD FILES");load.onClick=[this]{choose();};
        addAndMakeVisible(oldKit);oldKit.setButtonText("OLD KIT");oldKit.onClick=[this]{processor.resetSamples(false);list.updateContent();list.repaint();};
        addAndMakeVisible(restore);restore.setButtonText("RESTORE KIT");restore.onClick=[this]{processor.resetSamples();list.updateContent();list.repaint();};restore.setTooltip("Replace all 24 slots with the factory kit, including imported samples.");
        startTimerHz(25);setSize(1120,450);list.selectRow(juce::roundToInt(processor.parameters.getRawParameterValue("m7_2")->load()));
    }
    int getNumRows()override{return 24;}
    void paintListBoxItem(int row,juce::Graphics& g,int w,int h,bool selected)override{
        if(row==processor.playingSample()||(row==flashSlot&&juce::Time::getMillisecondCounterHiRes()<flashUntil))g.fillAll(juce::Colours::white.withAlpha(0.35f));
        else if(selected)g.fillAll(juce::Colours::white.withAlpha(0.18f));g.setColour(juce::Colours::white);
        const char* notes[]{"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};
        pixel::text(g,juce::String(row+1).paddedLeft('0',2)+"  "+juce::String(notes[row%12])+juce::String((48+row)/12-2)+"  "+processor.sampleName(row),{8,2,w-16,h-4},16);
    }
    void selectedRowsChanged(int row)override{if(row<0)return;auto* par=processor.parameters.getParameter("m7_2");par->beginChangeGesture();par->setValueNotifyingHost(par->convertTo0to1(static_cast<float>(row)));par->endChangeGesture();}
    void listBoxItemDoubleClicked(int,const juce::MouseEvent&)override{choose();}
    bool isInterestedInFileDrag(const juce::StringArray& files)override{return !files.isEmpty();}
    void filesDropped(const juce::StringArray& files,int x,int y)override{const auto q=list.getLocalPoint(this,juce::Point<int>{x,y});int row=list.getRowContainingPosition(q.x,q.y);loadFiles(files,row<0?std::max(0,list.getSelectedRow()):row);}
    void paint(juce::Graphics& g)override{g.fillAll(juce::Colours::black);g.setColour(juce::Colours::white);pixel::text(g,"BBOX / 24 SLOTS",{8,4,560,24},20);}
    void resized()override{oldKit.setBounds(getWidth()-532,4,168,28);restore.setBounds(getWidth()-356,4,168,28);load.setBounds(getWidth()-180,4,168,28);list.setBounds(8,40,getWidth()-16,getHeight()-48);}
private:
    void timerCallback()override{auto serial=processor.bboxHitCounter.load();if(serial!=lastSerial){lastSerial=serial;flashSlot=processor.lastBboxSample.load();flashUntil=juce::Time::getMillisecondCounterHiRes()+120;}list.repaint();}
    void loadFiles(const juce::StringArray& files,int row){juce::String errors;for(const auto& path:files){if(row>=24)break;auto e=processor.loadSample(row++,juce::File(path));if(e.isNotEmpty())errors+=e+"\n";}list.updateContent();list.repaint();if(errors.isNotEmpty())juce::AlertWindow::showMessageBoxAsync(juce::MessageBoxIconType::WarningIcon,"Sample import",errors);}
    void choose(){chooser=std::make_unique<juce::FileChooser>("Load BBOX samples",juce::File(),"*.wav;*.aif;*.aiff;*.flac");juce::Component::SafePointer<SamplePage> safe(this);
        chooser->launchAsync(juce::FileBrowserComponent::openMode|juce::FileBrowserComponent::canSelectFiles|juce::FileBrowserComponent::canSelectMultipleItems,[safe](const juce::FileChooser& fc){if(!safe)return;juce::StringArray paths;for(auto f:fc.getResults())paths.add(f.getFullPathName());safe->loadFiles(paths,std::max(0,safe->list.getSelectedRow()));});}
    unsigned lastSerial=0;int flashSlot=-1;double flashUntil=0;
    MonomachineNovaAudioProcessor& processor;juce::ListBox list;juce::TextButton load,restore,oldKit;std::unique_ptr<juce::FileChooser> chooser;
};
class FramedViewport : public juce::Viewport {
public:
    FramedViewport(){setOpaque(true);}
    void paint(juce::Graphics& g)override{g.fillAll(juce::Colours::black);}
    void paintOverChildren(juce::Graphics& g)override{g.setColour(juce::Colours::white.withAlpha(0.65f));g.drawRect(getLocalBounds(),2);}
};

}

struct MonomachineNovaAudioProcessorEditor::Surface final : juce::Component, public juce::DragAndDropContainer {
    explicit Surface(MonomachineNovaAudioProcessor& p):processor(p),machineButton(""),menuButton("MENU"),tempoButton(""),arpButton("ARP"),arpSettingsButton("SET"),matrixButton("MATRIX"),lfoButton("LFO2"),lfo3Button("LFO3"),msegButton("MSEG"),msegOutputButton("MSEG OUT"),closeButton("BACK"),gateButton(""){
        setLookAndFeel(&theme);
        for(auto& panel:cells)for(auto& cell:panel){cell=std::make_unique<Cell>(p);cell->openSamples=[this]{showSamples();};cell->openEnvelope=[this](int knob){showEnvelope(knob);};cell->openModes=[this](int section){showDspModes(section);};cell->addMsegModulation=[this](uint8_t target){processor.addMsegRoute(target);};cell->pickTarget=[this](uint8_t target){completeTargetPick(target);};cell->pickHover=[this](uint8_t target){pickHoverTarget=target;};addAndMakeVisible(*cell);}
        for(auto* button:{&machineButton,&menuButton,&tempoButton,&arpButton,&arpSettingsButton,&matrixButton,&lfoButton,&lfo3Button,&msegButton})addAndMakeVisible(*button);addAndMakeVisible(msegOutputButton);msegOutputButton.textHeight=12;msegOutputButton.setTooltip("Drag this external MSEG output point onto a Matrix target or parameter.");
        addAndMakeVisible(dspModeButton);dspModeButton.onClick=[this]{showDspModes(-1);};updateDspModeLabel();
        addAndMakeVisible(ampButton);ampButton.setButtonText("AMP");ampButton.textHeight=28;ampButton.textAlign=1;ampButton.onClick=[this]{showEnvelope(-1);};
        addAndMakeVisible(ampMode);ampMode.setScrollWheelEnabled(true);ampMode.addItem("LEGACY 1.4",1);ampMode.addItem("REFERENCE FIT",2);ampModeLink=std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(p.parameters,"amp_mode",ampMode);
        level.setSliderStyle(juce::Slider::LinearVertical);level.setTextBoxStyle(juce::Slider::NoTextBox,false,0,0);
        level.setColour(juce::Slider::trackColourId,ink);level.setColour(juce::Slider::thumbColourId,ink);level.setColour(juce::Slider::backgroundColourId,juce::Colours::black);addAndMakeVisible(level);
        levelLink=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters,"level",level);
        machineButton.textAlign=1;
        machineButton.onClick=[this]{showMachineMenu();};
        machineButton.dragHandler=[this](int steps){cycleMachine(steps);};
        menuButton.onClick=[this]{showMenu();};
        // BPM drags to change tempo; it no longer hijacks a settings page.
        tempoButton.textHeight=22;tempoButton.outlined=false;tempoButton.textAlign=1;
        tempoButton.dragHandler=[this](int steps){nudgeBpm(static_cast<float>(steps)*0.1f);};
        addAndMakeVisible(tempoSync);tempoSync.setButtonText("SYNC");tempoSyncLink=std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(p.parameters,"host_sync",tempoSync);
        addChildComponent(tempoEntry);tempoEntry.setName("BPM entry");tempoEntry.setInputRestrictions(7,"0123456789.");tempoEntry.onEscapeKey=[this]{tempoEntry.setVisible(false);};tempoEntry.onFocusLost=[this]{tempoEntry.setVisible(false);};
        tempoEntry.onReturnKey=[this]{if(tempoEntry.getText().isNotEmpty()){auto* bpm=processor.parameters.getParameter("bpm");bpm->beginChangeGesture();bpm->setValueNotifyingHost(bpm->convertTo0to1(juce::jlimit(30.0f,300.0f,tempoEntry.getText().getFloatValue())));bpm->endChangeGesture();setHostSyncOff();}tempoEntry.setVisible(false);update();};
        tempoButton.doubleClickHandler=[this]{tempoEntry.setText(tempoButton.getButtonText(),false);tempoEntry.setVisible(true);tempoEntry.toFront(true);tempoEntry.grabKeyboardFocus();tempoEntry.selectAll();};
        arpButton.dragPixelsPerStep=2;arpButton.dragHandler=[this](int steps){const bool sync=processor.parameters.getRawParameterValue("arp_sync")->load()>0.5f;auto* par=processor.parameters.getParameter(sync?"arp_grid":"arp_time");float value=par->convertFrom0to1(par->getValue())+static_cast<float>(steps)*(sync?1.0f:10.0f);par->beginChangeGesture();par->setValueNotifyingHost(par->convertTo0to1(juce::jlimit(sync?0.0f:5.0f,sync?15.0f:2000.0f,value)));par->endChangeGesture();update();};
        arpButton.textHeight=18;arpSettingsButton.textHeight=12;arpSettingsButton.setTooltip("Open the ARP step editor");arpSettingsButton.onClick=[this]{showSettings(false);};
        matrixButton.textHeight=22;matrixButton.onClick=[this]{showSettings(true);};
        lfoButton.textHeight=28;
        lfoButton.onClick=[this]{showLfo3=false;bind();repaint();};lfo3Button.textHeight=24;lfo3Button.onClick=[this]{showLfo3=true;bind();repaint();};lfoButton.setTooltip("Click to switch between LFO2 and LFO3; all three run independently.");
        msegButton.textHeight=24;msegButton.onClick=[this]{showMseg();};msegButton.setTooltip("Open the multi-segment modulation editor. Drag OUTPUT to a parameter or a matrix target.");
        closeButton.textHeight=20;closeButton.onClick=[this]{if(pickingTarget){for(auto& column:cells)for(auto& cell:column)if(cell)cell->setPickMode(false);pickingTarget=false;}viewport.setViewedComponent(nullptr);settings.reset();matrixPage.reset();samplePage.reset();msegPage.reset();envelopePage.reset();viewport.setVisible(false);closeButton.setVisible(false);repaint();};
        addChildComponent(viewport);addChildComponent(closeButton);
        addAndMakeVisible(arpEnabled);arpEnabled.setTooltip("Enable arpeggiator");
        arpLink=std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(p.parameters,"arp_on",arpEnabled);
        if(!processor.isSynthVersion){addAndMakeVisible(trigToggle);trigToggle.setButtonText("MIDI");trigLink=std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(p.parameters,"gate",trigToggle);}
        viewport.setScrollBarsShown(true,false);viewport.setScrollBarThickness(14);
        viewport.getVerticalScrollBar().setColour(juce::ScrollBar::thumbColourId,juce::Colours::white.withAlpha(0.7f));
        setSize(1260,584);bind();update();
    }
    ~Surface()override{viewport.setViewedComponent(nullptr);setLookAndFeel(nullptr);}
    void bind(){
        boundMachine=processor.machineIndex();updateDspModeLabel();const auto& m=nova::machines()[static_cast<size_t>(boundMachine)];machineButton.setButtonText(m.name+"  >");machineButton.textHeight=28;
        for(int i=0;i<8;++i){const auto& def=m.synthParams[static_cast<size_t>(i)];cells[0][static_cast<size_t>(i)]->bind(def.maxVal?nova::machineParam(m.id,i):juce::String(),def.name,-1,i,m.id);}
        const int pageForPanel[]={-1,0,3,1,2,showLfo3?5:4};
        for(int panel=1;panel<6;++panel)for(int i=0;i<8;++i){int page=pageForPanel[panel];cells[static_cast<size_t>(panel)][static_cast<size_t>(i)]->bind(nova::pageParam(page,i),nova::pageLabel(page,i),page,i);}
        lfoButton.setButtonText(showLfo3?"LFO2":"[LFO2]");lfo3Button.setButtonText(showLfo3?"[LFO3]":"LFO3");
        if(!processor.isSynthVersion)updateGateLabel();
        repaint();
    }
    void update(){
        if(boundMachine!=processor.machineIndex())bind();
        tempoButton.setButtonText(juce::String(processor.parameters.getRawParameterValue("host_sync")->load()>0.5f?processor.currentBpm():static_cast<double>(processor.parameters.getRawParameterValue("bpm")->load()),1));
        if(!processor.isSynthVersion)updateGateLabel();
        auto* sync=processor.parameters.getRawParameterValue("arp_sync");auto* rate=processor.parameters.getParameter("arp_grid");
        const auto text=sync->load()>0.5f?rate->getText(rate->getValue(),16):juce::String(processor.parameters.getRawParameterValue("arp_time")->load(),1)+" MS";
        arpButton.setButtonText("ARP "+text);
        repaint();
    }
    void paint(juce::Graphics& g)override{
        g.fillAll(juce::Colours::black);g.setColour(ink);
        // Original geometric mark, drawn locally (not the Elektron logo), in white.
        for(int i=0;i<5;++i){float angle=i*2*nova::pi/5;g.fillRect(25+juce::roundToInt(12*std::sin(angle)),22+juce::roundToInt(12*std::cos(angle)),8,8);}g.fillRect(24,20,13,13);
        pixel::text(g,processor.isSynthVersion?"MONOMACHINE NOVA SYNTH":"MONOMACHINE NOVA FX",{66,0,670,40},24);
        pixel::text(g,"BPM",{855,4,68,30},22);pixel::text(g,"LEV",{7,56,58,32},28);
        pixel::dotted(g,8,94,8,558);pixel::dotted(g,61,94,61,558);pixel::dotted(g,8,94,63,94);pixel::dotted(g,8,558,63,558);
        const char* names[]{"SYNT","","LFO1","FILT","EFFX",""};
        for(int panel=0;panel<6;++panel){int x=72+(panel%3)*399,y=94+(panel/3)*240;
            g.setColour(ink.withAlpha(0.35f));g.drawRect(x,y,387,236,1);g.setColour(ink);g.drawRect(x,y,387,30,2);pixel::text(g,names[panel],{x+5,y,377,30},28);
            g.setColour(ink);for(int c=0;c<=4;++c)pixel::dotted(g,x+c*96,y+34,x+c*96,y+228);
            pixel::dotted(g,x,y+34,x+388,y+34);pixel::dotted(g,x,y+130,x+388,y+130);pixel::dotted(g,x,y+228,x+388,y+228);}

        g.setColour(ink.withAlpha(0.4f));g.drawRect(8,94,55,464,2);
        g.setColour(ink.withAlpha(0.65f));g.drawText(processor.isSynthVersion?"BUILD 1.6.0 / Synth / mnm modes":"BUILD 1.6.0 / FX / mnm modes",850,570,370,13,juce::Justification::right);
    }
    void resized()override{
        ampButton.setBounds(473,95,120,28);ampMode.setBounds(640,97,213,25);
        machineButton.setBounds(72,58,387,30);dspModeButton.setBounds(473,58,387,30);menuButton.setBounds(1161,4,94,32);tempoButton.setBounds(925,4,88,30);tempoSync.setBounds(1020,4,96,30);tempoEntry.setBounds(925,4,88,30);
        arpEnabled.setBounds(471,58,26,30);arpButton.setBounds(500,58,120,30);arpSettingsButton.setBounds(624,58,34,30);trigToggle.setBounds(700,58,140,30);matrixButton.setBounds(870,58,140,30);
        lfoButton.setBounds(870,334,120,30);lfo3Button.setBounds(995,334,120,30);msegButton.setBounds(1090,334,82,30);msegOutputButton.setBounds(1178,334,78,30);
        level.setBounds(12,97,46,458);
        for(int panel=0;panel<6;++panel)for(int i=0;i<8;++i)cells[static_cast<size_t>(panel)][static_cast<size_t>(i)]->setBounds(75+(panel%3)*399+(i%4)*96,130+(panel/3)*240+(i/4)*96,91,91);
        viewport.setBounds(65,88,1192,480);closeButton.setBounds(1070,51,145,32);gateButton.setVisible(false);
    }
    void showMseg(){viewport.setViewedComponent(nullptr,false);settings.reset();matrixPage.reset();samplePage.reset();envelopePage.reset();msegPage=std::make_unique<MsegPage>(processor);viewport.setViewedComponent(msegPage.get(),false);viewport.setVisible(true);viewport.toFront(false);closeButton.setVisible(true);closeButton.toFront(false);}
    void showEnvelope(int knob){viewport.setViewedComponent(nullptr,false);settings.reset();matrixPage.reset();samplePage.reset();msegPage.reset();envelopePage=std::make_unique<EnvelopePage>(processor);viewport.setViewedComponent(envelopePage.get(),false);viewport.setVisible(true);viewport.toFront(false);closeButton.setVisible(true);closeButton.toFront(false);if(knob>=0)envelopePage->focusCurve(knob);}
    void setViewportMode(bool matrix){
        viewport.setViewedComponent(nullptr,false);envelopePage.reset();samplePage.reset();msegPage.reset();
        if(matrix){settings.reset();matrixPage=std::make_unique<MatrixPage>(processor);matrixPage->onTargetPick=[this](int row){beginTargetPick(row);};viewport.setViewedComponent(matrixPage.get(),false);}
        else{matrixPage.reset();settings=std::make_unique<ArpPage>(processor);viewport.setViewedComponent(settings.get(),false);}
        viewport.setVisible(true);viewport.toFront(false);closeButton.setVisible(true);closeButton.toFront(false);
    }
    void beginTargetPick(int route){
        pickRoute=juce::jlimit(0,15,route);pickingTarget=true;
        for(auto& column:cells)for(auto& cell:column)if(cell)cell->setPickMode(true);
        viewport.setVisible(false);repaint();
    }
    void completeTargetPick(uint8_t target){
        if(!pickingTarget)return;
        auto* p=processor.parameters.getParameter("r"+juce::String(pickRoute)+"_dest");
        if(p){p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(target)));p->endChangeGesture();}
        for(auto& column:cells)for(auto& cell:column)if(cell){cell->setPickMode(false);if(cell->isTarget(target))cell->flashPick();}
        pickingTarget=false;viewport.setVisible(true);viewport.toFront(false);repaint();
    }
    void showSamples(){viewport.setViewedComponent(nullptr,false);envelopePage.reset();settings.reset();matrixPage.reset();msegPage.reset();samplePage=std::make_unique<SamplePage>(processor);viewport.setViewedComponent(samplePage.get(),false);viewport.setVisible(true);viewport.toFront(false);closeButton.setVisible(true);closeButton.toFront(false);}
    void cycleMachine(int steps){
        auto* p=processor.parameters.getParameter("machine");if(!p)return;
        const int n=static_cast<int>(nova::machines().size());
        int raw=juce::roundToInt(p->convertFrom0to1(p->getValue()));raw=((raw+steps)%n+n)%n;
        p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(raw)));p->endChangeGesture();bind();
    }
    void setHostSyncOff(){auto* p=processor.parameters.getParameter("host_sync");p->beginChangeGesture();p->setValueNotifyingHost(0);p->endChangeGesture();}
    void nudgeBpm(float delta){
        auto* p=processor.parameters.getParameter("bpm");if(!p)return;
        const float base=processor.parameters.getRawParameterValue("host_sync")->load()>0.5f?static_cast<float>(processor.currentBpm()):p->convertFrom0to1(p->getValue());setHostSyncOff();const float v=juce::jlimit(30.0f,300.0f,base+delta);
        p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(v));p->endChangeGesture();
    }
    void toggleArp(){
        auto* p=processor.parameters.getParameter("arp_mode");if(!p)return;
        const int cur=juce::roundToInt(p->convertFrom0to1(p->getValue()));const int next=cur>0?0:1; // OFF <-> KEY; SID/ADD via MENU
        p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(next)));p->endChangeGesture();
    }
    void toggleGate(){
        auto* p=processor.parameters.getParameter("gate");if(!p)return;
        const float cur=p->convertFrom0to1(p->getValue());
        p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(cur>0.5f?0.0f:1.0f));p->endChangeGesture();updateGateLabel();
    }
    void updateGateLabel(){
        auto* p=processor.parameters.getParameter("gate");if(!p)return;
        const bool midi=p->convertFrom0to1(p->getValue())>0.5f;
        gateButton.setButtonText(midi?"FX MIDI":"FX LATCH");
        gateButton.setTooltip(midi?"FX is strictly triggered/gated by incoming MIDI notes (amp envelope active).":"Latch holds AMP open; incoming MIDI still drives note triggers and modulation.");
    }
    void showMachineMenu(){juce::PopupMenu menu;juce::String category;
        for(size_t i=0;i<nova::machines().size();++i){const auto& m=nova::machines()[i];if(category!=juce::String(m.category)){category=m.category;menu.addSectionHeader(category);}
            auto shortName=juce::String(m.name);if(shortName.startsWith(m.category))shortName=shortName.substring(static_cast<int>(m.category.size())).trimCharactersAtStart("- ");
            menu.addItem(static_cast<int>(i)+1,shortName,true,static_cast<int>(i)==boundMachine);}

        juce::Component::SafePointer<Surface> safe(this);
        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(&machineButton).withMinimumWidth(320),[safe](int result){if(!safe||result<=0)return;
            auto* p=safe->processor.parameters.getParameter("machine");p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(result-1)));p->endChangeGesture();safe->bind();});
    }
    // DSP mode list. Every section that owns DSP has its own list; entry "mnm" is the
    // recovered OS 1.32B firmware algorithm (default) and "old" is the previous Nova code.
    void showDspModes(int section){
        juce::PopupMenu menu;
        auto addSection=[&](int index){
            const int current=processor.dspMode(index);
            juce::PopupMenu sub;
            sub.addItem(index*10+monomachine::dspModeMnm+1,"mnm",true,current==monomachine::dspModeMnm);
            sub.addItem(index*10+monomachine::dspModeOld+1,"old",true,current==monomachine::dspModeOld);
            sub.addSeparator();
            sub.addItem(-1,monomachine::dspModeProvenance(index),false,false);
            menu.addSubMenu(juce::String(monomachine::dspSectionLabel(index))+"   ["+(current==monomachine::dspModeMnm?"mnm":"old")+"]",sub);
        };
        if(section>=0&&section<monomachine::DspSectionCount)addSection(section);
        else for(int i=0;i<monomachine::DspSectionCount;++i)addSection(i);
        juce::Component::SafePointer<Surface> safe(this);
        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(&dspModeButton).withMinimumWidth(320),[safe](int result){
            if(!safe||result<=0)return;
            const int index=(result-1)/10,mode=(result-1)%10;
            if(index<0||index>=monomachine::DspSectionCount)return;
            auto* p=safe->processor.parameters.getParameter(monomachine::dspModeParamId(index));if(!p)return;
            p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(mode)));p->endChangeGesture();
            safe->updateDspModeLabel();});
    }
    void updateDspModeLabel(){
        juce::String text="DSP MODE";
        for(int i=0;i<monomachine::DspSectionCount;++i)text+=" "+(processor.dspMode(i)==monomachine::dspModeMnm?juce::String("m"):juce::String("o"));
        dspModeButton.setButtonText(text);
        juce::String tip="DSP modes (m = mnm / original OS 1.32B, o = old):\n";
        for(int i=0;i<monomachine::DspSectionCount;++i)tip+=juce::String(monomachine::dspSectionLabel(i))+": "+(processor.dspMode(i)==monomachine::dspModeMnm?"mnm":"old")+" - "+monomachine::dspModeProvenance(i)+"\n";
        dspModeButton.setTooltip(tip);
    }
    void showSettings(bool matrix){setViewportMode(matrix);}
    void showMenu(){juce::PopupMenu menu;menu.addItem(1,"TEMPO / ARPEGGIATOR / GLOBAL");menu.addItem(2,"MODULATION MATRIX (16 ROUTES)");menu.addItem(8,"MSEG / CURVE EDITOR");
        if(processor.isSynthVersion){menu.addItem(7,"BBOX SAMPLES / DROP FILES");menu.addItem(3,"RESTORE NATIVE KIT");}
        menu.addSeparator();menu.addItem(4,"PANIC / CLEAR TAILS");menu.addItem(5,"RESET ALL PARAMETERS");menu.addItem(6,"ABOUT / LIMITATIONS");
        juce::Component::SafePointer<Surface> safe(this);menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(&menuButton),[safe](int result){if(!safe)return;
            if(result==1||result==2)safe->setViewportMode(result==2);if(result==8)safe->showMseg();
            if(result==3)safe->processor.resetSamples();if(result==4)safe->processor.requestPanic();
            if(result==5){safe->processor.requestPanic();for(auto* parameter:safe->processor.getParameters()){parameter->beginChangeGesture();parameter->setValueNotifyingHost(parameter->getDefaultValue());parameter->endChangeGesture();}safe->bind();}
            if(result==6)juce::AlertWindow::showMessageBoxAsync(juce::MessageBoxIconType::InfoIcon,"Monomachine Nova Synth / BUILD 1.6.0","Build 1.6.0 mnm modes: AMP editor, MIDI/LATCH, FX unity calibration.\nNative implementations; not an exact emulator.\nFX-CHORUS runs the translated OS 1.32B core (24-bit fixed point, DEL/DEP/SPD/MIX/FB/WID/LP/INP).\nINP is input gain everywhere, DIST/DSND are bipolar, DSND<0 pings across.\nFX: MIDI OFF = latch holds AMP open; MIDI ON = note envelope gate.\nAHD: HOLD=127 sustains. MENU contains arp and 8 modulation routes.\nBBOX: 24 native sample slots, CHRM keyboard map and embedded custom PCM. Factory ROM audio is not included.\nLFO/ARP use host BPM but are note-synchronised, not timeline-locked.\nNo oversampling or output limiter. Turn down LEV before testing.");
            if(result==7)safe->showSamples();
        });
    }
    void chooseSample(int slot){chooser=std::make_unique<juce::FileChooser>("Load MOD slot "+juce::String(slot+1),juce::File(),"*.wav;*.aif;*.aiff");
        juce::Component::SafePointer<Surface> safe(this);chooser->launchAsync(juce::FileBrowserComponent::openMode|juce::FileBrowserComponent::canSelectFiles,[safe,slot](const juce::FileChooser& fc){if(!safe)return;auto file=fc.getResult();if(!file.existsAsFile())return;
            auto error=safe->processor.loadSample(slot,file);if(error.isNotEmpty())juce::AlertWindow::showMessageBoxAsync(juce::MessageBoxIconType::WarningIcon,"Sample load",error);
        });
    }
    MonomachineNovaAudioProcessor& processor;PixelTheme theme;
    std::array<std::array<std::unique_ptr<Cell>,8>,6> cells;
    PixelButton machineButton,menuButton,tempoButton,arpButton,arpSettingsButton,matrixButton,lfoButton,lfo3Button,msegButton;MsegOutputButton msegOutputButton;PixelButton closeButton,gateButton;juce::Slider level;PixelButton dspModeButton{"DSP MODE"};
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> levelLink;
    juce::ToggleButton arpEnabled,trigToggle,tempoSync;juce::TextEditor tempoEntry;std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> tempoSyncLink;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> arpLink,trigLink;
    PixelButton ampButton{"AMP"};juce::ComboBox ampMode;std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> ampModeLink;std::unique_ptr<EnvelopePage> envelopePage;
    std::unique_ptr<SamplePage> samplePage;std::unique_ptr<MsegPage> msegPage;
    std::unique_ptr<ArpPage> settings;std::unique_ptr<MatrixPage> matrixPage;FramedViewport viewport;std::unique_ptr<juce::FileChooser> chooser;
    juce::TooltipWindow tooltips{this,800};bool showLfo3=false;int boundMachine=-1;bool pickingTarget=false;int pickRoute=-1,pickHoverTarget=-1;
};
MonomachineNovaAudioProcessorEditor::MonomachineNovaAudioProcessorEditor(MonomachineNovaAudioProcessor& p):AudioProcessorEditor(&p),processor(p){
    surface=std::make_unique<Surface>(p);addAndMakeVisible(*surface);
    // Default 1:1 pixel size; the window may be scaled wider (keeps the reference 1260x584 grid).
    setResizable(true,true);setResizeLimits(1260,584,3780,1752);getConstrainer()->setFixedAspectRatio(1260.0/584.0);setSize(1260,584);startTimerHz(20);
}
MonomachineNovaAudioProcessorEditor::~MonomachineNovaAudioProcessorEditor(){stopTimer();}
void MonomachineNovaAudioProcessorEditor::paint(juce::Graphics& g){g.fillAll(juce::Colours::black);}
void MonomachineNovaAudioProcessorEditor::resized(){if(surface){surface->setTopLeftPosition(0,0);surface->setTransform(juce::AffineTransform::scale(static_cast<float>(getWidth())/1260.0f,static_cast<float>(getHeight())/584.0f));}}
void MonomachineNovaAudioProcessorEditor::timerCallback(){surface->update();}
