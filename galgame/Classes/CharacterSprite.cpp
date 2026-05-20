#include "CharacterSprite.h"

USING_NS_CC;

CharacterSprite* CharacterSprite::create(const std::string& charId) {
    auto* p = new (std::nothrow) CharacterSprite();
    if (p && p->init(charId)) { p->autorelease(); return p; }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool CharacterSprite::init(const std::string& charId) {
    if (!Node::init()) return false;
    _charId = charId;
    return true;
}

Vec2 CharacterSprite::positionFor(const std::string& pos) {
    auto vs = Director::getInstance()->getVisibleSize();
    if (pos == "left")        return Vec2(vs.width * 0.25f, vs.height * 0.45f);
    if (pos == "right")       return Vec2(vs.width * 0.75f, vs.height * 0.45f);
    if (pos == "center")      return Vec2(vs.width * 0.50f, vs.height * 0.45f);
    if (pos == "farleft")     return Vec2(vs.width * 0.10f, vs.height * 0.45f);
    if (pos == "farright")    return Vec2(vs.width * 0.90f, vs.height * 0.45f);
    return Vec2(vs.width * 0.50f, vs.height * 0.45f); // default center
}

void CharacterSprite::showSprite(const std::string& filename, const std::string& position, float duration) {
    Vec2 targetPos = positionFor(position);

    if (_sprite) {
        // Crossfade to new sprite
        auto* oldSprite = _sprite;
        oldSprite->runAction(Sequence::create(
            FadeOut::create(duration * 0.5f),
            RemoveSelf::create(),
            nullptr
        ));
    }

    _sprite = Sprite::create(filename);
    if (!_sprite) {
        CCLOG("CharacterSprite: Failed to load sprite: %s", filename.c_str());
        return;
    }

    _sprite->setPosition(targetPos);
    _sprite->setOpacity(0);
    float scale = (Director::getInstance()->getVisibleSize().height * 0.6f) / _sprite->getContentSize().height;
    _sprite->setScale(scale);
    this->addChild(_sprite, 0);

    _sprite->runAction(FadeIn::create(duration));
}

void CharacterSprite::hideSprite(float duration) {
    if (!_sprite) return;
    _sprite->runAction(Sequence::create(
        FadeOut::create(duration),
        RemoveSelf::create(),
        nullptr
    ));
    _sprite = nullptr;
}

void CharacterSprite::setEmotion(const std::string& filename, float duration) {
    if (!_sprite) {
        showSprite(filename, "center", duration);
        return;
    }

    Vec2 pos = _sprite->getPosition();
    float scale = _sprite->getScale();

    auto* oldSprite = _sprite;
    oldSprite->runAction(Sequence::create(
        FadeOut::create(duration * 0.5f),
        RemoveSelf::create(),
        nullptr
    ));

    _sprite = Sprite::create(filename);
    if (!_sprite) return;

    _sprite->setPosition(pos);
    _sprite->setScale(scale);
    _sprite->setOpacity(0);
    this->addChild(_sprite, 0);
    _sprite->runAction(FadeIn::create(duration * 0.5f));
}
