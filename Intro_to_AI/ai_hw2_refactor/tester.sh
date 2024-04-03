#!/bin/bash
g++ -std=c++11 improve_tester.cpp -g -o input_tester.ex
./input_tester.ex $1 $2
