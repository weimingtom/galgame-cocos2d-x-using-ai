#include "AppDelegate.h"
#include "GameScene.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1280, 720);
static cocos2d::Size smallResolutionSize = cocos2d::Size(854, 480);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1280, 720);
static cocos2d::Size largeResolutionSize = cocos2d::Size(1920, 1080);

AppDelegate::AppDelegate() {}
AppDelegate::~AppDelegate() {}

void AppDelegate::initGLContextAttrs() {
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};
    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
//        glview = GLViewImpl::createWithRect("Galgame Engine Demo",
//            Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
        glview = GLViewImpl::createWithRect("Galgame Engine Demo",
            Rect(0, 0, smallResolutionSize.width, smallResolutionSize.height));
        director->setOpenGLView(glview);
    }

    director->setDisplayStats(false);//true);
    director->setAnimationInterval(1.0f / 60.0f);

    glview->setDesignResolutionSize(designResolutionSize.width,
        designResolutionSize.height,
        ResolutionPolicy::SHOW_ALL);

    // Register close handler
    auto* closeItem = MenuItemImage::create(
        "CloseNormal.png", "CloseSelected.png",
        [](Ref*) { Director::getInstance()->end(); });
    if (closeItem) {
        closeItem->setPosition(Vec2(
            designResolutionSize.width - closeItem->getContentSize().width / 2 - 5,
            closeItem->getContentSize().height / 2 + 5));
        // We'll add this in the scene instead
    }

    auto scene = GameScene::createScene();
    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
}
