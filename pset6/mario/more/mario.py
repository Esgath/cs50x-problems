from cs50 import get_int

while True:
    pyramid_height = get_int("Pyramid height: ")
    hashes = 0
    if pyramid_height > 8 or pyramid_height < 1:
        continue
    else:
        for i in range(pyramid_height):
            hashes += 1
            white_space = pyramid_height - hashes
            
            # print first half
            print(" " * white_space, end="")
            print("#" * hashes, end="")
            
            # print space between
            print(" " * 2, end="")
            
            # print second half
            print("#" * hashes, end="")
            print()
        break
