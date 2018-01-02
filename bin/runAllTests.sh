#!/bin/bash

set -x
set -u

test -f tags2/2j36v4onzk/2j36v4onzk.txt || { echo >&2 '! test -f tags2/2j36v4onzk/2j36v4onzk.txt , exiting'; exit 1;} # created by createTestLineBashFromStdinOR.sh

bin/create-debug-build.sh

tests/chk_aecf.sh
aecf-debug/tests

