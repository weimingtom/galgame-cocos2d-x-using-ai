# GalGame引擎 - 基于Cocos2D-X

这是一个基于Cocos2D-X开发的简单GalGame（视觉小说）引擎，包含了GalGame所需的核心功能。

## 功能特性

### 1. 对话系统
- 支持多角色对话
- 角色名称显示
- 文本显示效果

### 2. 角色管理系统
- 角色立绘显示
- 角色位置控制
- 淡入淡出效果
- 角色移动动画

### 3. 背景管理系统
- 背景切换
- 平滑过渡效果
- 支持多种背景图片

### 4. 选择分支系统
- 多选项支持
- 选择结果处理
- 影响剧情走向

### 5. 存档/读档系统
- 多槽位存档
- 游戏状态保存
- 快速读档功能

## 架构说明

### 核心组件

#### GameScene
游戏主场景，协调各个子系统的工作。

#### DialogLayer
负责显示对话文本和角色名称。

#### DialogManager
管理对话脚本和对话流程。

#### CharacterManager
管理角色立绘的显示和动画。

#### BackgroundManager
管理背景图片的切换和特效。

#### ChoiceMenuLayer
处理玩家的选择操作。

#### SaveGameManager
提供存档和读档功能。

### 数据结构

#### GameData命名空间
包含所有游戏数据结构：
- DialogData: 对话数据
- CharacterData: 角色数据
- SaveData: 存档数据

## 使用方法

1. 创建GameScene实例
2. 通过startGame()开始游戏
3. 使用键盘快捷键进行存档/读档:
   - F1: 快速存档
   - F2: 快速读档

## 扩展功能

此引擎提供了良好的扩展性，开发者可以：

- 扩展对话脚本格式
- 添加更多的角色表情
- 实现更复杂的分支逻辑
- 添加音效和音乐系统
- 实现自动播放模式

## 项目结构

```
Classes/
├── AppDelegate.*         # 应用程序入口
├── GameScene.*           # 主游戏场景
├── DialogLayer.*         # 对话层
├── DialogManager.*       # 对话管理器
├── CharacterManager.*    # 角色管理器
├── BackgroundManager.*   # 背景管理器
├── ChoiceMenuLayer.*     # 选择菜单层
├── SaveGameManager.*     # 存档管理器
└── GameData.h            # 游戏数据结构定义
Resources/
└── sample_script.txt     # 示例对话脚本
```

## 总结

这个GalGame引擎提供了制作视觉小说游戏所需的基础功能，具有良好的模块化设计，便于扩展和维护。