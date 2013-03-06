#!/bin/sh
mcaller=$1
shift
name=$( basename $mcaller )
if [[ $name =~ gdbcore_([a-z]+)\.sh ]] ; then
  bname=${BASH_REMATCH[1]}
  name="mas_$bname"
fi
echo "<< $name >>">&2

dir=$( realpath $( dirname $0 ) )
indir="$( realpath $dir/.. )"
updir="$( realpath $indir/.. )"
srcdir0="$indir/src"
srcdir="$indir/src/$bname"
binsdir="$srcdir/.libs"
# libsdirs="$srcdir0/.libs"
libsdirs="$( find -type d -name '.libs' -printf '%p ' )"
#for dl in server fileinfo init tools cmdmods maslibs client http listener channel ; do
# libsdirs="$libsdirs:$srcdir0/$dl/.libs"
#done


export LD_LIBRARY_PATH=$libsdirs
ulimit -c

corename=$( ls -1tr /tmp/core.$UID.$UID.${name}.* | tail -1 )
echo "name: {$name} ; core:{$corename}" >&2
if [[ -f "$corename" ]] ; then
  libtool --mode=execute gdb $binsdir/$name -c "$corename"
fi
