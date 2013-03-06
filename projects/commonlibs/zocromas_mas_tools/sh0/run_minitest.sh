#!/bin/sh

dir=$( realpath $( dirname $0 ) )

$dir/run_any.sh $0 "$@"
