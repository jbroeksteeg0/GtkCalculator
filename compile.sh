#!/usr/bin/sh
g++ *.cpp -o main -fpermissive `pkg-config --cflags gtk+-3.0 --libs gtk+-3.0`
