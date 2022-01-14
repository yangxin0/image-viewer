#! /usr/bin/env python

import sys
import struct
import cv2


def main(_, image_path, bgr888_path):
    img = cv2.imread(image_path, cv2.IMREAD_COLOR)
    (width, height, _) = img.shape
    fout = open(bgr888_path, "wb")
    for i in range(width):
        for j in range(height):
            fout.write(struct.pack("<BBB", *img[i, j, :].tolist()))
    fout.close()


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: ./dump_bgr888.py image_path output_path")
        exit(-1)
    main(*sys.argv)