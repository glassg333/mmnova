#include "PluginEditor.h"
#include "PixelFont.h"

#include <map>
#include <cmath>

// v1.3 UI pass, all per the black reference screen: white ink on black, outlined boxes,
// list knobs change by press-and-drag (the popup stays as a bonus), BPM drags instead of
// opening settings, the matrix gets its own pixel page, and the FX build carries a
// MIDI/FREE gate toggle right on the main screen.
namespace {
const juce::Colour ink=juce::Colours::white;      // foreground on black, like the LCD
const juce::Colour knockout=juce::Colours::black; // ink knocked out of filled bars
const juce::Colour dim=juce::Colour(0xff737373);

// 1.6.13: плавность прокрутки GUI-списков, своё значение на каждый контрол.
// Специально НЕ параметры VST (лимит автоматизируемых параметров не тратим):
// живут только в редакторе, диапазон вокруг дефолта подбирается вручную
// на странице MENU > TEMPO / ARPEGGIATOR / GLOBAL (секция GUI DRAG SPEED).
static std::map<juce::String,double>& uiSpeedMap(){ static std::map<juce::String,double> m; return m; }
static double uiSpeed(const juce::String& key,double def=1.0){ const auto& m=uiSpeedMap(); auto it=m.find(key); return it==m.end()?def:juce::jlimit(0.1,6.0,it->second); }
struct SpeedDef { const char* key; const char* label; double def; };
static const SpeedDef kSpeedDefs[]{
    {"MACHINE","MACHINE",0.4},{"BPM","BPM",1.0},{"ARP RATE","ARP RATE",1.0},{"KNOB LISTS","KNOB LISTS",1.0},
    {"VALUE BOXES","VALUE BOXES",1.0},{"MODE SYNT","SYNT MODE",1.0},{"MODE FILT","FILT MODE",1.0},
    {"MODE DIST","DIST MODE",1.0},{"MODE DLY","DLY MODE",1.0},{"ARP MODE","ARP PLAY",1.0},{"ARP VEL","ARP VEL",1.0},{"ARP PAGE","ARP PAGE",1.0}}; // 1.6.15: MODE AMP убран (комбо удалено)

class PixelLabel final : public juce::Label {
public:
    void paint(juce::Graphics& g) override { g.fillAll(juce::Colours::black); if (!isBeingEdited()) { g.setColour(ink); pixel::text(g, getText(), getLocalBounds().reduced(2, 0), 14, false); } } // 1.6.14: надписи всегда белые
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
    void drawButtonBackground(juce::Graphics& g, juce::Button& b, const juce::Colour&, bool hover, bool down) override { const bool filled = b.getToggleState() || down; g.fillAll(filled ? ink : juce::Colours::black); g.setColour(ink); g.drawRect(b.getLocalBounds().reduced(1), hover ? 2 : 1); } // 1.6.14: рамки белые
    void drawButtonText(juce::Graphics& g, juce::TextButton& b, bool, bool down) override { g.setColour(b.getToggleState() || down ? knockout : ink); pixel::text(g, b.getButtonText(), b.getLocalBounds().reduced(4, 0), 14, true); } // 1.6.14: названия кнопок всегда белые
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
    // 1.6.13: чёрный пиксельный скин для ВСЕХ popup-меню. 1.6.16: шрифт КРУПНЕЕ
    // (до 21 px, авто-усадка только у очень длинных строк) и ширина по пиксельным
    // метрикам -- меню читаются и не уезжают за экран.
    void drawPopupMenuBackgroundWithOptions(juce::Graphics& g,int width,int height,const juce::PopupMenu::Options&)override{
        g.fillAll(juce::Colours::black);g.setColour(ink);g.drawRect(0,0,width-1,height-1);
    }
    static int pixelScaleFor(const juce::String& s){ int scale=3; while(scale>1&&s.length()*6*scale>700)--scale; return scale; }
    void getIdealPopupMenuItemSizeWithOptions(const juce::String& text,bool isSeparator,int,int& idealWidth,int& idealHeight,const juce::PopupMenu::Options&)override{
        if(isSeparator){idealWidth=40;idealHeight=12;return;}
        const int scale=pixelScaleFor(text);
        idealWidth=juce::jlimit(120,760,text.length()*6*scale+28);
        idealHeight=scale>=3?30:scale==2?24:18;
    }
    void drawPopupMenuSectionHeaderWithOptions(juce::Graphics& g,const juce::Rectangle<int>& area,const juce::String& sectionName,const juce::PopupMenu::Options&)override{
        g.fillAll(juce::Colours::black);g.setColour(ink);pixel::text(g,sectionName,area.reduced(8,0),21,false);
    }
    void drawPopupMenuItemWithOptions(juce::Graphics& g,const juce::Rectangle<int>& area,bool highlighted,const juce::PopupMenu::Item& item,const juce::PopupMenu::Options&)override{
        g.fillAll(juce::Colours::black);
        if(item.isEnabled&&highlighted){g.setColour(ink);g.fillRect(area.reduced(2,0));}
        g.setColour(highlighted&&item.isEnabled?knockout:ink); // 1.6.16: всё белым (читаемость)
        pixel::text(g,item.text,area.reduced(10,0),21,item.isEnabled);
    }
    void drawScrollbar(juce::Graphics& g,juce::ScrollBar&,int x,int y,int width,int height,bool isVertical,int thumbStart,int thumbSize,bool,bool)override{
        g.fillAll(juce::Colours::black); // 1.6.13: скроллбар -- белый на чёрном, не синий
        g.setColour(ink.withAlpha(0.35f));g.drawRect(x,y,width,height,1);
        if(thumbSize<=0)return;
        g.setColour(ink);
        if(isVertical)g.fillRect(x+2,thumbStart,width-4,thumbSize);
        else g.fillRect(thumbStart,y+2,thumbSize,height-4);
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
    // 1.6.13: стрелки по бокам названия (шире влево/вправо = один шаг пресета).
    bool arrows=false;std::function<void(int)> arrowClick; // -1 = влево, +1 = вправо
    std::function<void(int,bool)> dragHandler;std::function<void()> doubleClickHandler;int dragPixelsPerStep=6; // 1.6.12: bool = мелкий шаг (Alt/Shift)
    int arrowSpan() const { // конец текста по правилам pixel::text (для хитбоксов стрелок)
        int scale=std::max(1,textHeight/7);const auto s=getButtonText().toUpperCase();
        while(scale>1&&s.length()*6*scale>getWidth()-10)--scale;
        const int w=s.length()*6*scale-scale;
        return (textAlign==0?(getWidth()-w)/2:5)+w;
    }
    void mouseDoubleClick(const juce::MouseEvent& e)override{if(doubleClickHandler)doubleClickHandler();else juce::TextButton::mouseDoubleClick(e);}
    void mouseWheelMove(const juce::MouseEvent& e,const juce::MouseWheelDetails& w)override{if(dragHandler&&std::abs(w.deltaY)>0.0001f)dragHandler(w.deltaY>0?1:-1,false);else juce::TextButton::mouseWheelMove(e,w);}
    std::function<void()> rightClick; // 1.6.14: ПКМ-хук (например, кнопка ARP -> страница ARP)
    void paintButton(juce::Graphics& g,bool /*over*/,bool down)override{
        g.fillAll(down&&outlined?ink:juce::Colours::black);
        if(outlined){g.setColour(ink);g.drawRect(getLocalBounds().reduced(1),1);}
        g.setColour(down&&outlined?knockout:ink);
        pixel::text(g,getButtonText(),getLocalBounds().reduced(5,0),textHeight,textAlign==0);
        if(arrows){ // 1.6.14: пара "< >" справа от текста (где была декоративная стрелка)
            const int end=arrowSpan(),cy=getHeight()/2;
            for(int i=0;i<5;++i){
                const int w=std::max(1,10-2*i);
                g.fillRect(end+6+2*i,cy-i,w,1);g.fillRect(end+6+2*i,cy+i,w,1); // "<" (влево)
                g.fillRect(end+22,cy-i,w,1);g.fillRect(end+22,cy+i,w,1);       // ">" (вправо)
            }
        }
    }
    void mouseDown(const juce::MouseEvent&e)override{
        if(rightClick&&e.mods.isPopupMenu()){rightClick();return;}
        if(arrows&&arrowClick){const int end=arrowSpan();
            if(e.x>=end+20&&e.x<=end+36){arrowClick(1);return;}  // ">": один шаг вперёд
            if(e.x>=end+4&&e.x<=end+20){arrowClick(-1);return;}  // "<": один шаг назад
        }
        dragStartX=e.x;dragAccum=0;wasDragged=false;juce::TextButton::mouseDown(e);
    }
    void mouseDrag(const juce::MouseEvent&e)override{
        if(!dragHandler)return;const int dx=e.x-dragStartX;const int pixels=std::max(1,dragPixelsPerStep);
        // 1.6.12: с Alt или Shift -- мелкий шаг (десятые), без -- целые.
        const bool fine=e.mods.isAltDown()||e.mods.isShiftDown();
        const int scale=fine?6:1;
        if(std::abs(dx)>=pixels){wasDragged=true;const int steps=(dx-dragAccum)/(pixels*scale);if(steps!=0){dragAccum+=steps*pixels*scale;dragHandler(steps,fine);}}
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

// 1.6.5/1.6.8: одинаковые кнопки LFO1/LFO2/LFO3. Клик -- выбрать страницу LFO.
// 1.6.8: ЛКМ-перетаскивание = прямой внутренний адресат (PAGE/DEST самого LFO,
// «прицел», без матрицы); ПКМ-перетаскивание = маршрут в матрицу.
class ModSourceButton final : public PixelButton, private juce::Timer {
public:
    explicit ModSourceButton(const juce::String& s):PixelButton(s){}
    std::function<void(bool)> beginDrag;                            // bool = drag into matrix (RMB)
    std::function<void(juce::Point<int>)> dragMove;
    std::function<void(juce::Point<int>,bool,bool)> endDrag;        // screen, dragged, matrix
    std::function<void()> holdHover;                                // 1.6.13: секунда удержания
    void mouseDown(const juce::MouseEvent& e) override { PixelButton::mouseDown(e); dragged=false; rightDrag=e.mods.isRightButtonDown(); if(beginDrag) beginDrag(rightDrag); }
    void mouseDrag(const juce::MouseEvent& e) override { dragged=true; if(dragMove) dragMove(e.getScreenPosition()); }
    void mouseUp(const juce::MouseEvent& e) override { juce::TextButton::mouseUp(e); if(endDrag) endDrag(e.getScreenPosition(),dragged,rightDrag); dragged=false; stopTimer(); }
    // 1.6.13: в режиме прицела можно ЗАДЕРЖАТЬСЯ над страницей другого LFO --
    // через секунду удержания панель переключается на этот LFO (LFO -> LFO).
    void mouseEnter(const juce::MouseEvent&) override { enterAt=juce::Time::getMillisecondCounter(); startTimerHz(10); }
    void mouseExit(const juce::MouseEvent&) override { stopTimer(); }
private:
    void timerCallback() override { if(holdHover&&juce::Time::getMillisecondCounter()-enterAt>=1000){stopTimer();holdHover();} }
    bool dragged=false,rightDrag=false; juce::uint32 enterAt=0;
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
    // 1.6.8: the callbacks receive the cursor position so popups can open right at it.
    std::function<void(juce::Point<int>)> curveMenu;std::function<void(juce::Point<int>)> modeMenu;bool popup=false;
    void mouseDown(const juce::MouseEvent& e)override{
        if(e.mods.isPopupMenu()){popup=true;const auto at=e.getScreenPosition();if(curveMenu)curveMenu(at);else if(modeMenu)modeMenu(at);return;}
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
    std::function<void(int,bool,juce::Point<int>)> openLocks; // 1.6.12: замки PAGE/DEST LFO
    std::function<void()> openSamples;std::function<void(int)> openEnvelope;std::function<void(int,juce::Point<int>)> openModes;std::function<void(uint8_t)> addMsegModulation;std::function<void(uint8_t)> pickTarget;std::function<void(juce::Point<int>)> openRepitch;std::function<void(juce::Point<int>)> openDsnd;std::function<void(juce::Point<int>)> openPorta; // 1.6.14: PORT -- окно скорости
    void setPickMode(bool enabled){pickMode=enabled&&modTarget;slider.setVisible(parameter&&!isChoice&&!pickMode);repaint();}
    // 1.6.13: подсветка ТОЛЬКО текущей ячейки прицела -- шлейфа на пройденных
    // ручках больше нет (раньше каждая ячейка вспыхивала и гасла ~секунду).
    void setHover(bool h){ if(hoverNow!=h){ hoverNow=h; repaint(); } }
    bool isTarget(uint8_t target) const noexcept { return modulationTarget() == target; }
    // 1.6.5: ячейки быстрых настроек MSEG не являются целями модуляции.
    bool canBeTarget() const noexcept { return modTarget && parameter != nullptr; }
    uint8_t targetId() const noexcept { return modulationTarget(); }
    void flashPick(){flashTicks=6;repaint();} // 1.6.12: короткая вспышка вместо секундного шлейфа
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
    void bind(const juce::String& parameterId,const juce::String& name,int page=-1,int index=-1,int engine=-1,bool modTargetEnabled=true){
        // 1.6.14: та же ячейка -- не перестраиваем (мгновенное переключение страниц LFO).
        if(id==parameterId&&label==name&&pageIndex==page&&knob==index&&machine==engine&&modTarget==modTargetEnabled&&parameter==processor.parameters.getParameter(id==juce::String()?juce::String(" "):id))return;
        attachment.reset();id=parameterId;label=name;pageIndex=page;knob=index;machine=engine;modTarget=modTargetEnabled;
        parameter=id.isEmpty()?nullptr:processor.parameters.getParameter(id);
        slider.curveMenu={};if(pageIndex==0&&(knob==0||knob==1||knob==2||knob==3))slider.curveMenu=[this](juce::Point<int>){if(openEnvelope)openEnvelope(knob);}; // 1.6.13: HOLD тоже огибающая
        // RMB elsewhere opens the DSP mode list of the section this knob belongs to
        // (prompt: "по пкм крутилка можно сделать переключение режима"). 1.6.8: the
        // popup spawns at the cursor, not at the DSP MODE button (SRR complaint).
        slider.modeMenu={};
        {
            int section=-1;
            if(machine>=0&&pageIndex<0)section=monomachine::DspSynt;
            else if(pageIndex==0)section=monomachine::DspAmp;
            else if(pageIndex==1)section=monomachine::DspFilter;
            else if(pageIndex==2)section=(knob>=3?monomachine::DspDelay:monomachine::DspDist);
            if(section>=0&&!slider.curveMenu)slider.modeMenu=[this,section](juce::Point<int> at){if(openModes)openModes(section,at);};
        }
        // 1.6.13: RMB on DTIM -- repitch-панель (как в пред. версии), RMB on DSND --
        // СВОЁ окно с режимом ping-pong (PP MODE больше не живёт в окне DTIM).
        if(pageIndex==2&&knob==3)slider.modeMenu=[this](juce::Point<int> at){if(openRepitch)openRepitch(at);};
        if(pageIndex==2&&knob==4)slider.modeMenu=[this](juce::Point<int> at){if(openDsnd)openDsnd(at);};
        if(pageIndex==0&&knob==7)slider.modeMenu=[this](juce::Point<int> at){if(openPorta)openPorta(at);}; // 1.6.14: PORT -- своё окно скорости
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
        if(pickMode){g.setColour(hoverNow?ink:ink.withAlpha(0.28f));const int th=(hoverNow?3:2);g.drawRect(getLocalBounds().reduced(2),th);if(hoverNow){g.drawLine(float(getWidth()/2-8),float(getHeight()/2),float(getWidth()/2+8),float(getHeight()/2),1.0f);g.drawLine(float(getWidth()/2),float(getHeight()/2-8),float(getWidth()/2),float(getHeight()/2+8),1.0f);}}
        else if(flashTicks>0){g.setColour(ink);g.drawRect(getLocalBounds().reduced(2),3);}
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
        if(pickMode&&parameter&&modTarget){if(pickTarget)pickTarget(modulationTarget());return;}
        // 1.6.12: ПКМ по PAGE/DEST страницы LFO -- панель замков этой ручки.
        if(pageIndex>=3&&(knob==0||knob==1)&&e.mods.isPopupMenu()){popupHandled=true;if(openLocks)openLocks(pageIndex-3,knob==1,e.getScreenPosition());return;}
        if(pageIndex==0&&(knob==0||knob==1||knob==2||knob==3)&&e.mods.isPopupMenu()){popupHandled=true;if(openEnvelope)openEnvelope(knob);return;} // 1.6.13: HOLD тоже огибающая
        if(machine==7&&knob==2&&e.mods.isPopupMenu()){popupHandled=true;if(openSamples)openSamples();return;}
        if(pageIndex==2&&knob==3&&e.mods.isPopupMenu()){popupHandled=true;if(openRepitch)openRepitch(e.getScreenPosition());return;} // DTIM: repitch
        if(pageIndex==2&&knob==4&&e.mods.isPopupMenu()){popupHandled=true;if(openDsnd)openDsnd(e.getScreenPosition());return;} // 1.6.13: DSND -- своё окно
        if(!parameter)return;
        if(isChoice){dragOriginY=static_cast<int>(e.getScreenPosition().y);dragBaseIndex=juce::roundToInt(parameter->convertFrom0to1(parameter->getValue()));dragMoved=false;gestureOpen=true;parameter->beginChangeGesture();}
    }
    void mouseDrag(const juce::MouseEvent&e)override{
        if(!parameter||!isChoice)return;
        const int dy=dragOriginY-static_cast<int>(e.getScreenPosition().y);
        if(std::abs(dy)>3)dragMoved=true;
        const int value=std::clamp(dragBaseIndex+juce::roundToInt(dy/8.0*uiSpeed("KNOB LISTS")),0,choiceCount(parameter)-1); // 1.6.13: плавность из GUI DRAG SPEED
        parameter->setValueNotifyingHost(hostFor(*parameter,value));
    }
    void mouseUp(const juce::MouseEvent&)override{
        if(popupHandled){popupHandled=false;return;}
        if(gestureOpen){gestureOpen=false;parameter->endChangeGesture();}
        if(!parameter||!isChoice||dragMoved){dragMoved=false;return;}
        juce::PopupMenu menu;menu.setLookAndFeel(&getLookAndFeel()); // 1.6.14: чёрный скин
        const int n=choiceCount(parameter);
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
    MonomachineNovaAudioProcessor& processor;juce::String id,label;int pageIndex=-1,knob=-1,machine=-1,flashTicks=0;bool isChoice=false,modTarget=true;
    int dragOriginY=0,dragBaseIndex=0;bool dragMoved=false,gestureOpen=false,popupHandled=false,pickMode=false,hoverNow=false;
    juce::RangedAudioParameter* parameter=nullptr;CurveSlider slider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;
};

// 1.6.12: окошко со значением вместо слайдера: ЛКМ-драг вверх/вниз = +-1
// (8 px на шаг), колесо = +-1, двойной клик = значение по умолчанию.
class DragValueBox final : public juce::Component, public juce::SettableTooltipClient {
public:
    DragValueBox(MonomachineNovaAudioProcessor& p, const juce::String& id, double loV, double hiV, double defV)
        : processor(p), paramId(id), lo(loV), hi(hiV), def(defV) {
        if (auto* par = processor.parameters.getParameter(paramId)) value = juce::jlimit(lo, hi, static_cast<double>(par->convertFrom0to1(par->getValue())));
        setMouseCursor(juce::MouseCursor::UpDownResizeCursor);
    }
    double getValue() const { return value; }
    int displayOffset = 0; // 1.6.14: показываем value + offset (для 0-базных параметров)
    void refreshFromParam() { // 1.6.14: синхронизация, пока параметр крутит снаружи (арп/хост)
        if (auto* par = processor.parameters.getParameter(paramId)) { const double v = juce::jlimit(lo, hi, static_cast<double>(par->convertFrom0to1(par->getValue()))); if (v != value) { value = v; repaint(); } }
    }
    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::black); g.setColour(ink); g.drawRect(getLocalBounds(), 1);
        pixel::text(g, juce::String(juce::roundToInt(value) + displayOffset), {4, 0, getWidth() - 8, getHeight()}, 17, true);
    }
    const char* speedKey="VALUE BOXES"; // 1.6.13: свой множитель скорости прокрутки
    void mouseDown(const juce::MouseEvent& e) override { dragY = e.getScreenPosition().y; dragBase = value; if (auto* par = par0()) par->beginChangeGesture(); }
    void mouseDrag(const juce::MouseEvent& e) override { const int dy = dragY - e.getScreenPosition().y; setValue(dragBase + dy / 8.0 * uiSpeed(speedKey)); }
    void mouseUp(const juce::MouseEvent&) override { if (auto* par = par0()) par->endChangeGesture(); }
    void mouseWheelMove(const juce::MouseEvent&, const juce::MouseWheelDetails& w) override { if (std::abs(w.deltaY) > 0.0001f) { if (auto* par = par0()) par->beginChangeGesture(); setValue(value + (w.deltaY > 0 ? 1 : -1)); if (auto* par = par0()) par->endChangeGesture(); } }
    void mouseDoubleClick(const juce::MouseEvent&) override { if (auto* par = par0()) par->beginChangeGesture(); setValue(def); if (auto* par = par0()) par->endChangeGesture(); }
private:
    juce::RangedAudioParameter* par0() const { return processor.parameters.getParameter(paramId); }
    void setValue(double v) { value = juce::jlimit(lo, hi, v); if (auto* par = par0()) par->setValueNotifyingHost(par->convertTo0to1(static_cast<float>(value))); repaint(); }
    MonomachineNovaAudioProcessor& processor; juce::String paramId; double lo = 0, hi = 1, def = 0, value = 0, dragBase = 0; int dragY = 0;
};

class RepitchSliderPanel final : public juce::Component {
public:
    explicit RepitchSliderPanel(MonomachineNovaAudioProcessor& p) : processor(p) {
        addRow(rep, "dly_repitch", 0.0, 3.0, 0.01);
        addRow(smo, "dly_repitch_smooth", 0.0, 127.0, 1.0);
        setSize(320, 88); // 1.6.13: как в пред. версии -- только REPITCH и SMOOTH
    }
    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::black); g.setColour(ink);
        auto* rep=processor.parameters.getParameter("dly_repitch");
        auto* smo=processor.parameters.getParameter("dly_repitch_smooth");
        pixel::text(g,"REPITCH",{6,3,86,18},13,false);
        pixel::text(g,repitchText(rep),{320-140,3,140,18},13,true);
        pixel::text(g,"SMOOTH",{6,41,86,18},13,false);
        pixel::text(g,smo?juce::String(juce::roundToInt(smo->convertFrom0to1(smo->getValue()))):juce::String("---"),{320-140,41,140,18},13,true);
    }
    void resized() override { rep.setBounds(6,22,getWidth()-12,18); smo.setBounds(6,60,getWidth()-12,18); }
private:
    juce::Slider rep,smo;
    MonomachineNovaAudioProcessor& processor;
    void addRow(juce::Slider& s,const char* id,double lo,double hi,double step){
        s.setSliderStyle(juce::Slider::LinearHorizontal);
        s.setTextBoxStyle(juce::Slider::NoTextBox,false,0,0);
        s.setRange(lo,hi,step);
        s.setColour(juce::Slider::trackColourId,ink);
        s.setColour(juce::Slider::thumbColourId,ink);
        s.setColour(juce::Slider::backgroundColourId,ink.withAlpha(0.2f));
        if(auto* par=processor.parameters.getParameter(id))s.setValue(par->convertFrom0to1(par->getValue()),juce::dontSendNotification);
        s.onDragStart=[this,id]{if(auto* par=processor.parameters.getParameter(id))par->beginChangeGesture();};
        s.onValueChange=[this,&s,id]{if(auto* par=processor.parameters.getParameter(id))par->setValueNotifyingHost(par->convertTo0to1(static_cast<float>(s.getValue())));repaint();};
        s.onDragEnd=[this,id]{if(auto* par=processor.parameters.getParameter(id))par->endChangeGesture();};
        addAndMakeVisible(s);
    }
    static juce::String repitchText(juce::RangedAudioParameter* par){
        if(!par)return "---";
        const float v=par->convertFrom0to1(par->getValue());
        if(v<=0.005f)return "OFF";
        if(std::abs(v-1.0f)<0.005f)return "FAST";
        if(std::abs(v-2.0f)<0.005f)return "MED (default)";
        if(std::abs(v-3.0f)<0.005f)return "TAPE ~2.5 S";
        return juce::String(v,2);
    }
};

// 1.6.13: у DSND СВОЁ окно доп. настроек (не вписывались в окно DTIM):
// PP MODE -- CLASSIC (как в прошивке: моно в левую линию, всё в сайде) или
// MID SAFE (тапы смешиваются 65/35, середина не вычитается). Дефолт CLASSIC.
class DsndPanel final : public juce::Component {
public:
    explicit DsndPanel(MonomachineNovaAudioProcessor& p) : processor(p), pp(p, "dly_ppmode", 0.0, 1.0, 0.0) {
        addAndMakeVisible(pp); setSize(320, 96);
    }
    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::black); g.setColour(ink);
        pixel::text(g,"DSND / DELAY SEND",{6,3,240,18},13,false);
        pixel::text(g,"PP MODE",{6,36,86,18},13,false);
        auto* ppm=processor.parameters.getParameter("dly_ppmode");
        pixel::text(g,ppm?(juce::roundToInt(ppm->convertFrom0to1(ppm->getValue()))==1?"MID SAFE":"CLASSIC"):juce::String("---"),{124,58,190,26},13,true);
    }
    void resized() override { pp.setBounds(6,58,110,26); }
private:
    DragValueBox pp; // 0 = CLASSIC, 1 = MID SAFE (двойной клик = CLASSIC)
    MonomachineNovaAudioProcessor& processor;
};

