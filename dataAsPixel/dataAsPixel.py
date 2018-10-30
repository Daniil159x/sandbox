#!/usr/bin/env python3

import math
import numpy as np
from PIL import Image

PNGmode = "RGB" # maybe RGBA

def encode(data):
    mod = len(PNGmode)

    n = len(data)
    pixels = (n + mod-1) // mod
    w = math.floor( math.sqrt( pixels ) )
    h = (pixels + w - 1) // w
    
    data += bytes([0] * (w*h*mod - n) )
    
    res = []
    for i in range(h):
        res += [[]]
        for j in range(w):
            idx = i*w*mod + j*mod
            res[i] += [data[idx:idx+mod]]
            
    arr = np.array(res)
    return Image.fromarray(arr, PNGmode)


def decode(img):
    arr = np.array(img)
    return bytes(arr.ravel())


if __name__ == "__main__":
    isEncode = ( input("Decode [Y/n]? ") == 'n' )

    if not isEncode: # decode
        with Image.open(input("Decode: ")) as toDecode, open("result.decode", "bw") as res:
            res.write( decode( toDecode.convert(PNGmode) ) )
            print("Created result.decode")
    else: # encode
        with open(input("Encode: "), "br") as toEncode:
            encode( toEncode.read() ).save("result.png")
            print("Created result.png")