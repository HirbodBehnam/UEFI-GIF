#!/bin/bash

# Extract frames
rm -r frames
mkdir frames
ffmpeg -i payan.mp4 -vf "scale=200:-2,fps=10" frames/%04d.jpg

# Exctract bits
python3 extract.py 10
