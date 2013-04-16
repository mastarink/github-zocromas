#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh
 #echo ZZZZZZZZZZZZ
  function dosh_list ()
  {
    local n v
    n=$( prjconfname )
    v=$( prjconfversion )
  # echo -e "  $( basename $( realpath . ) );\t\t$n\t: $v"
  # printf "%-30s %-30s %s\n"  $( basename $( realpath . ) ) $n $v
    printf "%-30s %s\n"  $n $v
  # pwd
  }
fi
