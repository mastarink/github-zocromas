#!/bin/sh
dir=$( realpath $( dirname $0 ) )
# indir="$( realpath $dir/.. )"
# updir="$( realpath $indir/.. )"
# srcdir="$indir/src"

$dir/debug_any.sh $0 $@ 

