# Galgame Engine Demo — Cocos2d-x C++ Visual Novel Engine

## Project structure

```
galgame/
├── CMakeLists.txt
├── Classes/
│   ├── AppDelegate.h / .cpp       — Application entry point
│   ├── GameScene.h / .cpp         — Main scene, orchestrates everything
│   ├── ScriptEngine.h / .cpp      — Parses .txt scripts and drives flow
│   ├── DialogueBox.h / .cpp       — Typewriter-effect text rendering
│   ├── CharacterSprite.h / .cpp   — Character sprites with fade in/out
│   └── ChoiceMenu.h / .cpp        — Player choice selection UI
└── Resources/
    └── scripts/
        └── chapter1.txt           — Demo script (visual novel scenario)
```

## Architecture

**ScriptEngine** — Parses a simple command-based `.txt` script format. Supports: `BG`, `CHAR`, `CHAR_HIDE`, `TEXT`, `CHOICE`/`CHOICES`, `LABEL`/`JUMP`, `WAIT`, `END`. Labels enable branching narratives via the choice system.

**DialogueBox** — Bottom-of-screen text panel with:
- Typewriter effect (configurable speed, 50ms per character default)
- Speaker name with auto-colored label
- Blinking "▼" indicator when text finishes
- Tap-to-skip-to-end, tap-again-to-advance

**CharacterSprite** — Character rendering with:
- Named character slots (`id` for referencing)
- Position presets: `left`, `center`, `right`, `farleft`, `farright`
- Auto-scaling to 60% of screen height
- Crossfade transitions on sprite change or hide

**ChoiceMenu** — Full-screen overlay with:
- Semi-transparent dark backdrop
- Vertically stacked choice buttons
- Fade-in animation
- Returns selection to GameScene for branching via `JUMP`

**GameScene** — Central coordinator:
- Listens for taps to advance dialogue
- Accumulates consecutive `CHOICE` commands, then displays them together
- Chains auto-advancing commands (BG/CHAR/JUMP/WAIT) until hitting interactive ones

## Script format (chapter1.txt)

```
BG: bg_classroom.png
CHAR: sakura char_sakura_normal.png center
TEXT: Sakura "Good morning!"
CHOICE: "Reply warmly" -> warm
CHOICE: "Stay silent" -> cold
CHOICES:
LABEL: warm
TEXT: Sakura "Thanks for being nice!"
JUMP: end
LABEL: cold
TEXT: Narrator "Awkward silence..."
LABEL: end
END:
```

## How to build

Set `COCOS2D_ROOT` to your cocos2d-x installation path in `CMakeLists.txt`, then:

```bash
cd galgame
mkdir build && cd build
cmake .. -DCOCOS2D_ROOT=/path/to/cocos2d-x
make
```

The engine expects image resources referenced in the script (backgrounds, character sprites) to exist under `Resources/`. When they're missing, the engine falls back to colored placeholders so the script logic remains testable without art assets.
