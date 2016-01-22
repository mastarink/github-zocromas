#!/bin/sh
. shn/libfindedit.bash
. shn/libuuid.bash
. shn/libednew.bash
function mased_t ()
{
  local auto_mased_dir='[auto]' auto_localvim_dir='[auto]' auto_edcfg='[auto]'
  shn_file_edit_simple "$@"
}
mased_t "$@"
