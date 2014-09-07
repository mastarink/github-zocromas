#!/bin/sh
function duftest ()
{
  local today=`datem`
  local now=`datemt`
  local tdir="$tdir0/$today"
  local testfile=$tdir/out${now}.test
  local cmpfile=$tdir0/good.test
  if [[ -d $tdir0 ]] ; then
    if ! [[ -d $tdir ]] ; then
      mkdir $tdir
    fi
    export MSH_CONF_DIR=$tdir0
    export MSH_DUF_OPTIONS=explain=0:trace-options=0:trace-action=0
    shn m i r -OPRdEinD -f523Xe  /mnt/new_media/media/photo/Pictures/photos/ --progress
    export MSH_DUF_OPTIONS=explain=0:trace-options=0:trace-action=0
    shn m i r   /mnt/new_media/media/photo/Pictures/photos/  -pd -RT -f --output-file=@$testfile
    if [[ -f $testfile ]] && [[ -f $cmpfile ]] ; then
      if diff $testfile $cmpfile ; then
        echo "Result file compare ok"
      fi
    else
      echo "No file $testfile OR $cmpfile"
    fi
  fi
}
function create_config ()
{
  local dbdir=$1
cat <<EEE >$tdir0/zocromas_duf.conf
trace-any=0
trace-stdout
trace-temp
uni-scan

# noopenat
db-directory=$dbdir

db-name=temp.db
db-name-adm=adm.db


# format=filesize,prefix,dirid,dirid_space,nfiles,nfiles_space,ndirs,ndirs_space,seq,nsame,md5,md5id,inode,filename,realpath,dataid,exifid,mimeid,nlink,human
format=filesize,mode,mtime,seq-leaf,nsame,md5,md5id,inode,filename,realpath,dataid,exifid,mimeid,mime,nlink,dirid,dirid_space,user,group,nameid,prefix
# trace-stderr
# trace-file=@${MSH_SHN_LAUNCHDATEMT}.trace-sql
# trace-file=@human/trace/${MSH_SHN_LAUNCHDATEM}.trace
EEE
}

function maintest ()
{
  local tdir00="test"
  local tdir0="$tdir00/duf"
    if [[ -f $tdir0/temp.db ]] ; then
      rm     $tdir0/temp.db
    fi
  create_config $tdir0
  duftest
  sqlite3 $tdir0/temp.db 'select "Paths" as Tb, count(*) as Cnt from paths UNION ALL select "Datas", count(*)  from filedatas UNION ALL select "Names", count(*) from filenames UNION ALL select "Sizes", count(*)  from sizes UNION ALL select "MD5s", count(*) from md5'
}

tput reset
echo "----------- $0 : ${BASH_SOURCE[@]}"
. shn/libwork.bash
maintest

# vi: ft=sh
