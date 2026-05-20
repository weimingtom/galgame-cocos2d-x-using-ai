#include "GameScene.h"

USING_NS_CC;

Scene* GameScene::createScene() {
    return GameScene::create();
}

bool GameScene::init() {
    if (!Scene::init()) return false;

    auto vs = Director::getInstance()->getVisibleSize();

    // Default background (dark gray placeholder)
    _background = Sprite::create();
    _background->setTextureRect(Rect(0, 0, vs.width, vs.height));
    _background->setColor(Color3B(30, 30, 40));
    _background->setPosition(Vec2(vs.width * 0.5f, vs.height * 0.5f));
    _background->setLocalZOrder(-10);
    this->addChild(_background, -10);

    // Dialogue box at the bottom
    _dialogueBox = DialogueBox::create();
    _dialogueBox->setPosition(Vec2(0, 0));
    _dialogueBox->setLocalZOrder(10);
    this->addChild(_dialogueBox, 10);

    // Choice menu overlay
    _choiceMenu = ChoiceMenu::create();
    _choiceMenu->setLocalZOrder(20);
    this->addChild(_choiceMenu, 20);

    // Touch-to-advance when dialogue is done typing
    auto* listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
CCLOG("GameScene::init");
    listener->onTouchBegan = [this](Touch*, Event*) -> bool {
CCLOG("GameScene::init onTouchBegan");
        if (_choiceMenu->isVisible()) return false; // let choice menu handle touches

        if (_engine.isFinished()) return false;

        if (_dialogueBox->isTyping()) {
            _dialogueBox->skipToEnd();
            return true;
        }

        // Advance: only when dialogue is shown and done typing
        if (_dialogueBox->isVisible() && !_dialogueBox->isTyping()) {
            advanceScript();
            return true;
        }

        // If no dialogue is visible, advance directly
        if (!_dialogueBox->isVisible()) {
            advanceScript();
            return true;
        }

        return false;
    };
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

auto listener1 = EventListenerMouse::create();
listener1->onMouseDown = [this](Event*) -> void {
        if (_choiceMenu->isVisible()) return; // let choice menu handle touches

        if (_engine.isFinished()) return;

        if (_dialogueBox->isTyping()) {
            _dialogueBox->skipToEnd();
            return;
        }

        // Advance: only when dialogue is shown and done typing
        if (_dialogueBox->isVisible() && !_dialogueBox->isTyping()) {
            advanceScript();
            return;
        }

        // If no dialogue is visible, advance directly
        if (!_dialogueBox->isVisible()) {
            advanceScript();
            return;
        }

        return;
    };
_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, this);

auto listener2 = EventListenerKeyboard::create();
    listener2->onKeyPressed = [this](cocos2d::EventKeyboard::KeyCode keyCode, 
cocos2d::Event* event) {
CCLOG("onKeyReleased");
        if (_choiceMenu->isVisible()) return; // let choice menu handle touches

        if (_engine.isFinished()) return;

        if (_dialogueBox->isTyping()) {
            _dialogueBox->skipToEnd();
            return;
        }

        // Advance: only when dialogue is shown and done typing
        if (_dialogueBox->isVisible() && !_dialogueBox->isTyping()) {
            advanceScript();
            return;
        }

        // If no dialogue is visible, advance directly
        if (!_dialogueBox->isVisible()) {
            advanceScript();
            return;
        }
};
Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener2, this);


    // Start after a short delay
    auto* scheduler = Director::getInstance()->getScheduler();
    scheduler->schedule([this](float) {
        startGame();
    }, this, 0, 0, 0.2f, false, "startup");

    return true;
}

void GameScene::startGame() {
    if (_engine.loadScript("scripts/chapter1.txt")) {
        _engine.start([this](const ScriptCommand& cmd) {
            processCommand(cmd);
        });
    } else {
        // Fallback: show a demo message if script is missing
        CCLOG("GameScene: No script found, using built-in demo");
        _background->setTextureRect(Rect(0, 0,
            Director::getInstance()->getVisibleSize().width,
            Director::getInstance()->getVisibleSize().height));
        _background->setColor(Color3B(40, 30, 50));
        _dialogueBox->showText("System", "Welcome to the Galgame Engine Demo!\n\nScript file not found. Create Resources/scripts/chapter1.txt to write your story.");
    }
}

