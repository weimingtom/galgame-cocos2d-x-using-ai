#include "ChoicePanel.h"

USING_NS_CC;
using namespace gal;

bool ChoicePanel::init()
{
    if (!Node::init()) return false;
    _container = Node::create();
    addChild(_container);
    setVisible(false);
    return true;
}

void ChoicePanel::showChoices(const std::vector<ChoiceOption>& options)
{
    _container->removeAllChildren();
    setVisible(true);

    const auto visible = Director::getInstance()->getVisibleSize();
    const auto origin = Director::getInstance()->getVisibleOrigin();

    const float btnW = 520.f;
    const float btnH = 52.f;
    const float gap = 14.f;
    const float totalH = options.empty() ? 0.f
        : options.size() * btnH + (options.size() - 1) * gap;
    float startY = origin.y + visible.height * 0.48f + totalH * 0.5f;

    for (size_t i = 0; i < options.size(); ++i) {
        const int index = static_cast<int>(i);

        auto btn = Node::create();
        btn->setContentSize(Size(btnW, btnH));
        btn->setAnchorPoint(Vec2(0.5f, 0.5f));
        btn->setPosition(Vec2(origin.x + visible.width * 0.5f, startY - i * (btnH + gap)));

        auto bg = LayerColor::create(Color4B(35, 45, 70, 230), btnW, btnH);
        btn->addChild(bg);

        auto border = LayerColor::create(Color4B(100, 160, 255, 120), btnW, 2.f);
        border->setPosition(Vec2(0, btnH - 2));
        btn->addChild(border);

        auto label = Label::createWithSystemFont(options[i].text, "Microsoft YaHei", 22);
        label->setTextColor(Color4B(240, 245, 255, 255));
        label->setPosition(Vec2(btnW * 0.5f, btnH * 0.5f));
        btn->addChild(label);

        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [btn](Touch* touch, Event*) {
            const Vec2 loc = btn->convertToNodeSpace(touch->getLocation());
            const Rect rect(0, 0, btn->getContentSize().width, btn->getContentSize().height);
            if (rect.containsPoint(loc)) {
                btn->setScale(0.97f);
                return true;
            }
            return false;
        };
        listener->onTouchEnded = [this, btn, index](Touch* touch, Event*) {
            btn->setScale(1.f);
            const Vec2 loc = btn->convertToNodeSpace(touch->getLocation());
            const Rect rect(0, 0, btn->getContentSize().width, btn->getContentSize().height);
            if (rect.containsPoint(loc) && onSelected) {
                onSelected(index);
            }
        };
        listener->onTouchCancelled = [btn](Touch*, Event*) { btn->setScale(1.f); };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, btn);

        btn->setOpacity(0);
        btn->setScale(0.92f);
        btn->runAction(Spawn::create(
            FadeIn::create(0.18f + static_cast<float>(i) * 0.05f),
            ScaleTo::create(0.18f + static_cast<float>(i) * 0.05f, 1.f),
            nullptr));

        _container->addChild(btn);
    }
}

void ChoicePanel::hideChoices()
{
    _container->removeAllChildren();
    setVisible(false);
}
