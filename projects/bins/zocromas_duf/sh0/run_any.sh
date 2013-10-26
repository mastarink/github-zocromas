#!/bin/sh
mcaller=$1
shift
name=$( basename $mcaller )
if [[ $name =~ run_([a-z]+)\.sh ]] ; then
  bname=${BASH_REMATCH[1]}
  name="mas_$bname"
fi

dir=$( realpath $( dirname $0 ) )
indir="$( realpath $dir/.. )"
updir="$( realpath $indir/.. )"
if [[ -d "$indir/src/$bname" ]] ; then
  srcdir="$indir/src/$bname"
else
  srcdir="$indir/src"
fi

if [[ "$name" ]] ; then
#  if [[ "$1" ]] ; then

#   clear >&2
# clear:
    echo -en "\e[H\e[2J" >&2
    echo "<<< $name >>>" >&2
    env | grep MAS_ZOCROMAS >&2

# for core dump:
    ulimit -c unlimited

    $dir/make.sh && usleep 500000 && clear && exec $srcdir/$name "$@"
#  else
#    clear && $dir/make.sh && exec $srcdir/$name -P5001 -H192.168.71.2
#  fi
fi
