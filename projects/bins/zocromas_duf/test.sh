#!/bin/sh
if [[ $MSH_SHN_PROJECT_DIR ]] && pushd $MSH_SHN_PROJECT_DIR ; then
  if  source ./.localrc ; then
    shn_load
    if [[ 1 -gt 0 ]] ; then
      td=$(realpath test/duf/tests20151006)
      wd=$td/`/bin/date '+%Y%m%d'`
      if ! [[ -d $wd ]] ; then
	mkdir $wd
      fi
      if  pushd $wd  ; then
	echo ">>>>>>>>>> $(pwd)" >&2
	run --config-dir=$MSH_SHN_PROJECT_DIR --cmds-dir=$td @test0.cmds
	echo ">>>>>>>>>> $(pwd)" >&2
	run --config-dir=$MSH_SHN_PROJECT_DIR --cmds-dir=$td @test1f.cmds
	echo ">>>>>>>>>> $(pwd)" >&2
	run --config-dir=$MSH_SHN_PROJECT_DIR --cmds-dir=$td @test2f.cmds
	echo ">>>>>>>>>> $(pwd)" >&2
	run --config-dir=$MSH_SHN_PROJECT_DIR --cmds-dir=$td @test_tagit1.cmds
	echo ">>>>>>>>>> $(pwd)" >&2
	run --config-dir=$MSH_SHN_PROJECT_DIR --cmds-dir=$td @test_tagit2.cmds
	echo ">>>>>>>>>> $(pwd)" >&2
	run --config-dir=$MSH_SHN_PROJECT_DIR --cmds-dir=$td @test_with_tagsf.cmds
	echo ">>>>>>>>>> $(pwd)" >&2
	popd 
      fi
    fi
  fi
  popd 
fi
