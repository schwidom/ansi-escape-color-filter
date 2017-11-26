#!/bin/bash

CODE=$'123\033456\033[789'

if test "$CODE" == "$(./aecf <<<$CODE)" ; then
 echo ok
else
 echo nok
 exit -1
fi