// 1.6.14: окно портаменто (ПКМ по ручке PORT страницы AMP): TIME = сама ручка,
// SPEED = множитель скорости глиссандо (porta_speed). Без задержек.
class PortaPanel final : public juce::Component {
public:
    explicit PortaPanel(MonomachineNovaAudioProcessor& p) : processor(p) {
        addRow(time, "p0_7", 0.0, 127.0, 1.0);
        addRow(speed, "porta_speed", 0.0, 127.0, 1.0);
        setSize(320, 88);
    }
    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::black); g.setColour(ink);
        pixel::text(g,"PORTAMENTO",{6,3,160,18},13,false);
        auto* tm=processor.parameters.getParameter("p0_7");
        auto* sp=processor.parameters.getParameter("porta_speed");
        pixel::text(g,"TIME",{6,41,64,18},13,false);
        pixel::text(g,tm?juce::String(juce::roundToInt(tm->convertFrom0to1(tm->getValue()))):juce::String("---"),{320-140,41,140,18},13,true);
        pixel::text(g,"SPEED",{130,41,64,18},13,false);
        pixel::text(g,sp?juce::String(juce::roundToInt(sp->convertFrom0to1(sp->getValue()))):juce::String("---"),{320-70,41,70,18},13,true);
    }
    void resized() override { time.setBounds(6,22,getWidth()-12,18); speed.setBounds(6,60,0,0); speed.setVisible(false); }
private:
    juce::Slider time,speed;
    MonomachineNovaAudioProcessor& processor;
    void addRow(juce::Slider& s,const char* id,double lo,double hi,double step){
        s.setSliderStyle(juce::Slider::LinearHorizontal); s.setTextBoxStyle(juce::Slider::NoTextBox,false,0,0); s.setRange(lo,hi,step);
        s.setColour(juce::Slider::trackColourId,ink); s.setColour(juce::Slider::thumbColourId,ink); s.setColour(juce::Slider::backgroundColourId,ink.withAlpha(0.2f));
        if(auto* par=processor.parameters.getParameter(id))s.setValue(par->convertFrom0to1(par->getValue()),juce::dontSendNotification);
        s.onDragStart=[this,id]{if(auto* par=processor.parameters.getParameter(id))par->beginChangeGesture();};
        s.onValueChange=[this,&s,id]{if(auto* par=processor.parameters.getParameter(id))par->setValueNotifyingHost(par->convertTo0to1(static_cast<float>(s.getValue())));repaint();};
        s.onDragEnd=[this,id]{if(auto* par=processor.parameters.getParameter(id))par->endChangeGesture();};
        addAndMakeVisible(s);
    }
};
class DragChoice final : public juce::ComboBox {
public:
    DragChoice(){setScrollWheelEnabled(true);}
    const char* speedKey=""; // 1.6.13: свой множитель скорости прокрутки (GUI DRAG SPEED)
    void mouseDown(const juce::MouseEvent& e)override{origin=e.getPosition();initial=getSelectedItemIndex();dragged=false;}
    void mouseDrag(const juce::MouseEvent& e)override{int d=(e.x-origin.x)+(origin.y-e.y);if(std::abs(d)>4){dragged=true;setSelectedItemIndex(juce::jlimit(0,std::max(0,getNumItems()-1),initial+juce::roundToInt(d/6.0*uiSpeed(speedKey))),juce::sendNotificationSync);}}
    void mouseUp(const juce::MouseEvent&)override{if(!dragged)showPopup();}
private:juce::Point<int> origin;int initial=0;bool dragged=false;
};
// 1.6.12: ПКМ по PAGE/DEST страницы LFO открывает эту панель замков. Строка:
// замок (ЛКМ = исключить значение из модуляции, ПКМ = SOLO -- приколотить
// модуляцию ТОЛЬКО к этому значению) + имя значения (ЛКМ = выставить ручку).
// SOLO переживает ручные изменения: поменял руками -- SOLO следует за новым.
class LfoLockPanel final : public juce::Component {
public:
    LfoLockPanel(MonomachineNovaAudioProcessor& p,int lfoIndex,bool isDest):processor(p),lfo(lfoIndex),dest(isDest){
        const int page=3+lfo;
        auto* knobPar=processor.parameters.getParameter(nova::pageParam(page,isDest?1:0));
        juce::StringArray choices;
        if(!dest)choices=knobPar->getAllValueStrings();
        else{auto* pg=processor.parameters.getParameter(nova::pageParam(page,0));const int cur=juce::jlimit(0,7,juce::roundToInt(pg->convertFrom0to1(pg->getValue())));
            // 1.6.16: правильные имена адресатов: PTCH -> ручки текущей машины,
            // LFO-страницы -> с приставкой LFO1..3 (голое PAGE/DEST было непонятно).
            const int mi=processor.machineIndex();
            for(int k=0;k<8;++k){
                if(cur==0)choices.add(mi>=0?juce::String(nova::machines()[static_cast<size_t>(mi)].synthParams[static_cast<size_t>(k)].name):juce::String(k));
                else{juce::String lbl=nova::pageLabel(cur-1,k);if(cur-1>=4)lbl="LFO"+juce::String(cur-4)+" "+lbl;choices.add(lbl);}
            }}
        for(int k=0;k<8;++k)rowNames[static_cast<size_t>(k)]=choices[static_cast<int>(juce::jlimit(0,choices.size()-1,k))];
        setSize(220,30+8*24+34);
    }
    void paint(juce::Graphics& g)override{
        g.fillAll(juce::Colours::black);g.setColour(ink);
        pixel::text(g,juce::String("LFO")+juce::String(lfo+1)+(dest?" DEST LOCKS":" PAGE LOCKS"),{6,4,208,18},13,false);
        const int mask=maskValue(),solo=soloValue();
        for(int k=0;k<8;++k){
            const int y=30+k*24;const bool locked=((mask>>k)&1)!=0;const bool soloed=(solo-1)==k;
            drawLockMark(g,10,y+3,soloed?2:(locked?1:0));
            g.setColour(ink.withAlpha(0.25f));g.drawHorizontalLine(y+23,6.0f,214.0f);
            g.setColour(ink);
            pixel::text(g,rowNames[static_cast<size_t>(k)],{40,y+1,174,20},13,false);
            if(knobValue()==k)g.drawRect(36,y,178,24,1);
        }
        g.setColour(ink);pixel::text(g,"CLEAR LOCKS",{6,getHeight()-28,120,20},12,false);
        pixel::text(g,"LMB LOCK (drag to paint) / RMB SOLO / click value = set",{6,getHeight()-16,214,14},9,false); // 1.6.14: белая подсказка
    }
    // 1.6.13: замочки можно РИСОВАТЬ зажатием: первый ряд тумблится, дальше
    // проведённые ряды получают то же состояние (маской, одним жестом).
    void mouseDown(const juce::MouseEvent& e)override{
        if(e.y>=getHeight()-28&&e.y<getHeight()-8){auto set=[this](const juce::String& parId,float v){if(auto* p=processor.parameters.getParameter(parId)){p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(v));p->endChangeGesture();}};
            set(id("page_locks"),0.0f);set(id("dest_locks"),0.0f);set(id("page_solo"),0.0f);set(id("dest_solo"),0.0f);repaint();return;}
        if(e.x<34){const int row=(static_cast<int>(e.y)-30)/24;if(row<0||row>7)return;
            const bool soloHere=(soloValue()-1)==row;
            if(e.mods.isPopupMenu()){setSolo(soloHere?0:row+1);return;} // ПКМ по замку = SOLO
            const int mask=maskValue();const bool was=((mask>>row)&1)!=0;
            paintValue=was?0:1;lockPainting=true; // первый ряд тумблится, дальше -- рисуем
            setMask(was?mask&~(1<<row):mask|(1<<row));return;}
        const int row=(static_cast<int>(e.y)-30)/24;if(row<0||row>7)return;
        if(auto* par=processor.parameters.getParameter(nova::pageParam(3+lfo,dest?1:0))){
            par->beginChangeGesture();par->setValueNotifyingHost(par->convertTo0to1(static_cast<float>(row)));par->endChangeGesture();
            if(soloValue()>0)setSolo(row+1); // 1.6.12: SOLO следует за ручной сменой
        }
        if(auto* owner=findParentComponentOfClass<juce::CallOutBox>())owner->exitModalState(0);
    }
    void mouseDrag(const juce::MouseEvent& e)override{
        if(!lockPainting)return;
        const int row=(static_cast<int>(e.y)-30)/24;if(row<0||row>7)return;
        int mask=maskValue();const bool was=((mask>>row)&1)!=0;
        if((paintValue==1)==was){mask=paintValue==1?mask|(1<<row):mask&~(1<<row);setMask(mask);}
    }
    void mouseUp(const juce::MouseEvent&)override{lockPainting=false;}
