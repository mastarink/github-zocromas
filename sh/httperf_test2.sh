#!/bin/sh

#!/bin/sh
port=5005
if [[ "$1" ]] ; then
  port=$1
  shift
fi
httperf --server zocromas.mastar.lan --http-version=1.0 --port=$port --server-name zocromas.mastar.lan:$port  --uri=/xcromas/get/server/info --num-conns=100 --num-calls=1000 $@
