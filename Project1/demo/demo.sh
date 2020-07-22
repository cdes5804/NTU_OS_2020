#!/bin/bash

if [[ $EUID -ne 0 ]]; then
	echo "Permission denied."
	exit 1
fi

for file in TIME_MEASUREMENT FIFO_1 PSJF_2 RR_3 SJF_4;
do
    input=../OS_PJ1_Test/"$file".txt
    echo "running $input"
    ../src/scheduler < "$input"
    dmesg -c | grep OS
    echo ""
done