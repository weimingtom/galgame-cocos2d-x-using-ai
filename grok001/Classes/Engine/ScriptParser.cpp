#include "ScriptParser.h"
#include "cocos2d.h"
#include "json/document.h"
#include "json/stringbuffer.h"

USING_NS_CC;
using namespace gal;

namespace {

CommandType parseType(const std::string& t)
{
    if (t == "bg" || t == "background") return CommandType::Background;
    if (t == "show") return CommandType::ShowCharacter;
    if (t == "hide") return CommandType::HideCharacter;
    if (t == "say") return CommandType::Say;
    if (t == "narrate" || t == "n") return CommandType::Narrate;
    if (t == "choice") return CommandType::Choice;
    if (t == "jump" || t == "goto") return CommandType::Jump;
    if (t == "wait") return CommandType::Wait;
    if (t == "clear") return CommandType::Clear;
    if (t == "end") return CommandType::End;
    return CommandType::Say;
}

bool parseCommand(const rapidjson::Value& v, ScriptCommand& cmd, std::string& error)
{
    if (!v.IsObject()) {
        error = "command must be an object";
        return false;
    }
    if (!v.HasMember("type") || !v["type"].IsString()) {
        error = "command missing string 'type'";
        return false;
    }

    cmd.type = parseType(v["type"].GetString());

    auto getStr = [&](const char* key) -> std::string {
        if (v.HasMember(key) && v[key].IsString()) return v[key].GetString();
        return {};
    };

    cmd.speaker = getStr("speaker");
    cmd.text = getStr("text");
    cmd.name = getStr("name");
    if (cmd.name.empty()) cmd.name = getStr("character");
    cmd.emotion = getStr("emotion");
    cmd.position = getStr("position");
    cmd.nextLabel = getStr("goto");
    if (cmd.nextLabel.empty()) cmd.nextLabel = getStr("next");

    if (v.HasMember("duration") && v["duration"].IsNumber()) {
        cmd.duration = static_cast<float>(v["duration"].GetDouble());
    }

    if (cmd.type == CommandType::Choice) {
        if (!v.HasMember("options") || !v["options"].IsArray()) {
            error = "choice command requires 'options' array";
            return false;
        }
        const auto& arr = v["options"];
        for (rapidjson::SizeType i = 0; i < arr.Size(); ++i) {
            const auto& opt = arr[i];
            if (!opt.IsObject()) continue;
            ChoiceOption o;
            if (opt.HasMember("text") && opt["text"].IsString()) o.text = opt["text"].GetString();
            if (opt.HasMember("goto") && opt["goto"].IsString()) o.nextLabel = opt["goto"].GetString();
            else if (opt.HasMember("next") && opt["next"].IsString()) o.nextLabel = opt["next"].GetString();
            cmd.options.push_back(o);
        }
    }

    return true;
}

} // namespace

bool ScriptParser::parseFile(const std::string& relativePath, ScriptData& out, std::string& error)
{
    const std::string full = FileUtils::getInstance()->fullPathForFilename(relativePath);
    if (full.empty() || !FileUtils::getInstance()->isFileExist(full)) {
        error = "script file not found: " + relativePath;
        return false;
    }
    const std::string text = FileUtils::getInstance()->getStringFromFile(full);
    return parseString(text, out, error);
}

bool ScriptParser::parseString(const std::string& jsonText, ScriptData& out, std::string& error)
{
    out = ScriptData{};
    rapidjson::Document doc;
    doc.Parse(jsonText.c_str());
    if (doc.HasParseError()) {
        error = "JSON parse error at offset " + std::to_string(doc.GetErrorOffset());
        return false;
    }
    if (!doc.IsObject()) {
        error = "root must be an object";
        return false;
    }

    if (doc.HasMember("title") && doc["title"].IsString()) {
        out.title = doc["title"].GetString();
    }
    if (doc.HasMember("start") && doc["start"].IsString()) {
        out.startLabel = doc["start"].GetString();
    }

    if (!doc.HasMember("labels") || !doc["labels"].IsObject()) {
        error = "missing 'labels' object";
        return false;
    }

    const auto& labels = doc["labels"];
    for (auto it = labels.MemberBegin(); it != labels.MemberEnd(); ++it) {
        ScriptLabel label;
        label.name = it->name.GetString();
        if (!it->value.IsArray()) {
            error = "label '" + label.name + "' must be an array of commands";
            return false;
        }
        const auto& arr = it->value;
        for (rapidjson::SizeType i = 0; i < arr.Size(); ++i) {
            ScriptCommand cmd;
            std::string cmdErr;
            if (!parseCommand(arr[i], cmd, cmdErr)) {
                error = "label '" + label.name + "' cmd#" + std::to_string(i) + ": " + cmdErr;
                return false;
            }
            label.commands.push_back(cmd);
        }
        out.labels[label.name] = std::move(label);
    }

    if (out.startLabel.empty() && !out.labels.empty()) {
        out.startLabel = out.labels.begin()->first;
    }
    if (out.labels.find(out.startLabel) == out.labels.end()) {
        error = "start label not found: " + out.startLabel;
        return false;
    }
    return true;
}
