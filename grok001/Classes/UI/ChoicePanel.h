#pragma once

#include "cocos2d.h"
#include "Engine/ScriptCommand.h"
#include <functional>
#include <vector>

class ChoicePanel : public cocos2d::Node {
public:
    CREATE_FUNC(ChoicePanel);
    bool init() override;

    void showChoices(const std::vector<gal::ChoiceOption>& options);
    void hideChoices();

    std::function<void(int index)> onSelected;

private:
    cocos2d::Node* _container = nullptr;
};
