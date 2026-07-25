#pragma once

#include <string>
#include <vector>
#include <map>

namespace gal {

enum class CommandType {
    Background,
    ShowCharacter,
    HideCharacter,
    Say,
    Narrate,
    Choice,
    Jump,
    Wait,
    Clear,
    End
};

struct ChoiceOption {
    std::string text;
    std::string nextLabel;
};

struct ScriptCommand {
    CommandType type = CommandType::Say;
    std::string speaker;
    std::string text;
    std::string name;       // bg / character id
    std::string emotion;    // character emotion / variant
    std::string position;   // left / center / right
    std::string nextLabel;  // jump target
    float duration = 0.f;
    std::vector<ChoiceOption> options;
};

struct ScriptLabel {
    std::string name;
    std::vector<ScriptCommand> commands;
};

struct ScriptData {
    std::string title;
    std::string startLabel;
    std::map<std::string, ScriptLabel> labels;
};

} // namespace gal
