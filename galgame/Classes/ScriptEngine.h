#ifndef _SCRIPT_ENGINE_H_
#define _SCRIPT_ENGINE_H_

#include "cocos2d.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

struct ScriptCommand {
    enum Type {
        BG,         // BG: filename
        CHAR,       // CHAR: id file position
        CHAR_HIDE,  // CHAR_HIDE: id
        TEXT,       // TEXT: speaker "text"
        CHOICE,     // CHOICE: "text" -> label
        CHOICES,    // CHOICES: (show accumulated choices)
        LABEL,      // LABEL: name
        JUMP,       // JUMP: label
        WAIT,       // WAIT: seconds
        END,        // END:
    };

    Type type;
    std::string param1;   // id / speaker / label / filename
    std::string param2;   // file / text / position / label
    std::string param3;   // position
};

class ScriptEngine {
public:
    using CommandCallback = std::function<void(const ScriptCommand&)>;

    ScriptEngine();
    ~ScriptEngine();

    bool loadScript(const std::string& filePath);
    void start(CommandCallback callback);
    void nextCommand();
    void jumpToLabel(const std::string& label);
    void skipTypewriter();

    bool isFinished() const { return _finished; }
    const ScriptCommand& currentCommand() const { return _commands[_currentIndex]; }
    size_t currentIndex() const { return _currentIndex; }

private:
    void parseLine(const std::string& line);
    void executeCurrent(CommandCallback& callback);

    std::vector<ScriptCommand> _commands;
    std::unordered_map<std::string, size_t> _labels;
    size_t _currentIndex = 0;
    bool _finished = false;
};

#endif
