import sys, imagecodecs

with open(sys.argv[1], 'rb') as f:
    data = f.read()

encoded = imagecodecs.lzw_encode(data)

with open(sys.argv[2], 'wb') as f:
    f.write(encoded)
