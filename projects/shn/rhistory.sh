#!/bin/sh
history | sed -ne 's@^\([[:digit:]]\+\)[[:blank:]]\+\([[:digit:]]\{8\}\)\.\([[:digit:]]\{2\}:[[:digit:]]\{2\}:[[:digit:]]\{2\}\)[[:blank:]]\+:\(run\>.*\)$@\1: \2,\3 \4@p'
