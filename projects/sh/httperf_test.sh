#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh

  port=5005
  if [[ "$1" ]] ; then
    port=$1
    shift
  fi
  if [[ -d "$testdir" ]] ; then
    echo zocromas.mastar.lan:$port > "$testdir/${now_stamp}.httperf_test"
    httperf --server zocromas.mastar.lan --http-version=1.0 --port=$port --server-name zocromas.mastar.lan:$port  --uri=/HarryHarrison.jpeg --num-conns=50 --num-calls=1000 $@ >> "$testdir/${now_stamp}.httperf_test"
    echo nginx.mastar.lan:8102 >> "$testdir/${now_stamp}.httperf_test"
    httperf --server nginx.mastar.lan --http-version=1.0 --port=$port --server-name nginx.mastar.lan:8102  --uri=/HarryHarrison.jpeg --num-conns=50 --num-calls=1000 $@ >> "$testdir/${now_stamp}.httperf_test"
    echo mastarink.net:80 >> "$testdir/${now_stamp}.httperf_test"
    httperf --server mastarink.net --http-version=1.0 --port=$port --server-name mastarink.net:80  --uri=/HarryHarrison.jpeg --num-conns=50 --num-calls=1000 $@ >> "$testdir/${now_stamp}.httperf_test"
  fi
  if [[ -f $testdir/${now_stamp}.httperf_test ]] ; then
    cat $testdir/${now_stamp}.httperf_test
  fi
fi
