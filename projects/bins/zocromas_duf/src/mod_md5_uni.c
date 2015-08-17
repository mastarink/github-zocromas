#define DUF_SQL_PDI_STMT

#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <openssl/md5.h>

#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"


#include "duf_config_ref.h"

#include "duf_pdi.h"
#include "duf_levinfo_ref.h"


#include "duf_sql_defs.h"
#include "duf_sql_field.h"
#include "duf_sql_field2.h"

#include "duf_sql.h"
#include "duf_sql1.h"
#include "duf_sql2.h"

/* #include "duf_dbg.h" */

/* #include "sql_beginning_selected.h" */
#include "sql_beginning_tables.h"


/* ########################################################################################## */
static int md5_dirent_content2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi );

/* ########################################################################################## */
static duf_sql_sequence_t final_sql = {.done = 0,
  .sql = {
          "UPDATE " DUF_DBPREF "md5 SET dup5cnt=(SELECT COUNT(*) " /* */
          " FROM " DUF_DBPREF "md5 AS md " /* */
          " JOIN " DUF_DBPREF "filedatas AS fd ON (fd.md5id=md." DUF_SQL_IDNAME ") " /* */
          " WHERE " DUF_DBPREF "md5." DUF_SQL_IDNAME "=md." DUF_SQL_IDNAME ")" /* */
          /* " WHERE " DUF_DBPREF "md5.md5sum1=md.md5sum1 AND " DUF_DBPREF "md5.md5sum2=md.md5sum2)" (* *) */
          ,
#if 0
          "INSERT OR IGNORE INTO " DUF_DBPREF "pathtot_dirs (Pathid, numdirs) " /* */
          "SELECT parents." DUF_SQL_IDNAME " AS Pathid, COUNT(*) AS numdirs " /* */
          " FROM " DUF_DBPREF "paths " /* */
          " JOIN " DUF_DBPREF "paths AS parents ON (parents." DUF_SQL_IDNAME "=paths.parentid) " /* */
          " GROUP BY parents." DUF_SQL_IDNAME "" /* */
          ,
#endif
          "DELETE FROM path_pairs" /* */
          ,
          "INSERT OR IGNORE INTO path_pairs (samefiles, Pathid1, Pathid2) SELECT count(*), fna.Pathid AS Pathid1, fnb.Pathid  AS Pathid2" /* */
          " FROM filenames AS fna" /* */
          "   JOIN filedatas AS fda ON (fna.dataid=fda.rowid)" /* */
          "   JOIN md5 AS mda ON (fda.md5id=mda.rowid)" /* */
          "   JOIN filedatas AS fdb ON (fdb.md5id=mda.rowid)" /* */
          "   JOIN filenames AS fnb ON (fdb.rowid=fnb.dataid)" /* */
          " WHERE Pathid1 < Pathid2 AND fna.name=fnb.name" /* */
          " GROUP BY Pathid1, Pathid2" /* */
          ,


          NULL,
          }
};

/* ########################################################################################## */

