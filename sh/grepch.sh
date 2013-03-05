#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh
  function grepch ()
  {
    local project projects
    if [[ "$rootdir" ]] && [[ -d "$rootdir" ]] ; then
      cd $rootdir || return 1
      if [[ -f "projects.list" ]] ; then
        projects=$( echo "`cat projects.list`" )
#       while read project ; do
#         if [[ "$projects" ]] ; then
#           projects="$projects,$project"
#         else
#           projects="$project"
#         fi
#       done < projects.list 
#	echo "projects: $projects"


#       find $projects \( -name .build -prune \) -o -type f -name '*.[ch]' -okdir grep -H --color $@ \{\} \; || return 1
        find $projects \( -name .build -prune \) -o -type f -name '*.[ch]' -execdir grep -H --color $@ \{\} 2>/dev/null \+ || return 1
      else
        return 1
  #   grep --color=yes -r --inc='*.[ch]' "$@" {commonlibs,bins,zoc*}
  #   find {commonlibs,bins,zoc*} -not -path '*/.build/*' -type f -name '*.[ch]' -execdir grep -H --color $@ \{\} \; | sed -ne 's@^\.\/@@p'
#     find {commonlibs,bins,zoc*} -not -path '*/.build/*' -type f -name '*.[ch]' -execdir grep -H --color $@ \{\} \+
      fi
    else
      return 1
    fi
    return 0
  }
  grepch $@
fi
