#!/bin/bash
for file in *.dds
do
    convert "$file" "$(basename "$file" .dds).png"
done
