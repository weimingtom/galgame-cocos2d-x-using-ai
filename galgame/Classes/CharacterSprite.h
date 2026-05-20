#ifndef _CHARACTER_SPRITE_H_
#define _CHARACTER_SPRITE_H_

#include "cocos2d.h"
#include <string>

class CharacterSprite : public cocos2d::Node {
public:
    static CharacterSprite* create(const std::string& charId);
    virtual bool init(const std::string& charId);

    void showSprite(const std::string& filename, const std::string& position, float duration = 0.5f);
    void hideSprite(float duration = 0.5f);
    void setEmotion(const std::string& filename, float duration = 0.3f);
    const std::string& getCharId() const { return _charId; }
    bool isVisible() const { return _sprite && _sprite->getOpacity() > 128; }

private:
    std::string _charId;
    cocos2d::Sprite* _sprite = nullptr;

    cocos2d::Vec2 positionFor(const std::string& pos);
};

#endif
