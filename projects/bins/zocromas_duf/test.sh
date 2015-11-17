#!/bin/sh
function sed1 ()
{
  sed  "s@'$(/bin/date '+%Y-%m-%d')[[:blank:]]\+[[:digit:]]\{2\}:[[:digit:]]\{2\}:[[:digit:]]\{2\}\.[[:digit:]]\{3\}');\$@@"
}
function sed2 ()
{
  sed  "s@'$(/bin/date '+%Y-%m-%d')[[:blank:]]\+[[:digit:]]\{2\}:[[:digit:]]\{2\}:[[:digit:]]\{2\}',\$@@"
}
function sed3 ()
{
  sed  "s@\(VALUES([^,]\+,[^,]\+,[^,]\+,[^,]\+,[^,]\+,[^,]\+,[^,]\+,[^,]\+,[^,]\+\)\(,'$(/bin/date '+%Y-%m-%d')[[:blank:]]\+[[:digit:]]\{2\}:[[:digit:]]\{2\}:[[:digit:]]\{2\}'\)@\1@"
}
function comparetest ()
{
  local ttype=$1 ; shift
  local name=$1 ; shift
  if ! [[ -f $matchdir/${ttype}.${name}.tmp ]] ; then
    echo "test error $(realpath --relative-to=$test_at $matchdir/${ttype}.${name}.tmp)" >&2
    echo "cp -a $(realpath --relative-to=$test_at ${ttype}.${name}.tmp) $(realpath --relative-to=$test_at $matchdir/)" >&2
    cp -a $(realpath  ${ttype}.${name}.tmp) $(realpath  $matchdir/)
  elif ! diff $matchdir/${ttype}.${name}.tmp ${ttype}.${name}.tmp > diff.${ttype}.${name}.tmp ; then
    echo "test error $(realpath --relative-to=$test_at diff.${ttype}.${name}.tmp)" >&2
    echo "--- $(realpath $matchdir/${ttype}.${name}.tmp)" >&2
    echo "--- $(realpath ${ttype}.${name}.tmp)" >&2
  else
    echo -n '.' >&2
  fi
}
function cmdtest_x ()
{
  local -i retcode=0
  local name=$1
  shift
  local -i okret=$1
  shift
  local  extra=$1
  shift
  local cmd
  if [[ $name ]] ; then
    export MSH_DUF_OPTIONS=disable-memusage:trace-temp=-100
    cmd="run ${extra} --trace-file=@trace.${name}.tmp --config-dir=$confdir --db-directory=$dbdir --cmds-dir=$td $@"
  # echo "$cmd" >&2
#   echo -n '<' >&2
    eval $cmd ; retcode=$?
#   echo -n '>' >&2
    
    if [[ $retcode -ne $okret ]] ; then
      echo "test returned: $retcode; must be $okret" >&2
    else
      echo -n '.' >&2
    fi  
    comparetest trace $name
    comparetest out $name
  else
    echo "$FUNCNAME error: no test name" >&2
  fi
  return $retcode
}
function cmdtest ()
{
  local name=$1
  shift
  local -i okret=$1
  shift
  if [[ $name ]] ; then
    cmdtest_x "$name" $okret ' ' $@
  else
    echo "$FUNCNAME error: no test name" >&2
  fi
}
function cmdtest_o ()
{
  local name=$1
  shift
  local -i okret=$1
  shift
  if [[ $name ]] ; then
    cmdtest_x "$name" $okret --output-file=@out.${name}.tmp $@
  else
    echo "$FUNCNAME error: no test name" >&2
  fi
}
function duftest ()
{
  unset MSH_DUF_OPTIONS
  local test_at=$(pwd)
  if [[ $MSH_SHN_PROJECT_DIR ]] && pushd $MSH_SHN_PROJECT_DIR &>/dev/null ; then
    if [[ -d test/tree ]] ; then
      rm -Rf test/tree/
    fi
#   if ! [[ -d test/tree ]] && cp -ar test/tree.r test/tree && [[ -d test/tree ]] && source ./.localrc ; then
    if ! [[ -d test/tree ]] && [[ -f test/test-tree.tar.gz ]] && tar -zxf test/test-tree.tar.gz && [[ -d test/tree ]] && source ./.localrc ; then
      export MSH_SHN_DISABLE_MSG=yes
      export MSH_SHN_DISABLE_TIMING=yes
      export MSH_SHN_DISABLE_MARKLINE=yes
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
	if [[ -d $wd ]] && [[ -d $dbdir ]] && [[ -d $matchdir ]] &&  pushd $wd &>/dev/null  ; then
	  rm -f *.tmp 
	  cmdtest_o err1 31                        -dfRu  --db-name=test_tree  --evaluate=taree test/tree/ -B --verbose
	  cmdtest_o err2 31                        -dfRu  --db-name=test_tree  --evaluate=tree test/tree/ --veerbose
	  cmdtest_o err3 31                        -dfRu  --db-name=test_tree  --evaluate=tree test/tree/ -B --verbose
	  cmdtest_o err4 31                        -dfRu  --db-name=test_tree  --evaluate=taree test/tree/ -B --veerbose
	  cmdtest_o test0 0 @test0.duf
	  cmdtest test1f  0 @test1f.duf
	  cmdtest test2f  0 @test2f.duf
	  cmdtest test3f  0 @test3f.duf


#	  shn m i r --disable-memusage --db-directory=$dbdir --config-dir=$confdir --cmds-dir=$td @test0a.duf
	  echo "db at $dbdir" >&2
	  echo '.dump paths'                   | sqlite3 $dbfile | sed1 | sed2 | sed3 > db.paths.sql.tmp
	  
	  comparetest db paths.sql
	  echo '.dump filedatas'               | sqlite3 $dbfile | sed1 | sed2 | sed3 > db.filedatas.sql.tmp
# date, inode!	  comparetest db filedatas.sql
	  echo '.dump filenames'               | sqlite3 $dbfile | sed1 | sed2 | sed3 > db.filenames.sql.tmp
	  comparetest db filenames.sql
	  echo '.dump md5'                     | sqlite3 $dbfile | sed1 | sed2 | sed3 > db.md5.sql.tmp
	  comparetest db md5.sql
	  echo '.dump sha1'                    | sqlite3 $dbfile | sed1 | sed2 | sed3 > db.sha1.sql.tmp
	  comparetest db sha1.sql
	  
	  cat <<SQL | sqlite3 -list $dbfile > table.paths.txt.tmp
.width 50
.headers off
.separator ", "
select dir_name from paths;
SQL
	  comparetest table paths.txt

	  cat <<SQL | sqlite3 -list $dbfile > table.path_dirnames.txt.tmp
.width 6 6 50
.headers off
.separator ", "
select rowid, parentid, dir_name from paths;
SQL
	  comparetest table path_dirnames.txt

	  cat <<SQL | sqlite3 -list $dbfile > table.filenames.txt.tmp
.width 50
.headers off
.separator ", "
select rowid, file_name from filenames;
SQL
	  comparetest table filenames.txt

	  cat <<SQL | sqlite3 -list $dbfile > table.md5.txt.tmp
.width 50
.headers off
.separator ", "
SELECT file_name, dup5cnt  FROM filenames AS fn LEFT JOIN filedatas AS fd on (fn.dataid=fd.rowid) LEFT JOIN md5 AS md ON (fd.md5id=md.rowid);
SQL
	  comparetest table md5.txt
	  
	  cat <<SQL | sqlite3 -list $dbfile > table.md5a.txt.tmp
.width 50
.headers off
.separator ", "
SELECT file_name, dup5cnt  FROM filenames AS fn LEFT JOIN filedatas AS fd on (fn.dataid=fd.rowid) LEFT JOIN md5 AS md ON (fd.md5id=md.rowid) WHERE dup5cnt>10;
SQL
	  comparetest table md5a.txt
	  

	  cmdtest_o test_tagit1 0 @test_tagit1.duf
	  cmdtest_o test_tagit2 0 @test_tagit2.duf
	  cmdtest_o test_with_tags 0 @test_with_tagsf.duf
	  cmdtest test_with_tagsf 0 @test_with_tagsf.duf
	  popd  &>/dev/null
	fi
      fi
    fi
    popd &>/dev/null
  fi
}
duftest $@
