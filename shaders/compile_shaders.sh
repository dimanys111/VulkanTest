#!/bin/bash

glslangValidator -V obj.vert -o objVert.spv
glslangValidator -V obj.frag -o objFrag.spv

glslangValidator -V sky.vert -o skyVert.spv
glslangValidator -V sky.frag -o skyFrag.spv

glslangValidator -V sun.vert -o sunVert.spv
glslangValidator -V sun.frag -o sunFrag.spv