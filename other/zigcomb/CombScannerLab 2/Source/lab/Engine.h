#pragma once

// Общий JUCE-независимый интерфейс для всех вариантов Comb Scanner.
// Каждый движок из other/zigcomb подключается через маленький адаптер (см. EngineRegistry.cpp),
// поэтому один и тот же код работает и в плагине, и в оффлайн-рендерере tools/render_engines.cpp.

#include <cstddef>

namespace lab
{
struct ParamInfo
{
    const char* key;    // короткий ключ, из него делается ID параметра (s0_gain и т.п.)
    const char* name;   // подпись на ручке
    float min;
    float max;
    float def;
    float step;
    const char* unit;   // "" / "ms" / "Hz" / "dB"
    int group;          // 0 = основные, 1 = internal (ниже основных)
    const char* desc;   // English tooltip; может быть nullptr
};

struct EngineInfo
{
    const char* id;      // короткий id страницы
    const char* title;   // название вкладки
    const char* source;  // откуда взят код (папка в репозитории)
    const char* note;    // чем этот вариант отличается (RU)
};

class Engine
{
public:
    virtual ~Engine() = default;

    virtual EngineInfo info() const = 0;

    // 0 == у движка нет выбора модели
    virtual int numModels() const = 0;
    virtual const char* modelName(int modelIndex) const = 0;

    virtual const ParamInfo* params() const = 0;
    virtual int numParams() const = 0;

    virtual void prepare (double sampleRate, int maximumBlockSize) = 0;
    virtual void reset() = 0;

    // index - позиция в params(), value - уже готовое значение (min..max)
    virtual void setParam (int index, float value) = 0;
    virtual void setModel (int modelIndex) = 0;

    virtual void process (float* const* channels, int numChannels, int numSamples) = 0;
};

// Реестр страниц
int numEngines();
Engine* createEngine (int index);   // владелец - вызывающий (std::unique_ptr)
}
