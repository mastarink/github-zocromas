#!/bin/sh
# bash
function read_makefile_am ()
{
  local mark
  mark=$1
  local lin i fn
  local -a lines
  local inblock last
  local -i got=0
  readarray -t lines
# echo " ${#lines[@]} -- ($mark)" >&2
  for (( i=0; i<${#lines[@]}; i++ )) ; do
    lin=${lines[$i]}
    if ! [[ $inblock ]] && [[ $lin =~ $mark=[[:blank:]]*(.*[^[:blank:]]|)[[:blank:]]*(\\|)$ ]] ; then
      lin=${BASH_REMATCH[1]}
#     echo "[$lin]"
      inblock=yes
      got+=1
    elif [[ $inblock ]] && [[ $lin =~ [[:blank:]]*([^[:blank:]\\].*[^[:blank:]\\]|)[[:blank:]]*(\\|)$ ]] ; then
      lin=${BASH_REMATCH[1]}
#     echo "[$lin]"
      if ! [[ ${BASH_REMATCH[2]} ]] ; then
        last=yes
      fi
    elif [[ $inblock ]] ; then
      echo "Why ($lin)" >&2
      unset lin
    fi
    if [[ $inblock ]] && [[ $got -gt 0 ]] ; then
      for fn in $lin ; do
	echo $fn
      done
      if [[ $last ]] ; then
        unset inblock
#	break
      fi
    fi    
  done
}
function manage_makefile_am ()
{
  local at
  at=$1
  local mark=$2
  local file
  file="$at/Makefile.am"
# shift
  if [[ -f $file ]] ; then
#   echo $file >&2
    files=(${files[@]} $( read_makefile_am $mark < $file | sort ))
  fi
}
function manage_makefiles_am ()
{
# 159 .h + 139 .c = 298
  local -a files
  local -a edfiles
  local -A in_make in_path in_edit in_any
  local fn fnf
  local -i cnt=0 cntc=0 cnth=0
  local -i cntf=0 cntfc=0 cntfh=0
  manage_makefile_am src _SOURCES
  manage_makefile_am src/inc noinst_HEADERS
# echo ${files[@]} >&2
# echo ${#files[@]} >&2
  for fn in ${files[@]} ; do
    if [[ $fn == *.c ]] && [[ -f src/$fn ]] ; then
      cntc+=1
      in_make[$fn]=1
      in_any[$fn]+=1
    elif [[ $fn == *.h ]] && [[ -f src/inc/$fn ]] ; then
      cnth+=1
      in_make[$fn]=1
      in_any[$fn]+=1
    fi
  done
  cnt=$(( $cntc + $cnth ))
  for fnf in src/*.c src/inc/*.h ; do
    fn=$(basename $fnf)
    if [[ $fn == *.c ]] && [[ -f src/$fn ]] ; then
      cntfc+=1
      in_path[$fn]=1
      in_any[$fn]+=1
    elif [[ $fn == *.h ]] && [[ -f src/inc/$fn ]] ; then
      cntfh+=1
      in_path[$fn]=1
      in_any[$fn]+=1
    fi  
  done
  cntf=$(( $cntfc + $cntfh ))

  edfiles=($(grep  -hr --inc='*.mased.vim' '^\<\(find\|sfind\|tab\s\+sfind\)\>\s*.*\.[ch]' editing/mased/ | sed 's@^\<\(find\|sfind\|tab\s\+sfind\)\>\s*\(.*\.[ch]\)@\2@' | sort | uniq))
  for fn in ${edfiles[@]} ; do
    in_edit[$fn]=1
    in_any[$fn]+=1
  done
  echo "edit:${#edfiles[@]} :: ${#in_edit[@]}" >&2
  echo "cntc:$cntc; cnth:$cnth; cnt:$cnt" >&2
  echo "cntfc:$cntfc; cntfh:$cntfh; cntf:$cntf" >&2
  echo "in_make:${#in_make[@]}" >&2
  echo "in_path:${#in_path[@]}" >&2
  echo "in_any:${#in_any[@]}" >&2
  for fn in ${!in_any[@]} ; do
    if ! [[ ${in_make[$fn]} ]] || ! [[ ${in_path[$fn]} ]] || ! [[ ${in_edit[$fn]} ]] ; then
      echo "make:[${in_make[$fn]:-0}] path:[${in_path[$fn]:-0}] edit:[${in_edit[$fn]:-0}] -- $fn"
    fi
  done
}
manage_makefiles_am $@
