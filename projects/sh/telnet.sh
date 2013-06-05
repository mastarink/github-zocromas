#!/bin/sh
function httpviatelnet ()
{
  local host ; host=$1 ; shift
  local port ; port=$1 ; shift
  local url ; url=$1 ; shift
  local method ; method=$1 ; shift
  if ! [[ "$method" ]] ; then method=GET ; fi
  local keep ; keep=$1 ; shift

  # don't echo -ne "\r\n" here;
  # telnet will do it !!
  { 
    echo "$method $url HTTP/1.1"
    echo "HOST: $host"
    if [[ "$keep" == 'keep-alive' ]] ; then 
      echo "Connection: keep-alive"
    fi
    echo
    sleep 1
  }  | telnet $host $port
}
httpviatelnet zocromas.mastar.lan 8102 /index.html              > 8102.telnet.txt
httpviatelnet zocromas.mastar.lan 80 /index.html                > 80.telnet.txt
httpviatelnet zocromas.mastar.lan 5005 /index.html              > 5005.telnet.txt
httpviatelnet zocromas.mastar.lan 8102 /index.html GET 'keep-alive' > 8102k.telnet.txt
httpviatelnet zocromas.mastar.lan 80 /index.html   GET 'keep-alive' > 80k.telnet.txt
httpviatelnet zocromas.mastar.lan 5005 /index.html GET 'keep-alive' > 5005k.telnet.txt
  echo "---------- zocromas.mastar.lan 8102 /index.html"
  cat 8102.telnet.txt
  cat 8102k.telnet.txt
  echo "---------- zocromas.mastar.lan 80 /index.html"
  cat 80.telnet.txt
  cat 80k.telnet.txt
  echo "---------- zocromas.mastar.lan 5005 /index.html"
  cat 5005.telnet.txt
  cat 5005k.telnet.txt
  echo "-----------------------------------------"