duf_scan_callbacks_t duf_collect_openat_md5_callbacks = {
  .title = "collect md5",
  .name = "md5",
  .init_scan = NULL,
  .def_opendir = 1,
  /* .dirent_dir_scan_before = NULL, */
  /* .dirent_file_scan_before = NULL, */
  /* .node_scan_before = collect_openat_md5_node_before, */
  /*  .leaf_scan =  collect_openat_md5_leaf, */
  /* .leaf_scan_fd = duf_dirent_md5_content, */
  .leaf_scan_fd2 = md5_dirent_content2,

  .use_std_leaf = 0,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node = 0,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .leaf = {.fieldset = "fn.Pathid AS dirid " /* */
           " , fd." DUF_SQL_IDNAME " AS filedataid, fd.inode AS inode " /* */
           " , fn.name AS filename, fd.size AS filesize " /* */
           " , fd.dev, fd.uid, fd.gid, fd.nlink, strftime('%s',fd.mtim) AS mtime, fd.rdev, fd.blksize, fd.blocks " /* */
           " , md.dup5cnt AS nsame " /* */
           " , fn." DUF_SQL_IDNAME " AS filenameid " /* */
           " , fd.mode AS filemode, md.md5sum1, md.md5sum2 " /* */
           " , fd.md5id AS md5id" /* */
           ,
           .selector2 =         /* */
           /* "SELECT %s " */
           " FROM " DUF_DBPREF "filenames AS fn " /* */
           " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "sizes as sz ON (sz.size=fd.size)" /* */
           " LEFT JOIN " DUF_DBPREF "md5 AS md ON (md." DUF_SQL_IDNAME "=fd.md5id)" /* */
           " LEFT JOIN " DUF_DBPREF "sd5 AS sd ON (sd." DUF_SQL_IDNAME "=fd.sd5id)" /* */
           "    WHERE "         /* */
           " ( fd.md5id   IS NULL OR md." DUF_SQL_IDNAME " IS NULL ) AND " /* */
           " sz.size > 0 AND "  /* */
           "(  :fFast IS NULL OR sz.size IS NULL OR sz.dupzcnt > 1 ) AND " /* */
           "(  :fFast IS NULL OR sd." DUF_SQL_IDNAME " IS NULL OR sd.dup2cnt > 1 ) AND" /* */
           " fn.Pathid=:parentdirID " /* */
           ,
           .selector_total2 =   /* */
           " FROM " DUF_DBPREF "filenames AS fn " /* */
           " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "sizes as sz ON (sz.size=fd.size)" /* */
           " LEFT JOIN " DUF_DBPREF "md5 AS md ON (md." DUF_SQL_IDNAME "=fd.md5id)" /* */
           " LEFT JOIN " DUF_DBPREF "sd5 AS sd ON (sd." DUF_SQL_IDNAME "=fd.sd5id)" /* */
           "    WHERE "         /* */
           " ( fd.md5id   IS NULL OR md." DUF_SQL_IDNAME " IS NULL ) AND " /* */
           " sz.size > 0 AND "  /* */
           "(  :fFast IS NULL OR sz.size IS NULL OR sz.dupzcnt > 1 ) AND " /* */
           "(  :fFast IS NULL OR sd." DUF_SQL_IDNAME " IS NULL OR sd.dup2cnt > 1 ) AND" /* */
           " 1 "                /* */
           }
  ,
  .node = {.fieldset = "pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname,  pt.ParentId " /* */
           ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize" /* */
           ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks, STRFTIME( '%s', pt.mtim ) AS mtime " /* */
           ,
           .selector2 =         /* */
           /* "SELECT     pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname,  pt.ParentId "                  */
           /* ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " */
           " FROM " DUF_DBPREF "paths AS pt " /* */
           " LEFT JOIN " DUF_SQL_TABLES_PATHTOT_DIRS_FULL "  AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_SQL_TABLES_PATHTOT_FILES_FULL " AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
#if 0
           " LEFT JOIN " DUF_DBPREF "pathtot_dirs AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "pathtot_files AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
#endif
           " WHERE pt.ParentId=:parentdirID AND ( :dirName IS NULL OR dirname=:dirName )" /* */
           ,
           .selector_total2 =   /* */
           " /* md5 */ FROM " DUF_SQL_TABLES_PATHS_FULL " AS p " /* */
           },
  .final_sql_seq = &final_sql,
};

/* ########################################################################################## */

