from PIL import Image, ImageDraw, ImageFont
import os

output_dir = os.path.join(os.path.dirname(__file__), "Resources", "images")
os.makedirs(output_dir, exist_ok=True)

# ============ 背景图片 (1280x720) ============

def make_gradient_bg(filename, colors, label):
    """生成渐变色背景图片"""
    W, H = 1280, 720
    img = Image.new("RGB", (W, H))
    draw = ImageDraw.Draw(img)
    
    r1, g1, b1 = colors[0]
    r2, g2, b2 = colors[1]
    
    for y in range(H):
        t = y / H
        r = int(r1 + (r2 - r1) * t)
        g = int(g1 + (g2 - g1) * t)
        b = int(b1 + (b2 - b1) * t)
        draw.line([(0, y), (W, y)], fill=(r, g, b))
    
    # 绘制标签文字
    try:
        font = ImageFont.truetype("arial.ttf", 36)
    except:
        font = ImageFont.load_default()
    
    bbox = draw.textbbox((0, 0), label, font=font)
    tw, th = bbox[2] - bbox[0], bbox[3] - bbox[1]
    draw.text((W//2 - tw//2, H//2 - th//2), label, fill=(255, 255, 255), font=font)
    
    # 绘制简单装饰元素
    draw.rounded_rectangle([50, 50, 200, 100], radius=10, fill=(255, 255, 255, 80), outline=(255, 255, 255))
    draw.ellipse([1100, 80, 1230, 200], outline=(255, 255, 255), width=2)
    
    path = os.path.join(output_dir, filename)
    img.save(path)
    print(f"  已生成: {path}")

print("正在生成背景图片...")

make_gradient_bg("bg_school_day.png",
                 [(135, 206, 235), (255, 255, 220)],
                 "School Day - 学校日景")

make_gradient_bg("bg_park_evening.png",
                 [(255, 140, 66), (45, 30, 80)],
                 "Park Evening - 公园傍晚")

make_gradient_bg("bg_cafe_night.png",
                 [(25, 25, 60), (80, 50, 30)],
                 "Cafe Night - 咖啡厅夜晚")


# ============ 角色立绘 (400x700, 透明背景PNG) ============

def make_character(filename, name, body_color, hair_color, skin_color):
    """生成简易角色立绘"""
    W, H = 400, 700
    img = Image.new("RGBA", (W, H), (0, 0, 0, 0))
    draw = ImageDraw.Draw(img)
    
    # 头部（椭圆）
    draw.ellipse([140, 30, 260, 170], fill=skin_color, outline=(0, 0, 0))
    
    # 头发
    draw.ellipse([130, 20, 270, 130], fill=hair_color, outline=(0, 0, 0))
    
    # 眼睛
    draw.ellipse([160, 85, 180, 110], fill=(50, 50, 200))   # 左眼
    draw.ellipse([220, 85, 240, 110], fill=(50, 50, 200))   # 右眼
    draw.ellipse([167, 92, 175, 103], fill=(255, 255, 255))  # 左高光
    draw.ellipse([227, 92, 235, 103], fill=(255, 255, 255))  # 右高光
    
    # 嘴巴
    draw.arc([180, 120, 220, 145], start=0, end=180, fill=(200, 80, 80), width=2)
    
    # 身体（梯形/矩形）
    draw.polygon([(150, 170), (250, 170), (280, 500), (120, 500)], fill=body_color, outline=(0, 0, 0))
    
    # 手臂
    draw.polygon([(150, 200), (100, 380), (120, 390), (170, 220)], fill=body_color, outline=(0, 0, 0))
    draw.polygon([(250, 200), (300, 380), (280, 390), (230, 220)], fill=body_color, outline=(0, 0, 0))
    
    # 手
    draw.ellipse([85, 370, 130, 410], fill=skin_color, outline=(0, 0, 0))
    draw.ellipse([270, 370, 315, 410], fill=skin_color, outline=(0, 0, 0))
    
    # 腿
    draw.polygon([(140, 500), (180, 500), (175, 660), (130, 660)], fill=(60, 60, 100), outline=(0, 0, 0))
    draw.polygon([(220, 500), (260, 500), (270, 660), (225, 660)], fill=(60, 60, 100), outline=(0, 0, 0))
    
    # 鞋子
    draw.rounded_rectangle([120, 655, 185, 690], radius=8, fill=(50, 50, 50), outline=(0, 0, 0))
    draw.rounded_rectangle([215, 655, 280, 690], radius=8, fill=(50, 50, 50), outline=(0, 0, 0))
    
    # 角色名字标签
    try:
        font = ImageFont.truetype("arial.ttf", 18)
    except:
        font = ImageFont.load_default()
    bbox = draw.textbbox((0, 0), name, font=font)
    tw = bbox[2] - bbox[0]
    draw.text((W//2 - tw//2, 670), name, fill=(0, 0, 0), font=font)
    
    path = os.path.join(output_dir, filename)
    img.save(path)
    print(f"  已生成: {path}")


print("\n正在生成角色立绘...")

make_character("alice_normal.png", "Alice",
               body_color=(255, 180, 200),    # 粉色上衣
               hair_color=(180, 120, 60),     # 棕色头发
               skin_color=(255, 225, 200))    # 肤色

make_character("bob_normal.png", "Bob",
               body_color=(100, 150, 220),    # 蓝色上衣
               hair_color=(60, 40, 30),       # 深棕色头发
               skin_color=(245, 215, 190))    # 肤色

print("\n所有图片生成完成！")
print(f"输出目录: {output_dir}")
