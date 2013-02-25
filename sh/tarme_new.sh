#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh
  
  # show_setup
  function tarme ()
  {
    if [[ "$savedirtarme" ]] &&  [[ -d "$savedirtarme" ]] && [[ "$updir" ]] && [[ -d "$updir" ]] && [[ "$indir" ]] && [[ -d "$indir" ]] ; then
      if [[ "$mas_name" ]] && [[ "$mas_vers" ]] ; then
	cd $indir || return 1
	touch {tmp,boxes,.build,autom4te.cache,m4,errors,src/.{deps,libs}}/mas-tar-exclude
#	tar -vv -jcf --wildcards --exclude-caches --exclude-tag mas-tar-exclude -X $rootdir/tar-common.exculde -f "$savedirtarme/${mas_name}.${mas_vers}.`datemt`.tar.bz2" .
	tar -jpc --exclude-caches --exclude-tag mas-tar-exclude -X $rootdir/tar-common.exculde -f "$savedirtarme/${mas_name}.${mas_vers}.tar-${now_stamp}.tar.bz2" .
#	tar -vv -Jpc --exclude-caches --exclude-tag mas-tar-exclude -X $rootdir/tar-common.exculde -f "$savedirtarme/${mas_name}.${mas_vers}.tar-${now_stamp}.tar.z" .
#	tar -vv -zpc --exclude-caches --exclude-tag mas-tar-exclude -X $rootdir/tar-common.exculde -f "$savedirtarme/${mas_name}.${mas_vers}.tar-${now_stamp}.tar.gz" .
#	tar -vv -Zpc --exclude-caches --exclude-tag mas-tar-exclude -X $rootdir/tar-common.exculde -f "$savedirtarme/${mas_name}.${mas_vers}.tar-${now_stamp}.tar.Z" .
      else
        echo ">>>>>>>>>> $LINENO" >&2
      fi    
    else
      echo ">>>>>>>>>> $LINENO" >&2
    fi
    return 0
  }
  tarme $@
fi
