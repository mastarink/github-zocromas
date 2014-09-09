#!/bin/sh
function duftest ()
{
  local today=`datem`
  local now=`datemt`
  local tdir="$tdir0/$today"
  local testfile=$tdir/out${now}.test
  local cmpfile=$tdir0/good.test
  local MSH_SHN_DISABLE_MSG=1
  if [[ -d $tdir0 ]] ; then
    if ! [[ -d $tdir ]] ; then
      mkdir $tdir
    fi
    export MSH_CONF_DIR=$tdir0
    export MSH_DUF_OPTIONS=explain=0:trace-options=0:trace-action=0
    echo "============ test 0" >>${testfile}.log
    shn m i r --memusage  -OPRdEinD -f523Xe  /mnt/new_media/media/photo/Pictures/photos/ --progress >>${testfile}.log
    export MSH_DUF_OPTIONS=explain=0:trace-options=0:trace-action=0
    echo "============ test 1" >>${testfile}.log
    shn m i r --memusage  /mnt/new_media/media/photo/Pictures/photos/  -pd -RT -f --output-file=@${testfile}.1 >>${testfile}.log
    if [[ -f ${testfile}.1 ]] && [[ -f ${cmpfile}.1 ]] ; then
      if diff ${testfile}.1 ${cmpfile}.1  >>${testfile}.log ; then
        echo "file 1 compare ok" >>${testfile}.log
      else
        echo "file 1 compare fail ${testfile}.1 ${cmpfile}.1" >>${testfile}.log
      fi
    else
      echo "No file ${testfile}.1 OR ${cmpfile}.1"
    fi
    echo "============ test 2" >>${testfile}.log
    shn m i r --memusage  /mnt/new_media/media/photo/Pictures/photos/  -pd -RT -f --max-depth=1 --output-file=@${testfile}.2 >>${testfile}.log
    if [[ -f ${testfile}.2 ]] && [[ -f ${cmpfile}.2 ]] ; then
      if diff ${testfile}.2 ${cmpfile}.2  >>${testfile}.log ; then
        echo "file 2 compare ok" >>${testfile}.log
      else
        echo "file 2 compare fail ${testfile}.2 ${cmpfile}.2" >>${testfile}.log
      fi
    else
      echo "No file ${testfile}.2 OR ${cmpfile}.2"
    fi    
    echo "============ test 3" >>${testfile}.log
    shn m i r --memusage  /mnt/new_media/media/photo/Pictures/photos/  -pd -RT -f --max-depth=2 --output-file=@${testfile}.3 >>${testfile}.log
    if [[ -f ${testfile}.3 ]] && [[ -f ${cmpfile}.3 ]] ; then
      if diff ${testfile}.3 ${cmpfile}.3  >>${testfile}.log ; then
        echo "file 3 compare ok" >>${testfile}.log
      else
        echo "file 3 compare fail ${testfile}.3 ${cmpfile}.3" >>${testfile}.log
      fi
    else
      echo "No file ${testfile}.3 OR ${cmpfile}.3"
    fi    
    echo "============ test 4" >>${testfile}.log
    sqlite3 $tdir0/temp.db 'select "Paths" as Tb, count(*) as Cnt from paths UNION ALL select "Datas", count(*)  from filedatas UNION ALL select "Names", count(*) from filenames UNION ALL select "Sizes", count(*)  from sizes UNION ALL select "MD5s", count(*) from md5' 2>/dev/null >>${testfile}.4
    if [[ -f ${testfile}.4 ]] && [[ -f ${cmpfile}.4 ]] ; then
      if diff ${testfile}.4 ${cmpfile}.4  >>${testfile}.log ; then
        echo "file 4 compare ok" >>${testfile}.log
      else
        echo "file 4 compare fail ${testfile}.4 ${cmpfile}.4" >>${testfile}.log
      fi
    else
      echo "No file ${testfile}.4 OR ${cmpfile}.4"
    fi    
    echo "============ end " >>${testfile}.log
    echo "4 mem protocols for 0,1,2,3 tests"
    grep 'WMWMOMWMWM' ${testfile}.log
    echo "4 output comparison for 1,2,3,4 tests"
    grep 'file .* compare' ${testfile}.log
    echo
    echo
    if [[ `grep 'WMWMOMWMWM.*EMP MEMT' ${testfile}.log|wc -l` == 4 ]] ; then
      echo "Memory usage OK"
    else
      echo "Memory usage BAD"
    fi
    if [[ `grep 'file .* compare ok' ${testfile}.log|wc -l` == 4 ]]; then
      echo "Output OK"
    else
      echo "Output BAD"
    fi
    echo
    echo

    echo " see ${testfile}.log for log"
    echo
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
}

tput reset
echo "----------- $0 : ${BASH_SOURCE[@]}"
. shn/libwork.bash
maintest

# vi: ft=sh
