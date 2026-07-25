#include "GalGameScene.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace gal;

Scene* GalGameScene::createScene()
{
    return GalGameScene::create();
}

bool GalGameScene::init()
{
    if (!Scene::init()) return false;

    const auto visible = Director::getInstance()->getVisibleSize();
    const auto origin = Director::getInstance()->getVisibleOrigin();

    _bg = LayerColor::create(Color4B(40, 55, 80, 255), visible.width, visible.height);
    _bg->setPosition(origin);
    addChild(_bg, 0);

    _bgLabel = Label::createWithSystemFont("", "Microsoft YaHei", 22);
    _bgLabel->setTextColor(Color4B(255, 255, 255, 90));
    _bgLabel->setPosition(Vec2(origin.x + visible.width * 0.5f, origin.y + visible.height - 36));
    addChild(_bgLabel, 1);

    _titleLabel = Label::createWithSystemFont("GalGame Demo", "Microsoft YaHei", 18);
    _titleLabel->setAnchorPoint(Vec2(0, 1));
    _titleLabel->setPosition(Vec2(origin.x + 16, origin.y + visible.height - 12));
    _titleLabel->setTextColor(Color4B(200, 220, 255, 160));
    addChild(_titleLabel, 5);

    _charLayer = Node::create();
    addChild(_charLayer, 2);

    _dialogue = DialogueBox::create();
    addChild(_dialogue, 10);

    _choices = ChoicePanel::create();
    addChild(_choices, 20);

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = [](Touch*, Event*) { return true; };
    listener->onTouchEnded = [this](Touch*, Event*) { onTap(nullptr); };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    bindEngine();

    std::string err;
    if (!_engine.loadScript("scripts/demo.json", err)) {
        auto errLabel = Label::createWithSystemFont("脚本加载失败:\n" + err, "Microsoft YaHei", 24);
        errLabel->setTextColor(Color4B::RED);
        errLabel->setPosition(Vec2(origin.x + visible.width * 0.5f, origin.y + visible.height * 0.5f));
        addChild(errLabel, 100);
        return true;
    }

    _titleLabel->setString(_engine.title());
    _engine.start();
    return true;
}

void GalGameScene::bindEngine()
{
    _engine.setCallbacks(
        [this](const std::string& speaker, const std::string& text, bool isNarration) {
            _choices->hideChoices();
            _dialogue->showLine(speaker, text, isNarration);
        },
        [this](const std::string& name) { setBackground(name); },
        [this](const std::string& name, const std::string& emotion, const std::string& position) {
            showCharacter(name, emotion, position);
        },
        [this](const std::string& name) { hideCharacter(name); },
        [this](const std::vector<ChoiceOption>& options) {
            _dialogue->hideBox();
            _choices->showChoices(options);
        },
        [this]() {
            for (auto& kv : _characters) {
                if (kv.second) kv.second->removeFromParent();
            }
            _characters.clear();
            _dialogue->hideBox();
            _choices->hideChoices();
        },
        [this]() {
            _choices->hideChoices();
            _dialogue->showLine("", "—— 完 ——\n（点击可重新开始）", true);
        },
        [this](float duration) {
            scheduleOnce([this](float) { _engine.onWaitFinished(); }, duration, "gal_wait");
        }
    );

    _choices->onSelected = [this](int index) {
        _choices->hideChoices();
        _dialogue->showBox();
        _engine.selectChoice(index);
    };
}

void GalGameScene::onTap(Ref*)
{
    if (_engine.isWaitingChoice() || _engine.isWaitingTimer()) return;

    if (_engine.isFinished()) {
        for (auto& kv : _characters) {
            if (kv.second) kv.second->removeFromParent();
        }
        _characters.clear();
        _choices->hideChoices();
        _engine.start();
        return;
    }

    if (_dialogue->isTyping()) {
        _dialogue->skipTyping();
        return;
    }
    _engine.advance();
}

void GalGameScene::setBackground(const std::string& name)
{
    unsigned h = 2166136261u;
    for (unsigned char c : name) { h ^= c; h *= 16777619u; }
    Color4B c(
        static_cast<GLubyte>(30 + (h & 0x3F)),
        static_cast<GLubyte>(40 + ((h >> 7) & 0x4F)),
        static_cast<GLubyte>(55 + ((h >> 14) & 0x5F)),
        255);
    _bg->setColor(Color3B(c.r, c.g, c.b));
    _bgLabel->setString("[背景] " + name);
}

Vec2 GalGameScene::positionFor(const std::string& position) const
{
    const auto visible = Director::getInstance()->getVisibleSize();
    const auto origin = Director::getInstance()->getVisibleOrigin();
    float x = origin.x + visible.width * 0.5f;
    if (position == "left") x = origin.x + visible.width * 0.22f;
    else if (position == "right") x = origin.x + visible.width * 0.78f;
    return Vec2(x, origin.y + 210.f);
}

void GalGameScene::showCharacter(const std::string& name, const std::string& emotion, const std::string& position)
{
    CharacterSprite* spr = nullptr;
    auto it = _characters.find(name);
    if (it != _characters.end() && it->second) {
        spr = it->second;
    } else {
        spr = CharacterSprite::create(name);
        _charLayer->addChild(spr);
        _characters[name] = spr;
        spr->playEnter();
    }
    spr->setEmotion(emotion);
    spr->setPosition(positionFor(position));
}

void GalGameScene::hideCharacter(const std::string& name)
{
    auto it = _characters.find(name);
    if (it == _characters.end() || !it->second) return;
    auto* spr = it->second;
    _characters.erase(it);
    spr->playExit();
}
