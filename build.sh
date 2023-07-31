#!/bin/bash

if [[ ! -d ./build ]] 
then
    mkdir build
fi
g++ *.cpp -Wall -std=c++17 -o ./build/assembly-decorations -lwiringPi   
