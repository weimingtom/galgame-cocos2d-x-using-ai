#include "ChoiceMenu.h"

USING_NS_CC;

ChoiceMenu* ChoiceMenu::create() {
    auto* p = new (std::nothrow) ChoiceMenu();
    if (p && p->init()) { p->autorelease(); return p; }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool ChoiceMenu::init() {
    if (!Node::init()) return false;

    auto vs = Director::getInstance()->getVisibleSize();

    // Semi-transparent overlay
    _bg = DrawNode::create();
    Vec2 verts[] = {
        Vec2::ZERO,
        Vec2(vs.width, 0),
        Vec2(vs.width, vs.height),
        Vec2(0, vs.height)
    };
    _bg->drawSolidPoly(verts, 4, Color4F(0, 0, 0, 0.5f));
    this->addChild(_bg, 0);

    this->setVisible(false);
    return true;
}

void ChoiceMenu::addChoice(const std::string& text, const std::string& jumpLabel) {
    _choiceTexts.push_back(text);
    _jumpLabels.push_back(jumpLabel);
}

void ChoiceMenu::show(ChoiceCallback callback) {
    this->removeChildByTag(100, true);
    _menu = nullptr;

    auto vs = Director::getInstance()->getVisibleSize();
    _menu = Menu::create();
    _menu->setTag(100);
    _menu->setPosition(Vec2::ZERO);

    float startY = vs.height * 0.55f;
    float spacing = 55.0f;

    for (size_t i = 0; i < _choiceTexts.size(); ++i) {
        auto* label = Label::createWithTTF(_choiceTexts[i], "fonts/arial.ttf", 24);
        label->setTextColor(Color4B(200, 200, 200, 255));

        auto* item = MenuItemLabel::create(label, [callback, i, this](Ref*) {
            callback(static_cast<int>(i), _jumpLabels[i]);
        });

        item->setPosition(Vec2(vs.width * 0.5f, startY - i * spacing));
        _menu->addChild(item);
    }

    this->addChild(_menu, 1);
    this->setVisible(true);

    // Fade in choices
    this->setOpacity(0);
    this->runAction(FadeIn::create(0.3f));
}

void ChoiceMenu::hide() {
    _choiceTexts.clear();
    _jumpLabels.clear();
    this->setVisible(false);
}
