#!/bin/sh
function select_any()
{
  mysql  --host=mysql.mastar.lan  --user=masdufnt --password='i2xV9KrTA54HRpj4e' masdufntdb  --table --execute="$@" 
}
select_any "$@"
