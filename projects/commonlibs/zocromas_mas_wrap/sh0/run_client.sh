#!/bin/sh

dir=$( realpath $( dirname $0 ) )
echo "<<< $dir >>>" >&2

$dir/run_any.sh $0 "$@"