private:
    juce::String id(const char* suffix)const{return juce::String("lfo")+juce::String(lfo+1)+"_"+juce::String(suffix);}
    int rawOf(const char* suffix)const{auto* p=processor.parameters.getParameter(id(suffix));return p?juce::roundToInt(p->convertFrom0to1(p->getValue())):0;}
    int maskValue()const{return rawOf(dest?"dest_locks":"page_locks");}
    int soloValue()const{return rawOf(dest?"dest_solo":"page_solo");}
    int knobValue()const{auto* p=processor.parameters.getParameter(nova::pageParam(3+lfo,dest?1:0));return p?juce::jlimit(0,7,juce::roundToInt(p->convertFrom0to1(p->getValue()))):0;}
    void setMask(int m){if(auto* p=processor.parameters.getParameter(id(dest?"dest_locks":"page_locks"))){p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(juce::jlimit(0,255,m))));p->endChangeGesture();}repaint();}
    void setSolo(int s){if(auto* p=processor.parameters.getParameter(id(dest?"dest_solo":"page_solo"))){p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(juce::jlimit(0,8,s))));p->endChangeGesture();}repaint();}
    static void drawLockMark(juce::Graphics& g,int x,int y,int mode){
        const float fx=float(x),fy=float(y);g.setColour(ink); // 1.6.14: замок всегда белый
        g.drawRect(fx+1.0f,fy+4.0f,14.0f,12.0f,1.0f);
        if(mode>0)g.fillRect(fx+5.0f,fy+8.0f,5.0f,4.0f);
        if(mode==2)g.drawVerticalLine(x+8,fy-1.0f,fy+4.0f);
        else{g.drawLine(fx+4.0f,fy+4.0f,fx+4.0f,fy+0.0f,1.0f);g.drawLine(fx+12.0f,fy+4.0f,fx+12.0f,fy+0.0f,1.0f);g.drawHorizontalLine(fy,fx+4.0f,fx+12.0f);}
    }
    MonomachineNovaAudioProcessor& processor;int lfo=0;bool dest=false;
    std::array<juce::String,8> rowNames{};bool lockPainting=false;int paintValue=0;
};

// 1.6.13: редактор множителя скорости прокрутки списка. Только UI: никаких
// VST-параметров (не тратим лимит автоматизации). Драг = +-0.05, колесо = +-0.05,
// двойной клик = значение по умолчанию.
class FloatBox final : public juce::Component {
public:
    FloatBox(const juce::String& key,double def):speedKey(key),defV(def){ value=uiSpeed(key,def); uiSpeedMap()[key]=value; setMouseCursor(juce::MouseCursor::UpDownResizeCursor); }
    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::black); g.setColour(ink); g.drawRect(getLocalBounds(),1);
        pixel::text(g,juce::String(value,2),{4,0,getWidth()-8,getHeight()},15,true);
    }
    // 1.6.15: драг начинается после 6 px мёртвой зоны и крутит значение МЯГКО
    // (48 px на x1, с Shift -- x0.1): случайный задев при скролле страницы
    // больше не швыряет множитель в крайние 0.10/6.00.
    void mouseDown(const juce::MouseEvent& e) override { dragY=e.getScreenPosition().y; dragBase=value; armed=false; }
    void mouseDrag(const juce::MouseEvent& e) override {
        const int dy=dragY-e.getScreenPosition().y;
        if(!armed){ if(std::abs(dy)<6)return; armed=true; }
        setValue(dragBase+dy/48.0*(e.mods.isShiftDown()?0.1:1.0));
    }
    void mouseWheelMove(const juce::MouseEvent&,const juce::MouseWheelDetails& w) override { if(std::abs(w.deltaY)>0.0001f) setValue(value+(w.deltaY>0?0.05:-0.05)); }
    void mouseDoubleClick(const juce::MouseEvent&) override { setValue(defV); }
    void refreshTo(double v){ value=juce::jlimit(0.1,6.0,v); uiSpeedMap()[speedKey]=value; repaint(); } // 1.6.15: для RESET ALL
private:
    void setValue(double v){ value=juce::jlimit(0.1,6.0,v); uiSpeedMap()[speedKey]=value; repaint(); }
    juce::String speedKey; double defV=1.0,value=1.0,dragBase=0; int dragY=0; bool armed=false;
};

// Global/tempo/arp list kept for the MENU page, restyled for the black screen.
class SettingsPage final : public juce::Component {
public:
    SettingsPage(MonomachineNovaAudioProcessor& p,bool matrix):processor(p){
        if(matrix)return; // the matrix has its own page below
        std::vector<juce::String> ids={"arp_on","arp_hold","host_sync","arp_range","arp_sync","arp_play","arp_grid","arp_length","arp_wrap","arp_velocity_mode","arp_step","arp_step_velocity","arp_step_transpose","arp_step_hold","arp_time","bpm","mseg_sync","mseg_loop","mseg_rate","dly_repitch","macro_x","macro_y"};
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
        // 1.6.13: GUI DRAG SPEED -- своя плавность прокрутки для каждого списка.
        for(const auto& def:kSpeedDefs){
            auto label=std::make_unique<PixelLabel>();label->setText(def.label,juce::dontSendNotification);
            label->setBounds(0,0,10,14);label->setJustificationType(juce::Justification::centredLeft);addAndMakeVisible(*label);
            auto box=std::make_unique<FloatBox>(def.key,def.def);addAndMakeVisible(*box);
            speedLabels.push_back(std::move(label));speedBoxes.push_back(std::move(box));}
        setSize(1120,static_cast<int>((rows.size()+1)/2)*70+160);
    }
    void paint(juce::Graphics& g)override{g.fillAll(juce::Colours::black);g.setColour(ink);
        pixel::text(g,"TEMPO / ARPEGGIATOR / GLOBAL",{10,4,700,22},18);
        pixel::text(g,"GUI DRAG SPEED (per list; drag/wheel, double-click = default; UI-only, not automatable)",{10,speedY()-18,900,16},12,false);}
    void resized()override{
        for(size_t i=0;i<rows.size();++i){int x=static_cast<int>(i%2)*550+10,y=static_cast<int>(i/2)*70+32;rows[i]->label.setBounds(x,y,515,25);rows[i]->combo.setBounds(x,y+26,500,27);rows[i]->slider.setBounds(x,y+26,500,27);rows[i]->toggle.setBounds(x,y+26,30,27);}
        for(size_t i=0;i<speedBoxes.size();++i){const int x=10+static_cast<int>(i%4)*280,y=speedY()+static_cast<int>(i/4)*50;speedLabels[i]->setBounds(x,y,130,16);speedBoxes[i]->setBounds(x,y+18,120,26);}}
private:
    int speedY() const { return static_cast<int>((rows.size()+1)/2)*70+52; }
    struct Row{juce::ToggleButton toggle;std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> buttonLink;juce::Label label;DragChoice combo;juce::Slider slider;std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> comboLink;std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderLink;};
    MonomachineNovaAudioProcessor& processor;std::vector<std::unique_ptr<Row>> rows;
    std::vector<std::unique_ptr<PixelLabel>> speedLabels;std::vector<std::unique_ptr<FloatBox>> speedBoxes;
};


// 1.6.6: RANDOM button with RMB = menu, LMB = instant randomize.
struct RndButton : juce::TextButton {
    std::function<void()> rmb;
    void mouseDown(const juce::MouseEvent& e) override { if (e.mods.isPopupMenu() && rmb) { rmb(); return; } juce::TextButton::mouseDown(e); }
};

// 1.6.8: step-sequencer pads instead of long thin faders. Two rows of 8 fat
// SQUARE pads (transpose on top, velocity below), pads sit close to each other
// like a classic step sequencer. Drag vertically on a pad to set its value,
// drag across pads to paint, RMB toggles hold.
class StepLane final : public juce::Component, public juce::SettableTooltipClient {
public:
    StepLane(MonomachineNovaAudioProcessor& p, std::function<int()> pageOf,
             std::function<void(int, int, const char*, float)> set)
        : processor(p), pageOf(std::move(pageOf)), setParam(std::move(set)) {}
    struct Geo { int pad = 0, gap = 0, x0 = 0, yTr = 0, yVel = 0, yHold = 0, holdH = 26; }; // 1.6.14: HOLD-ряд выше
    Geo geo() const {
        const int gap = 8, strip = 15, margin = 6, left = 36; // left = row captions TR/VEL/HOLD
        int pad = (getWidth() - left - margin - 7 * gap) / 8;
        const int byH = (getHeight() - margin * 2 - strip - gap * 2 - 26) / 2;
        pad = juce::jlimit(24, 240, std::min(pad, byH));
        Geo g; g.pad = pad; g.gap = gap;
        g.x0 = left + std::max(0, (getWidth() - left - margin - (pad * 8 + 7 * gap)) / 2);
        const int y0 = margin + std::max(0, (getHeight() - margin * 2 - strip - gap * 2 - 26 - (pad * 2 + gap)) / 2);
        g.yTr = y0; g.yVel = y0 + pad + gap; g.yHold = g.yVel + pad + gap;
        return g;
    }
    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::black);
        const int pg = juce::jlimit(0, 15, pageOf());
        const Geo geo = this->geo();
        g.setColour(ink.withAlpha(0.45f));
        pixel::text(g, "TR", {2, geo.yTr + geo.pad / 2 - 7, 30, 14}, 12, true);
        pixel::text(g, "VEL", {2, geo.yVel + geo.pad / 2 - 7, 30, 14}, 12, true);
        pixel::text(g, "HOLD", {2, geo.yHold, 34, 14}, 12, true);
        for (int i = 0; i < 8; ++i) {
            const auto pre = "arp_s" + juce::String(pg) + "_" + juce::String(i) + "_";
            const float tr = processor.parameters.getRawParameterValue(pre + "transpose")->load();
            const float ve = processor.parameters.getRawParameterValue(pre + "velocity")->load();
            const float ho = processor.parameters.getRawParameterValue(pre + "hold")->load();
            const int x = geo.x0 + i * (geo.pad + geo.gap);
            drawPad(g, x, geo.yTr, geo.pad, tr, true);
            drawPad(g, x, geo.yVel, geo.pad, ve, false);
            // 1.6.13: HOLD -- свой ряд кнопок; шапка над TR+VEL остаётся индикатором.
            g.setColour(ink.withAlpha(0.5f)); g.drawRect(x, geo.yHold, geo.pad, geo.holdH, 1);
            if (ho > 0.5f) {
                g.setColour(ink); g.fillRect(x + 2, geo.yHold + 2, geo.pad - 4, geo.holdH - 4);
                g.setColour(ink);
                g.fillRect(x, geo.yTr - 3, geo.pad, 3); g.fillRect(x, geo.yVel - 3, geo.pad, 3);
            }
            g.setColour(ink);
            pixel::text(g, juce::String(i + 1), {x, geo.yHold + geo.holdH + 1, geo.pad, 14}, 12, true);
        }
    }
    // 1.6.13: ЛКМ = рисовать значение по положению (TR: ноль в середине, биполярно;
    // VEL: однополярно, ноль ВНИЗУ, растёт вверх; HOLD-ряд: просто кнопки, красятся
    // зажатием). ПКМ-драг = СТИРАНИЕ velocity и transpose. Ряд фиксируется в mouseDown.
    void mouseDown(const juce::MouseEvent& e) override {
        const Geo geo = this->geo();
        if (e.mods.isPopupMenu()) { erasing = true; applyErase(e, geo); return; }
        holdPainting = e.y >= geo.yHold && e.y < geo.yHold + geo.holdH;
        if (holdPainting) { holdBase = -1; applyHold(e, geo); return; }
        if (!insidePads(geo, e.x, e.y)) return;
        painting = true;
        paintingTranspose = e.y < geo.yVel;
        paintAt(e, geo);
    }
    void mouseDrag(const juce::MouseEvent& e) override {
        const Geo geo = this->geo();
        if (erasing) { applyErase(e, geo); return; }
        if (holdPainting) { applyHold(e, geo); return; }
        // 1.6.14: за краями поля рисование продолжается по кламу -- крайние
        // значения velocity (пол/потолок) и стирание доходят до края.
        if (!painting) return;
        paintAt(e, geo);
    }
    void mouseUp(const juce::MouseEvent&) override { painting = false; holdPainting = false; erasing = false; }
    // 1.6.16: подсказка управления под курсором (ряд определяется по Y).
    void mouseMove(const juce::MouseEvent& e) override {
        const Geo geo = this->geo();
        juce::String tip;
        if (e.x >= geo.x0 && e.y >= geo.yTr && e.y < geo.yVel) tip = "TR (transpose -24..+24): LMB = drag from middle (bipolar), zero in the pad centre. RMB drag = erase transpose.";
        else if (e.x >= geo.x0 && e.y >= geo.yVel && e.y <= geo.yVel + geo.pad) tip = "VEL (velocity 0..127): LMB = drag from the BOTTOM (unipolar), bottom = 0 (no note). RMB drag = erase velocity.";
        else if (e.x >= geo.x0 && e.y >= geo.yHold && e.y < geo.yHold + geo.holdH) tip = "HOLD: LMB click/drag = paint holds; if the first cell you touch is ON, painting flips to ERASE (inverted). RMB drag = erase holds.";
        setTooltip(tip);
    }
private:
    void drawPad(juce::Graphics& g, int x, int y, int pad, float value, bool bipolar) const {
        g.setColour(ink.withAlpha(0.5f)); g.drawRect(x, y, pad, pad, 1);
        const int inner = 7;
        g.setColour(ink);
        if (bipolar) {
            const int mid = y + pad / 2;
            const int h = juce::roundToInt(juce::jlimit(-24.0f, 24.0f, value) / 24.0f * (pad / 2 - inner));
            g.setColour(ink.withAlpha(0.3f)); g.drawHorizontalLine(mid, float(x + inner), float(x + pad - inner));
            g.setColour(ink);
            if (h >= 0) g.fillRect(x + inner, mid - h, pad - inner * 2, h + 2); else g.fillRect(x + inner, mid, pad - inner * 2, -h + 2);
        } else {
            const int h = juce::roundToInt(juce::jlimit(0.0f, 127.0f, value) / 127.0f * (pad - inner * 2));
            g.fillRect(x + inner, y + pad - inner - h, pad - inner * 2, h); // растёт от низа (ноль внизу)
        }
        g.setColour(juce::Colours::black); g.fillRect(x + 2, y + 2, pad - 4, 15); // value notch
        g.setColour(ink);
        pixel::text(g, juce::String(juce::roundToInt(value)), {x + 2, y + 2, pad - 4, 14}, 12, true);
    }
    int columnFor(const Geo& geo, int x) const { if (x < geo.x0) return 0; return juce::jlimit(0, 7, (x - geo.x0) / (geo.pad + geo.gap)); }
    bool insidePads(const Geo& geo, int x, int y) const { return x >= geo.x0 && y >= geo.yTr && y <= geo.yVel + geo.pad; }
    void paintAt(const juce::MouseEvent& e, const Geo& geo) {
        const int pg = juce::jlimit(0, 15, pageOf()), st = columnFor(geo, e.x);
        const int y = juce::jlimit(geo.yTr, geo.yVel + geo.pad, static_cast<int>(e.y)); // 1.6.14: кламп за краями
        if (paintingTranspose) {
            // ноль в середине падa, биполярно вверх/вниз
            const float rel = juce::jlimit(0.0f, 1.0f, static_cast<float>(y - geo.yTr) / static_cast<float>(geo.pad));
            setParam(pg, st, "transpose", std::round((0.5f - rel) * 48.0f));
        } else {
            // однополярно, НАЧАЛО ВНИЗУ: низ падa = 0, верх = 127
            const float rel = juce::jlimit(0.0f, 1.0f, static_cast<float>(geo.yVel + geo.pad - y) / static_cast<float>(geo.pad));
            setParam(pg, st, "velocity", std::round(rel * 127.0f));
        }
        repaint();
    }
    void applyHold(const juce::MouseEvent& e, const Geo& geo) {
        if (e.x < geo.x0) return;
        const int pg = juce::jlimit(0, 15, pageOf()), st = columnFor(geo, e.x);
        if (holdBase < 0) { const auto pre = "arp_s" + juce::String(pg) + "_" + juce::String(st) + "_hold"; holdBase = processor.parameters.getRawParameterValue(pre)->load() > 0.5f ? 1 : 0; }
        setParam(pg, st, "hold", holdBase > 0 ? 0.0f : 1.0f); repaint();
    }
    // 1.6.16: ПКМ стирает только тот ряд, на котором находишься:
    // TR -> transpose, VEL -> velocity, HOLD -> hold (а не всё сразу).
    void applyErase(const juce::MouseEvent& e, const Geo& geo) {
        if (e.x < geo.x0) return;
        const int pg = juce::jlimit(0, 15, pageOf()), st = columnFor(geo, e.x);
        if (e.y < geo.yVel) setParam(pg, st, "transpose", 0.0f);
        else if (e.y <= geo.yVel + geo.pad) setParam(pg, st, "velocity", 0.0f);
        else if (e.y < geo.yHold + geo.holdH) setParam(pg, st, "hold", 0.0f);
        repaint();
    }
    bool painting = false, paintingTranspose = false, holdPainting = false, erasing = false; int holdBase = -1;
    MonomachineNovaAudioProcessor& processor;
    std::function<int()> pageOf; std::function<void(int, int, const char*, float)> setParam;
};

