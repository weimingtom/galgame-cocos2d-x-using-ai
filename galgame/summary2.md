# 占位图片生成说明

## 生成方式

使用 Pillow (PIL) 库通过 `generate_placeholders.py` 脚本生成，运行命令：

```bash
python generate_placeholders.py
```

## 生成的文件

| 文件名 | 尺寸 | 类型 | 说明 |
|---|---|---|---|
| `bg_classroom.png` | 1280×720 | 背景 | 教室场景：渐变墙壁 + 地板 + 窗户 + 黑板 + 课桌 |
| `char_sakura_normal.png` | 400×600 | 角色 | 樱（普通表情）：粉色上衣，棕色头发，透明背景 |
| `char_sakura_hopeful.png` | 400×600 | 角色 | 樱（期待表情）：更亮粉色上衣 + 蝴蝶结，闪闪大眼，灿烂笑容，透明背景 |
| `char_mc_normal.png` | 400×600 | 角色 | 主角：蓝色上衣，深色头发，透明背景 |

## 角色图结构

每张角色占位图包含以下绘制元素：

- **身体**：椭圆形的服装区域，带深色轮廓
- **颈部**：肤色矩形连接头部与身体
- **头部**：肤色椭圆，带深色轮廓
- **头发**：覆盖头部上半部分的弧形区域
- **眼睛**：白色椭圆 + 深色瞳孔 + 白色高光点
- **嘴巴**：弧线
- **腮红**：半透明粉色椭圆

`char_sakura_hopeful.png` 相比普通版额外包含：
- 更大的闪闪发亮的眼睛
- 更灿烂的弧形笑容
- 胸前的红色蝴蝶结

## 与脚本的对应关系

`chapter1.txt` 中引用的图片：

```
BG: bg_classroom.png                          # 教室背景
CHAR: sakura char_sakura_normal.png center     # 樱普通表情
CHAR: mc char_mc_normal.png left               # 主角
CHAR: sakura char_sakura_hopeful.png center    # 樱期待表情
```
