#define DUF_SQL_PDI_STMT

#include <string.h>
#include <errno.h>

/* man libmagic */
#include <magic.h>

#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"


#include "duf_config_ref.h"

#include "duf_pdi.h"
#include "duf_pdi_context.h"
#include "duf_pdi_stmt.h"
#include "duf_pdi_ref.h"

#include "duf_levinfo_ref.h"
#include "duf_levinfo_context.h"

#include "duf_sql_defs.h"
#include "duf_sql_field.h"


#include "duf_sql.h"
#include "duf_sql1.h"
#include "duf_sql2.h"

/* #include "duf_dbg.h" */

/* #include "sql_beginning_selected.h" */
#include "sql_beginning_tables.h"



/* ########################################################################################## */
static int dirent_content2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi );

/* ########################################################################################## */
static duf_sql_sequence_t final_sql = { /* */
  .done = 0,
  .sql = {
          "UPDATE " DUF_DBPREF "mime SET dupmimecnt=(SELECT COUNT(*) " /* */
          " FROM  " DUF_DBPREF "mime      AS mi " /* */
          " JOIN  " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fd.mimeid=mi." DUF_SQL_IDNAME ") " /* */
          " WHERE " DUF_DBPREF "mime." DUF_SQL_IDNAME "=mi." DUF_SQL_IDNAME ")" /* */
          /* " WHERE " DUF_DBPREF "mime.mime=mi.mime)" (* *) */
          ,


          NULL,
          }
};

/* ########################################################################################## */
#define Q_J " LEFT JOIN "
#define Q_FROM( _t, _a ) " FROM " DUF_DBPREF # _t " AS " # _a
#define Q_JOIN_ID( _up, _t, _as, _o )  " LEFT JOIN " DUF_DBPREF # _t " AS " # _as " ON ( " # _up "." # _o " = " # _as "." DUF_SQL_IDNAME " ) "
#define Q_JOIN_SYN( _up, _t, _as, _o ) " LEFT JOIN " DUF_DBPREF # _t " AS " # _as " ON ( " # _up "." # _o " = " # _as "." # _o ")"

duf_scan_callbacks_t duf_collect_mime_callbacks = {
  .title = "collect mime",
  .name = "mime",
  .def_opendir = 1,

  .leaf_scan_fd2 = dirent_content2,

/* TODO : exp;ain values of use_std_leaf and use_std_node TODO */
  .use_std_leaf = 0,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node = 0,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  /* filename for debug only */
  .leaf = {.fieldset =          /* */
           "'mime-leaf' AS fieldset_id, " /* */
           " fn.Pathid AS dirid, fn.name AS filename " /* */
           ", fn.name AS dfname, fd.size AS filesize, fd." DUF_SQL_IDNAME " as dataid " /* */
           ", 0 as ndirs, 0 as nfiles" /* */
           ", fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, fd.rdev, fd.blksize, fd.blocks " /* */
           "  "                 /* */
           ", strftime('%s',fd.mtim)   AS mtime " /* */
           ", fd.mode               AS filemode " /* */
           ", fn." DUF_SQL_IDNAME " AS filenameid " /* */
           " , fn." DUF_SQL_IDNAME " AS nameid " /* */
           ", fd.md5id              AS md5id " /* */
           ,
           .selector2 =         /* */
           " FROM      " DUF_SQL_TABLES_FILENAMES_FULL " AS fn " /* */
           /* Q_FROM( filenames, fn ) (* *) */
           " LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON ( fn.dataid = fd." DUF_SQL_IDNAME " ) " /* */
           " LEFT JOIN " DUF_DBPREF " mime      AS mi ON ( fd.mimeid = mi." DUF_SQL_IDNAME " ) " /* */
           /* Q_JOIN_ID( fd, mime, mi, mimeid) */
           " LEFT JOIN " DUF_DBPREF " sizes     AS sz ON ( sz.size   = fd.size               ) " /* */
           /* Q_JOIN_SYN( fd, sizes, sz, size ) (* *) */
           ,
           .matcher = " fn.Pathid = :parentdirID " /* */
           ,                    /* */
           .filter =            /* */
           " ( fd.mimeid IS NULL OR mi.mime IS NULL )  AND " /* */
           " sz.size > 0                               AND " /* */
           " 1 "                /* */
           ,
           .count_aggregate = "distinct fd." DUF_SQL_IDNAME
           },
  .node = {.fieldset =          /* */
           "'mime-node' AS fieldset_id, " /* */
           "pt." DUF_SQL_IDNAME " AS dirid" /* */
           ", pt." DUF_SQL_IDNAME " AS nameid " /* */
           ", pt.dirname, pt.dirname AS dfname, pt.parentid " /* */
           ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
           ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks, STRFTIME( '%s', pt.mtim ) AS mtime " /* */
           ,
           .selector2 =         /* */
           " FROM      " DUF_SQL_TABLES_PATHS_FULL "             AS pt " /* */
           " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL "  AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL " AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
#if 0
           " LEFT JOIN " DUF_DBPREF " pathtot_dirs AS td ON( td.Pathid = pt." DUF_SQL_IDNAME " ) " /* */
           " LEFT JOIN " DUF_DBPREF " pathtot_files AS tf ON( tf.Pathid = pt." DUF_SQL_IDNAME " ) " /* */
#endif
           ,
           .matcher = "pt.ParentId = :parentdirID  AND ( :dirName IS NULL OR dirname=:dirName )" /* */
           ,                    /* */
           .filter = NULL       /* */
#if 0
           ,
           .selector_total2 =   /* */
           " /* mime */ FROM " DUF_SQL_TABLES_PATHS_FULL " AS p " /* */
#endif
           },
  .final_sql_seq = &final_sql,
};

