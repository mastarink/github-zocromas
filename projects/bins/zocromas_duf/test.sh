#!/bin/sh

function test_duf ()
{
  database=${1:-test20140412}
  
  pwd
  echo "----- reset $LINENO $?"
  ls -l .localrc
  MSH_SHN_DISABLE_MSG=yes
  echo "----- reset $LINENO $?"
  source ./.localrc
  echo "----- reset $LINENO $?"

  shn m i r  --db-name=$database  --drop-tables --create-tables  --add-path /home/mastar/a/down/ >/tmp/test_duf.tmp
  echo "----- reset $LINENO $?"
  wc -l /tmp/test_duf.tmp

  sqlite3 /mnt/new_misc/develop/autotools/zoc-new/duf_db/$database .tables >/tmp/test_duf.tmp
  echo "----- $LINENO $?"
  wc -l /tmp/test_duf.tmp

  sqlite3 /mnt/new_misc/develop/autotools/zoc-new/duf_db/$database 'select * from duf_paths' >/tmp/test_duf.tmp
  echo "----- $LINENO $? paths before"
  wc -l /tmp/test_duf.tmp
  
  echo Filling $database
  shn m i r  --db-name=$database  --uni-scan /mnt/new_media/media/down/  -R  --fill >/tmp/test_duf.tmp
  echo "----- $LINENO $? fill"
  wc -l /tmp/test_duf.tmp

  shn m i r  --db-name=$database  --uni-scan /mnt/new_media/media/down/  -R  --print --tree --files >/tmp/test_duf.tmp
  echo "----- $LINENO $? print tree files"
  wc -l /tmp/test_duf.tmp

  shn m i r  --db-name=$database  --uni-scan /mnt/new_media/media/down/  -R  --print --tree >/tmp/test_duf.tmp
  echo "----- $LINENO $? print tree"
  wc -l /tmp/test_duf.tmp

  shn m i r  --db-name=$database  --uni-scan /mnt/new_media/media/down/tmp    --print  --files >/tmp/test_duf.tmp
  echo "----- $LINENO $? print files"
  wc -l /tmp/test_duf.tmp

  sqlite3 /mnt/new_misc/develop/autotools/zoc-new/duf_db/$database 'select * from duf_paths' >/tmp/test_duf.tmp
  echo "----- $LINENO $? paths after"
  wc -l /tmp/test_duf.tmp

  sqlite3 /mnt/new_misc/develop/autotools/zoc-new/duf_db/$database 'select * from duf_filenames' >/tmp/test_duf.tmp
  echo "----- $LINENO $? filenames after"
  wc -l /tmp/test_duf.tmp
}
time test_duf test20140412
