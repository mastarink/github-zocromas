#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */
#include <magic.h>


#include <errno.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_utils.h"
#include "duf_config.h"

#include "duf_pdi.h"
#include "duf_levinfo.h"

/* #include "duf_sql_def.h" */
#include "duf_sql_field.h"

#include "duf_sql.h"
#include "duf_sql1.h"

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
    if ( !duf_config->cli.disable.insert )
      r = duf_sql( "INSERT OR IGNORE INTO duf_mime ( mime, charset, tail) VALUES ('%s', '%s', '%s')", &changes, mime, chs, tail );
    duf_pdi_reg_changes( pdi, changes );
    if ( ( r == DUF_SQL_CONSTRAINT || !r ) && !changes )
    {
      if ( need_id )
      {
        duf_scan_callbacks_t sccb = {.fieldset = "mimeid" };
        r = duf_sql_select( duf_sel_cb_field_by_sccb, &mimeid, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_depthinfo_t * ) NULL, &sccb,
                            "SELECT id AS mimeid FROM duf_mime WHERE mime='%s' AND charset='%s'", mime, chs );
      }
      DUF_TRACE( mime, 0, "inserted before (SQLITE_OK) mimeid=%llu / %llu", mimeid, duf_sql_last_insert_rowid(  ) );
    }
    else if ( !r )
    {
      if ( need_id )
      {
        mimeid = duf_sql_last_insert_rowid(  );
        DUF_TRACE( mime, 0, "inserted now (SQLITE_OK) mimeid=%llu", mimeid );
      }
    }
    else
    {
      DUF_ERROR( "insert mime %d [%s]", r, duf_error_name( r ) );
      DUF_TEST_R( r );
    }
    DUF_TEST_R( r );
  }
  else
  {
    DUF_ERROR( "Wrong data" );
    r = DUF_ERROR_DATA;
  }
  DUF_TEST_R( r );
  if ( pr )
    *pr = r;
  DEBUG_ENDULL( mimeid );
  DUF_TEST_R( r );
  return mimeid;
}

unsigned long long
duf_make_mime_uni( int fd, int *pr )
{
  int r = 0;
  unsigned long long mimeid = 0;

  if ( pr )
    *pr = r;
  return mimeid;
}

static void
duf_mime_destructor( void *ctx )
{
  magic_t m = ( magic_t ) ctx;

  magic_close( m );
  DUF_TRACE( mime, 0, "closed mime" );
}

static int
duf_scan_dirent_content2( duf_sqlite_stmt_t * pstmt, int fd, const struct stat *pst_file, duf_depthinfo_t * pdi )
{
  int r = 0;
  unsigned long long mimeid = 0;

  DUF_UFIELD2( dataid );
  mimeid = duf_make_mime_uni( fd, &r );
  DUF_TEST_R( r );

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
      DUF_TRACE( mime, 0, "opened mime %s", m ? "OK" : "FAIL" );
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
        if ( 0 == strncmp( p, "charset=", 8 ) )
          charset = mas_strdup( p + 8 );
        else
          tail = mas_strdup( p );
        mimeid = duf_insert_mime_uni( pdi, mimet, charset, tail, 1 /*need_id */ , &r );
        DUF_TEST_R( r );
        if ( r >= 0 && mimeid && !duf_config->cli.disable.update )
        {
          int changes = 0;

          r = duf_sql( "UPDATE duf_filedatas SET mimeid='%llu' WHERE id='%lld'", &changes, mimeid, dataid );
          duf_pdi_reg_changes( pdi, changes );
          DUF_TEST_R( r );
        }
        mas_free( mimet );
        mas_free( charset );
        mas_free( tail );
      }
      DUF_TRACE( scan, 2, "  " DUF_DEPTH_PFMT ": scan 5    : %llu", duf_pdi_depth( pdi ), mimeid );
    }
  }
  DUF_TEST_R( r );
  return r;
}

duf_scan_callbacks_t duf_collect_mime_callbacks = {
  .title = "collect mime",
  .opendir = 1,
  .scan_mode_2 = 1,

  .leaf_scan_fd2 = duf_scan_dirent_content2,

  /* filename for debug only */
  .fieldset = "fn.Pathid AS dirid, fn.name AS filename, fd.size AS filesize, fd.id as dataid " /* */
        ", uid, gid, nlink, inode, mtim AS mtime " /* */
        ", fd.mode AS filemode " /* */
        ", fn.id AS filenameid " /* */
        ,
  .leaf_selector2 = "SELECT %s FROM duf_filenames AS fn " /* */
        " LEFT JOIN duf_filedatas AS fd ON ( fn.dataid = fd.id ) " /* */
        " WHERE "               /* */
        " fn.Pathid = :dirid ORDER BY fd.mimeid ",
  .node_selector = "SELECT pt.id AS dirid, pt.dirname, pt.dirname AS dfname,  pt.parentid " /* */
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
        " FROM duf_paths AS pt " /* */
        " LEFT JOIN duf_pathtot_dirs AS td ON (td.Pathid=pt.id) " /* */
        " LEFT JOIN duf_pathtot_files AS tf ON (tf.Pathid=pt.id) " /* */
        " WHERE pt.parentid = '%llu' ",
  .node_selector2 = "SELECT     pt.id AS dirid, pt.dirname, pt.dirname AS dfname,  pt.parentid " /* */
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
        " FROM duf_paths AS pt " /* */
        " LEFT JOIN duf_pathtot_dirs AS td ON (td.Pathid=pt.id) " /* */
        " LEFT JOIN duf_pathtot_files AS tf ON (tf.Pathid=pt.id) " /* */
        " WHERE pt.parentid = :dirid ",
  .final_sql_argv = NULL,
};
