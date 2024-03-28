#!/bin/bash

word=$"1" 
website=$"2" 
#input=$""

if [[ $# -ne 2 ]]
then
	echo "Usage goog.sh WORD WEBSITE";
	exit;
fi

	wget $2 -q -O site;
	grep -o $1 site | wc -l > sample.txt; 
	read input < sample.txt;
	echo $word ": "$input;

	rm sample.txt;
	rm site;
