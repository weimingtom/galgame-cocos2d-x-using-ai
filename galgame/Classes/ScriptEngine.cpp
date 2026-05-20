#include "ScriptEngine.h"
#include "cocos2d.h"
#include <fstream>
#include <sstream>

USING_NS_CC;

ScriptEngine::ScriptEngine() {}
ScriptEngine::~ScriptEngine() {}

static std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

static std::string unquote(const std::string& s) {
    if (s.size() >= 2 && s.front() == '"' && s.back() == '"')
        return s.substr(1, s.size() - 2);
    return s;
}

void ScriptEngine::parseLine(const std::string& rawLine) {
    std::string line = trim(rawLine);
    if (line.empty() || line[0] == '#') return;

    // Split into tag and body
    size_t colon = line.find(':');
    if (colon == std::string::npos) return;

    std::string tag = trim(line.substr(0, colon));
    std::string body = trim(line.substr(colon + 1));

    ScriptCommand cmd;

    if (tag == "BG") {
        cmd.type = ScriptCommand::BG;
        cmd.param1 = body;
    }
    else if (tag == "CHAR") {
        cmd.type = ScriptCommand::CHAR;
        // id file position
        std::istringstream iss(body);
        iss >> cmd.param1 >> cmd.param2 >> cmd.param3;
    }
    else if (tag == "CHAR_HIDE") {
        cmd.type = ScriptCommand::CHAR_HIDE;
        cmd.param1 = body;
    }
    else if (tag == "TEXT") {
        cmd.type = ScriptCommand::TEXT;
        // speaker "text"
        size_t space = body.find(' ');
        if (space != std::string::npos) {
            cmd.param1 = trim(body.substr(0, space));          // speaker
            cmd.param2 = unquote(trim(body.substr(space + 1))); // text
        }
    }
    else if (tag == "CHOICE") {
        cmd.type = ScriptCommand::CHOICE;
        // "text" -> label
        size_t arrow = body.find("->");
        if (arrow != std::string::npos) {
            cmd.param1 = unquote(trim(body.substr(0, arrow)));
            cmd.param2 = trim(body.substr(arrow + 2));
        }
    }
    else if (tag == "CHOICES") {
        cmd.type = ScriptCommand::CHOICES;
    }
    else if (tag == "LABEL") {
        cmd.type = ScriptCommand::LABEL;
        cmd.param1 = body;
        _labels[body] = _commands.size(); // point to next command
    }
    else if (tag == "JUMP") {
        cmd.type = ScriptCommand::JUMP;
        cmd.param1 = body;
    }
    else if (tag == "WAIT") {
        cmd.type = ScriptCommand::WAIT;
        cmd.param1 = body;
    }
    else if (tag == "END") {
        cmd.type = ScriptCommand::END;
    }

    _commands.push_back(cmd);
}

bool ScriptEngine::loadScript(const std::string& filePath) {
    _commands.clear();
    _labels.clear();
    _currentIndex = 0;
    _finished = false;

    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filePath);
    std::ifstream file(fullPath);
    if (!file.is_open()) {
        CCLOG("ScriptEngine: Cannot open script file: %s", filePath.c_str());
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        parseLine(line);
    }
    file.close();

    // Resolve label indices (must point to line after the LABEL command)
    // We already stored correct indices during parsing since LABEL records
    // the index where the next command will be inserted.
    // But we need to store them properly. Let's redo:
    _labels.clear();
    for (size_t i = 0; i < _commands.size(); ++i) {
        if (_commands[i].type == ScriptCommand::LABEL) {
            _labels[_commands[i].param1] = i + 1;
        }
    }

    CCLOG("ScriptEngine: Loaded %zu commands", _commands.size());
    return true;
}

void ScriptEngine::start(CommandCallback callback) {
    _currentIndex = 0;
    _finished = false;
    executeCurrent(callback);
}

void ScriptEngine::nextCommand() {
    _currentIndex++;
}

void ScriptEngine::jumpToLabel(const std::string& label) {
    auto it = _labels.find(label);
    if (it != _labels.end()) {
        _currentIndex = it->second;
        CCLOG("ScriptEngine: Jumping to label '%s' at index %zu", label.c_str(), _currentIndex);
    } else {
        CCLOG("ScriptEngine: Label '%s' not found!", label.c_str());
    }
}

void ScriptEngine::skipTypewriter() {
    // Handled externally by DialogueBox
}

void ScriptEngine::executeCurrent(CommandCallback& callback) {
    if (_currentIndex >= _commands.size()) {
        _finished = true;
        return;
    }

    const auto& cmd = _commands[_currentIndex];

    // Skip label commands silently
    if (cmd.type == ScriptCommand::LABEL) {
        _currentIndex++;
        executeCurrent(callback);
        return;
    }

    callback(cmd);

    // Auto-advance for commands that don't need user interaction
    if (cmd.type != ScriptCommand::TEXT &&
        cmd.type != ScriptCommand::CHOICES &&
        cmd.type != ScriptCommand::END) {
        // Will be advanced externally for TEXT and CHOICES
    }
}
