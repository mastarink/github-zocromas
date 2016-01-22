unset shn_gvimer_plus_bin shn_gvimer_plus_fuuid shn_gvimer_plus_resident shn_gvimer_plus_uuid shn_gvimer_plus_regfile_in shn_gvimer_plus_regfile shn_gvimer_plus_anywhere shn_gvimer_plus_nomased shn_gvimer_plus_filtyp shn_gvimer_plus_find shn_gvimer_plus_vpath shn_find_c_mastar_function shn_gvimer_plus_mased shn_gvimer_plus shn_gvimer_plus_bin

function shn_file_edit ()
{
  if [[ "$@" ]] ; then
    shn_project_file_cd $@
# export MSH_SHN_DEBUG=yes
    shn_gvimer_plus "$@"
# unset MSH_SHN_DEBUG
    shn_project_cd -
    shn_dbgmsg "after shn_project_cd $LINENO $FUNCNAME" >&2
  else
    shn_gvimer_plus
  fi
}
