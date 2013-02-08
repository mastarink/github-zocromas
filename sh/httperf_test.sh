#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh

  port=5005
  if [[ "$1" ]] ; then
    port=$1
    shift
  fi
  httperf --server zocromas.mastar.lan --http-version=1.0 --port=$port --server-name zocromas.mastar.lan:$port  --uri=/HarryHarrison.jpeg --num-conns=100 --num-calls=1000 $@
fi
