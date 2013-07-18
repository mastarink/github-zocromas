#!/bin/sh
if [[ -f "${MAS_PROJECTS_DIR:-.}/sh/setup.sh" ]] ; then
  . ${MAS_PROJECTS_DIR:-.}/sh/setup.sh
  
  # show_setup
  function tarme ()
  {
    if [[ "$savedirtarme" ]] &&  [[ -d "$savedirtarme" ]] && [[ "$topdir" ]] && [[ -d "$topdir" ]] && [[ "$indir" ]] && [[ -d "$indir" ]] ; then
      if [[ "$mas_name" ]] && [[ "$mas_vers" ]] ; then
	cd $indir || return 1
	make -s distclean     || return 1  
	cd $topdir  || return 1
	if [[ -d "$mas_logdir" ]] ; then
	  echo "rm $mas_logdir/server.*.log" >&2
	  echo "rm $mas_logdir/client.*.log" >&2
	  rm -f $mas_logdir/server.*.log
	  rm -f $mas_logdir/client.*.log
	  rm -f $make_errdir/make.*.result
	fi     
    #   echo "Saving $indir/*.{bz2,gz} to $savedirtar" >&2
    #   mv $indir/*.{bz2,gz} "$savedirtar"
	cd $topdir  || return 1
	if [[ "$mas_name" ]] && [[ -d "$mas_name" ]] ; then
	  echo "Saving *.tar.{bz2,gz} to $savedirtarme" >&2
	  tar --wildcards --exclude '.*.sw*' --exclude-caches --exclude-tag mas-tar-exclude -jvcf "$savedirtarme/${mas_name}.${mas_vers}.`datemt`.tar.bz2" $mas_name/  || return 1
	fi
    #     mv *.tar.bz2 "$savedirtarme"     
	echo "Change to $indir to rebuild" >&2
	if cd "$indir" ; then
	  sh/autoreconf.sh || return 1
	  sh/configure.sh || return 1
	  sh/make.sh || return 1
	else
	  echo "Error change to $indir" >&2
	  return 1
	fi     
      else
	echo "No mas_name / mas_vers" >&2
	return 1
      fi    
    fi
    return 0
  }
  tarme $@
fi
