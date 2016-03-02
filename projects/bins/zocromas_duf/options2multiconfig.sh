#!/bin/sh
# 0.  *ERR* macros move to error lib from duf_se.h; muc_se.h
#+1. make CR => CRMUC or something like muc_CR, DR => DRMUC etc. - see muc_se.h
#     STT/STTT; T2T; ETT; ERT; 
#     ST/STP; DT/DTP; ET/ETP; DR/DRP; SR/SRP; ER/ERP;
#     DOCR; CR/CRV; IF_CR/IF_CRV ...?
# 2. Make a copy of /home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/bins/zocromas_duf
# 3. for all files EXCEPT *option*.[ch]
# 4. replace function calls for all functions from /home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/commonlibs/zocromas_mas_multiconfig/funlist.tmp
# 5. replace all #include's for files at /home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/commonlibs/zocromas_mas_multiconfig/src/inc/*.h

function dofiles ()
{
  local f fun match
  local -a mucfuncs
  readarray -t mucfuncs < /home/mastar/.mas/lib/big/misc/develop/autotools/zoc/projects/commonlibs/zocromas_mas_multiconfig/funlist.tmp
  match=duf_config_cli_t
  for fun in ${mucfuncs[@]} ; do
    if [[ $match ]] ; then
      match="$match\|$fun"
    else
      match="$fun"
    fi
  done
  echo "[$match]" >&2
  for f in src/*.c src/inc/*.h ; do
    if [[ -f $f ]] && [[ $(basename $f) == duf_* ]] && ! [[ $(basename $f) == duf_*option*.[ch] ]] ; then
      if grep "\($match\)(" $f ; then
        echo "........... $f ........................................................"
      fi
    fi
  done
}
dofiles
