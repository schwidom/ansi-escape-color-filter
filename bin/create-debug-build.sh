#!/bin/bash

set -x
set -u

test -f tags2/2j36v4onzk/2j36v4onzk.txt || { echo >&2 '! test -f tags2/2j36v4onzk/2j36v4onzk.txt , exiting'; exit 1;} # created by createTestLineBashFromStdinOR.sh

mkdir -p aecf-debug
( cd aecf-debug
 cmake ../aecf
 make -j 8
)
