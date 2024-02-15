#!/bin/bash

num=0
inc=0

echo "Enter a number: "
read num

echo "Enter a number to multiply the first number by: "
read inc

echo $num "*" $inc = $(($num*$inc))

