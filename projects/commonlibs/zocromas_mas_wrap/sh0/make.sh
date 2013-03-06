#!/bin/sh
dir=$( realpath $( dirname $0 ) )
indir="$( realpath $dir/.. )"
updir="$( realpath $indir/.. )"
function masmake ()
{
  if make -j3 -s $@ ; then
    true
# elif ./configure ; then
#   make -s $@
# else
#   autoreconf && ./configure && make -s $@
  else
    false
  fi  
}
function gmasmake ()
{
# grep  --include='*.[ch]' -r '\<\(pipe\|fork\|waitpid\|accept\|connect\|bind\|getsockopt\|setsockopt\|inet_ntop\|listen\|close\|mkfifo\)\>\s*(' src inc | grep -v '^src/mas_lib[0-9]*\.c'
  ! grep -r --color --include='*.[ch]' --exclude-from='sh/grepfunexclude.txt' -r '\<\(pipe\|fork\|waitpid\|accept\|connect\|bind\|getsockopt\|setsockopt\|inet_ntop\|listen\|close\|mkfifo\)\>\s*(' src && \
  ! grep -r --color --include='*.[ch]' --exclude-from='sh/grepfunexclude.txt' -r '\<\(malloc\|free\|strdup\|strndup\)\>\s*(' src && \
  masmake $@
# make -s ctags
}
cd $dir/..
time gmasmake $@
