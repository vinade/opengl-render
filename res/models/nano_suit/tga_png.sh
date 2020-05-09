#!/bin/bash
for file in *.tga
do
    convert "$file" "$(basename "$file" .tga).png"
done
