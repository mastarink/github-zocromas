#!/bin/sh
function duftest()
{
  if [[ $MSH_SHN_PROJECT_DIR ]] && pushd $MSH_SHN_PROJECT_DIR ; then
    if  source ./.localrc ; then
      shn_load
      if [[ 1 -gt 0 ]] ; then
	td=$(realpath test/duf/tests20151006)
	wd=$td/`/bin/date '+%Y%m%d'`
	if ! [[ -d $wd ]] ; then
	  mkdir $wd
	fi
	local confdir=$td/dufconf
	local dbdir=$td/db
	if ! [[ -d $dbdir ]] ; then mkdir $dbdir ; fi
	if  pushd $wd  ; then
	  shn m i r --db-directory=$dbdir --config-dir=$confdir --cmds-dir=$td @test0.cmds
	  echo '.dump paths'     | sqlite3 $dbdir/test_tree.db > paths.sql
	  echo '.dump filedatas' | sqlite3 $dbdir/test_tree.db > filedatas.sql
	  echo '.dump filenames' | sqlite3 $dbdir/test_tree.db > filenames.sql
	  echo '.dump md5'       | sqlite3 $dbdir/test_tree.db > md5.sql
	  echo '.dump sha1'      | sqlite3 $dbdir/test_tree.db > sha1.sql
 return 1
	  shn m i r --db-directory=$dbdir --config-dir=$confdir --cmds-dir=$td @test1f.cmds
	  shn m i r --db-directory=$dbdir --config-dir=$confdir --cmds-dir=$td @test2f.cmds
	  shn m i r --db-directory=$dbdir --config-dir=$confdir --cmds-dir=$td @test_tagit1.cmds
	  shn m i r --db-directory=$dbdir --config-dir=$confdir --cmds-dir=$td @test_tagit2.cmds
	  shn m i r --db-directory=$dbdir --config-dir=$confdir --cmds-dir=$td @test_with_tagsf.cmds
	  popd 
	fi
      fi
    fi
    popd 
  fi
}
duftest $@
