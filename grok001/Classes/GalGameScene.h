#pragma once

#include "cocos2d.h"
#include "Engine/GalGameEngine.h"
#include "UI/DialogueBox.h"
#include "UI/ChoicePanel.h"
#include "UI/CharacterSprite.h"
#include <map>
#include <string>

class GalGameScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    CREATE_FUNC(GalGameScene);
    bool init() override;

private:
    void bindEngine();
    void onTap(cocos2d::Ref* sender);
    void setBackground(const std::string& name);
    void showCharacter(const std::string& name, const std::string& emotion, const std::string& position);
    void hideCharacter(const std::string& name);
    cocos2d::Vec2 positionFor(const std::string& position) const;

    gal::GalGameEngine _engine;
    DialogueBox* _dialogue = nullptr;
    ChoicePanel* _choices = nullptr;
    cocos2d::LayerColor* _bg = nullptr;
    cocos2d::Label* _bgLabel = nullptr;
    cocos2d::Label* _titleLabel = nullptr;
    cocos2d::Node* _charLayer = nullptr;
    std::map<std::string, CharacterSprite*> _characters;
};
