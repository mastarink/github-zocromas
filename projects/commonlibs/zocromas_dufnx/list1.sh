#!/bin/sh
function show_dufnx_list ()
{
  local sql="SELECT name, inode, atim, mtim, ctim, size, nsamesha1 as NS, hex_sha1 FROM dufnx_filefull, latest_updated as LU WHERE true $@"
  local sql="SELECT name, inode, mtim, size, nsamesha1 as NS, hex_sha1, latest_updated as LU FROM dufnx_filefull $@"
  echo "sql: $sql" >&2
  mysql -t -umasdufnt -pi2xV9KrTA54HRpj4e masdufntdb -e "$sql"
}
show_dufnx_list "$@" | nl
