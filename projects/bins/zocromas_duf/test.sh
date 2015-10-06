#!/bin/sh
if [[ $MSH_SHN_PROJECT_DIR ]] && pushd $MSH_SHN_PROJECT_DIR >/dev/null ; then
  source ./.localrc
  td=$(realpath test/duf)
  wd=$td/`/bin/date '+%Y%m%d'`
  if ! [[ -d $wd ]] ; then
    mkdir $wd
  fi
  if [[ 1 -gt 2 ]] ; then
    if  pushd $wd >/dev/null ; then
      lshn
      echo ">>>>>>>>>> $(pwd)" >&2
      run @$td/test0.cmds
      echo ">>>>>>>>>> $(pwd)" >&2
      run @$td/test1f.cmds
      echo ">>>>>>>>>> $(pwd)" >&2
      run @$td/test2f.cmds
      echo ">>>>>>>>>> $(pwd)" >&2
      run @$td/test_tagit1.cmds
      echo ">>>>>>>>>> $(pwd)" >&2
      run @$td/test_tagit2.cmds
      echo ">>>>>>>>>> $(pwd)" >&2
      run @$td/test_with_tagsf.cmds
      echo ">>>>>>>>>> $(pwd)" >&2
      popd >/dev/null
    fi
  fi
  popd >/dev/null
fi
