#!/bin/bash


OPTS="-std=c89 -Wall -Wextra -pedantic -O3 -flto -ffast-math"
SRCS="main.c platforms/macos.m"
LIBS="-framework Cocoa"

clang -o Viewer $OPTS $SRCS $LIBS
