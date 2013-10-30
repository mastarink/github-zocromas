#!/bin/sh
function sqf ()
{
  local db=/mnt/new_misc/develop/autotools/zoc-new/duf_db/duf-photo.sqlite3
  sqlite3 "$db" <<SQL
.header on
.separator "	"
  $@;
SQL
}
function sqfiles ()
{
  local db=/mnt/new_misc/develop/autotools/zoc-new/duf_db/duf-photo.sqlite3
  sqlite3 "$db" <<SQL
.header on
.separator "	"
  SELECT duf_filedatas.id, duf_filedatas.dev, duf_filedatas.inode, duf_filedatas.md5id, duf_filedatas.size, duf_filenames.id, 
         duf_filenames.pathid, strftime('%%s',  duf_md5.now) as seconds, duf_md5.now, duf_filenames.name
  FROM duf_filedatas 
  LEFT JOIN duf_filenames ON (duf_filedatas.id=duf_filenames.dataid)
  LEFT JOIN duf_md5 ON (duf_filedatas.md5id=duf_md5.id) $@;
SQL
}
function sqfiledatas ()
{
  local db=/mnt/new_misc/develop/autotools/zoc-new/duf_db/duf-photo.sqlite3
  local sql='SELECT * FROM duf_filedatas'" $@"
  sqlite3 -header -column "$db" "$sql"
}
function sqfilenames ()
{
  local db=/mnt/new_misc/develop/autotools/zoc-new/duf_db/duf-photo.sqlite3
  sqlite3 "$db" << SQL
.header on
.separator "		"
SELECT id,name FROM duf_filenames $@;
SQL
}
