#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh

  port=50005
  if [[ "$1" ]] ; then
    port=$1
    shift
  fi
  if [[ -d "$testdir" ]] ; then
    httperf --server zocromas.mastar.lan --http-version=1.0 --port=$port --server-name zocromas.mastar.lan:$port  --uri=/HarryHarrison.jpeg --num-conns=50 --num-calls=2000 $@ > "$testdir/${now_stamp}.httperf_test"
  fi
  if [[ -f $testdir/${now_stamp}.httperf_test ]] ; then
    cat $testdir/${now_stamp}.httperf_test
  fi
fi
