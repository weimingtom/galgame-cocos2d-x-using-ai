#pragma once

#include "cocos2d.h"
#include <functional>
#include <string>

class DialogueBox : public cocos2d::Node {
public:
    CREATE_FUNC(DialogueBox);
    bool init() override;

    void showLine(const std::string& speaker, const std::string& text, bool isNarration);
    void setTypingSpeed(float charsPerSecond) { _cps = charsPerSecond; }
    bool isTyping() const { return _typing; }
    void skipTyping();
    void hideBox();
    void showBox();

    std::function<void()> onLineFinished; // typing done (optional)

private:
    void updateTyping(float dt);
    void finishTyping();

    cocos2d::LayerColor* _panel = nullptr;
    cocos2d::Label* _nameLabel = nullptr;
    cocos2d::Label* _textLabel = nullptr;
    cocos2d::Label* _hintLabel = nullptr;

    std::string _fullText;
    size_t _visibleChars = 0;
    float _accum = 0.f;
    float _cps = 48.f;
    bool _typing = false;
};
