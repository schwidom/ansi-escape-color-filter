#!/bin/bash

test -f "CMakeLists.txt" || { echo >&2 '! test -f "CMakeLists.txt" , exiting'; exit 1;} # created by createTestLineBashFromStdinOR.sh

cmake .

