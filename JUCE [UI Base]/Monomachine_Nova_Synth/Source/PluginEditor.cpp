#include "PluginEditor.h"
#include "PixelFont.h"

namespace {
class PixelTheme final : public juce::LookAndFeel_V4 {
public:
    PixelTheme(){setColour(juce::PopupMenu::backgroundColourId,juce::Colour(0xff303c42));setColour(juce::PopupMenu::textColourId,juce::Colours::white);
        setColour(juce::PopupMenu::highlightedBackgroundColourId,juce::Colours::white);setColour(juce::PopupMenu::highlightedTextColourId,juce::Colours::black);
        setColour(juce::ComboBox::backgroundColourId,juce::Colours::white);setColour(juce::ComboBox::textColourId,juce::Colours::black);
        setColour(juce::Slider::textBoxTextColourId,juce::Colours::black);setColour(juce::Slider::textBoxOutlineColourId,juce::Colours::black);
        setColour(juce::TextButton::buttonColourId,juce::Colours::black);setColour(juce::TextButton::textColourOffId,juce::Colours::white);
    }
    void drawLinearSlider(juce::Graphics& g,int x,int y,int width,int height,float pos,float min,float max,const juce::Slider::SliderStyle style,juce::Slider& slider)override{
        if(style==juce::Slider::LinearVertical){g.setColour(juce::Colours::black);g.fillRect(x+width/2-9,juce::roundToInt(pos),18,std::max(1,y+height-juce::roundToInt(pos)));}
        else juce::LookAndFeel_V4::drawLinearSlider(g,x,y,width,height,pos,min,max,style,slider);
    }
    void drawRotarySlider(juce::Graphics& g,int x,int y,int width,int height,float pos,float start,float end,juce::Slider&)override{
        int cx=x+width/2,cy=y+height/2;g.setColour(juce::Colours::black);
        for(int i=0;i<16;++i){float a=2*nova::pi*i/16;int px=cx+juce::roundToInt(std::sin(a)*14),py=cy+juce::roundToInt(std::cos(a)*14);g.fillRect(px-1,py-1,3,3);}
        const float angle=start+pos*(end-start);
        for(int i=3;i<=11;++i)g.fillRect(cx+juce::roundToInt(std::sin(angle)*i)-1,cy-juce::roundToInt(std::cos(angle)*i)-1,3,3);
    }
};
class PixelButton final : public juce::TextButton {
public:
    explicit PixelButton(const juce::String& s):juce::TextButton(s){}
    int textHeight=28;
    void paintButton(juce::Graphics& g,bool over,bool down)override{
        g.fillAll(juce::Colours::black);g.setColour(juce::Colours::white);
        if(over||down)g.drawRect(getLocalBounds().reduced(2),1);
        pixel::text(g,getButtonText(),getLocalBounds().reduced(5,0),textHeight,false);
    }
};
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
        if(parameter&&!isChoice){attachment=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.parameters,id,slider);slider.setDoubleClickReturnValue(true,parameter->convertFrom0to1(parameter->getDefaultValue()));}
        repaint();
    }
    juce::String displayedValue()const{
        if(!parameter)return "---";float v=parameter->convertFrom0to1(parameter->getValue());int raw=juce::roundToInt(v);
        if(pageIndex>=3&&knob==1){int page=juce::roundToInt(processor.parameters.getRawParameterValue(nova::pageParam(pageIndex,0))->load());
            if(page==0){const char* ranges[]={"1ST","2ST","3ST","5ST","7ST","1OCT","2OCT","3OCT"};return ranges[juce::jlimit(0,7,raw)];}
            if(page==1)return juce::String(processor.machineIndex()>=0?nova::machines()[static_cast<size_t>(processor.machineIndex())].synthParams[static_cast<size_t>(juce::jlimit(0,7,raw))].name:"---");
            return nova::pageLabel(page-2,juce::jlimit(0,7,raw));}
        if(isChoice)return parameter->getText(parameter->getValue(),32);
        if(machine==10&&knob==0)return juce::String(v/16,1);
        if(machine==10&&knob==4)return juce::String(std::pow(2.0f,(v-32)/24),2);
        if((machine==8||machine==9)&&(label=="1FRQ"||label=="2FRQ"||label=="3FRQ"))return juce::String(monomachine::getFmListedRatio(static_cast<uint8_t>(raw/4)),2);
        if(label=="TUNE"&&machine!=17)return juce::String(raw-64);
        if((machine==10&&knob==1)||(machine==8&&knob==1)||(pageIndex==0&&knob==6)||(pageIndex==1&&knob>=6)||(pageIndex==2&&knob==1)||(pageIndex>=3&&knob==7))return juce::String(raw-64);
        if(pageIndex==0&&knob==1&&raw==127)return "INF";
        return juce::String(raw);
    }
    void paint(juce::Graphics& g)override{
        g.setColour(parameter?juce::Colours::black:juce::Colour(0xff999999));pixel::text(g,label,{3,3,getWidth()-6,23},21,true);
        if(isChoice){int cx=getWidth()/2,cy=49;
            if(pageIndex>=3&&knob==3){int type=juce::roundToInt(parameter->convertFrom0to1(parameter->getValue()));
                int lastX=cx-23,lastY=cy;
                for(int i=0;i<24;++i){float t=i/23.0f;float v=0;
                    if(type==0)v=1-4*std::abs(t-0.5f);else if(type==1)v=2*t-1;else if(type==2)v=t<0.5f?1.0f:-1.0f;else if(type==3)v=2*std::exp(-5*t)-1;else v=std::sin(static_cast<float>(i*13))*0.85f;
                    int xx=cx-23+i*2,yy=cy-juce::roundToInt(v*13);g.fillRect(xx,yy,3,3);
                    if(i>0)g.fillRect(xx,std::min(yy,lastY),2,std::max(2,std::abs(yy-lastY)));lastX=xx;lastY=yy;}juce::ignoreUnused(lastX);
            }else if(pageIndex>=3&&(knob==2||knob==4)){
                for(int i=0;i<16;++i){float a=2*nova::pi*i/16;g.fillRect(cx+juce::roundToInt(std::sin(a)*14)-1,cy+juce::roundToInt(std::cos(a)*14)-1,3,3);}
                float a=3.9f+parameter->getValue()*4.7f;for(int i=3;i<12;++i)g.fillRect(cx+juce::roundToInt(std::sin(a)*i)-1,cy-juce::roundToInt(std::cos(a)*i)-1,3,3);
            }else {g.drawRect(cx-17,cy-16,34,30,3);g.fillRect(cx-11,cy-10,5,8);g.fillRect(cx-4,cy-8,15,3);g.fillRect(cx-4,cy-1,15,3);g.fillRect(cx-4,cy+6,15,3);}
        }
        pixel::text(g,displayedValue(),{2,getHeight()-26,getWidth()-4,23},21,true);
    }
    void resized()override{slider.setBounds(getWidth()/2-26,27,52,43);}
    void mouseUp(const juce::MouseEvent&)override{
        if(!parameter||!isChoice)return;juce::PopupMenu menu;juce::StringArray names=parameter->getAllValueStrings();
        if(names.isEmpty()){for(int i=0;i<8;++i)names.add(juce::String(i+1));}
        int selected=juce::roundToInt(parameter->convertFrom0to1(parameter->getValue()));
        for(int i=0;i<names.size();++i)menu.addItem(i+1,names[i],true,i==selected);
        const juce::Component::SafePointer<Cell> safe(this);
        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(this),[safe](int result){if(safe&&result>0&&safe->parameter){safe->parameter->beginChangeGesture();safe->parameter->setValueNotifyingHost(safe->parameter->convertTo0to1(static_cast<float>(result-1)));safe->parameter->endChangeGesture();}});
    }
