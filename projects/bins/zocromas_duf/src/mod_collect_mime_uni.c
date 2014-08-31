#define DUF_SQL_PDI_STMT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */
#include <magic.h>


#include <errno.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"


#include "duf_utils.h"
#include "duf_config_ref.h"

#include "duf_pdi.h"
#include "duf_levinfo.h"

/* #include "duf_sql_const.h" */
#include "duf_sql_defs.h"
#include "duf_sql_field.h"

#include "duf_sql.h"
#include "duf_sql1.h"
#include "duf_sql2.h"

#include "duf_dbg.h"

static unsigned long long
duf_insert_mime_uni( duf_depthinfo_t * pdi, const char *mime, const char *chs, const char *tail, int need_id, int *pr )
{
  int r = 0;
  unsigned long long mimeid = 0;

  if ( mime )
  {
    int changes = 0;

    DEBUG_START(  );
    if ( need_id )
    {
      const char *sql = "SELECT " DUF_SQL_IDNAME " AS mimeid FROM " DUF_DBPREF "mime WHERE mime = :Mime";

      /* const char *sql = "SELECT " DUF_SQL_IDNAME " AS mimeid FROM " DUF_DBPREF "mime WHERE mime=:Mime AND charset=:charSet" ; */

      DUF_SQL_START_STMT( pdi, select_mime, sql, r, pstmt_select );
      DUF_SQL_BIND_S( Mime, mime, r, pstmt_select );
      /* DUF_SQL_BIND_S( charSet, chs, r, pstmt_select ); */
      /* DUF_SQL_BIND_S( Tail, tail, r, pstmt_select ); */
      DUF_SQL_STEP( r, pstmt_select );
      if ( r == DUF_SQL_ROW )
      {
        DUF_TRACE( current, 0, "<selected>" );
        mimeid = duf_sql_column_long_long( pstmt_select, 0 );
        r = 0;
      }
      if ( r == DUF_SQL_DONE )
        r = 0;
      DUF_TEST_R( r );
      DUF_SQL_END_STMT( r, pstmt_select );
    }

    if ( !mimeid && !duf_config->cli.disable.flag.insert )
    {
      const char *sql = "INSERT OR IGNORE INTO " DUF_DBPREF "mime ( mime ) VALUES ( :Mime )";

      /* "INSERT OR IGNORE INTO " DUF_DBPREF "mime ( mime, charset, tail ) VALUES (:Mime, :charSet, :Tail )"; */

      DUF_SQL_START_STMT( pdi, insert_mime, sql, r, pstmt_insert );
      DUF_TRACE( insert, 0, " S: %s ", sql );
      DUF_SQL_BIND_S( Mime, mime, r, pstmt_insert );
      /* DUF_SQL_BIND_S( charSet, chs, r, pstmt_insert ); */
      /* DUF_SQL_BIND_S( Tail, tail, r, pstmt_insert ); */
      DUF_SQL_STEP( r, pstmt_insert );
      /* DUF_TEST_R(r); */
      DUF_SQL_CHANGES( changes, r, pstmt_insert );
      /* DUF_ERROR( "changes:%d", changes ); */
      if ( need_id && changes )
      {
        mimeid = duf_sql_last_insert_rowid(  );
        DUF_TRACE( mime, 0, " inserted now( SQLITE_OK ) mimeid = %llu ", mimeid );
        assert( mimeid );
      }
      DUF_SQL_END_STMT( r, pstmt_insert );
    }
    DUF_TEST_R( r );
  }
  else
  {
    DUF_ERROR( " Wrong data " );
    r = DUF_ERROR_DATA;
  }
  DUF_TEST_R( r );
  if ( pr )
    *pr = r;
  DEBUG_ENDULL( mimeid );
  DUF_TEST_R( r );
  return mimeid;
}

static void
duf_mime_destructor( void *ctx )
{
  magic_t m = ( magic_t ) ctx;

  magic_close( m );
  DUF_TRACE( mime, 0, " closed mime " );
}

