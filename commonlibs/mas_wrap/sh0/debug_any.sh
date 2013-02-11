#!/bin/sh
mcaller=$1
shift
name=$( basename $mcaller )
if [[ $name =~ debug_([a-z]+)\.sh ]] ; then
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
libsdirs="$( find -type d -name '.libs' -printf '%p:' )"
#for dl in server fileinfo init tools cmdmods maslibs client http listener channel ; do
# libsdirs="$libsdirs:$srcdir0/$dl/.libs"
#done

function mas_debug ()
{
  local sedex cmdfile tmpcmd
  name=$1
  shift
  export LD_LIBRARY_PATH=$libsdirs
  ulimit -c

  cmdfile="$dir/debug_${name}.cmd"
  sedex="s@^\(run\)@\1  $@@"

  tmpcmd="${cmdfile}.tmp"
  sed -e "$sedex" $cmdfile > $tmpcmd
  echo "LD_LIBRARY_PATH=$LD_LIBRARY_PATH" >&2
  

  if [[ -f "$tmpcmd" ]] && [[ -s "$tmpcmd" ]] ; then
  # libtool --mode=execute gdb -batch $binsdir/$name -x $tmpcmd
    libtool --mode=execute gdb        $binsdir/$name -x $tmpcmd
  else
    echo "no file : $cmdfile" >&2
  fi
}
clear && $dir/make.sh && mas_debug $name $@

# You can run gdb with no arguments or options; but the most usual way to start GDB is with one  argument or two,
# specifying an executable program as the argument:
# gdb program
# You can also start with both an executable program and a core file specified:
# gdb program core
# You can, instead, specify a process ID as a second argument, if you want to debug a running process:
# gdb program 1234
