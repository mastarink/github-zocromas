#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh

  port=5005
  ofile="$testdir/${now_stamp}.httperf_test"
  conns=6
  calls=400
# the rate option means the number of connections to be created each second
# if we have num-con = 1000 and rate = 20 , then 20 requests will generated togother evey one second , so ideally the process should take 1000/20 = 50 seconds. 
  rate=1000
# opts=" --http-version=1.0 --uri=/HarryHarrison.jpeg --num-conns=$conns --num-calls=$calls --ra=$rate --wsess=100,50,10 --burst-len=5"
  opts=" --http-version=1.1 --uri=/HarryHarrison.jpeg --num-conns=$conns --num-calls=$calls"
  if [[ "$1" ]] ; then
    port=$1
    shift
  fi
  if [[ -d "$testdir" ]] ; then
    echo "`/bin/date '+%Y%m%d.%H%M%S.%N'` @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" > $ofile
    echo "`/bin/date '+%Y%m%d.%H%M%S.%N'`@@@ nginx.mastar.lan:8102" >> $ofile
    httperf --server zocromas.mastar.lan --port=8102  --server-name zocromas.mastar.lan:8102  $opts $@ >> $ofile
    echo "`/bin/date '+%Y%m%d.%H%M%S.%N'`@@@ mastarink.net:80" >> $ofile
    httperf --server zocromas.mastar.lan --port=80    --server-name zocromas.mastar.lan:80    $opts $@ >> $ofile
    echo "`/bin/date '+%Y%m%d.%H%M%S.%N'`@@@ zocromas.mastar.lan:$port" >> $ofile
    httperf --server zocromas.mastar.lan --port=$port --server-name zocromas.mastar.lan:$port $opts $@ >> $ofile
    echo "`/bin/date '+%Y%m%d.%H%M%S.%N'`@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" >> $ofile
  fi
  if [[ -f $ofile ]] ; then
    cat $ofile
    echo "@> $ofile" >&2
#   e $ofile
  fi
fi
