#!/bin/sh
function sqf_paths ()
{
  local db=$MSH_SHN_PRJTOP_DIR/duf_db/${1}.sqlite3
  shift
  ls $MSH_SHN_PRJTOP_DIR/duf_db >&2
  sqlite3 "$db" <<SQL
.header on
.mode column
.width 8 4 6 8 5 15 15 15 8 20 3 20
  SELECT * FROM duf_paths  $@;
SQL
}
function sqf_mdpath ()
{
  local db=$MSH_SHN_PRJTOP_DIR/duf_db/${1}.sqlite3
  shift
  ls $MSH_SHN_PRJTOP_DIR/duf_db >&2
  sqlite3 "$db" <<SQL
.header on
.mode column
.width 4 20 20 6 4 20
  SELECT * FROM duf_mdpath  $*;
SQL
}
function sqf_group ()
{
  local db="$MSH_SHN_PRJTOP_DIR/duf_db/${1}.sqlite3"
  shift
  ls $MSH_SHN_PRJTOP_DIR/duf_db >&2
  sqlite3 "$db" <<SQL
.header on
.mode column
  SELECT * FROM duf_group  $@;
SQL
}
function sqf_files ()
{
  local db=$MSH_SHN_PRJTOP_DIR/duf_db/${1}.sqlite3
  shift
  ls $MSH_SHN_PRJTOP_DIR/duf_db >&2
  sqlite3 "$db" <<SQL
.header on
.mode column
.width 4 4 60 4 4 20 7
  SELECT duf_filenames.*,duf_filedatas.id as fdid, filetype, filestatus
    FROM duf_filenames 
    LEFT JOIN duf_filedatas ON (duf_filedatas.id=duf_filenames.dataid) $@;
SQL
}
function sqf_exif ()
{
  local db=$MSH_SHN_PRJTOP_DIR/duf_db/${1}.sqlite3
  shift
  ls $MSH_SHN_PRJTOP_DIR/duf_db >&2
  sqlite3 "$db" <<SQL
.header on
.mode column
.width 3 6 20 20 12 14 3 20 20
  SELECT duf_exif.*, duf_filenames.name FROM duf_exif 
     LEFT JOIN duf_filedatas ON (duf_exif.dataid=duf_filedatas.id) 
     LEFT JOIN duf_filenames ON (duf_filedatas.id=duf_filenames.dataid) $@;
SQL
}
function sqf ()
{
  local db=$MSH_SHN_PRJTOP_DIR/duf_db/${1}.sqlite3
  shift
  ls $MSH_SHN_PRJTOP_DIR/duf_db >&2
  sqlite3 "$db" <<SQL
.header on
.separator "	"
  $@;
SQL
}
##function sqfiles ()
##{
##  local db=$MSH_SHN_PRJTOP_DIR/duf_db/${1}.sqlite3
##  shift
##  sqlite3 "$db" <<SQL
##.header on
##.separator "	"
##  SELECT duf_filedatas.id, duf_filedatas.dev, duf_filedatas.inode, duf_filedatas.md5id, duf_filedatas.size, duf_filenames.id, 
##         duf_filenames.pathid, strftime('%%s',  duf_md5.now) as seconds, duf_md5.now, duf_filenames.name
##  FROM duf_filedatas 
##  LEFT JOIN duf_filenames ON (duf_filedatas.id=duf_filenames.dataid)
##  LEFT JOIN duf_md5 ON (duf_filedatas.md5id=duf_md5.id) $@;
##SQL
##}
##function sqfiledatas ()
##{
##  local db=$MSH_SHN_PRJTOP_DIR/duf_db/${1}.sqlite3
##  shift
##  local sql='SELECT * FROM duf_filedatas'" $@"
##  sqlite3 -header -column "$db" "$sql"
##}
##function sqfilenames ()
##{
##  local db=$MSH_SHN_PRJTOP_DIR/duf_db/${1}.sqlite3
##  shift
##  sqlite3 "$db" << SQL
##.header on
##.separator "		"
##SELECT id,name FROM duf_filenames $@;
##SQL
##}
