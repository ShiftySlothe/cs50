from cs50 import get_int

# Get user input
h = -1
while h <= 0 or h > 8:
    h = get_int("Height: ")

# Print pyramid
for i in range(h):
    print((h - i - 1) * " ", end="")
    print((i + 1) * "#", end="")
    print("  ", end="")
    print((i + 1) * "#")