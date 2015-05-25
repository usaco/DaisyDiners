#!/bin/bash

OUTPUT=driver.dat

read -e -p "Board size (NxN)? " -i "100" sz
read -e -p "Number of rounds? " -i "1000" rd
read -e -p "Number of bots? " bots

echo "$sz $rd $bots" > $OUTPUT

for i in `seq 1 $bots`; do
	read -e -p "Bot #$i: " bot
	echo "../bots/$bot/$bot" >> $OUTPUT
done

