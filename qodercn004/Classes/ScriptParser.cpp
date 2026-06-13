#include "ScriptParser.h"
#include "platform/CCFileUtils.h"
#include <sstream>
#include <algorithm>

USING_NS_CC;

ScriptParser* ScriptParser::_instance = nullptr;

ScriptParser* ScriptParser::getInstance()
{
    if (_instance == nullptr) {
        _instance = new(std::nothrow) ScriptParser();
    }
    return _instance;
}

void ScriptParser::destroyInstance()
{
    if (_instance) {
        delete _instance;
        _instance = nullptr;
    }
}

ScriptParser::ScriptParser()
    : _currentLineIndex(0)
{
}

ScriptParser::~ScriptParser()
{
    _scriptLines.clear();
}

bool ScriptParser::loadScript(const std::string& scriptPath)
{
    std::string content = FileUtils::getInstance()->getStringFromFile(scriptPath);
    if (content.empty()) {
        CCLOG("Failed to load script: %s", scriptPath.c_str());
        return false;
    }
    
    parseScriptContent(content);
    reset();
    
    return true;
}

void ScriptParser::parseScriptContent(const std::string& content)
{
    std::istringstream stream(content);
    std::string line;
    
    while (std::getline(stream, line)) {
        // 去除行首行尾空白字符
        line = trim(line);
        
        // 忽略空行和注释行
        if (!line.empty() && line[0] != '/' && (line.length() > 1 && !(line[0] == '/' && line[1] == '/'))) {
            _scriptLines.push_back(line);
        }
    }
}

bool ScriptParser::executeNextLine()
{
    if (_currentLineIndex >= _scriptLines.size()) {
        return false; // 没有更多脚本行
    }
    
    _currentLine = _scriptLines[_currentLineIndex];
    _currentLineIndex++;
    
    executeCommand(_currentLine);
    
    return _currentLineIndex < _scriptLines.size();
}

void ScriptParser::executeCommand(const std::string& command)
{
    std::string cmd = command;
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower); // 转为小写便于比较
    
    if (cmd.find("背景:") == 0) {
        // 解析背景命令: "背景: bg_name"
int len = std::string("背景:").length();
        std::string bgName = trim(command.substr(/*3*/len)); // 去掉"背景:"部分
CCLOG("bgName:%s", bgName.c_str());
        if (_setBackgroundCallback) {
            _setBackgroundCallback(bgName);
        }
    }
    else if (cmd.find("角色进入:") == 0) {
        // 解析角色进入命令: "角色进入: position, "sprite_path""
        std::string params = command.substr(4); // 去掉"角色进入:"部分
        size_t commaPos = params.find(",");
        if (commaPos != std::string::npos) {
            std::string position = trim(params.substr(0, commaPos));
            std::string spritePath = trim(params.substr(commaPos + 1));
            
            // 去掉引号
            if (spritePath.front() == '"' && spritePath.back() == '"') {
                spritePath = spritePath.substr(1, spritePath.length() - 2);
            }
            
            if (_setCharacterCallback) {
                _setCharacterCallback("temp_char", spritePath, position);
            }
        }
    }
    else if (cmd.find("对话:") == 0) {
        // 解析对话命令: "对话: speaker, "text""
        std::string params = command.substr(3); // 去掉"对话:"部分
        size_t commaPos = params.find(",");
        if (commaPos != std::string::npos) {
            std::string speaker = trim(params.substr(0, commaPos));
            std::string text = trim(params.substr(commaPos + 1));
            
            // 去掉引号
            if (text.front() == '"' && text.back() == '"') {
                text = text.substr(1, text.length() - 2);
            }
            
            if (_setDialogCallback) {
                _setDialogCallback(speaker, text);
            }
        }
    }
    else if (cmd.find("选择:") == 0) {
        // 解析选择命令: "选择: "option1", "option2", "option3""
        std::string params = command.substr(3); // 去掉"选择:"部分
        std::vector<std::string> options;
        
        size_t start = 0;
        size_t end = 0;
        
        while ((end = params.find("\", \"", start)) != std::string::npos) {
            std::string option = params.substr(start, end - start);
            if (option.front() == '"') {
                option = option.substr(1);
            }
            options.push_back(option);
            start = end + 3; // Skip "\", \""
        }
        
        // 处理最后一个选项
        std::string lastOption = params.substr(start);
        if (lastOption.back() == '"') {
            lastOption = lastOption.substr(0, lastOption.length() - 1);
        }
        options.push_back(lastOption);
        
        if (_setChoiceCallback) {
            _setChoiceCallback(options, [](int choiceIndex) {
                CCLOG("Choice %d selected", choiceIndex);
            });
        }
    }
    else if (cmd == "等待") {
        if (_setWaitCallback) {
            _setWaitCallback();
        }
    }
    else if (cmd.find("如果选择") == 0) {
        // 这是一个条件语句，暂时不做处理
        CCLOG("Conditional statement: %s", command.c_str());
    }
    else if (cmd == "结束选择") {
        // 结束选择标记，暂时不做处理
        CCLOG("End of choice block");
    }
    else {
        CCLOG("Unknown command: %s", command.c_str());
    }
}

std::vector<std::string> ScriptParser::splitCommand(const std::string& command, char delimiter)
{
    std::vector<std::string> result;
    std::istringstream stream(command);
    std::string part;
    
    while (std::getline(stream, part, delimiter)) {
        result.push_back(trim(part));
    }
    
    return result;
}

std::string ScriptParser::trim(const std::string& str)
{
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return ""; // 字符串全是空白字符
    }
    
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

void ScriptParser::reset()
{
    _currentLineIndex = 0;
    _currentLine = "";
}