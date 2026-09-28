#include "PluginEditor.h"
#include "PixelFont.h"

#include <map>
#include <cmath>
#include <functional>
#include <limits>

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
    {"VALUE BOXES","VALUE BOXES",1.0},{"MODE SYNT","SYNT MODE",1.0},{"MODE AMP","AMP MODE",1.0},{"MODE FILT","FILT MODE",1.0}, // 1.6.21: MODE AMP вернулся
    {"MODE L","FILTER LOW MODE",1.0},{"MODE H","FILTER HIGH MODE",1.0},{"MODE S","DIST MODE",1.0},{"MODE DLY","DLY MODE",1.0},{"ARP MODE","ARP PLAY",1.0},{"ARP VEL","ARP VEL",1.0},{"ARP PAGE","ARP PAGE",1.0}}; // 1.9.1: MODE S is the sole DIST selector

class PixelLabel final : public juce::Label {
public:
    int textHeight=14; // 1.6.37
    void paint(juce::Graphics& g) override { g.fillAll(juce::Colours::black); if (!isBeingEdited()) { g.setColour(ink); pixel::text(g, getText(), getLocalBounds().reduced(2, 0), textHeight, getJustificationType() == juce::Justification::centred); } } // 1.6.37: РЕАЛЬНАЯ причина «не центрирован» -- лейбл комбобокса это PixelLabel со своим paint(), drawLabel не вызывался никогда; центр -- по justification
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
    juce::Label* createComboBoxTextBox(juce::ComboBox&) override { auto* l = new PixelLabel(); l->textHeight = 20; l->setJustificationType(juce::Justification::centred); return l; } // 1.6.37: текст MODE -- шрифт как у кнопок категорий (20)
    void positionComboBoxText(juce::ComboBox& box, juce::Label& label) override { label.setBounds(1, 1, box.getWidth() - 2, box.getHeight() - 2); label.setJustificationType(juce::Justification::centred); } // 1.6.33: лейбл ВНУТРИ рамки -- фон не съедает обводку, рамка как у кнопок категорий
    juce::Slider::SliderLayout getSliderLayout(juce::Slider& s) override {
        auto layout = LookAndFeel_V4::getSliderLayout(s); // 1.6.37: LEV -- фейдер на ВСЕЙ высоте компонента: V4 даёт зону с отступами ~15px под большой палец (заполнение 233..549 вместо 124..562)
        const auto rng = s.getRange();
        if (s.isVertical() && std::abs(rng.getLength() - 127.0) < 0.5 && rng.getStart() == 0.0) layout.sliderBounds = s.getLocalBounds();
        return layout;
    }
    void drawButtonBackground(juce::Graphics& g, juce::Button& b, const juce::Colour&, bool hover, bool down) override { const bool filled = b.getToggleState() || down; g.fillAll(filled ? ink : juce::Colours::black); g.setColour(ink); g.drawRect(b.getLocalBounds().reduced(1), hover ? 2 : 1); } // 1.6.14: рамки белые
    void drawButtonText(juce::Graphics& g, juce::TextButton& b, bool, bool down) override { g.setColour(b.getToggleState() || down ? knockout : ink); pixel::text(g, b.getButtonText(), b.getLocalBounds().reduced(4, 0), 14, true); } // 1.6.14: названия кнопок всегда белые
    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& b, bool, bool) override { g.fillAll(juce::Colours::black); const int s=std::min(b.getHeight()-2,28),oy=(b.getHeight()-s)/2; g.setColour(ink); g.drawRect(1, oy, s, s, 1); if (b.getToggleState()) { g.drawLine(1+s*2/10, oy+s*7/10, 1+s*4/10, oy+s*9/10, 2); g.drawLine(1+s*4/10, oy+s*9/10, 1+s*8/10, oy+s*2/10, 2); } pixel::text(g, b.getButtonText(), {34, 0, b.getWidth() - 36, b.getHeight()}, 13, false); } // 1.6.28: квадрат галки по высоте кнопки
    void drawTickBox(juce::Graphics& g,juce::Component&,float x,float y,float w,float h,bool ticked,bool enabled,bool,bool)override{juce::Rectangle<float> box(x+2,y+(h-14)*0.5f,std::min(14.0f,w-4),14);g.setColour(juce::Colours::black);g.fillRect(box);g.setColour(ink.withAlpha(enabled?0.85f:0.3f));g.drawRect(box,1);if(ticked)g.fillRect(box.reduced(3));}
    void drawLinearSlider(juce::Graphics& g,int x,int y,int width,int height,float pos,float min,float max,const juce::Slider::SliderStyle style,juce::Slider& slider)override{
        if(style==juce::Slider::LinearVertical){ // 1.6.35: LEV -- заполнение СВЕРХУ от кусочной позиции до линии секций; 127 -> y (риска макс), 100 -> риска дефолта, 0 -> низ
            const int cx=x+width/2;const double norm=juce::jlimit(0.0,1.0,slider.valueToProportionOfLength(slider.getValue()));
            const int top=y+juce::roundToInt((1.0-norm)*static_cast<double>(height));
            g.setColour(ink);g.fillRect(cx-6,top,13,std::max(1,y+height-top));
            g.setColour(juce::Colours::black);g.drawHorizontalLine(top+1,float(cx-6),float(cx+6));}
        else { g.setColour(ink.withAlpha(0.22f)); g.fillRect(x + 2, y + height / 2 - 2, width - 4, 4); g.setColour(ink); g.fillRect(x + 2, y + height / 2 - 2, std::max(2, juce::roundToInt(pos - x)), 4); g.fillEllipse(juce::roundToInt(pos) - 6.0f, y + height / 2 - 6.0f, 12.0f, 12.0f);
            const int fillEnd = std::max(2, juce::roundToInt(pos - x)); // 1.6.24: незаполненный остаток шкалы -- ПОЛАЯ белая рамка (виден предел)
            g.setColour(ink.withAlpha(0.85f)); g.drawRect(x + 2 + std::min(fillEnd, width - 4), y + height / 2 - 3, std::max(0, width - 4 - fillEnd), 6, 1); }
        juce::ignoreUnused(min, max, slider);
    }
    void drawRotarySlider(juce::Graphics& g,int x,int y,int width,int height,float pos,float start,float end,juce::Slider&)override{
        int cx=x+width/2,cy=y+height/2;g.setColour(ink);
        for(int i=0;i<16;++i){float a=2*nova::pi*i/16;int px=cx+juce::roundToInt(std::sin(a)*14),py=cy+juce::roundToInt(std::cos(a)*14);g.fillRect(px-1,py-1,3,3);}
        const float angle=start+pos*(end-start);
        for(int i=3;i<=11;++i)g.fillRect(cx+juce::roundToInt(std::sin(angle)*i)-1,cy-juce::roundToInt(std::cos(angle)*i)-1,3,3);
    }
    void drawComboBox(juce::Graphics& g,int width,int height,bool isDown,int buttonX,int,int,int,juce::ComboBox&)override{
        g.fillAll(juce::Colours::black);g.setColour(ink);g.drawRect(0,0,width,height); // 1.6.32: обводка заподлицо -- как у кнопки категории
        int ax=buttonX+width/2-4-2*(isDown?1:-1)+2;
        for(int i=0;i<5;++i)g.fillRect(ax-3+i,height/2+(isDown?i:-i)-(isDown?0:4),2,2);
    }
    // 1.6.13: чёрный пиксельный скин для ВСЕХ popup-меню. 1.6.16: шрифт КРУПНЕЕ
    // (до 21 px, авто-усадка только у очень длинных строк) и ширина по пиксельным
    // метрикам -- меню читаются и не уезжают за экран.
    void drawComboBoxTextWhenNothingSelected(juce::Graphics& g,juce::ComboBox& box,juce::Label&)override{
        g.setColour(ink.withAlpha(box.isEnabled()?0.9f:0.3f));pixel::text(g,box.getText(),box.getLocalBounds(),14,true);} // 1.6.30: текст MODE -- по центру
    void drawLabel(juce::Graphics& g,juce::Label& l)override{ // 1.6.35: текст MODE -- 1-в-1 как PixelButton категорий: отступ 5px, шрифт 20 (scale 3), по центру
        g.fillAll(juce::Colours::black);g.setColour(ink);
        pixel::text(g,l.getText(),l.getLocalBounds().reduced(5,0),20,true);}
    void drawPopupMenuBackgroundWithOptions(juce::Graphics& g,int width,int height,const juce::PopupMenu::Options&)override{
        g.fillAll(juce::Colours::black);g.setColour(juce::Colours::white);g.drawRect(juce::Rectangle<int>(0,0,width,height),1); // 1.6.25: белая рамка ВСЕХ списков -- чёрный фон больше не сливается
    }
    static int pixelScaleFor(const juce::String&){ return 2; } // 1.6.34: списки -- единый шрифт 14px
    void getIdealPopupMenuItemSizeWithOptions(const juce::String& text,bool isSeparator,int,int& idealWidth,int& idealHeight,const juce::PopupMenu::Options&)override{
        if(isSeparator){idealWidth=40;idealHeight=12;return;}
        const int scale=pixelScaleFor(text);
        idealWidth=juce::jlimit(120,700,text.length()*6*scale+28);
        idealHeight=22;
    }
    void drawPopupMenuSectionHeaderWithOptions(juce::Graphics& g,const juce::Rectangle<int>& area,const juce::String& sectionName,const juce::PopupMenu::Options&)override{
        g.setColour(juce::Colours::black);g.fillRect(area.reduced(1,0));g.setColour(ink);pixel::text(g,sectionName,area.reduced(8,0),14,false); // 1.6.34: заголовки папок -- тот же 14px
    }
    void drawPopupMenuItemWithOptions(juce::Graphics& g,const juce::Rectangle<int>& area,bool highlighted,const juce::PopupMenu::Item& item,const juce::PopupMenu::Options&)override{
if(item.isSeparator){g.setColour(juce::Colours::white.withAlpha(0.35f));g.drawHorizontalLine((area.getY()+area.getBottom())/2,area.getX()+6.0f,area.getRight()-6.0f);return;} // 1.6.42: сепаратор меню -- линия, а не пустая кнопка
        g.setColour(juce::Colours::black);g.fillRect(area.reduced(1,0)); // 1.6.25: красим только внутренность -- левый/правый край белой рамки живы
        if(item.isEnabled&&highlighted){g.setColour(ink);g.fillRect(area.reduced(2,0));}
        g.setColour(highlighted&&item.isEnabled?knockout:ink); // 1.6.16: всё белым (читаемость)
        pixel::text(g,item.text,area.reduced(10,0),14,false); // 1.6.34: шрифт списков меньше (14px) -- просторнее при том же скине
        if(item.subMenu!=nullptr){g.setColour(ink);const int ax=area.getRight()-16,ay=(area.getY()+area.getBottom())/2; // 1.6.30: стрелка ">" -- справа параметры
            for(int i=0;i<5;++i){g.fillRect(static_cast<float>(ax-4+i),static_cast<float>(ay-5+i),2.0f,2.0f);g.fillRect(static_cast<float>(ax-4+i),static_cast<float>(ay+3-i),2.0f,2.0f);}} // 1.6.31: остриё вправо -- к параметру
    }
    void drawScrollbar(juce::Graphics& g,juce::ScrollBar&,int x,int y,int width,int height,bool isVertical,int thumbStart,int thumbSize,bool,bool)override{
        g.fillAll(juce::Colours::black); // 1.6.13: скроллбар -- белый на чёрном, не синий
        g.setColour(ink.withAlpha(0.35f));g.drawRect(x,y,width,height,1);
        if(thumbSize<=0)return;
        g.setColour(ink);
        if(isVertical)g.fillRect(x+2,thumbStart,width-4,thumbSize);
        else g.fillRect(thumbStart,y+2,thumbSize,height-4);}
};
// 1.6.32: компактный скин ТОЛЬКО для выпадающих списков MODE-кнопок (остальные списки не тронуты)
class SmallMenuLaf final : public juce::LookAndFeel_V4 {
public:
    int minWidth=0; // 1.6.35: ширина родительской кнопки -- список не уже её
    int getPopupMenuBorderSizeWithOptions(const juce::PopupMenu::Options&)override{return 1;} // 1.6.37: убраны лишние пиксели рамки попапа по бокам
    void drawPopupMenuBackgroundWithOptions(juce::Graphics& g,int width,int height,const juce::PopupMenu::Options&)override{
        g.fillAll(juce::Colours::black);g.setColour(juce::Colours::white);g.drawRect(juce::Rectangle<int>(0,0,width,height),1);}
    void getIdealPopupMenuItemSizeWithOptions(const juce::String& text,bool isSeparator,int,int& idealWidth,int& idealHeight,const juce::PopupMenu::Options&)override{
        if(isSeparator){idealWidth=40;idealHeight=8;return;}
        idealWidth=juce::jlimit(96,420,juce::jmax(minWidth - 2,text.length()*12+24));idealHeight=22;} // 1.6.37: пункт = кнопка-2, внешняя ширина списка = ширина кнопки ровно
    void drawPopupMenuSectionHeaderWithOptions(juce::Graphics& g,const juce::Rectangle<int>& area,const juce::String& sectionName,const juce::PopupMenu::Options&)override{
        g.setColour(juce::Colours::black);g.fillRect(area.reduced(1,0));g.setColour(juce::Colours::white);pixel::text(g,sectionName,area.reduced(8,0),14,false);}
    void drawPopupMenuItemWithOptions(juce::Graphics& g,const juce::Rectangle<int>& area,bool highlighted,const juce::PopupMenu::Item& item,const juce::PopupMenu::Options&)override{
        g.setColour(juce::Colours::black);g.fillRect(area.reduced(1,0));
        if(item.isEnabled&&highlighted){g.setColour(juce::Colours::white);g.fillRect(area.reduced(2,0));}
        g.setColour(highlighted&&item.isEnabled?juce::Colours::black:juce::Colours::white);
        pixel::text(g,item.text,area.reduced(8,0),14,false);
        if(item.subMenu!=nullptr){g.setColour(juce::Colours::white);const int ax=area.getRight()-14,ay=(area.getY()+area.getBottom())/2;
            for(int i=0;i<5;++i){g.fillRect(static_cast<float>(ax-4+i),static_cast<float>(ay-5+i),2.0f,2.0f);g.fillRect(static_cast<float>(ax-4+i),static_cast<float>(ay+3-i),2.0f,2.0f);}}}
};
// 1.6.33: компактный скин списков -- общий статический экземпляр (MODE/CLEAR/MENU/AUX DEPTH)
static SmallMenuLaf& smallMenuLaf(){ static SmallMenuLaf l; return l; }
// Popup placement is deliberately screen-aware. Earlier revisions forced every
// menu below its control, which left long lists inaccessible at the lower edge
// of a plug-in window. JUCE's target-component placement chooses above/below
// and clips to the active display as needed.
static void showComboBelow(juce::ComboBox& box,bool small){
    smallMenuLaf().minWidth=small?box.getWidth():0;
    juce::PopupMenu m;m.setLookAndFeel(small?static_cast<juce::LookAndFeel*>(&smallMenuLaf()):&box.getLookAndFeel());
    const int cur=box.getSelectedItemIndex();
    for(int i=0;i<box.getNumItems();++i)m.addItem(i+1,box.getItemText(i),true,i==cur);
    const juce::Component::SafePointer<juce::ComboBox> safe(&box);
    m.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(&box),
        [safe](int res){if(safe!=nullptr&&res>0)safe->setSelectedItemIndex(res-1,juce::sendNotificationSync);});}

// 1.6.38/43: LAYOUT EDIT -- хранилище раскладки (layout.json) + оверлей-редактор.
// F1/MENU открыть-закрыть; ЛКМ двигает; CTRL+ЛКМ -- прилипание (линии); SHIFT+ЛКМ --
// размер (угол тянет обе стенки); ALT+ЛКМ -- одна стенка (зелёная подсветка);
// колесо -- ЗУМ для точной подгонки; рамка на пустом месте или SHIFT-клик -- выделить
// несколько, G -- склеить/расклеить группу; F2 -- заметка агенту (COPY COORDS);
// F3 -- перечитать layout.json из файла; F4 -- сохранить; C -- цвет/шрифт/размер текста;
// DEL -- вернуть выделенное; CTRL+Z/Y -- откат/повтор. Каждое движение сразу в файл.
static void applyLayoutText(juce::Component*,const juce::String&); // определение ниже (нужен завершённый Cell)
static bool layoutTextable(juce::Component*); // определение ниже
static juce::String layoutCaptureText(juce::Component*); // определение ниже
// 1.6.42/43: TextTag -- НАРИСОВАННЫЙ текст как объект редактора: двигается,
// ресайзится, переименовывается, цвет/шрифт/масштаб (C в редакторе).
class TextTag final : public juce::Component {
public:
    TextTag(juce::Component& owner,const char* id,juce::Rectangle<int> r,bool vlock=false):base(r),vLock(vlock){setComponentID(id);setBounds(r);setInterceptsMouseClicks(false,false);owner.addAndMakeVisible(this);}
    juce::Rectangle<int> base; bool vLock;
    juce::String tagText, factoryText;
    int fontFor(int baseFont) const { return juce::jmax(8,juce::roundToInt(baseFont*fontScale)*juce::jmax(1,getHeight())/juce::jmax(1,base.getHeight())); }
    juce::Colour tagColor{juce::Colours::white}; bool colorSet=false;
    int fontId=0; float fontScale=1.0f; // 1.6.43: 0 pixel, 1 sans, 2 mono
};
struct UiLayoutEntry { std::array<int,4> rect{0,0,0,0}; juce::String text,base; int color=0,fid=-1; float fscale=0; int z=-1; }; // 1.6.44: z = слой (индекс в родителе)
static void layoutStyleApply(juce::Component*,const UiLayoutEntry&); // определение ниже
struct UiLayoutStore {
    std::map<juce::String, UiLayoutEntry> map;
    std::map<juce::String, std::array<int,8>> arrs;
    struct Comment { int x=0,y=0; juce::String key,text; };
    std::vector<Comment> comments;
    std::vector<std::vector<juce::String>> groups; // 1.6.43: G-группы (двигаются вместе)
    static juce::File file(){ auto dir=juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory).getChildFile("MonomachineNova"); dir.createDirectory(); return dir.getChildFile("layout.json"); }
    void load(){ loadFrom(file().loadFileAsString()); }
    void loadFrom(const juce::String& blob){ map.clear(); arrs.clear(); comments.clear(); groups.clear(); if(blob.trim().isEmpty())return; auto v=juce::JSON::parse(blob); if(auto* o=v.getDynamicObject()){
        for(auto& nv:o->getProperties()){
            if(auto* a=nv.value.getArray()){ if(a->size()==8){ std::array<int,8> arr{}; for(int i2=0;i2<8;++i2)arr[(size_t)i2]=(int)a->getUnchecked(i2); arrs[nv.name.toString()]=arr; } else if(a->size()==4) map[nv.name.toString()]=UiLayoutEntry{{(int)a->getUnchecked(0),(int)a->getUnchecked(1),(int)a->getUnchecked(2),(int)a->getUnchecked(3)},{},{},0,-1,0,-1}; continue; }
            if(auto* e=nv.value.getDynamicObject()){ UiLayoutEntry en; if(auto* r=e->getProperty("r").getArray(); r!=nullptr&&r->size()==4){ en.rect={(int)r->getUnchecked(0),(int)r->getUnchecked(1),(int)r->getUnchecked(2),(int)r->getUnchecked(3)}; en.text=e->getProperty("t").toString(); en.base=e->getProperty("b").toString(); en.color=(int)e->getProperty("c"); en.fid=e->getProperty("fid"); en.fscale=(float)(double)e->getProperty("fs"); en.z=(int)e->getProperty("z"); map[nv.name.toString()]=en; } } }
        if(auto* cs=o->getProperty("comments").getArray(); cs!=nullptr) for(int i2=0;i2<cs->size();++i2) if(auto* n=cs->getUnchecked(i2).getDynamicObject()){ Comment c; c.x=(int)n->getProperty("x"); c.y=(int)n->getProperty("y"); c.key=n->getProperty("k").toString(); c.text=n->getProperty("t").toString(); comments.push_back(c); }
        if(auto* gs=o->getProperty("groups").getArray(); gs!=nullptr) for(int i2=0;i2<gs->size();++i2) if(auto* ga=gs->getUnchecked(i2).getArray()){ std::vector<juce::String> g; for(int k=0;k<ga->size();++k)g.push_back(ga->getUnchecked(k).toString()); if(g.size()>1)groups.push_back(g); } } }
    juce::String toJSON(){ juce::DynamicObject::Ptr o=new juce::DynamicObject(); for(auto& kv:map){ juce::DynamicObject::Ptr e=new juce::DynamicObject(); juce::Array<juce::var> r{kv.second.rect[0],kv.second.rect[1],kv.second.rect[2],kv.second.rect[3]}; e->setProperty("r",r); if(kv.second.text.isNotEmpty())e->setProperty("t",juce::var(kv.second.text)); if(kv.second.base.isNotEmpty())e->setProperty("b",juce::var(kv.second.base)); if(kv.second.color!=0)e->setProperty("c",kv.second.color); if(kv.second.fid>=0)e->setProperty("fid",kv.second.fid); if(kv.second.fscale>0)e->setProperty("fs",juce::var(kv.second.fscale)); if(kv.second.z>=0)e->setProperty("z",kv.second.z); o->setProperty(kv.first,juce::var(e)); } for(auto& kv:arrs){ juce::Array<juce::var> a; for(int v2:kv.second)a.add(v2); o->setProperty(kv.first,a); } juce::Array<juce::var> cs; for(auto& c:comments){ juce::DynamicObject::Ptr n=new juce::DynamicObject(); n->setProperty("x",c.x); n->setProperty("y",c.y); n->setProperty("k",juce::var(c.key)); n->setProperty("t",juce::var(c.text)); cs.add(juce::var(n)); } o->setProperty("comments",cs); juce::Array<juce::var> gs; for(auto& g:groups){ juce::Array<juce::var> ga; for(auto& id:g)ga.add(juce::var(id)); gs.add(juce::var(ga)); } o->setProperty("groups",gs); return juce::JSON::toString(juce::var(o),true); }
    bool save(){ file().getParentDirectory().createDirectory(); return file().replaceWithText(toJSON()); } // 1.6.51: результат наружу (диагностика "файл пустой")
    void put(const juce::String& id,juce::Rectangle<int> r){ auto& en=map[id]; en.rect={r.getX(),r.getY(),r.getWidth(),r.getHeight()}; }
    bool has(const juce::String& id)const{ return map.count(id)>0; }
    UiLayoutEntry entry(const juce::String& id)const{ auto it=map.find(id); return it==map.end()?UiLayoutEntry():it->second; }
    juce::Rectangle<int> get(const juce::String& id)const{ auto& a=map.at(id).rect; return {a[0],a[1],a[2],a[3]}; }
    void setText(const juce::String& id,const juce::String& t){ map[id].text=t; }
    juce::String textOf(const juce::String& id)const{ auto it=map.find(id); return it==map.end()?juce::String():it->second.text; }
    juce::String baseOf(const juce::String& id)const{ auto it=map.find(id); return it==map.end()?juce::String():it->second.base; }
    void setBase(const juce::String& id,const juce::String& b){ map[id].base=b; }
    void setStyle(const juce::String& id,int color,int fid,float fscale){ if(color!=0||fid>=0||fscale>0)if(!has(id))put(id,{}); map[id].color=color; map[id].fid=fid; map[id].fscale=fscale; }
    void setZ(const juce::String& id,int z2,juce::Rectangle<int> r){ if(z2<0)return; if(!has(id))put(id,r); map[id].z=z2; } // 1.6.44: слой элемента
    void erase(const juce::String& id){ map.erase(id); }
    void eraseAll(){ map.clear(); arrs.clear(); comments.clear(); groups.clear(); } // 1.6.52: полное стирание расклада
    std::vector<int> getArr(const juce::String& id)const{ std::vector<int> a; auto it=arrs.find(id); if(it!=arrs.end())for(int v2:it->second)a.push_back(v2); return a; }
    void setArr(const juce::String& id,const int* v2,int n){ auto& a=arrs[id]; for(int i2=0;i2<n&&i2<8;++i2)a[(size_t)i2]=v2[i2]; }
    void eraseArr(const juce::String& id){ arrs.erase(id); }
    int groupOf(const juce::String& id)const{ for(size_t g=0;g<groups.size();++g)for(auto& m:groups[g])if(m==id)return (int)g; return -1; }
    void setGroup(const std::vector<juce::String>& ids){ std::vector<std::vector<juce::String>> keep; for(auto& g:groups){ std::vector<juce::String> rest; for(auto& m:g)if(std::find(ids.begin(),ids.end(),m)==ids.end())rest.push_back(m); if((int)rest.size()>1)keep.push_back(rest); } if((int)ids.size()>1)keep.push_back(ids); groups=keep; }
};
static UiLayoutStore& uiLayout(){ static UiLayoutStore s; static bool inited=false; if(!inited){ inited=true; s.load(); } return s; }
static void uiLayoutApply(juce::Component& root,const char* rootKey){ for(auto* ch:root.getChildren()){ const auto cid=ch->getComponentID(); if(cid.isEmpty())continue; const juce::String id=juce::String(rootKey)+"."+cid; if(uiLayout().has(id)){ const auto en=uiLayout().entry(id); ch->setBounds({en.rect[0],en.rect[1],en.rect[2],en.rect[3]}); if(en.text.isNotEmpty())applyLayoutText(ch,en.text); layoutStyleApply(ch,en); if(en.z>=0){ auto* par=ch->getParentComponent(); const int cur=par->getIndexOfChildComponent(ch); if(cur!=en.z){ par->removeChildComponent(cur); par->addChildComponent(ch,en.z); } } } } } // 1.6.44: слой из layout.json
class LayoutOverlay final : public juce::Component {
public:
    LayoutOverlay(juce::Component& rootComp,const char* key):root(rootComp),rootKey(key){
        setInterceptsMouseClicks(true,false); setWantsKeyboardFocus(true);
        for(auto* ch:root.getChildren()){ const auto cid2=ch->getComponentID(); if(!cid2.isEmpty()&&cid2!="repitch"&&cid2!="dsnd"&&cid2!="porta"&&cid2!="gui") items.push_back(ch); } // 1.6.44: плавающие доп-окна редактор не двигает (свой драг/булавка)
        for(auto* ch:items) orig[ch]=ch->getBounds();
        lastLiveN=root.getNumChildComponents();
        baseT=root.getTransform();
        setSize(root.getWidth(),root.getHeight()); grabKeyboardFocus();
    }
    std::function<void()> onDone;
    juce::String nearestEdges(juce::Component* it) const { // 1.6.43: ближайшие границы соседей -- в readout редактора
        int bx=-1,bdx=1<<30,by=-1,bdy=1<<30;
        const int ix=it->getX(),iy=it->getY(),ir=ix+it->getWidth(),ib=iy+it->getHeight();
        for(auto* o:items){ if(o==it||o->getParentComponent()!=it->getParentComponent())continue;
            const int ol=o->getX(),orr=ol+o->getWidth(),ot=o->getY(),ob=ot+o->getHeight();
            for(int cx:{ol,orr}){ const int d=std::min(std::abs(cx-ix),std::abs(cx-ir)); if(d<bdx){bdx=d;bx=cx;} }
            for(int cy:{ot,ob}){ const int d=std::min(std::abs(cy-iy),std::abs(cy-ib)); if(d<bdy){bdy=d;by=cy;} } }
        juce::String s; if(bx>=0)s+="  nx="+juce::String(bx); if(by>=0)s+="  ny="+juce::String(by); return s; }
    ~LayoutOverlay() override { if(colorWin!=nullptr){ colorWin->setVisible(false); colorWin.reset(); } root.setTransform(baseT); } // 1.6.43: убрать пикер, вернуть зум 1x
    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::black.withAlpha(0.35f));
        g.setColour(juce::Colours::yellow.withAlpha(0.9f));
        if(guideX>=0)for(float y=2;y<getHeight();y+=12)g.fillRect((float)guideX,y,1.0f,7.0f);
        if(guideY>=0)for(float x=2;x<getWidth();x+=12)g.fillRect(x,(float)guideY,7.0f,1.0f);
        int ci=0;
        for(auto& c:uiLayout().comments){ if(c.key==rootKey){ const bool hot=(markerHover==ci);
            g.setColour(juce::Colours::yellow); g.drawRect(c.x,c.y,14,14,1);
            pixel::text(g,juce::String(ci+1),{c.x+3,c.y+1,10,12},11,false);
            if(hot)pixel::text(g,c.text,{juce::jlimit(2,juce::jmax(2,getWidth()-460),c.x+16),18,452,14},13,false); } ++ci; }
        for(auto* ch:items){ if(!ch->isVisible())continue; // 1.6.48: невидимые (закрытый viewport) -- не рисуем
            const auto r=rectOf(*ch); const bool inSel=std::find(sel.begin(),sel.end(),ch)!=sel.end();
            const bool hot=(ch==rsItem||(ch==hover&&rsItem==nullptr)); // 1.6.48 FIX: белым -- ТОЛЬКО тащимый (было: весь список во время жеста -- каша из рамок и readout)
            g.setColour(juce::Colours::white.withAlpha(hot?1.0f:0.45f)); g.drawRect(r,hot?2:1);
            if(inSel){ g.setColour(juce::Colours::lightblue.withAlpha(0.8f)); g.drawRect(r.expanded(3),1); }
            if(hot&&rsItem==ch&&rsMode>=2){ g.setColour(juce::Colours::lime); // 1.6.43/47: тянемая стенка/угол -- зелёная
                if((rsMask&1)!=0)g.fillRect(r.getX()-2,r.getY(),4,r.getHeight());
                if((rsMask&2)!=0)g.fillRect(r.getRight()-2,r.getY(),4,r.getHeight());
                if((rsMask&4)!=0)g.fillRect(r.getX(),r.getY()-2,r.getWidth(),4);
                if((rsMask&8)!=0)g.fillRect(r.getX(),r.getBottom()-2,r.getWidth(),4); }
            if(ch==rsItem){ const int by=juce::jlimit(18,getHeight()-16,r.getBottom()+2); // 1.6.48: readout один -- у тащимого
                pixel::text(g,ch->getComponentID()+" "+juce::String(ch->getX())+","+juce::String(ch->getY())+" "+juce::String(ch->getWidth())+"x"+juce::String(ch->getHeight())+"  z"+juce::String(zoom,2)+nearestEdges(ch),{2,by,680,14},14,false); } } // 1.6.43: nx/ny = ближайшая граница соседа
        if(marqActive){ g.setColour(juce::Colours::lightblue.withAlpha(0.9f)); g.drawRect(marq,1); g.setColour(juce::Colours::lightblue.withAlpha(0.08f)); g.fillRect(marq); }
        pixel::text(g,"F1 CLOSE  F2 MENU  F3 SAVE  F4 RESET  F5 RELOAD  WHEEL ZOOM x"+juce::String(zoom,2)+"  DRAG MOVE  CTRL SNAP  SHIFT SIZE  ALT EDGE  SHIFT-CLICK SELECT  Z LAYER  C COLOR  DEL REVERT",{4,getHeight()-18,1120,16},14,false); // 1.6.52: подсказка ВНИЗ (не лежит на контролах)
        // 1.6.52: ВИРТУАЛЬНЫХ КНОПОК НЕТ -- они перекрывали верхний ряд реальных контролов
        // (undo/redo/menu/bpm/dspmode не давали себя двигать). Вся механика -- в меню F2.
        if(savedFlash>0)pixel::text(g,savedFlash==2?"SAVE FAIL!":"SAVED!",{getWidth()-244,getHeight()-18,240,15},13,false); // 1.6.52: фидбек внизу справа
        if(copiedFlash>0)pixel::text(g,"COPIED!",{getWidth()-492,getHeight()-18,240,15},13,false); // 1.6.52
        pixel::text(g,"entries:"+juce::String(static_cast<int>(uiLayout().map.size())),{4,getHeight()-36,220,14},13,false); // 1.6.53
        if(wroteInfo.isNotEmpty())pixel::text(g,wroteInfo,{240,getHeight()-36,700,14},13,false); // 1.6.55: WROTE <id> [+N] -- жест записан
    }
    bool keyPressed(const juce::KeyPress& k) override {
        if(k.getModifiers().isCtrlDown()&&k.isKeyCode('Z')){ undoOp(); return true; }
        if(k.getModifiers().isCtrlDown()&&k.isKeyCode('Y')){ redoOp(); return true; }
        if(k.isKeyCode(juce::KeyPress::F1Key)){ if(onDone)onDone(); return true; }
        if(k.isKeyCode(juce::KeyPress::F2Key)){ editMenu(); return true; } // 1.6.52: F2 -- МЕНЮ редактора (save/reload/reset/undo/redo/copy json/erase/note)
        if(k.isKeyCode(juce::KeyPress::F3Key)){ saveFlash(); return true; } // 1.6.49: F3 -- СОХРАНИТЬ (кнопка SAVE дублирует)
        if(k.isKeyCode(juce::KeyPress::F5Key)){ pushHistory(); uiLayout().load(); root.resized(); repaint(); return true; } // 1.6.48/49: F5 -- перечитать layout.json с диска
        if(k.isKeyCode(juce::KeyPress::F4Key)){ resetAll(); return true; } // 1.6.48: F4 -- СБРОС расклада к дефолту
        if(k.isKeyCode('C')&&!sel.empty()&&!k.getModifiers().isCtrlDown()){ styleDialog(); return true; } // 1.6.49: без Ctrl (Ctrl+C не открывает стиль)
        if(k.isKeyCode('Z')&&!sel.empty()&&!k.getModifiers().isCtrlDown()){ zDialog(); return true; } // 1.6.49: без Ctrl (Ctrl+Z = откат, выше)
        if(k.isKeyCode(juce::KeyPress::deleteKey)||k.isKeyCode(juce::KeyPress::backspaceKey)){
            if(markerHover>=0){ pushHistory(); uiLayout().comments.erase(uiLayout().comments.begin()+markerHover); uiLayout().save(); markerHover=-1; repaint(); return true; }
            if(!sel.empty()){ for(auto* it:sel)fullRevert(it); sel.clear(); return true; }
            if(hover!=nullptr){ fullRevert(hover); return true; }
            return false; }
        if(k.isKeyCode(juce::KeyPress::escapeKey)){ if(onDone)onDone(); return true; }
        return false; }
    void mouseWheelMove(const juce::MouseEvent& e,const juce::MouseWheelDetails& w) override { // 1.6.43: зум колесом
        if(std::abs(w.deltaY)<0.01f)return;
        const float z=juce::jlimit(1.0f,4.0f,zoom*(w.deltaY>0?1.15f:1.0f/1.15f));
        const auto f=e.position.toFloat(); applyZoom(z,f.x,f.y); } // 1.6.43: зум вокруг курсора
    void syncItems(){ // 1.6.47: набор компонентов мог измениться -- пересобрать и вычистить висячие указатели (краш-гигиена)
        const int n=root.getNumChildComponents(); if(n==lastLiveN)return; lastLiveN=n;
        std::vector<juce::Component*> live; for(auto* ch:root.getChildren()){ const auto cid2=ch->getComponentID(); if(!cid2.isEmpty()&&cid2!="repitch"&&cid2!="dsnd"&&cid2!="porta"&&cid2!="gui") live.push_back(ch); }
        items=live; for(auto* ch:items)if(!orig.count(ch))orig[ch]=ch->getBounds();
        const auto alive=[&](juce::Component* c){ return c!=nullptr&&std::find(items.begin(),items.end(),c)!=items.end(); };
        if(!alive(drag))drag=nullptr;
        if(!alive(rsItem)){ rsItem=nullptr; rsMode=0; rsMask=0; }
        if(!alive(hover))hover=nullptr;
        sel.erase(std::remove_if(sel.begin(),sel.end(),[&](juce::Component* c){ return !alive(c); }),sel.end());
        for(auto it2=groupSelStart.begin();it2!=groupSelStart.end();){ if(!alive(it2->first))it2=groupSelStart.erase(it2); else ++it2; }
        for(auto it2=rsGroup.begin();it2!=rsGroup.end();){ if(!alive(it2->first))it2=rsGroup.erase(it2); else ++it2; } }
    void mouseDown(const juce::MouseEvent& e) override {
        if(e.mods.isMiddleButtonDown()){ panning=true; panSX=e.getScreenPosition(); return; } // 1.7.5: СКМ-драг -- тащить зумленную картинку
        const auto p=e.position.toInt(); lastMouse=p; syncItems(); grabKeyboardFocus(); uiLayout().load(); // 1.6.53 FIX: старт жеста = СВЕЖИЙ файл (другой инстанс Synth/FX мог перезаписать его своим магазином)
        preState=uiLayout().toJSON(); // 1.6.48: снимок ДО жеста
        if(e.mods.isPopupMenu()){ if(onDone)onDone(); return; } // 1.6.43: ПКМ в редакторе = закрыть
        auto* it=hitAt(p); // 1.6.52: клик идёт только в элементы (виртуальных зон больше нет)
        if(it==nullptr){ marqActive=true; marq={p.x,p.y,1,1}; marqStart=p; repaint(); return; } // рамка-выделение
        if(e.mods.isShiftDown()){ auto s=std::find(sel.begin(),sel.end(),it); if(s!=sel.end())sel.erase(s); else sel.push_back(it);
            if(sel.size()>1){ rsMode=4; rsMask=2|8; rsItem=it; drag=it; rsStart=p; rsSnap=rectOf(*it);
                rsGroup.clear(); for(auto* s2:sel)rsGroup[s2]=rectOf(*s2); } repaint(); return; } // 1.6.47: SHIFT-клик добавил -- тянешь = размер ВСЕМ от снапшота
        if(std::find(sel.begin(),sel.end(),it)==sel.end())sel={it}; // клик вне выделения = одиночный
        rsItem=it; drag=it; rsStart=p; rsSnap=rectOf(*it); // 1.6.47: ЖЕСТ = {элемент, точка старта, снапшот}
        if(e.mods.isAltDown()){ const auto r=rsSnap; const int dl=std::abs(p.x-r.getX()),dr=std::abs(p.x-r.getRight()),dt=std::abs(p.y-r.getY()),db=std::abs(p.y-r.getBottom());
            const int m=std::min(std::min(dl,dr),std::min(dt,db));
            if(m<=8)rsMode=2,rsMask=(m==dl?1:0)|(m==dr?2:0)|(m==dt?4:0)|(m==db?8:0); } // ALT+край
        if(rsMode==0){ rsMask=cornerMaskAt(p,it,true); if(rsMask!=0)rsMode=3; } // угол -- ТОЛЬКО углы; край без ALT = двигать
        if(rsMode==0)rsMode=1;
        rsGroup.clear(); for(auto* s2:sel)rsGroup[s2]=rectOf(*s2); // снапшот выделения
        repaint();
    }
    void mouseDrag(const juce::MouseEvent& e) override {
        if(panning){ const auto np=e.getScreenPosition(); applyZoom(zoom,panFx-(np.x-panSX.x),panFy-(np.y-panSX.y)); panSX=np; repaint(); return; } // 1.7.5: перетаскивание зумленной картинки
        const auto p=e.position.toInt();
        if(marqActive){ marq={juce::jmin(marqStart.x,p.x),juce::jmin(marqStart.y,p.y),std::abs(p.x-marqStart.x)+1,std::abs(p.y-marqStart.y)+1}; repaint(); return; }
        syncItems();
        if(rsMode==0||rsItem==nullptr)return;
        const int dx=p.x-rsStart.x,dy=p.y-rsStart.y; // 1.6.47: ОДНА дельта -- от точки старта
        if(rsMode==4&&std::abs(dx)+std::abs(dy)<3)return; // 1.6.54: микросдвиг после SHIFT-клика НЕ ресайзит всех
        if(rsMode>=2){ auto b=rsSnap; // размеры -- ТОЛЬКО от снапшота (улетание невозможно структурно)
            if(rsMode==4){ b.setWidth(juce::jlimit(8,900,rsSnap.getWidth()+dx)); b.setHeight(juce::jlimit(8,900,rsSnap.getHeight()+dy)); }
            else{ if((rsMask&1)!=0)b.setLeft(rsSnap.getX()+dx); if((rsMask&2)!=0)b.setWidth(rsSnap.getWidth()+dx);
                  if((rsMask&4)!=0)b.setTop(rsSnap.getY()+dy); if((rsMask&8)!=0)b.setHeight(rsSnap.getHeight()+dy);
                  if(b.getWidth()<8)b.setWidth(8); if(b.getHeight()<8)b.setHeight(8); }
            if(e.mods.isCtrlDown())snapResize(b,rsItem,rsMode==4?(2|8):rsMask);
            if(rsGroup.size()>1){ const int dL=b.getX()-rsSnap.getX(),dT=b.getY()-rsSnap.getY(),dR=b.getRight()-rsSnap.getRight(),dB=b.getBottom()-rsSnap.getBottom(),dW=b.getWidth()-rsSnap.getWidth(),dH=b.getHeight()-rsSnap.getHeight(); // тот же край/размер у ВСЕХ выделенных
                for(auto& gs:rsGroup){ if(gs.first==rsItem)continue; auto nb=gs.second;
                    if(rsMode==4){ nb.setWidth(juce::jmax(8,gs.second.getWidth()+dW)); nb.setHeight(juce::jmax(8,gs.second.getHeight()+dH)); }
                    else{ if((rsMask&1)!=0)nb.setLeft(gs.second.getX()+dL); if((rsMask&2)!=0)nb.setRight(gs.second.getRight()+dR);
                          if((rsMask&4)!=0)nb.setTop(gs.second.getY()+dT); if((rsMask&8)!=0)nb.setBottom(gs.second.getBottom()+dB);
                          if(nb.getWidth()<8)nb.setWidth(8); if(nb.getHeight()<8)nb.setHeight(8); }
                    applyBox(gs.first,nb); } }
            applyBox(rsItem,b); repaint(); return; }
        auto want=rsSnap.withPosition(rsSnap.getTopLeft()+juce::Point<int>(dx,dy)); // движение тоже от снапшота
        guideX=guideY=-1;
        if(e.mods.isCtrlDown())snap(want);
        const auto delta=want.getTopLeft()-rsSnap.getTopLeft();
        for(auto& gs:rsGroup)applyBox(gs.first,gs.second.withPosition(gs.second.getTopLeft()+delta)); // выделение двигается вместе
        repaint();
    }
    void mouseUp(const juce::MouseEvent& e) override { panning=false;
        const auto p=e.position.toInt();
        if(marqActive){ marqActive=false; syncItems(); sel.clear(); for(auto* ch:items)if(ch->isVisible()&&marq.intersects(rectOf(*ch)))sel.push_back(ch); repaint(); return; } // рамка = выделить
        syncItems();
        if(rsItem!=nullptr&&rsMode!=0){ const bool moved=(p-rsStart)!=juce::Point<int>(0,0); // 1.6.50 FIX: клик БЕЗ движения ничего не пишет
            const bool alive=std::find(items.begin(),items.end(),rsItem)!=items.end();
            if(alive&&moved){ uiLayout().put(rootKey+"."+rsItem->getComponentID(),rsItem->getBounds()); // 1.6.54: пишем ВСЁ АКТИВНОЕ ВЫДЕЛЕНИЕ (sel)
                for(auto* s2:sel)if(s2!=rsItem&&std::find(items.begin(),items.end(),s2)!=items.end()&&s2->isVisible())uiLayout().put(rootKey+"."+s2->getComponentID(),s2->getBounds());
                uiLayout().save();
                wroteInfo="WROTE "+juce::String(rsItem->getComponentID())+(sel.size()>1?" +"+juce::String(sel.size()-1):"")+"  entries:"+juce::String(static_cast<int>(uiLayout().map.size())); // 1.6.55: запись ВИДНА на канве
                juce::Timer::callAfterDelay(1500,[safe=juce::Component::SafePointer<LayoutOverlay>(this)]{ if(safe!=nullptr){ safe->wroteInfo={}; safe->repaint(); } }); } } // 1.6.47/50/54/55
        if(uiLayout().toJSON()!=preState){ histUndo.push_back(preState); if(histUndo.size()>60)histUndo.erase(histUndo.begin()); histRedo.clear(); } // 1.6.48 FIX: откат -- ТОЛЬКО реальные изменения (клики не забивали стек, Ctrl+Y умирал от чистки redo)
        drag=nullptr; rsItem=nullptr; rsMode=0; rsMask=0; rsGroup.clear(); guideX=guideY=-1; repaint();
    }
    void mouseDoubleClick(const juce::MouseEvent& e) override {
        const auto p=e.position.toInt();
        const int mi=markerAt(p); if(mi>=0){ commentDialog(p,mi); return; }
        auto* hit=hitAt(p); if(hit==nullptr)return;
        if(auto* tg=dynamic_cast<TextTag*>(hit); tg!=nullptr&&tg->vLock){ uiLayout().erase(rootKey+"."+hit->getComponentID()); uiLayout().save(); resetBoundsToFactory(hit); repaint(); return; }
        if(layoutTextable(hit)){ renameDialog(hit); return; }
        uiLayout().erase(rootKey+"."+hit->getComponentID()); uiLayout().save();
        if(orig.count(hit))hit->setBounds(orig[hit]); repaint(); }
    void mouseMove(const juce::MouseEvent& e) override {
        const auto p=e.position.toInt(); lastMouse=p; markerHover=markerAt(p);
        auto* h=hitAt(p); if(h!=hover){ hover=h; repaint(); }
        if(h!=nullptr){ const int cm=cornerMaskAt(p,h,true); setMouseCursor(cursorForMask(cm)); } else setMouseCursor(juce::MouseCursor::NormalCursor); } // 1.6.47: курсор по углам (края -- только через ALT, там зелёная подсветка)
private:
    void applyZoom(float z,float fx,float fy){ zoom=z; panFx=fx; panFy=fy; root.setTransform(baseT.followedBy(juce::AffineTransform().scaled(z,z,fx,fy))); } // 1.6.43: зум вокруг курсора
    static juce::MouseCursor cursorForMask(int m){ if(m==1||m==2)return juce::MouseCursor::LeftRightResizeCursor; if(m==4||m==8)return juce::MouseCursor::UpDownResizeCursor; if(m==(1|4)||m==(2|8))return juce::MouseCursor::TopLeftCornerResizeCursor; if(m==(2|4)||m==(1|8))return juce::MouseCursor::TopRightCornerResizeCursor; return juce::MouseCursor::NormalCursor; }
    int cornerMaskAt(juce::Point<int> p,juce::Component* it,bool cornersOnly=false){ const auto r=rectOf(*it); int m=0;
        const bool L=std::abs(p.x-r.getX())<=8,R=std::abs(p.x-r.getRight())<=8,T=std::abs(p.y-r.getY())<=8,B=std::abs(p.y-r.getBottom())<=8;
        if(cornersOnly&&((L==R)||(T==B)))return 0; // 1.6.47: угол = ровно одна X-стенка и ровно одна Y-стенка
        if(L||R||T||B){ if(L)m|=1; if(R)m|=2; if(T)m|=4; if(B)m|=8; } return m; }
    void applyBox(juce::Component* it,const juce::Rectangle<int>& b){ if(it->getParentComponent()==nullptr)return; const auto pos=it->getParentComponent()->getLocalPoint(this,b.getTopLeft()); it->setBounds(pos.x,pos.y,b.getWidth(),b.getHeight()); }
    int nearestLine(int v,const std::vector<int>& lines,int& guide){ int best=8; guide=-1; for(int L:lines){ const int d=L-v; if(std::abs(d)<std::abs(best)){best=d;guide=L;} } return best; }
    void snapResize(juce::Rectangle<int>& b,juce::Component* mover,int mask){
        guideX=guideY=-1; std::vector<int> lx,ly; for(auto* o:items){ if(o==mover)continue; const auto ro=rectOf(*o); lx.push_back(ro.getX());lx.push_back(ro.getCentreX());lx.push_back(ro.getRight()); ly.push_back(ro.getY());ly.push_back(ro.getCentreY());ly.push_back(ro.getBottom()); }
        int g;
        if((mask&1)!=0){ const int d=nearestLine(b.getX(),lx,g); if(g>=0){ b.setLeft(b.getX()+d); guideX=g; } }
        if((mask&2)!=0){ const int d=nearestLine(b.getRight(),lx,g); if(g>=0){ b.setWidth(b.getWidth()+d); guideX=g; } }
        if((mask&4)!=0){ const int d=nearestLine(b.getY(),ly,g); if(g>=0){ b.setTop(b.getY()+d); guideY=g; } }
        if((mask&8)!=0){ const int d=nearestLine(b.getBottom(),ly,g); if(g>=0){ b.setHeight(b.getHeight()+d); guideY=g; } } }
    void snap(juce::Rectangle<int>& b){
        const auto cur=b; int bestX=8,bestY=8,fixX=0,fixY=0; guideX=guideY=-1;
        for(auto* o:items){ if(o==drag)continue; const auto ro=rectOf(*o);
            const int cx[3]{cur.getX(),cur.getCentreX(),cur.getRight()}; const int ox[3]{ro.getX(),ro.getCentreX(),ro.getRight()};
            for(int i=0;i<3;++i)for(int j2=0;j2<3;++j2){ const int d=ox[j2]-cx[i]; if(std::abs(d)<std::abs(bestX)){ bestX=d; fixX=i; guideX=ox[j2]; } }
            const int cy[3]{cur.getY(),cur.getCentreY(),cur.getBottom()}; const int oy[3]{ro.getY(),ro.getCentreY(),ro.getBottom()};
            for(int i=0;i<3;++i)for(int j2=0;j2<3;++j2){ const int d=oy[j2]-cy[i]; if(std::abs(d)<std::abs(bestY)){ bestY=d; fixY=i; guideY=oy[j2]; } } }
        if(bestX<8)b.setX(cur.getX()+bestX); else guideX=-1;
        if(bestY<8)b.setY(cur.getY()+bestY); else guideY=-1;
    }
    void renameDialog(juce::Component* it){
        const juce::String key=rootKey+"."+it->getComponentID();
        auto* aw=new juce::AlertWindow("RENAME "+it->getComponentID(),"new name:",juce::MessageBoxIconType::NoIcon,nullptr);
        aw->addTextEditor("t",layoutCaptureText(it));
        aw->addButton("OK",1,juce::KeyPress(juce::KeyPress::returnKey));
        aw->addButton("FACTORY TEXT",2);
        aw->addButton("CANCEL",0,juce::KeyPress(juce::KeyPress::escapeKey));
        const juce::Component::SafePointer<juce::Component> safeIt(it);
        const juce::Component::SafePointer<LayoutOverlay> safe(this);
        aw->enterModalState(true,juce::ModalCallbackFunction::create([aw,safeIt,safe](int r){
            if(safeIt!=nullptr&&safe!=nullptr){
                const juce::String k=safe->rootKey+"."+safeIt->getComponentID();
                if(r==1){ const auto t2=aw->getTextEditorContents("t").trim(); safe->pushHistory();
                    if(!uiLayout().has(k))uiLayout().put(k,safeIt->getBounds());
                    if(uiLayout().baseOf(k).isEmpty())uiLayout().setBase(k,layoutCaptureText(safeIt));
                    uiLayout().setText(k,t2); applyLayoutText(safeIt,t2); uiLayout().save(); }
                else if(r==2){ safe->pushHistory(); const auto fb=uiLayout().baseOf(k); applyLayoutText(safeIt,fb.isNotEmpty()?fb:layoutCaptureText(safeIt)); uiLayout().setText(k,{}); uiLayout().save(); }
                safe->repaint(); }
            aw->exitModalState(0); juce::MessageManager::callAsync([aw]{delete aw; });
        }),false);
    }
    struct ColorWin final : public juce::DocumentWindow, public juce::ChangeListener { // 1.6.43: пикер цвета -- отдельное окно ОС (работает поверх редактора)
        struct Content final : public juce::Component {
            juce::ColourSelector cs{juce::ColourSelector::showColourAtTop|juce::ColourSelector::showSliders|juce::ColourSelector::showColourspace};
            juce::TextButton ok{"OK"}; std::function<void()> onOk; // 1.6.44: подтвердить цвет
            Content(){ cs.setName("pick"); addAndMakeVisible(cs); ok.onClick=[this]{ if(onOk)onOk(); }; addAndMakeVisible(ok); }
            void resized() override { cs.setBounds(0,0,getWidth(),getHeight()-36); ok.setBounds(getWidth()-88,getHeight()-32,84,27); }
        };
        Content content; std::function<void(juce::Colour)> onPick; std::function<void()> onWinClose;
        ColorWin():juce::DocumentWindow("COLOR",juce::Colours::black,juce::DocumentWindow::closeButton){ setUsingNativeTitleBar(false); content.cs.addChangeListener(this); content.onOk=[this]{ if(onWinClose)onWinClose(); }; setContentNonOwned(&content,true);
            content.setSize(300,336); setColour(backgroundColourId,juce::Colours::black); setResizable(false,false); setAlwaysOnTop(true); }
        void closeButtonPressed() override { if(onWinClose)onWinClose(); }
        void changeListenerCallback(juce::ChangeBroadcaster*) override { if(onPick)onPick(content.cs.getCurrentColour()); }
    };
    void openColorPicker(){ // 1.6.43: живой пикер -- цвет применяется сразу
        if(colorWin!=nullptr)return;
        colorWin=std::make_unique<ColorWin>();
        colorWin->onPick=[this](juce::Colour c){ pickColour=c; applyStyle(lastFid<0?0:lastFid,lastFs,c); };
        colorWin->onWinClose=[this]{ colorWin.reset(); grabKeyboardFocus(); }; // 1.6.51: фокус назад после пикера
        const auto area=juce::Desktop::getInstance().getDisplays().getPrimaryDisplay()->userArea;
        colorWin-> centreWithSize(320,340);
        colorWin->setTopLeftPosition(juce::jlimit(area.getX(),juce::jmax(area.getX(),area.getRight()-340),getScreenX()+getWidth()-360),getScreenY()+40);
        colorWin->setVisible(true); }
    void styleDialog(){ // 1.6.43: цвет / шрифт / размер текста выделенного
        juce::Component* it=sel.front();
        const juce::String k=rootKey+"."+it->getComponentID(); const auto en=uiLayout().entry(k);
        lastFid=en.fid; lastFs=en.fscale>0?en.fscale:1.0f; pickColour=en.color!=0?juce::Colour((juce::uint32)en.color):juce::Colours::white;
        auto* aw=new juce::AlertWindow("TEXT STYLE "+it->getComponentID(),"font size 0.5..3 (1 = factory); PICK COLOR opens a live picker",juce::MessageBoxIconType::NoIcon,nullptr);
        aw->addTextEditor("fs",juce::String(lastFs));
        aw->addButton("PIXEL",10); aw->addButton("SANS",11); aw->addButton("MONO",12); aw->addButton("PICK COLOR",20);
        aw->addButton("OK",1,juce::KeyPress(juce::KeyPress::returnKey));
        aw->addButton("RESET",3);
        aw->addButton("CANCEL",0,juce::KeyPress(juce::KeyPress::escapeKey));
        const juce::Component::SafePointer<LayoutOverlay> safe(this);
        const juce::Component::SafePointer<juce::Component> safeIt(it);
        aw->enterModalState(true,juce::ModalCallbackFunction::create([aw,safe,safeIt](int r){
            if(safe!=nullptr&&safeIt!=nullptr){
                const juce::String k=safe->rootKey+"."+safeIt->getComponentID();
                const float fs=juce::jlimit(0.5f,3.0f,(float)aw->getTextEditorContents("fs").getDoubleValue());
                safe->lastFs=fs;
                if(r>=10&&r<=12){ safe->lastFid=r-10; safe->applyStyle(safe->lastFid,fs,safe->pickColour); }
                else if(r==20){ aw->exitModalState(0); juce::MessageManager::callAsync([aw]{delete aw;}); safe->openColorPicker(); return; }
                else if(r==1){ safe->applyStyle(safe->lastFid<0?0:safe->lastFid,fs,safe->pickColour); }
                else if(r==3){ uiLayout().setStyle(k,0,-1,0); layoutStyleApply(safeIt,uiLayout().entry(k)); uiLayout().save(); safe->root.repaint(); }
                safe->repaint(); if(safe!=nullptr)safe->grabKeyboardFocus(); } // 1.6.51: фокус назад
            aw->exitModalState(0); juce::MessageManager::callAsync([aw]{delete aw; });
        }),false);
    }
    void applyStyle(int fid,float fs,juce::Colour col){ // 1.6.43: применить стиль к выделению
        for(auto* it:sel){ const juce::String k=rootKey+"."+it->getComponentID();
            if(!uiLayout().has(k))uiLayout().put(k,it->getBounds());
            uiLayout().setStyle(k,(int)col.getARGB(),fid,fs);
            layoutStyleApply(it,uiLayout().entry(k)); }
        uiLayout().save(); root.repaint(); }
    void resetBoundsToFactory(juce::Component* it){ // 1.6.49: теги -- к заводским (base), остальным вернёт дефолт resized()
        if(auto* tg=dynamic_cast<TextTag*>(it)){ tg->setBounds(tg->base); return; }
        if(orig.count(it))it->setBounds(orig[it]); }
    void fullRevert(juce::Component* it){
        const juce::String k=rootKey+"."+it->getComponentID();
        if(uiLayout().baseOf(k).isNotEmpty())applyLayoutText(it,uiLayout().baseOf(k));
        uiLayout().erase(k); uiLayout().save();
        resetBoundsToFactory(it); repaint(); }
    void resetAll(){
        syncItems();
        for(auto* ch:items){ const juce::String k=rootKey+"."+ch->getComponentID(); if(uiLayout().baseOf(k).isNotEmpty())applyLayoutText(ch,uiLayout().baseOf(k)); resetBoundsToFactory(ch); } // 1.6.49 FIX: теги возвращаются НА МЕСТО (base) -- название синта больше не остаётся съехавшим
        pushHistory(); uiLayout().map.clear(); uiLayout().arrs.clear(); uiLayout().groups.clear(); uiLayout().save(); root.resized(); repaint(); }
    void zDialog(){ // 1.6.44: Z -- слой выделенного (список 1..N + вперёд/назад)
        if(sel.empty())return; auto* it=sel.front(); auto* par=it->getParentComponent(); if(par==nullptr)return;
        std::vector<juce::Component*> layer; for(auto* ch:items) if(ch->getParentComponent()==par)layer.push_back(ch);
        std::sort(layer.begin(),layer.end(),[par](juce::Component* a,juce::Component* b){ return par->getIndexOfChildComponent(a)<par->getIndexOfChildComponent(b); });
        const int K=static_cast<int>(layer.size()); int cur=0;
        for(int i=0;i<K;++i)if(layer[(size_t)i]==it){cur=i;break;}
        juce::PopupMenu m; m.setLookAndFeel(&smallMenuLaf());
        m.addItem(1,"НА СЛОЙ ВПЕРЁД",cur<K-1); m.addItem(2,"НА СЛОЙ НАЗАД",cur>0); m.addSeparator();
        for(int i=0;i<K;++i)m.addItem(10+i,"СЛОЙ "+juce::String(i+1)+(i==0?" (САМЫЙ ЗАДНИЙ)":i==K-1?" (САМЫЙ ПЕРЕДНИЙ)":""),true,i==cur);
        const auto sp=juce::Component::SafePointer<LayoutOverlay>(this); const auto spi=juce::Component::SafePointer<juce::Component>(it);
        m.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(this),[sp,spi,layer,K,cur](int r){
            if(sp==nullptr||spi==nullptr||r<=0)return; auto* pr=spi->getParentComponent(); if(pr==nullptr)return;
            const auto saveZ=[&]{ uiLayout().setZ(sp->rootKey+"."+spi->getComponentID(),pr->getIndexOfChildComponent(spi),spi->getBounds()); uiLayout().save(); sp->repaint(); }; // 1.6.44: uiLayout() -- свободная функция
            if(r==1||r==2){ const int idx=pr->getIndexOfChildComponent(spi); const int nz=idx+(r==1?1:-1);
                if(nz>=0&&nz<pr->getNumChildComponents()){ pr->removeChildComponent(idx); pr->addChildComponent(spi,nz); saveZ(); } return; }
            const int rank=r-10; if(rank<0||rank>=K||rank==cur)return;
            juce::Component* ref=layer[(size_t)rank]; pr->removeChildComponent(pr->getIndexOfChildComponent(spi));
            const int ri=pr->getIndexOfChildComponent(ref); if(ri<0){ sp->repaint(); return; }
            pr->addChildComponent(spi,rank<cur?ri:ri+1); saveZ();
        if(sp!=nullptr)sp->grabKeyboardFocus(); }); // 1.6.51: фокус назад (хоткеи не умирают)
    }
    void pushHistory(){ histUndo.push_back(uiLayout().toJSON()); if(histUndo.size()>60)histUndo.erase(histUndo.begin()); histRedo.clear(); }
    void undoOp(){ if(histUndo.empty())return; histRedo.push_back(uiLayout().toJSON()); uiLayout().loadFrom(histUndo.back()); histUndo.pop_back(); uiLayout().save(); root.resized(); repaint(); } // 1.6.50 FIX: откат сразу в файл (файл == хранилище, "старая версия в layout.json" убрана)
    void redoOp(){ if(histRedo.empty())return; histUndo.push_back(uiLayout().toJSON()); uiLayout().loadFrom(histRedo.back()); histRedo.pop_back(); uiLayout().save(); root.resized(); repaint(); } // 1.6.50 FIX: повтор сразу в файл
    void editMenu(){ // 1.6.52: вся механика редактора -- в меню F2 (канва чистая, клики -- только элементам)
        auto* aw=new juce::AlertWindow("LAYOUT EDIT -- "+juce::String(rootKey),"COPY JSON = koordinaty v bufer; SAVE = file==screen; RELOAD = file wins; ERASE = clean default",juce::MessageBoxIconType::NoIcon,nullptr);
        aw->addButton("COPY JSON",6,juce::KeyPress(juce::KeyPress::returnKey)); // 1.6.55: ПЕРВЫМ и на Enter (пользователь)
        aw->addButton("SAVE",1); aw->addButton("RELOAD FILE",2); aw->addButton("RESET ALL",3);
        aw->addButton("UNDO",4); aw->addButton("REDO",5);
        aw->addButton("COPY COORDS",10); aw->addButton("ERASE FILE",7); aw->addButton("DEBUG: PUT PROBE",9); // 1.7.0: COPY COORDS вернулся (x,y из-под курсора)
        aw->addButton("CLOSE",0,juce::KeyPress(juce::KeyPress::escapeKey));
        const auto safe=juce::Component::SafePointer<LayoutOverlay>(this);
        aw->enterModalState(true,juce::ModalCallbackFunction::create([aw,safe](int r){
            if(safe!=nullptr){
                if(r==1)safe->saveFlash();
                else if(r==2){ safe->pushHistory(); uiLayout().load(); safe->root.resized(); safe->repaint(); }
                else if(r==3)safe->resetAll();
                else if(r==4)safe->undoOp();
                else if(r==5)safe->redoOp();
                else if(r==6){ safe->copiedFlash=2; juce::SystemClipboard::copyTextToClipboard(uiLayout().toJSON()); safe->repaint(); // 1.6.52: весь расклад в буфер
                    juce::Timer::callAfterDelay(900,[safe]{ if(safe!=nullptr){ safe->copiedFlash=0; safe->repaint(); } }); }
                else if(r==10){ juce::SystemClipboard::copyTextToClipboard(juce::String(safe->lastMouse.x)+","+juce::String(safe->lastMouse.y)); safe->copiedFlash=2; safe->repaint(); // 1.7.0: x,y курсора в буфер (координаты в JSON вручную)
                    juce::Timer::callAfterDelay(900,[safe]{ if(safe!=nullptr){ safe->copiedFlash=0; safe->repaint(); } }); }
                else if(r==7)safe->eraseFile();
                else if(r==9){ uiLayout().put(safe->rootKey+".__probe",juce::Rectangle<int>(1,1,10,10)); safe->saveFlash(); } // 1.6.53: проверка put+save МИМО жеста (в файле должен появиться __probe)
                safe->grabKeyboardFocus(); safe->repaint(); } // 1.6.52: фокус назад
            aw->exitModalState(0); juce::MessageManager::callAsync([aw]{delete aw; });
        }),false);
    }
    void eraseFile(){ pushHistory(); uiLayout().eraseAll(); uiLayout().file().deleteFile(); uiLayout().save(); root.resized(); repaint(); } // 1.6.52: стереть json -- чистый заводской расклад
    void saveFlash(){ const bool ok=uiLayout().save(); savedFlash=ok?1:2; repaint(); // 1.6.51: FAIL виден
        juce::Timer::callAfterDelay(900,[safe=juce::Component::SafePointer<LayoutOverlay>(this)]{ if(safe!=nullptr){ safe->savedFlash=0; safe->repaint(); } }); }
    int markerAt(juce::Point<int> p){ for(size_t i=0;i<uiLayout().comments.size();++i){ const auto& c=uiLayout().comments[(size_t)i]; if(c.key==rootKey&&juce::Rectangle<int>(c.x,c.y,14,14).contains(p))return static_cast<int>(i); } return -1; }
    void commentDialog(juce::Point<int> pos,int index){
        const bool isNew=index<0;
        auto* aw=new juce::AlertWindow(isNew?"NOTE FOR AGENT (F2)":"EDIT NOTE","text for the agent; COPY COORDS puts the x,y coordinate into the clipboard",juce::MessageBoxIconType::NoIcon,nullptr);
        aw->addTextEditor("t",isNew?juce::String():uiLayout().comments[(size_t)index].text);
        aw->addButton("SAVE",1,juce::KeyPress(juce::KeyPress::returnKey));
        aw->addButton("COPY COORDS",2);
        aw->addButton("CANCEL",0,juce::KeyPress(juce::KeyPress::escapeKey));
        aw->setTopLeftPosition(getScreenX()+juce::jlimit(2,juce::jmax(2,getWidth()-350),pos.x),getScreenY()+juce::jlimit(2,juce::jmax(2,getHeight()-130),pos.y+12));
        const juce::Component::SafePointer<LayoutOverlay> safe(this);
        aw->enterModalState(true,juce::ModalCallbackFunction::create([aw,safe,pos,index](int r){
            if(safe!=nullptr){
                if(r==1){ const auto t2=aw->getTextEditorContents("t").trim(); safe->pushHistory();
                    if(index<0){ UiLayoutStore::Comment c; c.x=pos.x; c.y=pos.y; c.key=safe->rootKey; c.text=t2; uiLayout().comments.push_back(c); }
                    else if((size_t)index<uiLayout().comments.size()) uiLayout().comments[(size_t)index].text=t2;
                    uiLayout().save(); safe->repaint(); }
                else if(r==2){ juce::SystemClipboard::copyTextToClipboard(juce::String(pos.x)+","+juce::String(pos.y)); safe->commentDialog(pos,index); }
            }
            if(safe!=nullptr)safe->grabKeyboardFocus(); // 1.6.51: фокус назад
            aw->exitModalState(0); juce::MessageManager::callAsync([aw]{delete aw; });
        }),false);
    }
    juce::Component* hitAt(juce::Point<int> p){ // 1.6.48: клик берёт САМЫЙ верхний ВИДИМЫЙ элемент (невидимый viewport больше не перехватывает)
        juce::Component* best=nullptr; int bz=-1;
        for(auto* ch:items){ if(!ch->isVisible()||!rectOf(*ch).contains(p))continue;
            auto* par=ch->getParentComponent(); const int z=par?par->getIndexOfChildComponent(ch):0;
            if(z>bz){bz=z;best=ch;} }
        return best; }
    juce::Rectangle<int> rectOf(juce::Component& ch){ const auto tl=getLocalPoint(&ch,juce::Point<int>(0,0)); return {tl.x,tl.y,ch.getWidth(),ch.getHeight()}; }
    juce::Component& root; juce::String rootKey;
    std::vector<juce::Component*> items; std::map<juce::Component*,juce::Rectangle<int>> orig;
    std::vector<juce::Component*> sel; std::map<juce::Component*,juce::Rectangle<int>> groupSelStart; // 1.6.43: выделение/группы
    bool marqActive=false; juce::Rectangle<int> marq; juce::Point<int> marqStart; // 1.6.43: рамка
    float zoom=1.0f; juce::AffineTransform baseT; bool panning=false; juce::Point<int> panSX; float panFx=0,panFy=0; int lastFid=-1; float lastFs=1.0f; juce::Colour pickColour{juce::Colours::white}; std::unique_ptr<ColorWin> colorWin; // 1.6.43/1.7.5: MMB-pan
    juce::Component* hover=nullptr; juce::Component* drag=nullptr; // drag == rsItem (нужен snap())
    int rsMode=0; // 1.6.47: ЕДИНЫЙ жест -- 0 нет, 1 двигать, 2 край (ALT), 3 угол, 4 размер (SHIFT); от снапшота
    int rsMask=0; juce::Point<int> rsStart; juce::Rectangle<int> rsSnap; juce::Component* rsItem=nullptr;
    std::map<juce::Component*,juce::Rectangle<int>> rsGroup; int lastLiveN=-1; juce::String preState; // 1.6.48: состояние до жеста (для undo)
    int guideX=-1,guideY=-1,savedFlash=0,markerHover=-1,copiedFlash=0; juce::String wroteInfo; // 1.6.55: что записалось последним жестом
    juce::Point<int> lastMouse; std::vector<juce::String> histUndo,histRedo;
};
// 1.6.32: UNDO/REDO -- снимок всего состояния (ручки, ARP-страницы, MSEG, матрица).
class UndoRedoButton final : public juce::Button {
public:
    explicit UndoRedoButton(bool isUndo):juce::Button(isUndo?"UNDO":"REDO"),undoMode(isUndo){}
    std::function<void()> action;
    void paintButton(juce::Graphics& g,bool,bool down)override{
        g.fillAll(down?juce::Colours::white:juce::Colours::black);
        g.setColour(down?juce::Colours::black:juce::Colours::white);
        const int w=getWidth(); // 1.6.33: КВАДРАТНАЯ пиксель-стрелка (крюк вверх, головка "<"), REDO зеркально; подпись UNDO/REDO
        auto sq=[&](int x,int y,int ww,int hh){g.fillRect(undoMode?x:w-2-x-ww,y,ww,hh);};
        sq(3,9,2,4);sq(5,7,2,8);sq(7,5,2,7);sq(7,10,13,2);sq(19,3,2,7);
        pixel::text(g,undoMode?"UNDO":"REDO",{0,19,w,10},7,true);}
    void clicked()override{if(action)action();}
private: bool undoMode;
};
// Outlined pixel button. `dragHandler(dx)` fires while the pointer moves horizontally,
// so the same control supports click (popup / toggle) and press-drag (cycle values).
class PixelButton : public juce::TextButton {
public:
    explicit PixelButton(const juce::String& s):juce::TextButton(s){}
    int textHeight=28;
    int textAlign=0; // 0 centred, 1 left
    bool outlined=true;
    bool flipIcon=false; // 1.7.6: рисовать квадратик со стрелкой вместо текста (DETACH)
    // 1.6.13: стрелки по бокам названия (шире влево/вправо = один шаг пресета).
    bool arrows=false;std::function<void(int)> arrowClick; // -1 = влево, +1 = вправо
    std::function<void(int,bool)> dragHandler;std::function<void()> doubleClickHandler;int dragPixelsPerStep=6;double wheelAccum=0;int dragStartY=0; // 1.6.12: bool = мелкий шаг (Alt/Shift)
    int arrowSpan() const { // конец текста по правилам pixel::text (для хитбоксов стрелок)
        int scale=std::max(1,textHeight/7);const auto s=getButtonText().toUpperCase();
        while(scale>1&&s.length()*6*scale>getWidth()-10)--scale;
        const int w=s.length()*6*scale-scale;
        return (textAlign==0?(getWidth()-w)/2:5)+w;
    }
    void mouseDoubleClick(const juce::MouseEvent& e)override{if(doubleClickHandler)doubleClickHandler();else juce::TextButton::mouseDoubleClick(e);}
    bool verticalDrag=false; // 1.6.24: драг по вертикали (имя машины)
    void mouseWheelMove(const juce::MouseEvent& e,const juce::MouseWheelDetails& w)override{
        if(dragHandler&&std::abs(w.deltaY)>0.0001f){ // 1.6.24: копим дельту -- колесо срабатывает каждый раз, а не через раз
            wheelAccum+=w.deltaY;
            while(wheelAccum>=0.12f){wheelAccum-=0.12f;dragHandler(1,false);}
            while(wheelAccum<=-0.12f){wheelAccum+=0.12f;dragHandler(-1,false);}}
        else juce::TextButton::mouseWheelMove(e,w);}
    std::function<void()> rightClick; // 1.6.14: ПКМ-хук (например, кнопка ARP -> страница ARP)
    int arrowHl=-1; bool arrowPressed=false; // 1.6.25: белая ТОЛЬКО нажатая стрелка
    bool grayBlink=false; // 1.6.19: серое моргание фона -- страница, которую модулирует прицел
    static bool blinkPhase(){ return (juce::Time::getMillisecondCounter()/280)&1; }
    uint32_t flashUntilMs=0; // 1.6.29: короткая вспышка-подтверждение (смена пресета стрелкой)
    void flashFor(int ms){auto safe=juce::Component::SafePointer<PixelButton>(this);flashUntilMs=juce::Time::getMillisecondCounter()+static_cast<uint32_t>(ms);repaint();
        juce::Timer::callAfterDelay(ms,[safe]{if(safe==nullptr)return;safe->flashUntilMs=0;safe->repaint();});}
    bool crosshair=false; // 1.7.9: рисовать прицел (как крестик DEST в матрице)
    void paintButton(juce::Graphics& g,bool /*over*/,bool down)override{
        const bool filled=(down&&outlined&&juce::Desktop::getInstance().getMainMouseSource().isDragging())||getToggleState()||juce::Time::getMillisecondCounter()<flashUntilMs; // 1.6.29: залипание "нажато" самоликвидируется
        g.fillAll(filled?ink:(grayBlink&&blinkPhase()?juce::Colour(0xff8f8f8f):juce::Colours::black));
        if(outlined){g.setColour(ink);g.drawRect(getLocalBounds(),1);} // 1.6.31: стенка заподлицо -- рамка имени машины совпадает с панелями
        g.setColour(filled?knockout:ink);
        if(flipIcon){ // 1.7.6: DETACH -- квадратик и стрелка, будто перелистываешь страницу в отдельное окно
            const int cy=getHeight()/2;
            g.drawRect(9,cy-6,12,12,1);
            g.drawLine(23,cy,32,cy,1);
            for(int i=0;i<4;++i){const int ww=std::max(1,8-2*i);g.fillRect(28+2*i,cy-i,ww,1);g.fillRect(28+2*i,cy+i,ww,1);}
            return; }
        auto textBox=getLocalBounds().reduced(5,0); if(crosshair)textBox.setRight(getWidth()-24); // 1.7.9: место под прицел
        pixel::text(g,getButtonText(),arrows?textBox.withRight(getWidth()-44):textBox,textHeight,textAlign==0);
        if(crosshair){const int cx=getWidth()-13,cy=getHeight()/2;g.drawLine(float(cx-5),float(cy),float(cx+5),float(cy),1);g.drawLine(float(cx),float(cy-5),float(cx),float(cy+5),1);g.drawRect(cx-2,cy-2,4,4,1);} // 1.7.9
        if(arrows){ // 1.6.24/25: стрелки в ОТДЕЛЬНОЙ рамке; нажатая стрелка белеет, вторая не реагирует
            const int end=getWidth()-54,cy=getHeight()/2; // 1.6.33: полоса стрелок вплотную к правой стенке кнопки
            g.setColour(juce::Colours::black);g.fillRect(end-2,2,56,getHeight()-4); // 1.6.26: СВОЙ чёрный фон полосы -- нажатие имени/списка не заливает стрелки белым
            g.setColour(ink);g.drawRect(end-3,cy-13,56,26,1);g.drawVerticalLine(end-3,cy-13,cy+13);
            for(int i=0;i<5;++i){
                const int w=std::max(1,10-2*i);
                g.setColour(arrowHl==0?knockout:ink);g.fillRect(end+6+2*i,cy-i,w,1);g.fillRect(end+6+2*i,cy+i,w,1); // "<"
                g.setColour(arrowHl==1?knockout:ink);g.fillRect(end+30,cy-i,w,1);g.fillRect(end+30,cy+i,w,1);       // ">"
            }
        }
    }
    void mouseDown(const juce::MouseEvent&e)override{
        if(rightClick&&e.mods.isPopupMenu()){rightClick();return;}
        if(arrows&&arrowClick){const int end=getWidth()-54; // 1.6.24/33: полоса стрелок забронирована целиком, вплотную к стенке
            if(e.x>=end-8){ // 1.6.28: запас у левой стрелки; список из зоны стрелок не открыть
                arrowHl=e.x<end+24?0:1;arrowPressed=true;repaint();
                if(e.x<end+24)arrowClick(-1);else arrowClick(1);
                return;}
        }
        dragStartX=e.x;dragStartY=static_cast<int>(e.getScreenPosition().y);dragAccum=0;wasDragged=false;juce::TextButton::mouseDown(e);
    }
    void mouseDrag(const juce::MouseEvent&e)override{
        if(arrowPressed)return; // 1.6.25: драг, начатый со стрелки, не листает пресеты
        if(!dragHandler||!e.mods.isLeftButtonDown())return; // 1.6.21: ПКМ больше не крутит значения
        const int dx=verticalDrag?(dragStartY-static_cast<int>(e.getScreenPosition().y)):(e.x-dragStartX); // 1.6.24: имя машины листается ВВЕРХ/ВНИЗ
        if(verticalDrag&&std::abs(dx)>3)wasDragged=true; // 1.6.28: драг имени -- никогда не открывает список
        const int pixels=std::max(1,dragPixelsPerStep);
        // 1.6.12: с Alt или Shift -- мелкий шаг (десятые), без -- целые.
        const bool fine=e.mods.isAltDown()||e.mods.isShiftDown();
        const int scale=fine?6:1;
        if(std::abs(dx)>=pixels){wasDragged=true;const int steps=(dx-dragAccum)/(pixels*scale);if(steps!=0){dragAccum+=steps*pixels*scale;dragHandler(steps,fine);}}
    }
    void mouseUp(const juce::MouseEvent&e)override{if(arrowPressed){arrowPressed=false;arrowHl=-1;wasDragged=false;repaint();return;}juce::TextButton::mouseUp(e);wasDragged=false;} // 1.6.28: стрелки -- изолированный жест, клик не доходит до кнопки
    void clicked()override{if(wasDragged)return;juce::TextButton::clicked();}
private:
    int dragStartX=0,dragAccum=0;bool wasDragged=false;
};
// 1.6.19: MSEG OUT назначается как LFO: прицел с подсветкой ручки, отпускание
// над ручкой = маршрут в матрицу. Без JUCE drag-призрака.
// 1.6.21: слой ПРОВОДА -- рисует патч-корд от гнезда к курсору во время
// прицела; не перехватывает мышь, всегда поверх детей.
// 1.6.29: галка-квадрат на всю высоту кнопки (ARP ENABLE): рамка закрыта
// со всех четырёх сторон, галка -- заливка по центру.
class SquareToggle final : public juce::ToggleButton {
public:
    SquareToggle(){setClickingTogglesState(true);}
    void paintButton(juce::Graphics& g,bool,bool)override{
        g.fillAll(juce::Colours::black);const auto b=getLocalBounds().reduced(1);
        g.setColour(ink.withAlpha(isEnabled()?0.85f:0.3f));g.drawRect(b,1);
        if(getToggleState()){g.setColour(ink);g.fillRect(b.reduced(4));}
    }
};
class CableLayer final : public juce::Component {
public:
    CableLayer(){setInterceptsMouseClicks(false,false);setAlwaysOnTop(true);}
    void begin(juce::Point<int> localFrom){on=true;from=localFrom;to=localFrom;repaint();}
    void drag(juce::Point<int> localTo){to=localTo;repaint();}
    void hide(){on=false;repaint();}
    void paint(juce::Graphics& g) override {
        if(!on)return;
        juce::Path p;p.startNewSubPath({float(from.x),float(from.y)});
        p.quadraticTo({(from.x+to.x)*0.5f,std::max(from.y,to.y)+40.0f},juce::Point<float>(float(to.x),float(to.y)));
        g.setColour(juce::Colours::white);g.strokePath(p,juce::PathStrokeType(3.0f));
        g.fillEllipse(float(to.x)-5.0f,float(to.y)-5.0f,10.0f,10.0f);
    }
    bool on=false;juce::Point<int> from,to;
}; // 1.6.24: только ЖИВОЙ кабель прицела (постоянные провода удалены -- двоились)

// 1.6.21: кнопка-источник модуляции: слева квадрат с НОМЕРОМ страницы (клик --
// выбрать страницу), справа "гнездо" -- из него тянется ПРОВОД к ручке-цели
// (как на примере dot and patchcord). Выбранная страница = БЕЛЫЙ квадрат,
// страница-цель прицела МИГАЕТ серым, источник во время тяги чуть притушен.
class ModSourceButton final : public juce::Component, public juce::SettableTooltipClient {
public:
    explicit ModSourceButton(const juce::String& n):label(n){setMouseCursor(juce::MouseCursor::PointingHandCursor);}
    juce::String label;
    std::function<void()> numberClick;                               // клик по квадрату = выбор страницы
    std::function<void(juce::Point<int>)> numberDrag;                // 1.8.0b: драг от кнопки = выбор страницы по ряду
    std::function<void(bool)> beginDrag;                             // bool = drag into matrix (RMB)
    std::function<void(juce::Point<int>)> dragMove;
    std::function<void(juce::Point<int>,bool,bool)> endDrag;         // screen, dragged, matrix
    bool selected=false,grayBlink=false,dimmed=false;
    bool jackEnabled=true,jackOnly=false; // 1.8.0b: кнопки страниц БЕЗ гнезда, ОДИН общий патч-корд (jackOnly -- компонент-гнездо)
    static bool blinkPhase(){ return (juce::Time::getMillisecondCounter()/280)&1; }
    juce::Rectangle<int> numberBounds() const { return jackEnabled&&!jackOnly?getLocalBounds().withRight(getWidth()-20):getLocalBounds(); }
    juce::Rectangle<int> jackBounds() const { return jackOnly?getLocalBounds():jackEnabled?getLocalBounds().withLeft(getWidth()-20):juce::Rectangle<int>(); }
    juce::Point<int> jackScreenCentre() const { return localPointToGlobal(jackBounds().getCentre()); }
    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::black);
        const auto num=numberBounds();
        const bool blink=grayBlink&&blinkPhase();
        juce::Colour fill=selected?juce::Colours::white:juce::Colours::black;
        if(dimmed)fill=juce::Colour(0xffc8c8c8);
        if(blink)fill=juce::Colour(0xff8f8f8f);
        g.setColour(juce::Colours::white);g.drawRect(num.reduced(1),1);
        g.setColour(fill);g.fillRect(num.reduced(3));
        if(!jackOnly){g.setColour((selected&&!blink&&!dimmed)?juce::Colours::black:juce::Colours::white);
            pixel::text(g,label,num,12,true);}
        else{g.setColour(selected?juce::Colours::white:juce::Colours::black);g.fillRect(num.reduced(2));} // фон гнезда
        if(jackEnabled){const auto c=jackBounds().getCentre();
            g.setColour(juce::Colours::white);g.drawEllipse(float(c.x)-6.0f,float(c.y)-6.0f,12.0f,12.0f,1.0f);
            if(dimmed)g.fillEllipse(float(c.x)-3.0f,float(c.y)-3.0f,6.0f,6.0f);}
    }
    void mouseDown(const juce::MouseEvent& e) override {
        rightDrag=e.mods.isRightButtonDown();dragStarted=false;
        if(jackBounds().contains(e.x,e.y))return; // гнездо: тянем провод, не кликаем
        if(jackOnly)return;
        if(numberClick)numberClick();             // клик по номеру = выбрать страницу
    }
    void mouseDrag(const juce::MouseEvent& e) override {
        if(jackBounds().contains(e.getMouseDownX(),e.getMouseDownY())){
            if(!dragStarted){if(e.getDistanceFromDragStart()<6)return;dragStarted=true;if(beginDrag)beginDrag(rightDrag);}
            if(dragMove)dragMove(e.getScreenPosition());
        }
        else if(numberDrag&&e.getDistanceFromDragStart()>3)numberDrag(e.getScreenPosition()); // 1.8.0b: press-and-hold на кнопке LFO -- ведёшь по ряду, страницы переключаются
    }
    void mouseUp(const juce::MouseEvent& e) override { if(dragStarted&&endDrag)endDrag(e.getScreenPosition(),true,rightDrag); dragStarted=false; }
private:
    bool rightDrag=false,dragStarted=false;
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
    if(target<0)return "OFF";
    if(target==244)return "ARP START";
    if(target==245)return "ARP END";
    if(target==246)return "P-LOCK START";
    if(target==247)return "P-LOCK END";
    if(target>=236)return "MSEG"+juce::String(target-235)+" RATE";
    if(target>=212)return "P2 LFO"+juce::String(4+(target-212)/8)+" "+nova::pageLabel(12+(target-212)/8,(target-212)%8);
    if(target>=188)return "P1 LFO"+juce::String(4+(target-188)/8)+" "+nova::pageLabel(6+(target-188)/8,(target-188)%8);
    if(target>=164)return "P2 LFO"+juce::String(1+(target-164)/8)+" "+nova::pageLabel(9+(target-164)/8,(target-164)%8);
    if(target>=132){ // P2 machine / AMP / FILT / EFFX target IDs 132..163
        const int t2=target-132;
        if(t2==15)return "P2 AMP MIX";
        if(t2<8){const auto& fxm=MonomachineNovaAudioProcessor::p2FxMachines();
            auto* mraw=p.parameters.getRawParameterValue("p2_machine");
            const int sel=juce::jlimit(0,static_cast<int>(fxm.size())-1,juce::roundToInt(mraw?mraw->load():0.0f));
            return "P2 SYNT "+juce::String(fxm[static_cast<size_t>(sel)].synthParams[static_cast<size_t>(juce::jlimit(0,7,t2))].name);}
        const char* p2sec[]{"AMP","FILT","EFFX"};
        const int sec=(t2-8)/8;
        return "P2 "+juce::String(p2sec[sec])+" "+nova::pageLabel(sec,(t2-8)%8);}
    if(target==131)return "LEGACY LFO FM"; // real retained target, never an empty separator
    if(target>=67)return "ROUTE "+juce::String(target-66);
    if(target==nova::kPitchMatrixTarget)return "P1 PITCH";
    if(target>=64)return target==64?"ARP RATE":"ARP GATE";
    if(target>=56)return "MSEG"+juce::String(target-55)+" OUT";
    if(target<8)return "P1 SYNT "+juce::String(nova::machines()[static_cast<size_t>(p.machineIndex())].synthParams[static_cast<size_t>(juce::jlimit(0,7,target))].name);
    const char* names[]{"P1 AMP","P1 FILT","P1 EFFX","P1 LFO1","P1 LFO2","P1 LFO3"};
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
    std::function<void()> pushEdit; // 1.6.32: снимок состояния ПЕРЕД правкой (undo)
    void attachUndo(juce::Slider::Listener* l){slider.addListener(l);} // 1.6.32: кручение ручки = точка отката
    std::function<void()> openSamples;std::function<void(int)> openEnvelope;std::function<void(int,juce::Point<int>)> openModes;std::function<void(uint8_t)> addMsegModulation;std::function<void(uint8_t)> pickTarget;std::function<void(juce::Point<int>)> openRepitch;std::function<void(juce::Point<int>)> openDsnd;std::function<void(juce::Point<int>)> openDelayFeedbackQ;std::function<void(juce::Point<int>)> openPorta; // 1.6.14: PORT -- окно скорости
    void setPickMode(bool enabled,bool matrixPick=false){pickMode=enabled&&modTarget;pickMatrix=matrixPick&&pickMode;if(!pickMode)setHover(false);slider.setVisible(parameter&&!isChoice);slider.setInterceptsMouseClicks(!pickMode,false);repaint();} // 1.6.31: рамка прицела не залипает // 1.6.28: в прицеле ручка НЕ крутится (ЛКМ = мапить, Alt+ЛКМ = крутить)
    // 1.6.13: подсветка ТОЛЬКО текущей ячейки прицела -- шлейфа на пройденных
    // ручках больше нет (раньше каждая ячейка вспыхивала и гасла ~секунду).
    void setHover(bool h){ if(hoverNow!=h){ hoverNow=h; repaint(); } }
    bool isTarget(uint8_t target) const noexcept { return canBeTarget() && modulationTarget() == target; }
    // 1.6.5: ячейки быстрых настроек MSEG не являются целями модуляции.
    bool canBeTarget() const noexcept { return modTarget && parameter != nullptr; }
    uint8_t targetId() const noexcept { return modulationTarget(); }
    void setTargetOverride(int t){ targetOverride=t; } // 1.8.0: P2-цели ячейке задаёт Surface
    std::function<void(uint8_t)> pickHover;
    // Direct LFO PAGE/DEST edits report their real target to Surface. They do
    // not navigate P1/P2 or LFO pages. A direct acknowledgement is transient:
    // repeated edits replace the old frame instead of leaving a static trail.
    std::function<void(uint8_t)> lfoDestinationChanged;
    void flashDirectLfoOutline(juce::uint32 milliseconds=180){
        directOutlineDuration=milliseconds;directOutlineUntil=juce::Time::getMillisecondCounter()+milliseconds;repaint();
    }
    void clearDirectLfoOutline(){if(directOutlineUntil!=0){directOutlineUntil=0;directOutlineDuration=0;repaint();}}
    juce::String labelOverride; // 1.6.41: LAYOUT EDIT -- оверрайд подписи ячейки
    juce::String layoutBaseText() const { return label; }
    void layoutSetText(const juce::String& s){ if(labelOverride!=s){ labelOverride=s; repaint(); } }
    bool isInterestedInDragSource(const juce::DragAndDropTarget::SourceDetails& details) override { return details.description.toString() == "MSEG" && parameter != nullptr && !isChoice; }
    void itemDropped(const juce::DragAndDropTarget::SourceDetails& details) override { if (details.description.toString() == "MSEG" && addMsegModulation) addMsegModulation(modulationTarget()); }
    bool isInterestedInFileDrag(const juce::StringArray& files)override{return machine==7&&knob==2&&!files.isEmpty();}
    void filesDropped(const juce::StringArray& files,int,int)override{
        int slot=juce::roundToInt(processor.parameters.getRawParameterValue("m7_2")->load());
        juce::String errors;for(const auto& name:files){if(slot>=24)break;auto error=processor.loadSample(slot++,juce::File(name));if(error.isNotEmpty())errors+=error+"\n";}
        if(errors.isNotEmpty())juce::AlertWindow::showMessageBoxAsync(juce::MessageBoxIconType::WarningIcon,"Sample import",errors);
    }
    juce::String choiceName(int value)const{
        if(pageIndex>=3&&knob==1){
            const auto* pageRaw=processor.parameters.getRawParameterValue(nova::pageParam(pageIndex,0));
            const int page=juce::jlimit(0,nova::kLfoPageChoiceCount-1,juce::roundToInt(pageRaw?pageRaw->load():0.0f));
            const int destination=juce::jlimit(0,nova::kLfoDestinationCount-1,value);
            const int target=nova::lfoDirectTarget(pageIndex>=nova::kP2LfoControlPageFirst,page,destination);
            if(target==nova::kPitchMatrixTarget){const char* ranges[]{"1ST","2ST","3ST","5ST","7ST","1OCT","2OCT","3OCT"};return ranges[destination];}
            return targetName(processor,target);
        }
        const auto names=parameter->getAllValueStrings();return names.size()>value?names[value]:parameter->getText(hostFor(*parameter,value),32);
    }
    void bind(const juce::String& parameterId,const juce::String& name,int page=-1,int index=-1,int engine=-1,bool modTargetEnabled=true,bool suppressMenus=false){ // 1.8.0: +suppressMenus (P2 -- секционные режимы свои)
        // 1.6.14: та же ячейка -- не перестраиваем (мгновенное переключение страниц LFO).
        if(id==parameterId&&label==name&&pageIndex==page&&knob==index&&machine==engine&&modTarget==modTargetEnabled&&parameter==processor.parameters.getParameter(id==juce::String()?juce::String(" "):id))return;
        attachment.reset();id=parameterId;label=name;pageIndex=page;knob=index;machine=engine;modTarget=modTargetEnabled;
        parameter=id.isEmpty()?nullptr:processor.parameters.getParameter(id);
        slider.curveMenu={};if(pageIndex==0&&(knob==0||knob==1||knob==2||knob==3))slider.curveMenu=[this](juce::Point<int>){if(openEnvelope)openEnvelope(knob);}; // 1.6.13: HOLD тоже огибающая
        // RMB elsewhere opens the DSP mode list of the section this knob belongs to
        // (prompt: "по пкм крутилка можно сделать переключение режима"). 1.6.8: the
        // popup spawns at the cursor, not at the DSP MODE button (SRR complaint).
        slider.modeMenu={};
        // Do not confuse the native CHORUS feedback word with the separate FB
        // LOOP panel. The former is inside the original chorus core and can
        // intentionally sustain a tail even while the global FB LOOP is OFF.
        if(id=="m15_4"||id=="p2m15_4")slider.setTooltip("CHORUS FB: feedback INSIDE the native CHORUS machine. It is independent of FB LOOP and its CLIP switch on the FX page.");
        else if(pageIndex==2&&(knob==6||knob==7))slider.setTooltip("DBAS/DWID: delay feedback-filter edges. Right-click either knob for optional DBAS Q / DWID Q.");
        else slider.setTooltip({});
        if(!suppressMenus){ // 1.8.0: у P2-ячеек секционные режимы (p2_mode_*) живут в комбох MODE -- ПКМ-меню P1-секций не открываем
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
        // DBAS and DWID stay ordinary visible knobs; their optional Q values
        // live in a compact panel opened by right-clicking either control.
        if(pageIndex==2&&(knob==6||knob==7))slider.modeMenu=[this](juce::Point<int> at){if(openDelayFeedbackQ)openDelayFeedbackQ(at);};
        if(pageIndex==0&&knob==7)slider.modeMenu=[this](juce::Point<int> at){if(openPorta)openPorta(at);}; // 1.6.14: PORT -- своё окно скорости
        isChoice=parameter&&(parameter->getAllValueStrings().size()>0||(pageIndex>=3&&knob==1));
        slider.setVisible(parameter&&!isChoice);setMouseCursor(juce::MouseCursor::NormalCursor); // 1.6.24: указательный палец -- только у кнопок-гнёзд
        if(parameter&&!isChoice){attachment=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.parameters,id,slider);slider.setDoubleClickReturnValue(true,parameter->convertFrom0to1(parameter->getDefaultValue()));} // double-click restores the stored default
        repaint();
    }
    juce::String displayedValue()const{
        if(!parameter)return "---";float v=parameter->convertFrom0to1(parameter->getValue());int raw=juce::roundToInt(v);
        if(pageIndex>=3&&knob==1)return choiceName(juce::jlimit(0,nova::kLfoDestinationCount-1,raw));
        if(isChoice)return parameter->getText(parameter->getValue(),32);
        if(machine==10&&knob==0)return juce::String(v/16,1);
        if(machine==10&&knob==4)return juce::String(std::pow(2.0f,(v-32)/24),2);
        if((machine==8||machine==9)&&(label=="1FRQ"||label=="2FRQ"||label=="3FRQ"))return juce::String(monomachine::getFmListedRatio(static_cast<uint8_t>(monomachine::mnm::fmStepIndex((float)raw))),2); // fm-stat-1: same measured knob->step map as the DSP core
        if(machine==2&&knob==3)return juce::String(raw-64);
        if(label=="TUNE"&&machine!=17)return juce::String(raw-64);
        if(label=="INP")return juce::String(raw-64); // 1.8.1: INPUT всех FX-слотов -- биполярный как на железе (64 = 0, -64..+63); звук не менялся
        // DIST (AMP page) and DSND (FX page) are bipolar on hardware: centre 0, -64..+63.
        if((pageIndex==0&&knob==4)||(pageIndex==2&&knob==4))return juce::String(raw-64);
        if((machine==10&&knob==1)||(machine==8&&knob==1)||(pageIndex==0&&knob==6)||(pageIndex==1&&knob>=6)||(pageIndex==2&&knob==1)||(pageIndex>=3&&knob==7))return juce::String(raw-64);
        if(pageIndex==0&&knob==1&&raw==127)return "INF";
        return juce::String(raw);
    }
    // Faceplate-only compaction: menus and raw parameter choices retain their
    // canonical P1/P2 names. The compact cell removes just that prefix and
    // draws a small side badge next to the PAGE/DEST list glyph.
    int compactLfoSide(const juce::String& text) const {
        return text.startsWithIgnoreCase("P1 ")?1:text.startsWithIgnoreCase("P2 ")?2:0;
    }
    juce::String compactDisplayedValue(int& side) const {
        const auto full=displayedValue();side=(pageIndex>=3&&(knob==0||knob==1))?compactLfoSide(full):0;
        return side==0?full:full.substring(3);
    }
    void drawLfoSideBadge(juce::Graphics& g,int cx,int cy,int side) const {
        if(side==0)return;const juce::Rectangle<int> badge{cx+18,cy-16,17,10};
        g.setColour(juce::Colours::black);g.fillRect(badge);g.setColour(ink);g.drawRect(badge,1);
        pixel::text(g,side==1?"P1":"P2",badge.reduced(1,0),8,true);
    }
    void paint(juce::Graphics& g)override{ // 1.6.25: как в прошлой версии -- всё содержимое видно, рамка только на текущей цели прицела, без мигания
        int compactSide=0;const auto compactValue=compactDisplayedValue(compactSide);
        g.setColour(parameter?ink:dim);pixel::text(g,labelOverride.isNotEmpty()?labelOverride:label,{3,3,getWidth()-6,23},21,true); // 1.6.41: переименование в LAYOUT EDIT
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
        if(isChoice&&compactSide!=0)drawLfoSideBadge(g,getWidth()/2,49,compactSide);
        pixel::text(g,compactValue,{2,getHeight()-26,getWidth()-4,23},21,true);
        if(pickMode&&hoverNow){g.setColour(ink);g.drawRect(getLocalBounds().reduced(2),3);}
        const auto now=juce::Time::getMillisecondCounter();
        if(directOutlineUntil>now&&directOutlineDuration>0){
            const float a=juce::jlimit(0.0f,1.0f,static_cast<float>(directOutlineUntil-now)/static_cast<float>(directOutlineDuration));
            g.setColour(ink.withAlpha(0.15f+0.85f*a));g.drawRect(getLocalBounds().reduced(2),3);
        }
    }
    void resized()override{slider.setBounds(getWidth()/2-26,27,52,43);} // 1.6.31: черно-белое мигание ячейки ОТКАЧЕНО
    void mouseWheelMove(const juce::MouseEvent& e,const juce::MouseWheelDetails& w)override{if(parameter&&isChoice&&std::abs(w.deltaY)>0.0001f){parameter->beginChangeGesture();const int raw=juce::roundToInt(parameter->convertFrom0to1(parameter->getValue()));int next=juce::jlimit(0,choiceCount(parameter)-1,raw+(w.deltaY>0?1:-1));if(lfoLockAxis()>=0&&!lfoChoiceAllowed(next))next=lfoFreeValue(next,w.deltaY>0?1:-1);parameter->setValueNotifyingHost(hostFor(*parameter,next));parameter->endChangeGesture();notifyLfoDestination();repaint();}else juce::Component::mouseWheelMove(e,w);}
    void mouseEnter(const juce::MouseEvent&) override { if(pickMode){setHover(true); if(pickHover) pickHover(modulationTarget());} }
    void mouseExit(const juce::MouseEvent&) override { if(pickMode) setHover(false); }

    // Press-drag is the primary way to swap lists (8 px per entry); a plain click
    // still opens the popup list as a bonus.
    void mouseDown(const juce::MouseEvent&e)override{pickClick=false;
        if(!pickMode&&pushEdit)pushEdit(); // 1.6.32: точка отката на начале любого жеста ячейки
        if(pickMode){pickClick=true; // 1.6.28: прицел -- ЛКМ только мапит; крутить -- с зажатым Alt
            if(e.mods.isAltDown()&&parameter&&!isChoice){altTurn=true;altTurnX=e.getScreenPosition().x;altTurnY=e.getScreenPosition().y;altTurnBase=parameter->getValue();parameter->beginChangeGesture();return;}
            if(pickMatrix&&parameter&&modTarget&&pickTarget)pickTarget(modulationTarget()); // 1.6.31: мигает рамка DEST в матрице (completeTargetPick)
            return; }
        // 1.6.12: ПКМ по PAGE/DEST страницы LFO -- панель замков этой ручки.
        if(pageIndex>=3&&(knob==0||knob==1)&&e.mods.isPopupMenu()){popupHandled=true;if(openLocks)openLocks(pageIndex-3,knob==1,e.getScreenPosition());return;}
        if(pageIndex==0&&(knob==0||knob==1||knob==2||knob==3)&&e.mods.isPopupMenu()){popupHandled=true;if(openEnvelope)openEnvelope(knob);return;} // 1.6.13: HOLD тоже огибающая
        if(machine==7&&knob==2&&e.mods.isPopupMenu()){popupHandled=true;if(openSamples)openSamples();return;} // 1.6.25: спец-обработчики ПЕРЕД общим фолбэком (раньше ПКМ по SAMPLES открывал список DSP)
        if(pageIndex==2&&knob==3&&e.mods.isPopupMenu()){popupHandled=true;if(openRepitch)openRepitch(e.getScreenPosition());return;} // DTIM: repitch
        if(pageIndex==2&&knob==4&&e.mods.isPopupMenu()){popupHandled=true;if(openDsnd)openDsnd(e.getScreenPosition());return;} // 1.6.13: DSND -- своё окно
        if(e.mods.isPopupMenu()&&parameter&&!isChoice){ // 1.6.24/25: ПКМ в любой точке ручки = её меню
            if(slider.curveMenu){popupHandled=true;slider.curveMenu(e.getScreenPosition());return;}
            if(slider.modeMenu){popupHandled=true;slider.modeMenu(e.getScreenPosition());return;} }
        if(!parameter)return;
        if(isChoice){dragOriginY=static_cast<int>(e.getScreenPosition().y);dragBaseIndex=juce::roundToInt(parameter->convertFrom0to1(parameter->getValue()));dragMoved=false;gestureOpen=true;parameter->beginChangeGesture();}
    }
    void mouseDrag(const juce::MouseEvent&e)override{
        if(altTurn){ // 1.6.28: Alt+драг в прицеле крутит значение (мягко, ~300 px на весь ход)
            if(!e.mods.isAltDown())return;
            const float v=juce::jlimit(0.0f,1.0f,altTurnBase+static_cast<float>((e.getScreenPosition().x-altTurnX)+(altTurnY-e.getScreenPosition().y))/300.0f);
            parameter->setValueNotifyingHost(v);repaint();return; }
        if(!parameter||!isChoice||!e.mods.isLeftButtonDown())return; // 1.6.24: листать список -- только ЛКМ
        const int dy=dragOriginY-static_cast<int>(e.getScreenPosition().y);
        if(std::abs(dy)>3)dragMoved=true;
        int value=std::clamp(dragBaseIndex+juce::roundToInt(dy/8.0*uiSpeed("KNOB LISTS")),0,choiceCount(parameter)-1); // 1.6.13: плавность из GUI DRAG SPEED
        if(lfoLockAxis()>=0&&!lfoChoiceAllowed(value)) value=lfoFreeValue(value,value>=dragBaseIndex?1:-1);
        parameter->setValueNotifyingHost(hostFor(*parameter,value));notifyLfoDestination();
    }
    void mouseUp(const juce::MouseEvent&)override{
        if(altTurn){altTurn=false;parameter->endChangeGesture();return;} // 1.6.28
        if(popupHandled){popupHandled=false;return;}
        if(pickClick){pickClick=false;return;}
        if(gestureOpen){gestureOpen=false;parameter->endChangeGesture();}
        if(!parameter||!isChoice||dragMoved){dragMoved=false;return;}
        juce::PopupMenu menu;menu.setLookAndFeel(&getLookAndFeel()); // 1.6.14: чёрный скин
        const int n=choiceCount(parameter);
        const auto names=parameter->getAllValueStrings();
        if(pageIndex>=3&&knob==0){
            // The order in the popup mirrors the requested P1/P2 hierarchy.
            // Item IDs remain raw PAGE+1, preserving automation/state values.
            const int cur=juce::jlimit(0,nova::kLfoPageChoiceCount-1,juce::roundToInt(parameter->convertFrom0to1(parameter->getValue())));
            const bool p2Source=pageIndex>=nova::kP2LfoControlPageFirst;
            auto addPage=[&](juce::PopupMenu& dst,int value){dst.addItem(value+1,nova::lfoPageName(p2Source,value),lfoChoiceAllowed(value),cur==value);};
            if(!p2Source){
                for(int v=0;v<=4;++v)addPage(menu,v); // PITCH, P1 SYNT/AMP/FILT/EFFX
                juce::PopupMenu p1Lfo;for(int v=5;v<=10;++v)addPage(p1Lfo,v);menu.addSubMenu("P1 LFO",p1Lfo);
                juce::PopupMenu p2Param;for(int v=11;v<=14;++v)addPage(p2Param,v);menu.addSubMenu("P2 PARAM",p2Param);
                juce::PopupMenu p2Lfo;for(int v=15;v<=20;++v)addPage(p2Lfo,v);menu.addSubMenu("P2 LFO",p2Lfo);
            }else{
                for(int v=1;v<=4;++v)addPage(menu,v); // P2 SYNT/AMP/FILT/EFFX
                juce::PopupMenu p2Lfo;for(int v=5;v<=10;++v)addPage(p2Lfo,v);menu.addSubMenu("P2 LFO",p2Lfo);
                juce::PopupMenu p1Param;addPage(p1Param,0);for(int v=11;v<=14;++v)addPage(p1Param,v);menu.addSubMenu("P1 PARAM",p1Param);
                juce::PopupMenu p1Lfo;for(int v=15;v<=20;++v)addPage(p1Lfo,v);menu.addSubMenu("P1 LFO",p1Lfo);
            }
        }
        else for(int i=0;i<n;++i)menu.addItem(i+1,choiceName(i),lfoChoiceAllowed(i),i==juce::roundToInt(parameter->convertFrom0to1(parameter->getValue())));
        const juce::Component::SafePointer<Cell> safe(this);
        int estW=120; for(int i=0;i<n;++i)estW=std::max(estW,static_cast<int>(choiceName(i).length())*12+28); estW=std::min(estW,700); // 1.6.44: ширина списка
        const int panelRight=72+juce::jlimit(0,2,(juce::jmax(0,getX())-75)/399)*399+387; const int lx=std::max(0,std::min(getScreenX(),panelRight-estW)); // 1.6.44: не выпирает за правую стенку панели; 1.8.1: колонка -- по геометрии ячейки (LFO 3..8/9..14 и MSEG -- правая; pageIndex%3 врал -- списки спавнились далеко от мышки)
        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(this).withTargetScreenArea(juce::Rectangle<int>(lx,getScreenY()+getHeight(),getWidth(),1)),[safe](int result){if(safe&&result>0&&safe->parameter&&safe->lfoChoiceAllowed(result-1)){if(safe->pushEdit)safe->pushEdit();safe->parameter->beginChangeGesture();safe->parameter->setValueNotifyingHost(hostFor(*safe->parameter,result-1));safe->parameter->endChangeGesture();safe->notifyLfoDestination();}}); // bound to the selected cell
    }
    int lfoLockAxis() const { return (pageIndex>=3&&(knob==0||knob==1))?(knob==0?0:1):-1; }
    bool lfoTargetBlocked(int v) const {
        const int axis=lfoLockAxis(); if(axis<0||pageIndex<3)return false;
        const int me=pageIndex-3; const juce::String mk=axis==0?"page_locks":"dest_locks"; const juce::String sk=axis==0?"page_solo":"dest_solo";
        for(int y=0;y<12;++y){if(y==me)continue;
            const juce::String pf=y<6?"lfo"+juce::String(y+1)+"_":"p2lfo"+juce::String(y-5)+"_";
            if(auto* m=processor.parameters.getParameter(pf+mk))if((((juce::roundToInt(m->convertFrom0to1(m->getValue())))>>v)&1)!=0)return true;
            if(auto* sl=processor.parameters.getParameter(pf+sk))if(juce::roundToInt(sl->convertFrom0to1(sl->getValue()))==v+1)return true;
        }
        return false;
    }
    bool lfoDestinationIsContinuous(int destination) const {
        if(pageIndex<3||knob!=1)return true;
        const auto* pageRaw=processor.parameters.getRawParameterValue(nova::pageParam(pageIndex,0));
        const int page=juce::jlimit(0,nova::kLfoPageChoiceCount-1,juce::roundToInt(pageRaw?pageRaw->load():0.0f));
        const int target=nova::lfoDirectTarget(pageIndex>=nova::kP2LfoControlPageFirst,page,juce::jlimit(0,nova::kLfoDestinationCount-1,destination));
        const int mi=processor.machineIndex();
        const int machineId=mi>=0?nova::machines()[static_cast<size_t>(mi)].id:0;
        return nova::lfoDirectTargetIsContinuousForCurrentP1Machine(machineId,target);
    }
    bool lfoChoiceAllowed(int v) const {return !lfoTargetBlocked(v)&&lfoDestinationIsContinuous(v);}
    int lfoFreeValue(int from,int dir) const {
        if(!parameter)return from;const int n=choiceCount(parameter);const int original=juce::roundToInt(parameter->convertFrom0to1(parameter->getValue()));int v=from;
        for(int i=0;i<n;++i){if(lfoChoiceAllowed(v))return v;const int next=juce::jlimit(0,n-1,v+dir);if(next==v)break;v=next;}
        return original;
    }
    void notifyLfoDestination(){
        if(!lfoDestinationChanged||pageIndex<3||(knob!=0&&knob!=1))return;
        const auto* pageRaw=processor.parameters.getRawParameterValue(nova::pageParam(pageIndex,0));
        const auto* destRaw=processor.parameters.getRawParameterValue(nova::pageParam(pageIndex,1));
        const int page=juce::jlimit(0,nova::kLfoPageChoiceCount-1,juce::roundToInt(pageRaw?pageRaw->load():0.0f));
        const int dest=juce::jlimit(0,nova::kLfoDestinationCount-1,juce::roundToInt(destRaw?destRaw->load():0.0f));
        const int target=nova::lfoDirectTarget(pageIndex>=nova::kP2LfoControlPageFirst,page,dest);
        if(target>=0)lfoDestinationChanged(static_cast<uint8_t>(target));
    }
    bool consumedPopup()const{return popupHandled;} // 1.6.43: rmbWatcher не гасит панель, открытую этой же ячейкой
private:
    uint8_t modulationTarget() const noexcept {
        if (targetOverride >= 0) return static_cast<uint8_t>(targetOverride); // 1.8.0: P2-ячейки -- цели 132..163 напрямую
        if (machine >= 0 && pageIndex < 0) return static_cast<uint8_t>(juce::jlimit(0, 7, knob));
        if (pageIndex >= 12) return static_cast<uint8_t>(212 + (pageIndex - 12) * 8 + juce::jlimit(0, 7, knob)); // 1.8.0e: P2 LFO4-6 (212..235)
        if (pageIndex >= 9)  return static_cast<uint8_t>(164 + (pageIndex - 9) * 8 + juce::jlimit(0, 7, knob)); // 1.8.0e: P2 LFO1-3 (164..187)
        if (pageIndex >= 6)  return static_cast<uint8_t>(188 + (pageIndex - 6) * 8 + juce::jlimit(0, 7, knob)); // 1.8.0e: P1 LFO4-6 (188..211)
        if (pageIndex >= 0) return static_cast<uint8_t>(juce::jlimit(0, 55, pageIndex * 8 + knob + 8)); // 1.8.0: цели 8..55 (LFO1-3 = 32..55)
        return 0;
    }
    void timerCallback()override{repaint();}
    MonomachineNovaAudioProcessor& processor;juce::String id,label;int pageIndex=-1,knob=-1,machine=-1,targetOverride=-1;bool isChoice=false,modTarget=true; // 1.8.0: +targetOverride
    int dragOriginY=0,dragBaseIndex=0;bool dragMoved=false,gestureOpen=false,popupHandled=false,pickClick=false,pickMode=false,pickMatrix=false,hoverNow=false,altTurn=false;int altTurnX=0,altTurnY=0;juce::uint32 directOutlineUntil=0,directOutlineDuration=0;float altTurnBase=0; // 1.6.28: Alt-кручение в прицеле
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
        g.fillAll(juce::Colours::black); g.setColour(ink); g.drawRect(getLocalBounds().reduced(1), 1); // 1.6.25: рамка на тех же пикселях, что у PixelButton -- GATE ровно с ARP
        pixel::text(g, juce::String(juce::roundToInt(value) + displayOffset), {4, 0, getWidth() - 8, getHeight()}, 17, true);
    }
    const char* speedKey="VALUE BOXES"; // 1.6.13: свой множитель скорости прокрутки
    std::function<void()> rightClick; // 1.6.21: ПКМ-хук (GATE на лицевой панели -> ARP)
    std::function<void()> doubleClickHandler; // specialised controls may reset a paired state.
    bool editOnRightClick=false;
    void mouseDown(const juce::MouseEvent& e) override {
        if(e.mods.isPopupMenu()){
            leftDrag=false;
            if(rightClick){rightClick();return;}
            if(editOnRightClick){showBlackValueEntry();return;}
        }
        leftDrag=!e.mods.isRightButtonDown(); dragY = e.getScreenPosition().y; dragBase = value; if (auto* par = par0()){par->beginChangeGesture();gestureStarted=true;} }
    double dragGain = 1.0; // 1.6.22: множитель чувствительности вертикального драга
    void mouseDrag(const juce::MouseEvent& e) override { if(!leftDrag)return; const int dy = dragY - e.getScreenPosition().y; setValue(dragBase + dy / 8.0 * uiSpeed(speedKey) * dragGain); }
    void mouseUp(const juce::MouseEvent&) override { if(gestureStarted){if (auto* par = par0()) par->endChangeGesture();gestureStarted=false;}leftDrag=false; }
    void mouseWheelMove(const juce::MouseEvent&, const juce::MouseWheelDetails& w) override { if (std::abs(w.deltaY) > 0.0001f) { if (auto* par = par0()) par->beginChangeGesture(); setValue(value + (w.deltaY > 0 ? 1 : -1)); if (auto* par = par0()) par->endChangeGesture(); } }
    bool editOnDoubleClick=false; // legacy controls can still request keyboard entry with double-click.
    void mouseDoubleClick(const juce::MouseEvent&) override {
        if(doubleClickHandler){doubleClickHandler();return;}
        if(editOnDoubleClick){showBlackValueEntry();return;}
        if (auto* par = par0()) par->beginChangeGesture(); setValue(def); if (auto* par = par0()) par->endChangeGesture(); }
private:
    void showBlackValueEntry() {
        auto* w=new juce::AlertWindow("ENTER VALUE",paramId,juce::MessageBoxIconType::NoIcon,nullptr);
        w->setColour(juce::AlertWindow::backgroundColourId,juce::Colours::black);
        w->setColour(juce::AlertWindow::textColourId,ink);
        w->setColour(juce::AlertWindow::outlineColourId,ink);
        w->setColour(juce::TextEditor::backgroundColourId,juce::Colours::black);
        w->setColour(juce::TextEditor::textColourId,ink);
        w->setColour(juce::TextEditor::outlineColourId,ink);
        w->setColour(juce::TextButton::buttonColourId,juce::Colours::black);
        w->setColour(juce::TextButton::buttonOnColourId,juce::Colours::black);
        w->setColour(juce::TextButton::textColourOffId,ink);
        w->setColour(juce::TextButton::textColourOnId,ink);
        w->addTextEditor("v",juce::String(juce::roundToInt(value)+displayOffset),juce::String(juce::roundToInt(lo)+displayOffset)+" .. "+juce::String(juce::roundToInt(hi)+displayOffset));
        w->addButton("OK",1);w->addButton("CANCEL",0);
        if(auto* ed=w->getTextEditor("v")){ed->setInputRestrictions(8,"-0123456789");ed->selectAll();}
        const juce::Component::SafePointer<DragValueBox> safe(this);
        w->enterModalState(true,juce::ModalCallbackFunction::create([safe,w](int r){
            if(safe&&r==1)if(auto* ed=w->getTextEditor("v")){if(auto* par=safe->par0())par->beginChangeGesture();safe->setValue(ed->getText().getDoubleValue()-safe->displayOffset);if(auto* par=safe->par0())par->endChangeGesture();}
            w->exitModalState(0);juce::MessageManager::callAsync([w]{delete w;});}),false);
        if(auto* ed=w->getTextEditor("v"))ed->grabKeyboardFocus();
    }
    juce::RangedAudioParameter* par0() const { return processor.parameters.getParameter(paramId); }
    void setValue(double v) { value = juce::jlimit(lo, hi, v); if (auto* par = par0()) par->setValueNotifyingHost(par->convertTo0to1(static_cast<float>(value))); repaint(); }
    MonomachineNovaAudioProcessor& processor; juce::String paramId; double lo = 0, hi = 1, def = 0, value = 0, dragBase = 0; int dragY = 0; bool leftDrag = true, gestureStarted = false;
};

// 1.6.41: текстовые оверрайды LAYOUT EDIT (определения ниже Cell -- нужен завершённый тип)
static bool layoutTextable(juce::Component* c){ return dynamic_cast<juce::Button*>(c)!=nullptr||dynamic_cast<juce::Label*>(c)!=nullptr||dynamic_cast<Cell*>(c)!=nullptr||dynamic_cast<TextTag*>(c)!=nullptr; }
static juce::String layoutCaptureText(juce::Component* c){ if(auto* b=dynamic_cast<juce::Button*>(c))return b->getButtonText(); if(auto* l=dynamic_cast<juce::Label*>(c))return l->getText(); if(auto* k=dynamic_cast<Cell*>(c))return k->layoutBaseText(); if(auto* tg=dynamic_cast<TextTag*>(c))return tg->tagText.isNotEmpty()?tg->tagText:tg->factoryText; return {}; }
static void applyLayoutText(juce::Component* c,const juce::String& t){ if(c==nullptr)return; if(auto* k=dynamic_cast<Cell*>(c)){k->layoutSetText(t);return;} if(auto* tg=dynamic_cast<TextTag*>(c)){tg->tagText=t; if(tg->getParentComponent()!=nullptr)tg->getParentComponent()->repaint(); return;} if(auto* b=dynamic_cast<juce::Button*>(c))b->setButtonText(t); else if(auto* l=dynamic_cast<juce::Label*>(c))l->setText(t,juce::dontSendNotification); }
static void layoutStyleApply(juce::Component* c,const UiLayoutEntry& en){ if(c==nullptr)return; if(auto* tg=dynamic_cast<TextTag*>(c)){ if(en.color!=0){tg->tagColor=juce::Colour((juce::uint32)en.color);tg->colorSet=true;} if(en.fid>=0)tg->fontId=en.fid; if(en.fscale>0)tg->fontScale=en.fscale; return; } // 1.6.43: стиль тегов
    if(auto* b=dynamic_cast<PixelButton*>(c)){ auto& props=b->getProperties(); if(props.getWithDefault("baseTH",juce::var()).isVoid())props.set("baseTH",juce::var(b->textHeight)); const float bs=(float)(int)props.getWithDefault("baseTH",juce::var(28)); if(en.fscale>0)b->textHeight=juce::jmax(8,juce::roundToInt(bs*en.fscale)); } }
// 1.6.44: база плавающих доп-окон (REPITCH/DSND/PORTA/GUI). ЛКМ за пустое место
// тащит окно; булавка справа вверху КРЕПИТ -- клик мимо закреплённое не закрывает
// (незакреплённые закрываются любым кликом мимо, как раньше).
class FloatingPanel : public juce::Component {
public:
    bool pinned=false;
    virtual juce::Rectangle<int> pinRect() const { return {getWidth()-19,4,14,14}; }
    void drawPin(juce::Graphics& g){ const auto r=pinRect(); g.setColour(ink); g.drawRect(r,1);
        const auto b=r.reduced(3);
        if(pinned){ g.fillRect(b.reduced(1)); g.setColour(juce::Colours::black); g.fillRect(b.getX()+2,b.getY()+2,2,2); }
        else { g.fillRect(b.getX()+3,b.getY(),3,3); g.fillRect(b.getX()+1,b.getY()+3,7,2); g.fillRect(b.getX()+3,b.getY()+5,3,5); } }
    void mouseDown(const juce::MouseEvent& e) override { dragOk=false;
        if(pinRect().contains(e.position.toInt())){ pinned=!pinned; if(pinned){ toFront(true); for(auto* c:getChildren())c->toFront(false); } repaint(); if(auto* pr=getParentComponent())pr->repaint(); return; } // 1.7.5 FIX: пин поднимает панель И её детей -- текст-теги больше не пропадают под слайдерами панели (setAlwaysOnTop на child в JUCE поднимал РОДИТЕЛЯ, а не панель)
        dragOk=true; dragger.startDraggingComponent(this,e); }
    void mouseDrag(const juce::MouseEvent& e) override { if(dragOk)dragger.dragComponent(this,e,nullptr); }
    void mouseUp(const juce::MouseEvent&) override { dragOk=false; }
private: juce::ComponentDragger dragger; bool dragOk=false;
};
class RepitchSliderPanel final : public FloatingPanel {
public:
    explicit RepitchSliderPanel(MonomachineNovaAudioProcessor& p) : processor(p) {
        addRow(rep, "dly_repitch", 0.0, 3.0, 0.01);
        addRow(smo, "dly_repitch_smooth", 0.0, 127.0, 1.0);
        setSize(272, 84); // 1.7.9: компактнее, текст не залезает под булавку
    }
    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::black); g.setColour(ink);
        g.drawRect(0,0,getWidth(),getHeight(),1); // 1.6.42: белая обводка доп-панели
        drawPin(g); // 1.6.44: булавка
        auto* rep=processor.parameters.getParameter("dly_repitch");
        auto* smo=processor.parameters.getParameter("dly_repitch_smooth");
        pixel::text(g,"REPITCH",{6,3,86,18},13,false);
        pixel::text(g,repitchText(rep),{126,3,136,18},13,true); // 1.7.9: булавка (справа вверху) больше не накрывает текст
        pixel::text(g,"SMOOTH",{6,41,86,18},13,false);
        pixel::text(g,smo?juce::String(juce::roundToInt(smo->convertFrom0to1(smo->getValue()))):juce::String("---"),{136,41,126,18},13,true); // 1.7.9
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
class DsndPanel final : public FloatingPanel {
public:
    explicit DsndPanel(MonomachineNovaAudioProcessor& p) : processor(p), pp(p, "dly_ppmode", 0.0, 1.0, 0.0) {
        addAndMakeVisible(pp); setSize(190, 78); // 1.7.9: компактно (по просьбе юзера) -- только PP MODE
    }
    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::black); g.setColour(ink);
        g.drawRect(0,0,getWidth(),getHeight(),1); // 1.6.42: белая обводка доп-панели
        drawPin(g); // 1.6.44: булавка
        pixel::text(g,"DSND",{6,3,120,18},13,false); // 1.7.9: компактная шапка
        pixel::text(g,"PP MODE",{6,34,86,18},13,false);
        auto* ppm=processor.parameters.getParameter("dly_ppmode");
        pixel::text(g,ppm?(juce::roundToInt(ppm->convertFrom0to1(ppm->getValue()))==1?"MID SAFE":"CLASSIC"):juce::String("---"),{92,34,90,18},13,true); // 1.7.9
    }
    void resized() override { pp.setBounds(6,52,96,22); } // 1.7.9: компактно
private:
    DragValueBox pp; // 0 = CLASSIC, 1 = MID SAFE (двойной клик = CLASSIC)
    MonomachineNovaAudioProcessor& processor;
};

// 1.9.8: right-click DBAS or DWID opens the two optional feedback-Q settings.
// The ordinary two-knob page remains unchanged; Q=0 is deliberately neutral.
class DelayFeedbackFilterPanel final : public FloatingPanel {
public:
    explicit DelayFeedbackFilterPanel(MonomachineNovaAudioProcessor& p,bool p2Page):processor(p),p2(p2Page){
        const char* ids[]{"dbas_q","dwid_q"};
        const char* names[]{"DBAS Q","DWID Q"};
        for(size_t i=0;i<sliders.size();++i){
            sliders[i].setSliderStyle(juce::Slider::LinearHorizontal);
            sliders[i].setTextBoxStyle(juce::Slider::TextBoxRight,false,48,20);
            sliders[i].setRange(0.0,127.0,1.0);
            sliders[i].setColour(juce::Slider::thumbColourId,ink);
            sliders[i].setColour(juce::Slider::trackColourId,ink.withAlpha(0.8f));
            sliders[i].setColour(juce::Slider::backgroundColourId,ink.withAlpha(0.18f));
            labels[i].setText(names[i],juce::dontSendNotification);
            addAndMakeVisible(labels[i]);addAndMakeVisible(sliders[i]);
            links[i]=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
                processor.parameters,(p2?"p2_dly_":"dly_")+juce::String(ids[i]),sliders[i]);
        }
        setSize(294,104);
    }
    void paint(juce::Graphics& g)override{g.fillAll(juce::Colours::black);g.setColour(ink);g.drawRect(getLocalBounds(),1);drawPin(g);
        pixel::text(g,(p2?"P2 ":"P1 ")+juce::String("DLY / FEEDBACK Q"),{7,3,238,17},13,false);
        pixel::text(g,"DBAS=HP  DWID=LP / width",{7,79,250,15},10,false);
    }
    void resized()override{for(size_t i=0;i<sliders.size();++i){const int y=24+static_cast<int>(i)*26;labels[i].setBounds(8,y,61,20);sliders[i].setBounds(70,y,214,20);}}
private:
    MonomachineNovaAudioProcessor& processor;bool p2=false;
    std::array<juce::Slider,2> sliders;std::array<juce::Label,2> labels;
    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>,2> links;
};

// 1.6.14: окно портаменто (ПКМ по ручке PORT страницы AMP): TIME = сама ручка,
// SPEED = множитель скорости глиссандо (porta_speed). Без задержек.
class PortaPanel final : public FloatingPanel {
public:
    explicit PortaPanel(MonomachineNovaAudioProcessor& p) : processor(p) {
        addRow(time, "p0_7", 0.0, 127.0, 1.0);
        addRow(speed, "porta_speed", 0.0, 127.0, 1.0);
        setSize(320, 88);
    }
    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::black); g.setColour(ink);
        g.drawRect(0,0,getWidth(),getHeight(),1); // 1.6.42: белая обводка доп-панели
        drawPin(g); // 1.6.44: булавка
        pixel::text(g,"PORTAMENTO",{6,3,160,18},13,false);
        auto* tm=processor.parameters.getParameter("p0_7");
        auto* sp=processor.parameters.getParameter("porta_speed");
        pixel::text(g,"TIME",{6,41,64,18},13,false);
        pixel::text(g,tm?juce::String(juce::roundToInt(tm->convertFrom0to1(tm->getValue()))):juce::String("---"),{74,41,56,18},13,true); // 1.7.9: TIME и SPEED не залезают под булавку
        pixel::text(g,"SPEED",{140,41,58,18},13,false); // 1.7.9
        pixel::text(g,sp?juce::String(juce::roundToInt(sp->convertFrom0to1(sp->getValue()))):juce::String("---"),{200,41,60,18},13,true); // 1.7.9
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
    bool smallPopup=false; // 1.6.32: компактный список (только MODE-кнопки)
    // A selector may substitute a compact grouped popup while retaining normal
    // ComboBox keyboard/wheel/drag semantics and its stable item indices.
    std::function<void(DragChoice&)> popupHandler;
    void mouseDown(const juce::MouseEvent& e)override{origin=e.getPosition();initial=getSelectedItemIndex();dragged=false;}
    void mouseDrag(const juce::MouseEvent& e)override{int d=(e.x-origin.x)+(origin.y-e.y);if(std::abs(d)>4){dragged=true;setSelectedItemIndex(juce::jlimit(0,std::max(0,getNumItems()-1),initial+juce::roundToInt(d/6.0*uiSpeed(speedKey))),juce::sendNotificationSync);}}
    void mouseUp(const juce::MouseEvent&)override{if(!dragged){if(popupHandler)popupHandler(*this);else showComboBelow(*this,smallPopup);}}
    double wheelAccum=0; // 1.7.1: накопление дельты -- скролл списков (SHAPE/GRID пресеты MSEG) больше не резкий
    void mouseWheelMove(const juce::MouseEvent&,const juce::MouseWheelDetails& w)override{if(getNumItems()<2)return;wheelAccum+=w.deltaY*(w.isReversed?-1.0:1.0);const double step=0.12;while(wheelAccum>=step){wheelAccum-=step;setSelectedItemIndex(juce::jlimit(0,getNumItems()-1,getSelectedItemIndex()-1),juce::sendNotificationSync);}while(wheelAccum<=-step){wheelAccum+=step;setSelectedItemIndex(juce::jlimit(0,getNumItems()-1,getSelectedItemIndex()+1),juce::sendNotificationSync);}} // 1.7.1: шаг списка ~за нотч, тейлпад не улетает
private:juce::Point<int> origin;int initial=0;bool dragged=false;
};
// 1.6.12: ПКМ по PAGE/DEST страницы LFO открывает эту панель замков. Строка:
// замок (ЛКМ = исключить значение из модуляции, ПКМ = SOLO -- приколотить
// модуляцию ТОЛЬКО к этому значению) + имя значения (ЛКМ = выставить ручку).
// SOLO переживает ручные изменения: поменял руками -- SOLO следует за новым.
// RMB panel for the deliberately opt-in physical filter controls.  These controls
// are independent per P1/P2 and their parameter defaults are exact zeros.
class FilterExtraPanel final : public FloatingPanel {
public:
    explicit FilterExtraPanel(MonomachineNovaAudioProcessor& p,bool p2Page):processor(p),p2(p2Page){
        const char* suffix[]{"vel_l","vel_h","kt_l","kt_h","sat"};
        const char* names[]{"VEL-L","VEL-H","KT-L","KT-H","SAT"};
        for(size_t i=0;i<sliders.size();++i){
            sliders[i].setSliderStyle(juce::Slider::LinearHorizontal);
            sliders[i].setTextBoxStyle(juce::Slider::TextBoxRight,false,54,20);
            sliders[i].setColour(juce::Slider::thumbColourId,ink);
            sliders[i].setColour(juce::Slider::trackColourId,ink.withAlpha(0.8f));
            sliders[i].setColour(juce::Slider::backgroundColourId,ink.withAlpha(0.18f));
            labels[i].setText(names[i],juce::dontSendNotification);
            addAndMakeVisible(labels[i]);addAndMakeVisible(sliders[i]);
            links[i]=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.parameters,(p2?"p2_filt_":"filt_")+juce::String(suffix[i]),sliders[i]);
        }
        setSize(332,176);
    }
    void paint(juce::Graphics& g)override{g.fillAll(juce::Colours::black);g.setColour(ink);g.drawRect(getLocalBounds(),1);drawPin(g);
        pixel::text(g,(p2?"P2 ":"P1 ")+juce::String("FILT / EXTRA"),{7,3,250,17},14,false);
        pixel::text(g,"0 = native / no change",{7,getHeight()-18,242,15},11,false);
    }
    void resized()override{for(size_t i=0;i<sliders.size();++i){const int y=24+(int)i*25;labels[i].setBounds(8,y,54,20);sliders[i].setBounds(65,y,255,20);}}
private:
    MonomachineNovaAudioProcessor& processor;bool p2=false;
    std::array<juce::Slider,5> sliders;std::array<juce::Label,5> labels;
    std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>,5> links;
};

class LfoLockPanel final : public juce::Component, private juce::Timer {
public:
    LfoLockPanel(MonomachineNovaAudioProcessor& p,int lfoIndex,bool):processor(p),lfo(lfoIndex){ // 1.6.30: ЕДИНОЕ окно -- секции PAGE и DEST вместе
        refreshNames();
        setSize(705,44+11*30+46); startTimerHz(10); // 1.6.33: строки 30px; 1.8.1: ТРИ колонки -- PAGE 0..10 | PAGE 11..20 | DEST (все 21 страница видна для лока)
    }
    ~LfoLockPanel() override { stopTimer(); }
    std::function<void()> onClose;
    void refreshNames(){
        const int page=3+lfo;
        const bool p2Source=lfo>=6;
        for(int k=0;k<nova::kLfoPageChoiceCount;++k)pageNames[static_cast<size_t>(k)]=nova::lfoPageName(p2Source,k);
        auto* pg=processor.parameters.getParameter(nova::pageParam(page,0));
        const int cur=juce::jlimit(0,nova::kLfoPageChoiceCount-1,juce::roundToInt(pg?pg->convertFrom0to1(pg->getValue()):0.0f));
        const int mi=processor.machineIndex();const int machineId=mi>=0?nova::machines()[static_cast<size_t>(mi)].id:0;
        for(int k=0;k<nova::kLfoDestinationCount;++k){
            const int target=nova::lfoDirectTarget(p2Source,cur,k);
            if(target==nova::kPitchMatrixTarget){const char* rng[]{"1ST","2ST","3ST","5ST","7ST","1OCT","2OCT","3OCT"};rowNames[static_cast<size_t>(k)]=rng[k];}
            else if(!nova::lfoDirectTargetIsContinuousForCurrentP1Machine(machineId,target))rowNames[static_cast<size_t>(k)]="-- unavailable --";
            else rowNames[static_cast<size_t>(k)]=targetName(processor,target);
        }
    }
    void timerCallback() override { repaint(); }
    static int rowsTop(){return 44;} static int rowH(){return 30;} static int colX(int s){return s==0?6:s==1?241:476;} // 1.6.33: сетка; 1.8.1: PAGE 0..10 | PAGE 11..20 | DEST
    void paint(juce::Graphics& g)override{
        refreshNames(); // живые имена каждый кадр
        g.fillAll(juce::Colours::black);g.setColour(ink);
        pixel::text(g,juce::String("LFO")+juce::String(lfo+1)+" LOCKS",{6,4,208,18},13,false);
        pixel::text(g,"PAGE 0..10",{46,rowsTop()-16,140,14},11,false);pixel::text(g,"PAGE 11..20",{colX(1)+16,rowsTop()-16,140,14},11,false);pixel::text(g,"DEST",{colX(2)+16,rowsTop()-16,120,14},11,false); // 1.8.1: три колонки
        for(int s=0;s<3;++s){const bool pg=(s<2);const int mask=maskValue(pg),solo=soloValue(pg);const int knob=knobValue(pg);const int bx=colX(s);const int nRows=s<2?(s==0?11:10):8;const int base=s==1?11:0;
            for(int k=0;k<nRows;++k){const int y=rowsTop()+k*rowH();const int val=base+k;
                const bool locked=((mask>>val)&1)!=0;const bool soloed=(solo-1)==val;
                drawLockMark(g,bx+6,y+6,soloed?2:(locked?1:0));
                g.setColour(ink.withAlpha(0.25f));g.drawHorizontalLine(y+rowH()-1,bx+4.0f,bx+223.0f);
                g.setColour(ink);
                pixel::text(g,pg?pageNames[static_cast<size_t>(val)]:rowNames[static_cast<size_t>(k)],{bx+42,y+3,176,22},14,false);
                if(pg?knob==val:knob==k)g.drawRect(bx+2,y,222,rowH(),1);}}
        g.setColour(ink);pixel::text(g,"CLEAR LOCKS",{6,getHeight()-30,120,20},12,false);
        pixel::text(g,"LMB LOCK (drag to paint) / RMB SOLO / click value = set",{6,getHeight()-16,690,14},9,false);
    }
    void mouseDown(const juce::MouseEvent& e)override{
        if(e.y>=getHeight()-30&&e.y<getHeight()-8){auto set=[this](const juce::String& parId,float v){if(auto* p=processor.parameters.getParameter(parId)){p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(v));p->endChangeGesture();}};
            set(id("page_locks"),0.0f);set(id("dest_locks"),0.0f);set(id("page_solo"),0.0f);set(id("dest_solo"),0.0f);repaint();return;}
        const int hit=rowAt(e.x,e.y);if(hit<0)return;const bool pg=(e.x<470);const int row=hit; // 1.6.33: колонка = PAGE слева / DEST справа; 1.8.1: row = ЗНАЧЕНИЕ (0..20 у PAGE)
        if(e.x<colX(e.x<235?0:e.x<470?1:2)+34){const bool soloHere=(soloValue(pg)-1)==row;
            if(e.mods.isPopupMenu()){setSolo(pg,soloHere?0:row+1);return;} // ПКМ по замку = SOLO
            const int mask=maskValue(pg);const bool was=((mask>>row)&1)!=0;
            paintValue=was?0:1;lockPainting=true;paintSection=e.x<235?0:e.x<470?1:2;
            setMask(pg,was?mask&~(1<<row):mask|(1<<row));return;}
        if(auto* par=processor.parameters.getParameter(nova::pageParam(3+lfo,pg?0:1))){
            if(!pg&&!destinationAvailable(row))return;
            if(otherLocksBlock(pg,row))return; // under another LFO's lock/solo
            par->beginChangeGesture();par->setValueNotifyingHost(par->convertTo0to1(static_cast<float>(row)));par->endChangeGesture();
            if(soloValue(pg)>0)setSolo(pg,row+1); // SOLO следует за ручной сменой
        }
        if(onClose)onClose();
        if(auto* owner=findParentComponentOfClass<juce::CallOutBox>())owner->exitModalState(0);
    }
    void mouseDrag(const juce::MouseEvent& e)override{
        if(!lockPainting)return;
        const int hit=rowAt(e.x,e.y);if(hit<0)return;const int row=hit;const bool pg=(e.x<470);const int sec=e.x<235?0:e.x<470?1:2; // 1.6.33: красим ТА СЕКЦИЯ, где курсор; 1.8.1: три секции
        if(sec!=paintSection)return; // красим в той колонке, где начали
        int mask=maskValue(pg);const bool was=((mask>>row)&1)!=0;
        if((paintValue==1)==was){mask=paintValue==1?mask|(1<<row):mask&~(1<<row);setMask(pg,mask);}
    }
    void mouseUp(const juce::MouseEvent&)override{lockPainting=false;}
private:
    bool destinationAvailable(int row)const{auto* pg=processor.parameters.getParameter(nova::pageParam(3+lfo,0));const int page=juce::jlimit(0,nova::kLfoPageChoiceCount-1,juce::roundToInt(pg?pg->convertFrom0to1(pg->getValue()):0.0f));const int mi=processor.machineIndex();const int machineId=mi>=0?nova::machines()[static_cast<size_t>(mi)].id:0;return nova::lfoDirectTargetIsContinuousForCurrentP1Machine(machineId,nova::lfoDirectTarget(lfo>=6,page,row));}
    int rowAt(int x,int y)const{if(y<rowsTop()||y>=rowsTop()+11*rowH())return -1;const int lr=(y-rowsTop())/rowH();if(x<6)return -1;if(x<235)return lr; // 1.8.1: PAGE 0..10
        if(x<470)return lr<10?11+lr:-1; // PAGE 11..20
        if(x<700)return lr<8?lr:-1;return -1;} // DEST 0..7
    juce::String id(const char* suffix)const{return (lfo<6?juce::String("lfo")+juce::String(lfo+1):juce::String("p2lfo")+juce::String(lfo-5))+"_"+juce::String(suffix);} // 1.8.0e: P1 = lfo1..6, P2 = p2lfo1..6
    int rawOf(const char* suffix)const{auto* p=processor.parameters.getParameter(id(suffix));return p?juce::roundToInt(p->convertFrom0to1(p->getValue())):0;}
    int maskValue(bool pg)const{return rawOf(pg?"page_locks":"dest_locks");}
    int soloValue(bool pg)const{return rawOf(pg?"page_solo":"dest_solo");}
    int knobValue(bool pg)const{auto* p=processor.parameters.getParameter(nova::pageParam(3+lfo,pg?0:1));return p?juce::jlimit(0,pg?20:7,juce::roundToInt(p->convertFrom0to1(p->getValue()))):0;} // 1.8.1: PAGE 0..20
    bool otherLocksBlock(bool pg,int row)const{const juce::String mk=pg?"page_locks":"dest_locks";const juce::String sk=pg?"page_solo":"dest_solo";for(int y=0;y<12;++y){if(y==lfo)continue; // 1.8.1 FIX: чужие замки среди ВСЕХ 12 LFO (было 3, только P1)
            const juce::String pf=y<6?"lfo"+juce::String(y+1)+"_":"p2lfo"+juce::String(y-5)+"_";
            if(auto* m=processor.parameters.getParameter(pf+mk))if((((juce::roundToInt(m->convertFrom0to1(m->getValue())))>>row)&1)!=0)return true;
            if(auto* s=processor.parameters.getParameter(pf+sk))if(juce::roundToInt(s->convertFrom0to1(s->getValue()))==row+1)return true;}return false;} // 1.6.30: enforcement чужих замков
    void setMask(bool pg,int m){if(auto* p=processor.parameters.getParameter(id(pg?"page_locks":"dest_locks"))){p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(juce::jlimit(0,pg?2097151:255,m))));p->endChangeGesture();}repaint();} // 1.8.1: маска PAGE 21 бит
    void setSolo(bool pg,int s){if(auto* p=processor.parameters.getParameter(id(pg?"page_solo":"dest_solo"))){p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(juce::jlimit(0,pg?21:8,s))));p->endChangeGesture();}repaint();} // 1.8.1: solo PAGE 0..21
    static void drawLockMark(juce::Graphics& g,int x,int y,int mode){
        const float fx=float(x),fy=float(y);g.setColour(ink);
        g.drawRect(fx+1.0f,fy+4.0f,14.0f,12.0f,1.0f);
        if(mode>0)g.fillRect(fx+5.0f,fy+8.0f,5.0f,4.0f);
        if(mode==2)g.drawVerticalLine(x+8,fy-1.0f,fy+4.0f);
        else{g.drawLine(fx+4.0f,fy+4.0f,fx+4.0f,fy+0.0f,1.0f);g.drawLine(fx+12.0f,fy+4.0f,fx+12.0f,fy+0.0f,1.0f);g.drawHorizontalLine(fy,fx+4.0f,fx+12.0f);}
    }
    MonomachineNovaAudioProcessor& processor;int lfo=0;
    std::array<juce::String,8> rowNames{};std::array<juce::String,21> pageNames{};bool lockPainting=false;int paintValue=0;int paintSection=0; // 1.8.1: 21 имя PAGE, секция 0..2
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

// 1.6.19: отдельное окно ручек плавности прокрутки (MENU > GUI OPTIONS).
class GuiSpeedPanel final : public FloatingPanel {
public:
    GuiSpeedPanel(){
        for(const auto& def:kSpeedDefs){
            auto label=std::make_unique<PixelLabel>();label->setText(def.label,juce::dontSendNotification);
            label->setBounds(0,0,10,14);label->setJustificationType(juce::Justification::centredLeft);addAndMakeVisible(*label);
            auto box=std::make_unique<FloatBox>(def.key,def.def);addAndMakeVisible(*box);
            labels.push_back(std::move(label));boxes.push_back(std::move(box));}
        reset.setButtonText("RESET");reset.textHeight=10;addAndMakeVisible(reset);
        reset.setTooltip("Restore all GUI drag speeds to their defaults.");
        reset.onClick=[this]{for(size_t i=0;i<boxes.size()&&i<sizeof(kSpeedDefs)/sizeof(kSpeedDefs[0]);++i)boxes[i]->refreshTo(kSpeedDefs[i].def);};
        setSize(1140,140);
    }
    void paint(juce::Graphics& g) override { g.fillAll(juce::Colours::black);g.setColour(ink);
        g.drawRect(0,0,getWidth(),getHeight(),1); // 1.6.42: белая обводка доп-панели
        drawPin(g); // 1.6.44: булавка
        pixel::text(g,"GUI DRAG SPEED (per list; drag/wheel, double-click = default)",{10,6,800,18},14,true); }
    void resized() override { reset.setBounds(985,6,120,24); // 1.6.44: правый угол -- булавка // 1.6.33: MAP FLASH удалена -- мигает только РАМКА DEST в матрице
        for(size_t i=0;i<boxes.size();++i){const int x=10+static_cast<int>(i%4)*280,y=40+static_cast<int>(i/4)*50;labels[static_cast<size_t>(i)]->setBounds(x,y,130,16);boxes[static_cast<size_t>(i)]->setBounds(x,y+18,120,26);}}
private:
    PixelButton reset{"RESET"};std::vector<std::unique_ptr<PixelLabel>> labels;std::vector<std::unique_ptr<FloatBox>> boxes;
};

// Global/tempo/arp list kept for the MENU page, restyled for the black screen.
class SettingsPage final : public juce::Component {
public:
    SettingsPage(MonomachineNovaAudioProcessor& p,bool matrix):processor(p){
        if(matrix)return; // the matrix has its own page below
        std::vector<juce::String> ids={"arp_on","arp_hold","host_sync","arp_range","arp_sync","arp_play","arp_grid","arp_length","arp_wrap","arp_velocity_mode","arp_step_start","arp_step_end","arp_step","arp_step_velocity","arp_step_transpose","arp_step_hold","arp_time","bpm","mseg_sync","mseg_loop","mseg_rate","dly_repitch","macro_x","macro_y"};
        if(!p.isSynthVersion)ids.push_back("gate"); // 1.8.0: fx_mix убран (глобальный микс не нужен -- вставка)
        // 1.8.0: P2 MIX переехал на AMP-страницу (слот PORT); P2 LEV убран -- LEV-фейдер P1 мастерит весь плагин
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
    StepLane(MonomachineNovaAudioProcessor& p,std::function<int()> startOf,std::function<int()> endOf,
             std::function<void(int,const char*,float)> set)
        : processor(p),windowStart(std::move(startOf)),windowEnd(std::move(endOf)),setParam(std::move(set)) {}
    struct Geo {int count=1,w=8,gap=1,x0=36,yTr=6,h=40,yVel=50,yHold=94,holdH=18,yPlay=116,playH=9;};
    Geo geo() const {
        Geo g;const int start=juce::jlimit(0,63,windowStart()),end=juce::jlimit(start,63,windowEnd());g.count=end-start+1;
        g.gap=g.count>32?1:g.count>16?2:6;const int avail=getWidth()-42-(g.count-1)*g.gap;g.w=juce::jlimit(7,220,std::max(7,avail/g.count));
        g.x0=36+std::max(0,(getWidth()-42-(g.w*g.count+(g.count-1)*g.gap))/2);
        const int usable=std::max(60,getHeight()-18-18-14);g.h=juce::jlimit(28,92,(usable-18-10)/2);
        g.yTr=6;g.yVel=g.yTr+g.h+5;g.yHold=g.yVel+g.h+5;g.holdH=18;g.yPlay=g.yHold+g.holdH+4;g.playH=9;return g;
    }
    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::black);const int start=juce::jlimit(0,63,windowStart()),end=juce::jlimit(start,63,windowEnd());const Geo q=geo();
        g.setColour(ink.withAlpha(0.45f));pixel::text(g,"TR",{2,q.yTr+q.h/2-7,30,14},12,true);pixel::text(g,"VEL",{2,q.yVel+q.h/2-7,30,14},12,true);pixel::text(g,"HOLD",{2,q.yHold+1,32,14},10,true);
        for(int col=0,absolute=start;absolute<=end;++absolute,++col){const int pg=absolute/8,st=absolute%8;const auto pre="arp_s"+juce::String(pg)+"_"+juce::String(st)+"_";const auto* trRaw=processor.parameters.getRawParameterValue(pre+"transpose");const auto* veRaw=processor.parameters.getRawParameterValue(pre+"velocity");const auto* hoRaw=processor.parameters.getRawParameterValue(pre+"hold");
            const float tr=trRaw?trRaw->load():0.0f,ve=veRaw?veRaw->load():127.0f,ho=hoRaw?hoRaw->load():0.0f;const int x=q.x0+col*(q.w+q.gap);drawPad(g,x,q.yTr,q.w,q.h,tr,true,q.w>=19);drawPad(g,x,q.yVel,q.w,q.h,ve,false,q.w>=19);
            g.setColour(ink.withAlpha(0.5f));g.drawRect(x,q.yHold,q.w,q.holdH,1);if(ho>0.5f){g.setColour(ink);g.fillRect(x+1,q.yHold+1,std::max(1,q.w-2),q.holdH-2);}
            g.setColour(ink.withAlpha(0.5f));g.drawRect(x,q.yPlay,q.w,q.playH,1);if(processor.arpStepEcho.load(std::memory_order_relaxed)==absolute){g.setColour(ink);g.fillRect(x+1,q.yPlay+1,std::max(1,q.w-2),std::max(1,q.playH-2));}
            if(q.w>=12)pixel::text(g,juce::String(absolute+1),{x,q.yPlay+q.playH+1,q.w,12},10,true);
        }
        if(q.count>16){g.setColour(ink.withAlpha(0.55f));pixel::text(g,juce::String("ACTIVE ")+juce::String(start+1)+".."+juce::String(end+1)+"  ("+juce::String(q.count)+" STEPS)",{38,getHeight()-15,getWidth()-42,13},10,false);}
    }
    void mouseDown(const juce::MouseEvent& e) override {const Geo q=geo();if(e.mods.isPopupMenu()){erasing=true;eraseRow=rowUnder(q,e.y);applyErase(e,q);return;}holdPainting=e.y>=q.yHold&&e.y<q.yHold+q.holdH;if(holdPainting){applyHold(e,q,true);return;}if(!insidePads(q,e.x,e.y))return;painting=true;paintingTranspose=e.y<q.yVel;paintAt(e,q);}
    void mouseDrag(const juce::MouseEvent& e) override {const Geo q=geo();if(erasing){applyErase(e,q);return;}if(holdPainting){applyHold(e,q,false);return;}if(painting)paintAt(e,q);}
    void mouseUp(const juce::MouseEvent&) override {painting=false;holdPainting=false;erasing=false;}
    void mouseMove(const juce::MouseEvent& e) override {const Geo q=geo();if(e.y>=q.yTr&&e.y<q.yVel)setTooltip("TRANSPOSE: draw -24..+24. RMB clears.");else if(e.y>=q.yVel&&e.y<=q.yVel+q.h)setTooltip("VELOCITY: draw 0..127. RMB clears.");else if(e.y>=q.yHold&&e.y<q.yHold+q.holdH)setTooltip("HOLD: LMB toggles/draws, RMB clears.");else setTooltip({});}
private:
    void drawPad(juce::Graphics& g,int x,int y,int w,int h,float value,bool bipolar,bool showValue) const {g.setColour(ink.withAlpha(0.5f));g.drawRect(x,y,w,h,1);const int inner=juce::jlimit(1,7,std::min(w/3,h/6));if(bipolar){const int mid=y+h/2;const int d=juce::roundToInt(juce::jlimit(-24.0f,24.0f,value)/24.0f*(h/2-inner));g.setColour(ink.withAlpha(0.3f));g.drawHorizontalLine(mid,float(x+1),float(x+w-1));g.setColour(ink);if(d>=0)g.fillRect(x+inner,mid-d,std::max(1,w-inner*2),d+1);else g.fillRect(x+inner,mid,std::max(1,w-inner*2),-d+1);}else{const int bh=juce::roundToInt(juce::jlimit(0.0f,127.0f,value)/127.0f*(h-inner*2));g.setColour(ink);g.fillRect(x+inner,y+h-inner-bh,std::max(1,w-inner*2),bh);}if(showValue){g.setColour(juce::Colours::black);g.fillRect(x+1,y+1,w-2,13);g.setColour(ink);pixel::text(g,juce::String(juce::roundToInt(value)),{x+1,y+1,w-2,12},10,true);}}
    int columnFor(const Geo& q,int x) const {const int start=juce::jlimit(0,63,windowStart()),end=juce::jlimit(start,63,windowEnd());return juce::jlimit(0,end-start,(x-q.x0)/(q.w+q.gap));}
    int absoluteFor(const Geo& q,int x) const {return juce::jlimit(0,63,juce::jlimit(0,63,windowStart())+columnFor(q,x));}
    bool insidePads(const Geo& q,int x,int y) const{return x>=q.x0&&y>=q.yTr&&y<=q.yVel+q.h;}
    int rowUnder(const Geo& q,int y) const{return y>=q.yTr&&y<q.yVel?0:y>=q.yVel&&y<=q.yVel+q.h?1:y>=q.yHold&&y<q.yHold+q.holdH?2:-1;}
    void paintAt(const juce::MouseEvent& e,const Geo& q){if(e.x<q.x0)return;const int absolute=absoluteFor(q,e.x);const int y=juce::jlimit(q.yTr,q.yVel+q.h,int(e.y));if(paintingTranspose){const float rel=juce::jlimit(0.0f,1.0f,float(y-q.yTr)/float(q.h));setParam(absolute,"transpose",std::round((0.5f-rel)*48.0f));}else{const float rel=juce::jlimit(0.0f,1.0f,float(q.yVel+q.h-y)/float(q.h));setParam(absolute,"velocity",std::round(rel*127.0f));}repaint();}
    void applyHold(const juce::MouseEvent& e,const Geo& q,bool click){if(e.x<q.x0)return;const bool on=click||int(e.y)<q.yHold+q.holdH/2;setParam(absoluteFor(q,e.x),"hold",on?1.0f:0.0f);repaint();}
    void applyErase(const juce::MouseEvent& e,const Geo& q){if(e.x<q.x0)return;const int row=eraseRow>=0?eraseRow:rowUnder(q,e.y);if(row==0)setParam(absoluteFor(q,e.x),"transpose",0.0f);else if(row==1)setParam(absoluteFor(q,e.x),"velocity",0.0f);else if(row==2)setParam(absoluteFor(q,e.x),"hold",0.0f);repaint();}
    bool painting=false,paintingTranspose=false,holdPainting=false,erasing=false;int eraseRow=-1;MonomachineNovaAudioProcessor& processor;std::function<int()> windowStart,windowEnd;std::function<void(int,const char*,float)> setParam;
};

// 1.7.1: P-LOCK -- линия значений выбранного слота (рисуется как velocity/hold), SLIDE -- флажки под ячейками.
class PlockLane final : public juce::Component, public juce::SettableTooltipClient {
public:
    PlockLane(MonomachineNovaAudioProcessor& p,std::function<int()> startOf,std::function<int()> endOf,std::function<int()> slotOf,std::function<juce::Rectangle<int>()> trackOf)
        : processor(p),windowStart(std::move(startOf)),windowEnd(std::move(endOf)),slotOf(std::move(slotOf)),alignmentTrack(std::move(trackOf)) {}
    std::function<void()> pushEdit;
    // P-LOCK values use the same horizontal step canvas as the normal lane.
    // This keeps widths and columns aligned even when Layout Edit moves the
    // P-LOCK component (including the user's existing negative-x layout).
    struct Geo {int count=1,w=8,gap=1,x0=36,yVal=7,h=56,ySlide=68,slideH=19;};
    Geo geo() const {
        Geo q;const int start=juce::jlimit(0,63,windowStart()),end=juce::jlimit(start,63,windowEnd());q.count=end-start+1;
        q.gap=q.count>32?1:q.count>16?2:6;
        const auto track=alignmentTrack?alignmentTrack():juce::Rectangle<int>{getX()+36,0,std::max(1,getWidth()-42),1};
        const int left=track.getX(),right=track.getRight();q.x0=left-getX();
        const int avail=std::max(q.count*7,right-left-(q.count-1)*q.gap);
        q.w=juce::jlimit(7,220,std::max(7,avail/q.count));return q;
    }
    void paint(juce::Graphics& g) override {g.fillAll(juce::Colours::black);const int start=juce::jlimit(0,63,windowStart()),end=juce::jlimit(start,63,windowEnd()),sl=juce::jlimit(0,processor.kPlockSlots-1,slotOf());const Geo q=geo();g.setColour(ink.withAlpha(0.45f));pixel::text(g,"S"+juce::String(sl+1),{2,q.yVal+q.h/2-7,30,14},12,true);
        for(int col=0,absolute=start;absolute<=end;++absolute,++col){const int pg=absolute/8,st=absolute%8;const float value=processor.plockValue(sl,pg,st);const bool slide=processor.plockSlideValue(sl,pg,st)>0.5f;const int x=q.x0+col*(q.w+q.gap);g.setColour(ink.withAlpha(0.5f));g.drawRect(x,q.yVal,q.w,q.h,1);g.setColour(ink.withAlpha(0.3f));g.drawHorizontalLine(q.yVal+q.h/2,float(x+1),float(x+q.w-1));if(value>0.5f){const int bh=juce::roundToInt(juce::jlimit(0.0f,127.0f,value)/127.0f*(q.h-4));g.setColour(ink);g.fillRect(x+1,q.yVal+q.h-2-bh,std::max(1,q.w-2),bh);if(q.w>=19){g.setColour(juce::Colours::black);g.fillRect(x+1,q.yVal+1,q.w-2,12);g.setColour(ink);pixel::text(g,juce::String(juce::roundToInt(value)),{x+1,q.yVal+1,q.w-2,11},9,true);}}
            g.setColour(ink.withAlpha(0.5f));g.drawRect(x,q.ySlide,q.w,q.slideH,1);if(slide&&q.w>=8){g.setColour(ink);pixel::text(g,"S",{x,q.ySlide+1,q.w,q.slideH-2},10,true);}if(processor.plockStepEcho.load(std::memory_order_relaxed)==absolute){g.setColour(ink);g.drawRect(x-1,q.yVal-1,q.w+2,q.h+2,1);}}
    }
    void mouseDown(const juce::MouseEvent& e) override {if(pushEdit)pushEdit();const Geo q=geo();if(e.mods.isPopupMenu()){erasing=true;erSlide=e.y>=q.ySlide;applyErase(e,q);return;}if(e.y>=q.ySlide&&e.y<q.ySlide+q.slideH){paintAt(e,q,true);return;}painting=true;paintAt(e,q,false);}
    void mouseDrag(const juce::MouseEvent& e) override {if(erasing)applyErase(e,geo());else if(painting)paintAt(e,geo(),false);}
    void mouseUp(const juce::MouseEvent&) override {painting=false;erasing=false;}
    void mouseMove(const juce::MouseEvent& e) override {const Geo q=geo();setTooltip(e.y>=q.ySlide?"SLIDE: click a step to glide to the next lock; RMB clears.":"P-LOCK values: draw 1..127; RMB clears.");}
private:
    int absoluteFor(const Geo& q,int x) const {const int start=juce::jlimit(0,63,windowStart()),end=juce::jlimit(start,63,windowEnd());return juce::jlimit(start,end,start+juce::jlimit(0,end-start,(x-q.x0)/(q.w+q.gap)));}
    void paintAt(const juce::MouseEvent& e,const Geo& q,bool slideClick){if(e.x<q.x0)return;const int absolute=absoluteFor(q,e.x),pg=absolute/8,st=absolute%8,sl=juce::jlimit(0,processor.kPlockSlots-1,slotOf());if(slideClick)processor.plockSetSlide(sl,pg,st,processor.plockSlideValue(sl,pg,st)<0.5f);else{const int y=juce::jlimit(q.yVal,q.yVal+q.h,int(e.y));const float rel=juce::jlimit(0.0f,1.0f,float(q.yVal+q.h-y)/float(q.h));processor.plockSetValue(sl,pg,st,std::round(rel*127.0f));}repaint();}
    void applyErase(const juce::MouseEvent& e,const Geo& q){if(e.x<q.x0)return;const int absolute=absoluteFor(q,e.x),pg=absolute/8,st=absolute%8,sl=juce::jlimit(0,processor.kPlockSlots-1,slotOf());if(erSlide)processor.plockSetSlide(sl,pg,st,false);else processor.plockSetValue(sl,pg,st,0.0f);repaint();}
    bool painting=false,erasing=false,erSlide=false;MonomachineNovaAudioProcessor& processor;std::function<int()> windowStart,windowEnd,slotOf;std::function<juce::Rectangle<int>()> alignmentTrack;
};

// 1.7.1: мини-крутилка дестинейшена P-LOCK (цели те же, что в матрице DEST): драг = листать цели, клик = список, ПКМ = очистить слот.
class PlockDestBox final : public juce::Component, public juce::SettableTooltipClient {
public:
    PlockDestBox(MonomachineNovaAudioProcessor& p, std::function<int()> slotOf, std::function<void()> onList)
        : processor(p), slotOf(std::move(slotOf)), listRequest(std::move(onList)) {}
    std::function<void()> pushEdit; // 1.7.3
    int curSlot() const { return juce::jlimit(0, processor.kPlockSlots - 1, slotOf()); }
    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::black); g.setColour(ink); g.drawRect(getLocalBounds().reduced(1), 1);
        const int t = processor.plockTarget(curSlot());
        pixel::text(g, t < 0 ? "-- no target --" : targetName(processor, t), {4, 0, getWidth() - 8, getHeight()}, 14, false); }
    void mouseDown(const juce::MouseEvent& e) override { if (pushEdit) pushEdit(); // 1.7.3: точка отката перед сменой цели
        dragY = e.getScreenPosition().y; lastApplied = 0; moved = false;
        if (e.mods.isPopupMenu()) { processor.plockSetTarget(curSlot(), -1); repaint(); } }
    void mouseDrag(const juce::MouseEvent& e) override {
        const int dy = dragY - e.getScreenPosition().y;
        while (std::abs(dy - lastApplied) >= 8) { const int dir = dy > lastApplied ? 1 : -1; lastApplied += dir * 8;
            int t = processor.plockTarget(curSlot());t=t<0?0:juce::jlimit(0,243,t+dir);
            // Route-depth IDs are matrix-internal and intentionally not a P-LOCK
            // scalar target; skip their retained compatibility gap while drag-cycling.
            if(t>=67&&t<=130)t=dir>0?131:66;
            processor.plockSetTarget(curSlot(),t);repaint();moved=true;} }
    void mouseUp(const juce::MouseEvent&) override { if (!moved && listRequest) listRequest(); }
    void mouseMove(const juce::MouseEvent&) override { setTooltip("P-LOCK destination (same list as the matrix DEST): drag = cycle targets, click = list, RMB = clear the slot."); }
private:
    MonomachineNovaAudioProcessor& processor; std::function<int()> slotOf; std::function<void()> listRequest; int dragY = 0, lastApplied = 0; bool moved = false;
};

class ArpPage final : public juce::Component, private juce::Timer {
public:
    std::function<void()> pushEdit; // 1.6.32: точка отката плагина перед массовой правкой шагов
    std::function<void(int)> armRequest; // 1.7.1: прицел P-LOCK (замыкает Surface)
public:
    explicit ArpPage(MonomachineNovaAudioProcessor& p) : processor(p), lane(p, [this] { return activeStart(); }, [this] { return activeEnd(); }, [this](int absolute, const char* f, float v) { setStepParameterAbsolute(absolute, f, v); }),
        plck(p, [this] { return plockActiveStart(); }, [this] { return plockActiveEnd(); }, [this] { return curSlot(); }, [this] { return normalStepTrack(); }),
        startBox(p, "arp_step_start", 1, 64, 1), endBox(p, "arp_step_end", 1, 64, 16), plockStartBox(p, "plock_step_start", 1, 64, 1), plockEndBox(p, "plock_step_end", 1, 64, 16), octavesBox(p, "arp_range", 1, 4, 1), gateBox(p, "arp_length", 1, 127, 64), wrapBox(p, "arp_wrap", 1, 16, 16) { // 1.6.43: WRAP -- квадрат со значением
        addToggle(arpEnable, "ARP ENABLE", "arp_on");
        addToggle(arpHold, "ARP HOLD", "arp_hold");
        addToggle(plockSync, "P-LOCK SYNC", "plock_sync");
        plockSync.setTooltip("ON: P-LOCK reads the same absolute ARP step (legacy behaviour). OFF: P-LOCK cycles its own START..END window once for every ARP note.");
        // 1.6.13: ОДНА галочка HOST TEMPO переключает режим RATE (FREE/BPM) --
        // это arp_sync, БЕЗ касания глобального host_sync. SYNC/TIME рядом
        // переключают режим простым кликом (никаких списков).
        hostTempo.setButtonText("HOST TEMPO");hostTempo.setClickingTogglesState(true);addAndMakeVisible(hostTempo);
        hostTempo.onClick=[this]{setClockMode(hostTempo.getToggleState());};
        hostTempo.setTooltip("The single ARP rate switch: ON = RATE follows the host BPM grid, OFF = free ms. Does NOT switch the plugin-wide host sync."); // 1.6.16: SYNC/TIME-кнопки удалены (дублировали галку)
        // 1.6.25: RETRIG SYNC убран (делал не то, что нужно)
        mode.speedKey="ARP MODE";velocityMode.speedKey="ARP VEL";startBox.speedKey="ARP START";endBox.speedKey="ARP END"; // 1.6.14: показываем 1..16
        // 1.6.14: STEP RND -- при доигрывании каждый новый цикл страницы генерит новый паттерн.
        autoSwapToggle.setClickingTogglesState(true);addAndMakeVisible(autoSwapToggle); // 1.6.42: AUTO SWAP PAGE вместо STEP RND
        autoSwapToggle.onClick=[this]{processor.arpAutoSwap.store(autoSwapToggle.getToggleState());};
        autoSwapToggle.setToggleState(processor.arpAutoSwap.load(),juce::dontSendNotification); // 1.7.6: состояние восстанавливается при пересоздании страницы
        autoSwapToggle.setTooltip("Legacy compatibility control. STEP playback now traverses the continuous START..END window.");
        followPlayToggle.setClickingTogglesState(true);addAndMakeVisible(followPlayToggle); // 1.6.42
        followPlayToggle.setTooltip("The lane already shows the entire active START..END window.");
        followPlayToggle.onClick=[this]{processor.arpFollowPlay.store(followPlayToggle.getToggleState());}; // 1.7.6
        followPlayToggle.setToggleState(processor.arpFollowPlay.load(),juce::dontSendNotification); // 1.7.6: галки AUTO SWAP/FOLLOW PLAY не отжимаются при закрытии окна
        // 1.6.21: PAGE RND -- вторая функция случайности: каждый новый проход играет СЛУЧАЙНУЮ страницу в пределах лимита (шаги по порядку). Параметр arp_step_random.
        pageRndToggle.setClickingTogglesState(true);addAndMakeVisible(pageRndToggle); // 1.6.42: подпись PAGE RND рисует страница
        pageRndToggle.setTooltip("PAGE RND: turning ON writes a new normal pattern into the active START..END steps. Turn it OFF to keep that pattern."); // 1.7.7
        pageRndLink = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processor.parameters, "arp_step_random", pageRndToggle);
        pageRndToggle.onClick=[this]{if(pageRndToggle.getToggleState())processor.queueArpPageRandomWrite(activeStart(),activeEnd());};
        // 1.7.1: STEP RND -- случайный ПОРЯДОК степов (мешок, пересобирается каждый проход); PAGE RND случайную СТРАНИЦУ, этот -- ПОРЯДОК
        stepRndToggle.setClickingTogglesState(true); addAndMakeVisible(stepRndToggle);
        stepRndToggle.setTooltip("STEP RND: the active START..END steps play in shuffled order; the bag is reshuffled every pass.");
        stepRndLink = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processor.parameters, "arp_step_rnd", stepRndToggle);
        // 1.7.1: P-LOCK -- прицел, дестинейшен (как в матрице DEST), полярность, слоты P1..P4
        destBox = std::make_unique<PlockDestBox>(processor, [this] { return curSlot(); }, [this] { showDestList(); });
        polBtn.setButtonText(processor.plockPolarity(0) == 0 ? "UNI" : "BIP"); polBtn.textHeight = 12; addAndMakeVisible(polBtn);
        polBtn.setTooltip("P-LOCK polarity (like the matrix DEST polarity): UNI = absolute value scaled to the target, BIP = +/- around the current value (lane centre 64 = neutral).");
        polBtn.onClick = [this] { const int p2 = processor.plockPolarity(curSlot()) == 0 ? 1 : 0; processor.plockSetPolarity(curSlot(), p2); polBtn.setButtonText(p2 == 0 ? "UNI" : "BIP"); repaint(); };
        // 1.7.11: отдельная кнопка P-LOCK убрана -- прицел теперь на слот-кнопке выше (она же показывает имя параметра)
        detachBtn.setButtonText("DETACH"); detachBtn.textHeight = 12; detachBtn.flipIcon = true; addAndMakeVisible(detachBtn); // 1.7.6: иконка -- квадратик со стрелкой (перелистывание в окно)
        detachBtn.setTooltip("DETACH: the ARP page in its own window -- tweak ARP there and watch the synth knobs here at the same time. Click again (on either window) to re-attach.");
        detachBtn.onClick = [this] { if (detachRequest) detachRequest(); };
        slotBtn.setButtonText("P-LIST"); slotBtn.textHeight = 12; addAndMakeVisible(slotBtn); // 1.7.9: крестик как прицел в матрице // 1.7.5: P-LIST -- клик = список, драг = листать слоты
        slotBtn.setTooltip("P-LIST: LMB click = pick the parameter from the list, LMB drag up/down = cycle slots. The button shows the current parameter. The crosshair button on the right is the AIM.");
        slotBtn.verticalDrag = true; slotBtn.dragPixelsPerStep = 10; // 1.7.5
        slotBtn.dragHandler = [this](int steps, bool) { if (steps == 0) return; slotDraggedMs = juce::Time::getMillisecondCounter();
            std::vector<int> order; for (int s = 0; s < processor.kPlockSlots; ++s) if (processor.plockTarget(s) >= 0) order.push_back(s);
            bool hasCur = false; for (int s : order) if (s == curSlot()) { hasCur = true; break; }
            if (!hasCur) order.push_back(curSlot());
            int pos = 0; for (size_t i = 0; i < order.size(); ++i) if (order[static_cast<size_t>(i)] == curSlot()) { pos = static_cast<int>(i); break; }
            setCurSlot(order[static_cast<size_t>(juce::jlimit(0, static_cast<int>(order.size()) - 1, pos + steps))]); }; // 1.7.5: драг листает слоты
        slotBtn.onClick = [this] { if (juce::Time::getMillisecondCounter() - slotDraggedMs < 400) return; showDestList(); }; // 1.8.0c: ЛКМ = СПИСОК (лкм-драг по-прежнему листает слоты) // 1.7.11 FIX: ЛКМ = ПРИЦЕЛ (крестик теперь рабочий: клик по ручке = цель слота); после драга прицел НЕ вызывается
        aimBtn.textHeight = 12; aimBtn.crosshair = true; addAndMakeVisible(aimBtn); // 1.8.0c: ОТДЕЛЬНЫЙ прицел P-LOCK (крестик переехал с P-LIST -- просил юзер)
        aimBtn.setTooltip("P-LOCK crosshair: click, then click a knob on the face panel -- its parameter locks to the steps of the current page (Elektron-style).");
        aimBtn.onClick = [this] { if (armRequest) armRequest(curSlot()); };
        slotBtn.rightClick = [this] { showDestList(); }; // 1.7.11: список целей переехал на ПКМ
        addAndMakeVisible(plck);
        plck.pushEdit=[this]{ if (pushEdit) pushEdit(); }; destBox->pushEdit=plck.pushEdit; // 1.7.3: P-LOCK пишется в undo
        setCurSlot(0);
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
        rateButton.rightClick=[this]{ // 1.6.25: список рейтов по ПКМ (значение -- пунктом списка)
            const bool sync=isSyncMode();juce::PopupMenu m;m.setLookAndFeel(&getLookAndFeel());
            static const float msVals[]{5,10,20,30,40,50,75,100,150,200,300,400,500,750,1000,1500,2000}; // static: живёт после закрытия лямбды
            juce::StringArray items;
            if(sync)items=processor.parameters.getParameter("arp_grid")->getAllValueStrings();
            else for(float v:msVals)items.add(juce::String(v,0)+" MS");
            auto* par=processor.parameters.getParameter(sync?"arp_grid":"arp_time");const float cur=par->convertFrom0to1(par->getValue());
            for(int i=0;i<items.size();++i){const bool hit=sync?i==juce::roundToInt(cur):std::abs(msVals[i]-cur)<5.0f;m.addItem(i+1,items[i],true,hit);}
            const juce::Component::SafePointer<ArpPage> safe(this);
            m.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(&rateButton),[safe,sync](int res){if(!safe||res<=0)return;
                static const float msVals2[]{5,10,20,30,40,50,75,100,150,200,300,400,500,750,1000,1500,2000};
                auto* p2=safe->processor.parameters.getParameter(sync?"arp_grid":"arp_time");p2->beginChangeGesture();
                p2->setValueNotifyingHost(p2->convertTo0to1(sync?static_cast<float>(res-1):msVals2[res-1]));p2->endChangeGesture();safe->refreshRate();}); };
        // 1.6.16: GATE -- просто квадрат со значением (слайдер убран); WRAP остаётся слайдером.
        addAndMakeVisible(wrapBox); // 1.6.43 FIX: WRAP -- квадрат со значением (как GATE); в 1.6.42 setRange и attachment были съедены комментарием -- нумерация была мертва
        wrapBox.setTooltip("WRAP: how many notes of the pool cycle in True/Up/Down/Cycl/Rnd modes (1..16). In STEP mode the page length is PAGE LIMIT x 8 steps.");
        addAndMakeVisible(startBox); addAndMakeVisible(endBox); addAndMakeVisible(plockStartBox); addAndMakeVisible(plockEndBox); addAndMakeVisible(octavesBox); addAndMakeVisible(gateBox); // independent P-LOCK window controls
        const std::pair<juce::Component*,const char*> layIds[]{{&arpEnable,"enable"},{&arpHold,"hold"},{&mode,"mode"},{&rateButton,"rate"},{&octavesBox,"oct"},{&gateBox,"gate"},{&hostTempo,"hosttempo"},{&wrapBox,"wrap"},{&velocityMode,"velmode"},{&pageRndButton,"rndpage"},{&pitchRndButton,"rndpitch"},{&velRndButton,"rndvel"},{&holdRndButton,"rndhold"},{&autoSwapToggle,"autoswap"},{&followPlayToggle,"follow"},{&startBox,"start"},{&endBox,"end"},{&wrapBox,"wrap"},{&lane,"lane"},{&stepRndToggle,"steprnd"},{&plck,"plocklane"},{&plkRndButton,"rndplk"},{&slideRndButton,"rndslide"},{&plockSync,"plocksync"},{&plockStartBox,"plockstart"},{&plockEndBox,"plockend"},{&polBtn,"plockpol"},{&slotBtn,"plockslot"},{&aimBtn,"plockaim"},{&normalShiftLeft,"normalshiftleft"},{&normalShiftRight,"normalshiftright"},{&plockShiftLeft,"plockshiftleft"},{&plockShiftRight,"plockshiftright"},{&detachBtn,"detach"}}; // 1.7.11: armBtn убран (прицел = слот-кнопка) // 1.7.2: кнопка слота в LAYOUT EDIT // 1.7.1: +P-LOCK/STEP RND в LAYOUT EDIT
        for(auto& pid:layIds)pid.first->setComponentID(pid.second); // 1.6.38
        // START/END deliberately do not use JUCE's bright stock entry dialog:
        // double-click restores the canonical normal window, while RMB opens a
        // black numeric editor. P-LOCK gets the same input convention locally.
        startBox.editOnRightClick=endBox.editOnRightClick=plockStartBox.editOnRightClick=plockEndBox.editOnRightClick=true;
        startBox.doubleClickHandler=[this]{resetNormalWindow();};endBox.doubleClickHandler=[this]{resetNormalWindow();};
        plockStartBox.doubleClickHandler=[this]{resetPlockWindow();};plockEndBox.doubleClickHandler=[this]{resetPlockWindow();};
        startBox.setTooltip("START: first active step, 1..64. Double-click resets normal START/END to 1..16; RMB enters a value.");
        endBox.setTooltip("END: last active step, 1..64. Double-click resets normal START/END to 1..16; RMB enters a value.");
        plockStartBox.setTooltip("P-LOCK START: first independent P-LOCK step. It applies when P-LOCK SYNC is OFF; RMB enters a value.");
        plockEndBox.setTooltip("P-LOCK END: last independent P-LOCK step. It applies when P-LOCK SYNC is OFF; RMB enters a value.");
        gateBox.speedKey="VALUE BOXES";gateBox.setTooltip("ARP gate length, 1..127. Drag / wheel = +-1, double-click = default.");
        // 1.6.13: RANDOM-надписи больше нет: кнопки PAGE/ALL, под ними PITCH/VEL.
        pageRndButton.setButtonText("PAGE"); pageRndButton.textHeight = 12; addAndMakeVisible(pageRndButton);
        pageRndButton.setTooltip("Randomize all normal fields in the active START..END window.");
        pitchRndButton.setButtonText("PITCH"); pitchRndButton.textHeight = 12; addAndMakeVisible(pitchRndButton);
        pitchRndButton.setTooltip("Randomize TRANSPOSE in the active START..END window.");
        velRndButton.setButtonText("VEL"); velRndButton.textHeight = 12; addAndMakeVisible(velRndButton);
        velRndButton.setTooltip("Randomize VELOCITY in the active START..END window.");
        holdRndButton.setButtonText("HOLD"); holdRndButton.textHeight = 12; addAndMakeVisible(holdRndButton); // 1.6.22
        holdRndButton.setTooltip("Randomize HOLD in the active START..END window.");
        plkRndButton.setButtonText("PLK"); plkRndButton.textHeight = 12; addAndMakeVisible(plkRndButton); // 1.7.1
        plkRndButton.setTooltip("P-LOCK random: LMB = randomize the p-lock values of the current slot on this page, RMB = on all 16 pages.");
        slideRndButton.setButtonText("SLIDE"); slideRndButton.textHeight = 12; addAndMakeVisible(slideRndButton); // 1.7.1
        slideRndButton.setTooltip("SLIDE random: LMB = randomize the slide flags of the current slot on this page, RMB = on all 16 pages.");
        holdRndButton.onClick=[this]{randomizeRange(activeStart(),activeEnd(),false,false,true);};
        plkRndButton.onClick=[this]{randomizePlock(plockActiveStart(),plockActiveEnd(),true,false);}; // 1.7.6: страница та, что видна в линии (FOLLOW PLAY учтён)
        plkRndButton.rightClick=[this]{randomizePlock(0,63,true,false);};
        slideRndButton.onClick=[this]{randomizePlock(plockActiveStart(),plockActiveEnd(),false,true);}; // 1.7.6
        slideRndButton.rightClick=[this]{randomizePlock(0,63,false,true);};
        pageRndButton.onClick=[this]{randomizeRange(activeStart(),activeEnd(),true,true,true);randomizePlock(plockActiveStart(),plockActiveEnd(),true,true);}; // 1.7.7: ВСЁ на странице (транспоз+вел+холд+слайды+P-LOCK), страница = видимая (FOLLOW PLAY)
        pitchRndButton.onClick=[this]{randomizeRange(activeStart(),activeEnd(),true,false,false);};
        velRndButton.onClick=[this]{randomizeRange(activeStart(),activeEnd(),false,true,false);};
        // 1.6.25: ПКМ по PAGE/PITCH/VEL/HOLD -- та же рандомизация на ВСЕХ страницах (ЛКМ -- только текущая)
        pageRndButton.rightClick=[this]{randomizeRange(0,63,true,true,true);};
        pitchRndButton.rightClick=[this]{randomizeRange(0,63,true,false,false);};
        velRndButton.rightClick=[this]{randomizeRange(0,63,false,true,false);};
        holdRndButton.rightClick=[this]{randomizeRange(0,63,false,false,true);};
        addAndMakeVisible(lane);
        normalShiftLeft.setButtonText("<");normalShiftRight.setButtonText(">");plockShiftLeft.setButtonText("<");plockShiftRight.setButtonText(">");
        for(auto* b:{&normalShiftLeft,&normalShiftRight,&plockShiftLeft,&plockShiftRight}){b->textHeight=15;addAndMakeVisible(*b);}
        normalShiftLeft.setTooltip("Shift the complete normal 64-step pattern left by one step.");normalShiftRight.setTooltip("Shift the complete normal 64-step pattern right by one step.");
        plockShiftLeft.setTooltip("Shift this P-LOCK slot's 64 steps left by one step.");plockShiftRight.setTooltip("Shift this P-LOCK slot's 64 steps right by one step.");
        normalShiftLeft.onClick=[this]{shiftNormalSteps(-1);};normalShiftRight.onClick=[this]{shiftNormalSteps(1);};
        plockShiftLeft.onClick=[this]{shiftPlockSteps(-1);};plockShiftRight.onClick=[this]{shiftPlockSteps(1);};
        // 1.6.19: сетка GUI DRAG SPEED переехала в MENU > GUI OPTIONS.
        setSize(1160, 478); startTimerHz(15); // 1.6.42: страница в 480 -- без скролла
        const std::pair<const char*,juce::Rectangle<int>> heads[]{{"hMODE",{127,25,66,14}},{"hOCT",{198,25,60,14}},{"hRATE",{237,25,60,14}},{"hGATE",{302,25,60,14}},{"hVEL",{352,25,95,14}},{"hWRAP",{454,25,52,14}},{"hASWAP",{517,21,70,13}},{"hFOLLOW",{586,28,86,13}},{"hSTART",{695,25,40,14}},{"hEND",{740,25,40,14}},{"hPAGERND",{786,25,50,14}},{"hSTEPRND",{838,25,72,14}},{"hRANDOM",{893,25,110,14}}}; // 1.6.43: заголовки ARP; 1.7.1: +hSTEPRND
        for(auto& h2:heads){ auto tg=std::make_unique<TextTag>(*this,h2.first,h2.second); tg->vLock=true; headTags[h2.first]=std::move(tg); }
    }
    ~ArpPage() override { stopTimer(); }
    juce::ComponentDragger winDragger; // 1.7.9: детач-окно тащится ЛКМ по пустому месту
    void mouseDown(const juce::MouseEvent& e) override { if (auto* w = findParentComponentOfClass<juce::DialogWindow>()) winDragger.startDraggingComponent(w, e); } // 1.7.9
    void mouseDrag(const juce::MouseEvent& e) override { if (auto* w = findParentComponentOfClass<juce::DialogWindow>()) winDragger.dragComponent(w, e, nullptr); } // 1.7.9
    void plockAfterBind(int slot, int target) { const int s2 = processor.plockAcquire(slot, target); processor.plockSetTarget(s2, target); processor.plockFillPage(s2, plockActiveStart()/8); setCurSlot(s2); plck.repaint(); repaint(); } // 1.7.2: та же цель = тот же слот, иначе новый; лейн показывает назначенный

    // 1.6.16: скролл страницы снова колёсиком (как было), ЛКМ-драг-скролл убран.
    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::black); g.setColour(ink); // 1.6.42: раскладка по скрину arp ref clean2 -- одна шапка, без заголовка/рамок, лейн выше (без скролла)
        auto drawHead=[this,&g](const char* id,const juce::String& fb,juce::Rectangle<int> r,int fh){ auto it2=headTags.find(id); if(it2==headTags.end())return; auto* tg=it2->second.get(); const juce::String s=tg->tagText.isNotEmpty()?tg->tagText:fb; g.setColour(tg->colorSet?tg->tagColor:ink);
            if(tg->fontId==1){g.setFont(juce::Font(juce::Font::getDefaultSansSerifFontName(),(float)tg->fontFor(fh),juce::Font::plain));g.drawText(s,tg->getBounds(),juce::Justification::centred);}
            else if(tg->fontId==2){g.setFont(juce::Font(juce::Font::getDefaultMonospacedFontName(),(float)tg->fontFor(fh),juce::Font::plain));g.drawText(s,tg->getBounds(),juce::Justification::centred);}
            else pixel::text(g,s,tg->getBounds(),tg->fontFor(fh),true); }; // 1.6.43: заголовки -- теги (цвет/шрифт/позиция из редактора)
        drawHead("hMODE","MODE",{127,25,66,14},14); drawHead("hOCT","OCT",{198,25,60,14},14);
        drawHead("hRATE","RATE",{237,25,60,14},14); drawHead("hGATE","GATE",{302,25,60,14},14);
        drawHead("hVEL","VELOCITY",{352,25,95,14},14); drawHead("hWRAP","WRAP",{454,25,52,14},14);
        drawHead("hASWAP","AUTO SWAP",{517,21,70,13},12); 
        drawHead("hFOLLOW","FOLLOW PLAY",{586,28,86,13},12);
        drawHead("hSTART","START",{695,25,40,14},12); drawHead("hEND","END",{740,25,40,14},12); drawHead("hPAGERND","PAGE RND",{786,25,50,14},14);
        drawHead("hSTEPRND","STEP RND",{838,25,72,14},14); // 1.7.1: подпись STEP RND (порядок степов)
        drawHead("hRANDOM","RANDOM",{893,25,110,14},14);
        // Labels follow their controls instead of remaining at the old fixed
        // rows below the step lanes when a saved ARP layout moves the shifts.
        const auto ps=plockStartBox.getBounds(),pe=plockEndBox.getBounds();
        pixel::text(g,"P START",{ps.getX(),ps.getY()-13,ps.getWidth(),12},10,true);
        pixel::text(g,"P END",{pe.getX(),pe.getY()-13,pe.getWidth(),12},10,true);
        const auto ns=normalShiftLeft.getBounds(),pls=plockShiftLeft.getBounds();
        pixel::text(g,"N",{ns.getX()-20,ns.getY()+4,16,14},10,true);
        pixel::text(g,"P",{pls.getX()-20,pls.getY()+4,16,14},10,true);
    }
    void resized() override {
        arpEnable.setBounds(21, 20, 104, 21); hostTempo.setBounds(21, 43, 104, 21); arpHold.setBounds(21, 66, 104, 21); // 1.6.44: хитбокс уже -- не достаёт до MODE(127) (1.6.42: галки стопкой слева)
        mode.setBounds(127, 45, 66, 26); octavesBox.setBounds(198, 45, 33, 26); rateButton.setBounds(237, 45, 59, 26); gateBox.setBounds(302, 45, 43, 26); velocityMode.setBounds(352, 45, 95, 26); wrapBox.setBounds(454, 45, 52, 26); // 1.6.43: квадрат со значением
        autoSwapToggle.setBounds(537, 45, 27, 26); followPlayToggle.setBounds(598, 45, 27, 26);
        startBox.setBounds(695, 45, 40, 26); endBox.setBounds(740, 45, 40, 26);
        pageRndToggle.setBounds(808, 45, 26, 26); stepRndToggle.setBounds(840, 45, 26, 26); // 1.7.1: STEP RND рядом с PAGE RND
        pageRndButton.setBounds(869, 45, 42, 26); pitchRndButton.setBounds(915, 45, 42, 26); velRndButton.setBounds(961, 45, 42, 26); holdRndButton.setBounds(1007, 45, 42, 26); // 1.7.1: ряд ужат -- +PLK/SLIDE
        plkRndButton.setBounds(1053, 45, 42, 26); slideRndButton.setBounds(1099, 45, 42, 26); // 1.7.1: P-LOCK/SLIDE в секции рандома: без ALL -- ПКМ на PAGE = ALL (было и в 1.6.25)
        // Canonical default geometry. uiLayoutApply below may intentionally
        // override these only when the user has saved a custom layout.
        lane.setBounds(21, 97, 1129, 197);
        plck.setBounds(21, 341, 1129, 96);
        // Compact P-LOCK header immediately above its value lane. The dynamic
        // captions in paint() follow these boxes if Layout Edit moves them.
        aimBtn.setBounds(399, 312, 40, 26);slotBtn.setBounds(439, 312, 159, 26);polBtn.setBounds(598, 312, 74, 26);
        plockSync.setBounds(680,312,96,26);plockStartBox.setBounds(780,312,42,26);plockEndBox.setBounds(826,312,42,26);
        // Separate normal/P-LOCK shift rows sit in the compact strip beneath
        // the normal steps; their captions follow the actual saved positions.
        normalShiftLeft.setBounds(68,295,18,22);normalShiftRight.setBounds(92,295,18,22);
        plockShiftLeft.setBounds(68,314,18,22);plockShiftRight.setBounds(92,314,18,22);
        detachBtn.setBounds(1098,12,52,21);destBox->setBounds(872,312,160,26);
        uiLayoutApply(*this,"arp"); // 1.6.38: сохранённая раскладка ARP-страницы
    }
private:
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    void addToggle(juce::ToggleButton& button, const juce::String& text, const juce::String& id) { button.setButtonText(text); button.setClickingTogglesState(true); addAndMakeVisible(button); toggleLinks.push_back(std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processor.parameters, id, button)); }
    juce::Rectangle<int> normalStepTrack() const {
        const auto q=lane.geo();const int left=lane.getX()+q.x0;
        const int right=left+q.count*q.w+(q.count-1)*q.gap;
        return {left,0,std::max(1,right-left),1};
    }
    bool isSyncMode() const { auto* s = processor.parameters.getRawParameterValue("arp_sync"); return s && s->load() > 0.5f; }
    int activeStart() const { return juce::jlimit(0,63,juce::roundToInt(startBox.getValue())-1); }
    int activeEnd() const { return juce::jlimit(activeStart(),63,juce::roundToInt(endBox.getValue())-1); }
    bool isPlockSync() const { return plockSync.getToggleState(); }
    int plockActiveStart() const { return isPlockSync()?activeStart():juce::jlimit(0,63,juce::roundToInt(plockStartBox.getValue())-1); }
    int plockActiveEnd() const { return isPlockSync()?activeEnd():juce::jlimit(plockActiveStart(),63,juce::roundToInt(plockEndBox.getValue())-1); }
    void setParameterValue(const juce::String& id,float value){if(auto* p=processor.parameters.getParameter(id)){p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(value));p->endChangeGesture();}}
    void resetNormalWindow(){if(pushEdit)pushEdit();setParameterValue("arp_step_start",1.0f);setParameterValue("arp_step_end",16.0f);startBox.refreshFromParam();endBox.refreshFromParam();rebindSteps();}
    void resetPlockWindow(){if(pushEdit)pushEdit();setParameterValue("plock_step_start",1.0f);setParameterValue("plock_step_end",16.0f);plockStartBox.refreshFromParam();plockEndBox.refreshFromParam();rebindSteps();}
    void setClockMode(bool sync) { if (auto* p = processor.parameters.getParameter("arp_sync")) { p->beginChangeGesture(); p->setValueNotifyingHost(p->convertTo0to1(sync ? 1.0f : 0.0f)); p->endChangeGesture(); } refreshRate(); }
    void refreshRate() { // единая кнопка RATE показывает значение текущего режима
        const bool sync = isSyncMode();
        auto* grid = processor.parameters.getParameter("arp_grid");
        const juce::String text = sync ? grid->getText(grid->getValue(), 16) : juce::String(processor.parameters.getRawParameterValue("arp_time")->load(), 0) + " MS";
        if (text != seenRateText) { rateButton.setButtonText(text); rateButton.repaint(); seenRateText = text; } // 1.7.5 FIX: repaint только при изменении -- галка SYNC/RATE не мигают при входе
        if (sync != seenSync) { hostTempo.setToggleState(sync, juce::dontSendNotification); seenSync = sync; } // 1.7.5
    }
    void setStepParameter(int pg, int st, const char* field, float value) { const auto id = "arp_s" + juce::String(pg) + "_" + juce::String(st) + "_" + field; if (auto* p = processor.parameters.getParameter(id)) { p->beginChangeGesture(); p->setValueNotifyingHost(p->convertTo0to1(value)); p->endChangeGesture(); } }
    void setStepParameterAbsolute(int absolute,const char* field,float value){absolute=juce::jlimit(0,63,absolute);setStepParameter(absolute/8,absolute%8,field,value);}
    void shiftNormalSteps(int direction){
        if(pushEdit)pushEdit();const int d=direction<0?-1:1;
        std::array<std::array<float,3>,64> saved{};const char* fields[]{"transpose","velocity","hold"};
        for(int step=0;step<64;++step)for(int f=0;f<3;++f){const auto id="arp_s"+juce::String(step/8)+"_"+juce::String(step%8)+"_"+fields[f];if(auto* raw=processor.parameters.getRawParameterValue(id))saved[static_cast<size_t>(step)][static_cast<size_t>(f)]=raw->load();}
        for(int step=0;step<64;++step){const int from=(step-d+64)%64;for(int f=0;f<3;++f)setStepParameterAbsolute(step,fields[f],saved[static_cast<size_t>(from)][static_cast<size_t>(f)]);}
        lane.repaint();
    }
    void shiftPlockSteps(int direction){
        const int slot=curSlot();if(processor.plockTarget(slot)<0)return;if(pushEdit)pushEdit();const int d=direction<0?-1:1;
        std::array<float,64> values{},slides{};for(int step=0;step<64;++step){values[static_cast<size_t>(step)]=processor.plockValue(slot,step/8,step%8);slides[static_cast<size_t>(step)]=processor.plockSlideValue(slot,step/8,step%8);}
        for(int step=0;step<64;++step){const int from=(step-d+64)%64;processor.plockSetValue(slot,step/8,step%8,values[static_cast<size_t>(from)]);processor.plockSetSlide(slot,step/8,step%8,slides[static_cast<size_t>(from)]>0.5f);}
        plck.repaint();
    }
    // 1.6.13: батч-рандом без жестов хоста (128 шагов больше не лагает):
    // приостанавливаем аудио-поток на время записи, в хост шлём только значения.
    // Randomisation addresses the live inclusive 1..64 window directly.  It
    // uses normal parameter gestures, so values persist and immediately appear
    // in every compressed lane rather than as a temporary preview.
    void randomizeRange(int first,int last,bool doTr,bool doVel,bool doHold){
        if(pushEdit)pushEdit();first=juce::jlimit(0,63,first);last=juce::jlimit(first,63,last);auto& r=juce::Random::getSystemRandom();
        for(int absolute=first;absolute<=last;++absolute){if(doHold)setStepParameterAbsolute(absolute,"hold",float(r.nextInt(2)));if(doTr)setStepParameterAbsolute(absolute,"transpose",float(r.nextInt(49)-24));if(doVel)setStepParameterAbsolute(absolute,"velocity",float(r.nextInt(128)));}
        lane.repaint();repaint();
    }
    // ---- 1.7.1: P-LOCK -------------------------------------------------------
    void randomizePlock(int first,int last,bool doVals,bool doSlide){
        int sl=juce::jlimit(0,processor.kPlockSlots-1,curSlot());if(processor.plockTarget(sl)<0){sl=-1;for(int i=0;i<processor.kPlockSlots;++i)if(processor.plockTarget(i)>=0){sl=i;break;}if(sl<0){showDestList();return;}}
        if(sl!=curSlot())setCurSlot(sl);if(pushEdit)pushEdit();first=juce::jlimit(0,63,first);last=juce::jlimit(first,63,last);auto& r=juce::Random::getSystemRandom();
        for(int absolute=first;absolute<=last;++absolute){const int pg=absolute/8,st=absolute%8;if(doVals)processor.plockSetValue(sl,pg,st,r.nextInt(100)<35?0.0f:float(1+r.nextInt(127)));if(doSlide)processor.plockSetSlide(sl,pg,st,r.nextInt(100)<25);}
        plck.repaint();
    }
    void showDestList(){
        juce::PopupMenu m;m.setLookAndFeel(&getLookAndFeel());int used=0;
        for(int s=0;s<processor.kPlockSlots;++s){const int target=processor.plockTarget(s);if(target<0)continue;m.addItem(30000+s,juce::String(s+1)+" - "+targetName(processor,target),true,curSlot()==s);++used;}
        if(used>0)m.addSeparator();m.addItem(29900,"NEW EMPTY SLOT",true,processor.plockTarget(curSlot())>=0);m.addItem(29901,"CLEAR THIS SLOT",true,processor.plockTarget(curSlot())<0);
        auto addTarget=[&](juce::PopupMenu& dst,int target){dst.addItem(10000+target,targetName(processor,target),true,processor.plockTarget(curSlot())==target);};
        auto addRange=[&](juce::PopupMenu& dst,int first,int count){for(int i=0;i<count;++i)addTarget(dst,first+i);};
        {juce::PopupMenu p1,synt;addRange(synt,0,8);addTarget(synt,66);p1.addSubMenu("SYNT",synt);juce::PopupMenu amp;addRange(amp,8,8);p1.addSubMenu("AMP",amp);juce::PopupMenu filt;addRange(filt,16,8);p1.addSubMenu("FILT",filt);juce::PopupMenu effx;addRange(effx,24,8);p1.addSubMenu("EFFX",effx);m.addSubMenu("P1 PARAM",p1);}
        {juce::PopupMenu p1l;for(int i=0;i<3;++i){juce::PopupMenu l;addRange(l,32+i*8,8);p1l.addSubMenu("LFO"+juce::String(i+1),l);}for(int i=0;i<3;++i){juce::PopupMenu l;addRange(l,188+i*8,8);p1l.addSubMenu("LFO"+juce::String(i+4),l);}m.addSubMenu("P1 LFO",p1l);}
        {juce::PopupMenu p2,synt;addRange(synt,132,8);p2.addSubMenu("SYNT",synt);juce::PopupMenu amp;addRange(amp,140,8);p2.addSubMenu("AMP",amp);juce::PopupMenu filt;addRange(filt,148,8);p2.addSubMenu("FILT",filt);juce::PopupMenu effx;addRange(effx,156,8);p2.addSubMenu("EFFX",effx);m.addSubMenu("P2 PARAM",p2);}
        {juce::PopupMenu p2l;for(int i=0;i<3;++i){juce::PopupMenu l;addRange(l,164+i*8,8);p2l.addSubMenu("LFO"+juce::String(i+1),l);}for(int i=0;i<3;++i){juce::PopupMenu l;addRange(l,212+i*8,8);p2l.addSubMenu("LFO"+juce::String(i+4),l);}m.addSubMenu("P2 LFO",p2l);}
        {juce::PopupMenu arp;addTarget(arp,64);addTarget(arp,65);m.addSubMenu("ARP",arp);} {juce::PopupMenu mseg;addRange(mseg,56,8);m.addSubMenu("MSEG OUT",mseg);} {juce::PopupMenu rates;addRange(rates,236,8);m.addSubMenu("MSEG RATE",rates);} {juce::PopupMenu legacy;addTarget(legacy,131);m.addSubMenu("LEGACY",legacy);}
        const juce::Component::SafePointer<ArpPage> safe(this);
        m.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(&slotBtn),[safe](int result){if(!safe||result<=0)return;if(result>=30000&&result<30000+safe->processor.kPlockSlots){safe->setCurSlot(result-30000);return;}if(result==29900){int freeSlot=safe->curSlot();for(int s=0;s<safe->processor.kPlockSlots;++s)if(safe->processor.plockTarget(s)<0){freeSlot=s;break;}safe->setCurSlot(freeSlot);return;}if(result==29901){if(safe->pushEdit)safe->pushEdit();safe->processor.plockSetTarget(safe->curSlot(),-1);safe->repaint();return;}if(result>=10000&&result<=10243)safe->bindPlockTarget(safe->curSlot(),result-10000);});
    }
    void bindPlockTarget(int slot, int tg) { // 1.7.2: та же цель = тот же слот, новая -- первый свободный; степы страницы получают текущее значение цели
        if (pushEdit) pushEdit(); // 1.7.3
        const int s2 = processor.plockAcquire(slot, tg);
        processor.plockSetTarget(s2, tg); processor.plockFillPage(s2, plockActiveStart()/8);
        setCurSlot(s2); plck.repaint(); repaint(); }
    int curSlot() const { return juce::jlimit(0, processor.kPlockSlots - 1, processor.plockUiSlot.load(std::memory_order_relaxed)); } // 1.7.9: читаем ОБЩИЙ слот (детач-окно и синх-страница синхронны) // 1.7.1
    void setCurSlot(int s) { plockSlot = juce::jlimit(0, processor.kPlockSlots - 1, s); processor.plockUiSlot.store(plockSlot, std::memory_order_relaxed); // 1.7.9: слот ОБЩИЙ -- синх-страница и детач-окно показывают один и тот же
        const int tg0 = processor.plockTarget(curSlot()); slotBtn.setButtonText(tg0 >= 0 ? targetName(processor, tg0) : juce::String("P-LIST")); slotBtn.repaint(); // 1.7.7: кнопка показывает ТЕКУЩИЙ параметр слота (над ней и так подпись P-LOCK)
        polBtn.setButtonText(processor.plockPolarity(curSlot()) == 0 ? "UNI" : "BIP"); plck.repaint(); repaint(); }
    void rebindSteps() { lane.repaint(); repaint(); }
    void timerCallback() override {
        refreshRate(); startBox.refreshFromParam(); endBox.refreshFromParam();plockStartBox.refreshFromParam();plockEndBox.refreshFromParam();
        { const int tg = processor.plockTarget(curSlot()); const juce::String want = tg >= 0 ? targetName(processor, tg) : juce::String("P-LIST"); // 1.7.7: имя параметра слота живёт (бинд/очистка/прицел из другого окна)
          if (slotBtn.getButtonText() != want) { slotBtn.setButtonText(want); slotBtn.repaint(); } }
        const int shown=(activeStart()<<18)|(activeEnd()<<12)|(plockActiveStart()<<6)|plockActiveEnd(); if(shown!=seenPage){seenPage=shown;rebindSteps();}
        lane.repaint(); plck.repaint(); repaint();
    }
    int pageDisplay() const { return activeStart()/8; } // legacy callers: first 8-step storage page
    juce::ToggleButton pageRndToggle; std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> pageRndLink; // 1.6.21
    juce::ToggleButton stepRndToggle; std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> stepRndLink; // 1.7.1: STEP RND
    MonomachineNovaAudioProcessor& processor; juce::ToggleButton arpEnable, arpHold, hostTempo, autoSwapToggle, followPlayToggle, plockSync; PixelButton rateButton{""}, pageRndButton{"PAGE"}, pitchRndButton{"PITCH"}, velRndButton{"VEL"}, holdRndButton{"HOLD"}, plkRndButton{"PLK"}, slideRndButton{"SLIDE"}, polBtn{"UNI"}; StepLane lane; PlockLane plck; // 1.7.11: armBtn убран -- прицел на слот-кнопке
    std::unique_ptr<PlockDestBox> destBox; PixelButton slotBtn{"S1"}, aimBtn{""}, detachBtn{"DETACH"}, normalShiftLeft{"<"}, normalShiftRight{">"}, plockShiftLeft{"<"}, plockShiftRight{">"}; int plockSlot = 0; public: std::function<void()> detachRequest; // 1.7.2/5: public -- замыкает Surface
    std::map<juce::String,std::unique_ptr<TextTag>> headTags; // 1.6.43: заголовки ARP как теги
    DragChoice mode, velocityMode; // 1.6.14: clock-список и PAGE-комбо удалены
    DragValueBox startBox, endBox, plockStartBox, plockEndBox, octavesBox, gateBox; // normal and independent P-LOCK boundaries
    DragValueBox wrapBox; // 1.6.43: WRAP -- квадрат со значением
    float rateAccum = 0; int seenPage = -1; juce::String seenRateText; bool seenSync = false; juce::uint32 slotDraggedMs = 0; // 1.7.5
    std::vector<std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>> toggleLinks; std::unique_ptr<ComboAttachment> modeLink, velocityModeLink; std::unique_ptr<SliderAttachment> wrapLink; // 1.6.16: attachment GATE убран
};

// The modulation matrix as a real screen page (8 routes x on/src/dest/depth),
// drawn in the same pixel language; cells change by drag, popup on click.
class MatrixPage final : public juce::Component, private juce::Timer {
public:
    explicit MatrixPage(MonomachineNovaAudioProcessor& p) : processor(p) { clearButton.setComponentID("clear"); // 1.6.38: LAYOUT EDIT
        const char* fields[]{"on", "src", "dest", "depth", "mode"};
        for (int r = 0; r < 64; ++r) for (int c = 0; c < 5; ++c) cells[static_cast<size_t>(r * 5 + c)] = p.parameters.getParameter("r" + juce::String(r) + "_" + fields[c]); // 1.7.7: 64 слота
        // 1.6.20: CLEAR -- стереть ВСЕ модуляции разом (все галочки ON в OFF).
        addAndMakeVisible(clearButton);clearButton.textHeight=14;clearButton.setTooltip("Erase ALL routes at once: every ON checkbox goes off.");
        auto wipeField=[this](const char* f){for(int r=0;r<64;++r)if(auto* p=processor.parameters.getParameter("r"+juce::String(r)+"_"+f)){p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(0.0f));p->endChangeGesture();}}; // 1.7.9 FIX: нотифицируем хост (CLEAR иногда не стирал колонки)а // 1.6.23: ровно ноль (getDefaultValue у float уже нормализован -- была двойная конвертация), без спама хосту
        clearButton.onClick=[this,wipeField]{for(const char* f:{"on","src","dest","depth","mode","lock","aux","aux_depth"})wipeField(f);repaint();}; // 1.6.35: CLEAR сбрасывает и AUX DEPTH // 1.6.21: ЛКМ = ВСЁ в дефолт (включая замки)
        clearButton.rightClick=[this,wipeField]{juce::PopupMenu m;m.setLookAndFeel(&smallMenuLaf()); // 1.6.33: компактный список
            m.addItem(1,"ERASE EVERYTHING (defaults)");m.addItem(2,"ONLY ROUTE ON/OFF");m.addItem(3,"ONLY DEPTH VALUES");m.addItem(4,"ONLY LOCKS");m.addItem(5,"ONLY POLARITY MODE");m.addItem(8,"ONLY SOURCES (src)");m.addItem(9,"ONLY TARGETS (dest)");m.addItem(6,"ONLY AUX SOURCE");m.addItem(7,"ONLY AUX DEPTH"); // 1.6.41
            const juce::Component::SafePointer<MatrixPage> safe(this);
            m.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(&clearButton),[safe,this,wipeField](int res){if(!safe||res<=0)return;
                if(res==1)for(const char* f:{"on","src","dest","depth","mode","lock","aux","aux_depth"})wipeField(f); // 1.6.41: ERASE чистит и AUX DEPTH
                else if(res==6)wipeField("aux");else if(res==7)wipeField("aux_depth"); // 1.6.41
                else if(res==2)wipeField("on");else if(res==3)wipeField("depth");else if(res==4)wipeField("lock");else if(res==5)wipeField("mode");else if(res==8)wipeField("src");else if(res==9)wipeField("dest"); // 1.7.9
                safe->repaint();});};
        clearButton.setBounds(1013,1,115,26);if(uiLayout().has("matrix.clear"))clearButton.setBounds(uiLayout().get("matrix.clear")); // 1.6.38: сохранённая позиция CLEAR
        if(uiLayout().has("matrix.colw")){ const auto cw=uiLayout().getArr("matrix.colw"); if((int)cw.size()==8){ for(int i=0;i<8;++i)colw[(size_t)i]=juce::jlimit(18,280,cw[(size_t)i]); normalizeCols(); } } // 1.6.41: ширины колонок из layout.json
        setSize(1145, 58 + 16 * 34 + 40); startTimerHz(15); // 1.7.9: по умолчанию 16 видимых строк (влезает без скролла), дальше -- по мере занятия слотов
    }
    ~MatrixPage() override { stopTimer(); }
    std::function<void(int)> onTargetPick;
    std::function<void()> pushEdit; // 1.6.32: точка отката перед правкой маршрута
    int flashDestRow=-1; juce::uint32 flashDestUntil=0; // 1.6.31
    void flashDestCell(int row){flashDestRow=row;flashDestUntil=juce::Time::getMillisecondCounter()+1320;repaint();} // 1.6.31: мигание рамки DEST после бинда прицелом
    static int percent(int raw) { return juce::jlimit(0, 100, juce::roundToInt(std::abs(raw) * 100.0f / (raw < 0 ? 64.0f : 63.0f))); }
    static juce::String signedPercent(int raw) { return raw == 0 ? "0%" : juce::String(raw < 0 ? "-" : "+") + juce::String(percent(raw)) + "%"; }

    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::black); g.setColour(ink);
        pixel::text(g, "MODULATION MATRIX", {10, 3, 400, 24}, 20);
        pixel::text(g, "CLICK HEADER TO SORT (ON, SOURCE, DEST, DEPTH, MODE) / DRAG MSEG OUTPUT TO TARGET", {470, 7, 650, 17}, 10, false);
        const char* headers[]{"", "ON", "SOURCE", "DESTINATION", "DEPTH", "MODE", "AUX SRC", "AUX DEPTH"}; // 1.6.32: + AUX DEPTH
        for (int c = 1; c < 8; ++c) pixel::text(g, headers[c], {colX(c), 30, colWidth(c), 19}, 13, true); // 1.6.34: рисуем ВСЕ заголовки (AUX DEPTHH был пропущен циклом c<7)
        for (int c = 1; c < 8; ++c) g.drawVerticalLine(colX(c) - 3, 30.0f, 49.0f); g.drawVerticalLine(colX(7) + colWidth(7) + 2, 30.0f, 49.0f); // 1.6.46: границы шапки НА ОДНОЙ вертикали со строками (colX-3; было colX-1), +передняя у ON и правая у AUX DEPTH (1.6.34)
        if (sortColumn >= 1 && sortColumn <= 7) { // pixel triangle, font-safe (1.6.8/41)
            g.setColour(ink.withAlpha(0.5f));
            const int ax = colX(sortColumn) + colWidth(sortColumn) - 18;
            for (int i = 0; i < 4; ++i) g.fillRect(ax + i * 2, sortDescending ? 34 + i * 2 : 42 - i * 2, 8 - i * 2, 2);
        }
        for (int r = 0; r < visibleRows; ++r) { // 1.7.9: рисуем только видимые строки
            const int y = rowY(r); g.setColour(ink.withAlpha(0.5f)); g.drawHorizontalLine(y + 31, 8.0f, static_cast<float>(colX(7) + colWidth(7) + 2)); // 1.6.46: строка кончается у последней колонки
            g.setColour(ink); pixel::text(g, juce::String(r + 1), {colX(0), y + 7, colWidth(0) - 4, 18}, 12, true); // 1.7.5: нумерация строк перед ONравой границы AUX DEPTH (1.6.44: 987)
            for (int c = 1; c <= 5; ++c) {
                auto* par = cell(r, c); if (!par) continue; const int value = juce::roundToInt(par->convertFrom0to1(par->getValue()));
                if (c == 1) { drawCheck(g, colX(c) + 10, y + 7, value > 0); continue; }
                juce::String text = c == 2 ? sourceName(value) : c == 3 ? (value == 0 ? juce::String("---") : destName(value - 1)) : c == 4 ? signedPercent(value) : polarityName(value); // 1.6.32: 0 = OFF; 1.7.5: ROUTE-цели -- «N - что модулирует»
                // 1.6.32: прочерки = значение 0 (OFF) независимо от галки ON -- "мимо первого параметра" больше не проскакивает
                if (c == 4) { // 1.6.28: контур + СЕРАЯ полоса заполнения + белая кромка (общий серый фон ячейки убран)
                    const int bw = colWidth(4) - 10, bx = colX(4) + 5, by = y + 2, bh = 25, mid = bx + bw / 2;
                    const int fw = juce::roundToInt(static_cast<float>(bw) * 0.5f * (std::abs(value) / 64.0f));
                    g.setColour(ink.withAlpha(0.45f)); g.drawRect(bx, by, bw, bh, 1);
                    if (fw > 0) { if (value >= 0) g.fillRect(mid, by, fw, bh); else g.fillRect(mid - fw, by, fw, bh); }
                    g.setColour(juce::Colours::white);
                    if (fw > 0) { const int edge = value >= 0 ? mid + fw : mid - fw; g.drawVerticalLine(edge, float(by + 1), float(by + bh - 1)); }
                    g.drawVerticalLine(mid, float(by + 1), float(by + bh - 1)); }
                // 1.6.13: параметры в DEST-колонке по центру -- сразу видно, какая строка короче.
                pixel::text(g, text, {colX(c) + 5, y + 2, colWidth(c) - (c == 3 ? 44 : 8), 25}, c == 3 ? 14 : 16, c == 4); // 1.6.32: DEST -- 14px у всех имён
                if (c == 3) { // 1.6.12: замок маршрута слева от прицела
                    const int lockMode = lockValue(r);
                    drawLock(g, colX(c) + colWidth(c) - 40, y + 7, lockMode); // 1.6.32: компактнее -- тексту DEST больше места
                    drawCrosshair(g, colX(c) + colWidth(c) - 14, y + 15); // 1.6.32
                }
            }
            { // AUX uses source+one: zero is OFF and 1..36 map to the 36 sources.
                auto* ap = processor.parameters.getRawParameterValue("r" + juce::String(r) + "_aux");
                const int aux = ap ? juce::jlimit(0, 36, juce::roundToInt(ap->load())) : 0;
                // 1.6.46: отдельный тусклый разделитель AUX убран -- общий ряд границ на colX(c)-3
                g.setColour(ink); pixel::text(g, aux == 0 ? juce::String("OFF") : sourceName(aux - 1), {colX(6) + 6, y + 2, colWidth(6) - 10, 25}, 14, false);
            }
            { // 1.6.32: AUX DEPTH -- вклад AUX (±100%), 0 = не влияет (по умолчанию)
                auto* dp = processor.parameters.getRawParameterValue("r" + juce::String(r) + "_aux_depth");
                const int dv = dp ? juce::jlimit(-64, 63, juce::roundToInt(dp->load())) : 0;
                const int bw = colWidth(7) - 10, bx = colX(7) + 5, by = y + 2, bh = 25, mid = bx + bw / 2;
                const int fw = juce::roundToInt(static_cast<float>(bw) * 0.5f * (std::abs(dv) / 64.0f));
                g.setColour(ink.withAlpha(0.45f)); g.drawRect(bx, by, bw, bh, 1);
                if (fw > 0) { if (dv >= 0) g.fillRect(mid, by, fw, bh); else g.fillRect(mid - fw, by, fw, bh); }
                g.setColour(juce::Colours::white);
                if (fw > 0) { const int edge = dv >= 0 ? mid + fw : mid - fw; g.drawVerticalLine(edge, float(by + 1), float(by + bh - 1)); }
                g.drawVerticalLine(mid, float(by + 1), float(by + bh - 1));
                pixel::text(g, signedPercent(dv), {bx, by, bw, bh}, 14, true);
            }
            if (flashLockOnly == r) { g.setColour(ink); for (int k = 0; k < 3; ++k) g.drawRect(colX(3) + colWidth(3) - 50, y + 3 + k, 18 - k * 2, 20 - k * 2, 1); }
            g.setColour(ink.withAlpha(0.55f)); for (int c = 1; c < 8; ++c) g.drawVerticalLine(colX(c) - 3, float(y), float(y + 31)); g.drawVerticalLine(colX(7) + colWidth(7) + 2, float(y), float(y + 31)); // 1.6.46: ВСЕ границы colX(c)-3 = ровно под шапкой; +правый край AUX DEPTH (1.6.30: линии колонок ярче)
        }
        flashLockOnly = -1;
        if (flashDestRow >= 0) { const juce::uint32 ms = juce::Time::getMillisecondCounter(); // 1.6.31: рамка DEST мигает x2
            if (ms > flashDestUntil) flashDestRow = -1;
            else if (((ms / 330) & 1) == 0) { g.setColour(ink); g.drawRect(colX(3) + 1, rowY(flashDestRow) + 1, colWidth(3) - 2, 29, 2); } }
        g.setColour(juce::Colours::white); g.drawRect(8, 26, 1120, visibleRows * 34 + 36, 1); // 1.6.25/1.7.7: рамка матрицы на 64 строки
        g.setColour(ink); pixel::text(g, "DEPTH 0..100%   UNIPOLAR = ONE SIDE   BIPOLAR = TWO SIDES   GRAB EMPTY LEFT EDGE TO REORDER   LOCK: LMB FREE/LOCK, RMB SOLO", {10, getHeight() - 24, 1115, 17}, 11, false); // 1.6.14: белая + альтернативы замка
        if (aimDragRow >= 0) { // 1.7.5: ПКМ-патчкорд из прицела -- дырка-гнездо на месте прицела, провод как у LFO-джека
            const int jx = colX(3) + colWidth(3) - 14, jy = rowY(aimDragRow) + 15;
            juce::Path p; p.startNewSubPath({float(jx), float(jy)});
            p.quadraticTo({(jx + aimDragPos.x) * 0.5f, std::max(jy, aimDragPos.y) + 40.0f}, juce::Point<float>(float(aimDragPos.x), float(aimDragPos.y)));
            g.setColour(juce::Colours::white); g.strokePath(p, juce::PathStrokeType(3.0f)); g.fillEllipse(float(aimDragPos.x) - 5.0f, float(aimDragPos.y) - 5.0f, 10.0f, 10.0f);
            g.drawEllipse(float(jx) - 6.0f, float(jy) - 6.0f, 12.0f, 12.0f, 1.0f); }
        if (aimHoverRow >= 0) { g.setColour(juce::Colours::white); const int bx = aimHoverCol == 4 ? colX(4) + 5 : colX(7) + 5, by = rowY(aimHoverRow) + 2; g.drawRect(bx - 2, by - 2, (aimHoverCol == 4 ? colWidth(4) - 10 : colWidth(7) - 10) + 4, 29, 2); } // 1.7.5: подсветка DEPTH/AUX DEPTH как у ручек лицевой панели
    }

    void mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& w) override { if (aimDragRow >= 0) { const float d = w.isReversed ? -w.deltaY : w.deltaY; if (auto* vp = findParentComponentOfClass<juce::Viewport>()) vp->setViewPosition(vp->getViewPositionX(), juce::jlimit(0, juce::jmax(0, getHeight() - vp->getViewHeight()), vp->getViewPositionY() - juce::roundToInt(d * 56.0f))); return; } // 1.7.6: в режиме патчкорда колесо листает матрицу -- режим не отменяется, параметр не крутится
        const int r = rowAt(e.y), c = colAt(e.x); if (r < 0 || c < 1 || std::abs(w.deltaY) < 0.0001f) return; auto* p = c == 6 ? processor.parameters.getParameter("r" + juce::String(r) + "_aux") : c == 7 ? processor.parameters.getParameter("r" + juce::String(r) + "_aux_depth") : cell(r, c); if (!p) return; const int old = juce::roundToInt(p->convertFrom0to1(p->getValue())); int lo = 0, hi = choiceCount(p) - 1; if (c == 4 || c == 7) { lo = -64; hi = 63; } else if (c == 5) hi = 1; else if (c == 6) hi = 18; else if (c == 3) hi = 248; // OFF plus stable target IDs 0..247
        p->beginChangeGesture(); p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(juce::jlimit(lo, hi, old + (w.deltaY > 0 ? 1 : -1))))); p->endChangeGesture(); repaint(); }
    void mouseMove(const juce::MouseEvent& e) override { // 1.6.14: подсказка при наведении на замок
        if (e.y < 56) { const int hc = colAt(e.x); setMouseCursor(hc >= 1 && std::abs(e.x - colX(hc)) <= 3 ? juce::MouseCursor::LeftRightResizeCursor : juce::MouseCursor::NormalCursor); } // 1.6.46: и перед ON (1.6.41: курсор ресайза у границы)
        else setMouseCursor(juce::MouseCursor::NormalCursor);
        const int r = rowAt(e.y); const bool lockZone = r >= 0 && e.x >= colX(3) + colWidth(3) - 50 && e.x < colX(3) + colWidth(3) - 22; // 1.6.32: хитбокс замка под компактную иконку
        const int zone = lockZone ? r + 1 : 0; if (zone != hoverLock) { hoverLock = zone; repaint(); }
    }
    void mouseDown(const juce::MouseEvent& e) override {
        if (e.y < 56) { const int c = colAt(e.x);
            if (c >= 1 && std::abs(e.x - (colX(c) - 3)) <= 6) { colDrag = c; colDragX = e.x; return; } // 1.7.9 FIX: хитбокс шире (съезжавшие клики)ая линия разделителя (она на colX-3) полосу номеров строк = сдвиг старта колонок от начала матрицы) (1.6.41: ресайз колонки -- тянуть границу в шапке)
            if (c >= 1 && c <= 7) sortBy(c); return; } // 1.6.8/41: ON..AUX DEPTH сортируются
        const int r = rowAt(e.y), c = colAt(e.x); if (r < 0 || c < 0) return; // 1.6.31: драг за колонками (края матрицы) ничего не меняет
        hitWasRmb = e.mods.isPopupMenu(); // 1.6.23: список значений -- только по ПКМ
        if (c == 0) { dragRow = r; dragTarget = r; return; }
        if (c == 1) { onPaintWasRmb = e.mods.isPopupMenu(); onPaint = onPaintWasRmb ? 0 : (cell(r, 1) && juce::roundToInt(cell(r, 1)->convertFrom0to1(cell(r, 1)->getValue())) > 0 ? 0 : 1); setOnPaint(r); return; } // 1.6.22: кисть ON -- ЛКМ красит, ПКМ стирает
        if (c == 3 && e.x >= colX(c) + colWidth(c) - 50 && e.x < colX(c) + colWidth(c) - 22) { // 1.6.32: границы под компактную иконку замка
            // 1.6.21: замки рисуются ПРОВЕДЕНИЕМ: первый замок задаёт кисть
            // (ЛКМ -- toggle в LOCK/FREE, ПКМ -- SOLO), дальше кисть красит все
            // строки под курсором. ПКМ без движения -- меню режимов (на отпускании).
            lockPaintWasRmb = e.mods.isPopupMenu();
            lockPaint = lockPaintWasRmb ? 2 : (lockValue(r) > 0 ? 0 : 1);
            lockPaintRow = r; setLock(r, lockPaint); return;
        }
        if (c == 3 && e.x >= colX(c) + colWidth(c) - 22) { // 1.6.31/32: прицел по кресту; замок маршрута НЕ мешает прицелу
            if (e.mods.isPopupMenu()) { aimDragRow = r; aimDragPos = {e.x, e.y}; aimHoverRow = -1; aimHoverCol = 0; repaint(); return; } // 1.7.5: ПКМ -- патчкорд В матрице (остаёмся в матрице)
            if (onTargetPick) onTargetPick(r); return;
        }
        if (c == 2 || c == 3 || c == 5 || c == 6 || c == 7) { // 1.6.25/32: клик = список (кроме DEPTH/AUX DEPTH -- там драг), ЛКМ-драг = листать значение
            hitWasRmb = e.mods.isPopupMenu();
            hitRow = r; hitCol = c; hitY = static_cast<int>(e.getScreenPosition().y); hitX = static_cast<int>(e.getScreenPosition().x); dragged = false; // 1.6.33: hitX обязателен -- без него драг AUX DEPTH брал мусор и бросало в +100
            auto* par = c == 6 ? processor.parameters.getParameter("r" + juce::String(r) + "_aux") : c == 7 ? processor.parameters.getParameter("r" + juce::String(r) + "_aux_depth") : cell(r, c);
            hitBase = par ? juce::roundToInt(par->convertFrom0to1(par->getValue())) : 0;
            if (!hitWasRmb && par) { gesture = true; par->beginChangeGesture(); }
            return; }
        if (!e.mods.isLeftButtonDown()) return; // 1.6.22: SRC/DEST/DEPTH/MODE -- только ЛКМ (ПКМ = альт-режимы и списки)
        hitRow = r; hitCol = c; hitY = static_cast<int>(e.getScreenPosition().y); hitX = static_cast<int>(e.getScreenPosition().x); hitBase = cell(r, c) ? juce::roundToInt(cell(r, c)->convertFrom0to1(cell(r, c)->getValue())) : 0; dragged = false; if (auto* p = cell(r, c)) { gesture = true; p->beginChangeGesture(); }
    }
    void mouseDoubleClick(const juce::MouseEvent& e) override { // 1.6.24/32: двойной ЛКМ = сброс (DEPTH/MODE -> 0, DEST -> OFF, AUX DEPTH -> 0)
        if (!e.mods.isLeftButtonDown()) return;
        const int r = rowAt(e.y), c = colAt(e.x); if (r < 0 || (c != 3 && c != 4 && c != 5 && c != 7)) return;
        auto* p = c == 7 ? processor.parameters.getParameter("r" + juce::String(r) + "_aux_depth") : cell(r, c); if (!p) return;
        p->beginChangeGesture(); p->setValueNotifyingHost(p->convertTo0to1(0.0f)); p->endChangeGesture(); repaint(); }
    void mouseDrag(const juce::MouseEvent& e) override {
        if (aimDragRow >= 0) { aimDragPos = {e.x, e.y}; const int rr = rowAt(e.y), cc = colAt(e.x); const int nh = (rr >= 0 && (cc == 4 || cc == 7) && rr != aimDragRow) ? rr : -1; const int nc = nh >= 0 ? cc : 0; if (nh != aimHoverRow || nc != aimHoverCol) { aimHoverRow = nh; aimHoverCol = nc; } repaint(); return; } // 1.7.5: тянут патчкорд -- подсветка DEPTH/AUX DEPTH
        if (colDrag > 0) { const int nx = static_cast<int>(e.getScreenPosition().x); colw[(size_t)colDrag - 1] = juce::jlimit(colMin(colDrag - 1), 280, colw[(size_t)colDrag - 1] + nx - colDragX); colDragX = nx; normalizeCols(); repaint(); return; } // 1.7.5: текст колонки не схлопывается (минимум по колонке) // 1.6.41: ресайз колонки, правый край неподвижен
        if (onPaint >= 0) { dragged = true; const int rr = rowAt(e.y); if (rr >= 0) setOnPaint(rr); return; } // 1.6.22: рисование галочек ON
        if (lockPaint >= 0) { dragged = true; const int rr = rowAt(e.y); if (rr >= 0 && rr != lockPaintRow) { setLock(rr, lockPaint); lockPaintRow = rr; } return; } // 1.6.21: рисование замков
        if (dragRow >= 0) { dragTarget = juce::jlimit(0, 15, rowAt(e.y)); repaint(); return; }
        if (hitRow < 0) return; const int dy = hitY - static_cast<int>(e.getScreenPosition().y); if (std::abs(dy) > 2) dragged = true;
        if (!e.mods.isLeftButtonDown()) return; // 1.6.25: ПКМ-драг -- ни правки, ни списка после отпускания
        auto* p = hitCol == 6 ? processor.parameters.getParameter("r" + juce::String(hitRow) + "_aux") : hitCol == 7 ? processor.parameters.getParameter("r" + juce::String(hitRow) + "_aux_depth") : cell(hitRow, hitCol); if (!p) return;
        if (hitCol == 4 || hitCol == 7) { const int dx = static_cast<int>(e.getScreenPosition().x) - hitX; const int amount = e.mods.isShiftDown() ? dx / 10 : dx; p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(juce::jlimit(-64, 63, hitBase + amount)))); } // 1.6.25/32: DEPTH и AUX DEPTH -- горизонтальный ЛКМ-драг
        else { const int hi = hitCol == 3 ? 248 : hitCol == 5 ? 1 : hitCol == 6 ? 32 : 17; p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(juce::jlimit(0, hi, hitBase + dy / 8)))); } // 1.6.32: DEST -- 0..64 (0 = OFF)
        repaint();
    }
    void mouseUp(const juce::MouseEvent& e) override {
        if (aimDragRow >= 0) { const int r = rowAt(e.y), c = colAt(e.x); const int s = aimDragRow; aimDragRow = -1; aimHoverRow = -1; // 1.7.5: drop ПКМ-патчкорда -- та же логика назначения, что у ЛКМ-прицела
            if (r >= 0 && (c == 4 || c == 7) && r != s) { // DEPTH или AUX DEPTH ДРУГОЙ строки = её глубина (dest = ROUTE r+1)
                auto* d = cell(s, 3); if (d) { const bool wasEmpty = juce::roundToInt(d->convertFrom0to1(d->getValue())) == 0;
                    if (pushEdit) pushEdit(); d->beginChangeGesture(); d->setValueNotifyingHost(d->convertTo0to1(static_cast<float>(67 + r + 1))); d->endChangeGesture();
                    if (wasEmpty) processor.moveModRoute(s, 0); // новый маршрут всплывает наверх
                    flashDestCell(wasEmpty ? 0 : s); } } // мигание DEST-ячейки источника
            repaint(); return; }
        if (colDrag > 0) { uiLayout().setArr("matrix.colw", colw, 8); colDrag = 0; } // 1.6.41: запомнить ширины колонок
        if (onPaint >= 0) { onPaint = -1; dragged = false; return; } // 1.6.22: конец кисти ON
        if (lockPaint >= 0) { const bool wasRmb = lockPaintWasRmb; const bool moved = dragged; lockPaint = -1; dragged = false;
            if (wasRmb && !moved) { // 1.6.21: ПКМ-клик по замку без движения = меню режимов (у курсора)
                const int r = rowAt(e.y);
                if (r >= 0) { juce::PopupMenu m; const int cur = lockValue(r);
                    m.setLookAndFeel(&getLookAndFeel());
                    m.addItem(1, "FREE (normal)", true, cur == 0);
                    m.addItem(2, "LOCK (aim/re-write cannot touch dest)", true, cur == 1);
                    m.addItem(3, "SOLO (pinned to this parameter)", true, cur == 2);
                    const juce::Component::SafePointer<MatrixPage> safe(this);
                    m.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(this).withTargetScreenArea(juce::Rectangle<int>(e.getScreenPosition().x, e.getScreenPosition().y, 1, 1)), [safe, r](int res) { if (safe && res > 0) safe->setLock(r, res - 1); });
                } }
            return; }
        if (dragRow >= 0) { if (dragTarget >= 0 && dragTarget != dragRow) { if (pushEdit) pushEdit(); processor.moveModRoute(dragRow, dragTarget); } dragRow = dragTarget = -1; repaint(); return; }
        if (gesture) { if (auto* gp = hitCol == 6 ? processor.parameters.getParameter("r" + juce::String(hitRow) + "_aux") : hitCol == 7 ? processor.parameters.getParameter("r" + juce::String(hitRow) + "_aux_depth") : cell(hitRow, hitCol)) gp->endChangeGesture(); gesture = false; } // 1.6.25/32
        const bool wasDrag = dragged; const int r = hitRow, c = hitCol; hitRow = hitCol = -1; dragged = false;
        if (wasDrag || r < 0) return;
        if (c == 4) { if (hitWasRmb) openValueMenu(r, 4, e.getScreenPosition()); return; } // 1.6.24: ПКМ по DEPTH = список значений
        if (c == 7) { openAuxDepthMenu(r, e.getScreenPosition()); return; } // 1.6.33: клик (ЛКМ/ПКМ) = список значений; горизонтальный драг = +-100
        if (c == 5) { if (!hitWasRmb) { if (auto* p = cell(r, 5)) toggle(*p); return; } openValueMenu(r, 5, e.getScreenPosition()); return; } // 1.6.32: UNI/BIP -- клик переключает, список по ПКМ
        if (c == 2 || c == 3) openValueMenu(r, c, e.getScreenPosition()); // 1.6.25: SOURCE/DEST -- список по клику
        else if (c == 6) openAuxMenu(r, e.getScreenPosition()); // 1.6.25: AUX SOURCE -- список по клику
    }
    void openValueMenu(int r, int c, juce::Point<int> screen) { // 1.6.24: единый список значений ячейки
        auto* par = cell(r, c); if (!par) return; juce::PopupMenu menu;
        menu.setLookAndFeel(&getLookAndFeel());
        if (c == 4) { const int values[]{-64, -48, -32, -16, 0, 16, 32, 48, 63}; for (int i = 0; i < 9; ++i) menu.addItem(i + 1, signedPercent(values[i])); } // -100% ... 0% ... +100%
        else if (c == 5) { const int selected = juce::roundToInt(par->convertFrom0to1(par->getValue())); for (int i = 0; i < 2; ++i) menu.addItem(i + 1, polarityName(i), true, i == selected); } // 1.6.25: UNIPOLAR/BIPOLAR
        else if (c == 3) {
            // One unambiguous menu-ID encoding (10000 + stable target ID).
            // Values in the parameter itself remain target+1, with zero OFF.
            const int selected=juce::roundToInt(par->convertFrom0to1(par->getValue()));
            auto addTarget=[&](juce::PopupMenu& dst,int target){dst.addItem(10000+target,targetName(processor,target),true,selected==target+1);};
            auto addRange=[&](juce::PopupMenu& dst,int first,int count){for(int i=0;i<count;++i)addTarget(dst,first+i);};
            menu.addItem(9000,"OFF",true,selected==0);
            {juce::PopupMenu p1;juce::PopupMenu synt;addRange(synt,0,8);addTarget(synt,nova::kPitchMatrixTarget);p1.addSubMenu("SYNT",synt);
                juce::PopupMenu amp;addRange(amp,8,8);p1.addSubMenu("AMP",amp);
                juce::PopupMenu filt;addRange(filt,16,8);p1.addSubMenu("FILT",filt);
                juce::PopupMenu effx;addRange(effx,24,8);p1.addSubMenu("EFFX",effx);menu.addSubMenu("P1 PARAM",p1);}
            {juce::PopupMenu p1l;for(int i=0;i<3;++i){juce::PopupMenu l;addRange(l,32+i*8,8);p1l.addSubMenu("LFO"+juce::String(i+1),l);}for(int i=0;i<3;++i){juce::PopupMenu l;addRange(l,188+i*8,8);p1l.addSubMenu("LFO"+juce::String(i+4),l);}menu.addSubMenu("P1 LFO",p1l);}
            {juce::PopupMenu p2;juce::PopupMenu synt;addRange(synt,132,8);p2.addSubMenu("SYNT",synt);
                juce::PopupMenu amp;addRange(amp,140,8);p2.addSubMenu("AMP",amp);
                juce::PopupMenu filt;addRange(filt,148,8);p2.addSubMenu("FILT",filt);
                juce::PopupMenu effx;addRange(effx,156,8);p2.addSubMenu("EFFX",effx);menu.addSubMenu("P2 PARAM",p2);}
            {juce::PopupMenu p2l;for(int i=0;i<3;++i){juce::PopupMenu l;addRange(l,164+i*8,8);p2l.addSubMenu("LFO"+juce::String(i+1),l);}for(int i=0;i<3;++i){juce::PopupMenu l;addRange(l,212+i*8,8);p2l.addSubMenu("LFO"+juce::String(i+4),l);}menu.addSubMenu("P2 LFO",p2l);}
            {juce::PopupMenu arp;addTarget(arp,64);addTarget(arp,65);addTarget(arp,244);addTarget(arp,245);menu.addSubMenu("ARP",arp);}
            {juce::PopupMenu plockPattern;addTarget(plockPattern,246);addTarget(plockPattern,247);menu.addSubMenu("P-LOCK PATTERN",plockPattern);}
            {juce::PopupMenu mseg;addRange(mseg,56,8);menu.addSubMenu("MSEG OUT",mseg);}
            {juce::PopupMenu msegRate;addRange(msegRate,236,8);menu.addSubMenu("MSEG RATE",msegRate);}
            {juce::PopupMenu legacy;addTarget(legacy,131);menu.addSubMenu("LEGACY",legacy);}
            {juce::PopupMenu routes;for(int n=0;n<64;++n)addTarget(routes,67+n);menu.addSubMenu("MATRIX ROUTE DEPTH",routes);}
        }
        else if (c == 2) { // source IDs are stable; MOD ENV is appended at 32..35.
            const int selected = juce::roundToInt(par->convertFrom0to1(par->getValue()));
            auto addSource=[&](juce::PopupMenu& target,int source){ target.addItem(1000 + source,sourceName(source),true,selected == source); };
            addSource(menu,17); // RANDOM stays immediately available.
            juce::PopupMenu midi; for(const int v:{0,1,7,8,9})addSource(midi,v);
            juce::PopupMenu lfom; for(const int v:{4,5,6,23,24,25})addSource(lfom,v);
            juce::PopupMenu p2l; for(int v=26;v<=31;++v)addSource(p2l,v);
            juce::PopupMenu arp; for(const int v:{13,14,15,16})addSource(arp,v);
            juce::PopupMenu msegm; for(const int v:{10,11,12,18,19,20,21,22})addSource(msegm,v);
            juce::PopupMenu mac; for(const int v:{2,3})addSource(mac,v);
            juce::PopupMenu modenv; for(int v=32;v<=35;++v)addSource(modenv,v);
            menu.addSubMenu("MIDI",midi);menu.addSubMenu("LFO",lfom);menu.addSubMenu("P2 LFO",p2l);menu.addSubMenu("ARP",arp);menu.addSubMenu("MSEG",msegm);menu.addSubMenu("MACRO",mac);menu.addSubMenu("MOD ENV",modenv);
        }
        const juce::Component::SafePointer<MatrixPage> safe(this);
        // 1.8.0b FIX: список У МЫШКИ (было намертво 120,227)
        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(this).withTargetScreenArea(juce::Rectangle<int>(screen.x, screen.y, 1, 1)), [safe, r, c](int result) {
            if(!safe||result<=0)return;auto* p=safe->cell(r,c);if(!p)return;if(safe->pushEdit)safe->pushEdit();
            p->beginChangeGesture();
            if(c==4){const int values[]{-64,-48,-32,-16,0,16,32,48,63};p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(values[result-1])));}
            else if(c==2&&result>=1000)p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(result-1000)));
            else if(c==3&&result==9000)p->setValueNotifyingHost(p->convertTo0to1(0.0f));
            else if(c==3&&result>=10000&&result<=10247)p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(result-9999))); // stored value = target + 1
            else p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(result-1)));
            p->endChangeGesture();
        });
    }
    void openAuxMenu(int r, juce::Point<int> screen) { // AUX = OFF or one of the 36 source IDs plus one.
        juce::PopupMenu m; m.setLookAndFeel(&getLookAndFeel());
        auto* par=processor.parameters.getParameter("r"+juce::String(r)+"_aux");
        const int cur=par?juce::jlimit(0,36,juce::roundToInt(par->convertFrom0to1(par->getValue()))):0;
        m.addItem(1000,"OFF",true,cur==0);
        auto addSource=[&](juce::PopupMenu& target,int source){target.addItem(1001+source,sourceName(source),true,cur==source+1);};
        addSource(m,17);
        juce::PopupMenu midi;for(const int v:{0,1,7,8,9})addSource(midi,v);
        juce::PopupMenu lfo;for(const int v:{4,5,6,23,24,25})addSource(lfo,v);
        juce::PopupMenu p2l;for(int v=26;v<=31;++v)addSource(p2l,v);
        juce::PopupMenu arp;for(const int v:{13,14,15,16})addSource(arp,v);
        juce::PopupMenu mseg;for(const int v:{10,11,12,18,19,20,21,22})addSource(mseg,v);
        juce::PopupMenu mac;for(const int v:{2,3})addSource(mac,v);
        juce::PopupMenu modenv;for(int v=32;v<=35;++v)addSource(modenv,v);
        m.addSubMenu("MIDI",midi);m.addSubMenu("LFO",lfo);m.addSubMenu("P2 LFO",p2l);m.addSubMenu("ARP",arp);m.addSubMenu("MSEG",mseg);m.addSubMenu("MACRO",mac);m.addSubMenu("MOD ENV",modenv);
        const juce::Component::SafePointer<MatrixPage> safe(this);
        m.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(this).withTargetScreenArea(juce::Rectangle<int>(screen.x,screen.y,1,1)),[safe,r](int result){
            if(!safe||result<1000)return;
            if(safe->pushEdit)safe->pushEdit();
            if(auto* p=safe->processor.parameters.getParameter("r"+juce::String(r)+"_aux")){
                const int value=result-1000; // OFF=0; selected source n becomes n+1.
                p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(value)));p->endChangeGesture();
                if(value>0)if(auto* d=safe->processor.parameters.getParameter("r"+juce::String(r)+"_aux_depth")){const int depth=juce::roundToInt(d->convertFrom0to1(d->getValue()));if(depth==0){d->beginChangeGesture();d->setValueNotifyingHost(d->convertTo0to1(63.0f));d->endChangeGesture();}}
            }
        });
    }
    void openAuxDepthMenu(int r, juce::Point<int> screen) { // 1.6.33: AUX DEPTH -- компактный список ПОД ячейкой (рамка, имена, сортировка по значению)
        juce::PopupMenu m; m.setLookAndFeel(&smallMenuLaf());
        const int values[]{-64, -48, -32, -16, 0, 16, 32, 48, 63};
        auto* par = processor.parameters.getParameter("r" + juce::String(r) + "_aux_depth"); const int cur = par ? juce::roundToInt(par->convertFrom0to1(par->getValue())) : 0;
        for (int i = 0; i < 9; ++i) m.addItem(i + 1, signedPercent(values[i]), true, values[i] == cur);
        const juce::Component::SafePointer<MatrixPage> safe(this);
        const auto below = localPointToGlobal(juce::Point<int>(colX(7), rowY(r) + 30));
        m.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(this).withTargetScreenArea(juce::Rectangle<int>(below.x, below.y, colWidth(7), 1)), [safe, r](int result) { if (!safe || result <= 0) return; if (safe->pushEdit) safe->pushEdit(); if (auto* p = safe->processor.parameters.getParameter("r" + juce::String(r) + "_aux_depth")) { const int values2[]{-64, -48, -32, -16, 0, 16, 32, 48, 63}; p->beginChangeGesture(); p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(values2[result - 1]))); p->endChangeGesture(); } });
    }
private:
    juce::String destName(int id) const { if (id >= 133) return targetName(processor, id - 1); if (id == 132) return targetName(processor, 131); if (id >= 67 && id < 132) { auto* dp = processor.parameters.getRawParameterValue("r" + juce::String(id - 67) + "_dest"); const int dv = dp ? juce::roundToInt(dp->load()) : 0; return juce::String(id - 66) + " - " + (dv > 1 ? targetName(processor, juce::jlimit(0, 247, dv - 1)) : juce::String("OFF")); } return targetName(processor, id); } // 1.7.7: строки-маршруты 1..64 // 1.8.0e FIX: value 131 = ROUTE 64 (лез "LFO FM"); 132 = legacy LFO FM; 133+ = P2 и LFO-строки
    static int choiceCount(juce::RangedAudioParameter* p) { if (!p) return 0; const auto n = p->getAllValueStrings(); return n.size() > 0 ? n.size() : juce::roundToInt(p->convertFrom0to1(1.0f)) + 1; }
    static juce::String sourceName(int v) { const char* n[]{"KEY", "VEL", "MACRO X", "MACRO Y", "LFO1", "LFO2", "LFO3", "PITCH WHL", "MOD WHL", "AFTERTOUCH", "MSEG1", "MSEG2", "MSEG3", "STEP VEL", "STEP TRANS", "ARP GATE", "ARP RATE", "RANDOM", "MSEG4", "MSEG5", "MSEG6", "MSEG7", "MSEG8", "LFO4", "LFO5", "LFO6", "P2 LFO1", "P2 LFO2", "P2 LFO3", "P2 LFO4", "P2 LFO5", "P2 LFO6", "MOD ENV1", "MOD ENV2", "MOD ENV3", "MOD ENV4"}; return n[juce::jlimit(0,35,v)]; }
    static juce::String polarityName(int v) { return v == 1 ? "BIPOLAR" : "UNIPOLAR"; }
    // 1.6.12: замок маршрута: 0 FREE, 1 LOCK (прицел/перезапись не трогают),
    // 2 SOLO (приколот к этому параметру; руками менять можно, замок висит).
    int lockValue(int r) const { auto* p = processor.parameters.getParameter("r" + juce::String(r) + "_lock"); return p ? juce::jlimit(0, 2, juce::roundToInt(p->convertFrom0to1(p->getValue()))) : 0; }
    PixelButton clearButton{"CLEAR"}; // 1.6.20: стереть все маршруты разом
    void setOnPaint(int r) { if (pushEdit) pushEdit(); if (auto* p = cell(r, 1)) { p->beginChangeGesture(); p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(juce::jlimit(0, 1, onPaint)))); p->endChangeGesture(); } repaint(); } // 1.6.22: кисть галочек ON
    int onPaint = -1; bool onPaintWasRmb = false; // 1.6.22
    void setLock(int r, int mode) { if (pushEdit) pushEdit(); if (auto* p = processor.parameters.getParameter("r" + juce::String(r) + "_lock")) { p->beginChangeGesture(); p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(juce::jlimit(0, 2, mode)))); p->endChangeGesture(); } repaint(); }
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
    int colw[8]{15, 52, 165, 165, 150, 124, 120, 148}; // 1.6.44: ширины из JSON пользователя (сумма 939) -- ресайз за границу в шапке (1.6.41); сумма 939 (+7 зазоров по 5 = правый край 1128)
    void normalizeCols(){ int w7=939; for(int i=0;i<7;++i)w7-=colw[(size_t)i]; int over=w7<30?30-w7:0; colw[7]=juce::jlimit(30,280,w7); // 1.6.41/43: последняя >= 30, лишнее поджимает предыдущие (не выталкиваются за край)
        for(int i=6;i>=0&&over>0;--i){ const int take=juce::jmin(over,colw[(size_t)i]-18); colw[(size_t)i]-=take; over-=take; } }
    int colX(int c) const { int x=13; const int n=juce::jlimit(0,7,c); for(int i=0;i<n;++i)x+=colw[(size_t)i]+5; return x; } // 1.6.44: колонки от ЛЕВОЙ границы (в окне = 85,133; было 154) -- 1.6.41: накопительные позиции от ширин
    int colWidth(int c) const { return colw[(size_t)juce::jlimit(0,7,c)]; } // 1.6.41
    int rowY(int r) const { return 58 + r * 34; }
    int colAt(int x) const { for (int c = 0; c < 8; ++c) if (x >= colX(c) && x < colX(c) + colWidth(c)) return c; return -1; } // 1.6.32
    int rowAt(int y) const { if (y < 58) return -1; const int r = (y - 58) / 34; return r >= 0 && r < visibleRows ? r : -1; } // 1.7.9
    juce::RangedAudioParameter* cell(int r, int c) { return cells[static_cast<size_t>(r * 5 + (c - 1))]; }
    int colDrag = 0, colDragX = 0; // 1.6.41: ресайз колонок матрицы
    int colMin(int i) const { static const int mins[]{15, 26, 110, 110, 80, 70, 64, 70}; return mins[juce::jlimit(0, 7, i)]; } // 1.7.5: минимум ширины -- текст не схлопывается
    int flashLockOnly = -1; int hoverLock = 0; int lockPaint = -1, lockPaintRow = -1; bool lockPaintWasRmb = false; bool hitWasRmb = false; // 1.6.23
    void toggle(juce::RangedAudioParameter& p) { p.beginChangeGesture(); p.setValueNotifyingHost(p.convertTo0to1(p.convertFrom0to1(p.getValue()) > 0.5f ? 0.0f : 1.0f)); p.endChangeGesture(); }
    void sortBy(int c) { if (sortColumn == c) sortDescending = !sortDescending; else { sortColumn = c; sortDescending = (c == 4 || c == 1); } processor.sortModRoutes(c, sortDescending); repaint(); } // 1.6.8: ON column sorts enabled-first by default
    void timerCallback() override { // 1.7.9: занятый слот добавляет строку в видимость (макс -- занятые + 1 свободный)
        int maxUsed = -1;
        for (int r = 0; r < 64; ++r) { auto* d = processor.parameters.getRawParameterValue("r" + juce::String(r) + "_dest");
            auto* dp = processor.parameters.getRawParameterValue("r" + juce::String(r) + "_depth");
            if ((d && juce::roundToInt(d->load()) > 0) || (dp && std::abs(dp->load()) > 0.5f)) maxUsed = r; }
        const int want = juce::jlimit(16, 64, maxUsed + 2);
        if (want != visibleRows) { visibleRows = want; setSize(getWidth(), 58 + visibleRows * 34 + 40); }
        repaint(); }
    MonomachineNovaAudioProcessor& processor; int visibleRows = 16; // 1.7.9: занятые слоты + 1 свободный, минимум 16 (без скролла)
    std::array<juce::RangedAudioParameter*, 320> cells{}; int hitRow = -1, hitCol = -1, hitY = 0, hitX = 0, hitBase = 0, dragRow = -1, dragTarget = -1, sortColumn = -1; bool gesture = false, dragged = false, sortDescending = false; // 1.7.7: cells 64x5
    int aimDragRow = -1, aimHoverRow = -1, aimHoverCol = 0; juce::Point<int> aimDragPos; // 1.7.5: ПКМ-патчкорд из прицела матрицы в DEPTH/AUX DEPTH
};

// 1.6.22: список с колесом мыши -- крутя колесо, листаем предустановки/сетку
// (виджет внутри вьюпорта раньше отдавал колесо прокрутке страницы).
class WheelCombo final : public juce::ComboBox {
public:
    void mouseDown(const juce::MouseEvent& e)override{if(e.mods.isLeftButtonDown()&&!isPopupActive()){showComboBelow(*this,false);return;}juce::ComboBox::mouseDown(e);} // 1.6.32: ниже хитбокса
    double wheelAccum=0; // 1.7.1: мягкий скролл
    void mouseWheelMove(const juce::MouseEvent& e,const juce::MouseWheelDetails& w) override {
        if(getNumItems()<2){juce::ComboBox::mouseWheelMove(e,w);return;}
        wheelAccum+=w.deltaY*(w.isReversed?-1.0:1.0);const double step=0.12;
        while(wheelAccum>=step){wheelAccum-=step;setSelectedItemIndex(juce::jlimit(0,getNumItems()-1,getSelectedItemIndex()-1),juce::sendNotificationSync);}
        while(wheelAccum<=-step){wheelAccum+=step;setSelectedItemIndex(juce::jlimit(0,getNumItems()-1,getSelectedItemIndex()+1),juce::sendNotificationSync);}
    }
};

// 1.6.19: LFO CURVE EDITOR -- пересоздан по референсу (ui reference/just idea):
// шапка с числом точек, ряд CUSTOM + SHIFT, сетка с рельсами и ручками точек,
// правая колонка форм (POINTS/DRAW/FLAT/RISE/FALL/TRI/SINE/SQUARE + clear),
// ряд инструментов (X/Y/SNAP/UNDO/REDO/INVERT/FLIP X/COPY/PASTE), маршруты с
// знаковой величиной (НЕ зависят от PAGE/DEST/DPTH) и подсказки жестов внизу.
// 1.6.21: LFO CURVE EDITOR x3 -- три страницы кривых (кнопки 1/2/3 как в ARP),
// STEPS-режим (ступеньки-квадраты), выбор сетки привязки (OFF/1/8/1/16/1/32),
// изгиб прямых: наведение делает линию жирнее, драг -- экспо-изгиб, ПКМ --
// сброс наклона. Источник провода -- гнездо на кнопке MSEG внешней панели.
class MsegPage final : public juce::Component, private juce::Timer, public juce::FileDragAndDropTarget {
public:
    MsegPage(MonomachineNovaAudioProcessor& p,int startPage) : processor(p), page(juce::jlimit(0,7,startPage)) {
        loop.setButtonText("LOOP"); sync.setButtonText("SYNC");
        addAndMakeVisible(loop); addAndMakeVisible(sync);
        retrig.speedKey="MSEG GRID"; addAndMakeVisible(retrig); // 1.7.7: список режимов вместо галки: RETRIG / ENVELOPE / FREE
        for (const char* m : {"RETRIG","ENVELOPE","FREE","SUSTAIN","LOOP POINT","LOOP HOLD"}) retrig.addItem(m, retrig.getNumItems()+1); // 1.7.9: имя ENVELOPE вернули (VITAL -- это про AMP-огибающую, не MSEG)
        retrig.setSelectedItemIndex(0, juce::dontSendNotification); // дефолт RETRIG
        for (PixelButton* b : {&syncModeBtn, &tripBtn, &dotBtn}) { b->textHeight = 12; addAndMakeVisible(*b); } // 1.7.10: единая ручка RATE как в Serum -- семья выбирается кнопками
        syncModeBtn.setTooltip("SYNC: ON = RATE in BPM divisions 32 BAR..1/256 + FAST (tempo sync, as in Serum). OFF = free TIME, period 1 ms .. 60 s. RIGHT-click = BPM / TIME / HZ.");
        syncModeBtn.onClick = [this] { setRmode(getRmode() >= 3 ? 0 : 3); };
        syncModeBtn.rightClick = [this] { juce::PopupMenu m; m.setLookAndFeel(&smallMenuLaf()); m.addItem(1, "BPM (sync)", true, getRmode() <= 2); m.addItem(2, "TIME (free)", true, getRmode() == 3); m.addItem(3, "HZ", true, getRmode() == 4);
            const juce::Component::SafePointer<MsegPage> safe(this); m.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(&syncModeBtn), [safe](int res) { if (!safe || res <= 0) return; safe->setRmode(res == 1 ? 0 : res == 2 ? 3 : 4); }); };
        tripBtn.setTooltip("TRIPLET: divisions are triplets (same scale, x1.5 longer steps).");
        tripBtn.onClick = [this] { setRmode(getRmode() == 1 ? 0 : 1); };
        dotBtn.setTooltip("DOTTED: divisions are dotted (same scale, x1.5 longer steps).");
        dotBtn.onClick = [this] { setRmode(getRmode() == 2 ? 0 : 2); };
        keyBtn.setButtonText("KEY"); addAndMakeVisible(keyBtn); // 1.7.8: KEYTRACK
        keyBtn.setTooltip("KEYTRACK: LFO/MSEG speed follows the note pitch -- +1 octave = x2 faster (as in Vital).");
        lpoint.setSliderStyle(juce::Slider::LinearHorizontal); lpoint.setTextBoxStyle(juce::Slider::TextBoxRight, false, 46, 18); lpoint.setRange(0.0, 1.0, 0.01); addAndMakeVisible(lpoint);
        lpoint.setTooltip("LOOP POINT (0..1) for SUSTAIN / LOOP POINT / LOOP HOLD modes: SUSTAIN holds here while the note is down; LOOP POINT loops the tail from here; LOOP HOLD loops 0..point while held."); // 1.7.8
        retrig.setTooltip("MSEG mode (this page, as in Vital): RETRIG = every new note restarts. ENVELOPE = one pass per note, then HOLDS the last point value. FREE = free-run, no restart.");
        { const char* mbs[]{"1:1","1:2","1:4","1:8","FRZ","OFF"}; // 1.7.7: настройки маркера под MSEG
          for (int i = 0; i < 6; ++i) { auto* b = markerBtns.add(new PixelButton{mbs[i]}); b->textHeight = 10; addAndMakeVisible(b);
              b->onClick = [this, i] { if (i < 4) { markerDiv = 1 << i; markerFrozen = false; markerOff = false; }
                  else if (i == 4) { markerFrozen = !markerFrozen; markerOff = false; } else { markerOff = !markerOff; } repaint(); }; } }
        rebuildPageStrip(); // 1.6.29: страницы списком (кнопки + драг + список по ПКМ)
        stepsButton.textHeight = 14; stepsButton.setClickingTogglesState(true); addAndMakeVisible(stepsButton); // 1.6.32: единый шрифт кнопок страницы
        stepsButton.setTooltip("STEPS: LMB = draw square stairs (display); RIGHT-click = BAKE the stairs into the curve permanently (again = bring the smooth shape back). With 256 points the stairs now reach the end."); // 1.7.9
        stepsButton.onClick = [this] { processor.setMsegSteps(page, stepsButton.getToggleState()); repaint(); };
        stepsButton.rightClick = [this] { // 1.7.9 FIX: ПКМ снова ЗАПЕКАЕТ квадраты в кривую (как просил юзер; в 1.7.7 точек не хватало, теперь лимит 256)
            if (stepsBaked) { pushUndo(); restore(bakedBackup); stepsBaked = false; }
            else { pushUndo(); bakedBackup = snapshot(); bakeSteps(); stepsBaked = true; }
            processor.setMsegSteps(page, false); stepsButton.setToggleState(false, juce::dontSendNotification); repaint(); };
        stepsButton.setTooltip("STEPS: LMB = draw square stairs (display); RIGHT-click = same mode toggle as LMB (1.7.7 -- no more baking away 32 points).");
        grid.setTextWhenNoChoicesAvailable("GRID"); // 1.6.29: ПОЛНАЯ сетка + тройки; GRID VIEW -- визуальная сетка; LINK синхронизирует
        for (const char* g : {"GRID OFF","GRID 1/1","GRID 1/2D","GRID 1/1T","GRID 1/2","GRID 1/4D","GRID 1/2T","GRID 1/4","GRID 1/8D","GRID 1/4T","GRID 1/8","GRID 1/16D","GRID 1/8T","GRID 1/16","GRID 1/32D","GRID 1/16T","GRID 1/32","GRID 1/64D","GRID 1/32T","GRID 1/64"}) grid.addItem(g, grid.getNumItems()+1); // 1.7.7: список как у ARP -- bars/dotted/triplets до 1/64
        grid.setSelectedItemIndex(12, juce::dontSendNotification); // 1/16
        grid.setTooltip("DRAW GRID: snapping for new points, drags and steps. Wheel changes it while you draw steps. CTRL+1 coarser, CTRL+2 finer, CTRL+3 triplets, CTRL+4 snap on/off. Hold Alt while drawing to bypass.");
        shape.smallPopup=grid.smallPopup=viewGrid.smallPopup=true;shape.speedKey="MSEG SHAPE";grid.speedKey="MSEG GRID";viewGrid.speedKey="MSEG VIEW"; // 1.6.33: компактные списки под кнопкой
        grid.onChange = [this] { if (linkBtn.getToggleState() && viewGrid.getSelectedItemIndex() != grid.getSelectedItemIndex()) { viewGrid.setSelectedItemIndex(grid.getSelectedItemIndex(), juce::dontSendNotification); } if (drawMode == 2) { stepsPrimed = false; } repaint(); }; // 1.6.31: смена сетки перезапускает степ-рисование
        addAndMakeVisible(grid);
        for (const char* g : {"VIEW OFF","VIEW 1/1","VIEW 1/2D","VIEW 1/1T","VIEW 1/2","VIEW 1/4D","VIEW 1/2T","VIEW 1/4","VIEW 1/8D","VIEW 1/4T","VIEW 1/8","VIEW 1/16D","VIEW 1/8T","VIEW 1/16","VIEW 1/32D","VIEW 1/16T","VIEW 1/32","VIEW 1/64D","VIEW 1/32T","VIEW 1/64"}) viewGrid.addItem(g, viewGrid.getNumItems()+1); // 1.7.7: как ARP
        viewGrid.setSelectedItemIndex(12, juce::dontSendNotification); // 1.6.33: старт = 1/16 как GRID -- LINK включён, видовая сетка не расходится с рисованием до первой смены
        viewGrid.setTooltip("GRID VIEW: the visual grid over the graph, independent from the snapping grid when LINK is off. Wheel over the graph cycles it.");
        viewGrid.onChange = [this] { if (linkBtn.getToggleState() && grid.getSelectedItemIndex() != viewGrid.getSelectedItemIndex()) { grid.setSelectedItemIndex(viewGrid.getSelectedItemIndex(), juce::dontSendNotification); } repaint(); };
        addAndMakeVisible(viewGrid);
        linkBtn.setButtonText("LINK"); linkBtn.textHeight = 14; linkBtn.setClickingTogglesState(true); linkBtn.setToggleState(true, juce::dontSendNotification); // 1.6.32
        linkBtn.setTooltip("LINK: keep the draw grid and the visual grid equal. Turn off to draw in triplets while watching 1/4, for example.");
        addAndMakeVisible(linkBtn);
        const std::pair<juce::Component*,const char*> layIds[]{{&shape,"shape"},{&grid,"grid"},{&viewGrid,"viewgrid"},{&linkBtn,"link"},{&stepsButton,"steps"},{&loop,"loop"},{&sync,"sync"},{&rate,"rate"},{&shift,"shift"}};
        for(auto& pid:layIds)pid.first->setComponentID(pid.second); // 1.6.38
        rate.setSliderStyle(juce::Slider::LinearHorizontal); rate.setTextBoxStyle(juce::Slider::TextBoxRight, false, 70, 22); rate.setRange(0.0, 1.0, 0.01); // 1.7.8: 0..1 -- позиция в семье RMODE
        rate.setTooltip("RATE -- one unified knob (as in Serum): SYNC on = BPM divisions 32 BAR..1/256 + FAST, SYNC off = free TIME 1 ms .. 60 s. TRIPLET/DOTTED buttons switch the division family. The current value is shown as SPEED under the graph."); // 1.7.10
        shape.addItem("CUSTOM",1);
        for (const char* s : {"FLAT","RISE","FALL","TRI","SINE","SQUARE"}) shape.addItem(s, shape.getNumItems()+1); // 1.6.30: custom flat rise fall tri sine square
        shape.setTooltip("Generate a shape (or edit points freely: the combo returns to CUSTOM).");
        shape.onChange = [this] { const bool wasCustom = lastShapeCustom, isCustom = shape.getSelectedId() == 1; // 1.6.29: CUSTOM помнит волну, с которой ушёл
            if (wasCustom && !isCustom) customBackup = snapshot();
            lastShapeCustom = isCustom;
            if (isCustom) { if (customBackup.size() >= 2) { pushUndo(); restore(customBackup); } repaint(); return; }
            pushUndo(); generate(shape.getText()); };
        addAndMakeVisible(shape);
        shift.setSliderStyle(juce::Slider::LinearHorizontal); shift.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0); shift.setRange(0.0, 360.0, 1.0);
        shift.setColour(juce::Slider::trackColourId, ink); shift.setColour(juce::Slider::thumbColourId, ink); shift.setColour(juce::Slider::backgroundColourId, ink.withAlpha(0.2f));
        shift.setTooltip("SHIFT: rotate the curve phase, degrees.");
        shift.onDragStart = [this] { pushUndo(); base = snapshot(); };
        shift.onValueChange = [this] { const float deg = static_cast<float>(shift.getValue()); restore(base); const float sh = deg / 360.0f; // 1.7.5 FIX: точки после поворота СОРТИРУЮТСЯ -- раньше кривая «переворачивалась»
            std::vector<Pt> rot;
            for (int i2 = 0; i2 < processor.msegPointCount(page); ++i2) rot.push_back({std::fmod(base[static_cast<size_t>(i2)].x - sh + 1.0f, 1.0f), base[static_cast<size_t>(i2)].y, base[static_cast<size_t>(i2)].k});
            std::stable_sort(rot.begin(), rot.end(), [](const Pt& a, const Pt& b) { return a.x < b.x; });
            for (size_t i2 = 0; i2 < rot.size(); ++i2) { processor.setMsegPoint(page, static_cast<int>(i2), rot[i2].x, rot[i2].y); processor.setMsegSegK(page, static_cast<int>(i2), rot[i2].k); }
            shiftLabel = juce::String(deg, 1) + " DEG"; repaint(); };
        shift.onDragEnd = [this] { if (std::abs(shift.getValue()) < 0.5f) shiftLabel = "0.0 DEG"; };
        shift.setDoubleClickReturnValue(true, 0.0);
        addAndMakeVisible(shift);
        const char* forms[]{"DRAW","DRAW STEP","RAND DRAW 1","RAND DRAW 2","VITAL"}; // 1.7.8: vital = сглаженное перо до 256 точек; 1.7.9: ПКМ по VITAL = форма волны (SINE/TRI/SAW/PULSE/RANDOM) // 1.6.30: плоские формы убраны (они в списке SHAPE)
        for (const char* f : forms) { auto* b = formButtons.add(new PixelButton{f}); b->textHeight = 16; b->setBounds(0,0,10,10); addAndMakeVisible(b); b->setComponentID(juce::String("form")+juce::String(formButtons.size()-1)); // 1.7.5: RAND DRAW 1/2 -- шрифт как у остальных кнопок; 1.6.32/41: доступны в LAYOUT EDIT
            if (juce::String(f).contains("DRAW")) b->setClickingTogglesState(true);
            if (juce::String(f) == "VITAL") b->rightClick = [this, b] { juce::PopupMenu m; m.setLookAndFeel(&smallMenuLaf()); // 1.7.9: ПКМ VITAL -- штамп формы волны
                static const char* wns[]{"SINE","TRI","SAW UP","SAW DOWN","PULSE 25","PULSE 50","RANDOM SMOOTH"};
                for (int w2 = 0; w2 < 7; ++w2) m.addItem(w2 + 1, wns[w2]);
                const juce::Component::SafePointer<MsegPage> safe2(this);
                m.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(b), [safe2](int res) { if (safe2 && res > 0) safe2->stampWave(res); }); };
            b->onClick = [this, f] {
                const juce::String s(f);
                if (s == "VITAL") { drawMode = (drawMode == 5) ? 0 : 5; if (drawMode != 0) pushUndo(); syncDrawButtons(); repaint(); return; } // 1.7.8: vital -- перо со сглаживанием
                if (s == "DRAW") { drawMode = (drawMode == 1) ? 0 : 1; if (drawMode != 0) pushUndo(); syncDrawButtons(); repaint(); return; } // 1.6.30: вход не рисует -- рисует рука
                else if (s == "DRAW STEP") { drawMode = (drawMode == 2) ? 0 : 2; stepsPrimed = false; if (drawMode != 0) pushUndo(); syncDrawButtons(); repaint(); return; } // 1.6.30: вход НЕ превращает кривую в квадраты
                else if (s == "RAND DRAW 1") { drawMode = (drawMode == 3) ? 0 : 3; if (drawMode != 0) pushUndo(); syncDrawButtons(); repaint(); return; } // 1.6.30: дикая random draw (экс-баг -- теперь фича)
                else if (s == "RAND DRAW 2") { drawMode = (drawMode == 4) ? 0 : 4; if (drawMode != 0) pushUndo(); syncDrawButtons(); repaint(); return; } // 1.6.30: управляемая random draw
                else { drawMode = 0; syncDrawButtons(); pushUndo(); generate(s); shape.setText(s, juce::dontSendNotification); } }; }
        clearButton.setButtonText("clear"); clearButton.textHeight = 14; addAndMakeVisible(clearButton); // 1.6.32
        clearButton.onClick = [this] { pushUndo(); processor.setMsegPoint(page, 0, 0.0f, 0.0f); processor.setMsegPoint(page, 1, 0.0f, 0.0f); processor.setMsegPointCount(page, 2); markCustom(); repaint(); }; // 1.6.30: только одна точка слева внизу
        const char* tools[]{"UNDO","REDO","INVERT","FLIP X","COPY","PASTE"};
        for (const char* tl : tools) { auto* b = toolButtons.add(new PixelButton{tl}); b->textHeight = 14; addAndMakeVisible(b); b->setComponentID(juce::String("tool")+juce::String(toolButtons.size()-1)); // 1.6.32/41: доступны в LAYOUT EDIT
            b->onClick = [this, tl] { tool(juce::String(tl)); }; }
        setSize(1170, 800); setWantsKeyboardFocus(true); startTimerHz(20); setPage(page);
        { const auto sz = uiLayout().getArr("msegsize"); // 1.7.1: редактируемый размер окна MSEG
          if (sz.size() >= 2 && sz[0] >= 1000 && sz[1] >= 700) setSize(sz[0], sz[1]); }
        tagRows=std::make_unique<TextTag>(*this,"rownames",juce::Rectangle<int>(54,routeY(),330,22),true); // 1.6.42: имена строк двигаются в LAYOUT
        tagHeads=std::make_unique<TextTag>(*this,"rowheads",juce::Rectangle<int>(16,routeY(),34,22),true); // 1.7.5: гнёзда-прицелы -- отдельный тег, двигаются и выделяются в LAYOUT EDIT (вертикаль привязана к строкам)
    }
    ~MsegPage() override { stopTimer(); }

    int curPage() const { return page; } // 1.6.31: для прицела из редактора
    std::function<void(int,int)> pickStart; std::function<void(int,int)> destListRequest; // 1.6.34 прицел; 1.7.5 список целей для UNASSIGNED DEST в матрице); pg, displayRow
    int flashRouteRow=-1; juce::uint32 flashRouteUntil=0; // 1.6.34: мигание строки маршрута после бинда
    int markerDiv=1; bool markerFrozen=false, markerOff=false; double markerDisp=0.0, markerLast=0.0; juce::OwnedArray<PixelButton> markerBtns; // 1.7.7: MARKER 1:1..1:8 / FRZ / OFF
    std::vector<int> selPoints; // 1.7.7: Ctrl-выделение точек (ПКМ по любой стирает все выделенные)
    void flashRow(int r){flashRouteRow=r;flashRouteUntil=juce::Time::getMillisecondCounter()+1320;repaint();}
    std::function<void()> pushEdit; // 1.6.32
    void rebuildPageStrip() { // 1.6.31: страницы фиксированы (8), «+»/«−» убраны; кнопка = номер текущей, клик -- список ниже хитбокса
        pageButtons.clear(true);
        auto* b = pageButtons.add(new PixelButton{juce::String(page + 1)});
        b->setComponentID("pg"+juce::String(pageButtons.size()-1)); // 1.6.38/41: уникальный ID на страницу (был один на все)
        b->textHeight = 14; // 1.6.32
        b->onClick = [this] { showPageList(); };
        b->verticalDrag = true; b->dragPixelsPerStep = 14; b->dragHandler = [this](int steps, bool) { if (steps != 0) setPage(page + steps); };
        addAndMakeVisible(b);
    }
    void showPageList() { // 1.6.31: чёрный пиксельный скин как у всех списков; 8 страниц без пометок
        juce::PopupMenu m; m.setLookAndFeel(&getLookAndFeel());
        for (int i = 0; i < 8; ++i) m.addItem(i + 1, "MSEG " + juce::String(i + 1), true, i == page);
        auto safe = juce::Component::SafePointer<MsegPage>(this);
        m.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(pageButtons.size() > 0 ? pageButtons.getUnchecked(0) : nullptr), [safe](int res) { if (safe != nullptr && res > 0) safe->setPage(res - 1); });
    }
    void removePageWithConfirm() { // 1.6.29: удаление страницы; переименование назначений -- в процессоре
        if (processor.msegPageCount() <= 1) return;
        const int p = page; bool used = false;
        for (int r = 0; r < 64 && !used; ++r) { // 1.7.7: 64 слота
            auto* s = processor.parameters.getRawParameterValue("r" + juce::String(r) + "_src"); auto* d = processor.parameters.getRawParameterValue("r" + juce::String(r) + "_dest"); auto* o = processor.parameters.getRawParameterValue("r" + juce::String(r) + "_on");
            if (!s || !d || !o || o->load() < 0.5f) continue;
            const int sv = juce::roundToInt(s->load()), dv = juce::roundToInt(d->load());
            const int sp = (sv >= 10 && sv <= 12) ? sv - 10 : (sv >= 18 && sv <= 22) ? sv - 15 : -1;
            if (sp == p) used = true;
            if (dv >= 56 && dv <= 63 && dv - 56 == p) used = true;
        }
        auto doRemove = [this, p] { processor.removeMsegPage(p); rebuildPageStrip(); setPage(juce::jlimit(0, processor.msegPageCount() - 1, p)); resized(); };
        if (!used) { doRemove(); return; }
        auto safe = juce::Component::SafePointer<MsegPage>(this);
        juce::AlertWindow::showOkCancelBox(juce::MessageBoxIconType::QuestionIcon, "DELETE MSEG PAGE",
            "MSEG" + juce::String(p + 1) + " is used by modulation routes. Delete it?\nRoutes pointing here are switched off; later pages are renamed (MSEG" + juce::String(p + 2) + " -> MSEG" + juce::String(p + 1) + ", ...).",
            "DELETE", "CANCEL", nullptr, juce::ModalCallbackFunction::create([safe, doRemove](int r) { if (safe == nullptr || r == 0) return; doRemove(); }));
    }
    void setPage(int m) {
        page = juce::jlimit(0, 7, m); if (isShowing()) grabKeyboardFocus(); // 1.6.31: страницы фиксированы -- активация не нужна
        customBackup.clear(); lastShapeCustom = true; // 1.6.29
        if (pageButtons.size() > 0) pageButtons.getUnchecked(0)->setButtonText(juce::String(page + 1)); // 1.6.31: просто номер текущей страницы
        const juce::String tag = page == 0 ? juce::String("mseg") : "mseg" + juce::String(page + 1); // 1.6.21: id-ы первой страницы -- легаси
        loopLink = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processor.parameters, tag + "_loop", loop);
        syncLink = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processor.parameters, tag + "_sync", sync);
        rateLink = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.parameters, tag + "_rate", rate);
        retrigLink = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(processor.parameters, tag + "_retrig", retrig); // 1.7.7: режим постранично
        keyLink = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processor.parameters, tag + "_key", keyBtn); // 1.7.8
        lpointLink = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.parameters, tag + "_lpoint", lpoint); // 1.7.8 (RETRIG|ENVELOPE|FREE)
        retrig.onChange = [this] { repaint(); };
        stepsButton.setToggleState(processor.msegSteps(page), juce::dontSendNotification);
        undo.clear(); redo.clear(); selected = -1; hoverSeg = -1; bendSeg = -1; drawing = false; stepsBaked = false; stepsPrimed = false; bakedBackup.clear(); refreshSyncBtns(); repaint(); // 1.7.10: SYNC/TRIP/DOT по rmode страницы
    }
    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::black); g.setColour(ink);
        pixel::text(g, "LFO CURVE EDITOR", {12, 4, 420, 26}, 22);
        pixel::text(g, juce::String(processor.msegPointCount(page)) + " / 256 POINTS", {450, 6, 260, 22}, 16, false); // 1.7.8: лимит 256
        pixel::text(g, juce::String(processor.msegPointCount(page)) + " / 256", {getWidth() - 154, 118 + formButtons.size() * 26 + 28, 140, 16}, 13, false); // 1.7.10: сколько точек задействовано -- под CLEAR
        pixel::text(g, "MSEG", {246, 8, 60, 18}, 13, false);
        pixel::text(g, "SHIFT", {shift.getX() - 66, shift.getY() + 4, 60, 18}, 13, false); // 1.7.0: подпись едет с ручкой
        pixel::text(g, shiftLabel, {shift.getRight() + 8, shift.getY() + 4, 90, 18}, 13, true); // 1.7.0: значение едет с ручкой
        // сетка + рельсы
        g.setColour(ink.withAlpha(0.22f)); g.drawRect(graphX, graphY, graphW, graphH, 1);
        { const int den = static_cast<int>(gridDenF(viewGrid.getSelectedItemIndex()) + 0.5f); // 1.6.29/1.7.7: GRID VIEW
            if (den > 0) for (int i = 0; i <= den; ++i) { const int x = graphX + juce::roundToInt(graphW * static_cast<float>(i) / static_cast<float>(den));
                g.setColour(ink.withAlpha(0.16f)); g.drawVerticalLine(x, float(graphY), float(graphY + graphH));
                g.setColour(ink.withAlpha(0.7f)); g.fillEllipse(static_cast<float>(x) - 2.0f, float(graphY) - 2.0f, 4.0f, 4.0f); g.fillEllipse(static_cast<float>(x) - 2.0f, float(graphY + graphH) - 2.0f, 4.0f, 4.0f); } }
        // точки: вертикали + кривая (ступеньки или изогнутые сегменты) + ручки
        const int n = processor.msegPointCount(page); // 1.6.31: вертикальные линии от точек убраны -- сетку рисует только GRID VIEW
        for (int i = 1; i < n; ++i) { // 1.6.21: сегмент по одному -- ховер/бенд рисуются ЖИРНЫМ
            const bool emph = (i - 1 == hoverSeg) || (i - 1 == bendSeg);
            juce::Path seg;
            if (processor.msegSteps(page)) {
                const float x0 = pointToX(processor.msegPointX(page, i - 1)), y0 = pointToY(processor.msegPointY(page, i - 1));
                const float x1 = pointToX(processor.msegPointX(page, i)), y1 = pointToY(processor.msegPointY(page, i));
                seg.startNewSubPath(x0, y0); seg.lineTo(x1, y0); seg.lineTo(x1, y1);
            } else {
                const float kk = processor.msegSegK(page, i - 1);
                for (int s = 0; s <= 24; ++s) { const float t = s / 24.0f; const float shaped = std::pow(t, std::pow(2.0f, kk));
                    const float px = pointToX(processor.msegPointX(page, i - 1) + (processor.msegPointX(page, i) - processor.msegPointX(page, i - 1)) * t);
                    const float py = pointToY(processor.msegPointY(page, i - 1) + (processor.msegPointY(page, i) - processor.msegPointY(page, i - 1)) * shaped);
                    if (s == 0) seg.startNewSubPath(px, py); else seg.lineTo(px, py); }
            }
            g.setColour(ink); g.strokePath(seg, juce::PathStrokeType(emph ? 4.0f : 2.0f));
        }
        for (int i = 0; i < n; ++i) { const float px = pointToX(processor.msegPointX(page, i)), py = pointToY(processor.msegPointY(page, i));
            const bool sel = std::find(selPoints.begin(), selPoints.end(), i) != selPoints.end();
            g.setColour(juce::Colours::black); g.fillEllipse(px - 6.0f, py - 6.0f, 12.0f, 12.0f);
            g.setColour(sel ? juce::Colours::white : ink); g.drawEllipse(px - 6.0f, py - 6.0f, 12.0f, 12.0f, i == selected ? 3.0f : sel ? 2.6f : 1.6f); } // 1.7.7: выделенные -- белые
        if (drawing && (drawMode == 1 || drawMode == 3 || drawMode == 4 || drawMode == 5) && stroke.size() >= 2) { // 1.6.32: живое превью штриха -- сама кривая не трогается
            juce::Path sp; for (size_t i = 0; i < stroke.size(); ++i) { const float px = pointToX(stroke[i].x), py = pointToY(stroke[i].y); if (i == 0) sp.startNewSubPath(px, py); else sp.lineTo(px, py); }
            g.setColour(ink.withAlpha(0.8f)); g.strokePath(sp, juce::PathStrokeType(2.0f)); }
        if (selected >= 0 && selected < n) { pixel::text(g, "X " + juce::String(juce::roundToInt(processor.msegPointX(page, selected) * 128)), {20, toolY() + 4, 60, 20}, 14, false);
            pixel::text(g, "Y " + juce::String(juce::roundToInt(processor.msegPointY(page, selected) * 128)), {110, toolY() + 4, 60, 20}, 14, false); }
        // ROUTING / SIGNED AMOUNT
        // 1.7.1: вся матрица дестинейшенов едет за тегом rownames в LAYOUT EDIT (гнездо/бар/крестик -- от него)
        const int rowX = tagHeads->getX(), barX = tagRows->getRight() + 11, eraseX = barX + 261; // 1.7.5: гнёзда -- от тега rowheads
        int row = 0;
        for (int r = 0; r < 64 && row < 8; ++r) { // 1.6.29/1.7.7: сканируем все 64 слота
            auto* src = processor.parameters.getRawParameterValue("r" + juce::String(r) + "_src");
            auto* dst = processor.parameters.getRawParameterValue("r" + juce::String(r) + "_dest");
            auto* on = processor.parameters.getRawParameterValue("r" + juce::String(r) + "_on");
            if (!src || !dst || !on || on->load() < 0.5f) continue;
            const int sv = juce::roundToInt(src->load());
            const int mIdx = (sv >= 10 && sv <= 12) ? sv - 10 : (sv >= 18 && sv <= 22) ? sv - 15 : -1; // 1.6.29: страницы 4..8 = источники 18..22
            if (mIdx < 0) continue;
            const int dRaw = juce::roundToInt(dst->load()); if (dRaw < 1) continue; // 1.6.32: 0 = OFF
            const int target = juce::jlimit(0, 247, dRaw - 1);
            const int y = routeY() + row * 34;
            g.setColour(ink); g.drawRect(rowX, y + 2, 34, 26, 1); // 1.6.31/34: гнездо прицела -- клик = прицел (как DEST в матрице)
            pixel::text(g, juce::String(row + 1), {rowX + 4, y, 26, 22}, 14, false);
            pixel::text(g, "M" + juce::String(mIdx + 1) + " " + targetName(processor,target), {tagRows->getX(), y, tagRows->getWidth(), 22}, 15, false);
            auto* depth = processor.parameters.getRawParameterValue("r" + juce::String(r) + "_depth");
            const float raw = depth ? depth->load() : 0.0f;
            const float frac = (raw + 64.0f) / 127.0f;
            g.setColour(ink.withAlpha(0.25f)); g.drawHorizontalLine(y + 14, float(barX), float(barX + 190)); // 1.7.1: ампаунт едет с матрицей (был 395..585)
            g.setColour(ink); g.fillEllipse(float(barX) + 190.0f * frac - 5.0f, float(y + 9), 10.0f, 10.0f);
            pixel::text(g, (raw < 0 ? "-" : raw > 0 ? "+" : juce::String()) + juce::String(juce::roundToInt(std::abs(raw) * 100.0f / (raw < 0 ? 64.0f : 63.0f))) + "%", {barX + 197, y, 60, 22}, 14, false); // 1.7.1: процент едет с баром
            g.setColour(ink); g.drawRect(eraseX, y + 2, 20, 18, 1); g.drawLine(float(eraseX + 5), float(y + 7), float(eraseX + 15), float(y + 15), 2.0f); g.drawLine(float(eraseX + 15), float(y + 7), float(eraseX + 5), float(y + 15), 2.0f); // 1.7.1: крестик едет с матрицей
            routeRows[static_cast<size_t>(row)] = r;
            ++row;
        }
        if (flashRouteRow >= 0 && juce::Time::getMillisecondCounter() < flashRouteUntil && ((juce::Time::getMillisecondCounter() / 280) & 1) == 0) { g.setColour(juce::Colours::white); g.drawRect(14, routeY() + flashRouteRow * 34 + 1, 372, 28, 2); } // 1.6.34: мигание DEST-ячейки маршрута (как в матрице)
        for (int rest = row; rest < 3; ++rest) { const int y2 = routeY() + rest * 34; // 1.6.34: UNASSIGNED -- гнездо прицела как у активных строк
            g.setColour(ink); g.drawRect(rowX, y2 + 2, 34, 26, 1); // 1.7.1
            const int cx2 = rowX + 17, cy2 = y2 + 15; // 1.7.1
            g.drawHorizontalLine(cy2, float(cx2 - 8), float(cx2 + 8)); g.drawVerticalLine(cx2, float(cy2 - 8), float(cy2 + 8));
            pixel::text(g, juce::String(rest + 1) + "  UNASSIGNED", {tagRows->getX(), y2, tagRows->getWidth(), 22}, 15, false); } // 1.6.42
        if (!markerOff) { const int px = juce::roundToInt(pointToX(static_cast<float>(markerDisp))); // 1.7.7: маркер с делителем скорости (настройки под маршрутами)
            if (px >= graphX && px <= graphX + graphW) { g.setColour(juce::Colours::white.withAlpha(0.85f)); g.drawVerticalLine(px, float(graphY), float(graphY + graphH)); g.fillRect(float(px) - 3.0f, float(graphY - 8), 6.0f, 6.0f); }
            pixel::text(g, "MARKER", {getWidth() - 154 - 6 * 46 - 66, routeY() + 3 * 34 + 9, 62, 16}, 11, false); }
        pixel::text(g, "LOOP POINT", {lpoint.getRight() + 6, lpoint.getY() + 4, 90, 16}, 11, false); // 1.7.8
        pixel::text(g, "SPEED " + speedText(), {graphX + 6, graphY + graphH - 16, 260, 14}, 11, false); // 1.7.9: текущая скорость страницы числом (под графом, у левого края)
        g.setColour(ink.withAlpha(0.6f)); for (int k = 0; k < 4; ++k) g.drawLine(float(getWidth() - 14 + k * 3), float(getHeight() - 2), float(getWidth() - 2), float(getHeight() - 14 + k * 3)); // 1.7.1: грип стретча окна MSEG
        pixel::text(g, "DOUBLE CLICK: ADD / REMOVE   DRAG POINT: MOVE   DRAG LINE: BEND (EXPO)   RIGHT CLICK LINE: STRAIGHTEN   RIGHT CLICK POINT: REMOVE   ALT: BYPASS SNAP   CTRL+1/2 GRID -/+   CTRL+3 TRIPLET   CTRL+4 SNAP   B DRAW   DRAG ROW NUMBER: AIM", {20, getHeight() - 24, 1120, 17}, 12, false); // 1.6.31
    }
    void resized() override {
        graphW = juce::jlimit(600, 2400, 984 + (getWidth() - 1170)); graphH = juce::jlimit(150, 1200, 264 + (getHeight() - 800)); // 1.7.1: граф тянется за окном; 1.7.10: ниже на 28px -- второй ряд управления (всё, кроме матрицы, собрано над графом)
        retrig.setBounds(704, 70, 72, 26); // 1.7.10: ряд 1 -- LOOP SYNC SHAPE страницы STEPS GRID LINK VIEW RETRIG SHIFT
        for (int i = 0; i < 6 && i < markerBtns.size(); ++i) markerBtns.getUnchecked(i)->setBounds(getWidth() - 154 - 6 * 46 + i * 46, routeY() + 3 * 34 + 4, 42, 18); // 1.7.7: MARKER
        syncModeBtn.setBounds(17, 98, 54, 22); tripBtn.setBounds(75, 98, 46, 22); dotBtn.setBounds(125, 98, 46, 22); // 1.7.10: ряд 2 -- семья RATE как в Serum
        keyBtn.setBounds(175, 100, 44, 18); lpoint.setBounds(223, 98, 128, 22); rate.setBounds(355, 98, 280, 22); // LOOP POINT + единая ручка RATE
        { static const int tx[]{643, 705, 767, 829, 891, 953}; // 1.7.10: UNDO/REDO/INVERT/FLIP X/COPY/PASTE -- в ряд 2 (под графом остаётся только матрица)
          for (int i = 0; i < toolButtons.size() && i < 6; ++i) toolButtons.getUnchecked(i)->setBounds(tx[i], 98, 60, 22); }
        shape.setBounds(150, 70, 120, 26); // 1.7.10: компактнее, ряд 1 собран заново
        if (pageButtons.size() > 0) pageButtons.getUnchecked(0)->setBounds(274, 70, 64, 26); // 1.7.10
        stepsButton.setBounds(342, 70, 90, 26); grid.setBounds(436, 70, 94, 26); linkBtn.setBounds(534, 70, 54, 26); viewGrid.setBounds(592, 70, 108, 26); // 1.7.10: плотный ряд без наложений
        shift.setBounds(844, 70, 100, 26); // 1.7.10: подпись SHIFT влезает между RETRIG и собой
        for (int i = 0; i < formButtons.size(); ++i) formButtons.getUnchecked(i)->setBounds(getWidth() - 154, 118 + i * 26, 140, 24); // 1.7.1: колонка форм от края окна
        clearButton.setBounds(getWidth() - 154, 118 + formButtons.size() * 26 + 6, 140, 20); // 1.7.1
        loop.setBounds(17, 70, 62, 26); sync.setBounds(79, 70, 67, 26); // 1.7.10: rate переехал в ряд 2 (единая ручка)
        uiLayoutApply(*this,"mseg"); // 1.6.38: сохранённая раскладка MSEG-страницы
    }
    void mouseDown(const juce::MouseEvent& e) override {
        const int rowX = tagHeads->getX(), barX = tagRows->getRight() + 11, eraseX = barX + 261; // 1.7.5: зоны -- от тегов rowheads/rownames
        if (e.x >= getWidth() - 18 && e.y >= getHeight() - 18) { resizing = true; resW = getWidth(); resH = getHeight(); resX = e.x; resY = e.y; return; } // 1.7.1: грип стретча окна
        for (int row = 0; row < 8; ++row) { const int rr = routeRows[static_cast<size_t>(row)]; if (rr < 0) continue;
            const int y = routeY() + row * 34;
            if (e.x >= eraseX - 2 && e.x <= eraseX + 22 && e.y >= y && e.y <= y + 30) { if (auto* on = processor.parameters.getParameter("r" + juce::String(rr) + "_on")) { on->beginChangeGesture(); on->setValueNotifyingHost(on->convertTo0to1(0.0f)); on->endChangeGesture(); } repaint(); return; } // 1.6.35: кнопка стирания рядом с баром
            if (e.x >= barX - 2 && e.x <= barX + 192 && e.y >= y - 3 && e.y <= y + 28) { routeDrag = rr; routeBase = processor.parameters.getRawParameterValue("r" + juce::String(rr) + "_depth")->load(); routeY0 = static_cast<int>(e.x); if (auto* d = processor.parameters.getParameter("r" + juce::String(rr) + "_depth")) d->beginChangeGesture(); return; } // 1.6.35: зона драга = бар 395..585
        }
        for (int row = 0; row < 8; ++row) { const int rr = routeRows[static_cast<size_t>(row)]; if (rr < 0) continue; // 1.6.31: прицел с гнезда-номера строки маршрута -- как в основной матрице
            const int y = routeY() + row * 34;
            if (e.x >= rowX - 2 && e.x <= rowX + 36 && e.y >= y && e.y <= y + 30 && !e.mods.isPopupMenu()) { if (pickStart) pickStart(page, row); return; } } // 1.6.34: гнездо = ПРИЦЕЛ (клик-клик), не провод
        { int assigned = 0; for (int row = 0; row < 8; ++row) if (routeRows[static_cast<size_t>(row)] >= 0) ++assigned; // 1.7.6 FIX: гнёзда обрабатываются ТОЛЬКО ниже занятых строк (список перехватывал прицел занятых)
          for (int rest = assigned; rest < 3; ++rest) { const int y2 = routeY() + rest * 34; // 1.6.34: UNASSIGNED-строка
            if (e.x >= rowX - 2 && e.x <= rowX + 36 && e.y >= y2 && e.y <= y2 + 30) { if (e.mods.isPopupMenu()) { if (destListRequest) destListRequest(page, rest); return; } // 1.7.6: ПКМ -- список целей (папки как в P-LIST)
                if (pickStart) pickStart(page, rest); return; } } } // 1.7.6: ЛКМ -- прицел как раньше (клик по ручке создаёт маршрут)
        if (e.x < graphX || e.x > graphX + graphW || e.y < graphY || e.y > graphY + graphH) return;
        if (isShowing()) grabKeyboardFocus(); // 1.6.29: хоткеи сетки
        if (drawMode > 0 && !e.mods.isPopupMenu()) { // 1.6.30: рисование
            pushUndo();
            if (drawMode == 2) { // 1.6.31: вход и первое касание кривую НЕ квадратят -- квадраты только в тронутых колонках
                if (!stepsPrimed) { stepOrig = snapshot(); const auto pts = stepOrig; for (int cc = 0; cc < 64; ++cc) stepLevels[static_cast<size_t>(cc)] = sampled(pts, (static_cast<float>(cc) + 0.5f) / static_cast<float>(gridDen())); stepTouched.fill(false); stepsPrimed = true; }
                const int col = juce::jlimit(0, gridDen() - 1, static_cast<int>(toX(e.x) * gridDen())); stepLevels[static_cast<size_t>(col)] = toY(e.y); stepTouched[static_cast<size_t>(col)] = true; rebuildStairs(); }
            else { snapStroke = drawMode == 1 && !e.mods.isAltDown(); stroke.clear(); stroke.push_back({toX(e.x), toY(e.y), 0.0f}); writeStroke(); } // Alt -- рисуем без сетки
            drawing = true; markCustom(); repaint(); return; }
        const int near = nearestPoint(e.position);
        if (e.mods.isCtrlDown() && !e.mods.isPopupMenu() && near >= 0) { // 1.7.7: Ctrl+клик -- выделение точки (ПКМ по любой выделенной сотрёт все)
            const auto it = std::find(selPoints.begin(), selPoints.end(), near);
            if (it != selPoints.end()) selPoints.erase(it); else selPoints.push_back(near);
            draggingPoint = false; repaint(); return; }
        if (e.mods.isPopupMenu()) { // 1.6.33: ПКМ по точке = РЕЖИМ СТИРАНИЯ (ведение мыши стирает точки); по линии = сбросить наклон
            if (!selPoints.empty() && near >= 0) { pushUndo(); // 1.7.7: стираем ВСЕ выделенные точки сразу
                std::vector<int> doomed; for (int idx : selPoints) if (idx > 0 && idx < processor.msegPointCount(page) - 1) doomed.push_back(idx);
                std::sort(doomed.begin(), doomed.end(), std::greater<int>()); for (int idx : doomed) processor.removeMsegPoint(page, idx);
                selPoints.clear(); selected = -1; markCustom(); repaint(); return; }
            selPoints.clear();
            if (near > 0 && near < processor.msegPointCount(page) - 1) { pushUndo(); erasing = true; processor.removeMsegPoint(page, near); selected = -1; markCustom(); }
            else { const int seg = nearestSegment(e.position); if (seg >= 0) { pushUndo(); processor.setMsegSegK(page, seg, 0.0f); } }
            repaint(); return; }
        if (e.getNumberOfClicks() >= 2) { if (near >= 0) { pushUndo(); if (near > 0 && near < processor.msegPointCount(page) - 1) processor.removeMsegPoint(page, near); selected = -1; }
            else { pushUndo(); float px = snapX(toX(e.x)), py = toY(e.y); processor.addMsegPoint(page, px, py); selected = nearestPoint(e.position); } selPoints.clear(); markCustom(); repaint(); return; }
        selected = near;
        if (selected < 0) { const int seg = nearestSegment(e.position); // 1.6.21: ЛКМ по прямой = её изгиб
            if (seg >= 0) { pushUndo(); bendSeg = seg; bendBase = processor.msegSegK(page, seg); bendY = static_cast<int>(e.y); return; } }
        if (selected < 0) { pushUndo(); selPoints.clear(); float px = snapX(toX(e.x)), py = toY(e.y); processor.addMsegPoint(page, px, py); selected = nearestPoint(e.position); }
        draggingPoint = selected >= 0; repaint();
    }
    void mouseDrag(const juce::MouseEvent& e) override {
        if (resizing) { const int w = juce::jlimit(1000, 1800, resW + (e.x - resX)), h = juce::jlimit(700, 1300, resH + (e.y - resY)); // 1.7.1: стретч
            if (w != getWidth() || h != getHeight()) setSize(w, h); repaint(); return; }
        if (routeDrag >= 0) { if (auto* d = processor.parameters.getParameter("r" + juce::String(routeDrag) + "_depth")) d->setValueNotifyingHost(d->convertTo0to1(juce::jlimit(-64.0f, 63.0f, routeBase + static_cast<float>(static_cast<int>(e.x) - routeY0) * 0.5f))); repaint(); return; } // 1.6.34: горизонтальный драг ампаунта (routeY0 хранит стартовый X)
        if (erasing) { const int nearE = nearestPoint(e.position); if (nearE > 0 && nearE < processor.msegPointCount(page) - 1) { processor.removeMsegPoint(page, nearE); markCustom(); } repaint(); return; } // 1.6.33: ПКМ-проведение стирает точки
        if (drawing && drawMode > 0) { // 1.6.30: перо
            if (drawMode == 1) { const float x = toX(e.x), y = toY(e.y); // штрих в любую сторону -- кривая НЕ переписывается до отпускания (1.6.32: дрожь убрана)
                if (stroke.empty() || std::abs(x - stroke.back().x) >= 0.015f || std::abs(y - stroke.back().y) >= 0.02f) stroke.push_back({x, y, 0.0f});
                repaint(); }
            else if (drawMode == 3) { const float x = toX(e.x), y = toY(e.y); // RAND 1: дикая random draw
                if (stroke.empty() || std::abs(x - stroke.back().x) >= 0.02f) { stroke.push_back({x, juce::jlimit(0.0f, 1.0f, y + (rng.nextFloat() * 2.0f - 1.0f) * 0.35f), 0.0f}); repaint(); } }
            else if (drawMode == 4) { const float x = toX(e.x), y = toY(e.y); // RAND 2: управляемая -- путь курсора + гладкий шум
                if (stroke.empty() || std::abs(x - stroke.back().x) >= 0.015f) { randPhase += 0.9f; const float j = (std::sin(randPhase * 1.7f) + std::sin(randPhase * 0.43f)) * 0.12f + (rng.nextFloat() * 2.0f - 1.0f) * 0.06f; // 1.6.32: запись только на отпускании
                    stroke.push_back({x, juce::jlimit(0.0f, 1.0f, y + j), 0.0f}); writeStroke(); } }
            else { const int den = gridDen(); const int col = juce::jlimit(0, den - 1, static_cast<int>(toX(e.x) * den)); stepLevels[static_cast<size_t>(col)] = toY(e.y); stepTouched[static_cast<size_t>(col)] = true; rebuildStairs(); }
            repaint(); return; }
        if (bendSeg >= 0) { processor.setMsegSegK(page, bendSeg, bendBase + static_cast<float>(bendY - static_cast<int>(e.y)) * 0.03f); markCustom(); repaint(); return; } // 1.6.21: экспо-изгиб
        if (!draggingPoint || selected < 0) return;
        const float x = toX(e.x), y = toY(e.y); const float lo = selected == 0 ? 0.0f : processor.msegPointX(page, selected - 1) + 0.005f; const float hi = selected == processor.msegPointCount(page) - 1 ? 1.0f : processor.msegPointX(page, selected + 1) - 0.005f;
        processor.setMsegPoint(page, selected, juce::jlimit(lo, hi, snapX(x)), y); markCustom(); repaint();
    }
    void mouseUp(const juce::MouseEvent& e) override {
        if (resizing) { resizing = false; const int arr[8] = { getWidth(), getHeight(), 0, 0, 0, 0, 0, 0 }; uiLayout().setArr("msegsize", arr, 8); uiLayout().save(); } // 1.7.1: размер окна -- в layout.json
        if (draggingPoint) pushUndo(); draggingPoint = false; erasing = false; bendSeg = -1; if (drawing && drawMode > 0 && drawMode != 2) { if (drawMode == 5) vitalStroke(); else writeStroke(); } stroke.clear(); drawing = false; if (routeDrag >= 0) { if (auto* d = processor.parameters.getParameter("r" + juce::String(routeDrag) + "_depth")) d->endChangeGesture(); routeDrag = -1; } } // 1.6.32: штрих пишется ОДИН раз на отпускании; 1.6.29: фиксация
    void mouseMove(const juce::MouseEvent& e) override { if (drawMode > 0) { setMouseCursor(juce::MouseCursor::CrosshairCursor); hoverSeg = -1; repaint(); return; } // 1.6.33: в DRAW -- только перекрестие; точки и изгибы не подсвечиваются
        const int near = nearestPoint(e.position); hoverSeg = near < 0 ? nearestSegment(e.position) : -1; // 1.6.21: линия жирнеет под курсором
        juce::Component::setMouseCursor(near >= 0 ? juce::MouseCursor::UpDownResizeCursor : juce::MouseCursor::CrosshairCursor); repaint(); }
private:
    struct Pt { float x = 0, y = 0, k = 0; };
    static float gridDenF(int idx) { static const float dens[]{0,4,5.3333f,6,8,10.6667f,12,16,21.3333f,24,32,42.6667f,48,64,85.3333f,96,128,170.6667f,192,256}; return dens[juce::jlimit(0, 19, idx)]; } // 1.7.7: деления на такт (bars/dotted/triplet -- как список ARP)
    juce::String speedText() const { // 1.7.9: скорость страницы в текущем R MODE (TEMPO -- доли, TIME -- период, HZ -- герцы)
        const juce::String tag = page == 0 ? juce::String("mseg") : "mseg" + juce::String(page + 1);
        auto* rv = processor.parameters.getRawParameterValue(tag + "_rate"); auto* rm = processor.parameters.getRawParameterValue(tag + "_rmode");
        if (!rv || !rm) return {};
        const int m = juce::jlimit(0, 4, juce::roundToInt(rm->load())); const float v = rv->load();
        if (m <= 2) { static const char* nm[]{"32 BAR","16 BAR","8 BAR","4 BAR","2 BAR","1 BAR","1/2","1/4","1/8","1/16","1/32","1/64","1/128","1/256","FAST"};
            return juce::String(nm[juce::jlimit(0, 14, juce::roundToInt(v * 14.0f))]) + (m == 1 ? " T" : m == 2 ? " D" : ""); }
        if (m == 3) { const double per = 0.001 * std::pow(60000.0, v);
            return per < 1.0 ? juce::String(juce::roundToInt(per * 1000.0)) + " MS" : per < 10.0 ? juce::String(per, 2) + " S" : juce::String(per, 1) + " S"; }
        const double hz = 0.001 * std::pow(10.0, 4.301 * v);
        return hz >= 1000.0 ? juce::String(hz / 1000.0, 2) + " KHZ" : juce::String(hz, 2) + " HZ"; }
    float snapX(float x) const { const float den = gridDenF(grid.getSelectedItemIndex()); // 1.6.29/1.7.7
        return den > 0 ? std::round(x * den) / den : x; }
    void gridStep(int delta) { const int idx = juce::jlimit(1, 19, grid.getSelectedItemIndex() + delta); grid.setSelectedItemIndex(idx, juce::sendNotificationSync); } // 1.6.29/1.7.7: ALT+1/2
    void gridTriplet() { const int idx = grid.getSelectedItemIndex(); const bool tri = idx == 3 || idx == 6 || idx == 9 || idx == 12 || idx == 15 || idx == 18; // 1.7.7: ALT+3 -- тройки в новом списке
        if (!tri) { straightIdx = idx; grid.setSelectedItemIndex(9, juce::sendNotificationSync); } else grid.setSelectedItemIndex(juce::jlimit(1, 19, straightIdx), juce::sendNotificationSync); }
    void gridSnapToggle() { if (grid.getSelectedItemIndex() > 0) { snapSavedIdx = grid.getSelectedItemIndex(); grid.setSelectedItemIndex(0, juce::sendNotificationSync); } // 1.6.29: ALT+4
        else grid.setSelectedItemIndex(juce::jlimit(1, 19, snapSavedIdx), juce::sendNotificationSync); }
    bool keyPressed(const juce::KeyPress& key) override { // 1.6.29: хоткеи как в Ableton
        if (key.getModifiers().isCtrlDown() && key.getKeyCode() == '1') { gridStep(-1); return true; } // 1.6.30: сетка -- CTRL (Alt остался обходом привязки при рисовании)
        if (key.getModifiers().isCtrlDown() && key.getKeyCode() == '2') { gridStep(1); return true; }
        if (key.getModifiers().isCtrlDown() && key.getKeyCode() == '3') { gridTriplet(); return true; }
        if (key.getModifiers().isCtrlDown() && key.getKeyCode() == '4') { gridSnapToggle(); return true; }
        if (!(key.getModifiers().isAltDown() || key.getModifiers().isCtrlDown() || key.getModifiers().isCommandDown()) && (key.getKeyCode() == 'B' || key.getKeyCode() == 'b')) {
            drawMode = drawMode == 0 ? 1 : 0; if (drawMode != 0) pushUndo(); syncDrawButtons(); repaint(); return true; } // 1.6.30: B не рисует сама
        return Component::keyPressed(key); }
    int toolY() const { return graphY + graphH + 10; } // 1.7.10: инструменты переехали в ряд 2, зона нужна для routeY()
    int getRmode() const { const juce::String tag = page == 0 ? juce::String("mseg") : "mseg" + juce::String(page + 1); auto* p = processor.parameters.getRawParameterValue(tag + "_rmode"); return p ? juce::jlimit(0, 4, juce::roundToInt(p->load())) : 0; } // 1.7.10
    void setRmode(int m) { const juce::String tag = page == 0 ? juce::String("mseg") : "mseg" + juce::String(page + 1); if (auto* p = processor.parameters.getParameter(tag + "_rmode")) { p->beginChangeGesture(); p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(juce::jlimit(0, 4, m)))); p->endChangeGesture(); } refreshSyncBtns(); } // 1.7.10: 0 TEMPO, 1 TRIPLET, 2 DOTTED, 3 TIME, 4 HZ
    void refreshSyncBtns() { const int m = getRmode(); syncModeBtn.setToggleState(m <= 2, juce::dontSendNotification); tripBtn.setToggleState(m == 1, juce::dontSendNotification); dotBtn.setToggleState(m == 2, juce::dontSendNotification); } // 1.7.10
    int routeY() const { return toolY() + 44; }
    std::vector<Pt> snapshot() const { std::vector<Pt> pts; for (int i = 0; i < processor.msegPointCount(page); ++i) pts.push_back({processor.msegPointX(page, i), processor.msegPointY(page, i), processor.msegSegK(page, i)}); return pts; }
    void restore(const std::vector<Pt>& pts) { if (pts.size() < 2) return; for (size_t i = 0; i < pts.size(); ++i) { processor.setMsegPoint(page, static_cast<int>(i), pts[i].x, pts[i].y); processor.setMsegSegK(page, static_cast<int>(i), pts[i].k); } processor.setMsegPointCount(page, static_cast<int>(pts.size())); } // 1.7.9: точки ДО count (без дёрганья всего графа)
    // 1.6.22: рисование пером. DRAW -- свободные точки, DRAW STEP -- квадратные
    // ступени по колонкам 1/16 (уровень каждой колонки -- по курсору).
    void markCustom() { shape.setText("CUSTOM", juce::dontSendNotification); lastShapeCustom = true; } // 1.6.29
    void syncDrawButtons() { for (int i = 0; i < formButtons.size(); ++i) { auto* b = formButtons.getUnchecked(i); const juce::String s = b->getButtonText();
        if (s == "VITAL") b->setToggleState(drawMode == 5, juce::dontSendNotification); else if (s == "DRAW") b->setToggleState(drawMode == 1, juce::dontSendNotification); else if (s == "DRAW STEP") b->setToggleState(drawMode == 2, juce::dontSendNotification); else if (s == "RAND DRAW 1") b->setToggleState(drawMode == 3, juce::dontSendNotification); else if (s == "RAND DRAW 2") b->setToggleState(drawMode == 4, juce::dontSendNotification); } }
    void startFreehand() { stroke.clear(); stroke.push_back({0.0f, 0.5f, 0.0f}); selected = -1; writeStroke(); repaint(); } // 1.6.29: штрих -- список точек
    void writeStroke() { auto pts = stroke; if (pts.empty()) return; // 1.6.31: штрих ДОБАВЛЯЕТСЯ к кривой -- точки вне штриха сохраняются (кривую больше не стирает)
        if (snapStroke && drawMode == 1) for (auto& p : pts) p.x = snapX(p.x); // обычный DRAW -- по сетке, Alt -- свободно
        std::sort(pts.begin(), pts.end(), [](const Pt& a, const Pt& b) { return a.x < b.x; });
        std::vector<Pt> thin; for (const auto& p : pts) if (thin.empty() || p.x - thin.back().x >= 0.015f) thin.push_back(p); else thin.back().y = p.y;
        if (thin.size() < 2) return;
        const float sx0 = thin.front().x, sx1 = thin.back().x;
        std::vector<Pt> merged;
        for (int i = 0; i < processor.msegPointCount(page); ++i) { const float x = processor.msegPointX(page, i); if (x < sx0 - 0.005f || x > sx1 + 0.005f) merged.push_back({x, processor.msegPointY(page, i), processor.msegSegK(page, i)}); }
        merged.insert(merged.end(), thin.begin(), thin.end());
        std::sort(merged.begin(), merged.end(), [](const Pt& a, const Pt& b) { return a.x < b.x; });
        if (merged.front().x > 0.02f) merged.insert(merged.begin(), {0.0f, merged.front().y, 0.0f});
        if (merged.back().x < 0.98f) merged.push_back({1.0f, merged.back().y, 0.0f});
        merged.front().x = 0.0f; merged.back().x = 1.0f;
        if (static_cast<int>(merged.size()) > 32) { std::vector<Pt> kept; for (int i = 0; i < 32; ++i) kept.push_back(merged[static_cast<size_t>(i * (static_cast<int>(merged.size()) - 1) / 31)]); kept.front().x = 0.0f; kept.back().x = 1.0f; merged.swap(kept); }
        for (size_t i = 0; i < merged.size(); ++i) processor.setMsegPoint(page, static_cast<int>(i), merged[i].x, merged[i].y); // 1.7.9: точки ДО count
        processor.setMsegPointCount(page, static_cast<int>(merged.size())); }
    void stampWave(int kind) { pushUndo(); processor.setMsegSteps(page, false); // 1.7.9: ПКМ по VITAL -- форма волны в граф (как paint patterns в Vital)
        const int n = kind == 7 ? 256 : 129;
        for (int i = 0; i < n; ++i) { const float ph = static_cast<float>(i) / static_cast<float>(n - 1); float y = 0.5f;
            if (kind == 1) y = 0.5f - 0.5f * std::cos(2.0f * juce::MathConstants<float>::pi * ph);
            else if (kind == 2) y = ph < 0.5f ? 1.0f - 4.0f * ph : 4.0f * ph - 3.0f;
            else if (kind == 3) y = 1.0f - ph;
            else if (kind == 4) y = ph;
            else if (kind == 5) y = ph < 0.25f ? 1.0f : 0.0f;
            else if (kind == 6) y = ph < 0.5f ? 1.0f : 0.0f;
            else y = 0.5f + 0.5f * std::sin(2.0f * juce::MathConstants<float>::pi * (ph + 0.13f * std::sin(5.0f * ph)));
            processor.setMsegPoint(page, i, ph, juce::jlimit(0.0f, 1.0f, y)); processor.setMsegSegK(page, i, 0.0f); }
        processor.setMsegPointCount(page, n); selected = -1; selPoints.clear(); markCustom(); repaint(); }
    void vitalStroke() { auto pts = stroke; if (pts.size() < 2) return; // 1.7.8: vital-перо -- сглаженная кривая до 256 точек (без прореживания до 32)
        std::sort(pts.begin(), pts.end(), [](const Pt& a, const Pt& b) { return a.x < b.x; });
        std::vector<Pt> thin; for (const auto& p : pts) if (thin.empty() || p.x - thin.back().x >= 0.004f) thin.push_back(p); else thin.back().y = p.y;
        if (thin.size() < 2) return;
        for (int pass = 0; pass < 2; ++pass) { std::vector<Pt> sm(thin); for (size_t i = 1; i + 1 < sm.size(); ++i) sm[i].y = (thin[i - 1].y + thin[i].y * 2.0f + thin[i + 1].y) * 0.25f; thin = sm; }
        const float sx0 = thin.front().x, sx1 = thin.back().x;
        std::vector<Pt> merged; for (int i = 0; i < processor.msegPointCount(page); ++i) { const float x = processor.msegPointX(page, i); if (x < sx0 - 0.005f || x > sx1 + 0.005f) merged.push_back({x, processor.msegPointY(page, i), processor.msegSegK(page, i)}); }
        merged.insert(merged.end(), thin.begin(), thin.end());
        std::sort(merged.begin(), merged.end(), [](const Pt& a, const Pt& b) { return a.x < b.x; });
        if (merged.front().x > 0.02f) merged.insert(merged.begin(), {0.0f, merged.front().y, 0.0f});
        if (merged.back().x < 0.98f) merged.push_back({1.0f, merged.back().y, 0.0f});
        merged.front().x = 0.0f; merged.back().x = 1.0f;
        if (static_cast<int>(merged.size()) > 256) { std::vector<Pt> kept; for (int i = 0; i < 256; ++i) kept.push_back(merged[static_cast<size_t>(i * (static_cast<int>(merged.size()) - 1) / 255)]); merged.swap(kept); }
        processor.setMsegSteps(page, false);
        for (size_t i = 0; i < merged.size(); ++i) { processor.setMsegPoint(page, static_cast<int>(i), merged[i].x, merged[i].y); processor.setMsegSegK(page, static_cast<int>(i), 0.0f); } // 1.7.9: точки ДО count
        processor.setMsegPointCount(page, static_cast<int>(merged.size()));
        markCustom(); }
    std::vector<Pt> stroke; juce::Random rng; float randPhase = 0.0f; bool snapStroke = true, stepsPrimed = false; // 1.6.30
    int gridDen() const { const int den = static_cast<int>(gridDenF(grid.getSelectedItemIndex()) + 0.5f); return den > 0 ? den : 16; } // 1.6.30/1.7.7: квадраты -- по сетке GRID
    void rebuildStairs() { const int n = gridDen(); int k0 = -1, k1 = -1; // 1.6.31: ступени пишутся только в тронутом диапазоне колонок, остальная кривая жива
        for (int c = 0; c < n; ++c) if (stepTouched[static_cast<size_t>(c)]) { if (k0 < 0) k0 = c; k1 = c; }
        if (k0 < 0) return;
        const float x0 = static_cast<float>(k0) / static_cast<float>(n), x1 = static_cast<float>(k1 + 1) / static_cast<float>(n);
        std::vector<Pt> pts;
        for (const auto& q : stepOrig) if (q.x < x0 - 0.001f || q.x > x1 + 0.001f) pts.push_back(q);
        pts.push_back({x0, stepLevels[static_cast<size_t>(k0)], 0.0f});
        for (int c = k0 + 1; c <= k1; ++c) if (stepLevels[static_cast<size_t>(c)] != stepLevels[static_cast<size_t>(c - 1)]) { const float x = static_cast<float>(c) / static_cast<float>(n);
            if (static_cast<int>(pts.size()) + 2 > 256) break; // 1.7.8: ступени до 256 точек
            pts.push_back({x, stepLevels[static_cast<size_t>(c - 1)], 0.0f}); pts.push_back({x, stepLevels[static_cast<size_t>(c)], 0.0f}); }
        pts.push_back({x1, stepLevels[static_cast<size_t>(k1)], 0.0f});
        std::sort(pts.begin(), pts.end(), [](const Pt& a, const Pt& b) { return a.x < b.x; });
        if (static_cast<int>(pts.size()) > 256) { std::vector<Pt> kept; for (int i = 0; i < 256; ++i) kept.push_back(pts[static_cast<size_t>(i * (static_cast<int>(pts.size()) - 1) / 255)]); pts.swap(kept); } // 1.7.9: лимит 256 -- квадраты до конца
        if (pts.size() < 2) return;
        processor.setMsegPointCount(page, static_cast<int>(pts.size()));
        for (size_t i = 0; i < pts.size(); ++i) processor.setMsegPoint(page, static_cast<int>(i), pts[i].x, pts[i].y); }
    void bakeSteps() { const int n = processor.msegPointCount(page); if (n < 2) return; std::vector<Pt> out; // 1.6.22: ступеньки -> настоящие точки
        out.push_back({processor.msegPointX(page, 0), processor.msegPointY(page, 0), processor.msegSegK(page, 0)});
        for (int i = 1; i < n; ++i) { const float xi = processor.msegPointX(page, i);
            if (static_cast<int>(out.size()) + 2 <= 256) out.push_back({xi, processor.msegPointY(page, i - 1), 0.0f}); // 1.7.7: 256 точек -- ступеньки достают до конца
            if (static_cast<int>(out.size()) + 1 <= 256) out.push_back({xi, processor.msegPointY(page, i), 0.0f}); else break; }
        restore(out); }
    float sampled(const std::vector<Pt>& pts, float phase) const { if (pts.empty()) return 0.0f; if (phase <= pts.front().x) return pts.front().y;
        for (size_t i = 1; i < pts.size(); ++i) if (phase <= pts[i].x) { const float span = std::max(0.000001f, pts[i].x - pts[static_cast<size_t>(i) - 1].x); const float t = (phase - pts[static_cast<size_t>(i) - 1].x) / span;
            return pts[static_cast<size_t>(i) - 1].y + (pts[i].y - pts[static_cast<size_t>(i) - 1].y) * std::pow(t, std::pow(2.0f, pts[static_cast<size_t>(i) - 1].k)); }
        return pts.back().y; }
    void pushUndo() { if (pushEdit) pushEdit(); undo.push_back(snapshot()); if (undo.size() > 48) undo.erase(undo.begin()); redo.clear(); } // 1.6.32: каждая правка кривой = точка отката плагина
    void generate(const juce::String& what) {
        if (what == "FLAT") { processor.setMsegPoint(page, 0, 0.0f, 0.5f); processor.setMsegPoint(page, 1, 1.0f, 0.5f); processor.setMsegPointCount(page, 2); }
        else if (what == "RISE") { processor.setMsegPointCount(page, 2); processor.setMsegPoint(page, 0, 0.0f, 0.0f); processor.setMsegPoint(page, 1, 1.0f, 1.0f); }
        else if (what == "FALL") { processor.setMsegPointCount(page, 2); processor.setMsegPoint(page, 0, 0.0f, 1.0f); processor.setMsegPoint(page, 1, 1.0f, 0.0f); }
        else if (what == "TRI") { processor.setMsegPoint(page, 0, 0.0f, 0.0f); processor.setMsegPoint(page, 1, 0.5f, 1.0f); processor.setMsegPointCount(page, 3); processor.setMsegPoint(page, 2, 1.0f, 0.0f); }
        else if (what == "SINE") { for (int i = 0; i < 33; ++i) { const float t = i / 32.0f; processor.setMsegPoint(page, i, t, 0.5f - 0.5f * std::cos(2.0f * juce::MathConstants<float>::pi * t)); } processor.setMsegPointCount(page, 33); } // 1.7.9: точки ДО count
        else if (what == "SQUARE") { processor.setMsegPoint(page, 0, 0.0f, 1.0f); processor.setMsegPoint(page, 1, 0.5f, 1.0f); processor.setMsegPoint(page, 2, 0.5f, 0.0f); processor.setMsegPoint(page, 3, 1.0f, 0.0f); processor.setMsegPointCount(page, 4); } // 1.7.9: точки ДО count
        for (int i = 0; i < 31; ++i) processor.setMsegSegK(page, i, 0.0f); // 1.6.21
        selected = -1; repaint();
    }
    void tool(const juce::String& tl) {
        if (tl == "UNDO") { if (!undo.empty()) { redo.push_back(snapshot()); restore(undo.back()); undo.pop_back(); markCustom(); } }
        else if (tl == "REDO") { if (!redo.empty()) { undo.push_back(snapshot()); restore(redo.back()); redo.pop_back(); markCustom(); } }
        else if (tl == "INVERT") { pushUndo(); for (int i = 0; i < processor.msegPointCount(page); ++i) processor.setMsegPoint(page, i, processor.msegPointX(page, i), 1.0f - processor.msegPointY(page, i)); markCustom(); }
        else if (tl == "FLIP X") { pushUndo(); const auto pts = snapshot(); for (size_t i = 0; i < pts.size(); ++i) processor.setMsegPoint(page, static_cast<int>(i), pts[pts.size() - 1 - i].x, pts[pts.size() - 1 - i].y); for (int i = 0; i < processor.msegPointCount(page); ++i) processor.setMsegPoint(page, i, 1.0f - processor.msegPointX(page, i), processor.msegPointY(page, i)); markCustom(); }
        else if (tl == "COPY") clipboard = snapshot();
        else if (tl == "PASTE") { if (clipboard.size() >= 2) { pushUndo(); restore(clipboard); markCustom(); } }
        repaint();
    }
    int nearestSegment(juce::Point<float> p) const { int best = -1; float bestD = 7.0f; const int n = processor.msegPointCount(page);
        for (int i = 1; i < n; ++i) for (int s = 0; s <= 16; ++s) { const float t = s / 16.0f; const float shaped = std::pow(t, std::pow(2.0f, processor.msegSegK(page, i - 1)));
            const float px = pointToX(processor.msegPointX(page, i - 1) + (processor.msegPointX(page, i) - processor.msegPointX(page, i - 1)) * t);
            const float py = pointToY(processor.msegPointY(page, i - 1) + (processor.msegPointY(page, i) - processor.msegPointY(page, i - 1)) * shaped);
            const float d = p.getDistanceFrom({px, py}); if (d < bestD) { bestD = d; best = i - 1; } }
        return best; }
    bool isInterestedInFileDrag(const juce::StringArray& files) override { for (auto& f : files) if (f.endsWithIgnoreCase(".wav") || f.endsWithIgnoreCase(".wave") || f.endsWithIgnoreCase(".aif") || f.endsWithIgnoreCase(".aiff")) return true; return false; } // 1.7.7: вейвтейбл (Serum .wav) кидаем прямо в MSEG
    void filesDropped(const juce::StringArray& files, int, int) override {
        juce::AudioFormatManager fm; fm.registerBasicFormats();
        for (auto& f : files) { if (!(f.endsWithIgnoreCase(".wav") || f.endsWithIgnoreCase(".wave") || f.endsWithIgnoreCase(".aif") || f.endsWithIgnoreCase(".aiff"))) continue;
            std::unique_ptr<juce::AudioFormatReader> rd(fm.createReaderFor(juce::File(f))); if (!rd) continue;
            const int total = static_cast<int>(std::min<juce::int64>(juce::jmax<juce::int64>(rd->lengthInSamples, 2), 65536));
            std::vector<float> samples(static_cast<size_t>(total), 0.0f); float* ch[1]{samples.data()};
            rd->read(ch, 1, 0, total);
            float peak = 0.0f; for (float s : samples) peak = std::max(peak, std::abs(s)); if (peak < 1e-6f) peak = 1.0f;
            pushUndo(); processor.setMsegSteps(page, false);
            const int n = juce::jlimit(2, 256, std::min(total, 256)); // 1.7.7: один период -> до 256 точек (как вейвтейбл)
            for (int i = 0; i < n; ++i) { const int a = i * total / n, b = juce::jmax(i * total / n + 1, (i + 1) * total / n);
                float avg = 0.0f; for (int k = a; k < b && k < total; ++k) avg += samples[static_cast<size_t>(k)];
                avg /= static_cast<float>(juce::jmax(1, b - a));
                processor.setMsegPoint(page, i, n > 1 ? static_cast<float>(i) / static_cast<float>(n - 1) : 0.0f, juce::jlimit(0.0f, 1.0f, 0.5f + 0.5f * (avg / peak)));
                processor.setMsegSegK(page, i, 0.0f); }
            processor.setMsegPointCount(page, n); // 1.7.9: точки ДО count
            markCustom(); repaint(); break; } }
    int nearestPoint(juce::Point<float> p) const { int best = -1; float distance = 15.0f; for (int i = 0; i < processor.msegPointCount(page); ++i) { const float d = p.getDistanceFrom({pointToX(processor.msegPointX(page, i)), pointToY(processor.msegPointY(page, i))}); if (d < distance) { distance = d; best = i; } } return best; }
    float pointToX(float x) const { return graphX + x * graphW; }
    float pointToY(float y) const { return graphY + (1.0f - y) * graphH; }
    float toX(float x) const { return juce::jlimit(0.0f, 1.0f, (x - graphX) / graphW); }
    float toY(float y) const { return juce::jlimit(0.0f, 1.0f, 1.0f - (y - graphY) / graphH); }
    void timerCallback() override { // 1.7.7: маркер PLAY FOLLOWER с настройками ниже MSEG (делитель/фриз/off)
        if (!markerOff && !markerFrozen) { const double rp = juce::jlimit(0.0, 1.0, processor.msegPhaseUi[static_cast<size_t>(page)].load());
            double d = rp - markerLast; if (d < -0.5) d += 1.0; else if (d > 0.5) d -= 1.0;
            markerDisp = std::fmod(markerDisp + d / markerDiv + 1.0, 1.0); markerLast = rp; }
        refreshSyncBtns(); repaint(); } // 1.7.10: SYNC/TRIP/DOT следят за rmode страницы
    MonomachineNovaAudioProcessor& processor; int page = 0;
    juce::ToggleButton loop, sync, keyBtn; juce::Slider rate, shift, lpoint; DragChoice shape, grid, viewGrid, retrig; PixelButton linkBtn{"LINK"}, syncModeBtn{"SYNC"}, tripBtn{"TRIP"}, dotBtn{"DOT"}; // 1.7.10: rmodeBox -> кнопки как в Serum (SYNC = BPM/TIME, TRIPLET, DOTTED) // 1.7.8: +KEYTRACK/LOOP POINT // 1.7.7: retrig -- список режимов (DragChoice)
    int straightIdx = 12, snapSavedIdx = 12; std::vector<Pt> customBackup; bool lastShapeCustom = true; // 1.7.7: индекс 1/16 в новом списке сетки; 1.6.29: память CUSTOM-волны
    PixelButton stepsButton{"STEPS"}; PixelButton clearButton{"clear"}; juce::OwnedArray<PixelButton> formButtons, toolButtons, pageButtons; // 1.6.29: страницы списком
    std::unique_ptr<TextTag> tagRows; std::unique_ptr<TextTag> tagHeads; // 1.6.42/1.7.5
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> loopLink, syncLink, keyLink; std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> retrigLink; std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rateLink, lpointLink; // 1.7.10: rmodeLink убран (кнопки пишут rmode напрямую)
    int selected = -1, routeDrag = -1, routeY0 = 0, hoverSeg = -1, bendSeg = -1, bendY = 0; float bendBase = 0, routeBase = 0; juce::String shiftLabel{"0.0 DEG"};
    int drawMode = 0; bool drawing = false; bool stepsBaked = false; std::array<float, 64> stepLevels{}; std::vector<Pt> bakedBackup; std::vector<Pt> stepOrig; std::array<bool, 64> stepTouched{}; // 1.6.31: ступени -- только в тронутых колонках
    bool draggingPoint = false, erasing = false;
    std::vector<std::vector<Pt>> undo, redo; std::vector<Pt> base, clipboard; std::array<int, 8> routeRows{{-1,-1,-1,-1,-1,-1,-1,-1}};
    int graphX = 16, graphY = 146, graphW = 984, graphH = 264; // 1.7.1: окно MSEG редактируемого размера; 1.7.10: граф ниже на 28px (два ряда управления), высота чуть меньше
    bool resizing = false; int resW = 0, resH = 0, resX = 0, resY = 0; // 1.7.1
};

class EnvelopePage final : public juce::Component,private juce::Timer {
public:
    explicit EnvelopePage(MonomachineNovaAudioProcessor& p,bool compact=false,bool p2eng=false):processor(p),mini(compact),p2Env(p2eng){ // 1.8.0: p2eng -- окно огибающей P2 (p2_amp_*)
        if(!mini){
            for(auto* b:{&ampTab,&filTab,&modTab}){b->textHeight=14;addAndMakeVisible(*b);}
            ampTab.setToggleState(true,juce::dontSendNotification);
            ampTab.onClick=[this]{if(selectTab)selectTab(0);};filTab.onClick=[this]{if(selectTab)selectTab(1);};modTab.onClick=[this]{if(selectTab)selectTab(2);};
        }
        mode.setScrollWheelEnabled(true);mode.smallPopup=true;mode.addItem("old",1);mode.addItem("mnm",2);mode.addItem("vital",3);addAndMakeVisible(mode);
        mode.setTooltip("old = legacy envelope; mnm = current kernel ADSR; vital = curve variant. Native/default remains mnm.");
        modeLink=std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(processor.parameters,p2Env?"p2_amp_mode":"amp_mode",mode); // 1.8.0: +P2
        mode.speedKey="MODE AMP"; // 1.6.30: ЛКМ-драг листает, обычный клик открывает список
        if(mini)mode.setVisible(false); // 1.8.0b: в малом окне своя MODE скрыта -- основная (MODE AMP) уже на лицевой (просьба юзера)
        juce::String ids[7];{const char* a[]{"p0_0","p0_1","p0_2","p0_3","amp_curve_a","amp_curve_d","amp_curve_r"};const char* b[]{"p2_amp_atk","p2_amp_hold","p2_amp_dec","p2_amp_rel","p2_amp_curve_a","p2_amp_curve_d","p2_amp_curve_r"};for(int i=0;i<7;++i)ids[i]=p2Env?juce::String(b[i]):juce::String(a[i]);} // 1.8.0: P2-огибающая
        const char* names[]{"ATK","HOLD","DEC","REL","ATK CURVE","DEC CURVE","REL CURVE"};
        for(size_t i=0;i<7;++i){auto& sl=sliders[i];sl.setColour(juce::Slider::thumbColourId,ink);sl.setColour(juce::Slider::trackColourId,ink.withAlpha(0.8f));sl.setColour(juce::Slider::backgroundColourId,ink.withAlpha(0.2f));sl.setSliderStyle(juce::Slider::LinearHorizontal);sl.setTextBoxStyle(juce::Slider::TextBoxRight,false,62,22);sl.setName(names[i]);sl.setDoubleClickReturnValue(true,i<4?(i<2?0:64):0);addAndMakeVisible(sl);links[i]=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters,ids[i],sl);labels[i].setText(names[i],juce::dontSendNotification);addAndMakeVisible(labels[i]);}
        gate.setColour(juce::Slider::thumbColourId,ink);gate.setColour(juce::Slider::trackColourId,ink.withAlpha(0.8f));gate.setColour(juce::Slider::backgroundColourId,ink.withAlpha(0.2f));gate.setRange(10,10000,1);gate.setValue(1000);gate.setSliderStyle(juce::Slider::LinearHorizontal);gate.setTextBoxStyle(juce::Slider::TextBoxRight,false,62,22);addAndMakeVisible(gate);
        gateLabel.setText("PREVIEW NOTE (ms)",juce::dontSendNotification);addAndMakeVisible(gateLabel);
        if(mini){gx=8;gy=32;gw=371;gh=114; // 1.7.11 FIX: MODE виден и в малом окне (тот же amp_mode -- синхронно с лицевой и большим окном)
            for(auto& sl:sliders)sl.setTextBoxStyle(juce::Slider::TextBoxRight,false,34,14);
            gate.setTextBoxStyle(juce::Slider::TextBoxRight,false,34,14);setSize(387,236);}else setSize(1170,465);startTimerHz(15);refresh();
    }
    void focusCurve(int knob){selected=knob==0?4:knob==1?-1:knob==2?5:6; // 1.6.13: HOLD открывает страницу без фокуса кривой
        if(selected>=0&&sliders[static_cast<size_t>(selected)].isEnabled())sliders[static_cast<size_t>(selected)].grabKeyboardFocus();repaint();}
    void resized()override{if(mini){mode.setBounds(232,4,150,20);for(int i=0;i<8;++i){const int x=8+(i%4)*95,y=152+(i/4)*38;if(i<7){labels[static_cast<size_t>(i)].setBounds(x,y,92,12);sliders[static_cast<size_t>(i)].setBounds(x,y+12,92,22);}else{gateLabel.setBounds(x,y,92,12);gate.setBounds(x,y+12,92,22);}}return;} // 1.6.30: компактная раскладка
        ampTab.setBounds(20,7,106,26);filTab.setBounds(130,7,106,26);modTab.setBounds(240,7,106,26);mode.setBounds(850,8,300,26);for(int i=0;i<8;++i){const int x=20+(i%4)*285,y=330+(i/4)*58;if(i<7){labels[static_cast<size_t>(i)].setBounds(x,y,260,20);sliders[static_cast<size_t>(i)].setBounds(x,y+20,265,28);}else{gateLabel.setBounds(x,y,265,20);gate.setBounds(x,y+20,265,28);}}} // 1.6.19: mode combo restored (x/y из 1.6.14)
    void paint(juce::Graphics& g)override{
        g.fillAll(juce::Colours::black);g.setColour(ink);
        if(mini){g.drawRect(0,0,387,236,1);g.drawHorizontalLine(28,0.0f,387.0f);g.drawVerticalLine(96,0.0f,28.0f);g.drawVerticalLine(288,0.0f,28.0f);} // 1.6.43 FIX: разделители по НАСТОЯЩИМ кнопкам AMP|MODE (96/288), а не треть ширины // 1.6.42: рамки AMP|MNM -- клетки для НАСТОЯЩИХ кнопок (скрин mode env digital reference)
        else pixel::text(g,"AMP / ENVELOPE",{365,8,420,26},22);
        const juce::Rectangle<float> box(static_cast<float>(gx),static_cast<float>(gy),static_cast<float>(gw),static_cast<float>(gh));g.setColour(ink.withAlpha(0.2f));g.drawRect(box);
        for(int i=1;i<4;++i)g.drawHorizontalLine(gy+i*gh/4,static_cast<float>(gx),static_cast<float>(gx+gw));
        for(int i=0;i<=4;++i){const float x=static_cast<float>(gx+gw*i/4);g.drawVerticalLine(juce::roundToInt(x),static_cast<float>(gy),static_cast<float>(gy+gh));if(!mini){g.setColour(ink);g.drawText(juce::String(duration*i/4,2)+" s",juce::roundToInt(x)-25,gy+gh+1,70,20,juce::Justification::centred);g.setColour(ink.withAlpha(0.2f));}}
        juce::Path path;for(size_t i=0;i<trace.size();++i){const float x=static_cast<float>(gx)+static_cast<float>(gw)*static_cast<float>(i)/std::max(1.0f,static_cast<float>(trace.size()-1));const float y=static_cast<float>(gy+gh)-static_cast<float>(gh)*trace[i];if(i==0)path.startNewSubPath(x,y);else path.lineTo(x,y);}g.setColour(ink);g.strokePath(path,juce::PathStrokeType(1.8f));
        const float off=static_cast<float>(gx)+static_cast<float>(gw)*static_cast<float>(gate.getValue()/1000/duration);if(off<gx+gw){g.setColour(ink.withAlpha(0.5f));g.drawVerticalLine(juce::roundToInt(off),static_cast<float>(gy),static_cast<float>(gy+gh));g.drawText("NOTE OFF",juce::roundToInt(off)+4,gy+2,85,18,juce::Justification::left);}
        if(!mini){g.setColour(ink);g.drawText(mode.getSelectedId()==1?"old = legacy envelope.":mode.getSelectedId()==2?"mnm = current kernel ADSR. Drag graph vertically: decay before NOTE OFF, release after.":"vital = curve variant (your curve controls still win when set).",20,307,1130,20,juce::Justification::left);}
        if(selected>=4){auto r=sliders[static_cast<size_t>(selected)].getBounds().expanded(2);g.drawRect(r);}
    }
    void mouseDown(const juce::MouseEvent& e)override{if(e.mods.isPopupMenu()){if(toggleSize)toggleSize();return;} // 1.7.5: ПКМ -- малое/большое окно, ПКМ-кручения нет
        if(e.y<gy||e.y>gy+gh||mode.getSelectedId()!=2)return;dragId=(e.x-gx)/static_cast<double>(gw)*duration<gate.getValue()/1000?"amp_curve_d":"amp_curve_r";dragParameter=processor.parameters.getParameter(dragId);origin=e.y;start=dragParameter->convertFrom0to1(dragParameter->getValue());dragParameter->beginChangeGesture();}
    void mouseDrag(const juce::MouseEvent& e)override{if(dragParameter)dragParameter->setValueNotifyingHost(dragParameter->convertTo0to1(juce::jlimit(-100.0f,100.0f,start+static_cast<float>(origin-e.y))));}
    void mouseUp(const juce::MouseEvent&)override{endGesture();}
    ~EnvelopePage()override{endGesture();}
private:
    void endGesture(){if(dragParameter){dragParameter->endChangeGesture();dragParameter=nullptr;}}
    void timerCallback()override{refresh();}
    void refresh(){std::array<double,9> values{};for(size_t i=0;i<7;++i)values[i]=sliders[i].getValue();values[7]=mode.getSelectedId();values[8]=gate.getValue();if(values==previous)return;previous=values;for(size_t i=4;i<7;++i)sliders[i].setEnabled(mode.getSelectedId()==2);
        nova::AmpEnvelope env;const int previewMode=mode.getSelectedId()==2?nova::AmpEnvelope::kKernelAlgorithm:(mode.getSelectedId()==3?3:0);env.configure(previewMode,{static_cast<float>(values[4]),static_cast<float>(values[5]),static_cast<float>(values[6])});env.reset(2000);for(size_t i=0;i<4;++i)env.p[i]=static_cast<float>(values[i]);env.on();std::vector<float> samples; samples.reserve(40000);
        for(int i=0;i<40000;++i){if(i==juce::roundToInt(gate.getValue()*2))env.off();samples.push_back(env.tick());if(i>2&&env.stage==0)break;}
        duration=std::max(0.002,static_cast<double>(samples.size())/2000);trace.clear();for(size_t i=0;i<1000;++i)trace.push_back(samples[std::min(samples.size()-1,i*samples.size()/1000)]);repaint();}
public:
    std::function<void()> toggleSize; // 1.7.5: переключение малое/большое (замыкает Surface)
    std::function<void(int)> selectTab;
    MonomachineNovaAudioProcessor& processor;PixelButton ampTab{"AMP ENV"},filTab{"FIL ENV"},modTab{"MOD ENV"};DragChoice mode;std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> modeLink;
    std::array<juce::Slider,7> sliders;std::array<juce::Label,7> labels;std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>,7> links;
    juce::Slider gate;juce::Label gateLabel;std::array<double,9> previous{};std::vector<float> trace;double duration=1;int selected=-1,origin=0;float start=0;juce::String dragId;bool p2Env=false;juce::RangedAudioParameter* dragParameter=nullptr;bool mini=false;int gx=30,gy=60,gw=1110,gh=225;
};

// Supplemental FIL ENV: separate from the original FILT ATK/DEC/BOFS/WOFS path.
// ENV FIL is the opt-in amount; BASE and WDTH are signed additive depths.
class FilterEnvelopePage final : public juce::Component, private juce::Timer {
public:
    explicit FilterEnvelopePage(MonomachineNovaAudioProcessor& p,bool p2Page):processor(p),p2(p2Page){
        for(auto* b:{&ampTab,&filTab,&modTab}){b->textHeight=14;addAndMakeVisible(*b);}
        filTab.setToggleState(true,juce::dontSendNotification);
        ampTab.onClick=[this]{if(selectTab)selectTab(0);};filTab.onClick=[this]{if(selectTab)selectTab(1);};modTab.onClick=[this]{if(selectTab)selectTab(2);};
        const char* ids[]{"env_atk","env_hold","env_dec","env_rel","env_mix","env_base_depth","env_width_depth"};
        const char* names[]{"ATK","HOLD","DEC","REL","ENV FIL","BASE","WDTH"};
        const float defaults[]{0,0,127,127,0,0,0};
        const auto prefix=p2?juce::String("p2_filt_"):juce::String("filt_");
        for(size_t i=0;i<sliders.size();++i){
            sliders[i].setSliderStyle(juce::Slider::LinearHorizontal);sliders[i].setTextBoxStyle(juce::Slider::TextBoxRight,false,58,22);
            sliders[i].setColour(juce::Slider::thumbColourId,ink);sliders[i].setColour(juce::Slider::trackColourId,ink.withAlpha(0.8f));sliders[i].setColour(juce::Slider::backgroundColourId,ink.withAlpha(0.18f));
            sliders[i].setDoubleClickReturnValue(true,defaults[i]);labels[i].setText(names[i],juce::dontSendNotification);addAndMakeVisible(labels[i]);addAndMakeVisible(sliders[i]);
            links[i]=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.parameters,prefix+ids[i],sliders[i]);
        }
        setSize(1170,465);startTimerHz(15);refresh();
    }
    ~FilterEnvelopePage()override{stopTimer();}
    void resized()override{ampTab.setBounds(20,7,106,26);filTab.setBounds(130,7,106,26);modTab.setBounds(240,7,106,26);
        for(int i=0;i<7;++i){const int x=20+(i%4)*285,y=290+(i/4)*62;labels[(size_t)i].setBounds(x,y,260,20);sliders[(size_t)i].setBounds(x,y+20,265,28);}}
    void paint(juce::Graphics& g)override{g.fillAll(juce::Colours::black);g.setColour(ink);pixel::text(g,(p2?"P2 ":"P1 ")+juce::String("FILT / SUPPLEMENTAL ENVELOPE"),{365,8,620,26},21);
        pixel::text(g,"Independent from native FILT ATK / DEC / BOFS / WOFS. ENV FIL = 0 is no effect.",{20,41,1110,17},12,false);
        const juce::Rectangle<float> box(20.0f,65.0f,1110.0f,180.0f);g.setColour(ink.withAlpha(0.2f));g.drawRect(box);for(int i=1;i<4;++i)g.drawHorizontalLine(65+i*45,20.0f,1130.0f);
        juce::Path path;for(size_t i=0;i<trace.size();++i){const float x=20.0f+1110.0f*(float)i/(float)std::max<size_t>(1,trace.size()-1);const float y=245.0f-180.0f*trace[i];if(i==0)path.startNewSubPath(x,y);else path.lineTo(x,y);}g.setColour(ink);g.strokePath(path,juce::PathStrokeType(1.8f));
        pixel::text(g,"Envelope routes additively through BASE / WDTH depths.",{20,262,760,16},12,false);
    }
    std::function<void(int)> selectTab;
private:
    void timerCallback()override{refresh();}
    void refresh(){std::array<double,4> now{};for(size_t i=0;i<4;++i)now[i]=sliders[i].getValue();if(now==previous)return;previous=now;
        nova::AmpEnvelope env;env.configure(nova::AmpEnvelope::kKernelAlgorithm,{});env.reset(2000);for(size_t i=0;i<4;++i)env.p[i]=(float)now[i];env.on();
        std::vector<float> raw;raw.reserve(40000);for(int i=0;i<40000;++i){if(i==1200)env.off();raw.push_back(env.tick());if(i>1202&&env.stage==0)break;}trace.clear();for(size_t i=0;i<900;++i)trace.push_back(raw[std::min(raw.size()-1,i*raw.size()/900)]);repaint();}
    MonomachineNovaAudioProcessor& processor;bool p2=false;PixelButton ampTab{"AMP ENV"},filTab{"FIL ENV"},modTab{"MOD ENV"};
    std::array<juce::Slider,7> sliders;std::array<juce::Label,7> labels;std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>,7> links;
    std::array<double,4> previous{};std::vector<float> trace;
};

// Four global MOD ENV pages.  They are sources 32..35 in the modulation matrix,
// so their page selector deliberately has no P1/P2 binding.
class ModEnvelopePage final : public juce::Component, private juce::Timer {
public:
    explicit ModEnvelopePage(MonomachineNovaAudioProcessor& p,int initial=0):processor(p),page(juce::jlimit(0,3,initial)){
        for(auto* b:{&ampTab,&filTab,&modTab}){b->textHeight=14;addAndMakeVisible(*b);}
        modTab.setToggleState(true,juce::dontSendNotification);
        ampTab.onClick=[this]{if(selectTab)selectTab(0);};filTab.onClick=[this]{if(selectTab)selectTab(1);};modTab.onClick=[this]{if(selectTab)selectTab(2);};
        for(int i=0;i<4;++i){pageButtons[(size_t)i].setButtonText(juce::String(i+1));pageButtons[(size_t)i].textHeight=15;pageButtons[(size_t)i].onClick=[this,i]{selectPage(i);};addAndMakeVisible(pageButtons[(size_t)i]);}
        routeButton.textHeight=14;routeButton.setButtonText("ROUTE TO...");routeButton.onClick=[this]{if(addRoute)addRoute(page);};addAndMakeVisible(routeButton);
        const char* names[]{"ATK","HOLD","DEC","REL"};const float defaults[]{0,0,127,127};
        for(size_t i=0;i<sliders.size();++i){sliders[i].setSliderStyle(juce::Slider::LinearHorizontal);sliders[i].setTextBoxStyle(juce::Slider::TextBoxRight,false,62,22);sliders[i].setColour(juce::Slider::thumbColourId,ink);sliders[i].setColour(juce::Slider::trackColourId,ink.withAlpha(0.8f));sliders[i].setColour(juce::Slider::backgroundColourId,ink.withAlpha(0.18f));sliders[i].setDoubleClickReturnValue(true,defaults[i]);labels[i].setText(names[i],juce::dontSendNotification);addAndMakeVisible(labels[i]);addAndMakeVisible(sliders[i]);}
        rebind();setSize(1170,465);startTimerHz(15);refresh();
    }
    ~ModEnvelopePage()override{stopTimer();}
    void resized()override{ampTab.setBounds(20,7,106,26);filTab.setBounds(130,7,106,26);modTab.setBounds(240,7,106,26);for(int i=0;i<4;++i)pageButtons[(size_t)i].setBounds(20+i*42,43,36,24);routeButton.setBounds(1000,42,130,25);for(int i=0;i<4;++i){const int x=20+i*285;labels[(size_t)i].setBounds(x,300,260,20);sliders[(size_t)i].setBounds(x,320,265,28);}}
    void paint(juce::Graphics& g)override{g.fillAll(juce::Colours::black);g.setColour(ink);pixel::text(g,"MOD ENV / "+juce::String(page+1)+"  ·  MATRIX SOURCE",{365,8,560,26},21);pixel::text(g,"Select one of four independent envelopes, then use it as MOD ENV"+juce::String(page+1)+" in MATRIX.",{210,47,700,17},12,false);
        const juce::Rectangle<float> box(20.0f,82.0f,1110.0f,185.0f);g.setColour(ink.withAlpha(0.2f));g.drawRect(box);for(int i=1;i<4;++i)g.drawHorizontalLine(82+i*46,20.0f,1130.0f);
        juce::Path path;for(size_t i=0;i<trace.size();++i){const float x=20.0f+1110.0f*(float)i/(float)std::max<size_t>(1,trace.size()-1);const float y=267.0f-185.0f*trace[i];if(i==0)path.startNewSubPath(x,y);else path.lineTo(x,y);}g.setColour(ink);g.strokePath(path,juce::PathStrokeType(1.8f));pixel::text(g,"ROUTE TO... creates a matrix route with this MOD ENV source.",{20,278,820,15},12,false);
    }
    std::function<void(int)> selectTab;std::function<void(int)> addRoute;
private:
    void selectPage(int next){if(next==page)return;page=juce::jlimit(0,3,next);rebind();previous={};refresh();}
    void rebind(){for(size_t i=0;i<links.size();++i){links[i].reset();const char* field[]{"atk","hold","dec","rel"};links[i]=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.parameters,"modenv"+juce::String(page+1)+"_"+field[i],sliders[i]);}for(int i=0;i<4;++i)pageButtons[(size_t)i].setToggleState(i==page,juce::dontSendNotification);repaint();}
    void timerCallback()override{refresh();}
    void refresh(){std::array<double,4> now{};for(size_t i=0;i<4;++i)now[i]=sliders[i].getValue();if(now==previous)return;previous=now;nova::AmpEnvelope env;env.configure(nova::AmpEnvelope::kKernelAlgorithm,{});env.reset(2000);for(size_t i=0;i<4;++i)env.p[i]=(float)now[i];env.on();std::vector<float> raw;raw.reserve(40000);for(int i=0;i<40000;++i){if(i==1200)env.off();raw.push_back(env.tick());if(i>1202&&env.stage==0)break;}trace.clear();for(size_t i=0;i<900;++i)trace.push_back(raw[std::min(raw.size()-1,i*raw.size()/900)]);repaint();}
    MonomachineNovaAudioProcessor& processor;int page=0;PixelButton ampTab{"AMP ENV"},filTab{"FIL ENV"},modTab{"MOD ENV"};std::array<PixelButton,4> pageButtons{PixelButton{"1"},PixelButton{"2"},PixelButton{"3"},PixelButton{"4"}};PixelButton routeButton{"ROUTE TO..."};
    std::array<juce::Slider,4> sliders;std::array<juce::Label,4> labels;std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>,4> links;std::array<double,4> previous{};std::vector<float> trace;
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


// ===== 1.8.4: вертикальная страница FX-SLOTS =================================
// Шляпки не являются новыми DSP-модулями: это подписи над уже существующими
// стадиями P1/P2. Реальные интерактивные элементы -- JUCE-кнопки, тумблеры,
// ручки и drag-handle; нарисованных «кнопок» здесь нет.
class FxSlotDragHandle final : public juce::Component, public juce::SettableTooltipClient {
public:
    std::function<void(juce::Point<int>)> began, moved, dropped;
    FxSlotDragHandle(){setMouseCursor(juce::MouseCursor::DraggingHandCursor);setTooltip("DRAG: move this slot to any chain gap");}
    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::black);g.setColour(ink);g.drawRect(getLocalBounds(),1);
        for(int y=7;y<getHeight()-4;y+=6){g.fillRect(getWidth()/2-4,y,3,2);g.fillRect(getWidth()/2+2,y,3,2);}
    }
    void mouseDown(const juce::MouseEvent& e) override { dragging=true;if(began)began(e.getScreenPosition()); }
    void mouseDrag(const juce::MouseEvent& e) override { if(dragging&&moved)moved(e.getScreenPosition()); }
    void mouseUp(const juce::MouseEvent& e) override { if(dragging&&dropped)dropped(e.getScreenPosition());dragging=false; }
private: bool dragging=false;
};

class FxSlotCard final : public juce::Component {
public:
    static constexpr int kWidth=760, kHeight=134;
    FxSlotCard(MonomachineNovaAudioProcessor& p,int index):processor(p),slot(index),machineButton("CHOOSE MACHINE"),removeButton("X"){
        addAndMakeVisible(handle);addAndMakeVisible(machineButton);addAndMakeVisible(onButton);addAndMakeVisible(removeButton);
        machineButton.textHeight=13;machineButton.textAlign=1;machineButton.setTooltip("MACHINE: native FX list, the same order and names as P2");
        removeButton.textHeight=14;removeButton.setTooltip("REMOVE SLOT: delete this user slot from the chain");
        onButton.setButtonText("ON");onButton.setTooltip("ON: bypass or enable the selected FX slot");
        for(int i=0;i<8;++i){
            auto& knob=knobs[static_cast<size_t>(i)];knob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);knob.setTextBoxStyle(juce::Slider::NoTextBox,false,0,0);knob.setScrollWheelEnabled(true);
            knob.onValueChange=[this,i]{if(!syncing)processor.fxSlotSetParameter(slot,i,static_cast<float>(knobs[static_cast<size_t>(i)].getValue()));};
            addAndMakeVisible(knob);
        }
        machineButton.onClick=[this]{showMachineMenu();};
        onButton.onClick=[this]{if(!syncing)processor.fxSlotSetOn(slot,onButton.getToggleState());};
        removeButton.onClick=[this]{processor.fxSlotRemove(slot);if(changed)changed();};
        handle.began=[this](juce::Point<int> q){if(dragBegin)dragBegin(slot,q);};
        handle.moved=[this](juce::Point<int> q){if(dragMove)dragMove(slot,q);};
        handle.dropped=[this](juce::Point<int> q){if(dragDrop)dragDrop(slot,q);};
    }
    std::function<void()> changed;
    std::function<void(int,juce::Point<int>)> dragBegin,dragMove,dragDrop;
    int index() const noexcept{return slot;}
    void sync(const MonomachineNovaAudioProcessor::FxSlotState& next){
        syncing=true;state=next;
        const auto* def=findMachine(next.machineId);
        machineButton.setButtonText(def?juce::String(def->name):"CHOOSE MACHINE");
        onButton.setEnabled(def!=nullptr);onButton.setToggleState(next.on,juce::dontSendNotification);
        for(int i=0;i<8;++i){
            auto& knob=knobs[static_cast<size_t>(i)];
            if(def!=nullptr&&def->synthParams[static_cast<size_t>(i)].maxVal>0){
                const auto& pdef=def->synthParams[static_cast<size_t>(i)];labels[static_cast<size_t>(i)]=pdef.name;
                knob.setRange(pdef.minVal,pdef.maxVal,1.0);knob.setValue(next.p[static_cast<size_t>(i)],juce::dontSendNotification);knob.setVisible(true);
            }else{labels[static_cast<size_t>(i)]="---";knob.setVisible(false);}
        }
        syncing=false;repaint();
    }
    void resized() override {
        handle.setBounds(6,7,30,28);machineButton.setBounds(42,7,242,28);onButton.setBounds(292,7,58,28);removeButton.setBounds(720,7,32,28);
        for(int i=0;i<8;++i)knobs[static_cast<size_t>(i)].setBounds(12+i*92,48,72,72);
    }
    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::black);g.setColour(ink);g.drawRect(getLocalBounds(),1);
        pixel::text(g,"SLOT "+juce::String(slot+1)+"  ·  g"+juce::String(state.zone),{366,10,330,22},12,false);
        for(int i=0;i<8;++i){
            const auto x=12+i*92;
            g.setColour(knobs[static_cast<size_t>(i)].isVisible()?ink:dim);pixel::text(g,labels[static_cast<size_t>(i)],{x,35,72,12},11,true);
            if(knobs[static_cast<size_t>(i)].isVisible())pixel::text(g,juce::String(juce::roundToInt(knobs[static_cast<size_t>(i)].getValue())),{x,118,72,13},11,true);
        }
    }
private:
    const monomachine::MachineDef* findMachine(int id) const {
        for(const auto& m:MonomachineNovaAudioProcessor::p2FxMachines())if(m.id==id)return &m;
        return nullptr;
    }
    void showMachineMenu(){
        const auto& fxm=MonomachineNovaAudioProcessor::p2FxMachines();
        juce::PopupMenu menu;menu.setLookAndFeel(&smallMenuLaf());
        // Не свой список: это тот же p2FxMachines() и тот же порядок, что у P2.
        for(int i=0;i<static_cast<int>(fxm.size());++i)menu.addItem(i+1,juce::String(fxm[static_cast<size_t>(i)].name),true,fxm[static_cast<size_t>(i)].id==state.machineId);
        menu.addSeparator();menu.addItem(1000,"EMPTY",true,state.machineId==0);
        const juce::Component::SafePointer<FxSlotCard> safe(this);
        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(&machineButton),[safe](int result){
            if(!safe||result<=0)return;
            int id=0;if(result!=1000){const auto& list=MonomachineNovaAudioProcessor::p2FxMachines();id=list[static_cast<size_t>(result-1)].id;}
            safe->processor.fxSlotSetMachine(safe->slot,id);if(safe->changed)safe->changed();
        });
    }
    MonomachineNovaAudioProcessor& processor;int slot;
    MonomachineNovaAudioProcessor::FxSlotState state{};bool syncing=false;
    FxSlotDragHandle handle;PixelButton machineButton,removeButton;juce::ToggleButton onButton;
    std::array<CurveSlider,8> knobs{};std::array<juce::String,8> labels{};
};

class FxFeedbackCard final : public juce::Component {
public:
    static constexpr int kWidth=760,kHeight=142;
    explicit FxFeedbackCard(MonomachineNovaAudioProcessor& p):processor(p),sendButton("SEND"),returnButton("RETURN"){
        addAndMakeVisible(onButton);addAndMakeVisible(clipButton);addAndMakeVisible(sendButton);addAndMakeVisible(returnButton);
        onButton.setButtonText("ON");onButton.setTooltip("Enable FB LOOP 1. The loop itself is exactly one sample; no block-delay shortcut.");
        clipButton.setButtonText("CLIP");clipButton.setTooltip("FB CLIP: ON = explicit ±1 hard clip in the recurrence. OFF = no clip and no automatic limiter; use SEND/FB/RETURN/DC HP deliberately.");
        sendButton.textHeight=12;returnButton.textHeight=12;
        sendButton.setTooltip("SEND: choose the chain bridge sampled after its FX-slots.");
        returnButton.setTooltip("RETURN: choose the chain bridge receiving the previous FB sample before its FX-slots.");
        const char* names[]{"SEND","FB","RETURN","DC HP"};
        for(int i=0;i<4;++i){
            auto& knob=knobs[static_cast<size_t>(i)];knob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);knob.setTextBoxStyle(juce::Slider::NoTextBox,false,0,0);knob.setRange(0,127,1);knob.setScrollWheelEnabled(true);
            knob.setTooltip(i==0?"SEND gain: 0..about +6 dB into the FB recurrence.":i==1?"FB gain: manual recurrence only. There is no automatic limiter.":i==2?"RETURN gain: 0..about +6 dB back into the selected bridge.":"DC HP: 0.1..100 Hz; raw 64 stays about 1 Hz. Up to 100 Hz is available for deliberate low-cut in the loop.");
            knob.onValueChange=[this,i]{if(syncing)return;const float v=static_cast<float>(knobs[static_cast<size_t>(i)].getValue());if(i==0)processor.feedbackSetSendGain(v);else if(i==1)processor.feedbackSetFeedbackGain(v);else if(i==2)processor.feedbackSetReturnGain(v);else processor.feedbackSetDcControl(v);};
            addAndMakeVisible(knob);labels[static_cast<size_t>(i)]=names[i];
        }
        onButton.onClick=[this]{if(!syncing)processor.feedbackSetOn(onButton.getToggleState());};
        clipButton.onClick=[this]{if(!syncing)processor.feedbackSetClipEnabled(clipButton.getToggleState());};
        sendButton.onClick=[this]{showZoneMenu(true);};returnButton.onClick=[this]{showZoneMenu(false);};
    }
    void sync(const MonomachineNovaAudioProcessor::FeedbackState& state){
        syncing=true;current=state;onButton.setToggleState(state.on,juce::dontSendNotification);clipButton.setToggleState(state.clipEnabled,juce::dontSendNotification);
        knobs[0].setValue(state.sendGain,juce::dontSendNotification);knobs[1].setValue(state.feedbackGain,juce::dontSendNotification);knobs[2].setValue(state.returnGain,juce::dontSendNotification);knobs[3].setValue(state.dcControl,juce::dontSendNotification);
        sendButton.setButtonText("SEND g"+juce::String(state.sendZone)+" · "+zoneCaption(state.sendZone));
        returnButton.setButtonText("RETURN g"+juce::String(state.returnZone)+" · "+zoneCaption(state.returnZone));
        syncing=false;repaint();
    }
    void resized() override {
        onButton.setBounds(10,7,50,26);clipButton.setBounds(68,7,60,26);sendButton.setBounds(138,7,260,26);returnButton.setBounds(408,7,342,26);
        for(int i=0;i<4;++i)knobs[static_cast<size_t>(i)].setBounds(36+i*180,52,76,76);
    }
    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::black);g.setColour(ink);g.drawRect(getLocalBounds(),1);
        pixel::text(g,current.clipEnabled?"FB LOOP 1 · 1 SAMPLE · HARD CLIP ±1 · NO AUTO LIMITER":"FB LOOP 1 · 1 SAMPLE · CLIP BYPASSED · NO AUTO LIMITER",{12,35,680,14},12,false);
        for(int i=0;i<4;++i){const int x=36+i*180;g.setColour(ink);pixel::text(g,labels[static_cast<size_t>(i)],{x,48,76,12},11,true);
            juce::String value;
            if(i<3)value=juce::String(juce::roundToInt(knobs[static_cast<size_t>(i)].getValue()));
            else {const float hz=MonomachineNovaAudioProcessor::feedbackDcHz(static_cast<float>(knobs[3].getValue()));value=hz<1.0f?juce::String(hz,2):juce::String(hz,1);value+=" Hz";}
            pixel::text(g,value,{x-12,126,100,13},11,true);
        }
        g.setColour(current.on?ink:dim);pixel::text(g,current.on?"ACTIVE: samplewise topology for exact 1-sample FB":"OFF: empty route is bit-transparent",{660,39,92,35},10,true);
    }
private:
    static juce::String zoneCaption(int zone){
        switch(zone){
            case MonomachineNovaAudioProcessor::fxG_P1pre:return "P1 IN";
            case MonomachineNovaAudioProcessor::fxG_EQ_FILT:return "EQ · FILT";
            case MonomachineNovaAudioProcessor::fxG_FILT_DIST:return "FILT · DIST";
            case MonomachineNovaAudioProcessor::fxG_DIST_ENV:return "DIST · ENV";
            case MonomachineNovaAudioProcessor::fxG_ENV_VOLPAN:return "ENV · VOL/PAN";
            case MonomachineNovaAudioProcessor::fxG_VOLPAN_SRR:return "VOL/PAN · SRR";
            case MonomachineNovaAudioProcessor::fxG_P1_SRR_DELAY:return "SRR · DELAY";
            case MonomachineNovaAudioProcessor::fxG_P1post:return "SPLIT IN";
            case MonomachineNovaAudioProcessor::fxG_P2pre:return "P2 IN / DRY TAP";
            case MonomachineNovaAudioProcessor::fxG_P2MACH_CHAIN:return "P2 MACH · WET";
            case MonomachineNovaAudioProcessor::fxG_P2_EQ_FILT:return "P2 EQ · FILT";
            case MonomachineNovaAudioProcessor::fxG_P2_FILT_DIST:return "P2 FILT · DIST";
            case MonomachineNovaAudioProcessor::fxG_P2_DIST_ENV:return "P2 DIST · ENV";
            case MonomachineNovaAudioProcessor::fxG_P2_ENV_VOLPAN:return "P2 ENV · VOL/PAN";
            case MonomachineNovaAudioProcessor::fxG_P2_VOLPAN_SRR:return "P2 VOL/PAN · SRR";
            case MonomachineNovaAudioProcessor::fxG_P2_SRR_DELAY:return "P2 SRR · DELAY";
            case MonomachineNovaAudioProcessor::fxG_P2post:return "P2 MIX OUT";
            default:return "UNKNOWN";
        }
    }
    void showZoneMenu(bool send){
        juce::PopupMenu menu;menu.setLookAndFeel(&smallMenuLaf());const int selected=send?current.sendZone:current.returnZone;
        for(int visual=0;visual<MonomachineNovaAudioProcessor::fxZonesCount;++visual){const int zone=MonomachineNovaAudioProcessor::fxVisualZoneAt(visual);menu.addItem(visual+1,"g"+juce::String(zone)+" · "+zoneCaption(zone),true,zone==selected);}
        const juce::Component::SafePointer<FxFeedbackCard> safe(this);
        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(send?static_cast<juce::Component*>(&sendButton):static_cast<juce::Component*>(&returnButton)),[safe,send](int result){if(!safe||result<=0)return;const int zone=MonomachineNovaAudioProcessor::fxVisualZoneAt(result-1);if(send)safe->processor.feedbackSetSendZone(zone);else safe->processor.feedbackSetReturnZone(zone);});
    }
    MonomachineNovaAudioProcessor& processor;MonomachineNovaAudioProcessor::FeedbackState current{};bool syncing=false;
    juce::ToggleButton onButton,clipButton;PixelButton sendButton,returnButton;std::array<CurveSlider,4> knobs{};std::array<juce::String,4> labels{};
};

class ClassicStageCard final : public juce::Component {
public:
    static constexpr int kWidth=760,kHeight=42;
    ClassicStageCard(int chainIn,int stageIn):chain(chainIn),stage(stageIn),onButton("ON"),modeButton("MODE"){
        addAndMakeVisible(handle);addAndMakeVisible(onButton);addAndMakeVisible(modeButton);onButton.textHeight=11;modeButton.textHeight=11;
        handle.setTooltip("DRAG: move this classic block so its output lands at another physical g point. FX-slots stay at their physical points.");
        onButton.setTooltip("ON/OFF bypass for this one classic block. Its fixed FX-slot points remain in place.");
        modeButton.setTooltip("MODE of this exact classic block. Existing algorithms stay selectable; future ones are added, never substituted.");
        handle.began=[this](juce::Point<int> q){if(dragBegin)dragBegin(chain,stage,q);};
        handle.moved=[this](juce::Point<int> q){if(dragMove)dragMove(chain,stage,q);};
        handle.dropped=[this](juce::Point<int> q){if(dragDrop)dragDrop(chain,stage,q);};
        onButton.onClick=[this]{if(enabledRequested)enabledRequested(chain,stage,!enabled);};
        modeButton.onClick=[this]{if(modeRequested)modeRequested(chain,stage,modeButton);};
    }
    std::function<void(int,int,juce::Point<int>)> dragBegin,dragMove,dragDrop;
    std::function<void(int,int,juce::Component&)> modeRequested;
    std::function<void(int,int,bool)> enabledRequested;
    void sync(const juce::String& text,const juce::String& mode,const juce::String& tip,bool isEnabled){label=text;modeText=mode;enabled=isEnabled;modeButton.setButtonText("MODE "+mode);onButton.setButtonText(enabled?"ON":"OFF");handle.setTooltip(tip);repaint();}
    void resized() override {handle.setBounds(7,7,30,28);onButton.setBounds(getWidth()-222,7,58,28);modeButton.setBounds(getWidth()-158,7,144,28);}
    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::black);g.setColour(enabled?ink:ink.withAlpha(0.42f));g.drawRect(getLocalBounds(),1);
        pixel::text(g,label+(enabled?juce::String():"  [BYPASS]"),{48,11,getWidth()-232,19},15,false);
        g.setColour((enabled?ink:ink.withAlpha(0.35f)).withAlpha(0.55f));pixel::text(g,"OUTPUT → DRAG TO g POINT",{350,12,228,16},10,true);
    }
private:
    int chain=0,stage=0;bool enabled=true;juce::String label,modeText;FxSlotDragHandle handle;PixelButton onButton,modeButton;
};

class FxSlotsPage final : public juce::Component, private juce::Timer {
public:
    explicit FxSlotsPage(MonomachineNovaAudioProcessor& p):processor(p){
        feedbackCard=std::make_unique<FxFeedbackCard>(processor);addAndMakeVisible(*feedbackCard);
        addAndMakeVisible(resetP1);addAndMakeVisible(resetP2);resetP1.textHeight=11;resetP2.textHeight=11;
        resetP1.setTooltip("Restore only P1 to documented Monomachine order: EQ → FILT → DIST → ENV → VOL/PAN → SRR → DELAY, and turn all its blocks ON. Modes stay unchanged.");
        resetP2.setTooltip("Restore only P2 to documented Monomachine order: EQ → FILT → DIST → ENV → VOL/PAN → SRR → DELAY, and turn all its blocks ON. Modes stay unchanged.");
        resetP1.onClick=[this]{processor.classicRouteReset(0);refresh(true);};resetP2.onClick=[this]{processor.classicRouteReset(1);refresh(true);};
        addAndMakeVisible(p2MixKnob);p2MixKnob.setComponentID("p2mix");p2MixKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);p2MixKnob.setTextBoxStyle(juce::Slider::NoTextBox,false,0,0);p2MixKnob.setRange(0,127,1);p2MixKnob.setScrollWheelEnabled(true);
        p2MixKnob.setTooltip("P2 MIX: 0 = dry copy taken after g7 FX-slots; 127 = the full P2 wet group. g7 is shared, g8/g10..g14 are wet-only, and g9 processes the resulting mix.");
        p2MixLink=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.parameters,"p2_mix",p2MixKnob);
        for(int chain=0;chain<2;++chain)for(int stage=0;stage<MonomachineNovaAudioProcessor::classicStagesCount;++stage){
            auto card=std::make_unique<ClassicStageCard>(chain,stage);wireClassicCard(*card);addAndMakeVisible(*card);classicCards[static_cast<size_t>(chain)][static_cast<size_t>(stage)]=std::move(card);
        }
        for(int visual=0;visual<MonomachineNovaAudioProcessor::fxZonesCount;++visual){
            const int zone=MonomachineNovaAudioProcessor::fxVisualZoneAt(visual);
            auto button=std::make_unique<PixelButton>("+ ADD SLOT");button->textHeight=12;button->setTooltip("ADD SLOT at this fixed physical chain point");
            button->onClick=[this,zone]{if(processor.fxSlotAdd(zone)<0)juce::AlertWindow::showMessageBoxAsync(juce::MessageBoxIconType::WarningIcon,"FX SLOTS","32 slots are already in use.");refresh(true);};
            addAndMakeVisible(*button);addButtons[static_cast<size_t>(zone)]=std::move(button);
        }
        startTimerHz(20);refresh(true);
    }
    ~FxSlotsPage() override{stopTimer();}
    void paint(juce::Graphics& g) override {
        g.fillAll(juce::Colours::black);g.setColour(ink);
        pixel::text(g,"FX SLOTS",{18,8,200,28},22,false);
        pixel::text(g,"DRAG CLASSIC BLOCKS OR FX SLOTS · PHYSICAL g POINTS STAY FIXED",{230,13,760,16},12,false);
        g.setColour(ink.withAlpha(0.6f));g.drawHorizontalLine(39,18.0f,float(getWidth()-18));
        if(classicBottom>classicTop){g.setColour(ink.withAlpha(0.35f));g.drawRect(14,classicTop-5,90,classicBottom-classicTop+10,1);pixel::text(g,"ORDER",{21,classicTop+7,76,14},11,true);pixel::text(g,"P1",{18,classicTop+23,82,14},10,true);}
        // P2 is one top-to-bottom page visually, but g7 takes an explicit dry
        // copy before the P2 machine. The rail makes that group-level P2 MIX
        // boundary visible without pretending that the individual blocks grew
        // their own new dry/wet controls.
        if(p2MixPointY>p2DryTapY){
            constexpr int dryX=15,wetX=105;
            g.setColour(ink.withAlpha(0.45f));g.drawLine(static_cast<float>(dryX),static_cast<float>(p2DryTapY),static_cast<float>(wetX),static_cast<float>(p2DryTapY));
            for(int yy=p2DryTapY;yy<p2MixPointY;yy+=8)g.drawLine(static_cast<float>(dryX),static_cast<float>(yy),static_cast<float>(dryX),static_cast<float>(juce::jmin(yy+4,p2MixPointY)));
            g.drawLine(static_cast<float>(dryX),static_cast<float>(p2MixPointY),112.0f,static_cast<float>(p2MixPointY));
            g.setColour(ink.withAlpha(0.32f));g.drawLine(static_cast<float>(wetX),static_cast<float>(p2WetStartY),static_cast<float>(wetX),static_cast<float>(p2MixPointY));
            g.setColour(ink);pixel::text(g,"DRY",{19,p2DryTapY+5,76,13},10,true);pixel::text(g,"WET",{19,p2WetStartY+5,76,13},10,true);
            pixel::text(g,"P2 MIX",{12,p2MixControlY-13,92,13},10,true);pixel::text(g,juce::String(juce::roundToInt(p2MixKnob.getValue())),{12,p2MixControlY+71,92,13},10,true);
        }
        for(const auto& node:nodes){g.setColour(node.strong?ink:ink.withAlpha(0.72f));g.drawRect(120,node.y,760,30,1);pixel::text(g,node.text,{134,node.y+6,730,18},15,false);}
        for(int visual=0;visual<MonomachineNovaAudioProcessor::fxZonesCount;++visual){const auto& gap=gaps[static_cast<size_t>(visual)];
            g.setColour(ink.withAlpha(0.34f));g.drawHorizontalLine(gap.y+17,112.0f,904.0f);
            // Physical point labels deliberately live on the right: their g number
            // does not change while classic blocks move through the route.
            g.setColour(ink);pixel::text(g,"g"+juce::String(gap.zone)+"  ·  "+gapCaption(gap.zone),{910,gap.y+7,238,16},10,false);
        }
        if(dragSlot>=0||dragClassicChain>=0){const auto q=getLocalPoint(nullptr,dragScreen);g.setColour(ink.withAlpha(0.75f));g.drawHorizontalLine(q.y,112.0f,904.0f);}
    }
private:
    struct Node {juce::String text;int y=0;bool strong=false;};
    struct Gap {int zone=0,y=0,cardY=0,count=0,bottom=0;};
    static juce::String gapCaption(int zone){
        switch(zone){
            case MonomachineNovaAudioProcessor::fxG_P1pre:return "P1 INPUT";
            case MonomachineNovaAudioProcessor::fxG_P1_SRR_DELAY:return "P1 SRR → DELAY";
            case MonomachineNovaAudioProcessor::fxG_P1post:return "P1 OUT / SPLIT";
            case MonomachineNovaAudioProcessor::fxG_P2pre:return "P2 INPUT / DRY TAP";
            case MonomachineNovaAudioProcessor::fxG_P2MACH_CHAIN:return "P2 WET INPUT";
            case MonomachineNovaAudioProcessor::fxG_P2_SRR_DELAY:return "P2 SRR → DELAY";
            case MonomachineNovaAudioProcessor::fxG_P2post:return "P2 MIX OUT";
            default:return zone<=MonomachineNovaAudioProcessor::fxG_P1post?"P1 FX POINT":"P2 FX POINT";
        }
    }
    static juce::String stageName(int stage){
        switch(stage){
            case MonomachineNovaAudioProcessor::classicDist:return "DIST";
            case MonomachineNovaAudioProcessor::classicEq:return "EQ";
            case MonomachineNovaAudioProcessor::classicFilt:return "FILT";
            case MonomachineNovaAudioProcessor::classicEnv:return "ENV";
            case MonomachineNovaAudioProcessor::classicVolPan:return "VOL / PAN";
            case MonomachineNovaAudioProcessor::classicSrr:return "SRR";
            case MonomachineNovaAudioProcessor::classicDelay:return "DELAY";
            default:return "UNKNOWN";
        }
    }
    juce::String modeParamId(int chain,int stage) const {
        if(stage==MonomachineNovaAudioProcessor::classicFilt)return chain==0?"mode_filt":"p2_mode_filt";
        if(stage==MonomachineNovaAudioProcessor::classicDelay)return chain==0?"mode_dly":"p2_mode_dly";
        return {};
    }
    juce::String saturationParamId(int chain) const {return chain==0?"hybrid_p1_mode_s":"hybrid_p2_mode_s";}
    juce::String stageMode(int chain,int stage) const {
        if(stage==MonomachineNovaAudioProcessor::classicDist){
            static constexpr const char* names[]{"MNM","OLD","MNM FIX","FOLD","ZERO","CLAMP"};
            const auto* raw=processor.parameters.getRawParameterValue(saturationParamId(chain));
            return names[juce::jlimit(0,5,juce::roundToInt(raw?raw->load():0.0f))];
        }
        const auto id=modeParamId(chain,stage);if(id.isEmpty())return "MNM";
        const auto* raw=processor.parameters.getRawParameterValue(id);return raw&&juce::roundToInt(raw->load())!=0?"OLD":"MNM";
    }
    juce::String stageTip(int chain,int stage) const {
        juce::String result=(chain==0?"P1 ":"P2 ")+stageName(stage)+": drag its output to another physical point. ";
        if(stage==MonomachineNovaAudioProcessor::classicDist)result+="MODE opens MODE S, the sole DIST selector: MNM, OLD, experimental MNM FIX, or an imported character. ON/OFF bypass leaves the surrounding FX-slot points fixed.";
        else if(modeParamId(chain,stage).isEmpty())result+="ON/OFF bypass leaves the surrounding FX-slot points fixed. MODE is reserved for a later alternative without replacing the native implementation.";
        else result+="Open MODE to choose the existing native or legacy implementation. ON/OFF bypass leaves the surrounding FX-slot points fixed.";
        return result;
    }
    juce::String p2MachineName() const {
        auto* raw=processor.parameters.getRawParameterValue("p2_machine");const auto& list=MonomachineNovaAudioProcessor::p2FxMachines();
        const int index=juce::jlimit(0,static_cast<int>(list.size())-1,juce::roundToInt(raw?raw->load():0.0f));return list[static_cast<size_t>(index)].name;
    }
    void addNode(const juce::String& text,bool strong,int& y){nodes.push_back({text,y,strong});y+=40;}
    void addClassicStage(int chain,int stage,int& y){
        auto* card=classicCards[static_cast<size_t>(chain)][static_cast<size_t>(stage)].get();if(card){card->setBounds(120,y,ClassicStageCard::kWidth,ClassicStageCard::kHeight);card->setVisible(true);}y+=ClassicStageCard::kHeight+8;
    }
    void addGap(int zone,int& y,const std::array<std::array<int,MonomachineNovaAudioProcessor::fxSlotsMax>,MonomachineNovaAudioProcessor::fxZonesCount>& order,const std::array<int,MonomachineNovaAudioProcessor::fxZonesCount>& counts){
        const int visual=MonomachineNovaAudioProcessor::fxVisualIndexOfZone(zone);auto& gap=gaps[static_cast<size_t>(visual)];
        gap={zone,y,y+29,counts[static_cast<size_t>(zone)],y};
        int cardY=gap.cardY;
        for(int i=0;i<gap.count;++i){const int slot=order[static_cast<size_t>(zone)][static_cast<size_t>(i)];auto& card=cards[static_cast<size_t>(slot)];if(card)card->setBounds(120,cardY,FxSlotCard::kWidth,FxSlotCard::kHeight);cardY+=FxSlotCard::kHeight+8;}
        addButtons[static_cast<size_t>(zone)]->setBounds(120,cardY,116,24);addButtons[static_cast<size_t>(zone)]->setVisible(true);
        gap.bottom=cardY+28;y=gap.bottom+12;
    }
    void refresh(bool force){
        if(feedbackCard)feedbackCard->sync(processor.feedbackState());
        const auto classic=processor.classicRouteState();
        for(int chain=0;chain<2;++chain)for(int stage=0;stage<MonomachineNovaAudioProcessor::classicStagesCount;++stage)if(auto* card=classicCards[static_cast<size_t>(chain)][static_cast<size_t>(stage)].get())card->sync((chain==0?"P1 · ":"P2 · ")+stageName(stage),stageMode(chain,stage),stageTip(chain,stage),chain==0?classic.p1Enabled[static_cast<size_t>(stage)]:classic.p2Enabled[static_cast<size_t>(stage)]);
        std::array<MonomachineNovaAudioProcessor::FxSlotState,MonomachineNovaAudioProcessor::fxSlotsMax> state{};
        std::array<std::array<int,MonomachineNovaAudioProcessor::fxSlotsMax>,MonomachineNovaAudioProcessor::fxZonesCount> grouped{};
        std::array<int,MonomachineNovaAudioProcessor::fxZonesCount> counts{};
        uint64_t structure=1469598103934665603ull;
        if(auto* p2raw=processor.parameters.getRawParameterValue("p2_machine")){structure^=static_cast<uint64_t>(juce::roundToInt(p2raw->load()));structure*=1099511628211ull;}
        for(int i=0;i<MonomachineNovaAudioProcessor::classicStagesCount;++i){const auto at=static_cast<size_t>(i);structure^=static_cast<uint64_t>(classic.p1[at]+11*i);structure*=1099511628211ull;structure^=static_cast<uint64_t>(classic.p2[at]+31*i);structure*=1099511628211ull;structure^=static_cast<uint64_t>((classic.p1Enabled[at]?0x4000:0)|(classic.p2Enabled[at]?0x8000:0));structure*=1099511628211ull;}
        for(int slot=0;slot<MonomachineNovaAudioProcessor::fxSlotsMax;++slot){state[static_cast<size_t>(slot)]=processor.fxSlotState(slot);const auto& s=state[static_cast<size_t>(slot)];
            structure^=static_cast<uint64_t>((s.used?1:0)|(s.zone<<1)|(s.machineId<<6)|(s.on?0x8000:0)|(s.order<<16));structure*=1099511628211ull;
            if(!s.used)continue;const int z=juce::jlimit(0,MonomachineNovaAudioProcessor::fxZonesCount-1,s.zone);grouped[static_cast<size_t>(z)][static_cast<size_t>(counts[static_cast<size_t>(z)]++)]=slot;
            if(!cards[static_cast<size_t>(slot)]){auto card=std::make_unique<FxSlotCard>(processor,slot);wireCard(*card);addAndMakeVisible(*card);cards[static_cast<size_t>(slot)]=std::move(card);}cards[static_cast<size_t>(slot)]->sync(s);
        }
        for(int z=0;z<MonomachineNovaAudioProcessor::fxZonesCount;++z)for(int i=1;i<counts[static_cast<size_t>(z)];++i){int key=grouped[static_cast<size_t>(z)][static_cast<size_t>(i)],j=i;while(j>0&&state[static_cast<size_t>(grouped[static_cast<size_t>(z)][static_cast<size_t>(j-1)])].order>state[static_cast<size_t>(key)].order){grouped[static_cast<size_t>(z)][static_cast<size_t>(j)]=grouped[static_cast<size_t>(z)][static_cast<size_t>(j-1)];--j;}grouped[static_cast<size_t>(z)][static_cast<size_t>(j)]=key;}
        for(int slot=0;slot<MonomachineNovaAudioProcessor::fxSlotsMax;++slot)if(!state[static_cast<size_t>(slot)].used&&cards[static_cast<size_t>(slot)]){removeChildComponent(cards[static_cast<size_t>(slot)].get());cards[static_cast<size_t>(slot)].reset();}
        if(force||structure!=lastStructure){lastStructure=structure;nodes.clear();int y=55;
            if(feedbackCard){feedbackCard->setBounds(120,y,FxFeedbackCard::kWidth,FxFeedbackCard::kHeight);y+=FxFeedbackCard::kHeight+14;}
            resetP1.setBounds(890,y+2,246,26);addNode("P1 · ENGINE",true,y);classicTop=y-40;
            static constexpr int p1Gaps[MonomachineNovaAudioProcessor::classicStagesCount+1]={MonomachineNovaAudioProcessor::fxG_P1pre,MonomachineNovaAudioProcessor::fxG_EQ_FILT,MonomachineNovaAudioProcessor::fxG_FILT_DIST,MonomachineNovaAudioProcessor::fxG_DIST_ENV,MonomachineNovaAudioProcessor::fxG_ENV_VOLPAN,MonomachineNovaAudioProcessor::fxG_VOLPAN_SRR,MonomachineNovaAudioProcessor::fxG_P1_SRR_DELAY,MonomachineNovaAudioProcessor::fxG_P1post};
            for(int position=0;position<MonomachineNovaAudioProcessor::classicStagesCount;++position){addGap(p1Gaps[position],y,grouped,counts);addClassicStage(0,classic.p1[static_cast<size_t>(position)],y);}addGap(p1Gaps[MonomachineNovaAudioProcessor::classicStagesCount],y,grouped,counts);classicBottom=y-12;
            addNode("--- SPLIT ---",true,y);addGap(MonomachineNovaAudioProcessor::fxG_P2pre,y,grouped,counts);
            p2DryTapY=gaps[static_cast<size_t>(MonomachineNovaAudioProcessor::fxVisualIndexOfZone(MonomachineNovaAudioProcessor::fxG_P2pre))].bottom;
            resetP2.setBounds(890,y+2,246,26);addNode("P2 · "+p2MachineName(),true,y);
            static constexpr int p2Gaps[MonomachineNovaAudioProcessor::classicStagesCount+1]={MonomachineNovaAudioProcessor::fxG_P2MACH_CHAIN,MonomachineNovaAudioProcessor::fxG_P2_EQ_FILT,MonomachineNovaAudioProcessor::fxG_P2_FILT_DIST,MonomachineNovaAudioProcessor::fxG_P2_DIST_ENV,MonomachineNovaAudioProcessor::fxG_P2_ENV_VOLPAN,MonomachineNovaAudioProcessor::fxG_P2_VOLPAN_SRR,MonomachineNovaAudioProcessor::fxG_P2_SRR_DELAY,MonomachineNovaAudioProcessor::fxG_P2post};
            for(int position=0;position<MonomachineNovaAudioProcessor::classicStagesCount;++position){addGap(p2Gaps[position],y,grouped,counts);if(position==0)p2WetStartY=gaps[static_cast<size_t>(MonomachineNovaAudioProcessor::fxVisualIndexOfZone(MonomachineNovaAudioProcessor::fxG_P2MACH_CHAIN))].y+17;addClassicStage(1,classic.p2[static_cast<size_t>(position)],y);}
            addGap(p2Gaps[MonomachineNovaAudioProcessor::classicStagesCount],y,grouped,counts);
            p2MixPointY=gaps[static_cast<size_t>(MonomachineNovaAudioProcessor::fxVisualIndexOfZone(MonomachineNovaAudioProcessor::fxG_P2post))].y+17;
            p2MixControlY=juce::jmax(p2WetStartY+20,p2MixPointY-88);p2MixKnob.setBounds(22,p2MixControlY,76,76);
            addNode("OUT",true,y);setSize(1160,juce::jmax(520,y+32));
        }
        repaint();
    }
    void wireCard(FxSlotCard& card){
        card.changed=[this]{refresh(true);};
        card.dragBegin=[this](int slot,juce::Point<int> q){dragSlot=slot;dragScreen=q;repaint();};
        card.dragMove=[this](int,juce::Point<int> q){dragScreen=q;repaint();};
        card.dragDrop=[this](int slot,juce::Point<int> q){dropSlot(slot,q);};
    }
    void wireClassicCard(ClassicStageCard& card){
        card.dragBegin=[this](int chain,int stage,juce::Point<int> q){dragClassicChain=chain;dragClassicStage=stage;dragScreen=q;repaint();};
        card.dragMove=[this](int,int,juce::Point<int> q){dragScreen=q;repaint();};
        card.dragDrop=[this](int chain,int stage,juce::Point<int> q){dropClassicStage(chain,stage,q);};
        card.modeRequested=[this](int chain,int stage,juce::Component& target){showClassicMode(chain,stage,target);};
        card.enabledRequested=[this](int chain,int stage,bool enabled){processor.classicRouteSetEnabled(chain,stage,enabled);refresh(false);};
    }
    void dropSlot(int slot,juce::Point<int> screen){
        const auto point=getLocalPoint(nullptr,screen);int best=0,bestDistance=std::numeric_limits<int>::max();
        for(int visual=0;visual<MonomachineNovaAudioProcessor::fxZonesCount;++visual){const auto& gap=gaps[static_cast<size_t>(visual)];const int distance=point.y<gap.y?gap.y-point.y:(point.y>gap.bottom?point.y-gap.bottom:0);if(distance<bestDistance){bestDistance=distance;best=visual;}}
        const auto& gap=gaps[static_cast<size_t>(best)];int insertion=0;
        for(int i=0;i<gap.count;++i){const int other=cardsInGap(gap.zone,i);if(other<0||other==slot)continue;auto* card=cards[static_cast<size_t>(other)].get();if(card&&point.y>card->getBounds().getCentreY())++insertion;}
        processor.fxSlotMoveToGap(slot,gap.zone,insertion);dragSlot=-1;refresh(true);
    }
    void dropClassicStage(int chain,int stage,juce::Point<int> screen){
        static constexpr int p1Outputs[MonomachineNovaAudioProcessor::classicStagesCount]={MonomachineNovaAudioProcessor::fxG_EQ_FILT,MonomachineNovaAudioProcessor::fxG_FILT_DIST,MonomachineNovaAudioProcessor::fxG_DIST_ENV,MonomachineNovaAudioProcessor::fxG_ENV_VOLPAN,MonomachineNovaAudioProcessor::fxG_VOLPAN_SRR,MonomachineNovaAudioProcessor::fxG_P1_SRR_DELAY,MonomachineNovaAudioProcessor::fxG_P1post};
        static constexpr int p2Outputs[MonomachineNovaAudioProcessor::classicStagesCount]={MonomachineNovaAudioProcessor::fxG_P2_EQ_FILT,MonomachineNovaAudioProcessor::fxG_P2_FILT_DIST,MonomachineNovaAudioProcessor::fxG_P2_DIST_ENV,MonomachineNovaAudioProcessor::fxG_P2_ENV_VOLPAN,MonomachineNovaAudioProcessor::fxG_P2_VOLPAN_SRR,MonomachineNovaAudioProcessor::fxG_P2_SRR_DELAY,MonomachineNovaAudioProcessor::fxG_P2post};
        const auto point=getLocalPoint(nullptr,screen);const auto& outputs=chain==0?p1Outputs:p2Outputs;int best=0,bestDistance=std::numeric_limits<int>::max();
        for(int position=0;position<MonomachineNovaAudioProcessor::classicStagesCount;++position){const int visual=MonomachineNovaAudioProcessor::fxVisualIndexOfZone(outputs[position]);const auto& gap=gaps[static_cast<size_t>(visual)];const int distance=point.y<gap.y?gap.y-point.y:(point.y>gap.bottom?point.y-gap.bottom:0);if(distance<bestDistance){bestDistance=distance;best=position;}}
        processor.classicRouteMove(chain,stage,best);dragClassicChain=-1;dragClassicStage=-1;refresh(true);
    }
    void showClassicMode(int chain,int stage,juce::Component& target){
        const bool isDist=stage==MonomachineNovaAudioProcessor::classicDist;
        const auto id=isDist?saturationParamId(chain):modeParamId(chain,stage);juce::PopupMenu menu;menu.setLookAndFeel(&smallMenuLaf());
        if(isDist){
            static constexpr const char* names[]{"MNM — retained current DIST","OLD — untouched reference","MNM FIX — experimental +3 dB near 3.5 kHz","FOLD — MNM-controlled colour","ZERO — MNM-controlled colour","CLAMP — MNM-controlled colour"};
            auto* raw=processor.parameters.getRawParameterValue(id);const int current=raw?juce::jlimit(0,5,juce::roundToInt(raw->load())):0;
            for(int mode=0;mode<6;++mode)menu.addItem(mode+1,names[mode],true,current==mode);
        } else if(id.isEmpty()){menu.addItem(1,"MNM — only implementation currently available",false,true);}
        else {auto* raw=processor.parameters.getRawParameterValue(id);const int current=raw?juce::jlimit(0,1,juce::roundToInt(raw->load())):0;menu.addItem(1,"mnm",true,current==0);menu.addItem(2,"old",true,current==1);}
        const juce::Component::SafePointer<FxSlotsPage> safe(this);
        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(&target),[safe,id](int result){if(!safe||result<=0||id.isEmpty())return;auto* parameter=safe->processor.parameters.getParameter(id);if(!parameter)return;parameter->beginChangeGesture();parameter->setValueNotifyingHost(parameter->convertTo0to1(static_cast<float>(result-1)));parameter->endChangeGesture();safe->refresh(false);});
    }
    int cardsInGap(int zone,int ordinal) const {
        std::array<std::pair<int,int>,MonomachineNovaAudioProcessor::fxSlotsMax> found{};int count=0;
        for(int slot=0;slot<MonomachineNovaAudioProcessor::fxSlotsMax;++slot){const auto s=processor.fxSlotState(slot);if(s.used&&s.zone==zone)found[static_cast<size_t>(count++)]=std::make_pair(s.order,slot);}
        for(int i=1;i<count;++i){auto key=found[static_cast<size_t>(i)];int j=i;while(j>0&&found[static_cast<size_t>(j-1)].first>key.first){found[static_cast<size_t>(j)]=found[static_cast<size_t>(j-1)];--j;}found[static_cast<size_t>(j)]=key;}
        return ordinal>=0&&ordinal<count?found[static_cast<size_t>(ordinal)].second:-1;
    }
    void timerCallback() override{refresh(false);}
    MonomachineNovaAudioProcessor& processor;
    std::unique_ptr<FxFeedbackCard> feedbackCard;PixelButton resetP1{"RESET P1 MNM ORDER"},resetP2{"RESET P2 MNM ORDER"};
    CurveSlider p2MixKnob;std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> p2MixLink;
    std::array<std::array<std::unique_ptr<ClassicStageCard>,MonomachineNovaAudioProcessor::classicStagesCount>,2> classicCards{};
    std::array<std::unique_ptr<FxSlotCard>,MonomachineNovaAudioProcessor::fxSlotsMax> cards{};
    std::array<std::unique_ptr<PixelButton>,MonomachineNovaAudioProcessor::fxZonesCount> addButtons{};
    std::array<Gap,MonomachineNovaAudioProcessor::fxZonesCount> gaps{};std::vector<Node> nodes;
    uint64_t lastStructure=0;int classicTop=0,classicBottom=0,p2DryTapY=0,p2WetStartY=0,p2MixPointY=0,p2MixControlY=0,dragSlot=-1,dragClassicChain=-1,dragClassicStage=-1;juce::Point<int> dragScreen;
};

}

// 1.6.30: док для списков -- появляется НИЖЕ своего хитбокса, без стрелки
// каллаута и без двойной обводки; клик мимо -- закрыть.
class PopupDock final : public juce::Component {
public:
    PopupDock(){setInterceptsMouseClicks(true,false);}
    void paint(juce::Graphics&)override{}
    void mouseDown(const juce::MouseEvent&)override{if(onDismiss)onDismiss();}
    std::function<void()> onDismiss;
};
struct MonomachineNovaAudioProcessorEditor::Surface final : juce::Component, public juce::DragAndDropContainer, public juce::Slider::Listener, private juce::Timer {
    void sliderDragStarted(juce::Slider*) override { pushUndoPoint(); } // 1.6.32: кручение ручки/фейдера = точка отката
    void sliderValueChanged(juce::Slider*) override {}
    void pushUndoPoint() { auto st=processor.parameters.copyState(); st.addChild(processor.plockToTree(),-1,nullptr); undoStack.push_back(st); if (undoStack.size() > 60) undoStack.erase(undoStack.begin()); redoStack.clear(); } // 1.7.3: снимок включает P-LOCK
    void applyTree(juce::ValueTree vt) { processor.parameters.replaceState(vt); if (auto pl=vt.getChildWithName("PLOCK"); pl.isValid()) processor.plockFromTree(pl); bind(); repaint(); } // 1.7.3: undo/redo откатывает и P-LOCK
    void doUndo() { if (undoStack.empty()) return; redoStack.push_back(processor.parameters.copyState()); applyTree(undoStack.back()); undoStack.pop_back(); }
    void doRedo() { if (redoStack.empty()) return; undoStack.push_back(processor.parameters.copyState()); applyTree(redoStack.back()); redoStack.pop_back(); }
    std::vector<juce::ValueTree> undoStack, redoStack; // 1.6.32: полный снимок состояния (ARP-страницы и MSEG тоже)
    explicit Surface(MonomachineNovaAudioProcessor& p):processor(p),machineButton(""),menuButton("MENU"),tempoButton(""),arpButton("ARP"),gateOuter(p,"arp_length",1,127,64),matrixButton("MATRIX"),lfo1Button("1"),lfo2Button("2"),lfo3Button("3"),closeButton("BACK"),gateButton(""){
        setLookAndFeel(&theme);
        dspMenuLf.setColour(juce::PopupMenu::backgroundColourId,juce::Colours::black);dspMenuLf.setColour(juce::PopupMenu::textColourId,juce::Colours::white);
        dspMenuLf.setColour(juce::PopupMenu::highlightedBackgroundColourId,juce::Colours::white);dspMenuLf.setColour(juce::PopupMenu::highlightedTextColourId,juce::Colours::black);
        dspMenuLf.setColour(juce::PopupMenu::headerTextColourId,juce::Colours::white); // 1.6.21
        int layP=0;for(auto& panel:cells){int layI=0;for(auto& cell:panel){cell=std::make_unique<Cell>(p);cell->openSamples=[this]{showSamples();};cell->openEnvelope=[this](int knob){showEnvelope(knob);};cell->openModes=[this](int section,juce::Point<int> at){showDspModes(section,at);};cell->openRepitch=[this](juce::Point<int> at){showRepitchMenu(at);};cell->openLocks=[this](int lfo,bool dest,juce::Point<int> at){showLfoLocks(lfo,dest,at);};cell->addMsegModulation=[this](uint8_t target){processor.addMsegRoute(0,target);};cell->pickTarget=[this](uint8_t target){completeTargetPick(target);};cell->pickHover=[this](uint8_t target){pickHoverTarget=target;};cell->lfoDestinationChanged=[this](uint8_t target){outlineLfoDestination(target);};cell->openDsnd=[this](juce::Point<int> at){showDsndMenu(at);};cell->openDelayFeedbackQ=[this](juce::Point<int> at){showDelayFeedbackQ(at);};cell->pushEdit=[this]{pushUndoPoint();};cell->attachUndo(this);cell->setComponentID("c"+juce::String(layP)+juce::String(layI));addAndMakeVisible(*cell);++layI;}++layP;}
        for(auto* button:{&machineButton,&menuButton,&tempoButton,&arpButton,&matrixButton,&fxButton,&p2Button})addAndMakeVisible(*button); // 1.8.4: отдельная FX-страница, P1|P2 не двигаем
        addAndMakeVisible(gateOuter);gateOuter.setTooltip("GATE: arp note length 1..127 -- how much of each step the note holds. Drag or scroll. RMB = ARP step editor."); // 1.6.21
        // 1.6.21: LFO 1/2/3 и MSEG 1/2/3 -- кнопки-гнёзда: клик по цифре = страница,
        // провод из гнезда = прицел на цель модуляции.
        lfoJack.jackOnly=true; // 1.8.0b: ЕДИННЫЙ патч-корд -- источник = выбранная страница LFO
        lfoJack.beginDrag=[this](bool matrix){beginModDrag(p2View?(26+lfoPageSel):(lfoPageSel<3?4+lfoPageSel:20+lfoPageSel),matrix);cableLayer.begin(getLocalPoint(nullptr,lfoJack.jackScreenCentre()));}; // 1.8.0e: P1 = 4..6 / 23..25, P2 = 26..31
        lfoJack.dragMove=[this](juce::Point<int> s){cableLayer.drag(getLocalPoint(nullptr,s));modDragMove(s);};
        lfoJack.endDrag=[this](juce::Point<int> s,bool dragged,bool matrix){cableLayer.hide();endModDrag(s,dragged,lfoPageSel,matrix);}; // источник считывается ЖИВЫМ (переключение на лету)
        for(auto* lfo:{&lfo1Button,&lfo2Button,&lfo3Button,&lfo4Button,&lfo5Button,&lfo6Button,&lfoJack})addAndMakeVisible(*lfo); // 1.8.0e
        addAndMakeVisible(cableLayer);
        gateOuter.dragGain=6.0;gateOuter.rightClick=[this]{if(settings)closeOverlay();else showSettings(false);}; // 1.6.22: ПКМ по GATE открывает И закрывает ARP; драг в 6 раз короче
        // 1.6.5: три одинаковые кнопки LFO1/2/3. Клик -- показать LFO в правой
        // нижней панели; пресс-и-тащи с отпусканием на ручке -- назначить
        // маршрут модуляции без второго клика.
        // 1.6.8: три ОДИНАКОВЫЕ кнопки LFO1/2/3 (один размер, один стиль; выбранная
        // -- залита, см. setToggleState в bind()). ЛКМ-тащить = прямой PAGE/DEST
        // этого LFO на наведённую ручку, ПКМ-тащить = маршрут в матрицу.
        auto wireLfo=[this](ModSourceButton& b,int i){
            b.label=juce::String(i+1);
            b.jackEnabled=false; // 1.8.0b: гнездо ОДНО рядом (lfoJack) -- источник = выбранная страница LFO
            b.numberClick=[this,i]{lfoPageSel=juce::jlimit(0,5,i);bind();}; // 1.8.0e: 6 страниц
            b.numberDrag=[this](juce::Point<int> s){const int over=overLfoButton(s);if(over>=0&&over!=lfoPageSel){lfoPageSel=over;bind();}}; // 1.8.0b: драг-выбор по ряду
            b.setTooltip("LFO"+juce::String(i+1)+": click the number = show this LFO page; pull the CORD from the jack and release over a knob: LEFT = this LFO modulates it directly (its own PAGE/DEST), RIGHT = a matrix route. While aiming, dwell briefly inside an LFO number to show its page; a fast pass does not switch it."); // cord-navigation intent guard
        };
        wireLfo(lfo1Button,0);wireLfo(lfo2Button,1);wireLfo(lfo3Button,2);wireLfo(lfo4Button,3);wireLfo(lfo5Button,4);wireLfo(lfo6Button,5); // 1.8.0e: ряд 1..6
                // 1.8.0b: кнопки-гнёзда MSEG убраны (по просьбе юзера); MSEG-кривые -- SOURCE в матрице и в редакторе MSEG
        addAndMakeVisible(dspModeButton);dspModeButton.onClick=[this]{showDspModes(-1);};updateDspModeLabel();
        addAndMakeVisible(undoBtn);addAndMakeVisible(redoBtn);undoBtn.action=[this]{doUndo();};redoBtn.action=[this]{doRedo();}; // 1.6.32: UNDO/REDO между BPM и MENU
        level.addListener(this); // 1.6.32: фейдер LEV тоже пишет точку отката
        addMouseListener(&rmbWatcher,true); // 1.6.42: слежу за всеми кликами -- ПКМ гасит доп-окно
        rmbWatcher.onDown=[this](const juce::MouseEvent& e){ if(extraPanel==nullptr)return; auto* src=e.eventComponent; // 1.6.44: закрывается ЛЮБЫМ кликом мимо (как раньше); клик-открыватель панель не гасит
 if(src==nullptr||src==extraPanel.get()||extraPanel->isParentOf(src))return;
 if(src==&filtCatButton&&extraKind=="filt-extra")return; // the RMB that opened this panel must not immediately dismiss it
 for(auto* pc=src;pc!=nullptr;pc=pc->getParentComponent()){ if(auto* cell=dynamic_cast<Cell*>(pc)){ if(cell->consumedPopup())return; } if(auto* cs=dynamic_cast<CurveSlider*>(pc)){ if(cs->popup)return; } } // 1.6.43/44: mouseDown ячейки/крутилки срабатывает раньше listener'а -- этот же клик только что открыл/переключил панель (ПКМ по самой крутилке = DSND/REPITCH)
 if(auto* fp=dynamic_cast<FloatingPanel*>(extraPanel.get()); fp!=nullptr&&fp->pinned)return; // 1.6.44: закреплено булавкой
 closeExtraPanel(); };
        viewport.setComponentID("viewport"); // 1.6.42: окно страниц ARP/MSEG/MATRIX двигается и ресайзится в LAYOUT EDIT
        tagTitle=std::make_unique<TextTag>(*this,"title",juce::Rectangle<int>(72,4,399,36));tagTitle->factoryText=processor.isSynthVersion?"MONOMACHINE NOVA SYNTH":"MONOMACHINE NOVA FX"; // 1.6.42 // 1.6.43: surf.title из JSON пользователя
        tagLogo=std::make_unique<TextTag>(*this,"logo",juce::Rectangle<int>(26,16,18,29)); // 1.6.43: surf.logo // 1.6.42: значок -- двигается
        tagLev=std::make_unique<TextTag>(*this,"lev",juce::Rectangle<int>(7,58,58,30));tagLev->factoryText="LEV";
        tagBpm=std::make_unique<TextTag>(*this,"bpm",juce::Rectangle<int>(1006,2,40,14));tagBpm->factoryText="BPM";
        const std::pair<juce::Component*,const char*> layIds[]{{&machineButton,"machine"},{&fxButton,"fx"},{&p2Button,"p2"},{&menuButton,"menu"},{&tempoSync,"sync"},{&tempoButton,"bpm"},{&arpEnabled,"arpon"},{&arpButton,"arp"},{&gateOuter,"gate"},{&trigToggle,"trig"},{&matrixButton,"matrix"},{&dspModeButton,"dspmode"},{&undoBtn,"undo"},{&redoBtn,"redo"},{&syntCatButton,"catsynt"},{&lfoCatButton,"catlfo"},{&filtCatButton,"catfilt"},{&effxCatButton,"cateffx"},{&msegCatButton,"catmseg"},{&syntModeCombo,"modesynt"},{&filtModeCombo,"modefilt"},{&hybridModeLCombo,"modehybridl"},{&hybridModeHCombo,"modehybridh"},{&hybridModeSCombo,"modehybrids"},{&effxDistCombo,"modedist"},{&effxDlyCombo,"modedly"},{&ampModeCombo,"modeamp"},{&level,"lev"},{&closeButton,"back"},{&lfo1Button,"lfo1"},{&lfo2Button,"lfo2"},{&lfo3Button,"lfo3"},{&lfo4Button,"lfo4"},{&lfo5Button,"lfo5"},{&lfo6Button,"lfo6"},{&lfoJack,"lfojack"},{&ampButton,"ampcat"}}; // 1.6.49: кнопка AMP -- тоже двигается в LAYOUT EDIT
        for(auto& pid:layIds)pid.first->setComponentID(pid.second); // 1.6.38: LAYOUT EDIT двигает именованное
        auto wireCat=[this](PixelButton& b,const char* tip){b.textHeight=20;addAndMakeVisible(b);b.setTooltip(tip);}; // 1.6.28: кликабельные заголовки категорий
        wireCat(syntCatButton,"SYNT: reserved for future functionality");
        wireCat(lfoCatButton,"LFO: reserved for future functionality");
        wireCat(filtCatButton,"FILT: right-click opens opt-in VEL / KEY TRACK / SAT extras for the current P1 or P2 page.");
        filtCatButton.rightClick=[this]{showFilterExtras(filtCatButton.getScreenPosition());};
        wireCat(effxCatButton,"EFFX: reserved for future functionality");
        wireCat(msegCatButton,"MSEG: open the curve editor");msegCatButton.onClick=[this]{showMseg(0);};
        addAndMakeVisible(ampButton);ampButton.setButtonText("AMP");ampButton.textHeight=20;ampButton.rightClick=[this]{showEnvelope(-1);};ampButton.onClick=[this]{toggleEnvMini();}; // 1.6.29: шрифт категории как у остальных
        // 1.6.14: выбор "amp old" удалён как бесполезный -- огибающая всегда mnm.
        syntModeCombo.speedKey="MODE SYNT";filtModeCombo.speedKey="MODE FILT";hybridModeLCombo.speedKey="MODE L";hybridModeHCombo.speedKey="MODE H";hybridModeSCombo.speedKey="MODE S";effxDlyCombo.speedKey="MODE DLY";ampModeCombo.speedKey="MODE AMP"; // side selectors + the sole DIST selector
        for(auto* mc:{&syntModeCombo,&filtModeCombo,&hybridModeLCombo,&hybridModeHCombo,&hybridModeSCombo,&effxDistCombo,&effxDlyCombo,&ampModeCombo})mc->smallPopup=true;
        for(auto* c:{&syntModeCombo,&filtModeCombo,&hybridModeLCombo,&hybridModeHCombo,&hybridModeSCombo,&effxDistCombo,&effxDlyCombo,&ampModeCombo}){addAndMakeVisible(*c);c->setScrollWheelEnabled(true);}
        filtModeCombo.setVisible(false); // native/old stays accessible through DSP MODE, not a fourth FILT control
        effxDistCombo.setVisible(false); // MODE S is the only user-facing DIST authority
        level.setSliderStyle(juce::Slider::LinearVertical);level.setTextBoxStyle(juce::Slider::NoTextBox,false,0,0);
        level.setColour(juce::Slider::trackColourId,ink);level.setColour(juce::Slider::thumbColourId,ink);level.setColour(juce::Slider::backgroundColourId,juce::Colours::black);addAndMakeVisible(level);
levelLink=std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(p.parameters,"level",level);
        {juce::NormalisableRange<double> lev(0.0,127.0,[](double,double,double p){p=juce::jlimit(0.0,1.0,p);const double d=(1.0-p)*438.0;const double v=d<=100.0?127.0-d*(27.0/100.0):100.0-(d-100.0)*(100.0/338.0);return juce::jlimit(0.0,127.0,std::round(v));},[](double,double,double v){v=juce::jlimit(0.0,127.0,v);const double d=v>=100.0?(127.0-v)*(100.0/27.0):100.0+(100.0-v)*(338.0/100.0);return juce::jlimit(0.0,1.0,1.0-d/438.0);},[](double lo,double hi,double v){return juce::jlimit(lo,hi,std::round(v));});level.setNormalisableRange(lev);} // 1.6.36: convertFrom0To1/convertTo0To1 НЕ перепутаны (в 1.6.35 клик давал 0, 100 выглядела как 127) // 1.6.35: кусочная шкала ПОСЛЕ attachment (иначе он перезаписывает её линейной): 127=риска макс, 100=риска дефолта
        machineButton.textAlign=1;
        // 1.6.13: стрелки по бокам названия (влево/вправо, клик = один шаг пресета),
        // драг прокрутки ЗАМЕДЛЕН (18 px на шаг + множитель MACHINE из GUI DRAG SPEED),
        // хитбоксы прежние.
        machineButton.arrows=true;machineButton.verticalDrag=true;machineButton.arrowClick=[this](int d){cycleMachine(d);}; // 1.6.24: драг по вертикали
        machineButton.onClick=[this]{showMachineMenu();};
        machineButton.dragPixelsPerStep=18;
        machineButton.dragHandler=[this](int steps,bool){machineAccum+=static_cast<float>(steps)*static_cast<float>(uiSpeed("MACHINE",1.0)); // 1.6.24: колесо/драг -- шаг за шагом, без накопления половинок
            const int whole=static_cast<int>(machineAccum>=0?std::floor(machineAccum):std::ceil(machineAccum));
            if(whole!=0){machineAccum-=static_cast<float>(whole);cycleMachine(whole);}};
        menuButton.onClick=[this]{showMenu();};
        // BPM drags to change tempo; it no longer hijacks a settings page.
        tempoButton.textHeight=16;tempoButton.outlined=false;tempoButton.textAlign=1;
        // 1.6.12: BPM целыми; Alt/Shift -- десятые. Буквы BPM мельче.
        tempoButton.dragHandler=[this](int steps,bool fine){nudgeBpm(static_cast<float>(steps)*(fine?0.1f:1.0f)*static_cast<float>(uiSpeed("BPM")));};
        addAndMakeVisible(tempoSync);tempoSync.setButtonText("SYNC"); // 1.6.14: без "BPM", ближе к значению
        // 1.6.20: SYNC на главной панели -- ОБЩИЙ темп-синк синта (host_sync): дает
        // всему синту темп из хоста. Режим RATE арпа (arp_sync) -- отдельная галочка
        // HOST TEMPO на странице ARP; они больше никак не связаны.
        tempoSyncLink=std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(p.parameters,"host_sync",tempoSync);
        tempoSync.setTooltip("SYNC: the whole synth follows the host BPM (tempo source for ARP/LFO/delay). The ARP rate mode switch is HOST TEMPO on the ARP page -- the two are independent.");
        addChildComponent(tempoEntry);tempoEntry.setName("BPM entry");tempoEntry.setInputRestrictions(7,"0123456789.");tempoEntry.onEscapeKey=[this]{tempoEntry.setVisible(false);};tempoEntry.onFocusLost=[this]{tempoEntry.setVisible(false);};
        tempoEntry.onReturnKey=[this]{if(tempoEntry.getText().isNotEmpty()){auto* bpm=processor.parameters.getParameter("bpm");bpm->beginChangeGesture();bpm->setValueNotifyingHost(bpm->convertTo0to1(juce::jlimit(30.0f,300.0f,tempoEntry.getText().getFloatValue())));bpm->endChangeGesture();setHostSyncOff();}tempoEntry.setVisible(false);update();};
        tempoButton.doubleClickHandler=[this]{tempoEntry.setText(tempoButton.getButtonText(),false);tempoEntry.setVisible(true);tempoEntry.toFront(true);tempoEntry.grabKeyboardFocus();tempoEntry.selectAll();};
        arpButton.dragPixelsPerStep=2;arpButton.dragHandler=[this](int steps,bool){arpAccum+=static_cast<float>(steps)*static_cast<float>(uiSpeed("ARP RATE"));
            const int whole=static_cast<int>(arpAccum>=0?std::floor(arpAccum):std::ceil(arpAccum));if(whole==0)return;arpAccum-=static_cast<float>(whole);
            const bool sync=processor.parameters.getRawParameterValue("arp_sync")->load()>0.5f;auto* par=processor.parameters.getParameter(sync?"arp_grid":"arp_time");
            const float value=par->convertFrom0to1(par->getValue())+static_cast<float>(whole)*(sync?1.0f:10.0f);
            par->beginChangeGesture();par->setValueNotifyingHost(par->convertTo0to1(juce::jlimit(sync?0.0f:5.0f,sync?15.0f:2000.0f,value)));par->endChangeGesture();update();};
        arpButton.textHeight=18;arpButton.rightClick=[this]{if(settings)closeOverlay();else showSettings(false);}; // 1.6.22: ПКМ по ARP открывает И закрывает страницу
        arpButton.doubleClickHandler=[this]{if(!settings)showSettings(false);}; // 1.6.24: двойной клик = страница ARP
        // 1.6.12: SET -- переключатель: повторное нажатие закрывает страницу ARP.
        matrixButton.textHeight=22;matrixButton.onClick=[this]{showSettings(true);};
        p2Button.textHeight=22;p2Button.setTooltip("P1|P2 (1.8.0): switch the faceplate. Label = the page you ARE on. P2 = an FX slot fed by the OUTPUT of P1: own DIST/VOL/PAN, FILT/EFFX pages, AMP gate (mutes tails) and LFO 4-6. Knobs keep their P1 places. P2 = INSERT: the signal goes through the picked machine; THRU = dry."); // 1.8.0
        p2Button.onClick=[this]{ pushUndoPoint(); p2View=!p2View; bind(); }; // 1.8.0: P1|P2 -- та же панель, те же места ручек
        fxButton.textHeight=15;fxButton.setTooltip("FX: vertical FX slots -- native P2 machine list, classic-stage hats, SPLIT and 32 user slots.");
        fxButton.onClick=[this]{if(fxSlotsPage)closeOverlay();else showFxSlots();};
        // 1.6.5: кнопки MSEG переехали в освободившуюся верхнюю правую панель
        // (ранее LFO1), рядом с быстрыми ручками RATE/SYNC/LOOP.
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
    dspMenuLf.setColour(juce::PopupMenu::backgroundColourId,juce::Colours::black); // 1.6.21: фон меню DSP MODE чёрный, шрифт обычный
    }
    ~Surface()override{removeMouseListener(&rmbWatcher);closeExtraPanel();stopTimer();viewport.setViewedComponent(nullptr);if(arpWindow)arpWindow->setLookAndFeel(nullptr);arpWindow.reset();setLookAndFeel(nullptr);} // 1.6.41: убрать плавающее окно доп-настроек; 1.7.6: LF детач-окна снять до удаления
    // 1.6.14: ESC выходит из матрицы/арпа/других поверхностных окон.
    bool keyPressed(const juce::KeyPress& k)override{
        if(k.isKeyCode(juce::KeyPress::escapeKey)){lfoDragPageCandidate=-1;lfoDragPageSince=0;if(viewport.isVisible()||pickingTarget)closeOverlay();return true;}
        return false;
    }
    void mouseDown(const juce::MouseEvent&)override{grabKeyboardFocus();} // чтобы ESC доходил
    // Cord navigation over LFO page buttons is deliberately gated: crossing a
    // category at speed cannot steal the visible LFO page. The cursor must be
    // inside the button's inner region for a short dwell; the timer lets that
    // intentional hold work even when the mouse stops moving.
    void timerCallback()override{
        if(modDragSrc<4){lfoDragPageCandidate=-1;lfoDragPageSince=0;for(int i=0;i<6;++i){auto& b=lfoButton(i);b.grayBlink=false;b.dimmed=false;}return;}
        updateLfoDragPageNavigation(modDragScreen);
        for(int i=0;i<6;++i)lfoButton(i).grayBlink=(lfoPageSel==i);
    }
    // Direct PAGE/DEST edits stay on the current faceplate. A visible matching
    // control receives one short fading acknowledgement; every older frame is
    // cleared first. Nothing survives a completed patchcord drag.
    void outlineLfoDestination(uint8_t target,juce::uint32 fadeMilliseconds=180){
        for(auto& column:cells)for(auto& cell:column)if(cell)cell->clearDirectLfoOutline();
        if(fadeMilliseconds==0)return;
        for(auto& column:cells)for(auto& cell:column)if(cell&&cell->isTarget(target))cell->flashDirectLfoOutline(fadeMilliseconds);
    }
    void bind(){
        for(auto& column:cells)for(auto& cell:column)if(cell)cell->setTargetOverride(-1); // 1.8.0: сброс P2-целей
        if(p2View){bindP2();return;} // P2 side stays user-selected except during cord hover navigation
        boundMachine=processor.machineIndex();updateDspModeLabel();const auto& m=nova::machines()[static_cast<size_t>(boundMachine)];machineButton.setButtonText(m.name);machineButton.textHeight=25;p2Button.setButtonText("P1"); // 1.6.13 // 1.8.0: подпись = текущая страница
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
        for(int i=0;i<6;++i){auto& b=lfoButton(i);b.selected=(lfoPageSel==i);b.label=juce::String(i+1);} // 1.8.0e: 6 кнопок, на обеих страницах 1..6
        if(!processor.isSynthVersion)updateGateLabel();
        repaint();
    }
    // 1.8.0: страница P2 -- те же 6 панелей и те же координаты, что у P1:
    // [0] ручки FX-слоты (p2m<id>_*), [1] P2 AMP (ATK/HOLD/DEC/REL + LEV/MIX),
    // [2] страница LFO4/5/6, [3] P2 FILT (p2_1_*), [4] P2 EFFX (p2_2_*), [5] MSEG (общие).
    void bindP2(){
        boundMachine=processor.machineIndex();
        const auto& fxm=MonomachineNovaAudioProcessor::p2FxMachines();
        auto* mraw=processor.parameters.getRawParameterValue("p2_machine");
        const int sel=juce::jlimit(0,static_cast<int>(fxm.size())-1,juce::roundToInt(mraw?mraw->load():0.0f));
        const auto& m=fxm[static_cast<size_t>(sel)];
        machineButton.setButtonText(m.name);machineButton.textHeight=25;p2Button.setButtonText("P2"); // 1.8.0: подпись = ТЕКУЩАЯ страница
        for(int i=0;i<8;++i){const auto& def=m.synthParams[static_cast<size_t>(i)];
            cells[0][static_cast<size_t>(i)]->bind(def.maxVal?("p2m"+juce::String(m.id)+"_"+juce::String(i)):juce::String(),def.maxVal?def.name:juce::String("---"),-1,i,-1);
            cells[0][static_cast<size_t>(i)]->setTargetOverride(132+i);} // P2 SYN 1..8
        const char* ampIds[]{"p2_amp_atk","p2_amp_hold","p2_amp_dec","p2_amp_rel"};
        for(int i=0;i<4;++i){cells[1][static_cast<size_t>(i)]->bind(ampIds[i],nova::pageLabel(0,i),0,i,-1,true);cells[1][static_cast<size_t>(i)]->setTargetOverride(140+i);} // P2 AMP; ПКМ-курсоры -> окно огибающей P2
        const char* amp2Ids[]{"p2_0_4","p2_0_5","p2_0_6"};
        for(int i=4;i<7;++i){cells[1][static_cast<size_t>(i)]->bind(amp2Ids[i-4],nova::pageLabel(0,i),0,i,-1,true);cells[1][static_cast<size_t>(i)]->setTargetOverride(132+i);} // 1.8.0 FIX: DIST/VOL/PAN как у P1 (цели 144..146)
        cells[1][7]->bind("p2_mix","MIX",-1,7,-1,true);cells[1][7]->setTargetOverride(147); // 1.8.0c: DRY/WET второй страницы на слоте PORT (модулируемая, цель 147)
        const int page=9+lfoPageSel; // 1.8.0e: P2 LFO1-6 -- страницы 9..14, кнопки 1..6
        for(int i=0;i<8;++i)cells[2][static_cast<size_t>(i)]->bind(nova::pageParam(page,i),nova::pageLabel(page,i),page,i);
        for(int i=0;i<8;++i){cells[3][static_cast<size_t>(i)]->bind("p2_1_"+juce::String(i),nova::pageLabel(1,i),1,i,-1,true);cells[3][static_cast<size_t>(i)]->setTargetOverride(148+i);} // P2 FILT
        for(int i=0;i<8;++i){cells[4][static_cast<size_t>(i)]->bind("p2_2_"+juce::String(i),nova::pageLabel(2,i),2,i,-1);cells[4][static_cast<size_t>(i)]->setTargetOverride(156+i);} // 1.8.0b FIX: P2 EFFX -- цели прицела (обводка как у P1); DTIM/DSND-меню общие
        cells[5][0]->bind("mseg_rate","RATE",-1,0,-1,false);
        cells[5][1]->bind("mseg_sync","SYNC",-1,1,-1,false);
        cells[5][2]->bind("mseg_loop","LOOP",-1,2,-1,false);
        for(int i=3;i<8;++i)cells[5][static_cast<size_t>(i)]->bind(juce::String(),"---",-1,i,-1,false);
        rebuildModeControls();
        for(int i=0;i<6;++i)lfoButton(i).selected=(lfoPageSel==i); // 1.8.0e
        repaint();
    }
    // 1.6.5: модуляция «прицелом» перетаскиванием с кнопок LFO1/2/3:
    // зажал кнопку, довёл до ручки, отпустил -- маршрут назначен (без второго
    // клика, в отличие от матрицы).
    int modSrcSel(int src)const{return (src>=4&&src<=6)?src-4:(src>=23&&src<=25)?src-20:(src>=26&&src<=31)?src-26:-1;} // 1.8.1b: LFO-источник корда -> 0..5
    int modSrcPage(int src)const{const int s2=modSrcSel(src);if(s2<0)return -1;return src>=26?9+s2:3+s2;} // source side is encoded in src, never in mutable p2View
    void beginModDrag(int src,bool matrix){modDragSrc=src;modDragSourcePage=modSrcPage(src);modDragMatrix=matrix;modDragOverP2Icon=false;modHover=nullptr;lfoDragPageCandidate=-1;lfoDragPageSince=0;
        for(auto& column:cells)for(auto& cell:column)if(cell)cell->clearDirectLfoOutline();
        const int mSel=modSrcSel(src);
        if(mSel>=0)lfoButton(mSel).dimmed=true; // 1.6.21: источник прицела чуть серее
        aimBackupValid=false; // 1.6.24: запомнить PAGE/DEST до попытки модуляции
        if(mSel>=0){const int mPg=modDragSourcePage;auto* pp=processor.parameters.getParameter(nova::pageParam(mPg,0));auto* dd=processor.parameters.getParameter(nova::pageParam(mPg,1)); // 1.8.1b: LFO4-6/P2 тоже
            if(pp&&dd){aimBackupPage=pp->convertFrom0to1(pp->getValue());aimBackupDest=dd->convertFrom0to1(dd->getValue());aimBackupValid=true;}}
        for(auto& column:cells)for(auto& cell:column)if(cell)cell->setPickMode(true);}
    ModSourceButton& lfoButton(int i){return i==0?lfo1Button:i==1?lfo2Button:i==2?lfo3Button:i==3?lfo4Button:i==4?lfo5Button:lfo6Button;} // 1.8.0e
    // 1.6.8: прицеливание кнопкой LFO1/2/3.
    // ЛКМ: наведение на ручку СРАЗУ переключает внутренний адресат этого LFO --
    // PAGE (1-й параметр страницы LFO) и DEST (2-й параметр, 0..7), заменяя
    // текущий. Матрица не участвует. PTCH|SYNT|AMP|FILT|EFFX|LFO1|LFO2|LFO3.
    // ПКМ: только подсветка; маршрут в матрицу создаётся при отпускании.
    void aimLfoDest(uint8_t target){
        const bool p2src=modDragSrc>=26;
        const int sel=p2src?modDragSrc-26:(modDragSrc>=23&&modDragSrc<=25)?modDragSrc-20:modDragSrc-4;
        if(modDragSrc<4||(modDragSrc>6&&(modDragSrc<23||modDragSrc>31)))return;
        const int page=p2src?9+sel:3+sel;
        int pageValue=-1;
        if(!nova::lfoPageForDirectTarget(p2src,target,pageValue))return; // ARP/MSEG/routes have no direct PAGE/DEST encoding.
        const int destination=target==nova::kPitchMatrixTarget?0:target%8;
        const int mi=processor.machineIndex();
        const int machineId=mi>=0?nova::machines()[static_cast<size_t>(mi)].id:0;
        if(!nova::lfoDirectTargetIsContinuousForCurrentP1Machine(machineId,target))return;
        auto set=[this](const juce::String& id,float value){if(auto* par=processor.parameters.getParameter(id)){par->beginChangeGesture();par->setValueNotifyingHost(par->convertTo0to1(value));par->endChangeGesture();}};
        set(nova::pageParam(page,0),static_cast<float>(pageValue));
        set(nova::pageParam(page,1),static_cast<float>(destination));
    }
    void modDragMove(juce::Point<int> screen){modDragScreen=screen;
        // P1/P2 is drag-navigation only: entering its icon toggles once and
        // gives short button feedback. Direct PAGE/DEST editing never calls it.
        const bool overP2=p2Button.getScreenBounds().contains(screen);
        if(overP2&&!modDragOverP2Icon){if(modHover)modHover->setHover(false);modHover=nullptr;p2View=!p2View;bind();p2Button.flashFor(180);}
        modDragOverP2Icon=overP2;
        updateLfoDragPageNavigation(screen);
        auto* cell=cellAtScreen(screen);if(cell!=modHover){
        if(modHover)modHover->setHover(false); // 1.6.13: подсветка снимается МГНОВЕННО, без шлейфа
        modHover=cell;
        if(cell){cell->setHover(true);if(!modDragMatrix&&cell->canBeTarget())aimLfoDest(cell->targetId());}}}
    Cell* cellAtScreen(juce::Point<int> screen){for(auto& column:cells)for(auto& cell:column)if(cell&&cell->getScreenBounds().contains(screen))return cell.get();return nullptr;}
    int overLfoButton(juce::Point<int> screen){const juce::Component* btns[6]={&lfo1Button,&lfo2Button,&lfo3Button,&lfo4Button,&lfo5Button,&lfo6Button};for(int i=0;i<6;++i)if(btns[i]->getScreenBounds().contains(screen))return i;return -1;} // release target: full button remains valid
    int overLfoNavigationRegion(juce::Point<int> screen){
        const juce::Component* btns[6]={&lfo1Button,&lfo2Button,&lfo3Button,&lfo4Button,&lfo5Button,&lfo6Button};
        for(int i=0;i<6;++i){auto r=btns[i]->getScreenBounds();r.reduce(6,4);if(r.contains(screen))return i;}
        return -1;
    }
    void updateLfoDragPageNavigation(juce::Point<int> screen){
        const int over=overLfoNavigationRegion(screen);
        if(over<0||over==lfoPageSel){lfoDragPageCandidate=-1;lfoDragPageSince=0;return;}
        const auto now=juce::Time::getMillisecondCounter();
        if(lfoDragPageCandidate!=over){lfoDragPageCandidate=over;lfoDragPageSince=now;return;}
        if(now-lfoDragPageSince<170u)return; // fast crossings are ignored
        lfoPageSel=juce::jlimit(0,5,over);lfoDragPageCandidate=-1;lfoDragPageSince=0;
        if(modHover)modHover->setHover(false);modHover=nullptr;bind();
    }
    void endModDrag(juce::Point<int> screen,bool dragged,int lfoIndex,bool matrix){
        for(auto& column:cells)for(auto& cell:column)if(cell)cell->setPickMode(false);
        // 1.6.17: отпустил прицел ЛКМ над кнопкой ДРУГОГО LFO -- этот LFO становится
        // целью (PAGE указывает на его страницу), и мы попадаем на его страницу,
        // будто нажали её кнопку: дальше можно выбрать конкретную ручку LFO2.
        if(dragged&&!matrix&&modDragSrc>=4){
            const int over=overLfoButton(screen);
            if(over>=0&&over!=lfoIndex){const int rowBase=p2View?(over<3?164:212):(over<3?32:188);aimLfoDest(static_cast<uint8_t>(rowBase+over%3*8));lfoPageSel=juce::jlimit(0,5,over);bind();} // 1.8.0e: строки своей страницы (P2/P1, тройки 1-3/4-6)
        }
        const int src=modDragSrc;modDragSrc=-1;if(modHover)modHover->setHover(false);modHover=nullptr;modDragMatrix=false;lfoDragPageCandidate=-1;lfoDragPageSince=0;for(int i=0;i<6;++i)lfoButton(i).grayBlink=false; // 1.6.31: мигание параметра убрано -- рамка DEST в матрице
        if(src<0){modDragOverP2Icon=false;modDragSourcePage=-1;return;}
        bool landed=false;
        if(dragged){
            auto* cell=cellAtScreen(screen);
            if(cell&&cell->canBeTarget()){
                landed=true;
                if(matrix){const int slot=processor.addRouteFromSource(src,cell->targetId()); // ПКМ: маршрут в матрицу // 1.8.0: LFO4-6 = 23..25
                    setViewportMode(true);if(matrixPage)matrixPage->flashDestCell(slot);} // 1.6.33: бинд кордом ОТКРЫВАЕТ матрицу, мигает РАМКА DEST-ячейки (как из прицела матрицы)
                else{ // direct PAGE/DEST; retain the visible faceplate and outline only its matching target
                    const auto target=cell->targetId();aimLfoDest(target);outlineLfoDestination(target,0);
                }
            }
        }
        else if(!matrix){lfoPageSel=juce::jlimit(0,5,lfoIndex);bind();} // клик ЛКМ -- показать страницу этого LFO
        // 1.6.24: провод отпущен НЕ на модуляторе -- откат PAGE/DEST к значениям
        // до попытки (наводение по пути меняло их вживую).
        if(dragged&&!matrix&&modSrcSel(src)>=0&&!landed&&overLfoButton(screen)<0&&aimBackupValid){ // 1.8.1b: откат для любого LFO (LFO4-6/P2 раньше не откатывался)
            auto set=[this](const juce::String& id,float v){if(auto* p=processor.parameters.getParameter(id)){p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(v));p->endChangeGesture();}};
            if(modDragSourcePage>=0){set(nova::pageParam(modDragSourcePage,0),aimBackupPage);set(nova::pageParam(modDragSourcePage,1),aimBackupDest);}
            lfoPageSel=modSrcSel(src);bind();}
        modDragOverP2Icon=false;modDragSourcePage=-1;
        repaint();
    }
    // 1.6.13: в режиме прицела удержание над кнопкой другого LFO (1 с) показывает его страницу.
    void holdLfoPage(int i){ if(modSrcSel(modDragSrc)>=0&&!modDragMatrix){ lfoPageSel=juce::jlimit(0,5,i); bind(); } } // 1.6.20: и возврат к исходному LFO
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
        {const int lx=tagLogo->getX()-tagLogo->base.getX(),ly=tagLogo->getY()-tagLogo->base.getY();for(int i=0;i<5;++i){float angle=i*2*nova::pi/5;g.fillRect(25+lx+juce::roundToInt(12*std::sin(angle)),22+ly+juce::roundToInt(12*std::cos(angle)),8,8);}g.fillRect(24+lx,20+ly,13,13);} // 1.6.42: значок двигается тегом
        auto drawTag=[this,&g](TextTag* tg,const juce::String& fb,int fh){ if(tg==nullptr)return; const juce::String s=tg->tagText.isNotEmpty()?tg->tagText:(tg->factoryText.isNotEmpty()?tg->factoryText:fb); g.setColour(tg->colorSet?tg->tagColor:ink); // 1.6.43: стиль тега
            if(tg->fontId==1){g.setFont(juce::Font(juce::Font::getDefaultSansSerifFontName(),(float)tg->fontFor(fh),juce::Font::plain));g.drawText(s,tg->getBounds(),juce::Justification::centredLeft);}
            else if(tg->fontId==2){g.setFont(juce::Font(juce::Font::getDefaultMonospacedFontName(),(float)tg->fontFor(fh),juce::Font::plain));g.drawText(s,tg->getBounds(),juce::Justification::centredLeft);}
            else pixel::text(g,s,tg->getBounds(),tg->fontFor(fh)); };
        drawTag(tagTitle.get(),processor.isSynthVersion?"MONOMACHINE NOVA SYNTH":"MONOMACHINE NOVA FX",24); // 1.6.43: + цвет/шрифт
        drawTag(tagBpm.get(),"BPM",12);drawTag(tagLev.get(),"LEV",28); // 1.6.43: + цвет/шрифт // 1.6.14: BPM-подпись над цифрами, SYNC слева
        pixel::dotted(g,8,94,8,562);pixel::dotted(g,61,94,61,562);pixel::dotted(g,8,94,63,94);pixel::dotted(g,8,562,63,562); // 1.6.31: низ пунктира = линия секций (562)
        g.setColour(juce::Colours::white);g.drawHorizontalLine(124,27.0f,43.0f);g.drawHorizontalLine(224,27.0f,43.0f); // 1.6.31: риски LEV по сетке панелей (макс = низ шапки, дефолт = пунктир ячеек)
        const juce::String lfoPanelName="LFO"; // 1.6.8: без цифры -- номер страницы виден на кнопках [LFO1]/[LFO2]/[LFO3]
        const juce::String names[]{"SYNT","",lfoPanelName,"FILT","EFFX","MSEG"}; // 1.6.8: LFO наверху, MSEG внизу
        for(int panel=0;panel<6;++panel){int x=72+(panel%3)*399,y=94+(panel/3)*240;
            g.setColour(ink);g.drawRect(x,y,387,30,2); // 1.6.31: серый прямоугольник панели убран (разметка не видна), рамка только у шапки
            g.setColour(ink);for(int c=0;c<=4;++c)pixel::dotted(g,x+c*96,y+34,x+c*96,y+228);
            pixel::dotted(g,x,y+34,x+388,y+34);pixel::dotted(g,x,y+130,x+388,y+130);pixel::dotted(g,x,y+228,x+388,y+228);}

        g.setColour(ink.withAlpha(0.65f));g.drawText(processor.isSynthVersion?"BUILD 1.9.9 / Synth":"BUILD 1.9.9 / FX",850,570,370,13,juce::Justification::right); // версия сборки видна всегда
    }
    void resized()override{
        ampButton.setBounds(471,95,96,28);ampModeCombo.setBounds(759,95,96,28); // 1.6.32: AMP -- правая стенка на границе колонки крутилок; MODE -- левый край на линии колонки, границы = границы категории
        syntCatButton.setBounds(72,95,96,28);lfoCatButton.setBounds(870,95,96,28);filtCatButton.setBounds(72,335,96,28);effxCatButton.setBounds(471,335,96,28);msegCatButton.setBounds(870,335,96,28); // 1.6.32 // 1.6.28: кнопки категорий
        syntModeCombo.setBounds(360,95,99,28);filtModeCombo.setBounds(0,0,0,0);hybridModeLCombo.setBounds(168,335,94,28);hybridModeHCombo.setBounds(264,335,94,28);hybridModeSCombo.setBounds(360,335,99,28);effxDistCombo.setBounds(663,335,96,28);effxDlyCombo.setBounds(759,335,99,28); // MODE DLY belongs above DTIM; MODE S remains above DIST/SRR
        ampModeCombo.setBounds(759,95,99,28); // 1.6.34: до рамки панели AMP
        machineButton.setBounds(72,58,387,30);dspModeButton.setBounds(1014,58,140,30);undoBtn.setBounds(1098,4,25,30);redoBtn.setBounds(1129,4,25,30);menuButton.setBounds(1161,0,94,32);tempoSync.setBounds(961,4,61,30);tempoButton.setBounds(1022,5,70,30);tempoEntry.setBounds(1022,5,70,30); // 1.6.43: surf.* из JSON пользователя (undo/redo/menu/sync/bpm) // 1.6.32: UNDO/REDO размером галки ARP; 1.6.20: SYNC вплотную к цифрам BPM
        arpEnabled.setBounds(471,57,32,32);arpButton.setBounds(507,59,116,30);gateOuter.setBounds(624,58,42,32);trigToggle.setBounds(661,58,105,30);matrixButton.setBounds(870,58,140,30);fxButton.setBounds(768,58,34,30);p2Button.setBounds(806,58,58,30); // 1.8.0: кнопка P1|P2 -- СЛЕВА от матрицы; сама матрица на старом месте // 1.6.34: галка -- верх на 1px выше и низ на 1px ниже уровня кнопки ARP (rate) // 1.6.33: галка -- чуть левее AMP-категории, чуть выше и чуть ниже вниз; 1.6.29: ARP вплотную к галке // 1.6.29: ARP прилегает к галке, шаги ARP->GATE->TRIG сохранены // 1.6.43: surf.* (arpon/arp/gate)
        lfo1Button.setBounds(975,97,30,24);lfo2Button.setBounds(1008,97,30,24);lfo3Button.setBounds(1041,97,30,24);lfo4Button.setBounds(1074,97,30,24);lfo5Button.setBounds(1107,97,30,24);lfo6Button.setBounds(1140,97,30,24); // 1.8.0e: ряд 1..6
        lfoJack.setBounds(1174,97,44,24); // 1.8.0e: патч-корд правее ряда 1..6
        level.setBounds(12,124,46,438); // 1.6.32: верх фейдера = риска максимума
        for(int panel=0;panel<6;++panel)for(int i=0;i<8;++i)cells[static_cast<size_t>(panel)][static_cast<size_t>(i)]->setBounds(75+(panel%3)*399+(i%4)*96,130+(panel/3)*240+(i/4)*96,91,91);
        viewport.setBounds(72,95,1180,470);closeButton.setBounds(870,58,140,30);gateButton.setVisible(false);cableLayer.setBounds(getLocalBounds()); // 1.6.21
        uiLayoutApply(*this,"surf"); // 1.6.38: сохранённая раскладка поверх дефолта -- В КОНЦЕ resized, после всех дефолтных setBounds
    }
    // 1.6.12: единое закрытие оверлея; SET теперь переключатель (повторное
    // нажатие закрывает страницу ARP -- без мигания пересозданной страницы).
    void closeOverlay(){
        lfoDragPageCandidate=-1;lfoDragPageSince=0; // overlay close/ESC also cancels pending cord-page intent
        if(pickingTarget){for(auto& column:cells)for(auto& cell:column)if(cell)cell->setPickMode(false);pickingTarget=false;modEnvPickPage=-1;}
        viewport.setViewedComponent(nullptr);settings.reset();matrixPage.reset();samplePage.reset();msegPage.reset();envelopePage.reset();fxSlotsPage.reset();viewport.setVisible(false);closeButton.setVisible(false);repaint();
    }
    void showMseg(int page=0){const int keepY=viewport.getViewPositionY();viewport.setViewedComponent(nullptr,false);settings.reset();matrixPage.reset();samplePage.reset();envelopePage.reset();fxSlotsPage.reset();msegPage=std::make_unique<MsegPage>(processor,page);msegPage->pushEdit=[this]{pushUndoPoint();};wireMsegAim();viewport.setViewedComponent(msegPage.get(),false);viewport.setViewPosition(juce::Point<int>(0,keepY));viewport.setVisible(true);viewport.toFront(false);closeButton.setVisible(true);closeButton.toFront(false);} // 1.6.35: возврат в MSEG -- скролл как был // 1.6.21: номер страницы = легаси mseg_* либо mseg2_*/mseg3_*
    void showFxSlots(){
        dismissDock();viewport.setViewedComponent(nullptr,false);settings.reset();matrixPage.reset();samplePage.reset();msegPage.reset();envelopePage.reset();
        fxSlotsPage=std::make_unique<FxSlotsPage>(processor);viewport.setViewedComponent(fxSlotsPage.get(),false);viewport.setViewPosition(0,0);
        viewport.setVisible(true);viewport.toFront(false);closeButton.setVisible(true);closeButton.toFront(false);
    }
    void showEnvelope(int knob){
        dismissDock();viewport.setViewedComponent(nullptr,false);settings.reset();matrixPage.reset();samplePage.reset();msegPage.reset();fxSlotsPage.reset();
        auto page=std::make_unique<EnvelopePage>(processor,false,p2View);auto* raw=page.get();
        page->toggleSize=[this]{const auto sp=juce::Component::SafePointer<Surface>(this);juce::MessageManager::callAsync([sp]{if(sp!=nullptr){sp->closeOverlay();sp->toggleEnvMini();}});};
        page->selectTab=[this](int tab){const auto safe=juce::Component::SafePointer<Surface>(this);juce::MessageManager::callAsync([safe,tab]{if(safe==nullptr)return;if(tab==1)safe->showFilterEnvelope();else if(tab==2)safe->showModEnvelope(0);});};
        envelopePage=std::move(page);viewport.setViewedComponent(envelopePage.get(),false);viewport.setVisible(true);viewport.toFront(false);closeButton.setVisible(true);closeButton.toFront(false);if(knob>=0)raw->focusCurve(knob);
    }
    void showFilterEnvelope(){
        dismissDock();viewport.setViewedComponent(nullptr,false);settings.reset();matrixPage.reset();samplePage.reset();msegPage.reset();fxSlotsPage.reset();
        auto page=std::make_unique<FilterEnvelopePage>(processor,p2View);page->selectTab=[this](int tab){const auto safe=juce::Component::SafePointer<Surface>(this);juce::MessageManager::callAsync([safe,tab]{if(safe==nullptr)return;if(tab==0)safe->showEnvelope(-1);else if(tab==2)safe->showModEnvelope(0);});};
        envelopePage=std::move(page);viewport.setViewedComponent(envelopePage.get(),false);viewport.setVisible(true);viewport.toFront(false);closeButton.setVisible(true);closeButton.toFront(false);
    }
    void showModEnvelope(int selected=0){
        dismissDock();viewport.setViewedComponent(nullptr,false);settings.reset();matrixPage.reset();samplePage.reset();msegPage.reset();fxSlotsPage.reset();
        auto page=std::make_unique<ModEnvelopePage>(processor,selected);page->selectTab=[this](int tab){const auto safe=juce::Component::SafePointer<Surface>(this);juce::MessageManager::callAsync([safe,tab]{if(safe==nullptr)return;if(tab==0)safe->showEnvelope(-1);else if(tab==1)safe->showFilterEnvelope();});};page->addRoute=[this](int env){beginModEnvTargetPick(env);};
        envelopePage=std::move(page);viewport.setViewedComponent(envelopePage.get(),false);viewport.setVisible(true);viewport.toFront(false);closeButton.setVisible(true);closeButton.toFront(false);
    }
    void setViewportMode(bool matrix){
        viewport.setViewedComponent(nullptr,false);envelopePage.reset();samplePage.reset();msegPage.reset();fxSlotsPage.reset();
        if(matrix){settings.reset();matrixPage=std::make_unique<MatrixPage>(processor);matrixPage->onTargetPick=[this](int row){beginTargetPick(row);};matrixPage->pushEdit=[this]{pushUndoPoint();};viewport.setViewedComponent(matrixPage.get(),false);}
        else{matrixPage.reset();settings=std::make_unique<ArpPage>(processor);settings->pushEdit=[this]{pushUndoPoint();};settings->armRequest=[this](int slot){beginPlockPick(slot);};settings->detachRequest=[this]{detachArp();};viewport.setViewedComponent(settings.get(),false);} // 1.7.1: прицел P-LOCK
        viewport.setVisible(true);viewport.toFront(false);closeButton.setVisible(true);closeButton.toFront(false);
    }
    struct ArpDialog final : juce::DialogWindow { ArpDialog():juce::DialogWindow("ARP",juce::Colours::black,true,true){setTitleBarHeight(0);setAlwaysOnTop(true);} void closeButtonPressed() override {} }; // 1.7.7: БЕЗ заголовка (заголовок не нужен), окно ПОВЕРХ всех
    void detachArp(){ // 1.7.7: DETACH = открыть окно; повторный DETACH (или кнопка ARP) = ВЕРНУТЬ страницу в синх. Окно = тот же класс ArpPage (копия раскладки), стиль мода
        if (arpWindow) { arpWindow.reset(); closeOverlay(); setViewportMode(false); return; }
        auto* page = new ArpPage(processor);
        page->pushEdit = [this] { pushUndoPoint(); }; page->armRequest = [this](int slot) { beginPlockPick(slot); };
        page->detachRequest = [this] { detachArp(); }; // DETACH на детачнутой странице = вернуть
        arpWindow = std::make_unique<ArpDialog>();
        arpWindow->setLookAndFeel(&getLookAndFeel()); // контент и списки в стиле мода
        arpWindow->setContentOwned(page, true);
        arpWindow->centreWithSize(page->getWidth(), page->getHeight() + 2); arpWindow->setVisible(true); arpWindow->toFront(true);
        closeOverlay(); } // страница ARP в оверлее закрывается -- ручки лицевой панели свободны
    void beginPlockPick(int slot){ // 1.7.1: прицел P-LOCK -- окно ARP прячется, клик по ручке = цель слота
        plockPickSlot=juce::jlimit(0,31,slot);pickingTarget=true; // 1.7.11 FIX: было 0..3 -- прицел молча не работал для слотов 4+
        for(auto& column:cells)for(auto& cell:column)if(cell)cell->setPickMode(true,true);
        viewport.setVisible(false);repaint();}
    void beginTargetPick(int route){
        pickRoute=juce::jlimit(0,63,route);pickingTarget=true; // 1.7.7: строки матрицы 0..63
        for(auto& column:cells)for(auto& cell:column)if(cell)cell->setPickMode(true,true); // 1.6.28: выбор из матрицы -- ЛКМ мапит
        viewport.setVisible(false);repaint();
    }
    void beginModEnvTargetPick(int env){
        modEnvPickPage=juce::jlimit(0,3,env);pickingTarget=true;
        for(auto& column:cells)for(auto& cell:column)if(cell)cell->setPickMode(true,true);
        viewport.setVisible(false);repaint();
    }
    juce::String matrixRouteName(int r) const { // 1.7.5/1.7.7: динамическое имя маршрута (0..63)
        auto* d = processor.parameters.getRawParameterValue("r" + juce::String(r) + "_dest"); const int v = d ? juce::roundToInt(d->load()) : 0;
        return v > 0 ? targetName(processor, juce::jlimit(0, 247, v - 1)) : juce::String("OFF"); }
    void showMsegDestList(int pg,int row){
        juce::PopupMenu m;m.setLookAndFeel(&getLookAndFeel());auto addTarget=[this](juce::PopupMenu& dst,int target){dst.addItem(10000+target,targetName(processor,target));};auto addRange=[&](juce::PopupMenu& dst,int first,int count){for(int i=0;i<count;++i)addTarget(dst,first+i);};
        {juce::PopupMenu p1,synt;addRange(synt,0,8);addTarget(synt,66);p1.addSubMenu("SYNT",synt);juce::PopupMenu amp;addRange(amp,8,8);p1.addSubMenu("AMP",amp);juce::PopupMenu filt;addRange(filt,16,8);p1.addSubMenu("FILT",filt);juce::PopupMenu fx;addRange(fx,24,8);p1.addSubMenu("EFFX",fx);m.addSubMenu("P1 PARAM",p1);}
        {juce::PopupMenu p1l;for(int i=0;i<3;++i){juce::PopupMenu q;addRange(q,32+i*8,8);p1l.addSubMenu("LFO"+juce::String(i+1),q);}for(int i=0;i<3;++i){juce::PopupMenu q;addRange(q,188+i*8,8);p1l.addSubMenu("LFO"+juce::String(i+4),q);}m.addSubMenu("P1 LFO",p1l);}
        {juce::PopupMenu p2,synt;addRange(synt,132,8);p2.addSubMenu("SYNT",synt);juce::PopupMenu amp;addRange(amp,140,8);p2.addSubMenu("AMP",amp);juce::PopupMenu filt;addRange(filt,148,8);p2.addSubMenu("FILT",filt);juce::PopupMenu fx;addRange(fx,156,8);p2.addSubMenu("EFFX",fx);m.addSubMenu("P2 PARAM",p2);}
        {juce::PopupMenu p2l;for(int i=0;i<3;++i){juce::PopupMenu q;addRange(q,164+i*8,8);p2l.addSubMenu("LFO"+juce::String(i+1),q);}for(int i=0;i<3;++i){juce::PopupMenu q;addRange(q,212+i*8,8);p2l.addSubMenu("LFO"+juce::String(i+4),q);}m.addSubMenu("P2 LFO",p2l);}
        {juce::PopupMenu arp;addTarget(arp,64);addTarget(arp,65);addTarget(arp,244);addTarget(arp,245);m.addSubMenu("ARP",arp);} {juce::PopupMenu plockPattern;addTarget(plockPattern,246);addTarget(plockPattern,247);m.addSubMenu("P-LOCK PATTERN",plockPattern);} {juce::PopupMenu outs;addRange(outs,56,8);m.addSubMenu("MSEG OUT",outs);} {juce::PopupMenu rates;addRange(rates,236,8);m.addSubMenu("MSEG RATE",rates);} {juce::PopupMenu legacy;addTarget(legacy,131);m.addSubMenu("LEGACY",legacy);} {juce::PopupMenu routes;addRange(routes,67,64);m.addSubMenu("MATRIX ROUTE DEPTH",routes);}
        const juce::Component::SafePointer<Surface> safe(this);m.showMenuAsync(juce::PopupMenu::Options(),[safe,pg,row](int result){if(!safe||result<10000||result>10247)return;safe->processor.addMsegRoute(pg,static_cast<uint8_t>(result-10000));safe->showMseg(pg);if(safe->msegPage)safe->msegPage->flashRow(row);});
    }
    void wireMsegAim(){ // 1.6.34: гнездо строки MSEG = ПРИЦЕЛ (клик-клик): окно MSEG прячется, клик по ручке = маршрут, возврат с миганием строки
        msegPage->pickStart=[this](int pg,int row){msegPickPage=pg;msegPickPageShown=pg;msegPickRow=row;pickingTarget=true;viewport.setVisible(false);for(auto& column:cells)for(auto& cell:column)if(cell)cell->setPickMode(true,true);repaint();};
        msegPage->destListRequest=[this](int pg,int row){showMsegDestList(pg,row);}; // 1.7.5
    }
    void toggleLayoutEdit(){ // 1.6.38: редактор раскладки -- поверх текущей страницы или лицевой панели
        if(layoutOverlay){ layoutOverlay=nullptr; repaint(); return; }
        auto* vc=viewport.getViewedComponent(); const char* key="surf";
        if(vc!=nullptr){ key=vc==matrixPage.get()?"matrix":vc==settings.get()?"arp":vc==msegPage.get()?"mseg":"page"; layRoot=vc; } else layRoot=this;
        layoutOverlay=std::make_unique<LayoutOverlay>(*layRoot,key);
        addAndMakeVisible(*layoutOverlay); layoutOverlay->setBounds(getLocalBounds()); layoutOverlay->toFront(true);
        layoutOverlay->onDone=[this]{ layoutOverlay=nullptr; repaint(); };
    }
    void completeTargetPick(uint8_t target){
        if(pickingTarget&&plockPickSlot>=0){ // 1.7.2: цель в слот P-LOCK (та же цель = тот же слот, иначе новый) -- назад на ARP
            const int slot=plockPickSlot;plockPickSlot=-1;pickingTarget=false;pushUndoPoint(); // 1.7.3: прицел-бинд = точка отката
            for(auto& column:cells)for(auto& cell:column)if(cell)cell->setPickMode(false); // 1.7.4 FIX: снять режим прицела -- без этого все ручки остаются мертвы
            setViewportMode(false);
            if(settings)settings->plockAfterBind(slot,static_cast<int>(target));
            repaint();return;}
        if(!pickingTarget)return; // 1.6.31: замок маршрута -- только от списков/случайной правки, прицелу не мешает
        if(modEnvPickPage>=0){
            const int env=modEnvPickPage;modEnvPickPage=-1;processor.addRouteFromSource(32+env,target);pickingTarget=false;
            for(auto& column:cells)for(auto& cell:column)if(cell)cell->setPickMode(false);
            showModEnvelope(env);repaint();return;
        }
        if(msegPickPage>=0){ // 1.6.34: прицел из строки маршрутов MSEG -- маршрут ЭТОЙ страницы, возврат в MSEG, мигает строка
            const int slot=processor.addMsegRoute(msegPickPage,target);
            pickingTarget=false;
            const int shown=msegPickPageShown,row=msegPickRow;msegPickPage=-1;
            showMseg(shown);if(msegPage)msegPage->flashRow(row);
            for(auto& column:cells)for(auto& cell:column)if(cell)cell->setPickMode(false);
            return; }
        auto* p=processor.parameters.getParameter("r"+juce::String(pickRoute)+"_dest");
        const bool wasEmpty=p&&juce::roundToInt(p->convertFrom0to1(p->getValue()))==0;
        if(p){p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(target+1)));p->endChangeGesture();} // 1.6.32: dest = цель+1 (0 = OFF)
        if(wasEmpty)processor.moveModRoute(pickRoute,0); // 1.6.34: новый маршрут ВСЕГДА всплывает наверх, остальные едут вниз
        for(auto& column:cells)for(auto& cell:column)if(cell)cell->setPickMode(false);
        setViewportMode(true); // 1.6.32: после бинда ОТКРЫВАЕТСЯ матрица
        if(matrixPage)matrixPage->flashDestCell(wasEmpty?0:pickRoute);
        pickingTarget=false;repaint();
    }
    void showSamples(){viewport.setViewedComponent(nullptr,false);envelopePage.reset();settings.reset();matrixPage.reset();msegPage.reset();fxSlotsPage.reset();samplePage=std::make_unique<SamplePage>(processor);viewport.setViewedComponent(samplePage.get(),false);viewport.setVisible(true);viewport.toFront(false);closeButton.setVisible(true);closeButton.toFront(false);}
    void cycleMachine(int steps){
        if(p2View){ // 1.8.0 FIX: в P2 стрелки/драг листают FX-слоты P2, а не машины инструментов
            auto* p=processor.parameters.getParameter("p2_machine");if(!p)return;
            const int n=static_cast<int>(MonomachineNovaAudioProcessor::p2FxMachines().size());
            int raw=juce::roundToInt(p->convertFrom0to1(p->getValue()));raw=((raw+steps)%n+n)%n;
            p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(raw)));p->endChangeGesture();bind();machineButton.flashFor(160);lastMachineCycleMs=juce::Time::getMillisecondCounter();return;
        }
        auto* p=processor.parameters.getParameter("machine");if(!p)return;
        const int n=static_cast<int>(nova::machines().size());
        int raw=juce::roundToInt(p->convertFrom0to1(p->getValue()));raw=((raw+steps)%n+n)%n;
        p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(raw)));p->endChangeGesture();bind();machineButton.flashFor(160);lastMachineCycleMs=juce::Time::getMillisecondCounter(); // 1.6.29: строка мигает -- пресет сменился и отжался
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
    void showDocked(std::unique_ptr<juce::Component> panel,juce::Rectangle<int> localAt,bool belowAnchor){ closeExtraPanel(); // 1.6.43: dock и доп-панель не сосуществуют
 // 1.6.30: новый стандарт -- список НИЖЕ своего хитбокса
        dismissDock();
        dockPanel=std::move(panel);
        dock=std::make_unique<PopupDock>();dock->setBounds(getLocalBounds());dock->onDismiss=[this]{dismissDock();};addAndMakeVisible(*dock);
        auto* raw=dockPanel.get();
        const int x=juce::jlimit(0,std::max(0,getWidth()-raw->getWidth()),localAt.getX());
        int y=belowAnchor?localAt.getBottom()+4:localAt.getY();
        y=juce::jlimit(0,std::max(0,getHeight()-raw->getHeight()),y);
        raw->setBounds(x,y,raw->getWidth(),raw->getHeight());addAndMakeVisible(*raw);
    }
    void dismissDock(){dock.reset();dockPanel.reset();}
    void toggleEnvMini(){if(dockPanel!=nullptr){dismissDock();return;}if(envelopePage!=nullptr)return; // 1.6.44: мини-ENV НЕ поверх большого окна
        showDocked(std::make_unique<EnvelopePage>(processor,true,p2View),juce::Rectangle<int>(471,95,387,236),false); // 1.8.0
        if(auto* ep=static_cast<EnvelopePage*>(dockPanel.get()))ep->toggleSize=[this]{const auto sp=juce::Component::SafePointer<Surface>(this);juce::MessageManager::callAsync([sp]{if(sp!=nullptr){sp->dismissDock();sp->showEnvelope(-1);}});}; // 1.7.6: ПКМ из малого -- открывает БОЛЬШОЕ окно
        ampButton.toFront(false);ampModeCombo.toFront(false);} // 1.6.42: AMP и MODE остаются видны и кликабельны поверх окна
    void showP2MachineMenu(){ // 1.8.0: выбор FX-слоты P2
        juce::PopupMenu menu;menu.setLookAndFeel(&smallMenuLaf());
        const auto& fxm=MonomachineNovaAudioProcessor::p2FxMachines();
        auto* mraw=processor.parameters.getRawParameterValue("p2_machine");
        const int cur=juce::jlimit(0,static_cast<int>(fxm.size())-1,juce::roundToInt(mraw?mraw->load():0.0f));
        for(int i=0;i<static_cast<int>(fxm.size());++i)menu.addItem(1+i,juce::String(fxm[static_cast<size_t>(i)].name),true,i==cur);
        juce::Component::SafePointer<Surface> safe(this);
        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(&machineButton),[safe](int res){if(!safe||res<=0)return;
            if(auto* p=safe->processor.parameters.getParameter("p2_machine")){safe->pushUndoPoint();p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(res-1)));p->endChangeGesture();}
            safe->bind();});
    }
    void showMachineMenu(){
        if(p2View){showP2MachineMenu();return;} // 1.8.0
        if(lastMachineCycleMs!=0&&juce::Time::getMillisecondCounter()-lastMachineCycleMs<300)return;
        // 1.6.22 (возврат к 1.6.20): тип генератора стоит ВЛЕВО от имени одним
        // словом (FM+STAT SAW) -- на первой машине категории и на подсвеченной
        // строке. Подсветка строки тускло-серая: белая заливка съедала текст.
        struct Picker final : juce::Component {
            MonomachineNovaAudioProcessor& processor;std::vector<int> order;int colW=0;int bound=-1,hover=-1;std::function<void(int)> onPick;std::function<void()> onClose;
            explicit Picker(MonomachineNovaAudioProcessor& p,int current):processor(p),bound(current){
                const int n=static_cast<int>(nova::machines().size());
                for(int pass=0;pass<2;++pass)for(int i=0;i<n;++i) // 1.6.25: SYNTH-машины первым списком, FX -- в самом низу (порядок id не менялся)
                    if((juce::String(nova::machines()[static_cast<size_t>(i)].category).startsWith("FX"))==(pass==1))order.push_back(i);
                int nameW=0;
                for(int idx:order){colW=std::max(colW,static_cast<int>(juce::String(nova::machines()[static_cast<size_t>(idx)].category).length())*12); // 1.6.28: 12 px/символ -- шрифт 14 не сжимается (SWAVE больше не мельче)
                    nameW=std::max(nameW,static_cast<int>(shortName(idx).length())*12);}
                colW=std::max(72,colW+24);
                setSize(std::min(680,colW+nameW+30),std::min<int>(464,static_cast<int>(order.size())*24+10)); setWantsKeyboardFocus(false); }
            static juce::String shortName(int i){ const auto& m=nova::machines()[static_cast<size_t>(i)];auto s=juce::String(m.name);if(s.startsWith(m.category))s=s.substring(static_cast<int>(m.category.size())).trimCharactersAtStart("- ");return s; }
            void paint(juce::Graphics& g) override {
                g.fillAll(juce::Colours::black);g.setColour(juce::Colours::white);g.drawRect(getLocalBounds(),1); // 1.6.25: белая рамка как у всех списков
                juce::String lastCategory;
                for(int row=0;row<static_cast<int>(order.size());++row){
                    const int i=order[static_cast<size_t>(row)];
                    const int y=5+row*24;const auto& m=nova::machines()[static_cast<size_t>(i)];
                    const bool firstOfCat=(lastCategory!=juce::String(m.category));
                    if(row==hover){g.setColour(ink);g.fillRect(4,y-1,getWidth()-8,22);g.setColour(knockout);} // БЕЛОЕ выделение строки
                    else g.setColour(ink);
                    if(firstOfCat||row==hover)pixel::text(g,juce::String(m.category),{8,y,colW-14,20},14,false); // 1.6.26: тип слева ОДИН раз на категорию (и на подсвеченной строке)
                    pixel::text(g,shortName(i),{colW+4,y,getWidth()-colW-10,20},14,false); // 1.6.26: КОРОТКИЕ имена справа, каждая строка (как в самом первом виде)
                    if(firstOfCat)lastCategory=m.category;
                    g.setColour(ink.withAlpha(0.15f));g.drawHorizontalLine(y+23,8.0f,static_cast<float>(getWidth()-8));
                }
            }
            void mouseMove(const juce::MouseEvent& e) override { const int h=(e.y-5)/24; if(h!=hover&&h>=0&&h<static_cast<int>(order.size())){hover=h;repaint();} }
            void mouseDown(const juce::MouseEvent& e) override { const int i=(e.y-5)/24; if(i>=0&&i<static_cast<int>(order.size())&&onPick){onPick(order[static_cast<size_t>(i)]);if(onClose)onClose();} }
            void mouseExit(const juce::MouseEvent&) override { hover=-1;repaint(); }
        };
        auto panel=std::make_unique<Picker>(processor,boundMachine);
        panel->onPick=[this](int i){auto* p=processor.parameters.getParameter("machine");p->beginChangeGesture();p->setValueNotifyingHost(p->convertTo0to1(static_cast<float>(i)));p->endChangeGesture();bind();};
        panel->onClose=[this]{dismissDock();};showDocked(std::move(panel),juce::Rectangle<int>(machineButton.getX(),machineButton.getBottom(),machineButton.getWidth(),0),true); // 1.6.29: окно ПОД строкой машины, в пределах окна плагина
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
        juce::PopupMenu menu;menu.setLookAndFeel(&dspMenuLf); // 1.6.21: обычный шрифт JUCE + ЧЁРНЫЙ фон (свой V4)
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
                for(int m=0;m<monomachine::dspSectionModeCount(index);++m){
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
            title+="   ["+juce::String(monomachine::dspSectionModeLabel(index,current))+"]";
            menu.addSubMenu(title,sub);
        };
        if(section>=0&&section<monomachine::DspSectionCount)addSection(section);
        else for(int i=0;i<monomachine::DspSectionCount;++i)addSection(i);
        if(section<0||section==monomachine::DspChorus){
            juce::PopupMenu chorusSafe;
            chorusSafe.addItem(-1,"SAFE IS NOT THE NATIVE/ORIGINAL PATH",false,false);
            chorusSafe.addItem(9001,"CHORUS SAFE: BYPASS CORE WHEN ITS MIX = 0",true,processor.chorusSafe.load());
            chorusSafe.addSeparator();
            chorusSafe.addItem(-1,"OFF (default): native 24-bit core keeps running and keeps its tail.",false,false);
            chorusSafe.addItem(-1,"ON: reset/bypass only at machine MIX=0; intended for checking CPU/tails.",false,false);
            menu.addSubMenu("CHORUS SAFE   [NOT ORIGINAL]",chorusSafe);
        }
        if(section<0||section==monomachine::DspRouting){
            juce::PopupMenu route;
            route.addItem(-1,"ONE CHAIN ONLY (VERIFIED), THE SWITCH WAS REMOVED IN 1.6.5",false,false);
            for(const auto& line:juce::StringArray::fromLines(juce::String(monomachine::dspRouteDescription())))
                if(line.isNotEmpty())route.addItem(-1,line,false,false);
            menu.addSubMenu("CHAIN / ROUTE   [fixed]",route);
        }
        // 1.6.20: канарейка живет ТОЛЬКО в полном меню (кнопка DSP MODE = дебаг-меню);
        // в меню отдельных разделов, открывающихся ПКМ на ручках, строки больше нет.
        if(section<0){
            menu.addSeparator();
            menu.addItem(-1,juce::String("ENCODING TEST: ")+juce::String("\xd0\xbf\xd1\x80\xd0\xbe\xd0\xb2\xd0\xb5\xd1\x80\xd0\xba\xd0\xb0")+juce::String(" (= 'proverka', broken on purpose)"),false,false);
        }
        juce::Component::SafePointer<Surface> safe(this);
        juce::PopupMenu::Options options=juce::PopupMenu::Options().withMinimumWidth(360).withTargetComponent(this); // 1.6.16: ширина по контенту
        if(screen.x>=0&&screen.y>=0)options=options.withTargetScreenArea(juce::Rectangle<int>(screen.x,screen.y,1,1)); // попап у курсора (SRR)
        else options=options.withTargetComponent(&dspModeButton);
        menu.showMenuAsync(options,[safe](int result){
            if(!safe||result<=0)return;
            if(result==9001){safe->processor.chorusSafe.store(!safe->processor.chorusSafe.load());safe->updateDspModeLabel();return;}
            const int index=(result-1)/100,mode=(result-1)%100;
            if(index<0||index>=monomachine::DspSectionCount)return;
            if(mode<0||mode>=monomachine::dspSectionModeCount(index))return;
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
            tip+=" ["+juce::String(monomachine::dspSectionModeLabel(i,mode))+"]";
            tip+= (mode==monomachine::dspModeMnm) ? " (primary)" : " (reserve)";
            tip+=" - "+juce::String(monomachine::dspModeProvenanceForMode(i,mode))+"\n";
        }
        tip+="\nCHORUS SAFE ["+juce::String(processor.chorusSafe.load()?"ON — NOT ORIGINAL":"OFF — native core path")+"]: when ON, it bypasses/reset the chorus core only at that machine's MIX=0.\n";
        tip+=juce::String(monomachine::dspRouteDescription());
        dspModeButton.setTooltip(tip);
    }
    // 1.6.6/1.6.8: RMB on DTIM -- how hard repeats repitch while sweeping delay
    // time. Now a SLIDER (OFF/FAST/MED/SLOW = 0..3) in a call-out box at the
    // cursor, not a list. The name showRepitchMenu is kept for the verify script.
    // 1.6.12: панель замков PAGE/DEST LFO у курсора.
    // 1.6.14: после запуска callout возвращаем фокус хосту -- QWERTY-ноты играют,
    // пока окно открыто (фокус ему больше не нужен: там только мышь).
    // 1.6.42: доп-настройки (REPITCH/DSND/PORTA/GUI) -- панель ВНУТРИ окна плагина у
    // крутилки, с белой обводкой. НЕ закрывается кликом вне; закрывается ЛЮБОЙ ПКМ в
    // окне плагина (кроме самой панели) либо повторным RMB по тому же контролу
    // (тумблер). Не висит поверх других приложений при alt-tab.
    // MODE L/H have 21 stable choices in 1.9.9. A flat popup would be taller
    // than the lower filter row has room for, so expose compact family groups.
    // Each family retains the physical ordering: lower = HP, BP, LP; upper =
    // LP, BP, HP. The item index remains the serialized parameter value.
    void showHybridModeMenu(DragChoice& box,bool lower){
        smallMenuLaf().minWidth=box.getWidth();
        juce::PopupMenu root;root.setLookAndFeel(&smallMenuLaf());
        const int current=box.getSelectedItemIndex();
        const auto add=[&](juce::PopupMenu& menu,int index,const char* label){
            menu.addItem(index+1,label,index>=0&&index<box.getNumItems(),index==current);
        };
        add(root,0,"NATIVE");root.addSeparator();

        juce::PopupMenu k35,moog,r303,rms20,rmoog;
        if(lower){
            add(k35,1,"K35 HP"); add(k35,6,"K35 LP");
            add(moog,2,"MOOG HP24"); add(moog,3,"MOOG HP12"); add(moog,4,"MOOG BP24"); add(moog,5,"MOOG BP12"); add(moog,7,"MOOG LP24"); add(moog,8,"MOOG LP12");
            add(r303,9,"R 303 HP"); add(r303,13,"R 303 BP"); add(r303,17,"R 303 LP");
            add(rms20,10,"R MS20 HP"); add(rms20,14,"R MS20 BP"); add(rms20,18,"R MS20 LP");
            add(rmoog,11,"R MOOG HP24"); add(rmoog,12,"R MOOG HP12"); add(rmoog,15,"R MOOG BP24"); add(rmoog,16,"R MOOG BP12"); add(rmoog,19,"R MOOG LP24"); add(rmoog,20,"R MOOG LP12");
        }else{
            add(k35,1,"K35 LP"); add(k35,6,"K35 HP");
            add(moog,2,"MOOG LP24"); add(moog,3,"MOOG LP12"); add(moog,4,"MOOG BP24"); add(moog,5,"MOOG BP12"); add(moog,7,"MOOG HP24"); add(moog,8,"MOOG HP12");
            add(r303,9,"R 303 LP"); add(r303,13,"R 303 BP"); add(r303,17,"R 303 HP");
            add(rms20,10,"R MS20 LP"); add(rms20,14,"R MS20 BP"); add(rms20,18,"R MS20 HP");
            add(rmoog,11,"R MOOG LP24"); add(rmoog,12,"R MOOG LP12"); add(rmoog,15,"R MOOG BP24"); add(rmoog,16,"R MOOG BP12"); add(rmoog,19,"R MOOG HP24"); add(rmoog,20,"R MOOG HP12");
        }
        root.addSubMenu("K35",k35);root.addSubMenu("MOOG",moog);
        root.addSeparator();root.addSubMenu("R 303",r303);root.addSubMenu("R MS20",rms20);root.addSubMenu("R MOOG",rmoog);
        const juce::Component::SafePointer<DragChoice> safe(&box);
        // Do not force a below-control rectangle: the PopupMenu chooses the
        // visible side of the screen, and the top-level menu is only six rows.
        root.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(&box),
            [safe](int result){if(safe!=nullptr&&result>0)safe->setSelectedItemIndex(result-1,juce::sendNotificationSync);});
    }

    void showCallout(std::unique_ptr<juce::Component> panel,juce::Point<int> screen,const juce::String& kind){
        if(extraPanel!=nullptr){ const bool same=(extraKind==kind); closeExtraPanel(); if(same)return; }
        auto* raw=panel.release();
        raw->setComponentID(kind);
        addAndMakeVisible(raw);
        const auto local=screen-getScreenPosition();
        raw->setTopLeftPosition(juce::jlimit(2,juce::jmax(2,getWidth()-raw->getWidth()-2),local.x),
                                juce::jlimit(2,juce::jmax(2,getHeight()-raw->getHeight()-2),local.y+12));
        raw->setVisible(true); raw->toFront(true); extraPanel.reset(raw); extraKind=kind; // 1.6.43: гарантия показа
        juce::Component::unfocusAllComponents();
    }
    void closeExtraPanel(){ if(extraPanel!=nullptr){ removeChildComponent(extraPanel.get()); extraPanel.reset(); } extraKind={}; }
    void showLfoLocks(int lfo,bool dest,juce::Point<int> screen){const juce::Point<int> local=screen-getScreenPosition();showDocked(std::make_unique<LfoLockPanel>(processor,lfo,dest),juce::Rectangle<int>(local.x-16,local.y,32,0),true);} // 1.6.30: док ниже ячейки -- единый стандарт списков
    void showRepitchMenu(juce::Point<int> screen){showCallout(std::make_unique<RepitchSliderPanel>(processor),screen,"repitch");} // 1.6.41: тумблер
    void showDsndMenu(juce::Point<int> screen){showCallout(std::make_unique<DsndPanel>(processor),screen,"dsnd");} // 1.6.41: тумблер
    void showDelayFeedbackQ(juce::Point<int> screen){showCallout(std::make_unique<DelayFeedbackFilterPanel>(processor,p2View),screen,"dly-feedback-q");}
    void showPortaMenu(juce::Point<int> screen){showCallout(std::make_unique<PortaPanel>(processor),screen,"porta");} // 1.6.14/41: тумблер
    void showFilterExtras(juce::Point<int> screen){showCallout(std::make_unique<FilterExtraPanel>(processor,p2View),screen,"filt-extra");}
    // 1.6.6: MODE selectors on page headers (like the envelope REFERENCE FIT):
    // SYNT header carries the section mode, or the machine's own algorithm
    // choice (SWAVE-SAW) when that machine is selected; FILT and EFFX headers
    // expose their sections directly.
    void rebuildModeControls(){
        auto wire=[this](juce::ComboBox& c,const juce::String& paramId,std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>& link,const juce::String& choices){
            link.reset();c.clear(juce::dontSendNotification);
            const auto names=juce::StringArray::fromTokens(choices,"|",{});
            for(int i=0;i<names.size();++i)c.addItem(names[i],i+1);
            link=std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(processor.parameters,paramId,c);};
        if(p2View){
            // P2 always uses its unchanged native FX-slot selection.
            syntModeLink.reset();syntModeCombo.clear(juce::dontSendNotification);syntModeCombo.setVisible(false);
            wire(filtModeCombo,"p2_mode_filt",filtModeLink,monomachine::dspSectionParameterChoices(monomachine::DspFilter));
            effxDistLink.reset();effxDistCombo.clear(juce::dontSendNotification); // MODE S owns P2 DIST
            wire(effxDlyCombo,"p2_mode_dly",effxDlyLink,monomachine::dspSectionParameterChoices(monomachine::DspDelay));
        }else{
        syntModeCombo.setVisible(true);
        if(processor.machineIndex()>=0&&nova::machines()[static_cast<size_t>(processor.machineIndex())].id==4)
            wire(syntModeCombo,"m4_3",syntModeLink,"ALG A|ALG B");
        else wire(syntModeCombo,processor.dspModeParamIdFor(monomachine::DspSynt),syntModeLink,monomachine::dspSectionParameterChoices(monomachine::DspSynt)); // 1.6.8: параметр выбранной машины
        wire(filtModeCombo,"mode_filt",filtModeLink,monomachine::dspSectionParameterChoices(monomachine::DspFilter));
        effxDistLink.reset();effxDistCombo.clear(juce::dontSendNotification); // MODE S owns P1 DIST
        wire(effxDlyCombo,"mode_dly",effxDlyLink,monomachine::dspSectionParameterChoices(monomachine::DspDelay));
        } // 1.8.0
        // MODE L/H select the physical BASE/WDTH sides in their respective
        // directions. MODE S is the sole selector for the existing DIST block.
        const auto hybridPrefix=p2View?juce::String("hybrid_p2_"):juce::String("hybrid_p1_");
        wire(hybridModeLCombo,hybridPrefix+"mode_l",hybridModeLLink,nova::hybridModeLChoices());
        wire(hybridModeHCombo,hybridPrefix+"mode_h",hybridModeHLink,nova::hybridModeHChoices());
        wire(hybridModeSCombo,hybridPrefix+"mode_s",hybridModeSLink,"MNM|OLD|MNM FIX|FOLD|ZERO|CLAMP");
        hybridModeLCombo.popupHandler=[this](DragChoice& box){showHybridModeMenu(box,true);};
        hybridModeHCombo.popupHandler=[this](DragChoice& box){showHybridModeMenu(box,false);};
        filtModeCombo.setVisible(false);effxDistCombo.setVisible(false);hybridModeLCombo.setVisible(true);hybridModeHCombo.setVisible(true);hybridModeSCombo.setVisible(true);
        ampModeLink.reset();ampModeCombo.clear(juce::dontSendNotification);ampModeCombo.addItem("old",1);ampModeCombo.addItem("mnm",2);ampModeCombo.addItem("vital",3);
        ampModeLink=std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(processor.parameters,p2View?"p2_amp_mode":"amp_mode",ampModeCombo);
        ampModeCombo.setTooltip("AMP envelope engine: old = legacy, mnm = current kernel, vital = curve variant. Default remains mnm.");
        syntModeCombo.setTooltip(p2View?"P2 uses the native FX-slot path.":"SYNT mode of this machine: mnm = native/default; old = retained reserve where applicable. Each machine remembers its mode.");
        filtModeCombo.setTooltip("FILT mode: old is the stable default for new instances; mnm remains available for A/B. It remains in DSP MODE while the three controls above FILT address the physical filter sides and DIST.");
        hybridModeLCombo.setTooltip("MODE L / BASE lower side: true high-pass / low-cut choices appear first in each compact family, then band-pass, then low-pass. R 303, R MS20 and R MOOG are appended IDs; existing choices stay unchanged.");
        hybridModeHCombo.setTooltip("MODE H / WDTH upper side: true low-pass / high-cut choices appear first in each compact family, then band-pass, then high-pass. The grouped popup stays reachable at the lower edge of the plug-in window.");
        hybridModeSCombo.setTooltip("MODE S: the sole selector for this existing post-filter DIST block. MNM is the retained current path; OLD is untouched; MNM FIX is an experimental +3 dB 3.5 kHz notch-compensation A/B candidate; FOLD, ZERO and CLAMP retain the MNM DIST knob progression and controlled output.");
        effxDlyCombo.setTooltip("DLY mode: old is the stable default; mnm remains native A/B; NEW is an opt-in Track Delay feedback-filter candidate. DBAS/DWID are feedback edges; right-click either knob for Q.");
    }
    void showSettings(bool matrix){if(!matrix&&arpWindow){arpWindow.reset();setViewportMode(false);return;} setViewportMode(matrix);} // 1.7.7: ARP при детаче = ВЕРНУТЬ страницу на место в синх
    void showGuiOptions(){showCallout(std::make_unique<GuiSpeedPanel>(),{menuButton.getScreenBounds().getX(),menuButton.getScreenBounds().getBottom()+4},"gui");} // 1.6.19/41: тоже плавающее
    void showMenu(){juce::PopupMenu menu;menu.setLookAndFeel(&smallMenuLaf()); // 1.6.14/33: чёрный компактный скин, список под кнопкой
        menu.addItem(5,"RESET ALL PARAMETERS");menu.addItem(1,"ARP");menu.addItem(2,"MOD MATRIX");menu.addItem(8,"MSEG / CURVE EDITOR");
        if(processor.isSynthVersion){menu.addItem(7,"BBOX SAMPLES");menu.addItem(3,"BBOX RESTORE KIT");}
        menu.addSeparator();menu.addItem(4,"PANIC / CLEAR TAILS");menu.addItem(9,"GUI");menu.addItem(10,"LAYOUT EDIT");menu.addItem(6,"ABOUT"); // 1.6.19/38
        menu.addItem(12,"FX SLOTS");
        juce::Component::SafePointer<Surface> safe(this);menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(&menuButton),[safe](int result){if(!safe)return;
            if(result==1||result==2)safe->setViewportMode(result==2);if(result==8)safe->showMseg();if(result==9)safe->showGuiOptions();if(result==10)safe->toggleLayoutEdit(); // 1.6.38
            if(result==3)safe->processor.resetSamples();if(result==4)safe->processor.requestPanic();
            if(result==5){safe->pushUndoPoint();safe->processor.plockResetAll();safe->processor.fxSlotsClearAll();safe->processor.classicRoutesResetAll();safe->processor.feedbackReset();safe->processor.requestPanic();safe->processor.resetAllMsegs();for(auto* parameter:safe->processor.getParameters()){parameter->beginChangeGesture();parameter->setValueNotifyingHost(parameter->getDefaultValue());parameter->endChangeGesture();}safe->bind();} // 1.7.5: сброс всего -- и P-LOCK тоже (с точкой отката); 1.6.32: ресет -- и MSEG-страницы
            if(result==6){auto& lf=juce::LookAndFeel::getDefaultLookAndFeel();lf.setColour(juce::AlertWindow::backgroundColourId,juce::Colours::black);lf.setColour(juce::AlertWindow::textColourId,juce::Colours::white);lf.setColour(juce::AlertWindow::outlineColourId,juce::Colours::white);} // 1.6.19: About на чёрном
            if(result==6)juce::AlertWindow::showMessageBoxAsync(juce::MessageBoxIconType::NoIcon,"MONOMACHINE NOVA","mnm mod by restrange aka kraduvremya"); // 1.6.29: только подпись -- история в FIXES_APPLIED, справка в MANUAL.md
            if(result==7)safe->showSamples();if(result==12)safe->showFxSlots(); // CHORUS SAFE belongs in DSP MODE, never in the general MENU
        });
    }
    void chooseSample(int slot){chooser=std::make_unique<juce::FileChooser>("Load MOD slot "+juce::String(slot+1),juce::File(),"*.wav;*.aif;*.aiff");
        juce::Component::SafePointer<Surface> safe(this);chooser->launchAsync(juce::FileBrowserComponent::openMode|juce::FileBrowserComponent::canSelectFiles,[safe,slot](const juce::FileChooser& fc){if(!safe)return;auto file=fc.getResult();if(!file.existsAsFile())return;
            auto error=safe->processor.loadSample(slot,file);if(error.isNotEmpty())juce::AlertWindow::showMessageBoxAsync(juce::MessageBoxIconType::WarningIcon,"Sample load",error);
        });
    }
    MonomachineNovaAudioProcessor& processor;PixelTheme theme;
    std::array<std::array<std::unique_ptr<Cell>,8>,6> cells;
    PixelButton machineButton,menuButton,tempoButton,arpButton,matrixButton,fxButton{"FX"},p2Button{"P1"},syntCatButton{"SYNT"},lfoCatButton{"LFO"},filtCatButton{"FILT"},effxCatButton{"EFFX"},msegCatButton{"MSEG"};DragValueBox gateOuter;ModSourceButton lfo1Button,lfo2Button,lfo3Button,lfo4Button{"4"},lfo5Button{"5"},lfo6Button{"6"},lfoJack{"J"};CableLayer cableLayer;int modMsegAim=0;bool p2View=false;PixelButton closeButton,gateButton;juce::Slider level;PixelButton dspModeButton{"DSP MODE"};UndoRedoButton undoBtn{true},redoBtn{false}; // 1.6.32 // 1.8.0: p2View -- P1|P2 вид лицевой
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> levelLink;
    SquareToggle arpEnabled;juce::ToggleButton trigToggle,tempoSync;juce::TextEditor tempoEntry;std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> tempoSyncLink; // 1.6.29: ARP-галка -- закрытый квадрат на всю высоту
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> arpLink,trigLink;
    PixelButton ampButton{"AMP"};DragChoice syntModeCombo,filtModeCombo,hybridModeLCombo,hybridModeHCombo,hybridModeSCombo,effxDistCombo,effxDlyCombo,ampModeCombo;std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> ampModeLink; // 1.6.21: комбо огибающей вернулось
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> syntModeLink,filtModeLink,hybridModeLLink,hybridModeHLink,hybridModeSLink,effxDistLink,effxDlyLink; // 1.6.6
    std::unique_ptr<juce::Component> envelopePage;
    std::unique_ptr<SamplePage> samplePage;std::unique_ptr<MsegPage> msegPage;
    std::unique_ptr<ArpPage> settings;std::unique_ptr<MatrixPage> matrixPage;std::unique_ptr<FxSlotsPage> fxSlotsPage;std::unique_ptr<juce::DialogWindow> arpWindow; // 1.7.5: отдельное окно ARP
    FramedViewport viewport;std::unique_ptr<juce::FileChooser> chooser;
    std::unique_ptr<TextTag> tagTitle,tagLogo,tagLev,tagBpm; // 1.6.42: нарисованные тексты в LAYOUT EDIT
    juce::LookAndFeel_V4 dspMenuLf; // 1.6.21: для меню DSP MODE -- обычный шрифт, чёрный фон
    juce::TooltipWindow tooltips{this,800};int lfoPageSel=0;int modDragSrc=-1,modDragSourcePage=-1;bool modDragMatrix=false,modDragOverP2Icon=false;Cell* modHover=nullptr;int boundMachine=-1;bool pickingTarget=false;int pickRoute=-1,pickHoverTarget=-1;int plockPickSlot=-1; // 1.7.1: прицел P-LOCK
    float machineAccum=0,arpAccum=0;juce::uint32 lastMachineCycleMs=0;std::unique_ptr<PopupDock> dock;std::unique_ptr<juce::Component> dockPanel; // 1.6.13: дробные аккумуляторы скорости драга
    juce::Point<int> modDragScreen;int lfoDragPageCandidate=-1;juce::uint32 lfoDragPageSince=0; // gated hover navigation between LFO page buttons
    float aimBackupPage=0,aimBackupDest=0;bool aimBackupValid=false; // 1.6.24: откат прицела при промахе
    std::unique_ptr<LayoutOverlay> layoutOverlay;juce::Component::SafePointer<juce::Component> layRoot; // 1.6.38: LAYOUT EDIT
    std::unique_ptr<juce::Component> extraPanel;juce::String extraKind; // 1.6.42: доп-настройки внутри окна плагина
    struct RmbWatcher final : public juce::MouseListener { std::function<void(const juce::MouseEvent&)> onDown; void mouseDown(const juce::MouseEvent& e) override { if(onDown)onDown(e); } }; // 1.6.42
    RmbWatcher rmbWatcher;
    int msegPickPage=-1,msegPickPageShown=0,msegPickRow=0,modEnvPickPage=-1; // 1.6.34: прицел из MSEG-строки routes; MOD ENV uses 0..3
};
MonomachineNovaAudioProcessorEditor::MonomachineNovaAudioProcessorEditor(MonomachineNovaAudioProcessor& p):AudioProcessorEditor(&p),processor(p){
    uiLayout(); // 1.6.38: layout.json грузится один раз, до первого resized
    surface=std::make_unique<Surface>(p);addAndMakeVisible(*surface);
    // Default 1:1 pixel size; the window may be scaled wider (keeps the reference 1260x584 grid).
    setResizable(true,true);setResizeLimits(1260,584,3780,1752);getConstrainer()->setFixedAspectRatio(1260.0/584.0);setSize(1260,584);startTimerHz(20);
}
MonomachineNovaAudioProcessorEditor::~MonomachineNovaAudioProcessorEditor(){stopTimer();}
void MonomachineNovaAudioProcessorEditor::startLayoutEdit(){if(surface!=nullptr)surface->toggleLayoutEdit();} // 1.6.40: старт LAYOUT EDIT для отдельной проги-редактора
bool MonomachineNovaAudioProcessorEditor::keyPressed(const juce::KeyPress& k){ // 1.6.41: F1 = редактор расклада, F2 = сохранить расклад
    if(k.isKeyCode(juce::KeyPress::F1Key)){ if(surface!=nullptr)surface->toggleLayoutEdit(); return true; }
    if(k.isKeyCode(juce::KeyPress::F2Key)){ uiLayout().save(); return true; }
    return false; }
void MonomachineNovaAudioProcessorEditor::paint(juce::Graphics& g){g.fillAll(juce::Colours::black);}
void MonomachineNovaAudioProcessorEditor::resized(){if(surface){surface->setTopLeftPosition(0,0);surface->setTransform(juce::AffineTransform::scale(static_cast<float>(getWidth())/1260.0f,static_cast<float>(getHeight())/584.0f));}}
void MonomachineNovaAudioProcessorEditor::timerCallback(){surface->update();}
