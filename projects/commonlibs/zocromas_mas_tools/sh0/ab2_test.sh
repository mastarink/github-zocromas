#!/bin/sh
fname=/tmp/ab2.tmp
/usr/sbin/ab2 $@ http://zocromas.mastar.lan:5005/HarryHarrison.jpeg > $fname 2>&1
#grep '^\(Time\sper\srequest:.*(mean)\)'  $fname
cat $fname
