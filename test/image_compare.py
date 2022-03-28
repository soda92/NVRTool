from PIL import ImageGrab

im = ImageGrab.grab(bbox=(0, 0, 200, 200))
im2 = ImageGrab.grab(bbox=(0, 0, 200, 200))

import imagehash

hash1 = imagehash.average_hash(im)
hash2 = imagehash.average_hash(im2)

print(hash1 - hash2)
