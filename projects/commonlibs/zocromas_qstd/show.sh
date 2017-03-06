#!/bin/sh
function select_table ()
{
  local table=$1

# mysql  -hmysql.mastar.lan  -umasdufnt -pi2xV9KrTA54HRpj4e masdufntdb  --skip-column-names
  mysql  --host=mysql.mastar.lan  --user=masdufnt --password='i2xV9KrTA54HRpj4e' masdufntdb  --table  <<SEL
SELECT * FROM ${table} LIMIT 45;
\\q
SEL
}
select_table 'filefull'