/* ########################################################################################## */

static unsigned long long
duf_insert_mime_uni( duf_depthinfo_t * pdi, const char *mime, const char *chs, const char *tail, int need_id, int *pr )
{
  int lr = 0;

  DEBUG_STARTULL( mimeid );

  if ( mime )
  {
    int changes = 0;

    if ( need_id )
    {
      const char *sql = "SELECT " DUF_SQL_IDNAME " AS mimeid FROM " DUF_DBPREF "mime WHERE mime = :Mime";

      /* const char *sql = "SELECT " DUF_SQL_IDNAME " AS mimeid FROM " DUF_DBPREF "mime WHERE mime=:Mime AND charset=:charSet" ; */

      DUF_SQL_START_STMT( pdi, select_mime, sql, lr, pstmt_select );
      DUF_SQL_BIND_S( Mime, mime, lr, pstmt_select );
      /* DUF_SQL_BIND_S( charSet, chs, lr, pstmt_select ); */
      /* DUF_SQL_BIND_S( Tail, tail, lr, pstmt_select ); */
      DUF_SQL_STEP( lr, pstmt_select );
      if ( lr == DUF_SQL_ROW )
      {
        DUF_TRACE( mod, 0, "<selected>" );
        mimeid = duf_sql_column_long_long( pstmt_select, 0 );
        lr = 0;
      }
      if ( lr == DUF_SQL_DONE )
        lr = 0;
      DUF_TEST_R( lr );
      DUF_SQL_END_STMT( select_mime, lr, pstmt_select );
    }

    if ( !mimeid && !duf_config->cli.disable.flag.insert )
    {
      const char *sql = "INSERT OR IGNORE INTO " DUF_DBPREF "mime ( mime ) VALUES ( :Mime )";

      /* "INSERT OR IGNORE INTO " DUF_DBPREF "mime ( mime, charset, tail ) VALUES (:Mime, :charSet, :Tail )"; */

      DUF_SQL_START_STMT( pdi, insert_mime, sql, lr, pstmt_insert );
      DUF_TRACE( mod, 3, " S: %s ", sql );
      DUF_SQL_BIND_S( Mime, mime, lr, pstmt_insert );
      /* DUF_SQL_BIND_S( charSet, chs, lr, pstmt_insert ); */
      /* DUF_SQL_BIND_S( Tail, tail, lr, pstmt_insert ); */
      DUF_SQL_STEP( lr, pstmt_insert );
      /* DUF_TEST_R(lr); */
      DUF_SQL_CHANGES( changes, lr, pstmt_insert );
      /* DUF_SHOW_ERROR( "changes:%d", changes ); */
      if ( need_id && changes )
      {
        mimeid = duf_sql_last_insert_rowid(  );
        DUF_TRACE( mime, 0, " inserted now( SQLITE_OK ) mimeid = %llu ", mimeid );
        assert( mimeid );
      }
      DUF_SQL_END_STMT( insert_mime, lr, pstmt_insert );
    }
    DUF_TEST_R( lr );
  }
  else
  {
    DUF_SHOW_ERROR( " Wrong data " );
    DUF_MAKE_ERROR( lr, DUF_ERROR_DATA );
  }
  DUF_TEST_R( lr );
  if ( pr )
    *pr = lr;
  DEBUG_ENDULL( mimeid );
}

