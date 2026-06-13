#include "BackgroundManager.h"

USING_NS_CC;

/*BackgroundManager* BackgroundManager::create()
{
    BackgroundManager *pRet = new(std::nothrow) BackgroundManager();
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

bool BackgroundManager::init()
{
    if (!Layer::init())
    {
        return false;
    }
//_backgroundSprite = nullptr;
    createDefaultBackground();
//changeBackground("images/bg_school_day.png");
    return true;
}

void BackgroundManager::createDefaultBackground()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
CCLOG("BackgroundManager::createDefaultBackground");    
    // 创建默认背景（纯色背景）
    _backgroundSprite = Sprite::create();
    if (_backgroundSprite == nullptr) {
        // 如果无法加载精灵，则使用DrawNode绘制背景
        auto drawNode = DrawNode::create();
        Color4F bgColor(0.5f, 0.5f, 0.8f, 1.0f); // 淡蓝色背景
        drawNode->drawSolidRect(
            Vec2(origin.x, origin.y),
            Vec2(origin.x + visibleSize.width, origin.y + visibleSize.height),
            bgColor
        );
        this->addChild(drawNode, 0);
        _backgroundSprite = Sprite::create(); // 临时精灵对象
CCLOG("BackgroundManager::createDefaultBackground 1");  
    } else {
        _backgroundSprite->setContentSize(visibleSize);
        _backgroundSprite->setPosition(Vec2(origin.x + visibleSize.width/2, 
                                          origin.y + visibleSize.height/2));
        this->addChild(_backgroundSprite, 0);
CCLOG("BackgroundManager::createDefaultBackground 2");  
    }
    
    _currentBgPath = "";
}

void BackgroundManager::changeBackground(const std::string& bgPath, float transitionDuration)
{
    if (_backgroundSprite) {
CCLOG("BackgroundManager::changeBackground 1");  
#if 0
        // 淡出当前背景
        auto fadeOut = FadeOut::create(transitionDuration / 2);
        
        // 更换背景后的回调
        auto changeTexture = CallFunc::create([this, bgPath]() {
            // 尝试加载新背景
            Texture2D* newTexture = Director::getInstance()->getTextureCache()->addImage(bgPath);
            if (newTexture) {
                _backgroundSprite->setTexture(newTexture);
                _currentBgPath = bgPath;
            } else {
                CCLOG("Could not load background: %s", bgPath.c_str());
            }
        });
        
        // 淡入新背景
        auto fadeIn = FadeIn::create(transitionDuration / 2);
        
        // 执行序列动画
        _backgroundSprite->runAction(Sequence::create(fadeOut, changeTexture, fadeIn, nullptr));
#else
        //if (_backgroundSprite) {
        //	this->removeChild(_backgroundSprite);
	//}
	
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	this->removeAllChildren();
        _backgroundSprite = Sprite::create(bgPath);
        if (_backgroundSprite) {
		_backgroundSprite->setContentSize(visibleSize);
		_backgroundSprite->setPosition(Vec2(origin.x + visibleSize.width/2, 
		                                  origin.y + visibleSize.height/2));
        	this->addChild(_backgroundSprite, 0);
        } else {
		auto drawNode = DrawNode::create();
		Color4F bgColor(0.5f, 0.5f, 0.8f, 1.0f); // 淡蓝色背景
		drawNode->drawSolidRect(
		    Vec2(origin.x, origin.y),
		    Vec2(origin.x + visibleSize.width, origin.y + visibleSize.height),
		    bgColor
		);
		this->addChild(drawNode, 0);	
	}
#endif
    }
}

void BackgroundManager::fadeOut(float duration)
{
    if (_backgroundSprite) {
        auto fadeOut = FadeOut::create(duration);
        _backgroundSprite->runAction(fadeOut);
    }
}

void BackgroundManager::fadeIn(float duration)
{
    if (_backgroundSprite) {
        auto fadeIn = FadeIn::create(duration);
        _backgroundSprite->runAction(fadeIn);
    }
}