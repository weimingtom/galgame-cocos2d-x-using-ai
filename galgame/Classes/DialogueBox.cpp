#include "DialogueBox.h"

USING_NS_CC;

DialogueBox* DialogueBox::create() {
    auto* p = new (std::nothrow) DialogueBox();
    if (p && p->init()) { p->autorelease(); return p; }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool DialogueBox::init() {
    if (!Node::init()) return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();

    // Semi-transparent background box
    _background = DrawNode::create();
    float boxH = 180;
    Vec2 origin(0, 0);
    Vec2 dest(visibleSize.width, boxH);
    Vec2 verts[] = {origin, Vec2(dest.x, origin.y), dest, Vec2(origin.x, dest.y)};
    _background->drawSolidPoly(verts, 4, Color4F(0, 0, 0, 0.75f));
    _background->setPosition(Vec2(0, 0));
    this->addChild(_background, 0);

    // Border line at top of the box
    auto* border = DrawNode::create();
    border->drawLine(Vec2(0, boxH), Vec2(visibleSize.width, boxH), Color4F(1, 1, 1, 0.6f));
    border->drawLine(Vec2(0, boxH + 1), Vec2(visibleSize.width, boxH + 1), Color4F(1, 1, 1, 0.3f));
    this->addChild(border, 1);

    // Speaker name label
    _nameLabel = Label::createWithTTF("", "fonts/arial.ttf", 22);
    _nameLabel->setAnchorPoint(Vec2(0, 0));
    _nameLabel->setPosition(Vec2(30, boxH + 10));
    _nameLabel->setTextColor(Color4B(255, 220, 100, 255));
    this->addChild(_nameLabel, 2);

    // Text label
    _textLabel = Label::createWithTTF("", "fonts/arial.ttf", 20);
    _textLabel->setAnchorPoint(Vec2(0, 1));
    _textLabel->setPosition(Vec2(50, boxH - 20));
    _textLabel->setDimensions(visibleSize.width - 100, 0);
    _textLabel->setTextColor(Color4B::WHITE);
    this->addChild(_textLabel, 2);

    // Blinking indicator triangle
    _indicator = Label::createWithTTF("▼", "fonts/arial.ttf", 16);
    _indicator->setPosition(Vec2(visibleSize.width - 40, 30));
    _indicator->setTextColor(Color4B(255, 255, 255, 200));
    _indicator->setVisible(false);
    this->addChild(_indicator, 2);

    // Run the blinking action
    auto* blink = RepeatForever::create(Sequence::create(
        FadeTo::create(0.4f, 60),
        FadeTo::create(0.4f, 200),
        nullptr));
    _indicator->runAction(blink);

    // Touch listener to advance text
    auto* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this](Touch*, Event*) {
        if (_isTyping) {
            skipToEnd();
        }
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void DialogueBox::showText(const std::string& speaker, const std::string& text, float charDelay) {
    _nameLabel->setString(speaker);
    _fullText = text;
    _currentText = "";
    _charIndex = 0;
    _charDelay = charDelay;
    _elapsed = 0.0f;
    _isTyping = true;
    _indicator->setVisible(false);

    // Show name with a slight color based on speaker
    std::hash<std::string> hasher;
    size_t h = hasher(speaker);
    _nameLabel->setTextColor(Color4B(
        200 + (h % 56),
        180 + ((h >> 8) % 56),
        80 + ((h >> 16) % 80),
        255
    ));

    this->schedule([this](float dt) {
        typeNextChar(dt);
    }, 0.016f, "typewriter");
}

void DialogueBox::typeNextChar(float dt) {
    if (!_isTyping) return;

    _elapsed += dt;
    while (_elapsed >= _charDelay && _charIndex < _fullText.size()) {
        _elapsed -= _charDelay;
        _currentText += _fullText[_charIndex++];
    }

    _textLabel->setString(_currentText);

    if (_charIndex >= _fullText.size()) {
        _isTyping = false;
        this->unschedule("typewriter");
        _indicator->setVisible(true);
    }
}

void DialogueBox::skipToEnd() {
    if (!_isTyping) return;
    _currentText = _fullText;
    _charIndex = _fullText.size();
    _textLabel->setString(_currentText);
    _isTyping = false;
    this->unschedule("typewriter");
    _indicator->setVisible(true);
}

void DialogueBox::hide() {
    this->setVisible(false);
}

void DialogueBox::show() {
    this->setVisible(true);
}
