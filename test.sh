#!/bin/bash

if [[ $EUID -ne 0 ]]; then
	echo "Permission denied."
	exit 1
fi

for entry in "./OS_PJ1_Test"/*
do
    echo "running $entry"
    filename=$(echo "$entry" | cut -d/ -f 3)
    echo "$filename"
    outfile=./output/$(echo "$filename" | cut -d. -f 1)_stdout.txt
    dout=./output/$(echo "$filename" | cut -d. -f 1)_dmesg.txt
    dmesg -c
    ./src/scheduler < "$entry" > "$outfile"
    dmesg | grep OS > "$dout"
done