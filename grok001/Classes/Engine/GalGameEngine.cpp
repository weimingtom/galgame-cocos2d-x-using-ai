#include "GalGameEngine.h"
#include "ScriptParser.h"

using namespace gal;

bool GalGameEngine::loadScript(const std::string& relativePath, std::string& error)
{
    ScriptData data;
    if (!ScriptParser::parseFile(relativePath, data, error)) {
        return false;
    }
    return loadScriptData(std::move(data));
}

bool GalGameEngine::loadScriptData(ScriptData data)
{
    _script = std::move(data);
    _currentLabel.clear();
    _index = 0;
    _waitingChoice = false;
    _waitingTimer = false;
    _finished = false;
    _busy = false;
    return true;
}

void GalGameEngine::setCallbacks(OnSay onSay,
                                 OnBackground onBg,
                                 OnShowCharacter onShow,
                                 OnHideCharacter onHide,
                                 OnChoice onChoice,
                                 OnClear onClear,
                                 OnEnd onEnd,
                                 OnWait onWait)
{
    _onSay = std::move(onSay);
    _onBg = std::move(onBg);
    _onShow = std::move(onShow);
    _onHide = std::move(onHide);
    _onChoice = std::move(onChoice);
    _onClear = std::move(onClear);
    _onEnd = std::move(onEnd);
    _onWait = std::move(onWait);
}

void GalGameEngine::start()
{
    _finished = false;
    _waitingChoice = false;
    _waitingTimer = false;
    _busy = false;
    if (!jumpTo(_script.startLabel)) {
        _finished = true;
        if (_onEnd) _onEnd();
        return;
    }
    runCurrent();
}

bool GalGameEngine::jumpTo(const std::string& label)
{
    auto it = _script.labels.find(label);
    if (it == _script.labels.end()) {
        return false;
    }
    _currentLabel = label;
    _index = 0;
    return true;
}

void GalGameEngine::runCurrent()
{
    if (_finished || _waitingChoice || _waitingTimer) return;

    auto it = _script.labels.find(_currentLabel);
    if (it == _script.labels.end()) {
        _finished = true;
        if (_onEnd) _onEnd();
        return;
    }

    const auto& cmds = it->second.commands;
    while (_index < cmds.size()) {
        const ScriptCommand& cmd = cmds[_index];
        ++_index;
        execute(cmd);
        if (_busy || _waitingChoice || _waitingTimer || _finished) {
            return;
        }
    }

    // Label exhausted without jump/end
    _finished = true;
    if (_onEnd) _onEnd();
}

void GalGameEngine::execute(const ScriptCommand& cmd)
{
    switch (cmd.type) {
    case CommandType::Background:
        if (_onBg) _onBg(cmd.name);
        break;
    case CommandType::ShowCharacter:
        if (_onShow) _onShow(cmd.name, cmd.emotion, cmd.position.empty() ? "center" : cmd.position);
        break;
    case CommandType::HideCharacter:
        if (_onHide) _onHide(cmd.name);
        break;
    case CommandType::Say:
        _busy = true;
        if (_onSay) _onSay(cmd.speaker, cmd.text, false);
        break;
    case CommandType::Narrate:
        _busy = true;
        if (_onSay) _onSay("", cmd.text, true);
        break;
    case CommandType::Choice:
        _waitingChoice = true;
        if (_onChoice) _onChoice(cmd.options);
        break;
    case CommandType::Jump:
        if (!jumpTo(cmd.nextLabel)) {
            _finished = true;
            if (_onEnd) _onEnd();
        }
        break;
    case CommandType::Wait:
        _waitingTimer = true;
        if (_onWait) _onWait(cmd.duration > 0.f ? cmd.duration : 1.f);
        break;
    case CommandType::Clear:
        if (_onClear) _onClear();
        break;
    case CommandType::End:
        _finished = true;
        if (_onEnd) _onEnd();
        break;
    }
}

void GalGameEngine::advance()
{
    if (_finished || _waitingChoice || _waitingTimer) return;
    if (!_busy) {
        runCurrent();
        return;
    }
    _busy = false;
    runCurrent();
}

void GalGameEngine::selectChoice(int index)
{
    if (!_waitingChoice) return;
    auto it = _script.labels.find(_currentLabel);
    if (it == _script.labels.end() || _index == 0) return;

    // Last executed command is at _index-1 (choice)
    const ScriptCommand& choiceCmd = it->second.commands[_index - 1];
    if (index < 0 || index >= static_cast<int>(choiceCmd.options.size())) return;

    _waitingChoice = false;
    const std::string& next = choiceCmd.options[index].nextLabel;
    if (!jumpTo(next)) {
        _finished = true;
        if (_onEnd) _onEnd();
        return;
    }
    runCurrent();
}

void GalGameEngine::onWaitFinished()
{
    if (!_waitingTimer) return;
    _waitingTimer = false;
    runCurrent();
}
