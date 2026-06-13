#ifndef __DIALOG_MANAGER_H__
#define __DIALOG_MANAGER_H__

#include "cocos2d.h"
#include "GameData.h"

/**
 * @brief 对话管理器类
 * 管理对话数据、推进逻辑和分支选择
 */
class DialogManager : public cocos2d::Ref
{
public:
    static DialogManager* getInstance();
    static void destroyInstance();
    
    /**
     * @brief 加载对话脚本
     * @param scriptPath 脚本文件路径
     */
    bool loadScript(const std::string& scriptPath);
    
    /**
     * @brief 获取当前对话
     */
    GameData::DialogData getCurrentDialog() const;
    
    /**
     * @brief 推进到下一对话
     */
    bool nextDialog();
    
    /**
     * @brief 跳转到指定对话索引
     * @param index 对话索引
     */
    bool goToDialog(int index);
    
    /**
     * @brief 检查是否有更多对话
     */
    bool hasNextDialog() const;
    
    /**
     * @brief 获取当前对话索引
     */
    int getCurrentIndex() const { return _currentIndex; }
    
    /**
     * @brief 获取对话总数
     */
    int getTotalDialogCount() const { return _dialogs.size(); }
    
    /**
     * @brief 重置对话到开头
     */
    void reset();

private:
    static DialogManager* _instance;
    
    std::vector<GameData::DialogData> _dialogs;  // 对话数据列表
    int _currentIndex;                           // 当前对话索引
    std::string _currentScriptPath;             // 当前脚本路径
    
    DialogManager();
    virtual ~DialogManager();
    
    void parseScriptContent(const std::string& content); // 解析脚本内容
};

#endif // __DIALOG_MANAGER_H__