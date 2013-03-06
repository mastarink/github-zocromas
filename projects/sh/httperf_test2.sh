#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh

  port=5005
  if [[ "$1" ]] ; then
    port=$1
    shift
  fi
  if [[ -d "$testdir" ]] ; then
    httperf --server zocromas.mastar.lan --http-version=1.0 --port=$port --server-name zocromas.mastar.lan:$port  --uri=/xcromas/get/server/info --num-conns=100 --num-calls=1000 $@ > "$testdir/${now_stamp}.httperf_test2"
  fi
  if [[ -f $testdir/${now_stamp}.httperf_test2 ]] ; then
    cat $testdir/${now_stamp}.httperf_test2
  fi
fi
