#!/bin/sh
function mas_clear_history ()
{
  local prefix=$1 num xx
  echo "prefix:$prefix" >&2
  if [[ $prefix ]] ; then
    num=$( history | grep '^[[:blank:]]*[[:digit:]]\+[[:blank:]]\+[[:digit:]]\+\.[[:digit:]]\+:[[:digit:]]\+:[[:digit:]]\+[[:blank:]]\+:'$prefix'\(\>\|$\|[[:blank:]]\)' | wc -l )
    echo "... of $num" >&2
    while xx=$( history | \
    	grep '^[[:blank:]]*[[:digit:]]\+[[:blank:]]\+[[:digit:]]\+\.[[:digit:]]\+:[[:digit:]]\+:[[:digit:]]\+[[:blank:]]\+:'$prefix'\(\>\|$\|[[:blank:]]\)' | \
	head -1 | \
	awk '{print $1}' ) \
     && [[ $xx ]]
    do
      echo "#$xx of $num" >&2
      history -d $xx
   done
 fi
}
function mas_clear_history2 ()
{
  local prefix=$1 num n
  echo -n .
  if [[ "$prefix" ]] ; then
    num=$( history | tac | grep '^[[:blank:]]*[[:digit:]]\+[[:blank:]]\+[[:digit:]]\+\.[[:digit:]]\+:[[:digit:]]\+:[[:digit:]]\+[[:blank:]]\+:'$prefix'\(\>\|$\|[[:blank:]]\)' | wc -l )
    if [[ $num -gt 0 ]] ; then
      echo "prefix:$prefix  ... of $num" >&2
      for n in $( history | tac | \
	  grep '^[[:blank:]]*[[:digit:]]\+[[:blank:]]\+[[:digit:]]\+\.[[:digit:]]\+:[[:digit:]]\+:[[:digit:]]\+[[:blank:]]\+:'$prefix'\(\>\|$\|[[:blank:]]\)' | \
	  awk '{print $1}' )
      do
	echo "#$n of $num" >&2
	history -d $n
      done
    fi
  fi
}
function mas_clear_history_na ()
{
  local  num n
  echo -n .
  num=$( history | tac | grep '^[[:blank:]]*[[:digit:]]\+[[:blank:]]\+[[:digit:]]\+\.[[:digit:]]\+:[[:digit:]]\+:[[:digit:]]\+[[:blank:]]\+:\(\\\|[^a-z0-9\.]\)' | wc -l )
  if [[ $num -gt 0 ]] ; then
    echo "... of $num" >&2
    for n in $( history | tac | \
	grep  '^[[:blank:]]*[[:digit:]]\+[[:blank:]]\+[[:digit:]]\+\.[[:digit:]]\+:[[:digit:]]\+:[[:digit:]]\+[[:blank:]]\+:\(\\\|[^a-z0-9\.]\).*\(\>\|$\|[[:blank:]]\)' | \
	awk '{print $1}' )
    do
      echo "#$n of $num" >&2
      history -d $n
    done
  fi
}

function mas_clear_history_mult ()
{
  local prefix
  for prefix in $@ ; do
    mas_clear_history2 "$prefix"
  done
}
# history | grep -v '\<\(run\|git\|find\|shn\|history\|tput\|lshn\|exiftool\|jpeginfo\|exifcom\|exiftags\|exifgthumb\|ldd\|export\|unset\|cpp\)\>'
# source  shn/slow_clean_history.sh  'euses'
# source  shn/slow_clean_history.sh grep find gthumb e g j cat rm eix euses gcc-config zocromas_runonce gvim me r-tag wcalc apropos date locate gunzip gzip gz gh gimp r DO_AT_STAGE eom complete touch cp rmm ptgz declare alias mv tree set echp echo rmdir mkdir m cmp gc eselect type ddd q pwd pigz unpigz cd ee w duf_dir_scan2_stages.c duf_item_scan2.c duf_action.c gcc kj most ifnames fg ter, if his lshb which evim du df vimdot exif qgit4 gitstats gitview em same-as test/sh/test20140907.113444.sh file md5sum chmod  man diff realpath data- k xxd .auxdir/.build/config.status e_WRAP_FUNC ls wget cal src/duf.c  head pstat gg qlist qfile crc32 head pstat p-PO  mplayer ps htop gf qa eduf_dir_scan2_stages.c gpep hg ech sq1 sqlite3 chmo gmin5ID h src/inc/duf_option_types.h shopt bash cscope ctags notifymas select for time shn_errmsg dsfdsf mme replace automake autoscan autoconf aclocal lsnn unxet tuxcards gduf_qscan_dirs_by_dirid2 prm wc exp qls sho esrc/inc/duf_option_types.h duf_option_showflags ln shopty g_DIR local locale f env dsad commi p tail fdisk propos when e_FLAG_INTEGRITYe tzselect strftimr strftime mvcp gINDE lsdb ldbs sql host src/duf_dir_scan2_stages.c duf_md5id_match .auxdir/.build/src/zocromas_duf make selecy sha512sum sha512 x alais mshterminal.sh stat daten xx whatis ne sha1sum mount fallocate ew ll last lasr xset pstree e286 gitcommit grepchp grepchp ail yail esrc autoreconf xclip symtree gitcli treesize shn_project_version sq rty rix gitk gitg remame nm dq1 exit sqlite2 readlink tty
mas_clear_history_na
mas_clear_history_mult $@
