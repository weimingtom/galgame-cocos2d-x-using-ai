#ifndef _CHOICE_MENU_H_
#define _CHOICE_MENU_H_

#include "cocos2d.h"
#include <string>
#include <vector>
#include <functional>

class ChoiceMenu : public cocos2d::Node {
public:
    using ChoiceCallback = std::function<void(int index, const std::string& label)>;

    static ChoiceMenu* create();
    virtual bool init() override;

    void addChoice(const std::string& text, const std::string& jumpLabel);
    void show(ChoiceCallback callback);
    void hide();

private:
    cocos2d::DrawNode* _bg = nullptr;
    cocos2d::Menu* _menu = nullptr;
    std::vector<std::string> _jumpLabels;
    std::vector<std::string> _choiceTexts;
};

#endif
