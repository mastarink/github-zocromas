#!/bin/sh
function show_qstd_list ()
{
  local sql="SELECT name, inode, atim, mtim, ctim, size, nsamesha1 as NS, hex_sha1 FROM qstd_filefull, latest_updated as LU WHERE true $@"
  local sql="SELECT name, inode, mtim, size, nsamesha1 as NS, hex_sha1, latest_updated as LU FROM qstd_filefull $@"
  echo "sql: $sql" >&2
  mysql -t -umasdufnt -pi2xV9KrTA54HRpj4e masdufntdb -e "$sql"
}
show_qstd_list "$@" | nl
