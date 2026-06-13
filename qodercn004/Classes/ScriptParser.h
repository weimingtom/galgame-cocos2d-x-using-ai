#ifndef __SCRIPT_PARSER_H__
#define __SCRIPT_PARSER_H__

#include "cocos2d.h"
#include "GameData.h"
#include <functional>

/**
 * @brief 脚本解析器类
 * 解析和执行GalGame脚本
 */
class ScriptParser : public cocos2d::Ref
{
public:
    static ScriptParser* getInstance();
    static void destroyInstance();
    
    /**
     * @brief 加载并解析脚本
     * @param scriptPath 脚本文件路径
     */
    bool loadScript(const std::string& scriptPath);
    
    /**
     * @brief 执行下一行脚本
     * @return 是否还有更多脚本行
     */
    bool executeNextLine();
    
    /**
     * @brief 获取当前脚本行
     */
    std::string getCurrentLine() const { return _currentLine; }
    
    /**
     * @brief 检查是否有更多脚本
     */
    bool hasMoreLines() const { return _currentLineIndex < _scriptLines.size(); }
    
    /**
     * @brief 重置脚本到开头
     */
    void reset();
    
    /**
     * @brief 设置各种回调函数
     */
    void setBackgroundCallback(std::function<void(const std::string&)> callback) { _setBackgroundCallback = callback; }
    void setCharacterCallback(std::function<void(const std::string&, const std::string&, const std::string&)> callback) { _setCharacterCallback = callback; }
    void setDialogCallback(std::function<void(const std::string&, const std::string&)> callback) { _setDialogCallback = callback; }
    void setChoiceCallback(std::function<void(const std::vector<std::string>&, std::function<void(int)>)> callback) { _setChoiceCallback = callback; }
    void setWaitCallback(std::function<void()> callback) { _setWaitCallback = callback; }

private:
    static ScriptParser* _instance;
    
    std::vector<std::string> _scriptLines;  // 脚本行列表
    int _currentLineIndex;                  // 当前脚本行索引
    std::string _currentLine;               // 当前脚本行
    
    // 回调函数
    std::function<void(const std::string&)> _setBackgroundCallback;
    std::function<void(const std::string&, const std::string&, const std::string&)> _setCharacterCallback;
    std::function<void(const std::string&, const std::string&)> _setDialogCallback;
    std::function<void(const std::vector<std::string>&, std::function<void(int)>)> _setChoiceCallback;
    std::function<void()> _setWaitCallback;
    
    ScriptParser();
    virtual ~ScriptParser();
    
    void parseScriptContent(const std::string& content);  // 解析脚本内容
    void executeCommand(const std::string& command);      // 执行命令
    std::vector<std::string> splitCommand(const std::string& command, char delimiter);  // 分割命令
    std::string trim(const std::string& str);            // 去除字符串首尾空白
};

#endif // __SCRIPT_PARSER_H__