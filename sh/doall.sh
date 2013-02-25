#!/bin/sh
if [[ -f "sh/do_functions.sh" ]] ; then
  . sh/setup.sh
  . sh/do_functions.sh
  export MAS_DOALL_NOW_STAMP=`datemt`
  export MAS_DOALL_STAMP=`datem`
  doall $@
  for arg in "$@" ; do
    case $arg in
    tarme_new)
      echo "General : sh/ files/" >&2
      pushd $rootdir >/dev/null
      tar -jpc --exclude-caches --exclude-tag mas-tar-exclude -X $rootdir/tar-common.exculde -f "$savedirtarme/general.${now_stamp}.tar.bz2" files/ sh/
      tar -jpcf "$savedirtarme/git.${now_stamp}.tar.bz2" .git/
      tar -Jpcf "$savedirtarme/git.${now_stamp}.tar.z" .git/
#     tar -Zpcf "$savedirtarme/git.${now_stamp}.tar.Z" .git/
      tar -zpcf "$savedirtarme/git.${now_stamp}.tar.gz" .git/
      popd >/dev/null
    ;;
    *)
    ;;
    esac
  done
fi