class ArpPage final : public juce::Component, private juce::Timer {
public:
    explicit ArpPage(MonomachineNovaAudioProcessor& p) : processor(p), lane(p, [this] { return juce::jlimit(0, 15, juce::roundToInt(pageBox.getValue())); }, [this](int pg, int st, const char* f, float v) { setStepParameter(pg, st, f, v); }),
        pageBox(p, "arp_step_page", 0, 15, 0), pageLimit(p, "arp_step_page_limit", 1, 16, 1), octavesBox(p, "arp_range", 1, 4, 1), gateBox(p, "arp_length", 1, 127, 64) {
        addToggle(arpEnable, "ARP ENABLE", "arp_on");
        addToggle(arpHold, "ARP HOLD", "arp_hold");
        // 1.6.13: ОДНА галочка HOST TEMPO переключает режим RATE (FREE/BPM) --
        // это arp_sync, БЕЗ касания глобального host_sync. SYNC/TIME рядом
        // переключают режим простым кликом (никаких списков).
        hostTempo.setButtonText("HOST TEMPO");hostTempo.setClickingTogglesState(true);addAndMakeVisible(hostTempo);
        hostTempo.onClick=[this]{setClockMode(hostTempo.getToggleState());};
        hostTempo.setTooltip("The single ARP rate switch: ON = RATE follows the host BPM grid, OFF = free ms. Does NOT switch the plugin-wide host sync."); // 1.6.16: SYNC/TIME-кнопки удалены (дублировали галку)
        mode.speedKey="ARP MODE";velocityMode.speedKey="ARP VEL";pageBox.speedKey="ARP PAGE";pageBox.displayOffset=1; // 1.6.14: показываем 1..16
        // 1.6.14: STEP RND -- при доигрывании каждый новый цикл страницы генерит новый паттерн.
        stepRnd.setButtonText("STEP RND");stepRnd.setClickingTogglesState(true);addAndMakeVisible(stepRnd);
        stepRnd.setTooltip("While the arpeggiator plays, every new pass over the page generates a fresh random pattern (transpose+velocity+hold). UI-only switch, not a VST parameter.");
        addAndMakeVisible(mode); mode.addItemList(processor.parameters.getParameter("arp_play")->getAllValueStrings(), 1); modeLink = std::make_unique<ComboAttachment>(processor.parameters, "arp_play", mode);
        addAndMakeVisible(velocityMode); velocityMode.addItemList(processor.parameters.getParameter("arp_velocity_mode")->getAllValueStrings(), 1); velocityModeLink = std::make_unique<ComboAttachment>(processor.parameters, "arp_velocity_mode", velocityMode);
        // 1.6.14: переключение страниц = два квадратных окошка: текущая страница (по умолчанию 1) и лимит страниц.
        // 1.6.13: RATE -- единая кнопка в одном месте; вид и шаг зависят от HOST TEMPO.
        rateButton.textHeight=18;addAndMakeVisible(rateButton);
        rateButton.dragPixelsPerStep=8;
        rateButton.dragHandler=[this](int steps,bool fine){rateAccum+=static_cast<float>(steps)*(fine?0.2f:1.0f)*static_cast<float>(uiSpeed("ARP RATE"));
            const int whole=static_cast<int>(rateAccum>=0?std::floor(rateAccum):std::ceil(rateAccum));if(whole==0)return;rateAccum-=static_cast<float>(whole);
            const bool sync=isSyncMode();auto* par=processor.parameters.getParameter(sync?"arp_grid":"arp_time");
            const float base=par->convertFrom0to1(par->getValue());
            const float v=juce::jlimit(sync?0.0f:5.0f,sync?15.0f:2000.0f,base+static_cast<float>(whole)*(sync?1.0f:10.0f));
            par->beginChangeGesture();par->setValueNotifyingHost(par->convertTo0to1(v));par->endChangeGesture();refreshRate();};
        rateButton.doubleClickHandler=[this]{const bool sync=isSyncMode();auto* par=processor.parameters.getParameter(sync?"arp_grid":"arp_time");par->beginChangeGesture();par->setValueNotifyingHost(par->convertTo0to1(par->getDefaultValue()));par->endChangeGesture();refreshRate();};
        // 1.6.16: GATE -- просто квадрат со значением (слайдер убран); WRAP остаётся слайдером.
        addAndMakeVisible(wrap); wrap.setSliderStyle(juce::Slider::LinearHorizontal); wrap.setTextBoxStyle(juce::Slider::TextBoxRight, false, 54, 24); wrap.setRange(1, 16, 1); wrapLink = std::make_unique<SliderAttachment>(processor.parameters, "arp_wrap", wrap);
        addAndMakeVisible(pageLimit); addAndMakeVisible(octavesBox); addAndMakeVisible(gateBox);
        gateBox.speedKey="VALUE BOXES";gateBox.setTooltip("ARP gate length, 1..127. Drag / wheel = +-1, double-click = default.");
        // 1.6.13: RANDOM-надписи больше нет: кнопки PAGE/ALL, под ними PITCH/VEL.
        pageRndButton.setButtonText("PAGE"); pageRndButton.textHeight = 12; addAndMakeVisible(pageRndButton);
        pageRndButton.setTooltip("Randomize all fields of the 8 steps of the current page.");
        allRndButton.setButtonText("ALL"); allRndButton.textHeight = 12; addAndMakeVisible(allRndButton);
        allRndButton.setTooltip("Randomize all 128 steps (16 pages x 8) in one batch -- no lag.");
        pitchRndButton.setButtonText("PITCH"); pitchRndButton.textHeight = 12; addAndMakeVisible(pitchRndButton);
        pitchRndButton.setTooltip("Randomize TRANSPOSE of the 8 steps of the current page.");
        velRndButton.setButtonText("VEL"); velRndButton.textHeight = 12; addAndMakeVisible(velRndButton);
        velRndButton.setTooltip("Randomize VELOCITY of the 8 steps of the current page.");
        pageRndButton.onClick=[this]{const int pg=curPage();randomizeRange(pg,pg,true,true,true);};
        pitchRndButton.onClick=[this]{const int pg=curPage();randomizeRange(pg,pg,true,false,false);};
        velRndButton.onClick=[this]{const int pg=curPage();randomizeRange(pg,pg,false,true,false);};
        allRndButton.onClick=[this]{randomizeRange(0,15,true,true,true);};
        addAndMakeVisible(lane);
        // 1.6.14: GUI DRAG SPEED -- своя плавность прокрутки каждого списка (только UI,
        // лимит автоматизируемых VST-параметров не тратится). 1.6.15: RESET ALL.
        speedReset.setButtonText("RESET");speedReset.textHeight=10;addAndMakeVisible(speedReset);
        speedReset.setTooltip("Restore all GUI drag speeds to their defaults.");
        speedReset.onClick=[this]{for(size_t i=0;i<speedBoxes.size()&&i<sizeof(kSpeedDefs)/sizeof(kSpeedDefs[0]);++i)speedBoxes[i]->refreshTo(kSpeedDefs[i].def);};
        for(const auto& def:kSpeedDefs){
            auto label=std::make_unique<PixelLabel>();label->setText(def.label,juce::dontSendNotification);
            label->setBounds(0,0,10,14);label->setJustificationType(juce::Justification::centredLeft);addAndMakeVisible(*label);
            auto box=std::make_unique<FloatBox>(def.key,def.def);addAndMakeVisible(*box);
            speedLabels.push_back(std::move(label));speedBoxes.push_back(std::move(box));}
        rebindSteps(); refreshRate(); setSize(1160, 1010); startTimerHz(15);
    }
    ~ArpPage() override { stopTimer(); }

    // 1.6.16: скролл страницы снова колёсиком (как было), ЛКМ-драг-скролл убран.
    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::black); g.setColour(ink);
        pixel::text(g, "ARP / STEP SEQUENCER", {12, 4, 600, 26}, 21);
        g.setColour(ink.withAlpha(0.35f)); g.drawRect(10, 38, 1140, 108, 1); g.drawRect(10, 158, 1140, 116, 1); g.drawRect(10, 286, 1140, 390, 1);
        pixel::text(g, "PLAY", {24, 48, 80, 20}, 14, true); pixel::text(g, "MODE", {328, 48, 80, 20}, 14, true); pixel::text(g, "RATE", {518, 48, 80, 20}, 14, true);
        pixel::text(g, "OCT", {925, 48, 80, 20}, 14, true); pixel::text(g, "GATE", {995, 48, 80, 20}, 14, true);
        pixel::text(g, "WRAP", {24, 166, 80, 20}, 14, true); pixel::text(g, "VELOCITY", {210, 166, 110, 20}, 14, true); pixel::text(g, "PAGE LIMIT", {340, 166, 110, 20}, 14, true);
        pixel::text(g, "PAGE", {24, 166+62, 80, 20}, 14, true);
        pixel::text(g, "RANDOM", {470, 166, 110, 20}, 14, true);
        pixel::text(g, "STEPS", {24, 294, 80, 20}, 15, true);
        // 1.6.14: GUI DRAG SPEED -- подпись секции
        pixel::text(g, "GUI DRAG SPEED (per list; drag/wheel, double-click = default)", {24, speedY()-22, 800, 16}, 13, true);
    }
    void resized() override {
        arpEnable.setBounds(20, 76, 135, 28); arpHold.setBounds(170, 76, 125, 28); mode.setBounds(328, 76, 170, 28);
        rateButton.setBounds(518, 76, 250, 28); // 1.6.13: единая кнопка RATE в одном месте
        octavesBox.setBounds(925, 76, 56, 28); gateBox.setBounds(995, 76, 64, 28); // 1.6.16: GATE -- квадрат со значением
        hostTempo.setBounds(24, 110, 150, 28); // 1.6.16: единственный переключатель режима RATE
        wrap.setBounds(24, 195, 160, 28); velocityMode.setBounds(210, 195, 110, 28); pageLimit.setBounds(340, 195, 64, 28);
        pageRndButton.setBounds(470, 195, 64, 28); allRndButton.setBounds(538, 195, 64, 28); // встык
        pitchRndButton.setBounds(470, 227, 64, 26); velRndButton.setBounds(538, 227, 64, 26); // под PAGE и под ALL
        pageBox.setBounds(24, 238, 64, 28); // текущая страница (по умолчанию 1)
        stepRnd.setBounds(620, 195, 110, 28); // 1.6.14: STEP RND рядом с RANDOM
        lane.setBounds(20, 318, 1115, 360);
        speedReset.setBounds(860, speedY()-24, 90, 22); // 1.6.15: RESET ALL у заголовка сетки
        for(size_t i=0;i<speedBoxes.size();++i){const int x=10+static_cast<int>(i%4)*280,y=speedY()+static_cast<int>(i/4)*50;speedLabels[i]->setBounds(x,y,130,16);speedBoxes[i]->setBounds(x,y+18,120,26);}
    }
