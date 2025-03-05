#!/bin/bash


OPTS="-o viewer.out -std=c89 -Wall"
SRCS="main.c platforms/macos.m core/*.c"
LIBS="-framework Cocoa"

clang $OPTS $SRCS $LIBS
