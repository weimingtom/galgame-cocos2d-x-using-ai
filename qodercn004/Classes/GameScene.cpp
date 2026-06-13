#include "GameScene.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}

bool GameScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    initializeComponents();
    setupUI();
    setupSaveLoadUI();

    return true;
}

void GameScene::initializeComponents()
{
    // 初始化背景管理器
    _bgManager = BackgroundManager::create();
    this->addChild(_bgManager, 0); // 背景层在最底层
//_bgManager->changeBackground("bg_cafe_night");
//_bgManager->fadeIn();    

    // 初始化角色管理器
    _charManager = CharacterManager::create();
    this->addChild(_charManager, 1); // 角色层在中间

    // 初始化对话层
    _dialogLayer = DialogLayer::create();
    this->addChild(_dialogLayer, 2); // 对话层在最顶层
    
    // 初始化选择菜单层
    _choiceMenu = ChoiceMenuLayer::create();
    this->addChild(_choiceMenu, 3); // 选择菜单在最顶层


    
    // 初始化脚本解析器
    _scriptParser = ScriptParser::getInstance();
    setupScriptCallbacks();
    
    // 加载示例脚本
    _scriptParser->loadScript("sample_script.txt");
}

/*void GameScene::setupUI()
{
    // 设置点击事件监听对话层
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [=](Touch* touch, Event* event){
        // 触发对话系统下一条
        _dialogLayer->nextDialog();
        return true;
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}*/

void GameScene::setupSaveLoadUI()
{
    // 创建存档/读档菜单按钮（仅作示例）
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event){
        if (keyCode == EventKeyboard::KeyCode::KEY_F1) {
            // F1键快速存档到第0槽
            saveGame(0);
        } else if (keyCode == EventKeyboard::KeyCode::KEY_F2) {
            // F2键快速读档第0槽
            loadGame(0);
        }
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

void GameScene::startGame()
{
    // 开始游戏的逻辑
    CCLOG("Game started");
}

void GameScene::saveGame(int slot)
{
    auto saveMgr = SaveGameManager::getInstance();
    GameData::SaveData saveData;
    
    // 填充存档数据
    saveData.chapter = 1;
    saveData.scene = 1;
    // 这里应该获取当前游戏状态
    
    if (saveMgr->saveGame(slot, saveData)) {
        CCLOG("Game saved to slot %d", slot);
    } else {
        CCLOG("Failed to save game to slot %d", slot);
    }
}

void GameScene::loadGame(int slot)
{
    auto saveMgr = SaveGameManager::getInstance();
    GameData::SaveData saveData = saveMgr->loadGame(slot);
    
    CCLOG("Game loaded from slot %d", slot);
    // 这里应该根据存档数据恢复游戏状态
}

void GameScene::showSaveLoadMenu()
{
    // 显示存档/读档菜单的逻辑
    CCLOG("Show save/load menu");
}

void GameScene::onChoiceMade(int choiceIndex)
{
    // 处理玩家选择的逻辑
    CCLOG("Player chose option %d", choiceIndex);
    // 根据选择跳转到不同剧情
}

void GameScene::setupScriptCallbacks()
{
    if (_scriptParser) {
        _scriptParser->setBackgroundCallback(CC_CALLBACK_1(GameScene::onBackgroundChange, this));
        _scriptParser->setCharacterCallback(CC_CALLBACK_3(GameScene::onCharacterAction, this));
        _scriptParser->setDialogCallback(CC_CALLBACK_2(GameScene::onDialogShow, this));
        _scriptParser->setChoiceCallback(CC_CALLBACK_2(GameScene::onChoiceShow, this));
        _scriptParser->setWaitCallback(CC_CALLBACK_0(GameScene::onWaitAction, this));
    }
}

void GameScene::onBackgroundChange(const std::string& bgName)
{
    if (_bgManager) {
        // 构建背景图片路径
        std::string bgPath = "images/" + bgName + ".png";
        _bgManager->changeBackground(bgPath);
    }
}

void GameScene::onCharacterAction(const std::string& charId, const std::string& spritePath, const std::string& position)
{
    if (_charManager) {
        // 根据位置决定角色显示位置
        cocos2d::Vec2 pos;
        if (position == "alice_right") {
            pos = cocos2d::Vec2(600, 200); // 屏幕右侧
        } else if (position == "alice_left") {
            pos = cocos2d::Vec2(200, 200); // 屏幕左侧
        } else {
            pos = cocos2d::Vec2(400, 200); // 屏幕中央
        }
        
        _charManager->addCharacter(charId, "images/" + spritePath, pos);
    }
}

void GameScene::onDialogShow(const std::string& speaker, const std::string& text)
{
    if (_dialogLayer) {
        _dialogLayer->showDialog(text, speaker);
    }
}

void GameScene::onChoiceShow(const std::vector<std::string>& choices, std::function<void(int)> callback)
{
    if (_choiceMenu) {
        _choiceMenu->showChoices(choices, callback);
    }
}

void GameScene::onWaitAction()
{
    // 等待动作，通常什么都不做，让玩家阅读对话
    CCLOG("Waiting for player input...");
}

void GameScene::executeNextScriptLine()
{
    if (_scriptParser && _scriptParser->hasMoreLines()) {
        _scriptParser->executeNextLine();
    } else {
        CCLOG("End of script reached");
    }
}

void GameScene::setupUI()
{
    // 设置点击事件监听对话层
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [=](Touch* touch, Event* event){
        // 检查是否显示选择菜单
        if (_choiceMenu && _choiceMenu->isVisible()) {
            // 如果选择菜单显示，则不执行脚本
            return true;
        }
        
        // 触发对话系统下一条或执行下一行脚本
        if (_scriptParser && _scriptParser->hasMoreLines()) {
            executeNextScriptLine();
        } else {
            _dialogLayer->nextDialog();
        }
        return true;
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}
