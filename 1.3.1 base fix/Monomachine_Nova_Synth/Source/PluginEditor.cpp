#include "PluginEditor.h"
#include "PixelFont.h"

// v1.3 UI pass, all per the black reference screen: white ink on black, outlined boxes,
// list knobs change by press-and-drag (the popup stays as a bonus), BPM drags instead of
// opening settings, the matrix gets its own pixel page, and the FX build carries a
// MIDI/FREE gate toggle right on the main screen.
namespace {
const juce::Colour ink=juce::Colours::white;      // foreground on black, like the LCD
const juce::Colour knockout=juce::Colours::black; // ink knocked out of filled bars
const juce::Colour dim=juce::Colour(0xff808080);  // unused cells

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
    void drawLinearSlider(juce::Graphics& g,int x,int y,int width,int height,float pos,float min,float max,const juce::Slider::SliderStyle style,juce::Slider& slider)override{
        if(style==juce::Slider::LinearVertical){g.setColour(ink);g.fillRect(x+width/2-9,juce::roundToInt(pos),18,std::max(1,y+height-juce::roundToInt(pos)));}
        else juce::LookAndFeel_V4::drawLinearSlider(g,x,y,width,height,pos,min,max,style,slider);
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
class PixelButton final : public juce::TextButton {
public:
    explicit PixelButton(const juce::String& s):juce::TextButton(s){}
    int textHeight=28;
    int textAlign=0; // 0 centred, 1 left
    bool outlined=true;
    std::function<void(int)> dragHandler;
    void paintButton(juce::Graphics& g,bool /*over*/,bool down)override{
        g.fillAll(down&&outlined?ink:juce::Colours::black);
        if(outlined){g.setColour(ink);g.drawRect(getLocalBounds().reduced(1),1);}
        g.setColour(down&&outlined?knockout:ink);
        pixel::text(g,getButtonText(),getLocalBounds().reduced(5,0),textHeight,textAlign==0);
    }
    void mouseDown(const juce::MouseEvent&e)override{dragStartX=e.x;dragAccum=0;wasDragged=false;juce::TextButton::mouseDown(e);}
    void mouseDrag(const juce::MouseEvent&e)override{
        if(!dragHandler)return;const int dx=e.x-dragStartX;
        if(std::abs(dx)>=6){wasDragged=true;const int steps=(dx-dragAccum)/6;if(steps!=0){dragAccum+=steps*6;dragHandler(steps);}}
    }
    void mouseUp(const juce::MouseEvent&e)override{juce::TextButton::mouseUp(e);wasDragged=false;}
    void clicked()override{if(wasDragged)return;juce::TextButton::clicked();}
private:
    int dragStartX=0,dragAccum=0;bool wasDragged=false;
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

class Cell final : public juce::Component,private juce::Timer {
public:
    explicit Cell(MonomachineNovaAudioProcessor& p):processor(p){
        slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);slider.setTextBoxStyle(juce::Slider::NoTextBox,false,0,0);
        slider.setScrollWheelEnabled(false);addAndMakeVisible(slider);startTimerHz(20);
    }
    void bind(const juce::String& parameterId,const juce::String& name,int page=-1,int index=-1,int engine=-1){
        attachment.reset();id=parameterId;label=name;pageIndex=page;knob=index;machine=engine;
        parameter=id.isEmpty()?nullptr:processor.parameters.getParameter(id);
        isChoice=parameter&&(parameter->getAllValueStrings().size()>0||(pageIndex>=3&&knob==1));
        slider.setVisible(parameter&&!isChoice);setMouseCursor(parameter?juce::MouseCursor::PointingHandCursor:juce::MouseCursor::NormalCursor);
        if(parameter&&!isChoice){attachment=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.parameters,id,slider);slider.setDoubleClickReturnValue(true,parameter->getDefaultValue());} // double-click restores the stored default
        repaint();
    }
    juce::String displayedValue()const{
        if(!parameter)return "---";float v=parameter->convertFrom0to1(parameter->getValue());int raw=juce::roundToInt(v);
        if(pageIndex>=3&&knob==1){int page=juce::roundToInt(processor.parameters.getRawParameterValue(nova::pageParam(pageIndex,0))->load());
            if(page==0){const char* ranges[]{"1ST","2ST","3ST","5ST","7ST","1OCT","2OCT","3OCT"};return ranges[juce::jlimit(0,7,raw)];}
            if(page==1)return juce::String(processor.machineIndex()>=0?nova::machines()[static_cast<size_t>(processor.machineIndex())].synthParams[static_cast<size_t>(juce::jlimit(0,7,raw))].name:"---");
            return nova::pageLabel(page-2,juce::jlimit(0,7,raw));}
        if(isChoice)return parameter->getText(parameter->getValue(),32);
        if(machine==7&&knob==3){auto* mode=processor.parameters.getParameter("m7_2");if(mode&&mode->convertFrom0to1(mode->getValue())>0.5f){
            const char* osc[]{"KICK","SNAR","HAT","TOM","CLAP","RIM","TONE","NOIZ","METL","ZAP"};return osc[juce::jlimit(0,9,raw)];}}
        if(machine==10&&knob==0)return juce::String(v/16,1);
        if(machine==10&&knob==4)return juce::String(std::pow(2.0f,(v-32)/24),2);
        if((machine==8||machine==9)&&(label=="1FRQ"||label=="2FRQ"||label=="3FRQ"))return juce::String(monomachine::getFmListedRatio(static_cast<uint8_t>(raw/4)),2);
        if(label=="TUNE"&&machine!=17)return juce::String(raw-64);
        // DIST (AMP page) and DSND (FX page) are bipolar on hardware: centre 0, -64..+63.
        if((pageIndex==0&&knob==4)||(pageIndex==2&&knob==4))return juce::String(raw-64);
        if((machine==10&&knob==1)||(machine==8&&knob==1)||(pageIndex==0&&knob==6)||(pageIndex==1&&knob>=6)||(pageIndex==2&&knob==1)||(pageIndex>=3&&knob==7))return juce::String(raw-64);
        if(pageIndex==0&&knob==1&&raw==127)return "INF";
        return juce::String(raw);
    }
    void paint(juce::Graphics& g)override{
        g.setColour(parameter?ink:dim);pixel::text(g,label,{3,3,getWidth()-6,23},21,true);
        if(isChoice){int cx=getWidth()/2,cy=49;
            if(pageIndex>=3&&knob==3){int type=juce::roundToInt(parameter->convertFrom0to1(parameter->getValue()));
                int lastY=cy;
                for(int i=0;i<24;++i){float t=i/23.0f;float v=0;
                    if(type==4){v=((i/6)%2)?-0.7f:0.7f;v*=1.0f-((i%6)/6.0f)*0.4f;}
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
    // Press-drag is the primary way to swap lists (8 px per entry); a plain click
    // still opens the popup list as a bonus.
    void mouseDown(const juce::MouseEvent&e)override{
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
        if(gestureOpen){gestureOpen=false;parameter->endChangeGesture();}
        if(!parameter||!isChoice||dragMoved){dragMoved=false;return;}
        juce::PopupMenu menu;const int n=choiceCount(parameter);
        const auto names=parameter->getAllValueStrings();
        for(int i=0;i<n;++i)menu.addItem(i+1,names.size()>i?names[i]:parameter->getText(hostFor(*parameter,i),32),true,i==juce::roundToInt(parameter->convertFrom0to1(parameter->getValue())));
        const juce::Component::SafePointer<Cell> safe(this);
        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(this),[safe](int result){if(safe&&result>0&&safe->parameter){safe->parameter->beginChangeGesture();safe->parameter->setValueNotifyingHost(hostFor(*safe->parameter,result-1));safe->parameter->endChangeGesture();}});
    }
private:
    void timerCallback()override{repaint();}
    MonomachineNovaAudioProcessor& processor;juce::String id,label;int pageIndex=-1,knob=-1,machine=-1;bool isChoice=false;
    int dragOriginY=0,dragBaseIndex=0;bool dragMoved=false,gestureOpen=false;
    juce::RangedAudioParameter* parameter=nullptr;juce::Slider slider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;
};

// Global/tempo/arp list kept for the MENU page, restyled for the black screen.
class SettingsPage final : public juce::Component {
public:
    SettingsPage(MonomachineNovaAudioProcessor& p,bool matrix):processor(p){
        if(matrix)return; // the matrix has its own page below
        std::vector<juce::String> ids={"bpm","host_sync","arp_mode","arp_play","arp_speed","arp_range","arp_length","macro_x","macro_y"};
        if(!p.isSynthVersion){ids.push_back("fx_mix");ids.push_back("gate");}
        for(const auto& id:ids){auto* parameter=p.parameters.getParameter(id);if(!parameter)continue;
            auto row=std::make_unique<Row>();row->label.setText(parameter->getName(128),juce::dontSendNotification);row->label.setColour(juce::Label::textColourId,ink);
            addAndMakeVisible(row->label);auto choices=parameter->getAllValueStrings();
            if(!choices.isEmpty()){row->combo.addItemList(choices,1);addAndMakeVisible(row->combo);row->comboLink=std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(p.parameters,id,row->combo);}
            else {row->slider.setSliderStyle(juce::Slider::LinearHorizontal);row->slider.setTextBoxStyle(juce::Slider::TextBoxRight,false,70,24);
                row->slider.setColour(juce::Slider::trackColourId,ink);row->slider.setColour(juce::Slider::thumbColourId,ink);row->slider.setColour(juce::Slider::rotarySliderFillColourId,ink);addAndMakeVisible(row->slider);
                row->sliderLink=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters,id,row->slider);}
            rows.push_back(std::move(row));}
        setSize(1120,static_cast<int>((rows.size()+1)/2)*62+20);
    }
    void paint(juce::Graphics& g)override{g.fillAll(juce::Colours::black);g.setColour(ink);pixel::text(g,"TEMPO / ARPEGGIATOR / GLOBAL",{10,4,700,22},18);}
    void resized()override{for(size_t i=0;i<rows.size();++i){int x=static_cast<int>(i%2)*550+10,y=static_cast<int>(i/2)*62+32;rows[i]->label.setBounds(x,y,515,25);rows[i]->combo.setBounds(x,y+26,500,27);rows[i]->slider.setBounds(x,y+26,500,27);}}
private:
    struct Row{juce::Label label;juce::ComboBox combo;juce::Slider slider;std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> comboLink;std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderLink;};
    MonomachineNovaAudioProcessor& processor;std::vector<std::unique_ptr<Row>> rows;
};

// The modulation matrix as a real screen page (8 routes x on/src/dest/depth),
// drawn in the same pixel language; cells change by drag, popup on click.
class MatrixPage final : public juce::Component,private juce::Timer {
public:
    explicit MatrixPage(MonomachineNovaAudioProcessor& p):processor(p){
        const char* fields[]{"on","src","dest","depth"};
        for(int r=0;r<8;++r)for(int c=0;c<4;++c)cells[static_cast<size_t>(r*4+c)]=p.parameters.getParameter("r"+juce::String(r)+"_"+fields[c]);
        startTimerHz(15);
    }
    ~MatrixPage()override{stopTimer();}
    void paint(juce::Graphics& g)override{
        g.fillAll(juce::Colours::black);g.setColour(ink);
        pixel::text(g,"MODULATION MATRIX - 8 ROUTES",{8,2,800,22},18);
        const char* headers[]{"ROUTE","ON","SOURCE","DESTINATION","DEPTH +/-64"};
        for(int c=0;c<5;++c)pixel::text(g,headers[c],{colX(c),28,colWidth(c),20},14,true);
        for(int r=0;r<8;++r){
            pixel::text(g,"ROUTE "+juce::String(r+1),{colX(0),rowY(r),colWidth(0),24},18,true);
            for(int c=1;c<=4;++c){auto* par=cell(r,c);if(!par){pixel::text(g,"---",{colX(c),rowY(r),colWidth(c),24},18,true);continue;}
                const int v=juce::roundToInt(par->convertFrom0to1(par->getValue()));
                juce::String text;
                if(c==1)text=v>0?"ON":"OFF";
                else if(c==2)text=sourceName(v);
                else if(c==3){const char* sections[]{"AMP","FLT","FX","L1","L2","L3"};text=v<8?"SYN "+juce::String(v+1):juce::String(sections[juce::jlimit(0,5,(v-8)/8)])+" "+nova::pageLabel((v-8)/8,(v-8)%8);}
                else text=juce::String(v-64);
                pixel::text(g,text,{colX(c),rowY(r),colWidth(c),24},18,true);}
            for(int c=1;c<=4;++c)pixel::dotted(g,colX(c)-2,rowY(r)-4,colX(c)-2,rowY(r)+30);
        }
        pixel::dotted(g,6,52,getWidth()-10,52);
        g.setColour(dim);pixel::text(g,"drag a field to change it - click for the list - ON toggles on click",{8,getHeight()-24,1100,18},14);
    }
    void mouseDown(const juce::MouseEvent&e)override{
        int r=rowAt(e.y),c=colAt(e.x);if(r<0||c<1)return;
        if(c==1){auto* par=cell(r,1);if(par){toggle(*par);}return;}
        hitRow=r;hitCol=c;hitY=static_cast<int>(e.getScreenPosition().y);hitBase=cell(r,c)?juce::roundToInt(cell(r,c)->convertFrom0to1(cell(r,c)->getValue())):0;dragged=false;
        if(auto* par=cell(r,c)){gesture=true;par->beginChangeGesture();}
    }
    void mouseDrag(const juce::MouseEvent&e)override{
        if(hitRow<0)return;auto* par=cell(hitRow,hitCol);if(!par)return;
        const int dy=hitY-static_cast<int>(e.getScreenPosition().y);if(std::abs(dy)>3)dragged=true;
        const int steps=dy/10;
        if(hitCol==4)par->setValueNotifyingHost(par->convertTo0to1(static_cast<float>(std::clamp(hitBase+steps,-64,63))));
        else{const int next=std::clamp(hitBase+steps,0,std::max(1,choiceCount(par)-1));par->setValueNotifyingHost(hostFor(*par,next));}
    }
    void mouseUp(const juce::MouseEvent&)override{
        if(gesture&&cell(hitRow,hitCol)){cell(hitRow,hitCol)->endChangeGesture();gesture=false;}
        const bool wasDrag=dragged;int r=hitRow,c=hitCol;hitRow=-1;dragged=false;
        if(wasDrag||c<2||r<0)return;
        auto* par=cell(r,c);if(!par)return;
        const int n=choiceCount(par);if(n<2)return;
        juce::PopupMenu menu;const auto names=par->getAllValueStrings();
        const int selected=juce::roundToInt(par->convertFrom0to1(par->getValue()));
        for(int i=0;i<n;++i)menu.addItem(i+1,names.size()>i?names[i]:juce::String(i),true,i==selected);
        const juce::Component::SafePointer<MatrixPage> safe(this);
        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(this),[safe,r,c](int result){if(!safe||result<=0)return;auto* p=safe->cell(r,c);if(!p)return;
            p->beginChangeGesture();p->setValueNotifyingHost(hostFor(*p,result-1));p->endChangeGesture();});
    }
private:
    static int choiceCount(juce::RangedAudioParameter* p){if(!p)return 0;const auto names=p->getAllValueStrings();if(names.size()>0)return names.size();return juce::roundToInt(p->convertFrom0to1(1.0f))+1;}
    static juce::String sourceName(int v){const char* names[]{"KEY","VEL","MAC X","MAC Y","LFO1","LFO2","LFO3"};return names[juce::jlimit(0,6,v)];}
    int colX(int c)const{return 10+c*228;}
    int colWidth(int c)const{return c==0?150:220;}
    int rowY(int r)const{return 58+r*36;}
    int colAt(int x)const{int c=(x-10)/228;return c>=1&&c<=4?c:0;}
    int rowAt(int y)const{int r=(y-40)/36;return r>=0&&r<8?r:-1;}
    juce::RangedAudioParameter* cell(int r,int c){return cells[static_cast<size_t>(r*4+(c-1))];}
    void toggle(juce::RangedAudioParameter& p){p.beginChangeGesture();p.setValueNotifyingHost(p.convertTo0to1(p.convertFrom0to1(p.getValue())>0.5f?0.0f:1.0f));p.endChangeGesture();}
    void timerCallback()override{repaint();}
    MonomachineNovaAudioProcessor& processor;
    std::array<juce::RangedAudioParameter*,32> cells{};
    int hitRow=-1,hitCol=-1,hitY=0,hitBase=0;bool gesture=false,dragged=false;
};
}

struct MonomachineNovaAudioProcessorEditor::Surface final : juce::Component {
    explicit Surface(MonomachineNovaAudioProcessor& p):processor(p),machineButton(""),menuButton("MENU"),tempoButton(""),arpButton("ARP"),matrixButton("matrix"),lfoButton("LFO2 [LFO3]"),closeButton("BACK"),gateButton(""){
        setLookAndFeel(&theme);
        for(auto& panel:cells)for(auto& cell:panel){cell=std::make_unique<Cell>(p);addAndMakeVisible(*cell);}
        for(auto* button:{&machineButton,&menuButton,&tempoButton,&arpButton,&matrixButton,&lfoButton})addAndMakeVisible(*button);
        level.setSliderStyle(juce::Slider::LinearVertical);level.setTextBoxStyle(juce::Slider::NoTextBox,false,0,0);
        level.setColour(juce::Slider::trackColourId,ink);level.setColour(juce::Slider::thumbColourId,ink);level.setColour(juce::Slider::backgroundColourId,juce::Colours::black);addAndMakeVisible(level);
        levelLink=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters,"level",level);
        machineButton.textAlign=1;
        machineButton.onClick=[this]{showMachineMenu();};
        machineButton.dragHandler=[this](int steps){cycleMachine(steps);};
        menuButton.onClick=[this]{showMenu();};
        // BPM drags to change tempo; it no longer hijacks a settings page.
        tempoButton.textHeight=35;tempoButton.outlined=false;tempoButton.textAlign=1;
        tempoButton.dragHandler=[this](int steps){nudgeBpm(static_cast<float>(steps)*0.1f);};
        arpButton.textHeight=22;arpButton.onClick=[this]{toggleArp();};
        matrixButton.textHeight=22;matrixButton.onClick=[this]{showSettings(true);};
        lfoButton.textHeight=28;
        lfoButton.onClick=[this]{showLfo3=!showLfo3;bind();repaint();};lfoButton.setTooltip("Click to switch between LFO2 and LFO3; all three run independently.");
        closeButton.textHeight=20;closeButton.onClick=[this]{viewport.setViewedComponent(nullptr);settings.reset();matrixPage.reset();viewport.setVisible(false);closeButton.setVisible(false);repaint();};
        addChildComponent(viewport);addChildComponent(closeButton);
        if(!processor.isSynthVersion){addAndMakeVisible(gateButton);gateButton.textHeight=18;gateButton.outlined=false;gateButton.textAlign=1;
            gateButton.onClick=[this]{toggleGate();};updateGateLabel();}
        else gateButton.setVisible(false);
        setSize(1260,584);bind();
    }
    ~Surface()override{viewport.setViewedComponent(nullptr);setLookAndFeel(nullptr);}
    void bind(){
        boundMachine=processor.machineIndex();const auto& m=nova::machines()[static_cast<size_t>(boundMachine)];machineButton.setButtonText(m.name+"  >");machineButton.textHeight=28;
        for(int i=0;i<8;++i){const auto& def=m.synthParams[static_cast<size_t>(i)];cells[0][static_cast<size_t>(i)]->bind(def.maxVal?nova::machineParam(m.id,i):juce::String(),def.name,-1,i,m.id);}
        const int pageForPanel[]={-1,0,3,1,2,showLfo3?5:4};
        for(int panel=1;panel<6;++panel)for(int i=0;i<8;++i){int page=pageForPanel[panel];cells[static_cast<size_t>(panel)][static_cast<size_t>(i)]->bind(nova::pageParam(page,i),nova::pageLabel(page,i),page,i);}
        lfoButton.setButtonText(showLfo3?"LFO3 [LFO2]":"LFO2 [LFO3]");
        if(!processor.isSynthVersion)updateGateLabel();
        repaint();
    }
    void update(){
        if(boundMachine!=processor.machineIndex())bind();
        tempoButton.setButtonText(juce::String(processor.currentBpm(),1));
        if(!processor.isSynthVersion)updateGateLabel();
        repaint();
    }
    void paint(juce::Graphics& g)override{
        g.fillAll(juce::Colours::black);g.setColour(ink);
        // Original geometric mark, drawn locally (not the Elektron logo), in white.
        for(int i=0;i<5;++i){float angle=i*2*nova::pi/5;g.fillRect(25+juce::roundToInt(12*std::sin(angle)),22+juce::roundToInt(12*std::cos(angle)),8,8);}g.fillRect(24,20,13,13);
        pixel::text(g,"MONOMACHINE NOVA",{66,0,380,37},21);pixel::text(g,processor.isSynthVersion?"SYNTH / NATIVE APPROXIMATION":"FX / NATIVE APPROXIMATION",{66,36,600,18},14);
        pixel::text(g,"BPM",{853,0,96,38},28);pixel::text(g,"LEV",{7,56,58,32},28);
        pixel::dotted(g,8,94,8,558);pixel::dotted(g,61,94,61,558);pixel::dotted(g,8,94,63,94);pixel::dotted(g,8,558,63,558);
        const char* names[]{"SYNTHESIS","AMPLIFICATION","LFO1","FILTER","EFFECTS",""};
        for(int panel=0;panel<6;++panel){int x=72+(panel%3)*399,y=94+(panel/3)*240;
            g.fillRect(x,y,387,30);g.setColour(knockout);pixel::text(g,names[panel],{x+5,y,377,30},28);
            g.setColour(ink);for(int c=0;c<=4;++c)pixel::dotted(g,x+c*96,y+34,x+c*96,y+228);
            pixel::dotted(g,x,y+34,x+388,y+34);pixel::dotted(g,x,y+130,x+388,y+130);pixel::dotted(g,x,y+228,x+388,y+228);}
        pixel::text(g,"V1.3.0 / UNIFIED",{8,562,460,18},14);
        pixel::text(g,processor.outputPeak()>1?"OVER - LOWER LEV":"INDEPENDENT NATIVE DSP",{processor.isSynthVersion?882:700,562,processor.isSynthVersion?371:170,18},14);
    }
    void resized()override{
        machineButton.setBounds(72,58,387,30);menuButton.setBounds(1161,4,94,32);tempoButton.setBounds(947,0,140,38);
        arpButton.setBounds(471,58,180,30);matrixButton.setBounds(870,58,140,30);
        lfoButton.setBounds(870,334,387,30);
        level.setBounds(12,97,46,458);
        for(int panel=0;panel<6;++panel)for(int i=0;i<8;++i)cells[static_cast<size_t>(panel)][static_cast<size_t>(i)]->setBounds(75+(panel%3)*399+(i%4)*96,130+(panel/3)*240+(i/4)*96,91,91);
        viewport.setBounds(65,88,1150,470);closeButton.setBounds(1070,51,145,32);gateButton.setBounds(902,558,250,26);
    }
    void setViewportMode(bool matrix){
        if(matrix){settings.reset();matrixPage=std::make_unique<MatrixPage>(processor);viewport.setViewedComponent(matrixPage.get(),false);}
        else{matrixPage.reset();settings=std::make_unique<SettingsPage>(processor,false);viewport.setViewedComponent(settings.get(),false);}
        viewport.setVisible(true);viewport.toFront(false);closeButton.setVisible(true);closeButton.toFront(false);
    }
    void cycleMachine(int steps){
        auto* p=processor.parameters.getParameter("machine");if(!p)return;
        const int n=static_cast<int>(nova::machines().size());
        int raw=juce::roundToInt(p->convertFrom0to1(p->getValue()));raw=((raw+steps)%n+n)%n;
        p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(raw)));p->endChangeGesture();bind();
    }
    void nudgeBpm(float delta){
        auto* p=processor.parameters.getParameter("bpm");if(!p)return;
        const float v=juce::jlimit(30.0f,300.0f,p->convertFrom0to1(p->getValue())+delta);
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
        gateButton.setButtonText(midi?"FX GATE: MIDI NOTE":"FX GATE: FREE RUN");
        gateButton.setTooltip(midi?"FX is strictly triggered/gated by incoming MIDI notes (amp envelope active).":"FX runs continuously without any envelope, like a normal insert.");
    }
    void showMachineMenu(){juce::PopupMenu menu;juce::String category;
        for(size_t i=0;i<nova::machines().size();++i){const auto& m=nova::machines()[i];if(category!=juce::String(m.category)){category=m.category;menu.addSectionHeader(category);}
            auto shortName=juce::String(m.name);if(shortName.startsWith(m.category))shortName=shortName.substring(static_cast<int>(m.category.size())).trimCharactersAtStart("- ");
            menu.addItem(static_cast<int>(i)+1,shortName,true,static_cast<int>(i)==boundMachine);}
        menu.addItem(static_cast<int>(nova::machines().size())+1,"ARP MODE: CYCLE OFF/KEY/SID/ADD");
        juce::Component::SafePointer<Surface> safe(this);
        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(&machineButton).withMinimumWidth(320),[safe](int result){if(!safe||result<=0)return;
            if(result>static_cast<int>(nova::machines().size())){auto* a=safe->processor.parameters.getParameter("arp_mode");if(a){const int n=static_cast<int>(a->convertFrom0to1(a->getValue()));
                a->beginChangeGesture();a->setValueNotifyingHost(a->convertTo0to1(static_cast<float>((n+1)%4)));a->endChangeGesture();}return;}
            auto* p=safe->processor.parameters.getParameter("machine");p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(result-1)));p->endChangeGesture();safe->bind();});
    }
    void showSettings(bool matrix){setViewportMode(matrix);}
    void showMenu(){juce::PopupMenu menu;menu.addItem(1,"TEMPO / ARPEGGIATOR / GLOBAL");menu.addItem(2,"MODULATION MATRIX (8 ROUTES)");
        if(processor.isSynthVersion){juce::PopupMenu samples;for(int i=0;i<10;++i)samples.addItem(100+i,"LOAD SLOT "+juce::String(i+1)+" : "+processor.sampleName(i));samples.addSeparator();samples.addItem(3,"RESTORE SYNTHETIC KIT");menu.addSubMenu("MOD SAMPLES (WAV / AIFF)",samples);}
        menu.addSeparator();menu.addItem(4,"PANIC / CLEAR TAILS");menu.addItem(5,"RESET ALL PARAMETERS");menu.addItem(6,"ABOUT / LIMITATIONS");
        juce::Component::SafePointer<Surface> safe(this);menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(&menuButton),[safe](int result){if(!safe)return;
            if(result==1||result==2)safe->setViewportMode(result==2);
            if(result==3)safe->processor.resetSamples();if(result==4)safe->processor.requestPanic();
            if(result==5){safe->processor.requestPanic();for(auto* parameter:safe->processor.getParameters()){parameter->beginChangeGesture();parameter->setValueNotifyingHost(parameter->getDefaultValue());parameter->endChangeGesture();}safe->bind();}
            if(result==6)juce::AlertWindow::showMessageBoxAsync(juce::MessageBoxIconType::InfoIcon,"MonomachineNova Unified","Independent native approximations; not an exact emulator.\nFX-CHORUS runs the translated OS 1.32B core (24-bit fixed point, DEL/DEP/SPD/MIX/FB/WID/LP/INP).\nINP is input gain everywhere, DIST/DSND are bipolar, DSND<0 pings across.\nFX build: GATE OFF = free running, GATE ON = strict MIDI note triggering.\nAHD: HOLD=127 sustains. MENU contains arp and 8 modulation routes.\nMOD (DPRO) synthesises drum OSCs on top of 10 sample slots; custom PCM is embedded in DAW state.\nLFO/ARP use host BPM but are note-synchronised, not timeline-locked.\nNo oversampling or output limiter. Turn down LEV before testing.");
            if(result>=100&&result<110)safe->chooseSample(result-100);
        });
    }
    void chooseSample(int slot){chooser=std::make_unique<juce::FileChooser>("Load MOD slot "+juce::String(slot+1),juce::File(),"*.wav;*.aif;*.aiff");
        juce::Component::SafePointer<Surface> safe(this);chooser->launchAsync(juce::FileBrowserComponent::openMode|juce::FileBrowserComponent::canSelectFiles,[safe,slot](const juce::FileChooser& fc){if(!safe)return;auto file=fc.getResult();if(!file.existsAsFile())return;
            auto error=safe->processor.loadSample(slot,file);if(error.isNotEmpty())juce::AlertWindow::showMessageBoxAsync(juce::MessageBoxIconType::WarningIcon,"Sample load",error);
        });
    }
    MonomachineNovaAudioProcessor& processor;PixelTheme theme;
    std::array<std::array<std::unique_ptr<Cell>,8>,6> cells;
    PixelButton machineButton,menuButton,tempoButton,arpButton,matrixButton,lfoButton,closeButton,gateButton;juce::Slider level;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> levelLink;
    std::unique_ptr<SettingsPage> settings;std::unique_ptr<MatrixPage> matrixPage;juce::Viewport viewport;std::unique_ptr<juce::FileChooser> chooser;
    juce::TooltipWindow tooltips{this,800};bool showLfo3=false;int boundMachine=-1;
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
