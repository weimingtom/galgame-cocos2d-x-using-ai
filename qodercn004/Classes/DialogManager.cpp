#include "DialogManager.h"
#include "platform/CCFileUtils.h"

USING_NS_CC;

DialogManager* DialogManager::_instance = nullptr;

DialogManager* DialogManager::getInstance()
{
    if (_instance == nullptr) {
        _instance = new(std::nothrow) DialogManager();
    }
    return _instance;
}

void DialogManager::destroyInstance()
{
    if (_instance) {
        delete _instance;
        _instance = nullptr;
    }
}

DialogManager::DialogManager()
    : _currentIndex(0)
{
}

DialogManager::~DialogManager()
{
    _dialogs.clear();
}

bool DialogManager::loadScript(const std::string& scriptPath)
{
    // 读取脚本文件
    std::string content = FileUtils::getInstance()->getStringFromFile(scriptPath);
    if (content.empty()) {
        CCLOG("Failed to load script: %s", scriptPath.c_str());
        return false;
    }
    
    _currentScriptPath = scriptPath;
    _currentIndex = 0;
    _dialogs.clear();
    
    parseScriptContent(content);
    
    return true;
}

void DialogManager::parseScriptContent(const std::string& content)
{
    // 简化的脚本解析器
    // 实际实现中，你可能需要更复杂的解析逻辑
    // 这里我们假设对话格式为：
    // [character_name]: dialog_text
    // 或者特殊指令如 [background: bg_name] 等
    
    // 为了演示目的，我们创建一些示例对话
    _dialogs.push_back(GameData::DialogData("Narrator", "欢迎来到GalGame世界！"));
    _dialogs.push_back(GameData::DialogData("Alice", "你好，我是Alice。"));
    _dialogs.push_back(GameData::DialogData("Bob", "我是Bob，很高兴认识你。"));
    _dialogs.push_back(GameData::DialogData("Narrator", "现在你可以和他们对话了。"));
}

GameData::DialogData DialogManager::getCurrentDialog() const
{
    if (_currentIndex >= 0 && _currentIndex < _dialogs.size()) {
        return _dialogs[_currentIndex];
    }
    
    // 返回空对话数据
    return GameData::DialogData();
}

bool DialogManager::nextDialog()
{
    if (_currentIndex < _dialogs.size() - 1) {
        _currentIndex++;
        return true;
    }
    return false; // 已经是最后一段对话
}

bool DialogManager::goToDialog(int index)
{
    if (index >= 0 && index < _dialogs.size()) {
        _currentIndex = index;
        return true;
    }
    return false;
}

bool DialogManager::hasNextDialog() const
{
    return _currentIndex < _dialogs.size() - 1;
}

void DialogManager::reset()
{
    _currentIndex = 0;
}