void GameScene::processCommand(const ScriptCommand& cmd) {
    auto vs = Director::getInstance()->getVisibleSize();

    switch (cmd.type) {

    case ScriptCommand::BG: {
        // Try to load background image; fall back to colored placeholder
        auto* bg = Sprite::create(cmd.param1);
        if (bg) {
            bg->setPosition(Vec2(vs.width * 0.5f, vs.height * 0.5f));
            float scaleX = vs.width / bg->getContentSize().width;
            float scaleY = vs.height / bg->getContentSize().height;
            bg->setScale(MAX(scaleX, scaleY));
            this->removeChild(_background);
            _background = bg;
            _background->setLocalZOrder(-10);
            this->addChild(_background, -10);
        }
        advanceScript();
        break;
    }

    case ScriptCommand::CHAR: {
        auto* character = CharacterSprite::create(cmd.param1);
        character->setLocalZOrder(0);
        this->addChild(character, 0);
        _characters[cmd.param1] = character;
        character->showSprite(cmd.param2, cmd.param3);
        advanceScript();
        break;
    }

    case ScriptCommand::CHAR_HIDE: {
        auto it = _characters.find(cmd.param1);
        if (it != _characters.end()) {
            it->second->hideSprite();
            _characters.erase(it);
        }
        advanceScript();
        break;
    }

    case ScriptCommand::TEXT: {
        _dialogueBox->showText(cmd.param1, cmd.param2);
        // Don't auto-advance; player taps to continue
        break;
    }

    case ScriptCommand::CHOICE: {
        _pendingChoiceTexts.push_back(cmd.param1);
        _pendingChoiceLabels.push_back(cmd.param2);
        // Collect consecutive CHOICE commands
        _engine.nextCommand();
        size_t safety = 0;
        while (!_engine.isFinished() && _engine.currentCommand().type == ScriptCommand::CHOICE && safety < 20) {
            const auto& nextCmd = _engine.currentCommand();
            _pendingChoiceTexts.push_back(nextCmd.param1);
            _pendingChoiceLabels.push_back(nextCmd.param2);
            _engine.nextCommand();
            safety++;
        }
        // After collecting, process the next non-CHOICE command (should be CHOICES)
        if (!_engine.isFinished()) {
            processCommand(_engine.currentCommand());
        }
        break;
    }

    case ScriptCommand::CHOICES: {
        // Show all accumulated choices
        _choiceMenu->hide(); // reset
        for (size_t i = 0; i < _pendingChoiceTexts.size(); ++i) {
            _choiceMenu->addChoice(_pendingChoiceTexts[i], _pendingChoiceLabels[i]);
        }
        _pendingChoiceTexts.clear();
        _pendingChoiceLabels.clear();
        _dialogueBox->hide();
        _choiceMenu->show([this](int index, const std::string& jumpLabel) {
            onChoiceSelected(index, jumpLabel);
        });
        // Don't auto-advance
        break;
    }

    case ScriptCommand::JUMP: {
        _engine.jumpToLabel(cmd.param1);
        advanceScript();
        break;
    }

    case ScriptCommand::WAIT: {
        float delay = std::stof(cmd.param1);
        auto* scheduler = Director::getInstance()->getScheduler();
        scheduler->schedule([this](float) {
            advanceScript();
        }, this, 0, 0, delay, false, "wait_timer");
        break;
    }

    case ScriptCommand::END: {
        _dialogueBox->hide();
        for (auto& pair : _characters) {
            pair.second->hideSprite();
        }
        CCLOG("GameScene: Script ended.");
        break;
    }

    default:
        break;
    }
}

void GameScene::advanceScript() {
    if (_engine.isFinished()) return;

    _engine.nextCommand();
    if (_engine.isFinished()) return;

    const auto& cmd = _engine.currentCommand();

    // Skip labels silently
    if (cmd.type == ScriptCommand::LABEL) {
        _engine.nextCommand();
        if (_engine.isFinished()) return;
        processCommand(_engine.currentCommand());
        return;
    }

    processCommand(cmd);
}

void GameScene::onChoiceSelected(int index, const std::string& jumpLabel) {
    _choiceMenu->hide();
    _dialogueBox->show();
    _engine.jumpToLabel(jumpLabel);
    advanceScript();
}