private:
    void timerCallback()override{repaint();}
    MonomachineNovaAudioProcessor& processor;juce::String id,label;int pageIndex=-1,knob=-1,machine=-1;bool isChoice=false;
    juce::RangedAudioParameter* parameter=nullptr;juce::Slider slider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;
};

class SettingsPage final : public juce::Component {
public:
    SettingsPage(MonomachineNovaAudioProcessor& p,bool matrix):processor(p){
        std::vector<juce::String> ids;
        if(matrix){for(int r=0;r<8;++r)for(const char* f:{"on","src","dest","depth"})ids.push_back("r"+juce::String(r)+"_"+f);}
        else {ids={"bpm","host_sync","arp_mode","arp_play","arp_speed","arp_range","arp_length","macro_x","macro_y"};if(!p.isSynthVersion){ids.push_back("fx_mix");ids.push_back("gate");}}
        for(const auto& id:ids){auto* parameter=p.parameters.getParameter(id);if(!parameter)continue;
            auto row=std::make_unique<Row>();row->label.setText(parameter->getName(64),juce::dontSendNotification);row->label.setColour(juce::Label::textColourId,juce::Colours::black);
            if(id.startsWith("r"))row->label.setText("ROUTE "+juce::String(id.substring(1,2).getIntValue()+1)+" / "+id.fromFirstOccurrenceOf("_",false,false).toUpperCase(),juce::dontSendNotification);
            addAndMakeVisible(row->label);auto choices=parameter->getAllValueStrings();
            if(!choices.isEmpty()){row->combo.addItemList(choices,1);addAndMakeVisible(row->combo);row->comboLink=std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(p.parameters,id,row->combo);}
            else {row->slider.setSliderStyle(juce::Slider::LinearHorizontal);row->slider.setTextBoxStyle(juce::Slider::TextBoxRight,false,70,24);row->slider.setColour(juce::Slider::trackColourId,juce::Colours::black);addAndMakeVisible(row->slider);row->sliderLink=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters,id,row->slider);}
            rows.push_back(std::move(row));}
        setSize(1120,static_cast<int>((rows.size()+1)/2)*62+20);
    }
    void paint(juce::Graphics& g)override{g.fillAll(juce::Colours::white);}
    void resized()override{for(size_t i=0;i<rows.size();++i){int x=static_cast<int>(i%2)*550+10,y=static_cast<int>(i/2)*62;rows[i]->label.setBounds(x,y,515,25);rows[i]->combo.setBounds(x,y+26,500,27);rows[i]->slider.setBounds(x,y+26,500,27);}}
