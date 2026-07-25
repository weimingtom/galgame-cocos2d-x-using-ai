#include "DialogueBox.h"

USING_NS_CC;

bool DialogueBox::init()
{
    if (!Node::init()) return false;

    const auto visible = Director::getInstance()->getVisibleSize();
    const auto origin = Director::getInstance()->getVisibleOrigin();

    const float boxH = 180.f;
    const float margin = 24.f;

    _panel = LayerColor::create(Color4B(20, 24, 40, 210), visible.width - margin * 2, boxH);
    _panel->setPosition(Vec2(origin.x + margin, origin.y + 28.f));
    addChild(_panel);

    // Soft top border line
    auto line = LayerColor::create(Color4B(120, 180, 255, 160), visible.width - margin * 2, 2.f);
    line->setPosition(Vec2(0, boxH - 2.f));
    _panel->addChild(line);

    _nameLabel = Label::createWithSystemFont("", "Microsoft YaHei", 26);
    _nameLabel->setAnchorPoint(Vec2(0, 1));
    _nameLabel->setPosition(Vec2(20, boxH - 14));
    _nameLabel->setTextColor(Color4B(160, 210, 255, 255));
    _panel->addChild(_nameLabel);

    _textLabel = Label::createWithSystemFont("", "Microsoft YaHei", 24);
    _textLabel->setAnchorPoint(Vec2(0, 1));
    _textLabel->setPosition(Vec2(20, boxH - 52));
    _textLabel->setDimensions(visible.width - margin * 2 - 40, boxH - 70);
    _textLabel->setAlignment(TextHAlignment::LEFT, TextVAlignment::TOP);
    _textLabel->setTextColor(Color4B(245, 245, 250, 255));
    _panel->addChild(_textLabel);

    _hintLabel = Label::createWithSystemFont("▼ 点击继续", "Microsoft YaHei", 16);
    _hintLabel->setAnchorPoint(Vec2(1, 0));
    _hintLabel->setPosition(Vec2(visible.width - margin * 2 - 16, 10));
    _hintLabel->setTextColor(Color4B(180, 190, 210, 180));
    _hintLabel->setVisible(false);
    _panel->addChild(_hintLabel);

    setCascadeOpacityEnabled(true);
    schedule([this](float dt) { updateTyping(dt); }, "dlg_typing");
    return true;
}

void DialogueBox::showLine(const std::string& speaker, const std::string& text, bool isNarration)
{
    showBox();
    _fullText = text;
    _visibleChars = 0;
    _accum = 0.f;
    _typing = true;
    _hintLabel->setVisible(false);

    if (isNarration || speaker.empty()) {
        _nameLabel->setString("");
        _textLabel->setTextColor(Color4B(210, 215, 230, 255));
    } else {
        _nameLabel->setString(speaker);
        _textLabel->setTextColor(Color4B(245, 245, 250, 255));
    }
    _textLabel->setString("");
}

void DialogueBox::updateTyping(float dt)
{
    if (!_typing) return;
    _accum += dt * _cps;
    while (_accum >= 1.f && _visibleChars < _fullText.size()) {
        _accum -= 1.f;
        // Advance by UTF-8 codepoint
        unsigned char c = static_cast<unsigned char>(_fullText[_visibleChars]);
        if ((c & 0x80) == 0) {
            ++_visibleChars;
        } else if ((c & 0xE0) == 0xC0) {
            _visibleChars = std::min(_visibleChars + 2, _fullText.size());
        } else if ((c & 0xF0) == 0xE0) {
            _visibleChars = std::min(_visibleChars + 3, _fullText.size());
        } else {
            _visibleChars = std::min(_visibleChars + 4, _fullText.size());
        }
        _textLabel->setString(_fullText.substr(0, _visibleChars));
    }
    if (_visibleChars >= _fullText.size()) {
        finishTyping();
    }
}

void DialogueBox::finishTyping()
{
    _typing = false;
    _textLabel->setString(_fullText);
    _hintLabel->setVisible(true);
    if (onLineFinished) onLineFinished();
}

void DialogueBox::skipTyping()
{
    if (!_typing) return;
    _visibleChars = _fullText.size();
    finishTyping();
}

void DialogueBox::hideBox()
{
    setVisible(false);
}

void DialogueBox::showBox()
{
    setVisible(true);
}
