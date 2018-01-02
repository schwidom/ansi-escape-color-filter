#!/bin/bash

test -f tags2/2j36v4onzk/2j36v4onzk.txt || { echo >&2 '! test -f tags2/2j36v4onzk/2j36v4onzk.txt , exiting'; exit 1;} # created by createTestLineBashFromStdinOR.sh

CODE=$'123\033456\033[789'

if test "$CODE" == "$(./aecf-debug/aecf <<<$CODE)" ; then
 echo ok
else
 echo nok
 exit -1
fi