private:
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    void addToggle(juce::ToggleButton& button, const juce::String& text, const juce::String& id) { button.setButtonText(text); button.setClickingTogglesState(true); addAndMakeVisible(button); toggleLinks.push_back(std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processor.parameters, id, button)); }
    bool isSyncMode() const { auto* s = processor.parameters.getRawParameterValue("arp_sync"); return s && s->load() > 0.5f; }
    int curPage() const { return juce::jlimit(0, 15, juce::roundToInt(pageBox.getValue())); } // 0-базная
    int speedY() const { return 700; }
    void setClockMode(bool sync) { if (auto* p = processor.parameters.getParameter("arp_sync")) { p->beginChangeGesture(); p->setValueNotifyingHost(p->convertTo0to1(sync ? 1.0f : 0.0f)); p->endChangeGesture(); } refreshRate(); }
    void refreshRate() { // единая кнопка RATE показывает значение текущего режима
        auto* grid = processor.parameters.getParameter("arp_grid");
        rateButton.setButtonText(isSyncMode() ? grid->getText(grid->getValue(), 16) : juce::String(processor.parameters.getRawParameterValue("arp_time")->load(), 0) + " MS");
        hostTempo.setToggleState(isSyncMode(), juce::dontSendNotification);
        rateButton.repaint();
    }
    void setStepParameter(int pg, int st, const char* field, float value) { const auto id = "arp_s" + juce::String(pg) + "_" + juce::String(st) + "_" + field; if (auto* p = processor.parameters.getParameter(id)) { p->beginChangeGesture(); p->setValueNotifyingHost(p->convertTo0to1(value)); p->endChangeGesture(); } }
    // 1.6.13: батч-рандом без жестов хоста (128 шагов больше не лагает):
    // приостанавливаем аудио-поток на время записи, в хост шлём только значения.
    void randomizeRange(int pg0, int pg1, bool doTr, bool doVel, bool doHold) {
        pg0 = juce::jlimit(0, 15, pg0); pg1 = juce::jlimit(0, 15, pg1);
        auto& r = juce::Random::getSystemRandom();
        processor.suspendProcessing(true);
        for (int pg = pg0; pg <= pg1; ++pg) for (int st = 0; st < 8; ++st) {
            // 1.6.16: setValue БЕЗ нотификации хоста -- Ableton не считает это
            // автоматизацией и не строит сотни точек (лаги исчезали только так).
            const auto put = [this](int pg2, int st2, const char* f, float v) { if (auto* p = processor.parameters.getParameter("arp_s" + juce::String(pg2) + "_" + juce::String(st2) + "_" + f)) p->setValue(p->convertTo0to1(v)); };
            if (doHold) put(pg, st, "hold", float(r.nextInt(2)));
            if (doTr) put(pg, st, "transpose", float(r.nextInt(49) - 24));
            if (doVel) put(pg, st, "velocity", float(r.nextInt(128)));
        }
        processor.suspendProcessing(false);
        lane.repaint(); repaint();
    }
    void rebindSteps() { lane.repaint(); repaint(); }
    void timerCallback() override {
        refreshRate(); pageBox.refreshFromParam();
        const int raw = curPage(); if (raw != seenPage) { seenPage = raw; rebindSteps(); }
        // 1.6.14: STEP RND -- новый паттерн страницы на каждом новом цикле арпеджиатора.
        const int echo = processor.arpStepEcho.load();
        if (stepRnd.getToggleState() && echo < lastStepEcho) { const int pg = curPage(); randomizeRange(pg, pg, true, true, true); }
        lastStepEcho = echo;
        lane.repaint(); repaint();
    }
    MonomachineNovaAudioProcessor& processor; juce::ToggleButton arpEnable, arpHold, hostTempo, stepRnd; PixelButton rateButton{""}, pageRndButton{"PAGE"}, allRndButton{"ALL"}, pitchRndButton{"PITCH"}, velRndButton{"VEL"}; StepLane lane;
    DragChoice mode, velocityMode; // 1.6.14: clock-список и PAGE-комбо удалены
    DragValueBox pageBox, pageLimit, octavesBox, gateBox; // 1.6.16: GATE -- окошко
    juce::Slider wrap;
    PixelButton speedReset{"RESET"}; float rateAccum = 0; int seenPage = -1, lastStepEcho = -1;
    std::vector<std::unique_ptr<PixelLabel>> speedLabels; std::vector<std::unique_ptr<FloatBox>> speedBoxes; // 1.6.14: GUI DRAG SPEED
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>> toggleLinks; std::unique_ptr<ComboAttachment> modeLink, velocityModeLink; std::unique_ptr<SliderAttachment> wrapLink; // 1.6.16: attachment GATE убран
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
        pixel::text(g, "CLICK HEADER TO SORT (ON, SOURCE, DEST, DEPTH, MODE) / DRAG MSEG OUTPUT TO TARGET", {470, 7, 650, 17}, 10, false);
        const char* headers[]{"", "ON", "SOURCE", "DESTINATION", "DEPTH", "MODE"};
        for (int c = 0; c < 6; ++c) pixel::text(g, headers[c], {colX(c), 30, colWidth(c), 19}, 13, true);
        if (sortColumn >= 1 && sortColumn <= 5) { // pixel triangle, font-safe (1.6.8)
            g.setColour(ink.withAlpha(0.5f));
            const int ax = colX(sortColumn) + colWidth(sortColumn) - 18;
            for (int i = 0; i < 4; ++i) g.fillRect(ax + i * 2, sortDescending ? 34 + i * 2 : 42 - i * 2, 8 - i * 2, 2);
        }
        for (int r = 0; r < 16; ++r) {
            const int y = rowY(r); g.setColour(ink.withAlpha(0.18f)); g.drawHorizontalLine(y + 31, 8.0f, 1128.0f);
            for (int c = 1; c <= 5; ++c) {
                auto* par = cell(r, c); if (!par) continue; const int value = juce::roundToInt(par->convertFrom0to1(par->getValue()));
                if (c == 1) { drawCheck(g, colX(c) + 10, y + 7, value > 0); continue; }
                juce::String text = c == 2 ? sourceName(value) : c == 3 ? targetName(processor, juce::jlimit(0, 55, value)) : c == 4 ? signedPercent(value) : polarityName(value);
                // 1.6.13: параметры в DEST-колонке по центру -- сразу видно, какая строка короче.
                pixel::text(g, text, {colX(c) + 5, y + 2, colWidth(c) - (c == 3 ? 58 : 8), 25}, c == 3 ? 14 : 16, c == 3 ? true : c != 3);
                if (c == 3) { // 1.6.12: замок маршрута слева от прицела
                    const int lockMode = lockValue(r);
                    drawLock(g, colX(c) + colWidth(c) - 46, y + 7, lockMode);
                    drawCrosshair(g, colX(c) + colWidth(c) - 18, y + 15);
                }
            }
            if (flashLockOnly == r) { g.setColour(ink); for (int k = 0; k < 3; ++k) g.drawRect(colX(3) + colWidth(3) - 50, y + 3 + k, 18 - k * 2, 20 - k * 2, 1); }
            if (hoverLock == r + 1) { // 1.6.14: подсказка режима замка при наведении
                const int lm = lockValue(r);
                pixel::text(g, lm == 2 ? "SOLO (RMB: modes)" : lm == 1 ? "LOCK (RMB: modes)" : "FREE (LMB: LOCK, RMB: modes)", {colX(3) + colWidth(3) - 240, y - 1, 190, 15}, 10, true);
            }
            g.setColour(ink.withAlpha(0.22f)); for (int c = 1; c <= 5; ++c) g.drawVerticalLine(colX(c) - 4, float(y), float(y + 31));
        }
        flashLockOnly = -1;
        g.setColour(ink.withAlpha(0.35f)); g.drawRect(8, 26, 1120, 16 * 34 + 36, 1);
        g.setColour(ink); pixel::text(g, "DEPTH 0..100%   UNIPOLAR = ONE SIDE   BIPOLAR = TWO SIDES   GRAB EMPTY LEFT EDGE TO REORDER   LOCK: LMB FREE/LOCK, RMB SOLO", {10, getHeight() - 24, 1115, 17}, 11, false); // 1.6.14: белая + альтернативы замка
    }

    void mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& w) override { const int r = rowAt(e.y), c = colAt(e.x); if (r < 0 || c < 1 || std::abs(w.deltaY) < 0.0001f) return; auto* p = cell(r, c); if (!p) return; const int old = juce::roundToInt(p->convertFrom0to1(p->getValue())); const int lo = c == 4 ? -64 : 0, hi = c == 4 ? 63 : (c == 5 ? 1 : choiceCount(p) - 1); p->beginChangeGesture(); p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(juce::jlimit(lo, hi, old + (w.deltaY > 0 ? 1 : -1))))); p->endChangeGesture(); repaint(); }
    void mouseMove(const juce::MouseEvent& e) override { // 1.6.14: подсказка при наведении на замок
        const int r = rowAt(e.y); const bool lockZone = r >= 0 && e.x >= colX(3) + colWidth(3) - 50 && e.x < colX(3) + colWidth(3) - 28;
        const int zone = lockZone ? r + 1 : 0; if (zone != hoverLock) { hoverLock = zone; repaint(); }
    }
    void mouseDown(const juce::MouseEvent& e) override {
        if (e.y < 56) { const int c = colAt(e.x); if (c >= 1 && c <= 5) sortBy(c); return; } // 1.6.8: the ON checkbox column sorts too
        const int r = rowAt(e.y), c = colAt(e.x); if (r < 0) return;
        if (c == 0) { dragRow = r; dragTarget = r; return; }
        if (c == 1) { if (auto* p = cell(r, 1)) toggle(*p); return; }
        if (c == 3 && e.x >= colX(c) + colWidth(c) - 46 && e.x < colX(c) + colWidth(c) - 28) {
            if (e.mods.isPopupMenu()) { // 1.6.12: ПКМ по замку -- режимы
                juce::PopupMenu m; const int cur = lockValue(r);
                m.setLookAndFeel(&getLookAndFeel()); // 1.6.14: чёрный скин
                m.addItem(1, "FREE (normal)", true, cur == 0);
                m.addItem(2, "LOCK (aim/re-write cannot touch dest)", true, cur == 1);
                m.addItem(3, "SOLO (pinned to this parameter)", true, cur == 2);
                const juce::Component::SafePointer<MatrixPage> safe(this);
                m.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(this).withTargetScreenArea(juce::Rectangle<int>(e.getScreenPosition().x, e.getScreenPosition().y, 1, 1)), [safe, r](int res) { if (safe && res > 0) safe->setLock(r, res - 1); }); // 1.6.14: меню замка у курсора
                return;
            }
            setLock(r, lockValue(r) > 0 ? 0 : 1); return; // ЛКМ: FREE <-> LOCK
        }
        if (c == 3 && e.x >= colX(c) + colWidth(c) - 28) {
            if (lockValue(r) > 0) { flashLockOnly = r; repaint(); return; } // 1.6.12: прицел блокирован замком
            if (onTargetPick) onTargetPick(r); return;
        }
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
        menu.setLookAndFeel(&getLookAndFeel()); // 1.6.14: чёрный скин
        if (c == 4) { const int values[]{0, 16, 32, 48, 63, -16, -32, -48, -64}; for (int i = 0; i < 9; ++i) menu.addItem(i + 1, signedPercent(values[i])); }
        else if (c == 3) { // 1.6.12: адресаты группируются по страницам как на панели
            const int selected = juce::roundToInt(par->convertFrom0to1(par->getValue()));
            const char* groups[]{"MACHINE >", "AMP >", "FILTER >", "EFFX >", "LFO1 >", "LFO2 >", "LFO3 >"};
            for (int g = 0; g < 7; ++g) {
                juce::PopupMenu sub;
                for (int k = 0; k < 8; ++k) { const int target = g * 8 + k; sub.addItem(target + 1, targetName(processor, target), true, target == selected); }
                menu.addSubMenu(juce::String(groups[g]), sub, true, nullptr, g == selected / 8);
            }
        }
        else { const int n = c == 5 ? 2 : choiceCount(par); const int selected = juce::roundToInt(par->convertFrom0to1(par->getValue())); const auto names = par->getAllValueStrings(); for (int i = 0; i < n; ++i) menu.addItem(i + 1, c == 2 ? sourceName(i) : (names.size() > i ? names[i] : polarityName(i)), true, i == selected); }
        const auto screen = e.getScreenPosition(); const juce::Component::SafePointer<MatrixPage> safe(this);
        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(this).withTargetScreenArea(juce::Rectangle<int>(screen.x, screen.y, 1, 1)), [safe, r, c](int result) { if (!safe || result <= 0) return; auto* p = safe->cell(r, c); if (!p) return; if (c == 4) { const int values[]{0, 16, 32, 48, 63, -16, -32, -48, -64}; p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(values[result - 1]))); } else p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(result - 1))); });
    }
