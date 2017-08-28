#!/bin/bash
NAME=$(cat filename.txt)
if["abc"="abc"]then
	echo "aaa"
else
	echo $NAME
fi
