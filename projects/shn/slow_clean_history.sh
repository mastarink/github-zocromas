#!/bin/sh
function mas_clear_history ()
{
  local prefix=$1 num xx
  echo "prefix:$prefix" >&2
  if [[ $prefix ]] ; then
    num=$( history | grep '^[[:blank:]]*[[:digit:]]\+[[:blank:]]\+[[:digit:]]\+\.[[:digit:]]\+:[[:digit:]]\+:[[:digit:]]\+[[:blank:]]\+:'$prefix'\(\>\|$\|[[:blank:]]\)' | wc -l )
    echo "... of $num" >&2
    while xx=$( history | \
    	grep '^[[:blank:]]*[[:digit:]]\+[[:blank:]]\+[[:digit:]]\+\.[[:digit:]]\+:[[:digit:]]\+:[[:digit:]]\+[[:blank:]]\+:'$prefix'\(\>\|$\|[[:blank:]]\)' | \
	head -1 | \
	awk '{print $1}' ) \
     && [[ $xx ]]
    do
      echo "#$xx of $num" >&2
      history -d $xx
   done
 fi
}
function mas_clear_history2 ()
{
  local prefix=$1 num n
  echo "prefix:$prefix" >&2
  if [[ "$prefix" ]] ; then
    num=$( history | tac | grep '^[[:blank:]]*[[:digit:]]\+[[:blank:]]\+[[:digit:]]\+\.[[:digit:]]\+:[[:digit:]]\+:[[:digit:]]\+[[:blank:]]\+:'$prefix'\(\>\|$\|[[:blank:]]\)' | wc -l )
    echo "... of $num" >&2
    for n in $( history | tac | \
    	grep '^[[:blank:]]*[[:digit:]]\+[[:blank:]]\+[[:digit:]]\+\.[[:digit:]]\+:[[:digit:]]\+:[[:digit:]]\+[[:blank:]]\+:'$prefix'\(\>\|$\|[[:blank:]]\)' | \
	awk '{print $1}' )
    do
      echo "#$n of $num" >&2
      history -d $n
   done
 fi
}
# history | grep -v '\<\(run\|git\|find\|shn\|history\|tput\|lshn\|exiftool\|jpeginfo\|exifcom\|exiftags\|exifgthumb\|ldd\|export\|unset\|cpp\)\>'
# source  shn/slow_clean_history.sh  'euses'
mas_clear_history2 $@
