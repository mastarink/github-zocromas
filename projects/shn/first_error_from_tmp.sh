#!/bin/sh
sed -ne "s@^${MSH_SHN_PROJECT_DIR}/\(mod\|src\)/\+\([[:alpha:]_][[:alnum:]_]\+\.c\):\([[:alnum:]]\+\):\([[:alnum:]]\+\):[[:blank:]]*error:.*@\1/\2:\3@p" /tmp/shn_build.make.zocromas_duf.result | head -1

