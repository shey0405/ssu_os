#!/bin/bash
NAME=$(cat filename.txt)
if [ $NAME = hsc3380_SRR490124_1_F ]
then
echo success
else
echo fail
fi
