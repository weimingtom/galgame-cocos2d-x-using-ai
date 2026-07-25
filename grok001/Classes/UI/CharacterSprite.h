#pragma once

#include "cocos2d.h"
#include <functional>
#include <string>

/** Procedural character stand-in (colored silhouette + name). */
class CharacterSprite : public cocos2d::Node {
public:
    static CharacterSprite* create(const std::string& name);
    bool initWithName(const std::string& name);

    void setEmotion(const std::string& emotion);
    void playEnter();
    void playExit(const std::function<void()>& done = nullptr);

    const std::string& characterName() const { return _name; }

private:
    cocos2d::Color3B colorForName(const std::string& name) const;

    std::string _name;
    cocos2d::LayerColor* _body = nullptr;
    cocos2d::LayerColor* _head = nullptr;
    cocos2d::Label* _label = nullptr;
    cocos2d::Label* _emotionLabel = nullptr;
};
