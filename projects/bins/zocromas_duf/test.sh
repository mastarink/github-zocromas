#!/bin/sh
function sed1 ()
{
  sed  "s@'$(/bin/date '+%Y-%m-%d')[[:blank:]]\+[[:digit:]]\{2\}:[[:digit:]]\{2\}:[[:digit:]]\{2\}\.[[:digit:]]\{3\}');\$@@"
}
function sed2 ()
{
  sed  "s@'$(/bin/date '+%Y-%m-%d')[[:blank:]]\+[[:digit:]]\{2\}:[[:digit:]]\{2\}:[[:digit:]]\{2\}',\$@@"
}

function duftest()
{
  unset MSH_DUF_OPTIONS
  if [[ $MSH_SHN_PROJECT_DIR ]] && pushd $MSH_SHN_PROJECT_DIR &>/dev/null ; then
    export MSH_SHN_DISABLE_MSG=yes
    export MSH_SHN_DISABLE_TIMING=yes
    export MSH_SHN_DISABLE_MARKLINE=yes
    if  source ./.localrc ; then
      shn_load
      if [[ 1 -gt 0 ]] ; then
	td=$(realpath test/duf/tests20151019)
	wd=$td/`/bin/date '+%Y%m%d'`
	if ! [[ -d $wd ]] ; then
	  mkdir $wd
	fi
	local retcode
	local confdir=$td/dufconf
	local dbdir=$td/db
	local matchdir=$td/match
	local dbfile=$dbdir/test_tree/test_tree.db
	if ! [[ -d $dbdir ]] ; then mkdir $dbdir ; fi
	if ! [[ -d $matchdir ]] ; then mkdir $matchdir ; fi
	if  pushd $wd &>/dev/null  ; then
	  export MSH_DUF_OPTIONS=disable-memusage:trace-temp=-100
	  rm *.tmp
	  
	  shn m i r --trace-file=@wronkopt1.tmp  -dfRu  --db-name=test_tree  --evaluate=taree test/tree/ -B --verbose ; retcode=$?
	  if [[ $retcode -ne 31 ]] ; then echo "test returned: $retcode" >&2 ; fi
	  if ! [[ -f $matchdir/wronkopt1.tmp ]] || ! diff $matchdir/wronkopt1.tmp wronkopt1.tmp > diff1.tmp ; then echo "test error $matchdir/wronkopt1.tmp" >&2 ; fi
	  
	  shn m i r --trace-file=@wronkopt2.tmp  -dfRu  --db-name=test_tree  --evaluate=tree test/tree/ --veerbose ; retcode=$?
	  if [[ $retcode -ne 31 ]] ; then echo "test returned: $retcode" >&2 ; fi
	  if ! [[ -f $matchdir/wronkopt2.tmp ]] || ! diff $matchdir/wronkopt2.tmp wronkopt2.tmp > diff2.tmp ; then echo "test error $matchdir/wronkopt2.tmp" >&2 ; fi
	  
	  shn m i r --trace-file=@wronkopt3.tmp  -dfRu  --db-name=test_tree  --evaluate=tree test/tree/ -B --verbose ; retcode=$?
	  if [[ $retcode -ne 31 ]] ; then echo "test returned: $retcode" >&2 ; fi
	  if ! [[ -f $matchdir/wronkopt3.tmp ]] || ! diff $matchdir/wronkopt3.tmp wronkopt3.tmp > diff3.tmp ; then echo "test error $matchdir/wronkopt3.tmp" >&2 ; fi
	  
	  shn m i r --trace-file=@wronkopt4.tmp  -dfRu  --db-name=test_tree  --evaluate=taree test/tree/ -B --veerbose ; retcode=$?
	  if [[ $retcode -ne 31 ]] ; then echo "test returned: $retcode" >&2 ; fi
	  if ! [[ -f $matchdir/wronkopt4.tmp ]] || ! diff $matchdir/wronkopt4.tmp wronkopt4.tmp > diff4.tmp ; then echo "test error $matchdir/wronkopt4.tmp" >&2 ; fi
 	  
	  shn m i r --trace-file=@wronkopt5.tmp --db-directory=$dbdir --config-dir=$confdir --cmds-dir=$td @test0.duf ; retcode=$?
	  if [[ $retcode -ne 0 ]] ; then echo "test returned: $retcode" >&2 ; fi
	  if ! [[ -f $matchdir/wronkopt5.tmp ]] || ! diff $matchdir/wronkopt5.tmp wronkopt5.tmp > diff5.tmp ; then echo "test error $matchdir/wronkopt5.tmp" >&2 ; fi

#	  shn m i r --disable-memusage --db-directory=$dbdir --config-dir=$confdir --cmds-dir=$td @test0a.duf
	  echo "db at $dbdir" >&2
	  echo '.dump paths'                   | sqlite3 $dbfile | sed1 | sed2 > paths.sql.tmp
	  if ! [[ -f $matchdir/paths.sql.tmp ]] || ! diff $matchdir/paths.sql.tmp paths.sql.tmp > diff-paths.sql.tmp ; then echo "test error $matchdir/paths.sql.tmp" >&2 ; fi
	  echo '.dump filedatas'               | sqlite3 $dbfile | sed1 | sed2 > filedatas.sql.tmp
	  if ! [[ -f $matchdir/filedatas.sql.tmp ]] || ! diff $matchdir/filedatas.sql.tmp filedatas.sql.tmp > diff-filedatas.sql.tmp ; then echo "test error $matchdir/filedatas.sql.tmp" >&2 ; fi
	  echo '.dump filenames'               | sqlite3 $dbfile | sed1 | sed2 > filenames.sql.tmp
	  if ! [[ -f $matchdir/filenames.sql.tmp ]] || ! diff $matchdir/filenames.sql.tmp filenames.sql.tmp > diff-filenames.sql.tmp ; then echo "test error $matchdir/filenames.sql.tmp" >&2 ; fi
	  echo '.dump md5'                     | sqlite3 $dbfile | sed1 | sed2 > md5.sql.tmp
	  if ! [[ -f $matchdir/md5.sql.tmp ]] || ! diff $matchdir/md5.sql.tmp md5.sql.tmp > diff-md5.sql.tmp ; then echo "test error $matchdir/md5.sql.tmp" >&2 ; fi
	  echo '.dump sha1'                    | sqlite3 $dbfile | sed1 | sed2 > sha1.sql.tmp
	  if ! [[ -f $matchdir/sha1.sql.tmp ]] || ! diff $matchdir/sha1.sql.tmp sha1.sql.tmp > diff-sha1.sql.tmp ; then echo "test error $matchdir/sha1.sql.tmp" >&2 ; fi
	  
	  cat <<SQL1 | sqlite3 -list $dbfile > paths.txt.tmp
.width 50
.headers off
.separator ", "
select dir_name from paths;
SQL1
	  if ! [[ -f $matchdir/paths.txt.tmp ]] || ! diff $matchdir/paths.txt.tmp paths.txt.tmp > diff-paths.txt.tmp ; then echo "test error $matchdir/paths.txt.tmp" >&2 ; fi

	  cat <<SQL2 | sqlite3 -list $dbfile > path_dirnames.txt.tmp
.width 6 50
.headers off
.separator ", "
select rowid, dir_name from paths;
SQL2
	  if ! [[ -f $matchdir/path_dirnames.txt.tmp ]] || ! diff $matchdir/path_dirnames.txt.tmp path_dirnames.txt.tmp > diff-path_dirnames.txt.tmp ; then echo "test error $matchdir/path_dirnames.txt.tmp" >&2 ; fi

	  cat <<SQL3 | sqlite3 -list $dbfile > filenames.txt.tmp
.width 50
.headers off
.separator ", "
select rowid, file_name from filenames;
SQL3
	  if ! [[ -f $matchdir/filenames.txt.tmp ]] || ! diff $matchdir/filenames.txt.tmp filenames.txt.tmp > diff-filenames.txt.tmp ; then echo "test error $matchdir/filenames.txt.tmp" >&2 ; fi
	  
	  shn m i r --disable-memusage --db-directory=$dbdir --config-dir=$confdir --cmds-dir=$td @test1f.duf
	  shn m i r --disable-memusage --db-directory=$dbdir --config-dir=$confdir --cmds-dir=$td @test2f.duf
	  shn m i r --disable-memusage --db-directory=$dbdir --config-dir=$confdir --cmds-dir=$td @test_tagit1.duf
	  shn m i r --disable-memusage --db-directory=$dbdir --config-dir=$confdir --cmds-dir=$td @test_tagit2.duf
	  shn m i r --disable-memusage --db-directory=$dbdir --config-dir=$confdir --cmds-dir=$td @test_with_tagsf.duf
	  popd  &>/dev/null
	fi
      fi
    fi
    popd &>/dev/null
  fi
}
duftest $@
