#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "cocos2d.h"
#include "ScriptEngine.h"
#include "DialogueBox.h"
#include "CharacterSprite.h"
#include "ChoiceMenu.h"
#include <unordered_map>

class GameScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(GameScene);

private:
    void startGame();
    void processCommand(const ScriptCommand& cmd);
    void advanceScript();
    void onChoiceSelected(int index, const std::string& jumpLabel);

    // State
    ScriptEngine _engine;
    DialogueBox* _dialogueBox = nullptr;
    ChoiceMenu* _choiceMenu = nullptr;
    cocos2d::Sprite* _background = nullptr;
    std::unordered_map<std::string, CharacterSprite*> _characters;

    // Choice accumulation
    std::vector<std::string> _pendingChoiceTexts;
    std::vector<std::string> _pendingChoiceLabels;
};

#endif
