#ifndef __DIALOG_LAYER_H__
#define __DIALOG_LAYER_H__

#include "cocos2d.h"
#include "GameData.h"

class DialogManager; // 前向声明

/**
 * @brief 对话层类
 * 显示游戏中的对话文本、角色姓名等
 */
class DialogLayer : public cocos2d::Layer
{
public:
    //static DialogLayer* create();
    virtual bool init();
    
    CREATE_FUNC(DialogLayer);

    /**
     * @brief 显示下一条对话
     */
    void nextDialog();
    
    /**
     * @brief 显示指定对话
     * @param dialogText 对话内容
     * @param speakerName 说话者名称
     */
    void showDialog(const std::string& dialogText, const std::string& speakerName = "");
    
    /**
     * @brief 隐藏对话框
     */
    void hideDialog();

private:
    cocos2d::Sprite* _dialogBox;       // 对话框背景
    cocos2d::Label* _dialogText;       // 对话文本
    cocos2d::Label* _speakerName;      // 说话者名称
    DialogManager* _dialogManager;     // 对话管理器
    
    void createDialogBox();            // 创建对话框UI
    void updateDialogDisplay();        // 更新对话显示
};

#endif // __DIALOG_LAYER_H__