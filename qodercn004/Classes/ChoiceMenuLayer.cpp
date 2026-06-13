#include "ChoiceMenuLayer.h"

USING_NS_CC;

/*ChoiceMenuLayer* ChoiceMenuLayer::create()
{
    ChoiceMenuLayer *pRet = new(std::nothrow) ChoiceMenuLayer();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}*/

bool ChoiceMenuLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }

    createChoiceMenu();
    this->setVisible(false); // 初始隐藏

    return true;
}

void ChoiceMenuLayer::createChoiceMenu()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // 创建菜单
    _choiceMenu = Menu::create();
    _choiceMenu->setPosition(Vec2::ZERO);
    this->addChild(_choiceMenu, 1);
    
    // 设置默认位置
    _choiceMenu->setPosition(Vec2(origin.x + visibleSize.width/2, 
                                 origin.y + visibleSize.height/2));
}

void ChoiceMenuLayer::showChoices(const std::vector<std::string>& choices, 
                                 std::function<void(int)> callback)
{
    // 清除之前的选项
    for (auto item : _choiceItems) {
        _choiceMenu->removeChild(item, true);
    }
    _choiceItems.clear();
    
    _callback = callback;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // 计算每个选项的位置
    float totalHeight = choices.size() * 60; // 假设每个选项高度为60
    float startY = origin.y + visibleSize.height/2 + totalHeight/2 - 30; // 起始Y坐标
    
    for (int i = 0; i < choices.size(); i++) {
        auto menuItem = MenuItemFont::create(choices[i], 
                                           CC_CALLBACK_1(ChoiceMenuLayer::onChoiceSelected, this));
        menuItem->setTag(i); // 使用tag存储选项索引
        menuItem->setPosition(Vec2(0, -i * 60)); // 垂直排列
        menuItem->setColor(Color3B::YELLOW);
        menuItem->setFontSizeObj(24);
        
        _choiceItems.push_back(menuItem);
        _choiceMenu->addChild(menuItem);
    }
    
    this->setVisible(true);
}

void ChoiceMenuLayer::hideChoices()
{
    this->setVisible(false);
    
    // 清除所有选项
    for (auto item : _choiceItems) {
        _choiceMenu->removeChild(item, true);
    }
    _choiceItems.clear();
}

void ChoiceMenuLayer::onChoiceSelected(cocos2d::Ref* sender)
{
    auto menuItem = static_cast<MenuItemFont*>(sender);
    int selectedIndex = menuItem->getTag();
    
    // 隐藏菜单
    hideChoices();
    
    // 调用回调函数
    if (_callback) {
        _callback(selectedIndex);
    }
}