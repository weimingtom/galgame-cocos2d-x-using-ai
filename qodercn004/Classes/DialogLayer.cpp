#include "DialogLayer.h"
#include "DialogManager.h"

USING_NS_CC;

/*DialogLayer* DialogLayer::create()
{
    DialogLayer *pRet = new(std::nothrow) DialogLayer();
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

bool DialogLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }

    createDialogBox();
    hideDialog(); // 初始隐藏对话框
    
    // 初始化对话管理器并加载示例脚本
    _dialogManager = DialogManager::getInstance();
    // 加载示例对话
    updateDialogDisplay();

    return true;
}

void DialogLayer::createDialogBox()
{
    // 创建对话框背景
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // 创建一个矩形背景作为对话框
    _dialogBox = Sprite::create();
    if (_dialogBox == nullptr) {
        // 如果无法加载精灵，则手动绘制矩形
        auto drawNode = DrawNode::create();
        Color4F bgColor(0.0f, 0.0f, 0.0f, 0.7f); // 半透明黑色背景
        Vec2 pos = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.2f);
        drawNode->drawSolidRect(
            Vec2(-visibleSize.width * 0.45f, -visibleSize.height * 0.15f),
            Vec2(visibleSize.width * 0.45f, visibleSize.height * 0.15f),
            bgColor
        );
        this->addChild(drawNode, 0);
        _dialogBox = Sprite::create(); // 使用临时精灵对象
    } else {
        _dialogBox->setContentSize(Size(visibleSize.width * 0.9f, visibleSize.height * 0.3f));
        _dialogBox->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.2f));
        _dialogBox->setColor(Color3B::BLACK);
        _dialogBox->setOpacity(180); // 70% 透明度
        this->addChild(_dialogBox, 0);
    }

    // 创建说话者名称标签
    _speakerName = Label::createWithTTF("", "fonts/arial.ttf", 24);
    if (_speakerName != nullptr) {
        _speakerName->setTextColor(Color4B::YELLOW);
        _speakerName->setPosition(Vec2(origin.x + visibleSize.width * 0.1f, origin.y + visibleSize.height * 0.32f));
        this->addChild(_speakerName, 1);
    }

    // 创建对话文本标签
    _dialogText = Label::createWithTTF("", "fonts/arial.ttf", 20);
    if (_dialogText != nullptr) {
        _dialogText->setTextColor(Color4B::WHITE);
        _dialogText->setDimensions(visibleSize.width * 0.8f, visibleSize.height * 0.2f);
        _dialogText->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
        _dialogText->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 0.15f));
        this->addChild(_dialogText, 1);
    }
}

void DialogLayer::updateDialogDisplay()
{
    if (_dialogManager) {
        auto currentDialog = _dialogManager->getCurrentDialog();
        if (!currentDialog.speaker.empty() || !currentDialog.text.empty()) {
            showDialog(currentDialog.text, currentDialog.speaker);
        }
    }
}

void DialogLayer::nextDialog()
{
    if (_dialogManager && _dialogManager->nextDialog()) {
        updateDialogDisplay();
    } else {
        CCLOG("Reached end of dialog");
    }
}

void DialogLayer::showDialog(const std::string& dialogText, const std::string& speakerName)
{
    if (_dialogText) {
        _dialogText->setString(dialogText);
    }
    
    if (_speakerName) {
        if (!speakerName.empty()) {
            _speakerName->setString(speakerName);
            _speakerName->setVisible(true);
        } else {
            _speakerName->setVisible(false);
        }
    }
    
    // 确保对话框可见
    if (_dialogBox) {
        _dialogBox->setVisible(true);
    }
}

void DialogLayer::hideDialog()
{
    if (_dialogBox) {
        _dialogBox->setVisible(false);
    }
    if (_dialogText) {
        _dialogText->setString("");
    }
    if (_speakerName) {
        _speakerName->setString("");
        _speakerName->setVisible(false);
    }
}

/*
void DialogLayer::nextDialog()
{
    // 这里会根据游戏数据推进到下一个对话
    // 简单示例：只处理当前显示的对话
    CCLOG("Next dialog triggered");
}
*/
