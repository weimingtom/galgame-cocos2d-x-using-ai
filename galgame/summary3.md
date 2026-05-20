# 字体文件说明

## 来源

从 Windows 系统字体目录 `C:\Windows\Fonts\arial.ttf` 复制而来。

## 路径

```
Resources/fonts/arial.ttf
```

与代码中 `DialogueBox.cpp` 和 `ChoiceMenu.cpp` 的引用路径一致：

```cpp
Label::createWithTTF("...", "fonts/arial.ttf", 24);
```

## 为什么用 Arial

- Arial 是 sans-serif 字体，在游戏 UI 中清晰可读
- Windows 系统自带，无需额外下载
- 支持英文、数字和常用符号，满足 demo 脚本的显示需求

## 注意事项

- Arial 不包含中文字形，如需在脚本中使用中文，应替换为支持 CJK 的字体（如 Windows 自带的 `msyh.ttf` 微软雅黑，或开源的 Noto Sans CJK）
- Arial 是 Microsoft 专有字体，仅限 demo/个人使用，公开发布时应替换为 SIL OFL 等开源授权的字体（如 Noto Sans、Liberation Sans、Inter）
- 替换字体时，只需将新的 `.ttf` 文件放入 `Resources/fonts/` 并修改代码中对应的字体文件名即可
