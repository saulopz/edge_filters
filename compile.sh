#!/bin/sh
# edge detection program
clang++ -g src/edge.cpp src/net.cpp -o edge -lSDL2 -lSDL2_image

# trainning algorithm for perceptron
clang++ -g src/train.cpp -o train -lSDL2 -lSDL2_image