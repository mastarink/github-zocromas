#!/bin/sh
# bash
function read_makefile_am ()
{
  local -a files
  local -A hfiles
  local mark
  mark=$1
  local lin i fn
  local -a lines
  local inblock last
# local -i got=0
  readarray -t lines
# echo " ${#lines[@]} -- ($mark)" >&2
  for (( i=0; i<${#lines[@]}; i++ )) ; do
    lin=${lines[$i]}
#   echo "0 ${inblock:-not} [$lin] --- mark:[$mark] [${BASH_REMATCH[1]}] [${BASH_REMATCH[2]}]" >&2
    if ! [[ $inblock ]] && [[ $lin =~ ^[[:blank:]]*[_[:alnum:]]*$mark[[:blank:]]*=[[:blank:]]*([^\\]*[^\\[:blank:]]|)[[:blank:]]*(\\|)$ ]] ; then
      lin=${BASH_REMATCH[1]}
      if [[ "$lin" ]] ; then
#       echo "1 ${inblock:-not} [$lin] --- [${BASH_REMATCH[1]}] [${BASH_REMATCH[2]}]" >&2
        files=(${files[@]} $lin)
#       got+=1
      fi
      if [[ "${BASH_REMATCH[2]}" ]] ; then
        inblock=yes
      else
        unset inblock
      fi
    elif [[ $inblock ]] && [[ $lin =~ ^[[:blank:]]*([^[:blank:]\\][^\\]*[^\\[:blank:]\\]|)[[:blank:]]*(\\|)$ ]] ; then
      lin=${BASH_REMATCH[1]}
      if [[ "$lin" ]] ; then
#       echo "2 ${inblock:-not} [$lin] --- [${BASH_REMATCH[1]}] [${BASH_REMATCH[2]}]" >&2
        files=(${files[@]} $lin)
      fi
      if ! [[ ${BASH_REMATCH[2]} ]] ; then
        last=yes
	unset inblock
#	echo 'last ****' >&2
      fi
    elif [[ $inblock ]] ; then
#     echo "3 ${inblock:-not} [$lin] --- [${BASH_REMATCH[1]}] [${BASH_REMATCH[2]}]" >&2
      echo "Why ($lin)" >&2
      unset lin
#   else
#     echo "--- Why ($lin -- $mark)" >&2
    fi
#     if [[ $inblock ]] && [[ ${#files[@]} -gt 0 ]] ; then
#         for fn in ${files[@]} ; do
# #	echo "T [$fn]" >&2
#          echo $fn
#        done
#       if [[ $last ]] ; then
#         unset inblock
# #	break
#       fi
#     fi    
  done
  for fn in ${files[@]} ; do
    hfiles[$fn]=1
  done
  for fn in ${!hfiles[@]} ; do
#   echo "T [$fn]" >&2
    echo $fn
  done | sort
}
function manage_makefile_am ()
{
  local -a new_files
  local at
  at=$1
  local mark=$2
  local file
  file="$at/Makefile.am"
# shift
  if [[ -f $file ]] ; then
#   echo "$file : xxxxxxxxxxxxxx ----------------"
    new_files=($(read_makefile_am $mark < $file))
#   echo "new_files: ${new_files[@]}"
    files=(${files[@]} ${new_files[@]})
#   echo "$file : xxxxxxxxxxxxxx ${files[@]}"
  fi
}
function manage_makefiles_am ()
{
# 159 .h + 139 .c = 298
  local -a files
  local -a edfiles incfiles
  local -iA in_make in_path in_edit in_include in_any
  local fn fnf
  local -i cnt=0 cntc=0 cnth=0
  local -i cntf=0 cntfc=0 cntfh=0
  manage_makefile_am src _SOURCES
# echo "1 zzzzzzzzzzzzz ${files[@]}" >&2
  manage_makefile_am mod _SOURCES
# echo "2 zzzzzzzzzzzzz ${files[@]}" >&2
  manage_makefile_am src/inc noinst_HEADERS
# echo "3 zzzzzzzzzzzzz ${files[@]}" >&2
  manage_makefile_am mod/inc noinst_HEADERS
# echo "4 zzzzzzzzzzzzz ${files[@]}" >&2
# echo ${#files[@]} >&2
  for fn in ${files[@]} ; do
    if [[ $fn == *.c ]] && [[ -f src/$fn ]] ; then
      cntc+=1
      in_make[$fn]=1
      in_any[$fn]+=1
    elif [[ $fn == *.c ]] && [[ -f mod/$fn ]] ; then
      cntc+=1
      in_make[$fn]=1
      in_any[$fn]+=1
    elif [[ $fn == *.h ]] && [[ -f src/inc/$fn ]] ; then
      cnth+=1
      in_make[$fn]=1
      in_any[$fn]+=1
    elif [[ $fn == *.h ]] && [[ -f mod/inc/$fn ]] ; then
      cnth+=1
      in_make[$fn]=1
      in_any[$fn]+=1
    fi
  done
  cnt=$(( $cntc + $cnth ))
  for fnf in {src,mod}/*.c {src,mod}/inc/*.h  ; do
    fn=$(basename $fnf)
    if [[ $fn == *.c ]] && [[ -f "src/$fn" ]] ; then
      cntfc+=1
      in_path[$fn]=1
      in_any[$fn]+=1
    elif [[ $fn == *.c ]] && [[ -f "mod/$fn" ]] ; then
      cntfc+=1
      in_path[$fn]=1
      in_any[$fn]+=1
    elif [[ $fn == *.h ]] && [[ -f "src/inc/$fn" ]] ; then
      cntfh+=1
      in_path[$fn]=1
      in_any[$fn]+=1
    elif [[ $fn == *.h ]] && [[ -f "mod/inc/$fn" ]] ; then
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
# incfiles=($(grep  -hr --inc='*.c' '^\s*#include\s\+"[a-z_]\+\.h"' src/ | sed 's@^\s*#include\s\+"\([a-z_]\+\.h\).*$"@\1@' | sort | uniq))
  incfiles=($(grep  -hr --inc='*.[ch]' '^\s*#\s*include\s\+"[a-z0-9_]\+\.h"' src/ mod/ | sed -ne 's@^\s*#\s*include\s\+\"\([a-z0-9_]\+\.h\)\".*$@\1@p' | sort | uniq))
  for fn in ${incfiles[@]} ; do
    in_include[$fn]=1
    in_any[$fn]+=1
#   echo "$fn" >&2
  done
  echo "edit:${#edfiles[@]} :: ${#in_edit[@]}" >&2
  echo "cntc:$cntc; cnth:$cnth; cnt:$cnt" >&2
  echo "cntfc:$cntfc; cntfh:$cntfh; cntf:$cntf" >&2
  echo "in_make:${#in_make[@]}" >&2
  echo "in_path:${#in_path[@]}" >&2
  echo "in_any:${#in_any[@]}" >&2
  for fn in ${!in_any[@]} ; do
    if  ( ! [[ ${in_make[$fn]} ]] || ! [[ ${in_path[$fn]} ]] || ! [[ ${in_edit[$fn]} ]]  || ( [[ $fn == *.h ]] && ! [[ ${in_include[$fn]} ]] ) ) ; then
      if ! [[ ${in_make[$fn]:-0} -gt 0 ]]    ; then printf "\t%s" "NOT make "
      else                                          printf "\t%s" "    MAKE "
      fi
      if ! [[ ${in_path[$fn]:-0} -gt 0 ]]    ; then printf "\t%s" "NOT path "
      else                                          printf "\t%s" "    PATH "
      fi
      if ! [[ ${in_edit[$fn]:-0} -gt 0 ]]    ; then printf "\t%s" "NOT edit "
      else                                          printf "\t%s" "    EDIT "
      fi
      if ! [[ ${in_include[$fn]:-0} -gt 0 ]] ; then printf "\t%s" "NOT incl "
      else                                          printf "\t%s" "    INCL "
      fi
      echo " -- $fn"
    fi
  done
}
manage_makefiles_am $@
