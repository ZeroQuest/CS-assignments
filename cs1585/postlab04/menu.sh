#!/bin/bash

input=""
file=*

menuOptions()
{
	echo "v) View" $file;
	echo "e) Edit" $file;
	echo "c) Compile" $file;
	echo "x) Execute" $file;
	echo "q) Quit" $file;
}

for file in *
do

# if [[ -f $file]]
#then
	menuOptions;

	read input

case $input in
	v)
		less $file;
		;;

	e)
		nano $file;
		;;

	c)
		g++ -Wall $file -o a.out;
		;;

	x)
		if [[ !$file.out ]]
		then
			g++ -Wall $file -o a.out;
		fi
		./a.out
		;;

	q)
		exit
		;;
	*)
		echo "Invalid input. Moving to next file.";
		echo "";
		;;
esac
#fi
done
