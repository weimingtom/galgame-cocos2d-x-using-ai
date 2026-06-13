#ifndef __CHOICE_MENU_LAYER_H__
#define __CHOICE_MENU_LAYER_H__

#include "cocos2d.h"
#include "GameData.h"

/**
 * @brief 选择菜单层类
 * 显示可选项供玩家选择，影响剧情走向
 */
class ChoiceMenuLayer : public cocos2d::Layer
{
public:
    //static ChoiceMenuLayer* create();
    virtual bool init();
    
    CREATE_FUNC(ChoiceMenuLayer);

    /**
     * @brief 显示选择菜单
     * @param choices 选项列表
     * @param callback 选择回调函数
     */
    void showChoices(const std::vector<std::string>& choices, 
                    std::function<void(int)> callback);
    
    /**
     * @brief 隐藏选择菜单
     */
    void hideChoices();

private:
    std::vector<cocos2d::MenuItemFont*> _choiceItems;  // 选项菜单项
    cocos2d::Menu* _choiceMenu;                        // 选项菜单
    std::function<void(int)> _callback;                // 回调函数
    
    void createChoiceMenu();                           // 创建选择菜单
    void onChoiceSelected(cocos2d::Ref* sender);       // 选项被选择时的回调
};

#endif // __CHOICE_MENU_LAYER_H__