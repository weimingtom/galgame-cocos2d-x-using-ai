#include "CharacterManager.h"

USING_NS_CC;

/*CharacterManager* CharacterManager::create()
{
    CharacterManager *pRet = new(std::nothrow) CharacterManager();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}*/

bool CharacterManager::init()
{
    if (!Layer::init())
    {
        return false;
    }

    loadCharacterData();
    return true;
}

void CharacterManager::loadCharacterData()
{
    // 这里可以加载配置文件或预设角色数据
    // 示例：添加一些虚拟角色数据
    GameData::CharacterData character;
    character.id = "player";
    character.name = "Player";
    character.defaultSprite = "player_default.png";
    _characterData[character.id] = character;
}

void CharacterManager::addCharacter(const std::string& charId, const std::string& spritePath, 
                                   const Vec2& position)
{
    // 先移除同名角色（如果存在）
    if (_characters.find(charId) != _characters.end()) {
        this->removeChild(_characters[charId]);
        _characters.erase(charId);
    }
    
    // 创建新的角色精灵
    auto characterSprite = Sprite::create(spritePath);
    if (characterSprite) {
        characterSprite->setPosition(position);
        characterSprite->setOpacity(0); // 初始透明
        this->addChild(characterSprite);
        _characters[charId] = characterSprite;
        
        // 执行淡入效果
        auto fadeInAction = FadeIn::create(0.5f);
        characterSprite->runAction(fadeInAction);
    }
}

void CharacterManager::removeCharacter(const std::string& charId)
{
    if (_characters.find(charId) != _characters.end()) {
        auto characterSprite = _characters[charId];
        auto fadeOutAction = FadeOut::create(0.5f);
        
        // 淡出后移除精灵
        auto removeFromParent = CallFunc::create([this, charId, characterSprite]() {
            this->removeChild(characterSprite);
            _characters.erase(charId);
        });
        
        characterSprite->runAction(Sequence::create(fadeOutAction, removeFromParent, nullptr));
    }
}

void CharacterManager::setExpression(const std::string& charId, const std::string& expression)
{
    // 在实际实现中，这里会根据表情标识切换不同的纹理
    // 暂时留空实现
    CCLOG("Set expression %s for character %s", expression.c_str(), charId.c_str());
}

void CharacterManager::moveTo(const std::string& charId, const Vec2& newPosition, float duration)
{
    if (_characters.find(charId) != _characters.end()) {
        auto moveAction = MoveTo::create(duration, newPosition);
        _characters[charId]->runAction(moveAction);
    }
}

void CharacterManager::fadeIn(const std::string& charId, float duration)
{
    if (_characters.find(charId) != _characters.end()) {
        auto fadeInAction = FadeIn::create(duration);
        _characters[charId]->runAction(fadeInAction);
    }
}

void CharacterManager::fadeOut(const std::string& charId, float duration)
{
    if (_characters.find(charId) != _characters.end()) {
        auto fadeOutAction = FadeOut::create(duration);
        _characters[charId]->runAction(fadeOutAction);
    }
}