private:
    struct Row{juce::Label label;juce::ComboBox combo;juce::Slider slider;std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> comboLink;std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderLink;};
    MonomachineNovaAudioProcessor& processor;std::vector<std::unique_ptr<Row>> rows;
};
}

struct MonomachineNovaAudioProcessorEditor::Surface final : juce::Component {
    explicit Surface(MonomachineNovaAudioProcessor& p):processor(p),machineButton(""),menuButton("MENU"),tempoButton("120.0"),lfoButton("LFO2 [LFO3]"),closeButton("BACK"){
        setLookAndFeel(&theme);
        for(auto& panel:cells)for(auto& cell:panel){cell=std::make_unique<Cell>(p);addAndMakeVisible(*cell);}
        for(auto* button:{&machineButton,&menuButton,&tempoButton,&lfoButton})addAndMakeVisible(*button);
        level.setSliderStyle(juce::Slider::LinearVertical);level.setTextBoxStyle(juce::Slider::NoTextBox,false,0,0);level.setColour(juce::Slider::trackColourId,juce::Colours::black);level.setColour(juce::Slider::thumbColourId,juce::Colours::black);addAndMakeVisible(level);
        levelLink=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters,"level",level);
        machineButton.onClick=[this]{showMachineMenu();};menuButton.onClick=[this]{showMenu();};tempoButton.onClick=[this]{showSettings(false);};
        lfoButton.onClick=[this]{showLfo3=!showLfo3;bind();};lfoButton.setTooltip("Click to switch between LFO2 and LFO3; all three run independently.");
        closeButton.onClick=[this]{viewport.setViewedComponent(nullptr);settings.reset();viewport.setVisible(false);closeButton.setVisible(false);repaint();};
        addChildComponent(viewport);addChildComponent(closeButton);
        setSize(1260,584);bind();
    }
    ~Surface()override{viewport.setViewedComponent(nullptr);setLookAndFeel(nullptr);}
    void bind(){
        boundMachine=processor.machineIndex();const auto& m=nova::machines()[static_cast<size_t>(boundMachine)];machineButton.setButtonText(m.name+"  >");machineButton.textHeight=28;
        for(int i=0;i<8;++i){const auto& def=m.synthParams[static_cast<size_t>(i)];cells[0][static_cast<size_t>(i)]->bind(def.maxVal?nova::machineParam(m.id,i):juce::String(),def.name,-1,i,m.id);}
        const int pageForPanel[]={-1,0,3,1,2,showLfo3?5:4};
        for(int panel=1;panel<6;++panel)for(int i=0;i<8;++i){int page=pageForPanel[panel];cells[static_cast<size_t>(panel)][static_cast<size_t>(i)]->bind(nova::pageParam(page,i),nova::pageLabel(page,i),page,i);}
        lfoButton.setButtonText(showLfo3?"LFO3 [LFO2]":"LFO2 [LFO3]");repaint();
    }
    void update(){if(boundMachine!=processor.machineIndex())bind();tempoButton.setButtonText(juce::String(processor.currentBpm(),1));repaint();}
    void paint(juce::Graphics& g)override{
        g.fillAll(juce::Colours::white);g.setColour(juce::Colours::black);
        // Original geometric mark, drawn locally (not the Elektron logo).
        for(int i=0;i<5;++i){float angle=i*2*nova::pi/5;g.fillRect(25+juce::roundToInt(12*std::sin(angle)),22+juce::roundToInt(12*std::cos(angle)),8,8);}g.fillRect(24,20,13,13);
        pixel::text(g,"MONOMACHINE NOVA",{66,0,380,37},21);pixel::text(g,processor.isSynthVersion?"SYNTH / NATIVE APPROXIMATION":"FX / NATIVE APPROXIMATION",{66,36,600,18},14);
        pixel::text(g,"BPM",{901,0,101,38},28);pixel::text(g,"LEV",{7,56,58,32},28);
        pixel::dotted(g,8,94,8,558);pixel::dotted(g,61,94,61,558);pixel::dotted(g,8,94,63,94);pixel::dotted(g,8,558,63,558);
        const char* names[]={"SYNTHESIS","AMPLIFICATION","LFO1","FILTER","EFFECTS",""};
        for(int panel=0;panel<6;++panel){int x=72+(panel%3)*399,y=94+(panel/3)*240;
            g.fillRect(x,y,387,30);g.setColour(juce::Colours::white);pixel::text(g,names[panel],{x+5,y,377,30},28);
            g.setColour(juce::Colours::black);for(int c=0;c<=4;++c)pixel::dotted(g,x+c*96,y+34,x+c*96,y+228);
            pixel::dotted(g,x,y+34,x+388,y+34);pixel::dotted(g,x,y+130,x+388,y+130);pixel::dotted(g,x,y+228,x+388,y+228);}
        pixel::text(g,"V1.2.0 / UNIFIED",{8,562,460,18},14);
        pixel::text(g,processor.outputPeak()>1?"OVER - LOWER LEV":"INDEPENDENT NATIVE DSP",{882,562,371,18},14);
    }
    void resized()override{
        machineButton.setBounds(72,58,387,30);menuButton.setBounds(1161,4,94,32);tempoButton.setBounds(1013,0,140,38);tempoButton.textHeight=35;
        level.setBounds(12,97,46,458);lfoButton.setBounds(870,334,387,30);
        for(int panel=0;panel<6;++panel)for(int i=0;i<8;++i)cells[static_cast<size_t>(panel)][static_cast<size_t>(i)]->setBounds(75+(panel%3)*399+(i%4)*96,130+(panel/3)*240+(i/4)*96,91,91);
        viewport.setBounds(65,88,1150,470);closeButton.setBounds(1070,51,145,32);
    }
    void showMachineMenu(){juce::PopupMenu menu;juce::String category;
        for(size_t i=0;i<nova::machines().size();++i){const auto& m=nova::machines()[i];if(category!=juce::String(m.category)){category=m.category;menu.addSectionHeader(category);}
            auto shortName=juce::String(m.name);if(shortName.startsWith(m.category))shortName=shortName.substring(static_cast<int>(m.category.size())).trimCharactersAtStart("- ");
            menu.addItem(static_cast<int>(i)+1,shortName,true,static_cast<int>(i)==boundMachine);}
        juce::Component::SafePointer<Surface> safe(this);
        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(&machineButton).withMinimumWidth(320),[safe](int result){if(safe&&result>0){auto* p=safe->processor.parameters.getParameter("machine");p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(result-1)));p->endChangeGesture();safe->bind();}});
    }
    void showSettings(bool matrix){viewport.setViewedComponent(nullptr);settings=std::make_unique<SettingsPage>(processor,matrix);viewport.setViewedComponent(settings.get(),false);viewport.setVisible(true);viewport.toFront(false);closeButton.setVisible(true);closeButton.toFront(false);}
    void showMenu(){juce::PopupMenu menu;menu.addItem(1,"TEMPO / ARPEGGIATOR / GLOBAL");menu.addItem(2,"MODULATION MATRIX (8 ROUTES)");
        if(processor.isSynthVersion){juce::PopupMenu samples;for(int i=0;i<10;++i)samples.addItem(100+i,"LOAD SLOT "+juce::String(i+1)+" : "+processor.sampleName(i));samples.addSeparator();samples.addItem(3,"RESTORE SYNTHETIC KIT");menu.addSubMenu("BBOX SAMPLES (WAV / AIFF)",samples);}
        menu.addSeparator();menu.addItem(4,"PANIC / CLEAR TAILS");menu.addItem(5,"RESET ALL PARAMETERS");menu.addItem(6,"ABOUT / LIMITATIONS");
        juce::Component::SafePointer<Surface> safe(this);menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(&menuButton),[safe](int result){if(!safe)return;
            if(result==1||result==2)safe->showSettings(result==2);
            if(result==3)safe->processor.resetSamples();if(result==4)safe->processor.requestPanic();
            if(result==5){safe->processor.requestPanic();for(auto* parameter:safe->processor.getParameters()){parameter->beginChangeGesture();parameter->setValueNotifyingHost(parameter->getDefaultValue());parameter->endChangeGesture();}safe->bind();}
            if(result==6)juce::AlertWindow::showMessageBoxAsync(juce::MessageBoxIconType::InfoIcon,"MonomachineNova Unified","Independent native approximations; not an exact Elektron emulator.\n15 synth modes + 7 FX modes in separate plugins.\nAHD: HOLD=127 sustains. MENU contains arp and 8 modulation routes.\nBBOX loads the first 5 seconds, mono at 44.1 kHz; custom PCM is embedded in DAW state.\nLFO/ARP use host BPM but are note-synchronised, not timeline-locked.\nNo oversampling or output limiter. Turn down LEV before testing.");
            if(result>=100&&result<110)safe->chooseSample(result-100);
        });
    }
    void chooseSample(int slot){chooser=std::make_unique<juce::FileChooser>("Load BBOX slot "+juce::String(slot+1),juce::File(),"*.wav;*.aif;*.aiff");
        juce::Component::SafePointer<Surface> safe(this);chooser->launchAsync(juce::FileBrowserComponent::openMode|juce::FileBrowserComponent::canSelectFiles,[safe,slot](const juce::FileChooser& fc){if(!safe)return;auto file=fc.getResult();if(!file.existsAsFile())return;
            auto error=safe->processor.loadSample(slot,file);if(error.isNotEmpty())juce::AlertWindow::showMessageBoxAsync(juce::MessageBoxIconType::WarningIcon,"Sample load",error);
        });
    }
    MonomachineNovaAudioProcessor& processor;PixelTheme theme;
    std::array<std::array<std::unique_ptr<Cell>,8>,6> cells;
    PixelButton machineButton,menuButton,tempoButton,lfoButton,closeButton;juce::Slider level;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> levelLink;
    std::unique_ptr<SettingsPage> settings;juce::Viewport viewport;std::unique_ptr<juce::FileChooser> chooser;
    juce::TooltipWindow tooltips{this,800};bool showLfo3=false;int boundMachine=-1;
};
MonomachineNovaAudioProcessorEditor::MonomachineNovaAudioProcessorEditor(MonomachineNovaAudioProcessor& p):AudioProcessorEditor(&p),processor(p){
    surface=std::make_unique<Surface>(p);addAndMakeVisible(*surface);setResizable(true,true);setResizeLimits(1008,468,1890,876);getConstrainer()->setFixedAspectRatio(1260.0/584.0);setSize(1260,584);startTimerHz(20);
}
MonomachineNovaAudioProcessorEditor::~MonomachineNovaAudioProcessorEditor(){stopTimer();}
void MonomachineNovaAudioProcessorEditor::paint(juce::Graphics& g){g.fillAll(juce::Colours::white);}
void MonomachineNovaAudioProcessorEditor::resized(){if(surface){surface->setTopLeftPosition(0,0);surface->setTransform(juce::AffineTransform::scale(static_cast<float>(getWidth())/1260.0f,static_cast<float>(getHeight())/584.0f));}}
void MonomachineNovaAudioProcessorEditor::timerCallback(){surface->update();}
