#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "DialogLayer.h"
#include "CharacterManager.h"
#include "BackgroundManager.h"
#include "ChoiceMenuLayer.h"
#include "SaveGameManager.h"
#include "ScriptParser.h"

/**
 * @brief 游戏主场景类
 * 管理整个游戏流程、场景切换等
 */
class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    // 实现CREATE_FUNC宏
    CREATE_FUNC(GameScene);
    
    // 提供接口给外部控制
    void startGame();
    void saveGame(int slot);
    void loadGame(int slot);
    void showSaveLoadMenu();
    
    // 脚本执行回调
    void onBackgroundChange(const std::string& bgName);
    void onCharacterAction(const std::string& charId, const std::string& spritePath, const std::string& position);
    void onDialogShow(const std::string& speaker, const std::string& text);
    void onChoiceShow(const std::vector<std::string>& choices, std::function<void(int)> callback);
    void onWaitAction();

private:
    DialogLayer* _dialogLayer;      // 对话层
    CharacterManager* _charManager; // 角色管理器
    BackgroundManager* _bgManager;  // 背景管理器
    ChoiceMenuLayer* _choiceMenu;   // 选择菜单层
    ScriptParser* _scriptParser;    // 脚本解析器
    
    void initializeComponents();    // 初始化组件
    void setupUI();                 // 设置UI界面
    void setupSaveLoadUI();         // 设置存档/读档UI
    void setupScriptCallbacks();    // 设置脚本回调函数
    void onChoiceMade(int choiceIndex); // 选择结果处理
    void executeNextScriptLine();   // 执行下一行脚本
};

#endif // __GAME_SCENE_H__