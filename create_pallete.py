from PIL import Image

def hsv_to_rgb(h, s, v):
    if h == 360:
        h = 0
    c = v * s # chroma
    x = c * (1 - abs((h / 60) % 2 - 1))
    m = v - c
    if 0 <= h < 60:
        rgb = (c, x, 0)
    elif 60 <= h < 120:
        rgb = (x, c, 0)
    elif 120 <= h < 180:
        rgb = (0, c, x)
    elif 180 <= h < 240:
        rgb = (0, x, c)
    elif 240 <= h < 300:
        rgb = (x, 0, c)
    elif 300 <= h < 360:
        rgb = (c, 0, x)
    rgb = tuple(map(lambda a: int((a+m) * 255), rgb))
    return rgb

img = Image.new("RGB", (8, 4))
# x axis 1 to 8 : hue
# y axis 1 to 4 : saturation
for i in range(4):
    for j in range(8):
        if j == 7:
            hsv = 360*j//7, 0, i/3
        else:
            hsv = 360*j//7, (4-i)/4, (i+1)/4
        print(hsv)
        img.putpixel((j, i), hsv_to_rgb(*hsv))
img.save("pallete.png")