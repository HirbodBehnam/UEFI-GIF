from PIL import Image
import os
import sys

# Check arguments
if len(sys.argv) != 2:
    print("Please pass the FPS as the first argument")
    exit(1)
FPS = int(sys.argv[1])

# Create the buffer
gif_buffer: list[list[int]] = []
width = 0
height = 0
for file in sorted(os.listdir("frames")):
    # Create buffer for this file
    bgra_buffer: list[int] = []
    with Image.open("frames/" + file) as img:
        for pixel in img.getdata():
            bgra_buffer.append((pixel[0] << 16) | (pixel[1] << 8) | pixel[2])
    # Append the buffer to master buffer
    gif_buffer.append(bgra_buffer)
    width, height = img.size

# Write the buffer to file
print(f"Using {width} and {height} as width and height")
with open("embdedded_file.h", "w") as output:
    output.write(f"#define GIF_WIDTH {width}\n")
    output.write(f"#define GIF_HEIGHT {height}\n")
    output.write(f"#define GIF_FRAMES {len(gif_buffer)}\n")
    output.write(f"#define GIF_FPS {FPS}\n")
    output.write(f"uint32_t gif_frames[{len(gif_buffer)}][{width * height}] = {'{'}\n")
    for frame in gif_buffer:
        output.write("{")
        for pixel in frame:
            output.write(str(pixel) + ", ")
        output.write("},\n")
    output.write("};\n") 