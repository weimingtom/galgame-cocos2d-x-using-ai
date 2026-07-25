#pragma once

#include "ScriptCommand.h"
#include <functional>
#include <string>

namespace gal {

class GalGameEngine {
public:
    using OnSay = std::function<void(const std::string& speaker, const std::string& text, bool isNarration)>;
    using OnBackground = std::function<void(const std::string& name)>;
    using OnShowCharacter = std::function<void(const std::string& name, const std::string& emotion, const std::string& position)>;
    using OnHideCharacter = std::function<void(const std::string& name)>;
    using OnChoice = std::function<void(const std::vector<ChoiceOption>& options)>;
    using OnClear = std::function<void()>;
    using OnEnd = std::function<void()>;
    using OnWait = std::function<void(float duration)>;

    bool loadScript(const std::string& relativePath, std::string& error);
    bool loadScriptData(ScriptData data);

    void setCallbacks(OnSay onSay,
                      OnBackground onBg,
                      OnShowCharacter onShow,
                      OnHideCharacter onHide,
                      OnChoice onChoice,
                      OnClear onClear,
                      OnEnd onEnd,
                      OnWait onWait);

    void start();
    void advance();                 // click / next line
    void selectChoice(int index);   // after choice shown
    void onWaitFinished();          // after wait timer

    bool isWaitingChoice() const { return _waitingChoice; }
    bool isWaitingTimer() const { return _waitingTimer; }
    bool isFinished() const { return _finished; }
    const std::string& title() const { return _script.title; }
    const std::string& currentLabel() const { return _currentLabel; }

private:
    void runCurrent();
    void execute(const ScriptCommand& cmd);
    bool jumpTo(const std::string& label);

    ScriptData _script;
    std::string _currentLabel;
    size_t _index = 0;
    bool _waitingChoice = false;
    bool _waitingTimer = false;
    bool _finished = false;
    bool _busy = false; // waiting for UI to acknowledge advance

    OnSay _onSay;
    OnBackground _onBg;
    OnShowCharacter _onShow;
    OnHideCharacter _onHide;
    OnChoice _onChoice;
    OnClear _onClear;
    OnEnd _onEnd;
    OnWait _onWait;
};

} // namespace gal