static unsigned long long
duf_insert_md5_uni( duf_depthinfo_t * pdi, unsigned long long *md64, const char *msg, int need_id, int *pr )
{
  unsigned long long md5id = -1;
  int lr = 0;
  int changes = 0;

#ifdef MAS_TRACING
  const char *real_path = duf_levinfo_path( pdi );
#endif

  DEBUG_START(  );
  if ( md64 && md64[1] && md64[0] )
  {
    if ( !duf_config->cli.disable.flag.insert )
    {
#if 1
      static const char *sql = "INSERT OR IGNORE INTO " DUF_DBPREF "md5 ( md5sum1, md5sum2 ) VALUES ( :md5sum1, :md5sum2 )";

      DUF_TRACE( md5, 0, "%016llx%016llx %s%s", md64[1], md64[0], real_path, msg );
      DUF_SQL_START_STMT( pdi, insert_md5, sql, lr, pstmt );
      DUF_TRACE( insert, 0, "S:%s", sql );
      DUF_SQL_BIND_LL( md5sum1, md64[1], lr, pstmt );
      DUF_SQL_BIND_LL( md5sum2, md64[0], lr, pstmt );
      DUF_SQL_STEP( lr, pstmt );
      DUF_SQL_CHANGES( changes, lr, pstmt );
      DUF_SQL_END_STMT( insert_md5, lr, pstmt );
#else
      lr = duf_sql( "INSERT OR IGNORE INTO " DUF_DBPREF "md5 (md5sum1,md5sum2) VALUES ('%lld','%lld')", &changes, md64[1], md64[0] );
#endif
    }
    duf_pdi_reg_changes( pdi, changes );
    if ( ( lr == DUF_SQL_CONSTRAINT || !lr ) && !changes )
    {
      if ( need_id )
      {
        duf_scan_callbacks_t sccb = {.leaf.fieldset = "md5id" };
        duf_sccb_handle_t csccbh = {.sccb = &sccb };
        lr = duf_sql_select( duf_sel_cb_field_by_sccb, &md5id, STR_CB_DEF, STR_CB_UDATA_DEF, /* */
                             &csccbh, /* */
                             "SELECT " DUF_SQL_IDNAME " AS md5id FROM " DUF_DBPREF "md5 WHERE md5sum1='%lld' AND md5sum2='%lld'", md64[1], md64[0] );
      }
    }
    else if ( !lr /* assume SQLITE_OK */  )
    {
      if ( need_id && changes )
      {
        md5id = duf_sql_last_insert_rowid(  );
      }
    }
    else
    {
      DUF_SHOW_ERROR( "insert md5 %d", lr );
    }
  }
  else
  {
    DUF_SHOW_ERROR( "Wrong data" );
    DUF_MAKE_ERROR( lr, DUF_ERROR_DATA );
    DUF_TEST_R( lr );
  }

  if ( pr )
    *pr = lr;

  DEBUG_ENDULL( md5id );
  return md5id;
}

static int
duf_make_md5_uni( int fd, unsigned char *pmd )
{
  DEBUG_STARTR( r );
  size_t bufsz = 1024 * 1024 * 1;
  MD5_CTX ctx;

  memset( &ctx, 0, sizeof( ctx ) );
  DUF_TRACE( md5, 0, "make" );
  {
    char *buffer;

    buffer = mas_malloc( bufsz );
    if ( buffer )
    {
      if ( !duf_config->cli.disable.flag.calculate && ( MD5_Init( &ctx ) != 1 ) )
        DUF_MAKE_ERROR( r, DUF_ERROR_MD5 );
      DUF_TEST_R( r );
      {
        int maxcnt = 0;
        int cnt = 0;

        while ( r >= 0 && ( maxcnt == 0 || cnt++ < maxcnt ) )
        {
          int rr;

          DUF_TRACE( md5, 10, "read fd:%u", fd );
          rr = read( fd, buffer, bufsz );
          DUF_TRACE( md5, 10, "read rr:%u", rr );
          if ( rr < 0 )
          {
            DUF_ERRSYS( "read file" );

            DUF_MAKE_ERROR( r, DUF_ERROR_READ );
            DUF_TEST_R( r );
            break;
          }
          if ( rr > 0 && !duf_config->cli.disable.flag.calculate )
          {
            if ( MD5_Update( &ctx, buffer, rr ) != 1 )
              DUF_MAKE_ERROR( r, DUF_ERROR_MD5 );
          }
          if ( rr <= 0 )
            break;
          DUF_TEST_R( r );
        }
      }
      mas_free( buffer );
    }
    else
    {
      DUF_MAKE_ERROR( r, DUF_ERROR_MEMORY );
    }
  }
  if ( !duf_config->cli.disable.flag.calculate && MD5_Final( pmd, &ctx ) != 1 )
    DUF_MAKE_ERROR( r, DUF_ERROR_MD5 );
  DEBUG_ENDR( r );
}

