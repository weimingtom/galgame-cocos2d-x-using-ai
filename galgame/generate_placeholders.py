"""Generate placeholder images for Galgame demo using Pillow."""
import os
from PIL import Image, ImageDraw

RES = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'Resources')
os.makedirs(RES, exist_ok=True)


def create_bg(filename, width, height, top_color, bot_color):
    """Vertical gradient background with a simple room scene."""
    img = Image.new('RGB', (width, height))
    for y in range(height):
        t = y / (height - 1)
        r = int(top_color[0] + (bot_color[0] - top_color[0]) * t)
        g = int(top_color[1] + (bot_color[1] - top_color[1]) * t)
        b = int(top_color[2] + (bot_color[2] - top_color[2]) * t)
        for x in range(width):
            img.putpixel((x, y), (r, g, b))

    draw = ImageDraw.Draw(img)
    # Floor line
    floor_y = int(height * 0.72)
    draw.rectangle([0, floor_y, width, height], fill=(140, 120, 95))
    # Floor board lines
    for i in range(6):
        x = int(width * i / 5)
        draw.line([x, floor_y, x, height], fill=(120, 100, 80), width=2)
    # Window on back wall
    win_x1 = int(width * 0.35)
    win_x2 = int(width * 0.65)
    win_y1 = int(height * 0.10)
    win_y2 = int(height * 0.50)
    draw.rectangle([win_x1, win_y1, win_x2, win_y2], fill=(135, 200, 235), outline=(100, 70, 40), width=4)
    # Window cross
    draw.line([(win_x1 + win_x2)//2, win_y1, (win_x1 + win_x2)//2, win_y2], fill=(100, 70, 40), width=3)
    draw.line([win_x1, (win_y1 + win_y2)//2, win_x2, (win_y1 + win_y2)//2], fill=(100, 70, 40), width=3)
    # Blackboard
    bx1 = int(width * 0.10)
    bx2 = int(width * 0.30)
    by1 = int(height * 0.20)
    by2 = int(height * 0.55)
    draw.rectangle([bx1, by1, bx2, by2], fill=(30, 50, 30), outline=(80, 60, 30), width=4)
    # Desk
    draw.rectangle([width//4, floor_y - 30, width*3//4, floor_y], fill=(160, 130, 100))

    path = os.path.join(RES, filename)
    img.save(path)
    print(f"  {path} ({width}x{height})")


def create_character(filename, width, height, body_color, hair_color, outfit_color):
    """Simple anime-style character placeholder with transparent background."""
    img = Image.new('RGBA', (width, height), (0, 0, 0, 0))
    draw = ImageDraw.Draw(img)
    cx, cy = width // 2, height // 2

    # Shadow/outline helper
    def draw_ellipse_filled(xy, fill, outline=None):
        if outline:
            x0, y0, x1, y1 = xy
            draw.ellipse([x0-2, y0-2, x1+2, y1+2], fill=outline)
        draw.ellipse(xy, fill=fill)

    # Body
    body_x0 = int(width * 0.25)
    body_x1 = int(width * 0.75)
    body_y0 = int(height * 0.50)
    body_y1 = int(height * 0.92)
    draw_ellipse_filled([body_x0, body_y0, body_x1, body_y1], outfit_color, (40, 40, 40, 255))

    # Neck
    neck_x0 = cx - 6
    neck_x1 = cx + 6
    draw.rectangle([neck_x0, int(height * 0.44), neck_x1, int(height * 0.54)], fill=(255, 210, 170, 255))

    # Head
    head_rx = int(width * 0.28)
    head_ry = int(height * 0.20)
    head_cx = cx
    head_cy = int(height * 0.26)
    draw_ellipse_filled(
        [head_cx - head_rx, head_cy - head_ry, head_cx + head_rx, head_cy + head_ry],
        (255, 220, 177, 255), (40, 40, 40, 255)
    )

    # Hair (covers top of head)
    hair_pts = []
    for y in range(0, head_cy + head_ry):
        for x in range(head_cx - head_rx - 6, head_cx + head_rx + 6):
            dx = (x - head_cx) / (head_rx + 6)
            dy = (y - head_cy) / (head_ry + 6)
            if dx*dx + dy*dy <= 1.05:
                img.putpixel((x, y), (*hair_color, 255))

    # Eyes
    eye_y = int(height * 0.24)
    eye_r = int(width * 0.05)
    for ex in [cx - int(width * 0.09), cx + int(width * 0.09)]:
        draw.ellipse([ex - eye_r, eye_y - eye_r//2, ex + eye_r, eye_y + eye_r//2], fill=(255, 255, 255, 255))
        draw.ellipse([ex - eye_r//2, eye_y - eye_r//2, ex + eye_r//2, eye_y + eye_r//2], fill=(30, 30, 50, 255))
        # Highlight
        draw.ellipse([ex - 3, eye_y - 3, ex + 2, eye_y + 1], fill=(255, 255, 255, 255))

    # Mouth
    mouth_y = int(height * 0.30)
    draw.arc([cx - 5, mouth_y - 2, cx + 5, mouth_y + 4], 0, 180, fill=(200, 120, 100, 255), width=1)

    # Blush
    blush_y = int(height * 0.27)
    for bx, bdir in [(cx - int(width * 0.12), -1), (cx + int(width * 0.12), 1)]:
        draw.ellipse([bx - 8, blush_y - 4, bx + 8, blush_y + 4], fill=(255, 150, 150, 80))

    path = os.path.join(RES, filename)
    img.save(path)
    print(f"  {path} ({width}x{height})")


def create_character_hopeful(filename, width, height, body_color, hair_color, outfit_color):
    """Happy/hopeful variant — brighter eyes, bigger smile, slightly tilted."""
    img = Image.new('RGBA', (width, height), (0, 0, 0, 0))
    draw = ImageDraw.Draw(img)
    cx, cy = width // 2, height // 2

    def draw_ellipse_filled(xy, fill, outline=None):
        if outline:
            x0, y0, x1, y1 = xy
            draw.ellipse([x0-2, y0-2, x1+2, y1+2], fill=outline)
        draw.ellipse(xy, fill=fill)

    # Body
    body_x0 = int(width * 0.25)
    body_x1 = int(width * 0.75)
    body_y0 = int(height * 0.50)
    body_y1 = int(height * 0.92)
    draw_ellipse_filled([body_x0, body_y0, body_x1, body_y1], outfit_color, (40, 40, 40, 255))

    # Add a ribbon/bow
    bow_x = cx - int(width * 0.06)
    bow_y = int(height * 0.49)
    draw.ellipse([bow_x - 8, bow_y - 10, bow_x + 8, bow_y + 10], fill=(255, 100, 100, 255))

    # Neck
    neck_x0 = cx - 6
    neck_x1 = cx + 6
    draw.rectangle([neck_x0, int(height * 0.44), neck_x1, int(height * 0.54)], fill=(255, 210, 170, 255))

    # Head
    head_rx = int(width * 0.28)
    head_ry = int(height * 0.20)
    head_cx = cx
    head_cy = int(height * 0.26)
    draw_ellipse_filled(
        [head_cx - head_rx, head_cy - head_ry, head_cx + head_rx, head_cy + head_ry],
        (255, 220, 177, 255), (40, 40, 40, 255)
    )

    # Hair
    for y in range(0, head_cy + head_ry):
        for x in range(head_cx - head_rx - 6, head_cx + head_rx + 6):
            dx = (x - head_cx) / (head_rx + 6)
            dy = (y - head_cy) / (head_ry + 6)
            if dx*dx + dy*dy <= 1.05:
                img.putpixel((x, y), (*hair_color, 255))

    # Sparkle eyes (bigger)
    eye_y = int(height * 0.24)
    eye_r = int(width * 0.06)
    for ex in [cx - int(width * 0.09), cx + int(width * 0.09)]:
        draw.ellipse([ex - eye_r, eye_y - eye_r//2, ex + eye_r, eye_y + eye_r//2], fill=(255, 255, 255, 255))
        draw.ellipse([ex - eye_r//2, eye_y - eye_r//2, ex + eye_r//2, eye_y + eye_r//2], fill=(60, 80, 50, 255))
        # Big sparkle
        draw.ellipse([ex - 4, eye_y - 4, ex + 3, eye_y + 2], fill=(255, 255, 255, 255))
        draw.ellipse([ex + 2, eye_y - 2, ex + 5, eye_y], fill=(255, 255, 255, 200))

    # Big smile
    mouth_y = int(height * 0.30)
    draw.arc([cx - 7, mouth_y - 3, cx + 7, mouth_y + 5], 0, 180, fill=(200, 100, 80, 255), width=2)

    # Strong blush
    blush_y = int(height * 0.27)
    for bx in [cx - int(width * 0.12), cx + int(width * 0.12)]:
        draw.ellipse([bx - 10, blush_y - 5, bx + 10, blush_y + 5], fill=(255, 130, 130, 100))

    path = os.path.join(RES, filename)
    img.save(path)
    print(f"  {path} ({width}x{height})")


if __name__ == '__main__':
    print(f"Generating placeholder images to: {RES}\n")

    # Background: classroom
    create_bg('bg_classroom.png', 1280, 720,
              top_color=(160, 145, 130),
              bot_color=(210, 195, 175))

    # Sakura (normal) — pink outfit, brown hair
    create_character('char_sakura_normal.png', 400, 600,
                     body_color=(255, 210, 170),
                     hair_color=(130, 90, 60),
                     outfit_color=(230, 150, 160))

    # Sakura (hopeful) — brighter, happier
    create_character_hopeful('char_sakura_hopeful.png', 400, 600,
                             body_color=(255, 210, 170),
                             hair_color=(130, 90, 60),
                             outfit_color=(250, 170, 180))

    # MC — blue outfit, dark hair
    create_character('char_mc_normal.png', 400, 600,
                     body_color=(255, 215, 175),
                     hair_color=(45, 45, 60),
                     outfit_color=(100, 140, 200))

    print("\nDone! 4 placeholder images generated.")
