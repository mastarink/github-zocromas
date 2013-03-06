for d in zoc*/mased commonlibs/*/mased bins/*/mased ; do
  if [[ -d $d ]] ; then
    pushd $d >/dev/null
    pwd
    ln -s $( realpath --relative-to . /home/mastar/.mas/develop/autotools/zoc/zocromas/mased/sh.mased.vim )
    popd >/dev/null
  fi
done
