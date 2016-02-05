#!/bin/sh
function sederr()
{
  local errfile=${1:-/tmp/shn_build.make.zocromas_duf.result}
  sed -ne "s@^\s\+from\s\+${MSH_SHN_PROJECT_DIR}/\(mod\|src\)/\+\([[:alpha:]_][[:alnum:]_]\+\.c\):\([[:digit:]]\+\).*\$@\2:\3@p" $errfile
  sed -ne "s@^${MSH_SHN_PROJECT_DIR}/\(mod\|src\)/\+\([[:alpha:]_][[:alnum:]_]\+\.c\):\([[:alnum:]]\+\):\([[:alnum:]]\+\):[[:blank:]]*error:.*@\1/\2:\3@p" $errfile
}
sederr "$*" | head -1
