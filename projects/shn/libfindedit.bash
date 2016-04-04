shn_find_c_mastar_def ()
{
  local name=$1
  shift
  local res=1
  local -a afileq
  if [[ $name =~ ^([[:alpha:]][[:alnum:]_]+)$ ]] &&  afileq=($( grep -rl --inc='*.[ch]' "^\s*#\s*define\>\s\+$name\>" $@ )) ; then
    echo ${afileq[0]}
    res=0
  fi
  return $res
}
shn_find_c_mastar_function ()
{
  local name=$1
  shift
  local finddirs="$@"
  
  local sname pname res=1
# local cmd="grep -rl --inc='*.c'"
  local afileq
  if [[ $name =~ ^([[:alpha:]][[:alnum:]_]+)$ ]] ; then
    if [[ $name =~ ^duf_(.*)$ ]] ; then
      sname=${BASH_REMATCH[1]}
      pname=$name
    else
      sname=$name
      pname="duf_${name}"
    fi
    shn_msg "is it function? sname:$sname; pname:$pname; name:$name"
    if afileq=($( grep -rl --inc='*.c' "^$name\>(" $finddirs )) \
    || afileq=($( grep -rl --inc='*.c' "^$pname\>(" $finddirs )) \
    || afileq=($( grep -rl --inc='*.c' "^DUF_WRAPPED(\s*${name}\>\s*)\s*(" $finddirs )) \
    || afileq=($( grep -rl --inc='*.c' "^\(SR\|SRX\|SRN\|SRP\|SRP0\)(\s*[A-Z]\+,.*\<${sname}\>" $finddirs )) ; then
      echo $afileq
      res=0
    fi
  fi
  return $res
}