private:
    static int choiceCount(juce::RangedAudioParameter* p) { if (!p) return 0; const auto n = p->getAllValueStrings(); return n.size() > 0 ? n.size() : juce::roundToInt(p->convertFrom0to1(1.0f)) + 1; }
    static juce::String sourceName(int v) { const char* n[]{"KEY", "VEL", "MACRO X", "MACRO Y", "LFO1", "LFO2", "LFO3", "PITCH WHL", "MOD WHL", "AFTERTOUCH", "MSEG", "STEP VEL", "STEP TRANS", "ARP GATE", "ARP RATE", "RANDOM"}; return n[juce::jlimit(0, 15, v)]; }
    static juce::String polarityName(int v) { return v == 1 ? "BIPOLAR" : "UNIPOLAR"; }
    // 1.6.12: замок маршрута: 0 FREE, 1 LOCK (прицел/перезапись не трогают),
    // 2 SOLO (приколот к этому параметру; руками менять можно, замок висит).
    int lockValue(int r) const { auto* p = processor.parameters.getParameter("r" + juce::String(r) + "_lock"); return p ? juce::jlimit(0, 2, juce::roundToInt(p->convertFrom0to1(p->getValue()))) : 0; }
    void setLock(int r, int mode) { if (auto* p = processor.parameters.getParameter("r" + juce::String(r) + "_lock")) { p->beginChangeGesture(); p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(juce::jlimit(0, 2, mode)))); p->endChangeGesture(); } repaint(); }
    static void drawLock(juce::Graphics& g, int x, int y, int mode) {
        const float fx = float(x), fy = float(y);
        g.setColour(ink); // 1.6.14: замок всегда белый (тусклый был не виден)
        g.drawRect(fx + 0.0f, fy + 5.0f, 14.0f, 11.0f, 1.0f);                // корпус
        if (mode > 0) g.fillRect(fx + 5.0f, fy + 9.0f, 4.0f, 3.0f);          // скважина
        if (mode == 2) g.drawVerticalLine(x + 7, fy - 3.0f, fy + 5.0f);      // SOLO: язычок вверх
        else { g.drawLine(fx + 3.0f, fy + 5.0f, fx + 3.0f, fy + 1.0f, 1.0f); g.drawLine(fx + 11.0f, fy + 5.0f, fx + 11.0f, fy + 1.0f, 1.0f); g.drawHorizontalLine(fy + 1, fx + 3.0f, fx + 11.0f); } // дуга
    }
    static void drawCheck(juce::Graphics& g, int x, int y, bool checked) { const float fx = float(x), fy = float(y); g.setColour(ink); g.drawRect(fx, fy, 18.0f, 18.0f, 1.0f); if (checked) { g.drawLine(fx + 4.0f, fy + 9.0f, fx + 8.0f, fy + 14.0f, 2.0f); g.drawLine(fx + 8.0f, fy + 14.0f, fx + 15.0f, fy + 4.0f, 2.0f); } }
    static void drawCrosshair(juce::Graphics& g, int x, int y) { const float fx = float(x), fy = float(y); g.setColour(ink); g.drawEllipse(fx - 6.0f, fy - 6.0f, 12.0f, 12.0f, 1.0f); g.drawLine(fx - 9.0f, fy, fx + 9.0f, fy, 1.0f); g.drawLine(fx, fy - 9.0f, fx, fy + 9.0f, 1.0f); }
    int colX(int c) const { const int x[]{8, 28, 78, 245, 580, 735}; return x[juce::jlimit(0, 5, c)]; }
    int colWidth(int c) const { const int w[]{18, 42, 165, 330, 150, 190}; return w[juce::jlimit(0, 5, c)]; }
    int rowY(int r) const { return 58 + r * 34; }
    int colAt(int x) const { for (int c = 0; c < 6; ++c) if (x >= colX(c) && x < colX(c) + colWidth(c)) return c; return -1; }
    int rowAt(int y) const { if (y < 58) return -1; const int r = (y - 58) / 34; return r >= 0 && r < 16 ? r : -1; }
    juce::RangedAudioParameter* cell(int r, int c) { return cells[static_cast<size_t>(r * 5 + (c - 1))]; }
    int flashLockOnly = -1; int hoverLock = 0; // 1.6.14: hover-подсказка замка
    void toggle(juce::RangedAudioParameter& p) { p.beginChangeGesture(); p.setValueNotifyingHost(p.convertTo0to1(p.convertFrom0to1(p.getValue()) > 0.5f ? 0.0f : 1.0f)); p.endChangeGesture(); }
    void sortBy(int c) { if (sortColumn == c) sortDescending = !sortDescending; else { sortColumn = c; sortDescending = (c == 4 || c == 1); } processor.sortModRoutes(c, sortDescending); repaint(); } // 1.6.8: ON column sorts enabled-first by default
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
        for (int i = 1; i < 4; ++i) g.drawHorizontalLine(graphY + graphH * i / 4, float(graphX), float(graphX + graphW));
        for (int i = 1; i < 8; ++i) g.drawVerticalLine(graphX + graphW * i / 8, float(graphY), float(graphY + graphH));
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
        if (selected < 0) { processor.addMsegPoint(toX(float(e.x)), toY(float(e.y))); selected = nearestPoint(e.position); }
        draggingPoint = selected >= 0; repaint();
    }
    void mouseDrag(const juce::MouseEvent& e) override {
        if (erasing) { eraseAt(e.position); return; }
        if (!draggingPoint || selected < 0) return;
        const float x = toX(float(e.x)), y = toY(float(e.y)); float lo = selected == 0 ? 0.0f : processor.msegPointX(selected - 1) + 0.005f; float hi = selected == processor.msegPointCount() - 1 ? 1.0f : processor.msegPointX(selected + 1) - 0.005f; processor.setMsegPoint(selected, juce::jlimit(lo, hi, x), y); repaint();
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
        mode.setScrollWheelEnabled(true);mode.addItem("mnm",2);mode.setSelectedId(2,juce::dontSendNotification);mode.setVisible(false); // 1.6.14: old AMP DSP удалён -- всегда mnm
        // 1.6.14: attachment удалён вместе с old AMP DSP
        const char* ids[]{"p0_0","p0_1","p0_2","p0_3","amp_curve_a","amp_curve_d","amp_curve_r"};
        const char* names[]{"ATK","HOLD","DEC","REL","ATK CURVE","DEC CURVE","REL CURVE"};
        for(size_t i=0;i<7;++i){auto& sl=sliders[i];sl.setColour(juce::Slider::thumbColourId,ink);sl.setColour(juce::Slider::trackColourId,ink.withAlpha(0.8f));sl.setColour(juce::Slider::backgroundColourId,ink.withAlpha(0.2f));sl.setSliderStyle(juce::Slider::LinearHorizontal);sl.setTextBoxStyle(juce::Slider::TextBoxRight,false,62,22);sl.setName(names[i]);sl.setDoubleClickReturnValue(true,i<4?(i<2?0:64):0);addAndMakeVisible(sl);links[i]=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters,ids[i],sl);labels[i].setText(names[i],juce::dontSendNotification);addAndMakeVisible(labels[i]);}
        gate.setColour(juce::Slider::thumbColourId,ink);gate.setColour(juce::Slider::trackColourId,ink.withAlpha(0.8f));gate.setColour(juce::Slider::backgroundColourId,ink.withAlpha(0.2f));gate.setRange(10,10000,1);gate.setValue(1000);gate.setSliderStyle(juce::Slider::LinearHorizontal);gate.setTextBoxStyle(juce::Slider::TextBoxRight,false,62,22);addAndMakeVisible(gate);
        gateLabel.setText("PREVIEW NOTE (ms)",juce::dontSendNotification);addAndMakeVisible(gateLabel);
        setSize(1170,465);startTimerHz(15);refresh();
    }
    void focusCurve(int knob){selected=knob==0?4:knob==1?-1:knob==2?5:6; // 1.6.13: HOLD открывает страницу без фокуса кривой
        if(selected>=0&&sliders[static_cast<size_t>(selected)].isEnabled())sliders[static_cast<size_t>(selected)].grabKeyboardFocus();repaint();}
    void resized()override{for(int i=0;i<8;++i){const int x=20+(i%4)*285,y=330+(i/4)*58;if(i<7){labels[static_cast<size_t>(i)].setBounds(x,y,260,20);sliders[static_cast<size_t>(i)].setBounds(x,y+20,265,28);}else{gateLabel.setBounds(x,y,265,20);gate.setBounds(x,y+20,265,28);}}} // 1.6.14: mode-комбо удалено
    void paint(juce::Graphics& g)override{
        g.fillAll(juce::Colours::black);g.setColour(ink);pixel::text(g,"AMP / ENVELOPE",{20,8,650,26},22);
        const juce::Rectangle<float> box(30,60,1110,225);g.setColour(ink.withAlpha(0.2f));g.drawRect(box);
        for(int i=1;i<4;++i)g.drawHorizontalLine(60+i*56,30.0f,1140.0f);
        for(int i=0;i<=4;++i){float x=30.0f+1110.0f*static_cast<float>(i)/4.0f;g.drawVerticalLine(juce::roundToInt(x),60,285);g.setColour(ink);g.drawText(juce::String(duration*i/4,2)+" s",juce::roundToInt(x)-25,286,70,20,juce::Justification::centred);g.setColour(ink.withAlpha(0.2f));}
        juce::Path path;for(size_t i=0;i<trace.size();++i){float x=30+1110*static_cast<float>(i)/std::max(1.0f,static_cast<float>(trace.size()-1));float y=285-225*trace[i];if(i==0)path.startNewSubPath(x,y);else path.lineTo(x,y);}g.setColour(ink);g.strokePath(path,juce::PathStrokeType(1.8f));
        const float off=30+1110*static_cast<float>(gate.getValue()/1000/duration);if(off<1140){g.setColour(ink.withAlpha(0.5f));g.drawVerticalLine(juce::roundToInt(off),60,285);g.drawText("NOTE OFF",juce::roundToInt(off)+4,62,85,18,juce::Justification::left);}
        g.setColour(ink);g.drawText("mnm = firmware envelope. Drag graph vertically: decay before NOTE OFF, release after. Preview uses the DSP envelope; display limited to 20 s.",20,307,1130,20,juce::Justification::left); // 1.6.14: old удалён
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
        {auto& sb=list.getVerticalScrollBar();sb.setColour(juce::ScrollBar::thumbColourId,juce::Colours::white);sb.setColour(juce::ScrollBar::trackColourId,juce::Colours::black);} // 1.6.13: белый, не синий
        addAndMakeVisible(load);load.setButtonText("LOAD FILES");load.onClick=[this]{choose();};
        addAndMakeVisible(oldKit);oldKit.setButtonText("OLD KIT");oldKit.onClick=[this]{processor.resetSamples(false);list.updateContent();list.repaint();};
        addAndMakeVisible(restore);restore.setButtonText("RESTORE KIT");restore.onClick=[this]{processor.resetSamples();list.updateContent();list.repaint();};restore.setTooltip("Replace all 24 slots with the factory kit, including imported samples.");
        startTimerHz(25);setSize(1120,450);list.selectRow(juce::roundToInt(processor.parameters.getRawParameterValue("m7_2")->load()));
    }
    int getNumRows()override{return 24;}
    void paintListBoxItem(int row,juce::Graphics& g,int w,int h,bool selected)override{
        // 1.6.12: серое превью -- эти слоты займут перетаскиваемые файлы.
        if(dragRow>=0&&row>=dragRow&&row<dragRow+dragFiles){g.fillAll(juce::Colours::white.withAlpha(0.10f));g.setColour(juce::Colours::white.withAlpha(0.55f));
            const char* notes2[]{"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};
            pixel::text(g,juce::String(row+1).paddedLeft('0',2)+"  "+juce::String(notes2[row%12])+juce::String((48+row)/12-2)+"  + WILL LOAD",{8,2,w-16,h-4},16);return;}
        if(row==processor.playingSample()||(row==flashSlot&&juce::Time::getMillisecondCounterHiRes()<flashUntil))g.fillAll(juce::Colours::white.withAlpha(0.35f));
        else if(selected)g.fillAll(juce::Colours::white.withAlpha(0.18f));g.setColour(juce::Colours::white);
        const char* notes[]{"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};
        // 1.6.13: PLAY-колонка рядом с номером -- можно прослушать, что в слоте.
        const bool hasSample=processor.sampleName(row).trim().isNotEmpty()&&processor.sampleName(row)!="---";
        pixel::text(g,juce::String(row+1).paddedLeft('0',2),{8,2,30,h-4},16);
        const int cy=h/2;g.setColour(hasSample?juce::Colours::white:juce::Colours::white.withAlpha(0.25f));
        for(int i=0;i<6;++i)g.fillRect(44+i,cy-i,2,2*i+1); // треугольник "плей" вправо
        g.setColour(juce::Colours::white);
        pixel::text(g,juce::String(notes[row%12])+juce::String((48+row)/12-2)+"  "+processor.sampleName(row),{64,2,w-72,h-4},16);
    }
    void selectedRowsChanged(int row)override{if(row<0)return;auto* par=processor.parameters.getParameter("m7_2");par->beginChangeGesture();par->setValueNotifyingHost(par->convertTo0to1(static_cast<float>(row)));par->endChangeGesture();}
    void listBoxItemClicked(int row,const juce::MouseEvent& e)override{if(row<0)return;if(e.x>=40&&e.x<=58){processor.previewSample(row);list.selectRow(row);} } // 1.6.13: PLAY
    void listBoxItemDoubleClicked(int,const juce::MouseEvent&)override{choose();}
    bool isInterestedInFileDrag(const juce::StringArray& files)override{return !files.isEmpty();}
    // 1.6.12: пока тащишь файлы -- показываем, какие слоты будут заняты.
    void fileDragEnter(const juce::StringArray& files,int x,int y)override{dragFiles=files.size();updateDragRow(x,y);}
    void fileDragMove(const juce::StringArray& files,int x,int y)override{dragFiles=files.size();updateDragRow(x,y);}
    void fileDragExit(const juce::StringArray&)override{dragRow=-1;dragFiles=0;list.repaint();}
    void filesDropped(const juce::StringArray& files,int x,int y)override{dragRow=-1;dragFiles=0;const auto q=list.getLocalPoint(this,juce::Point<int>{x,y});int row=list.getRowContainingPosition(q.x,q.y);loadFiles(files,row<0?std::max(0,list.getSelectedRow()):row);}
    void updateDragRow(int x,int y){const auto q=list.getLocalPoint(this,juce::Point<int>{x,y});int row=list.getRowContainingPosition(q.x,q.y);const int next=(row<0?std::max(0,list.getSelectedRow()):row);if(next!=dragRow){dragRow=next;list.repaint();}}
    void paint(juce::Graphics& g)override{g.fillAll(juce::Colours::black);g.setColour(juce::Colours::white);pixel::text(g,"BBOX / 24 SLOTS",{8,4,560,24},20);}
    void resized()override{oldKit.setBounds(getWidth()-532,4,168,28);restore.setBounds(getWidth()-356,4,168,28);load.setBounds(getWidth()-180,4,168,28);list.setBounds(8,40,getWidth()-16,getHeight()-48);}
private:
    void timerCallback()override{auto serial=processor.bboxHitCounter.load();if(serial!=lastSerial){lastSerial=serial;flashSlot=processor.lastBboxSample.load();flashUntil=juce::Time::getMillisecondCounterHiRes()+120;}list.repaint();}
    void loadFiles(const juce::StringArray& files,int row){juce::String errors;for(const auto& path:files){if(row>=24)break;auto e=processor.loadSample(row++,juce::File(path));if(e.isNotEmpty())errors+=e+"\n";}list.updateContent();list.repaint();if(errors.isNotEmpty())juce::AlertWindow::showMessageBoxAsync(juce::MessageBoxIconType::WarningIcon,"Sample import",errors);}
    void choose(){chooser=std::make_unique<juce::FileChooser>("Load BBOX samples",juce::File(),"*.wav;*.aif;*.aiff;*.flac");juce::Component::SafePointer<SamplePage> safe(this);
        chooser->launchAsync(juce::FileBrowserComponent::openMode|juce::FileBrowserComponent::canSelectFiles|juce::FileBrowserComponent::canSelectMultipleItems,[safe](const juce::FileChooser& fc){if(!safe)return;juce::StringArray paths;for(auto f:fc.getResults())paths.add(f.getFullPathName());safe->loadFiles(paths,std::max(0,safe->list.getSelectedRow()));});}
    unsigned lastSerial=0;int flashSlot=-1;double flashUntil=0;int dragRow=-1,dragFiles=0; // 1.6.12: превью дропа
    MonomachineNovaAudioProcessor& processor;juce::ListBox list;juce::TextButton load,restore,oldKit;std::unique_ptr<juce::FileChooser> chooser;
};
class FramedViewport : public juce::Viewport {
public:
    FramedViewport(){setOpaque(true);}
    void paint(juce::Graphics& g)override{g.fillAll(juce::Colours::black);}
    void paintOverChildren(juce::Graphics& g)override{g.setColour(juce::Colours::white.withAlpha(0.65f));g.drawRect(getLocalBounds(),2);}
};

}

