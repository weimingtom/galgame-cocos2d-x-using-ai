#ifndef _DIALOGUE_BOX_H_
#define _DIALOGUE_BOX_H_

#include "cocos2d.h"
#include <string>

class DialogueBox : public cocos2d::Node {
public:
    static DialogueBox* create();
    virtual bool init() override;

    void showText(const std::string& speaker, const std::string& text, float charDelay = 0.05f);
    void skipToEnd();
    bool isTyping() const { return _isTyping; }
    void hide();
    void show();

private:
    void typeNextChar(float dt);
    void onTap(cocos2d::Touch* touch, cocos2d::Event* event);

    cocos2d::DrawNode* _background = nullptr;
    cocos2d::Label* _nameLabel = nullptr;
    cocos2d::Label* _textLabel = nullptr;
    cocos2d::Label* _indicator = nullptr;   // blinking triangle

    std::string _fullText;
    std::string _currentText;
    size_t _charIndex = 0;
    float _charDelay = 0.05f;
    float _elapsed = 0.0f;
    bool _isTyping = false;
};

#endif
