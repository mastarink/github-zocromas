#!/bin/sh
function duftest()
{
  if [[ $MSH_SHN_PROJECT_DIR ]] && pushd $MSH_SHN_PROJECT_DIR &>/dev/null ; then
    export MSH_SHN_DISABLE_MSG=yes
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
	local dbfile=$dbdir/test_tree/test_tree.db
	if ! [[ -d $dbdir ]] ; then mkdir $dbdir ; fi
	if  pushd $wd &>/dev/null  ; then
 	  shn m i r --db-directory=$dbdir --config-dir=$confdir --cmds-dir=$td @test0.cmds
#	  shn m i r --disable-memusage --db-directory=$dbdir --config-dir=$confdir --cmds-dir=$td @test0a.cmds
	  echo "db at $dbdir" >&2
	  echo '.dump paths'                   | sqlite3 $dbfile > paths.sql
	  echo '.dump filedatas'               | sqlite3 $dbfile > filedatas.sql
	  echo '.dump filenames'               | sqlite3 $dbfile > filenames.sql
	  echo '.dump md5'                     | sqlite3 $dbfile > md5.sql
	  echo '.dump sha1'                    | sqlite3 $dbfile > sha1.sql
	  
	  cat <<SQL1 | sqlite3 -list $dbfile > paths.txt
.width 50
.headers off
.separator ", "
select dirname from paths;
SQL1
	  cat <<SQL2 | sqlite3 -list $dbfile > path_dirnames.txt
.width 6 50
.headers off
.separator ", "
select rowid, dirname from paths;
SQL2
	  cat <<SQL3 | sqlite3 -list $dbfile > filenames.txt
.width 50
.headers off
.separator ", "
select rowid, name from filenames;
SQL3
	  
	  shn m i r --disable-memusage --db-directory=$dbdir --config-dir=$confdir --cmds-dir=$td @test1f.cmds
	  shn m i r --disable-memusage --db-directory=$dbdir --config-dir=$confdir --cmds-dir=$td @test2f.cmds
	  shn m i r --disable-memusage --db-directory=$dbdir --config-dir=$confdir --cmds-dir=$td @test_tagit1.cmds
	  shn m i r --disable-memusage --db-directory=$dbdir --config-dir=$confdir --cmds-dir=$td @test_tagit2.cmds
	  shn m i r --disable-memusage --db-directory=$dbdir --config-dir=$confdir --cmds-dir=$td @test_with_tagsf.cmds
	  popd  &>/dev/null
	fi
      fi
    fi
    popd &>/dev/null
  fi
}
duftest $@