struct MonomachineNovaAudioProcessorEditor::Surface final : juce::Component, public juce::DragAndDropContainer, private juce::Timer {
    explicit Surface(MonomachineNovaAudioProcessor& p):processor(p),machineButton(""),menuButton("MENU"),tempoButton(""),arpButton("ARP"),arpSettingsButton("SET"),matrixButton("MATRIX"),lfo1Button("LFO1"),lfo2Button("LFO2"),lfo3Button("LFO3"),msegButton("MSEG"),msegOutputButton("MSEG OUT"),closeButton("BACK"),gateButton(""){
        setLookAndFeel(&theme);
        for(auto& panel:cells)for(auto& cell:panel){cell=std::make_unique<Cell>(p);cell->openSamples=[this]{showSamples();};cell->openEnvelope=[this](int knob){showEnvelope(knob);};cell->openModes=[this](int section,juce::Point<int> at){showDspModes(section,at);};cell->openRepitch=[this](juce::Point<int> at){showRepitchMenu(at);};cell->openLocks=[this](int lfo,bool dest,juce::Point<int> at){showLfoLocks(lfo,dest,at);};cell->addMsegModulation=[this](uint8_t target){processor.addMsegRoute(target);};cell->pickTarget=[this](uint8_t target){completeTargetPick(target);};cell->pickHover=[this](uint8_t target){pickHoverTarget=target;};cell->openDsnd=[this](juce::Point<int> at){showDsndMenu(at);};addAndMakeVisible(*cell);}
        for(auto* button:{&machineButton,&menuButton,&tempoButton,&arpButton,&arpSettingsButton,&matrixButton,&msegButton})addAndMakeVisible(*button);for(auto* lfo:{&lfo1Button,&lfo2Button,&lfo3Button})addAndMakeVisible(*lfo);addAndMakeVisible(msegOutputButton);msegOutputButton.textHeight=12;msegOutputButton.setTooltip("Drag this external MSEG output point onto a Matrix target or parameter.");
        // 1.6.5: три одинаковые кнопки LFO1/2/3. Клик -- показать LFO в правой
        // нижней панели; пресс-и-тащи с отпусканием на ручке -- назначить
        // маршрут модуляции без второго клика.
        // 1.6.8: три ОДИНАКОВЫЕ кнопки LFO1/2/3 (один размер, один стиль; выбранная
        // -- залита, см. setToggleState в bind()). ЛКМ-тащить = прямой PAGE/DEST
        // этого LFO на наведённую ручку, ПКМ-тащить = маршрут в матрицу.
        auto wireLfo=[this](ModSourceButton& b,int i){
            b.textHeight=24;
            b.beginDrag=[this,i](bool matrix){beginModDrag(4+i,matrix);};
            b.dragMove=[this](juce::Point<int> s){modDragMove(s);};
            b.endDrag=[this,i](juce::Point<int> s,bool dragged,bool matrix){endModDrag(s,dragged,i,matrix);};
            b.holdHover=[this,i]{holdLfoPage(i);}; // 1.6.13: секунда удержания = перейти на страницу этого LFO
            b.setTooltip("LFO"+juce::String(i+1)+": click = show this LFO panel; LEFT-drag onto a knob = this LFO modulates it directly (its own PAGE/DEST, no matrix); RIGHT-drag onto a knob = create a matrix route. While aiming: hold over another LFO button for 1 s to switch to its page.");
        };
        wireLfo(lfo1Button,0);wireLfo(lfo2Button,1);wireLfo(lfo3Button,2);
        addAndMakeVisible(dspModeButton);dspModeButton.onClick=[this]{showDspModes(-1);};updateDspModeLabel();
        addAndMakeVisible(ampButton);ampButton.setButtonText("AMP");ampButton.textHeight=28;ampButton.textAlign=1;ampButton.onClick=[this]{showEnvelope(-1);};
        // 1.6.14: выбор "amp old" удалён как бесполезный -- огибающая всегда mnm.
        syntModeCombo.speedKey="MODE SYNT";filtModeCombo.speedKey="MODE FILT";effxDistCombo.speedKey="MODE DIST";effxDlyCombo.speedKey="MODE DLY";
        syntModeCombo.addItem("fma",3); // 1.6.14: новый режим FM (proper DIST), у каждой машины свой
        for(auto* c:{&syntModeCombo,&filtModeCombo,&effxDistCombo,&effxDlyCombo}){addAndMakeVisible(*c);c->setScrollWheelEnabled(true);} // 1.6.6 header MODE combos
        level.setSliderStyle(juce::Slider::LinearVertical);level.setTextBoxStyle(juce::Slider::NoTextBox,false,0,0);
        level.setColour(juce::Slider::trackColourId,ink);level.setColour(juce::Slider::thumbColourId,ink);level.setColour(juce::Slider::backgroundColourId,juce::Colours::black);addAndMakeVisible(level);
        levelLink=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters,"level",level);
        machineButton.textAlign=1;
        // 1.6.13: стрелки по бокам названия (влево/вправо, клик = один шаг пресета),
        // драг прокрутки ЗАМЕДЛЕН (18 px на шаг + множитель MACHINE из GUI DRAG SPEED),
        // хитбоксы прежние.
        machineButton.arrows=true;machineButton.arrowClick=[this](int d){cycleMachine(d);};
        machineButton.onClick=[this]{showMachineMenu();};
        machineButton.dragPixelsPerStep=18;
        machineButton.dragHandler=[this](int steps,bool){machineAccum+=static_cast<float>(steps)*static_cast<float>(uiSpeed("MACHINE",0.4));
            const int whole=static_cast<int>(machineAccum>=0?std::floor(machineAccum):std::ceil(machineAccum));
            if(whole!=0){machineAccum-=static_cast<float>(whole);cycleMachine(whole);}};
        menuButton.onClick=[this]{showMenu();};
        // BPM drags to change tempo; it no longer hijacks a settings page.
        tempoButton.textHeight=16;tempoButton.outlined=false;tempoButton.textAlign=1;
        // 1.6.12: BPM целыми; Alt/Shift -- десятые. Буквы BPM мельче.
        tempoButton.dragHandler=[this](int steps,bool fine){nudgeBpm(static_cast<float>(steps)*(fine?0.1f:1.0f)*static_cast<float>(uiSpeed("BPM")));};
        addAndMakeVisible(tempoSync);tempoSync.setButtonText("SYNC"); // 1.6.14: без "BPM", ближе к значению
        // 1.6.12: SYNC на главной панели -- это синх ARPA (arp_sync), а не глобальный
        // host_sync: глобальный темп-синк больше не выключается этой кнопкой.
        tempoSyncLink=std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(p.parameters,"arp_sync",tempoSync);
        tempoSync.setTooltip("ARP clock only: SYNC = host BPM grid, TIME = free ms. Global host tempo sync lives in ARP page (HOST TEMPO).");
        addChildComponent(tempoEntry);tempoEntry.setName("BPM entry");tempoEntry.setInputRestrictions(7,"0123456789.");tempoEntry.onEscapeKey=[this]{tempoEntry.setVisible(false);};tempoEntry.onFocusLost=[this]{tempoEntry.setVisible(false);};
        tempoEntry.onReturnKey=[this]{if(tempoEntry.getText().isNotEmpty()){auto* bpm=processor.parameters.getParameter("bpm");bpm->beginChangeGesture();bpm->setValueNotifyingHost(bpm->convertTo0to1(juce::jlimit(30.0f,300.0f,tempoEntry.getText().getFloatValue())));bpm->endChangeGesture();setHostSyncOff();}tempoEntry.setVisible(false);update();};
        tempoButton.doubleClickHandler=[this]{tempoEntry.setText(tempoButton.getButtonText(),false);tempoEntry.setVisible(true);tempoEntry.toFront(true);tempoEntry.grabKeyboardFocus();tempoEntry.selectAll();};
        arpButton.dragPixelsPerStep=2;arpButton.dragHandler=[this](int steps,bool){arpAccum+=static_cast<float>(steps)*static_cast<float>(uiSpeed("ARP RATE"));
            const int whole=static_cast<int>(arpAccum>=0?std::floor(arpAccum):std::ceil(arpAccum));if(whole==0)return;arpAccum-=static_cast<float>(whole);
            const bool sync=processor.parameters.getRawParameterValue("arp_sync")->load()>0.5f;auto* par=processor.parameters.getParameter(sync?"arp_grid":"arp_time");
            const float value=par->convertFrom0to1(par->getValue())+static_cast<float>(whole)*(sync?1.0f:10.0f);
            par->beginChangeGesture();par->setValueNotifyingHost(par->convertTo0to1(juce::jlimit(sync?0.0f:5.0f,sync?15.0f:2000.0f,value)));par->endChangeGesture();update();};
        arpButton.textHeight=18;arpButton.rightClick=[this]{showSettings(false);}; // 1.6.14: ПКМ по ARP = страница ARP
        arpSettingsButton.textHeight=12;arpSettingsButton.setTooltip("Open the ARP step editor (press again to close)");
        // 1.6.12: SET -- переключатель: повторное нажатие закрывает страницу ARP.
        arpSettingsButton.onClick=[this]{if(settings&&viewport.isVisible()&&viewport.getViewedComponent()==settings.get())closeOverlay();else showSettings(false);};
        matrixButton.textHeight=22;matrixButton.onClick=[this]{showSettings(true);};
        // 1.6.5: кнопки MSEG переехали в освободившуюся верхнюю правую панель
        // (ранее LFO1), рядом с быстрыми ручками RATE/SYNC/LOOP.
        msegButton.textHeight=16;msegButton.onClick=[this]{showMseg();};msegButton.setTooltip("Open the multi-segment modulation editor. Drag OUTPUT to a parameter or a matrix target.");
        closeButton.textHeight=20;closeButton.onClick=[this]{closeOverlay();};
        addChildComponent(viewport);addChildComponent(closeButton);
        addAndMakeVisible(arpEnabled);arpEnabled.setTooltip("Enable arpeggiator");
        arpLink=std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(p.parameters,"arp_on",arpEnabled);
        if(!processor.isSynthVersion){addAndMakeVisible(trigToggle);trigToggle.setButtonText("MIDI");trigLink=std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(p.parameters,"gate",trigToggle);}
        viewport.setScrollBarsShown(true,false);viewport.setScrollBarThickness(14);
        viewport.getVerticalScrollBar().setColour(juce::ScrollBar::thumbColourId,juce::Colours::white.withAlpha(0.7f));
        setWantsKeyboardFocus(true); // 1.6.14: ESC закрывает страницы
        startTimerHz(10); // 1.6.14: удержание над кнопкой другого LFO при прицеле
        setSize(1260,584);bind();update();
    }
    ~Surface()override{stopTimer();viewport.setViewedComponent(nullptr);setLookAndFeel(nullptr);}
    // 1.6.14: ESC выходит из матрицы/арпа/других поверхностных окон.
    bool keyPressed(const juce::KeyPress& k)override{
        if(k.isKeyCode(juce::KeyPress::escapeKey)){if(viewport.isVisible()||pickingTarget)closeOverlay();return true;}
        return false;
    }
    void mouseDown(const juce::MouseEvent&)override{grabKeyboardFocus();} // чтобы ESC доходил
    // 1.6.14: при перетягивании прицела удержание над кнопкой другого LFO (1 с)
    // переключает панель на его страницу -- работает и без движения мыши.
    void timerCallback()override{
        if(modDragSrc<4||modDragMatrix){lfoHoldIndex=-1;return;}
        int over=-1;
        juce::Component* btns[3]={&lfo1Button,&lfo2Button,&lfo3Button};
        for(int i=0;i<3;++i)if(btns[i]->getScreenBounds().contains(modDragScreen)){over=i;break;}
        if(over>=0&&over!=modDragSrc-4){
            const auto now=juce::Time::getMillisecondCounter();
            if(lfoHoldIndex!=over){lfoHoldIndex=over;lfoHoldSince=now;}
            else if(now-lfoHoldSince>=1000){lfoHoldIndex=-1;lfoPageSel=juce::jlimit(0,2,over);bind();}
        } else lfoHoldIndex=-1;
    }
    void bind(){
        boundMachine=processor.machineIndex();updateDspModeLabel();const auto& m=nova::machines()[static_cast<size_t>(boundMachine)];machineButton.setButtonText(m.name);machineButton.textHeight=28; // 1.6.13: стрелки -- по бокам текста (кликабельные), не декорация в конце
        for(int i=0;i<8;++i){const auto& def=m.synthParams[static_cast<size_t>(i)];const bool hideSw=(m.id==4&&i==3);cells[0][static_cast<size_t>(i)]->bind(hideSw?juce::String():(def.maxVal?nova::machineParam(m.id,i):juce::String()),hideSw?"---":def.name,-1,i,m.id,!hideSw);} // 1.6.6: SWAVE MODE lives in the SYNT header
        // 1.6.8: панели LFO и MSEG поменялись местами. LFO теперь в верхней правой
        // панели (панель 2), MSEG с быстрыми ручками RATE/SYNC/LOOP -- в нижней
        // правой (панель 5, где LFO был раньше). Быстрые ручки MSEG -- не цели
        // модуляции, правая нижняя панель показывает выбранный LFO... точнее,
        // выбранный LFO теперь сверху, а MSEG -- снизу.
        cells[5][0]->bind("mseg_rate","RATE",-1,0,-1,false);
        cells[5][1]->bind("mseg_sync","SYNC",-1,1,-1,false);
        cells[5][2]->bind("mseg_loop","LOOP",-1,2,-1,false);
        for(int i=3;i<8;++i)cells[5][static_cast<size_t>(i)]->bind(juce::String(),"---",-1,i,-1,false);
        // 1.6.8: панель 2 -- выбранная страница LFO (PAGE/DEST/TRIG/WAVE/MULT/SPD/INTL/DPTH),
        // пропускается только панель 5 (там быстрые ручки MSEG). Страницу LFO
        // переключают кнопки LFO1/2/3 в шапке; крутится она ручками панели.
        const int pageForPanel[]={-1,0,3+lfoPageSel,1,2,-1};
        for(int panel=1;panel<6;++panel){if(panel==5)continue;for(int i=0;i<8;++i){int page=pageForPanel[panel];cells[static_cast<size_t>(panel)][static_cast<size_t>(i)]->bind(nova::pageParam(page,i),nova::pageLabel(page,i),page,i);}}
        rebuildModeControls(); // 1.6.6
        // 1.6.8: выбранная LFO-страница -- ЗАЛИТАЯ кнопка (toggle state), а не
        // скобки в тексте: все три кнопки выглядят одинаково и стоят ровно.
        lfo1Button.setToggleState(lfoPageSel==0,juce::dontSendNotification);
        lfo2Button.setToggleState(lfoPageSel==1,juce::dontSendNotification);
        lfo3Button.setToggleState(lfoPageSel==2,juce::dontSendNotification);
        if(!processor.isSynthVersion)updateGateLabel();
        repaint();
    }
    // 1.6.5: модуляция «прицелом» перетаскиванием с кнопок LFO1/2/3:
    // зажал кнопку, довёл до ручки, отпустил -- маршрут назначен (без второго
    // клика, в отличие от матрицы).
    void beginModDrag(int src,bool matrix){modDragSrc=src;modDragMatrix=matrix;modHover=nullptr;for(auto& column:cells)for(auto& cell:column)if(cell)cell->setPickMode(true);}
    // 1.6.8: прицеливание кнопкой LFO1/2/3.
    // ЛКМ: наведение на ручку СРАЗУ переключает внутренний адресат этого LFO --
    // PAGE (1-й параметр страницы LFO) и DEST (2-й параметр, 0..7), заменяя
    // текущий. Матрица не участвует. PTCH|SYNT|AMP|FILT|EFFX|LFO1|LFO2|LFO3.
    // ПКМ: только подсветка; маршрут в матрицу создаётся при отпускании.
    void aimLfoDest(uint8_t target){
        if(modDragSrc<4||modDragSrc>6)return; // прицел только с кнопок LFO1/2/3
        const int lfo=modDragSrc-4, page=3+lfo;
        auto set=[this](const juce::String& id,float value){if(auto* p=processor.parameters.getParameter(id)){p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(value));p->endChangeGesture();}};
        set(nova::pageParam(page,0),static_cast<float>(target/8+1)); // PAGE: 1=SYNT..7=LFO3
        set(nova::pageParam(page,1),static_cast<float>(target%8));   // DEST: параметр внутри страницы
    }
    void modDragMove(juce::Point<int> screen){modDragScreen=screen;auto* cell=cellAtScreen(screen);if(cell!=modHover){
        if(modHover)modHover->setHover(false); // 1.6.13: подсветка снимается МГНОВЕННО, без шлейфа
        modHover=cell;
        if(cell){cell->setHover(true);if(!modDragMatrix&&cell->canBeTarget())aimLfoDest(cell->targetId());}}}
    void endModDrag(juce::Point<int> screen,bool dragged,int lfoIndex,bool matrix){
        for(auto& column:cells)for(auto& cell:column)if(cell)cell->setPickMode(false);
        const int src=modDragSrc;modDragSrc=-1;if(modHover)modHover->setHover(false);modHover=nullptr;modDragMatrix=false;lfoHoldIndex=-1;
        if(src<0)return;
        if(dragged){
            auto* cell=cellAtScreen(screen);
            if(cell&&cell->canBeTarget()){
                if(matrix)processor.addRouteFromSource(src,cell->targetId()); // ПКМ: маршрут в матрицу
                else{ // 1.6.12: ЛКМ -- прямой PAGE/DEST; панель сразу показывает этот LFO
                    aimLfoDest(cell->targetId());lfoPageSel=juce::jlimit(0,2,lfoIndex);bind();
                }
                cell->flashPick();
            }
        }
        else if(!matrix){lfoPageSel=juce::jlimit(0,2,lfoIndex);bind();} // клик ЛКМ -- показать страницу этого LFO
        repaint();
    }
    Cell* cellAtScreen(juce::Point<int> screen){for(auto& column:cells)for(auto& cell:column)if(cell&&cell->getScreenBounds().contains(screen))return cell.get();return nullptr;}
    // 1.6.13: в режиме прицела удержание над кнопкой другого LFO (1 с) показывает его страницу.
    void holdLfoPage(int i){ if(modDragSrc>=4&&!modDragMatrix&&modDragSrc-4!=i){ lfoPageSel=juce::jlimit(0,2,i); bind(); } }
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
        pixel::text(g,"BPM",{1006,2,40,14},12);pixel::text(g,"LEV",{7,56,58,32},28); // 1.6.14: BPM-подпись над цифрами, SYNC слева
        pixel::dotted(g,8,94,8,558);pixel::dotted(g,61,94,61,558);pixel::dotted(g,8,94,63,94);pixel::dotted(g,8,558,63,558);
        const juce::String lfoPanelName="LFO"; // 1.6.8: без цифры -- номер страницы виден на кнопках [LFO1]/[LFO2]/[LFO3]
        const juce::String names[]{"SYNT","",lfoPanelName,"FILT","EFFX","MSEG"}; // 1.6.8: LFO наверху, MSEG внизу
        for(int panel=0;panel<6;++panel){int x=72+(panel%3)*399,y=94+(panel/3)*240;
            g.setColour(ink.withAlpha(0.35f));g.drawRect(x,y,387,236,1);g.setColour(ink);g.drawRect(x,y,387,30,2);pixel::text(g,names[panel],{x+5,y,377,30},28);
            g.setColour(ink);for(int c=0;c<=4;++c)pixel::dotted(g,x+c*96,y+34,x+c*96,y+228);
            pixel::dotted(g,x,y+34,x+388,y+34);pixel::dotted(g,x,y+130,x+388,y+130);pixel::dotted(g,x,y+228,x+388,y+228);}

        g.setColour(ink.withAlpha(0.4f));g.drawRect(8,94,55,464,2);
        g.setColour(ink.withAlpha(0.65f));g.drawText(processor.isSynthVersion?"BUILD 1.6.16 / Synth":"BUILD 1.6.16 / FX",850,570,370,13,juce::Justification::right); // версия сборки видна всегда
    }
    void resized()override{
        ampButton.setBounds(473,95,120,28); // 1.6.14: ampMode-комбо удалено
        syntModeCombo.setBounds(317,97,140,25);filtModeCombo.setBounds(317,337,140,25);effxDistCombo.setBounds(568,337,140,25);effxDlyCombo.setBounds(713,337,140,25); // 1.6.6
        machineButton.setBounds(72,58,387,30);dspModeButton.setBounds(1014,58,140,30);menuButton.setBounds(1161,4,94,32);tempoSync.setBounds(934,4,68,30);tempoButton.setBounds(1006,4,100,30);tempoEntry.setBounds(1006,4,100,30); // 1.6.14: SYNC вплотную к BPM
        arpEnabled.setBounds(471,58,26,30);arpButton.setBounds(500,58,120,30);arpSettingsButton.setBounds(624,58,34,30);trigToggle.setBounds(700,58,140,30);matrixButton.setBounds(870,58,140,30);
        lfo1Button.setBounds(975,97,90,24);lfo2Button.setBounds(1071,97,90,24);lfo3Button.setBounds(1167,97,90,24); // 1.6.8: три ОДИНАКОВЫЕ кнопки, ровно в шапке LFO-панели
        msegButton.setBounds(1000,337,90,24);msegOutputButton.setBounds(1096,337,110,24); // 1.6.8: MSEG-кнопки в шапке нижней правой панели
        level.setBounds(12,97,46,458);
        for(int panel=0;panel<6;++panel)for(int i=0;i<8;++i)cells[static_cast<size_t>(panel)][static_cast<size_t>(i)]->setBounds(75+(panel%3)*399+(i%4)*96,130+(panel/3)*240+(i/4)*96,91,91);
        viewport.setBounds(65,88,1192,480);closeButton.setBounds(870,58,140,30);gateButton.setVisible(false); // 1.6.12: BACK ровно на месте MATRIX
    }
    // 1.6.12: единое закрытие оверлея; SET теперь переключатель (повторное
    // нажатие закрывает страницу ARP -- без мигания пересозданной страницы).
    void closeOverlay(){
        if(pickingTarget){for(auto& column:cells)for(auto& cell:column)if(cell)cell->setPickMode(false);pickingTarget=false;}
        viewport.setViewedComponent(nullptr);settings.reset();matrixPage.reset();samplePage.reset();msegPage.reset();envelopePage.reset();viewport.setVisible(false);closeButton.setVisible(false);repaint();
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
        { // 1.6.12: замок маршрута защищает dest от прицела
          auto* lk=processor.parameters.getParameter("r"+juce::String(pickRoute)+"_lock");
          if(lk&&juce::roundToInt(lk->convertFrom0to1(lk->getValue()))>0){for(auto& column:cells)for(auto& cell:column)if(cell)cell->setPickMode(false);pickingTarget=false;viewport.setVisible(true);viewport.toFront(false);repaint();return;}
        }
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
        menu.setLookAndFeel(&getLookAndFeel()); // 1.6.14: чёрный пиксельный скин гарантиован
        for(size_t i=0;i<nova::machines().size();++i){const auto& m=nova::machines()[i];if(category!=juce::String(m.category)){category=m.category;menu.addSectionHeader(category);}
            auto shortName=juce::String(m.name);if(shortName.startsWith(m.category))shortName=shortName.substring(static_cast<int>(m.category.size())).trimCharactersAtStart("- ");
            menu.addItem(static_cast<int>(i)+1,shortName,true,static_cast<int>(i)==boundMachine);}

        juce::Component::SafePointer<Surface> safe(this);
        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(&machineButton).withMinimumWidth(320),[safe](int result){if(!safe||result<=0)return;
            auto* p=safe->processor.parameters.getParameter("machine");p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(result-1)));p->endChangeGesture();safe->bind();});
    }
    // DSP mode list, 1.6.5: разделы сгруппированы по папкам-категориям
    // (dspSectionCategory), ROUTE убран (цепочка одна -- справка отдельным
    // подменю), каждая строка начинается с имени своего раздела.
    // 1.6.8: все подписи на английском (крякозябры были из-за \\xNN UTF-8 в
    // juce::String(const char*)); попап открывается у курсора (жалоба на SRR);
    // режим SYNT пишется в параметр выбранной машины (mode_synt_m<id>);
    // в конце меню -- проверочная строка ENCODING TEST со словом "проверка"
    // в СТАРОЙ сломанной кодировке, чтобы видеть, вернулись ли кракозябры.
    void showDspModes(int section,juce::Point<int> screen=juce::Point<int>(-1,-1)){
        // 1.6.12: по просьбе вернули ПРЕЖНЕЕ отображение по группам-папкам
        // (подменю на раздел), всё на английском, попап у курсора (SRR),
        // в конце -- ENCODING TEST со словом "проверка" в старой сломанной
        // кодировке (канарейка: оно ДОЛЖНО выглядеть кракозябрами).
        juce::PopupMenu menu;
        menu.setLookAndFeel(&getLookAndFeel()); // 1.6.14: чёрный пиксельный скин гарантиован
        auto addSection=[&](int index){
            if(!monomachine::dspSectionVisibleInMenu(index))return; // ROUTE: переключателя больше нет
            const int current=processor.dspMode(index);
            const int primary=monomachine::dspModePrimaryForSection(index);
            juce::PopupMenu sub;
            sub.addItem(-1,juce::String("FOLDER: ")+monomachine::dspSectionCategory(index),false,false);
            sub.addItem(index*100+primary+1,
                        juce::String(monomachine::dspModeProvenanceForMode(index,primary)),
                        true, current==primary);
            if(monomachine::kShowBackupModes){
                bool any=false;
                for(int m=0;m<monomachine::dspModeCount;++m){
                    if(m==primary)continue;
                    if(!monomachine::dspModeAllowedForSection(index,m))continue;
                    if(!any){sub.addSeparator();sub.addItem(-1,"BACKUP (not the primary engine):",false,false);any=true;}
                    sub.addItem(index*100+m+1,
                                juce::String(monomachine::dspModeProvenanceForMode(index,m)),
                                true, current==m);
                }
            }
            sub.addSeparator();
            sub.addItem(-1, monomachine::dspModeProvenance(index), false, false);
            juce::String title=juce::String(monomachine::dspSectionCategory(index))+" / "+juce::String(monomachine::dspSectionLabel(index));
            if(index==monomachine::DspSynt)title+=" ["+juce::String(nova::machines()[static_cast<size_t>(processor.machineIndex())].name)+"]"; // режим SYNT у каждой машины свой
            title+="   ["+juce::String(monomachine::dspModeLabel(current))+"]";
            menu.addSubMenu(title,sub);
        };
        if(section>=0&&section<monomachine::DspSectionCount)addSection(section);
        else for(int i=0;i<monomachine::DspSectionCount;++i)addSection(i);
        if(section<0||section==monomachine::DspRouting){
            juce::PopupMenu route;
            route.addItem(-1,"ONE CHAIN ONLY (VERIFIED), THE SWITCH WAS REMOVED IN 1.6.5",false,false);
            for(const auto& line:juce::StringArray::fromLines(juce::String(monomachine::dspRouteDescription())))
                if(line.isNotEmpty())route.addItem(-1,line,false,false);
            menu.addSubMenu("CHAIN / ROUTE   [fixed]",route);
        }
        menu.addSeparator();
        menu.addItem(-1,juce::String("ENCODING TEST: ")+juce::String("\xd0\xbf\xd1\x80\xd0\xbe\xd0\xb2\xd0\xb5\xd1\x80\xd0\xba\xd0\xb0")+juce::String(" (= 'proverka', broken on purpose)"),false,false);
        juce::Component::SafePointer<Surface> safe(this);
        juce::PopupMenu::Options options=juce::PopupMenu::Options().withMinimumWidth(360).withTargetComponent(this); // 1.6.16: ширина по контенту
        if(screen.x>=0&&screen.y>=0)options=options.withTargetScreenArea(juce::Rectangle<int>(screen.x,screen.y,1,1)); // попап у курсора (SRR)
        else options=options.withTargetComponent(&dspModeButton);
        menu.showMenuAsync(options,[safe](int result){
            if(!safe||result<=0)return;
            const int index=(result-1)/100,mode=(result-1)%100;
            if(index<0||index>=monomachine::DspSectionCount)return;
            if(mode<0||mode>=monomachine::dspModeCount)return;
            if(!monomachine::dspModeAllowedForSection(index,mode))return;   // чужой режим в раздел не попадёт
            auto* p=safe->processor.parameters.getParameter(safe->processor.dspModeParamIdFor(index));if(!p)return; // SYNT -- параметр выбранной машины
            p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(mode)));p->endChangeGesture();
            safe->updateDspModeLabel();});
    }
    void updateDspModeLabel(){
        dspModeButton.setButtonText("DSP MODE");
        // 1.6.5: тултип сгруппирован по папкам, строки начинаются с имени
        // своего раздела; в конце -- подробное описание цепочки (ROUTE).
        // 1.6.8: английский текст (см. DspModes.hpp), SYNT -- режим выбранной машины.
        juce::String tip="DSP MODE -- section folders, primary mode is mnm:\n";
        for(int i=0;i<monomachine::DspSectionCount;++i){
            if(!monomachine::dspSectionVisibleInMenu(i))continue;
            const int mode=processor.dspMode(i);
            tip+=juce::String(monomachine::dspSectionCategory(i))+" / "+juce::String(monomachine::dspSectionLabel(i));
            if(i==monomachine::DspSynt)tip+=" ["+juce::String(nova::machines()[static_cast<size_t>(processor.machineIndex())].name)+"]";
            tip+=" ["+juce::String(monomachine::dspModeLabel(mode))+"]";
            tip+= (mode==monomachine::dspModeMnm) ? " (primary)" : " (reserve)";
            tip+=" - "+juce::String(monomachine::dspModeProvenanceForMode(i,mode))+"\n";
        }
        tip+="\n";
        tip+=juce::String(monomachine::dspRouteDescription());
        dspModeButton.setTooltip(tip);
    }
    // 1.6.6/1.6.8: RMB on DTIM -- how hard repeats repitch while sweeping delay
    // time. Now a SLIDER (OFF/FAST/MED/SLOW = 0..3) in a call-out box at the
    // cursor, not a list. The name showRepitchMenu is kept for the verify script.
    // 1.6.12: панель замков PAGE/DEST LFO у курсора.
    // 1.6.14: после запуска callout возвращаем фокус хосту -- QWERTY-ноты играют,
    // пока окно открыто (фокус ему больше не нужен: там только мышь).
    void showCallout(std::unique_ptr<juce::Component> panel,juce::Point<int> screen){
        juce::CallOutBox::launchAsynchronously(std::move(panel),juce::Rectangle<int>(screen.x,screen.y,1,1),nullptr);
        juce::Component::unfocusAllComponents();
    }
    void showLfoLocks(int lfo,bool dest,juce::Point<int> screen){showCallout(std::make_unique<LfoLockPanel>(processor,lfo,dest),screen);}
    void showRepitchMenu(juce::Point<int> screen){showCallout(std::make_unique<RepitchSliderPanel>(processor),screen);}
    void showDsndMenu(juce::Point<int> screen){showCallout(std::make_unique<DsndPanel>(processor),screen);}
    void showPortaMenu(juce::Point<int> screen){showCallout(std::make_unique<PortaPanel>(processor),screen);} // 1.6.14
    // 1.6.6: MODE selectors on page headers (like the envelope REFERENCE FIT):
    // SYNT header carries the section mode, or the machine's own algorithm
    // choice (SWAVE-SAW) when that machine is selected; FILT and EFFX headers
    // expose their sections directly.
    void rebuildModeControls(){
        auto wire=[this](juce::ComboBox& c,const juce::String& paramId,std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>& link,const char* a,const char* b){
            link.reset();c.clear(juce::dontSendNotification);c.addItem(a,1);c.addItem(b,2);
            link=std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(processor.parameters,paramId,c);};
        if(processor.machineIndex()>=0&&nova::machines()[static_cast<size_t>(processor.machineIndex())].id==4)
            wire(syntModeCombo,"m4_3",syntModeLink,"ALG A","ALG B");
        else wire(syntModeCombo,processor.dspModeParamIdFor(monomachine::DspSynt),syntModeLink,"mnm","old"); // 1.6.8: параметр выбранной машины
        wire(filtModeCombo,"mode_filt",filtModeLink,"mnm","old");
        wire(effxDistCombo,"mode_dist",effxDistLink,"mnm","old");
        wire(effxDlyCombo,"mode_dly",effxDlyLink,"mnm","old");
        syntModeCombo.setTooltip("SYNT mode of THIS machine (mnm = firmware DSP, old = reserve). Each machine remembers its own mode while you cycle machines. For SWAVE-SAW: oscillator algorithm.");
        filtModeCombo.setTooltip("FILT mode: mnm = real firmware filter dump, old = previous biquad.");
        effxDistCombo.setTooltip("SRR/DIST mode.");
        effxDlyCombo.setTooltip("DLY mode: mnm = firmware FX-DLY line, old = previous delay.");
    }
    void showSettings(bool matrix){setViewportMode(matrix);}
    void showMenu(){juce::PopupMenu menu;menu.setLookAndFeel(&getLookAndFeel()); // 1.6.14: чёрный скин
        menu.addItem(1,"TEMPO / ARPEGGIATOR / GLOBAL");menu.addItem(2,"MODULATION MATRIX (16 ROUTES)");menu.addItem(8,"MSEG / CURVE EDITOR");
        if(processor.isSynthVersion){menu.addItem(7,"BBOX SAMPLES / DROP FILES");menu.addItem(3,"RESTORE NATIVE KIT");}
        menu.addSeparator();menu.addItem(4,"PANIC / CLEAR TAILS");menu.addItem(5,"RESET ALL PARAMETERS");menu.addItem(6,"ABOUT / LIMITATIONS");
        juce::Component::SafePointer<Surface> safe(this);menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(&menuButton),[safe](int result){if(!safe)return;
            if(result==1||result==2)safe->setViewportMode(result==2);if(result==8)safe->showMseg();
            if(result==3)safe->processor.resetSamples();if(result==4)safe->processor.requestPanic();
            if(result==5){safe->processor.requestPanic();for(auto* parameter:safe->processor.getParameters()){parameter->beginChangeGesture();parameter->setValueNotifyingHost(parameter->getDefaultValue());parameter->endChangeGesture();}safe->bind();}
            if(result==6)juce::AlertWindow::showMessageBoxAsync(juce::MessageBoxIconType::InfoIcon,"Monomachine Nova Synth / BUILD 1.6.16","Build 1.6.16 (+1 every patch):\nARP page scrolls with the mouse wheel again (the LMB-drag scroll is gone).\nARP lane: RMB erases only the row you are on (TR -> transpose, VEL -> velocity, HOLD -> hold); hover any pad for the control hint.\nRANDOM of steps no longer spams host automation (Ableton lag fixed): bulk changes are written without host notifications.\nHOST TEMPO is now the ONLY arp rate mode switch (SYNC/TIME buttons removed as duplicates); GATE is a plain value box (slider removed).\nPopup menus: bigger pixel font (up to 21 px), sane widths.\nLock panel: proper DEST names (machine knob names on PTCH, LFO1..3 prefixes on LFO pages).\n1.6.15: Menus are pixel-black everywhere (top-level menus carry the theme explicitly). Machine arrows are a proper < > pair right of the name. GUI DRAG SPEED grid now really shows in ARP page (bottom) -- per-list scroll feel, UI-only. LFO page switch is instant (cells skip rebind); hold the aim over another LFO button for 1 s to switch pages (works without moving). Locks draw full white (matrix + LFO panel), lock RMB menu opens at the cursor, hover shows the mode. AMP 'old' DSP removed -- always mnm; envelope page always mnm. ARP: page = square value box (default 1) + limit; STEP RND regenerates the page each cycle; lane paints to the edges (velocity floor/ceiling reachable), HOLD row taller; no wheel-scroll, drag empty space to scroll. PORTA gets its own panel (RMB on PORT): faster, explicit glide + SPEED. New FM mode 'fma': firmware FM core with PROPER DIST (soft saturation, no ratio-like fold), per machine. ESC closes matrix/arp pages; callouts no longer eat the QWERTY keyboard; RMB on the main ARP button opens the ARP page.\n1.6.13: All popup menus (DSP MODE, matrix, knob lists) use the black pixel skin now, item text centred.\nMachine name has clickable arrows on both sides (one preset per click); machine scrolling is slower; GUI DRAG SPEED page (MENU > TEMPO/GLOBAL, bottom) stores a per-control scroll multiplier -- UI-only, never a VST automation parameter.\nARP: ONE unified RATE button (grid or ms per the HOST TEMPO checkbox); SYNC and TIME switch the mode by plain click; HOST TEMPO is arp-only and never touches the plugin-wide host sync; stray slider flash on SET is gone.\nARP lane: TR drags from the middle (bipolar), VEL from the bottom (unipolar), HOLD has its own button row (paint with LMB), RMB drag erases velocity+transpose.\nRANDOM: PAGE and ALL kept, PITCH and VEL added below; ALL randomizes 128 steps in one lag-free batch.\nBBOX file list: PLAY button per row (audition a slot), white scrollbar.\nDelay: DSND gets its own ping-pong panel; DTIM keeps REPITCH/SMOOTH only.\nMatrix: DEST column text centred, sort crash hardened; LFO aim: instant highlight (no trail), hold over another LFO button for 1 s to switch pages, lock marks can be drag-painted; RMB on HOLD opens the envelope like other knobs.\n1.6.12: DSP MODE menus are grouped into folder submenus again.\nARP: SET toggles the page; strict pad hitboxes (velocity no longer changes while painting transpose); RMB drag paints HOLD; PAGE/velocity/rate/sync/mode lists drag with LMB; OCT+GATE sit next to RATE; PAGE LIMIT and RANDOM are value boxes; RANDOM has PAGE and ALL buttons.\nSYNC button on the main page = ARP clock only.\nBACK sits exactly where MATRIX was. BPM drags in whole steps (Alt/Shift = tenths), smaller BPM letters.\nMatrix: DEST grouped by pages (MACHINE/AMP/FILTER/EFFX/LFO1..3); per-route LOCK button (LMB toggle, RMB = FREE/LOCK/SOLO).\nLFO PAGE/DEST: RMB opens the lock panel -- blacklist values or SOLO one value; modulation skips locked values.\nLFO INTL now works per the manual: interlace (wave alternating with zero), 0 = off.\nBBOX: gentler STRT (cubic, 25 ms cap) and adaptive de-click (transient kept, strong click removed); dropped files show gray WILL LOAD slots.\nDelay ping-pong: new MID SAFE mode (RMB on DTIM/DSND panel, PP MODE row).\nEnvelope modes renamed: old (= legacy 1.4) and mnm (ex REFERENCE FIT).\nPrevious 1.6.9-1.6.11: LFO page knobs restored, LFO buttons LMB aim / RMB matrix, ENCODING TEST line, per-machine SYNT mode.\nBase 1.6.5: AMP editor, MIDI/LATCH, FX unity calibration.\nNative implementations; not an exact emulator.\nFX-CHORUS runs the translated OS 1.32B core (24-bit fixed point, DEL/DEP/SPD/MIX/FB/WID/LP/INP).\nINP is input gain everywhere, DIST/DSND are bipolar, DSND<0 pings across.\nFX: MIDI OFF = latch holds AMP open; MIDI ON = note envelope gate.\nAHD: HOLD=127 sustains. MENU contains arp and 8 modulation routes.\nBBOX: 24 native sample slots, CHRM keyboard map and embedded custom PCM. Factory ROM audio is not included.\nLFO/ARP use host BPM but are note-synchronised, not timeline-locked.\nNo oversampling or output limiter. Turn down LEV before testing.");
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
    PixelButton machineButton,menuButton,tempoButton,arpButton,arpSettingsButton,matrixButton,msegButton;ModSourceButton lfo1Button,lfo2Button,lfo3Button;MsegOutputButton msegOutputButton;PixelButton closeButton,gateButton;juce::Slider level;PixelButton dspModeButton{"DSP MODE"};
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> levelLink;
    juce::ToggleButton arpEnabled,trigToggle,tempoSync;juce::TextEditor tempoEntry;std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> tempoSyncLink;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> arpLink,trigLink;
    PixelButton ampButton{"AMP"};DragChoice syntModeCombo,filtModeCombo,effxDistCombo,effxDlyCombo; // 1.6.14: ampMode удалён (old AMP DSP удалён)
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> syntModeLink,filtModeLink,effxDistLink,effxDlyLink; // 1.6.6
    std::unique_ptr<EnvelopePage> envelopePage;
    std::unique_ptr<SamplePage> samplePage;std::unique_ptr<MsegPage> msegPage;
    std::unique_ptr<ArpPage> settings;std::unique_ptr<MatrixPage> matrixPage;FramedViewport viewport;std::unique_ptr<juce::FileChooser> chooser;
    juce::TooltipWindow tooltips{this,800};int lfoPageSel=1;int modDragSrc=-1;bool modDragMatrix=false;Cell* modHover=nullptr;int boundMachine=-1;bool pickingTarget=false;int pickRoute=-1,pickHoverTarget=-1;float machineAccum=0,arpAccum=0; // 1.6.13: дробные аккумуляторы скорости драга
    juce::Point<int> modDragScreen;int lfoHoldIndex=-1;juce::uint32 lfoHoldSince=0; // 1.6.14: удержание над другим LFO
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