static void
mime_destructor( void *ctx )
{
  magic_t magic = ( magic_t ) ctx;

  magic_close( magic );
  DUF_TRACE( mime, 0, " closed mime " );
}

/*
 * pstmt is needed for dataid
 * */
static int
dirent_content2( duf_sqlite_stmt_t * pstmt, /* const struct stat *pst_file_needless, */ duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  unsigned long long mimeid = 0;



  DUF_TRACE( mod, 0, " mime" );

  if ( r >= 0 )
  {
    const char *mime = NULL;

    magic_t magic = NULL;

    if ( 1 )
      magic = ( magic_t ) duf_pdi_context( pdi );
    else
      magic = ( magic_t ) duf_levinfo_context_up( pdi );

    if ( !magic )
    {
      magic = magic_open( MAGIC_MIME | MAGIC_PRESERVE_ATIME );
      DUF_TRACE( mime, 0, " opened mime %s ", magic ? " OK " : " FAIL " );
      if ( 1 )
      {
        duf_pdi_set_context( pdi, magic );
        duf_pdi_set_context_destructor( pdi, mime_destructor );
      }
      else
      {
        duf_levinfo_set_context_up( pdi, magic );
        duf_levinfo_set_context_up_destructor( pdi, mime_destructor );
      }
    }
    DOR( r, magic_load( magic, NULL ) );

    mime = magic_descriptor( magic, duf_levinfo_dfd( pdi ) );
    DUF_TRACE( mime, 0, " opened mime %s : %s", magic ? " OK " : " FAIL ", mime );

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
            const char *sql = " UPDATE " DUF_SQL_TABLES_FILEDATAS_FULL " SET mimeid = :mimeID WHERE " DUF_SQL_IDNAME " = :dataID ";

            DUF_SQL_START_STMT( pdi, update_mime, sql, r, pstmt_update );
            DUF_TRACE( mod, 3, " S: %s ", sql );
            DUF_SQL_BIND_LL( mimeID, mimeid, r, pstmt_update );
            DUF_SQL_BIND_LL( dataID, dataid, r, pstmt_update );
            DUF_SQL_STEP( r, pstmt_update );
            /* DUF_TEST_R(r); */
            DUF_SQL_CHANGES( changes, r, pstmt_update );
            DUF_SQL_END_STMT( update_mime, r, pstmt_update );
          }
          else
          {
            DOR( r, duf_sql( " UPDATE " DUF_SQL_TABLES_FILEDATAS_FULL " SET mimeid = %llu WHERE " DUF_SQL_IDNAME " = %lld", &changes, mimeid, dataid ) );
            duf_pdi_reg_changes( pdi, changes );
          }


          DUF_TEST_R( r );
        }
        mas_free( mimet );
        mas_free( charset );
        mas_free( tail );
      }
      /* DUF_TRACE( mod, 12, " " DUF_DEPTH_PFMT ": scan 5: %llu ", duf_pdi_depth( pdi ), mimeid ); */
    }
  }
  DEBUG_ENDR( r );
}
