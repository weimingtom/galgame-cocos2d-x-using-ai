#ifndef __BACKGROUND_MANAGER_H__
#define __BACKGROUND_MANAGER_H__

#include "cocos2d.h"
#include "GameData.h"

/**
 * @brief 背景管理器类
 * 管理游戏中场景的背景图片切换、特效等
 */
class BackgroundManager : public cocos2d::Layer
{
public:
    //static BackgroundManager* create();
    virtual bool init();
    
    CREATE_FUNC(BackgroundManager);

    /**
     * @brief 更改背景图片
     * @param bgPath 背景图片路径
     * @param transitionDuration 过渡动画持续时间
     */
    void changeBackground(const std::string& bgPath, float transitionDuration = 1.0f);
    
    /**
     * @brief 获取当前背景图片路径
     */
    std::string getCurrentBackground() const { return _currentBgPath; }
    
    /**
     * @brief 淡出当前背景
     * @param duration 持续时间
     */
    void fadeOut(float duration = 1.0f);
    
    /**
     * @brief 淡入当前背景
     * @param duration 持续时间
     */
    void fadeIn(float duration = 1.0f);

private:
    cocos2d::Sprite* _backgroundSprite; // 背景精灵
    std::string _currentBgPath;         // 当前背景路径
    
    void createDefaultBackground();     // 创建默认背景
};

#endif // __BACKGROUND_MANAGER_H__