static int
duf_make_md5r_uni( duf_depthinfo_t * pdi, unsigned char *pmdr )
{
  DEBUG_STARTR( r );
  unsigned char amd5[MD5_DIGEST_LENGTH];
  int fd;

  memset( amd5, 0, sizeof( amd5 ) );
  fd = duf_levinfo_dfd( pdi );
  r = duf_make_md5_uni( fd, amd5 );
  /* reverse */
  for ( int i = 0; i < sizeof( amd5 ) / sizeof( amd5[0] ); i++ )
    pmdr[i] = amd5[sizeof( amd5 ) / sizeof( amd5[0] ) - i - 1];


  DEBUG_ENDR( r );
}

static int
md5_dirent_content2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
#if 0
  unsigned long long data[MD5_DIGEST_LENGTH * sizeof( unsigned char ) / sizeof( unsigned long long )];

  DUF_UFIELD2( filedataid );
  DUF_SFIELD2( filename );
  DUF_TRACE( md5, 0, "+ %s", filename );

  if ( duf_config->cli.disable.flag.calculate )
    r = 0;
  else
    DOR( r, duf_make_md5r_uni( pdi, ( unsigned char * ) data ) );

  if ( r >= 0 )
  {
    unsigned long long md5id = 0;

    DUF_TRACE( md5, 0, "insert %s", filename );


    md5id = duf_insert_md5_uni( pdi, data, filename /* for dbg message only */ , 1 /*need_id */ , &r );
    if ( r >= 0 && md5id )
    {
      int changes = 0;

      pdi->cnts.dirent_content2++;
      if ( r >= 0 && !duf_config->cli.disable.flag.update )
        DOR( r, duf_sql( "UPDATE " DUF_DBPREF "filedatas SET md5id='%llu' WHERE " DUF_SQL_IDNAME "='%lld'", &changes, md5id, filedataid ) );
      duf_pdi_reg_changes( pdi, changes );
      DUF_TEST_R( r );
    }
    DUF_TRACE( md5, 0, "%016llx%016llx : md5id: %llu", data[1], data[0], md5id );
    /* DUF_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan 5    * %016llx%016llx : %llu", duf_pdi_depth( pdi ), pmd[1], pmd[0], md5id ); */
  }
#else
  unsigned char amd5r[MD5_DIGEST_LENGTH];


  DUF_UFIELD2( filedataid );
  DUF_SFIELD2( filename );
  DUF_TRACE( md5, 0, "+ %s", filename );

  if ( duf_config->cli.disable.flag.calculate )
    r = 0;
  else
    DOR( r, duf_make_md5r_uni( pdi, amd5r ) );

  if ( r >= 0 )
  {
    unsigned long long md5id = 0;
    unsigned long long *pmd = ( unsigned long long * ) &amd5r;

    DUF_TRACE( md5, 0, "insert %s", filename );


    md5id = duf_insert_md5_uni( pdi, pmd, filename /* for dbg message only */ , 1 /*need_id */ , &r );
    if ( r >= 0 && md5id )
    {
      int changes = 0;

      pdi->cnts.dirent_content2++;
      if ( r >= 0 && !duf_config->cli.disable.flag.update )
        DOR( r, duf_sql( "UPDATE " DUF_DBPREF "filedatas SET md5id='%llu' WHERE " DUF_SQL_IDNAME "='%lld'", &changes, md5id, filedataid ) );
      duf_pdi_reg_changes( pdi, changes );
      DUF_TEST_R( r );
    }
    DUF_TRACE( md5, 0, "%016llx%016llx : md5id: %llu", pmd[1], pmd[0], md5id );
    /* DUF_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan 5    * %016llx%016llx : %llu", duf_pdi_depth( pdi ), pmd[1], pmd[0], md5id ); */
  }
