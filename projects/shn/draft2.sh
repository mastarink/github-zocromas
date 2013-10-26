function draft2()
{
  local t=$1 ; shift
  local arg 
  local i
  return 0
  echo "-$0--$t [$0]-------" >&2
  i=0
  for arg in $* ; do
    echo "${i}. arg: $arg" >&2
    i=$(( $i + 1 ))
  done
  echo "--------" >&2
  for (( i=0 ; $i < ${#BASH_SOURCE[@]} ; i++ )) ; do
    echo "${i}. src1: ${BASH_SOURCE[$i]} : ${FUNCNAME[$i]}" >&2
  done
  echo "--------" >&2
  for (( i=0 ; $i < ${#BASH_SOURCE[@]} ; i++ )) ; do
    echo "${i}. src1a: ${BASH_SOURCE[$i]} : ${FUNCNAME[$i]}" >&2
  done
  echo "--------" >&2
  for (( i=0 ; $i < ${#BASH_SOURCE[@]} ; i++ )) ; do
    echo "${i}. src2: ${BASH_SOURCE[$i]} : ${FUNCNAME[$i]}" >&2
  done
  echo "--------" >&2
  for (( i=0 ; $i < ${#BASH_SOURCE[@]} ; i++ )) ; do
    echo "${i}. src2a: ${BASH_SOURCE[$i]} : ${FUNCNAME[$i]}" >&2
  done
  echo "--------" >&2
}
function mtest()
{
  local t=$1 ; shift
  echo "-$0--$t [mtest]-------" >&2
  local i=0
  for (( i=0 ; $i < ${#BASH_SOURCE[@]} ; i++ )) ; do
    echo "${i}. a src1: ${BASH_SOURCE[$i]} : ${FUNCNAME[$i]}" >&2
  done
}
function draft_a1()
{
  local t=$1 ; shift
  echo "-$0--$t [draft_a1 > 2]-------" >&2
  for (( i=0 ; $i < ${#BASH_SOURCE[@]} ; i++ )) ; do
    echo "${i}. a1 src1: ${BASH_SOURCE[$i]} : ${FUNCNAME[$i]}" >&2
  done
  draft2 $t $@
}
function draft_a()
{
  local t=$1 ; shift
  echo "-$0--$t [draft_a > a1]-------" >&2
  local i=0
  for (( i=0 ; $i < ${#BASH_SOURCE[@]} ; i++ )) ; do
    echo "${i}. a src1: ${BASH_SOURCE[$i]} : ${FUNCNAME[$i]}" >&2
  done
  draft_a1 $t $@
}
function draft_b1()
{
  local t=$1 ; shift
  echo "-$0--$t [draft_b1 > 2]-------" >&2
  draft2 $t $@
}
function draft_b()
{
  local t=$1 ; shift
  echo "-$0--$t [draft_b > b1]-------" >&2
  draft_b1 $t $@
}
return 0
