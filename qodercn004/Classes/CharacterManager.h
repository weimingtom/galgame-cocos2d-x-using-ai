#ifndef __CHARACTER_MANAGER_H__
#define __CHARACTER_MANAGER_H__

#include "cocos2d.h"
#include "GameData.h"

/**
 * @brief 角色管理器类
 * 管理游戏中出现的角色立绘、位置、动画等
 */
class CharacterManager : public cocos2d::Layer
{
public:
    //static CharacterManager* create();
    virtual bool init();
    
    CREATE_FUNC(CharacterManager);

    /**
     * @brief 添加角色到场景
     * @param charId 角色ID
     * @param spritePath 立绘资源路径
     * @param position 显示位置
     */
    void addCharacter(const std::string& charId, const std::string& spritePath, 
                     const cocos2d::Vec2& position);
    
    /**
     * @brief 移除角色
     * @param charId 角色ID
     */
    void removeCharacter(const std::string& charId);
    
    /**
     * @brief 显示角色的不同表情
     * @param charId 角色ID
     * @param expression 表情标识
     */
    void setExpression(const std::string& charId, const std::string& expression);
    
    /**
     * @brief 移动角色到新位置
     * @param charId 角色ID
     * @param newPosition 新位置
     * @param duration 动画持续时间
     */
    void moveTo(const std::string& charId, const cocos2d::Vec2& newPosition, float duration = 0.5f);
    
    /**
     * @brief 淡入角色
     * @param charId 角色ID
     * @param duration 动画持续时间
     */
    void fadeIn(const std::string& charId, float duration = 0.5f);
    
    /**
     * @brief 淡出角色
     * @param charId 角色ID
     * @param duration 动画持续时间
     */
    void fadeOut(const std::string& charId, float duration = 0.5f);

private:
    std::map<std::string, cocos2d::Sprite*> _characters; // 角色精灵映射
    std::map<std::string, GameData::CharacterData> _characterData; // 角色数据
    
    void loadCharacterData(); // 加载角色配置数据
};

#endif // __CHARACTER_MANAGER_H__