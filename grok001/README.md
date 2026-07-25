# Cocos2d-x GalGame 引擎 Demo

基于 **cocos2d-x 4.0** 的轻量 GalGame（视觉小说）引擎示例。支持 JSON 剧本、打字机对话框、角色登场/退场、分支选项与多结局。

## 功能

| 能力 | 说明 |
|------|------|
| JSON 剧本 | `bg` / `show` / `hide` / `say` / `narrate` / `choice` / `jump` / `wait` / `clear` / `end` |
| 对话框 | UTF-8 打字机效果，点击跳过 |
| 选项 | 多分支跳转 |
| 角色 | 无立绘时用程序化色块占位（可换成真实 PNG） |
| Demo | 《夏日回廊》短篇，含 Good / Normal 结局 |

## 目录

```
Classes/
  Engine/          # 剧本解析与执行
  UI/              # 对话框、选项、角色占位
  GalGameScene.*   # 主场景
  AppDelegate.*
Resources/scripts/demo.json
proj.win32/        # Windows 入口
proj.linux/        # Linux 入口
proj.ios_mac/mac/  # macOS 入口
```

## 环境准备

1. 安装 [CMake](https://cmake.org/) ≥ 3.15、Visual Studio 2019/2022（含 C++ 桌面开发）
2. 获取 [cocos2d-x 4.0](https://github.com/cocos2d/cocos2d-x/releases)，任选其一：
   - 解压到本仓库的 `cocos2d/` 目录（需存在 `cocos2d/CMakeLists.txt`）
   - 或设置环境变量 `COCOS2DX_ROOT` 指向引擎根目录

首次使用引擎请先按官方说明拉取第三方依赖（`download-deps.py`）。

## 编译（Windows）

```bat
build_win.bat
```

或手动：

```bat
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A x64
cmake --build . --config Release
```

也可指定引擎路径：

```bat
cmake .. -DCOCOS2DX_ROOT_PATH=D:\path\to\cocos2d-x -G "Visual Studio 17 2022" -A x64
```

## 游玩

运行生成的 `GalGameDemo.exe`，点击推进对话；出现选项时点选分支。结束后再点可重开。

## 编写剧本

编辑 `Resources/scripts/demo.json`：

```json
{
  "title": "我的故事",
  "start": "intro",
  "labels": {
    "intro": [
      { "type": "bg", "name": "classroom" },
      { "type": "show", "character": "角色A", "emotion": "微笑", "position": "center" },
      { "type": "say", "speaker": "角色A", "text": "你好。" },
      {
        "type": "choice",
        "options": [
          { "text": "选项一", "goto": "a" },
          { "text": "选项二", "goto": "b" }
        ]
      }
    ],
    "a": [
      { "type": "narrate", "text": "结局 A" },
      { "type": "end" }
    ],
    "b": [
      { "type": "narrate", "text": "结局 B" },
      { "type": "end" }
    ]
  }
}
```

### 指令一览

- `bg` — 切换背景（`name`）
- `show` — 显示角色（`character` / `emotion` / `position`: left|center|right）
- `hide` — 隐藏角色
- `say` — 对白（`speaker` + `text`）
- `narrate` — 旁白
- `choice` — 选项数组（`text` + `goto`）
- `jump` — 跳转到 label（`goto`）
- `wait` — 等待秒数（`duration`）
- `clear` — 清空角色与 UI
- `end` — 结束

## 接入真实立绘

当前角色为程序化色块。可在 `GalGameScene::showCharacter` 中改为加载：

`Resources/characters/<name>_<emotion>.png`

背景同理：`Resources/backgrounds/<name>.png`。

## 架构简述

```
JSON 剧本 → ScriptParser → GalGameEngine（指令队列）
                              ↓ callbacks
                    GalGameScene / DialogueBox / ChoicePanel
```

引擎与渲染解耦：换 UI 或换脚本格式时，主要改 Parser / Scene 即可。
