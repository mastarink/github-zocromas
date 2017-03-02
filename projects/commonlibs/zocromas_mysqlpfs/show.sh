#!/bin/sh
function select_table ()
{
  local table=$1

# mysql  -hmysql.mastar.lan  -umasdufnt -pi2xV9KrTA54HRpj4e masdufntdb  --skip-column-names
  mysql  --host=mysql.mastar.lan  --user=masdufnt --password='i2xV9KrTA54HRpj4e' masdufntdb  --table  <<SEL
SELECT * from ${table};
\\q
SEL
}
select_table 'filenames'
