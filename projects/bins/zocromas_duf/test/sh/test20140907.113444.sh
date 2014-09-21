#!/bin/sh
function unitestid ()
{
  local testname=$1 
  testid=$testname
  if [[ "$testname" =~ ^test(.*)$ ]] ; then
    testid=${BASH_REMATCH[1]}
  fi
  echo "============ [testid=$testid] ($testname)" >&2
  echo "============ [testid=$testid] ($testname)"
}
function unicmp ()
{
# echo "comparing $testid" >&2
  if [[ "$testid" ]] && [[ -f ${testfile}.${testid} ]] && [[ -f ${cmpfile}.${testid} ]] ; then
    if diff ${testfile}.${testid} ${cmpfile}.${testid}  >>${testfile}.log ; then
#     echo "[testid=$testid] compare ok" >&2
      echo "[testid=$testid] compare ok"
      return 0
    else
#     echo "[testid=$testid] compare fail ${testfile}.${testid} ${cmpfile}.${testid}" >&2
      echo "[testid=$testid] compare fail ${testfile}.${testid} ${cmpfile}.${testid}"
    fi
  else
#   echo "[testid=$testid] No file ${testfile}.${testid} OR ${cmpfile}.${testid}" >&2
    echo "[testid=$testid] No file ${testfile}.${testid} OR ${cmpfile}.${testid}"
  fi
  return 1
}
function testcreadd ()
{
  local testid='x'
  unitestid $FUNCNAME
  shn m i r --memusage  -OP  /mnt/new_media/media/photo/Pictures/photos/ || return 1
  echo "add" >${testfile}.${testid}
  unicmp $FUNCNAME
}
function test0 ()
{
  local testid='x'
  unitestid $FUNCNAME
  shn m i r --memusage  -OPRdEinD -f523Xe  /mnt/new_media/media/photo/Pictures/photos/ --progress || return 1
  echo "scan" >${testfile}.${testid}
  unicmp $FUNCNAME
}
function test1 ()
{
  local testid='x'
  unitestid $FUNCNAME
  
  shn m i r --memusage  /mnt/new_media/media/photo/Pictures/photos/  -pd -RT -f --output-file=@${testfile}.${testid} || return 1
  unicmp $FUNCNAME
}
function test2 ()
{
  local testid='x'
  unitestid $FUNCNAME
  shn m i r --memusage  /mnt/new_media/media/photo/Pictures/photos/  -pd -RT -f --max-depth=1 --output-file=@${testfile}.${testid} || return 1
  unicmp $FUNCNAME
}
function test3 ()
{
  local testid='x'
  unitestid $FUNCNAME
  shn m i r --memusage  /mnt/new_media/media/photo/Pictures/photos/  -pd -RT -f --max-depth=2 --output-file=@${testfile}.${testid} || return 1
  unicmp $FUNCNAME
}
function test4 ()
{
  local testid='x'
  unitestid $FUNCNAME
  shn m i r --memusage /mnt/new_media/media/photo/Pictures/photos/sel/ -pd -R -f --size=-500 --min-size=1 --output-file=@${testfile}.${testid} || return 1
  unicmp $FUNCNAME
}

function testsql1 ()
{
  local testid='x'
  unitestid $FUNCNAME
  echo 'FAKED WMWMOMWMWM EMP MEMT'
  time sqlite3 $tdir0/temp.db 'select "Paths" as Tb, count(*) as Cnt from paths UNION ALL select "Datas", count(*)  from filedatas UNION ALL select "Names", count(*) from filenames UNION ALL select "Sizes", count(*)  from sizes UNION ALL select "MD5s", count(*) from md5' 2>/dev/null >${testfile}.${testid}
  unicmp $FUNCNAME
}
function duftest ()
{
  local today=`datem`
  local now=`datemt`
  local tdir="$tdir0/$today"
  local testfile=$tdir/out${now}.test
  local cmpfile=$tdir0/good.test
  local MSH_SHN_DISABLE_MSG=1
  local oktests=0
  local alltests=0
  if [[ -d $tdir0 ]] ; then
    if ! [[ -d $tdir ]] ; then
      mkdir $tdir
    fi
    export MSH_CONF_DIR=$tdir0
    export MSH_DUF_OPTIONS=explain=0:trace-options=0:trace-action=0
    declare -a duftests
    duftests=(testcreadd test0 test1 test2 test3 test4 testsql1)
    echo "To make ${#duftests[@]} tests"
    for testf in ${duftests[@]} ; do    
      echo
      echo "########### run $testf #######################################"
      eval $testf && let oktests++
      let alltests++
    done >>${testfile}.log
    echo "============ end " >>${testfile}.log
    echo "----------------------------------"
    if [[ $oktests -eq $alltests ]] ; then
      echo "OK tests ${MSHPR_BBLUE}$oktests of $alltests${MSHPR_ATTROFF}"
    else
      echo "OK tests ${MSHPR_BYELLOWONRED}$oktests of $alltests${MSHPR_ATTROFF}"
    fi
    
    echo "----------------------------------"
    echo "$alltests mem protocols for all tests"
    local goodmem=`grep 'WMWMOMWMWM.*EMP MEMT' ${testfile}.log|wc -l`
    if [[ ${goodmem:-0} -eq $alltests ]] ; then
      echo "Memory usage ${MSHPR_BBLUE}OK${MSHPR_ATTROFF}"
    else
      echo "Memory usage ${MSHPR_BYELLOWONRED}BAD (good $goodmem of $alltests)${MSHPR_ATTROFF}"
    fi


    echo "----------------------------------"
    echo "$alltests output comparison for all tests"
    local goodcomapred=`grep '\[testid=.*\] compare ok' ${testfile}.log|wc -l`
    if [[ ${goodcomapred:-0} -eq $alltests ]]; then
      echo "Output ${MSHPR_BBLUE}OK${MSHPR_ATTROFF}"
    else
      echo "Output ${MSHPR_BYELLOWONRED}BAD (good $goodcomapred of $alltests)${MSHPR_ATTROFF}"
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