#endif
  DEBUG_ENDR( r );
}

/* static int                                                                                                                          */
/* duf_dirent_content_by_precord( duf_depthinfo_t * pdi, duf_record_t * precord, const char *fname, unsigned long long filesize ) */
/* {                                                                                                                                   */
/*   DEBUG_STARTR( r );                                                                                                                */
/*   int ffd = duf_levinfo_dfd( pdi );                                                                                                 */
/*                                                                                                                                     */
/*   DUF_SHOW_ERROR( "ffd:%d for " DUF_DEPTH_PFMT "", ffd, duf_pdi_depth( pdi ) );                                                     */
/*   if ( ffd )                                                                                                                        */
/*   {                                                                                                                                 */
/*     DUF_TRACE( md5, 2, "openat ffd:%d", ffd );                                                                                      */
/*     if ( ffd > 0 )                                                                                                                  */
/*     {                                                                                                                               */
/*       r = duf_dirent_md5_content( ffd, duf_levinfo_stat( pdi ), pdi, precord );                                                */
/*     }                                                                                                                               */
/*     else                                                                                                                            */
/*     {                                                                                                                               */
/*       DUF_ERRSYS( "open to read file : %s", fname );                                                                                */
/*       r = ffd;                                                                                                                      */
/*       DUF_MAKE_ERROR(r, DUF_ERROR_OPEN);                                                                                            */
/*     }                                                                                                                               */
/*     DUF_TEST_R( r );                                                                                                                */
/*     DUF_TEST_R( r );                                                                                                                */
/*   }                                                                                                                                 */
/*   else                                                                                                                              */
/*     DUF_MAKE_ERROR(r, DUF_ERROR_DATA);                                                                                              */
/*   DUF_TEST_R( r );                                                                                                                  */
/*   DEBUG_ENDR( r );                                                                                                                  */
/* }                                                                                                                                   */

/* callback of type duf_scan_hook_file_t */
/* static int                                                                    */
/* collect_openat_md5_leaf( duf_depthinfo_t * pdi, duf_record_t * precord ) */
/* {                                                                             */
/*   DEBUG_STARTR( r );                                                                                                                */
/*                                                                               */
/*   DUF_SFIELD( filename );                                                     */
/*   DUF_UFIELD( filesize );                                                     */
/*   DEBUG_START(  );                                                            */
/*   r = duf_dirent_content_by_precord( pdi, precord, filename, filesize ); */
/*   DEBUG_ENDR( r );                                                            */
/* }                                                                             */

/* 
 * this is callback of type: duf_scan_hook_dir_t
 * */
/* static int                                                                                                             */
/* collect_openat_md5_node_before( unsigned long long pathid_unused, duf_depthinfo_t * pdi, duf_record_t * precord ) */
/* {                                                                                                                      */
/*   DEBUG_STARTR( r );                                                                                                   */
/*   const char *real_path = NULL;                                                                                        */
/*                                                                                                                        */
/*   DEBUG_START(  );                                                                                                     */
/*                                                                                                                        */
/*   real_path = duf_levinfo_path( pdi );                                                                                 */
/*   DUF_TRACE( md5, 0, "L%d; " DUF_SQL_IDNAME "%-7llu  real_path=%s;", duf_pdi_depth( pdi ), pathid_unused, real_path );                 */
/*                                                                                                                        */
/*   DEBUG_ENDR( r );                                                                                                     */
/* }                                                                                                                      */

/* currently used for --same-md5  ??? */