static int
duf_scan_dirent_mime_content2( duf_sqlite_stmt_t * pstmt, int fd, const struct stat *pst_file, duf_depthinfo_t * pdi )
{
  int r = 0;
  unsigned long long mimeid = 0;

  DUF_TEST_R( r );

  /* {                                    */
  /*   DUF_SFIELD2( filename );           */
  /*   DUF_ERROR( "@@@@@ %s", filename ); */
  /* }                                    */
  if ( r >= 0 )
  {
    const char *mime = NULL;

    magic_t m = NULL;

    if ( 1 )
      m = ( magic_t ) duf_pdi_context( pdi );
    else
      m = ( magic_t ) duf_levinfo_context_up( pdi );

    if ( !m )
    {
      m = magic_open( MAGIC_MIME | MAGIC_PRESERVE_ATIME );
      DUF_TRACE( mime, 0, " opened mime %s ", m ? " OK " : " FAIL " );
      if ( 1 )
      {
        duf_pdi_set_context( pdi, m );
        duf_pdi_set_context_destructor( pdi, duf_mime_destructor );
      }
      else
      {
        duf_levinfo_set_context_up( pdi, m );
        duf_levinfo_set_context_up_destructor( pdi, duf_mime_destructor );
      }
    }
    r = magic_load( m, NULL );
    DUF_TEST_R( r );

    mime = magic_descriptor( m, fd );

    if ( mime )
    {
      const char *p = strchr( mime, ';' );

      if ( p )
      {
        char *mimet = NULL;
        char *charset = NULL;
        char *tail = NULL;

        mimet = mas_strndup( mime, p - mime );
        p++;
        while ( p && *p && *p <= ' ' )
          p++;
        if ( 0 == strncmp( p, " charset = ", 8 ) )
          charset = mas_strdup( p + 8 );
        else
          tail = mas_strdup( p );
        mimeid = duf_insert_mime_uni( pdi, mimet, charset, tail, 1 /*need_id */ , &r );
        DUF_TEST_R( r );
        if ( r >= 0 && mimeid && !duf_config->cli.disable.flag.update )
        {
          int changes = 0;

          DUF_UFIELD2( dataid );

          if ( 1 )
          {
            const char *sql = " UPDATE " DUF_DBPREF " filedatas SET mimeid = :mimeID WHERE " DUF_SQL_IDNAME " = :dataID ";

            DUF_SQL_START_STMT( pdi, update_mime, sql, r, pstmt_update );
            DUF_TRACE( update, 0, " S: %s ", sql );
            DUF_SQL_BIND_LL( mimeID, mimeid, r, pstmt_update );
            DUF_SQL_BIND_LL( dataID, dataid, r, pstmt_update );
            DUF_SQL_STEP( r, pstmt_update );
            /* DUF_TEST_R(r); */
            DUF_SQL_CHANGES( changes, r, pstmt_update );
            DUF_SQL_END_STMT( r, pstmt_update );
          }
          else
          {
            r = duf_sql( " UPDATE " DUF_DBPREF " filedatas SET mimeid = %llu WHERE " DUF_SQL_IDNAME " = %lld", &changes, mimeid, dataid );
            duf_pdi_reg_changes( pdi, changes );
          }


          DUF_TEST_R( r );
        }
        mas_free( mimet );
        mas_free( charset );
        mas_free( tail );
      }
      /* DUF_TRACE( scan, 12, " " DUF_DEPTH_PFMT ": scan 5: %llu ", duf_pdi_depth( pdi ), mimeid ); */
    }
  }
  DUF_TEST_R( r );
  return r;
}






static const char *final_sql[] = {
  "UPDATE " DUF_DBPREF "mime SET dupmimecnt=(SELECT COUNT(*) " /* */
        " FROM " DUF_DBPREF "mime AS mi " /* */
        " JOIN " DUF_DBPREF "filedatas AS fd ON (fd.mimeid=mi." DUF_SQL_IDNAME ") " /* */
        " WHERE " DUF_DBPREF "mime.mime=mi.mime)" /* */
        ,


  NULL,
};



duf_scan_callbacks_t duf_collect_mime_callbacks = {
  .title = "collect mime",
  .def_opendir = 1,
  .scan_mode_2 = 1,

  .leaf_scan_fd2 = duf_scan_dirent_mime_content2,

  /* filename for debug only */
  .leaf = {.fieldset = " fn.Pathid AS dirid, fn.name AS filename, fd.size AS filesize, fd." DUF_SQL_IDNAME " as dataid " /* */
           ", uid, gid, nlink, inode, mtim AS mtime " /* */
           ", fd.mode AS filemode " /* */
           ", fn." DUF_SQL_IDNAME " AS filenameid " /* */
           ", fd.md5id AS md5id" /* */
           ,
           .selector2 =         /* */
           " FROM " DUF_DBPREF " filenames AS fn " /* */
           " LEFT JOIN " DUF_DBPREF " filedatas AS fd ON( fn.dataid = fd." DUF_SQL_IDNAME " ) " /* */
           " LEFT JOIN " DUF_DBPREF " mime AS mi ON( fd.mimeid = mi." DUF_SQL_IDNAME " ) " /* */
           " LEFT JOIN " DUF_DBPREF " sizes as sz ON (sz.size=fd.size)" /* */
           " WHERE "            /* */
           " ( fd.mimeid IS NULL OR mi.mime IS NULL ) AND " " sz.size > 0 AND" " fn.Pathid = :dirID " /* */
           ,
           .selector_total2 =   /* */
           " FROM " DUF_DBPREF " filenames AS fn " /* */
           " LEFT JOIN " DUF_DBPREF " filedatas AS fd ON( fn.dataid = fd." DUF_SQL_IDNAME " ) " /* */
           " LEFT JOIN " DUF_DBPREF " mime AS mi ON( fd.mimeid = mi." DUF_SQL_IDNAME " ) " /* */
           " LEFT JOIN " DUF_DBPREF " sizes as sz ON (sz.size=fd.size)" /* */
           " WHERE "            /* */
           " ( fd.mimeid IS NULL OR mi.mime IS NULL ) AND " /* */
           " sz.size > 0 " " ORDER BY fd.mimeid " /* */
           },
  .node = {.fieldset = " pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname, pt.parentid " /* */
           ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
           ,
           .selector2 =         /* */
           " FROM " DUF_DBPREF " paths AS pt " /* */
           " LEFT JOIN " DUF_DBPREF " pathtot_dirs AS td ON( td.Pathid = pt." DUF_SQL_IDNAME " ) " /* */
           " LEFT JOIN " DUF_DBPREF " pathtot_files AS tf ON( tf.Pathid = pt." DUF_SQL_IDNAME " ) " /* */
           " WHERE pt.parentid = :dirID " /* */
           },
  .final_sql_argv = final_sql,
};
