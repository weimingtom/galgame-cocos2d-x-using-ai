#include "CharacterSprite.h"

USING_NS_CC;

CharacterSprite* CharacterSprite::create(const std::string& name)
{
    auto* p = new (std::nothrow) CharacterSprite();
    if (p && p->initWithName(name)) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool CharacterSprite::initWithName(const std::string& name)
{
    if (!Node::init()) return false;
    _name = name;
    const Color3B c = colorForName(name);

    _body = LayerColor::create(Color4B(c.r, c.g, c.b, 230), 140, 280);
    _body->setAnchorPoint(Vec2(0.5f, 0));
    _body->setIgnoreAnchorPointForPosition(false);
    _body->setPosition(Vec2(0, 0));
    addChild(_body);

    _head = LayerColor::create(Color4B(
        static_cast<GLubyte>(std::min(255, c.r + 30)),
        static_cast<GLubyte>(std::min(255, c.g + 30)),
        static_cast<GLubyte>(std::min(255, c.b + 30)), 255), 90, 90);
    _head->setAnchorPoint(Vec2(0.5f, 0));
    _head->setIgnoreAnchorPointForPosition(false);
    _head->setPosition(Vec2(0, 280));
    addChild(_head);

    _label = Label::createWithSystemFont(name, "Microsoft YaHei", 20);
    _label->setPosition(Vec2(0, -24));
    _label->setTextColor(Color4B::WHITE);
    addChild(_label);

    _emotionLabel = Label::createWithSystemFont("", "Microsoft YaHei", 18);
    _emotionLabel->setPosition(Vec2(0, 325));
    _emotionLabel->setTextColor(Color4B(255, 230, 150, 255));
    addChild(_emotionLabel);

    setContentSize(Size(140, 370));
    setAnchorPoint(Vec2(0.5f, 0));
    setCascadeOpacityEnabled(true);
    return true;
}

Color3B CharacterSprite::colorForName(const std::string& name) const
{
    // Stable hash -> pastel-ish color
    unsigned h = 2166136261u;
    for (unsigned char ch : name) {
        h ^= ch;
        h *= 16777619u;
    }
    return Color3B(
        static_cast<GLubyte>(80 + (h & 0x7F)),
        static_cast<GLubyte>(80 + ((h >> 8) & 0x7F)),
        static_cast<GLubyte>(100 + ((h >> 16) & 0x7F)));
}

void CharacterSprite::setEmotion(const std::string& emotion)
{
    if (emotion.empty()) {
        _emotionLabel->setString("");
        return;
    }
    _emotionLabel->setString("(" + emotion + ")");
}

void CharacterSprite::playEnter()
{
    setOpacity(0);
    setScale(0.95f);
    runAction(Spawn::create(FadeIn::create(0.25f), ScaleTo::create(0.25f, 1.f), nullptr));
}

void CharacterSprite::playExit(const std::function<void()>& done)
{
    runAction(Sequence::create(
        Spawn::create(FadeOut::create(0.2f), ScaleTo::create(0.2f, 0.9f), nullptr),
        CallFunc::create([done]() { if (done) done(); }),
        RemoveSelf::create(),
        nullptr));
}
