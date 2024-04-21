#!/bin/bash
g++ -std=c++11 slowIterative.cpp -o slowIterative.ex
./slowIterative.ex $1 $2
