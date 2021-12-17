#!/bin/bash

for i in $(seq 4)
do
    time ./bf_ver3 < "input_$i.txt" > /dev/null
done