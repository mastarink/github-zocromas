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

static unsigned long long
duf_insert_sd5_uni( duf_depthinfo_t * pdi, unsigned long long *md64, const char *filename, size_t fsize, int need_id, int *pr )
{
  unsigned long long sd5id = -1;
  int lr = 0;
  int changes = 0;
  const char *real_path = duf_levinfo_path( pdi );

  DEBUG_START(  );
  if ( md64 && md64[1] && md64[0] )
  {
    if ( !duf_config->cli.disable.flag.insert )
    {
      if ( 1 )
      {
        static const char *sql = "INSERT OR IGNORE INTO " DUF_DBPREF "sd5 (sd5sum1,sd5sum2) VALUES (:sd5sum1,:sd5sum2)";

        DUF_TRACE( sd5, 0, "%016llx%016llx %s%s", md64[1], md64[0], real_path, filename );
        DUF_SQL_START_STMT( pdi, insert_sd5, sql, lr, pstmt );
        DUF_TRACE( insert, 0, "S:%s", sql );
        DUF_SQL_BIND_LL( sd5sum1, md64[1], lr, pstmt );
        DUF_SQL_BIND_LL( sd5sum2, md64[0], lr, pstmt );
        DUF_SQL_STEP( lr, pstmt );
        DUF_SQL_CHANGES( changes, lr, pstmt );
        DUF_SQL_END_STMT( insert_sd5, lr, pstmt );
      }
      else
      {
        lr = duf_sql( "INSERT OR IGNORE INTO " DUF_DBPREF "sd5 (sd5sum1,sd5sum2) VALUES ('%lld','%lld')", &changes, md64[1], md64[0] );
      }
    }
    duf_pdi_reg_changes( pdi, changes );
    if ( ( lr == DUF_SQL_CONSTRAINT || !lr ) && !changes )
    {
      if ( need_id )
      {
        duf_scan_callbacks_t sccb = {.leaf.fieldset = "sd5id" };
        duf_sccb_handle_t csccbh = {.sccb = &sccb };
        lr = duf_sql_select( duf_sel_cb_field_by_sccb, &sd5id, STR_CB_DEF, STR_CB_UDATA_DEF, &csccbh /*, ( const duf_dirhandle_t * ) NULL off */ ,
                             "SELECT " DUF_SQL_IDNAME " AS sd5id FROM " DUF_DBPREF "sd5 WHERE sd5sum1='%lld' AND sd5sum2='%lld'", md64[1], md64[0] );
      }
    }
    else if ( !lr /* assume SQLITE_OK */  )
    {
      if ( need_id && changes )
      {
        sd5id = duf_sql_last_insert_rowid(  );
      }
    }
    else
    {
      DUF_SHOW_ERROR( "insert sd5 %d", lr );
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

  DEBUG_ENDULL( sd5id );
  return sd5id;
}

static int
duf_make_sd5_uni( int fd, unsigned char *pmd )
{
  size_t bufsz = 256 * 1;
  MD5_CTX ctx;

  DEBUG_STARTR( r );

  memset( &ctx, 0, sizeof( ctx ) );
  {
    char *buffer;

    buffer = mas_malloc( bufsz );
    if ( buffer )
    {
      int cnt = 0;
      int maxcnt = 2;

      if ( !duf_config->cli.disable.flag.calculate && ( MD5_Init( &ctx ) != 1 ) )
        DUF_MAKE_ERROR( r, DUF_ERROR_MD5 );
      DUF_TEST_R( r );
      /* lseek( fd, -bufsz * maxcnt, SEEK_END ); */
      while ( r >= 0 && cnt++ < maxcnt )
      {
        int rr;

        rr = read( fd, buffer, bufsz );
        if ( rr < 0 )
        {
          DUF_ERRSYS( "read file" );

          DUF_MAKE_ERROR( r, DUF_ERROR_READ );
          DUF_TEST_R( r );
          break;
        }
        if ( rr > 0 )
        {
          if ( !duf_config->cli.disable.flag.calculate && MD5_Update( &ctx, buffer, rr ) != 1 )
            DUF_MAKE_ERROR( r, DUF_ERROR_MD5 );
        }
        if ( rr <= 0 )
          break;

        DUF_TEST_R( r );
      }
      mas_free( buffer );
    }
    else
    {
      DUF_MAKE_ERROR( r, DUF_ERROR_MEMORY );
    }
  }
  if ( duf_config->cli.disable.flag.calculate )
  {
  }
  else if ( MD5_Final( pmd, &ctx ) != 1 )
    DUF_MAKE_ERROR( r, DUF_ERROR_MD5 );
  DEBUG_ENDR( r );
}

/* static int                                                                                                                               */
/* duf_scan_dirent_content( int fd, const struct stat *pst_file, duf_depthinfo_t * pdi, duf_record_t * precord )                            */
/* {                                                                                                                                        */
/*   unsigned char mdr[MD5_DIGEST_LENGTH];                                                                                                  */
/*   unsigned char md[MD5_DIGEST_LENGTH];                                                                                                   */
/*                                                                                                                                          */
  /* DEBUG_STARTR( r ); */
/*   DUF_UFIELD( filedataid );                                                                                                              */
/*   DUF_SFIELD( filename );                                                                                                                */
/*                                                                                                                                          */
/*   memset( md, 0, sizeof( md ) );                                                                                                         */
/*   if ( !duf_config->cli.disable.flag.calculate )                                                                                         */
/*     r = duf_make_sd5_uni( fd, md );                                                                                                      */
/*   DUF_TEST_R( r );                                                                                                                       */
/*   (* reverse *)                                                                                                                          */
/*   for ( int i = 0; i < sizeof( md ) / sizeof( md[0] ); i++ )                                                                             */
/*     mdr[i] = md[sizeof( md ) / sizeof( md[0] ) - i - 1];                                                                                 */
/*   if ( r >= 0 )                                                                                                                          */
/*   {                                                                                                                                      */
/*     unsigned long long sd5id = 0;                                                                                                        */
/*     unsigned long long *pmd;                                                                                                             */
/*                                                                                                                                          */
/*     pmd = ( unsigned long long * ) &mdr;                                                                                                 */
/*     if ( duf_config->cli.disable.flag.calculate )                                                                                        */
/*       pmd[0] = pmd[1] = duf_levinfo_dirid( pdi );                                                                                        */
/*     sd5id = duf_insert_sd5_uni( pdi, pmd, filename, pst_file->st_size, 1 (*need_id *) , &r );                                            */
/*     if ( r >= 0 && sd5id )                                                                                                               */
/*     {                                                                                                                                    */
/*       int changes = 0;                                                                                                                   */
/*                                                                                                                                          */
/*       if ( r >= 0 && !duf_config->cli.disable.flag.update )                                                                              */
/*         r = duf_sql( "UPDATE " DUF_DBPREF "filedatas SET sd5id='%llu' WHERE " DUF_SQL_IDNAME "='%lld'", &changes, sd5id, filedataid );   */
/*       duf_pdi_reg_changes( pdi, changes );                                                                                               */
/*       DUF_TEST_R( r );                                                                                                                   */
/*     }                                                                                                                                    */
/*     DUF_TRACE( sd5, 0, "%016llx%016llx : sd5id: %llu", pmd[1], pmd[0], sd5id );                                                          */
/*     (* DUF_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan 5    * %016llx%016llx : %llu", duf_pdi_depth( pdi ), pmd[1], pmd[0], sd5id ); *) */
/*   }                                                                                                                                      */
  /* DEBUG_ENDR( r ); */
/* }                                                                                                                                        */

static int
duf_scan_dirent_sd5_content2( duf_sqlite_stmt_t * pstmt, int fd, const struct stat *pst_file, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  unsigned char mdr[MD5_DIGEST_LENGTH];
  unsigned char md[MD5_DIGEST_LENGTH];

  assert( fd == duf_levinfo_dfd( pdi ) );
  assert( pst_file == duf_levinfo_stat( pdi ) );


  DUF_UFIELD2( filedataid );
  DUF_SFIELD2( filename );
  DUF_TRACE( sd5, 0, "+" );

  memset( md, 0, sizeof( md ) );
  if ( !duf_config->cli.disable.flag.calculate )
    r = duf_make_sd5_uni( fd, md );
  DUF_TEST_R( r );
  /* reverse */
  for ( int i = 0; i < sizeof( md ) / sizeof( md[0] ); i++ )
    mdr[i] = md[sizeof( md ) / sizeof( md[0] ) - i - 1];

  if ( r >= 0 )
  {
    unsigned long long sd5id = 0;
    unsigned long long *pmd;

    pmd = ( unsigned long long * ) &mdr;
    if ( duf_config->cli.disable.flag.calculate )
      pmd[0] = pmd[1] = duf_levinfo_dirid( pdi ) + 74;
    sd5id = duf_insert_sd5_uni( pdi, pmd, filename, pst_file->st_size, 1 /*need_id */ , &r );
    if ( r >= 0 && sd5id )
    {
      int changes = 0;

      if ( r >= 0 && !duf_config->cli.disable.flag.update )
        r = duf_sql( "UPDATE " DUF_DBPREF "filedatas SET sd5id=%llu WHERE " DUF_SQL_IDNAME "=%lld", &changes, sd5id, filedataid );
      duf_pdi_reg_changes( pdi, changes );
      DUF_TEST_R( r );
    }
    DUF_TRACE( sd5, 0, "%016llx%016llx : sd5id: %llu", pmd[1], pmd[0], sd5id );
    /* DUF_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan 5    * %016llx%016llx : %llu", duf_pdi_depth( pdi ), pmd[1], pmd[0], sd5id ); */
  }
  DEBUG_ENDR( r );
}



/* 
 * this is callback of type: duf_scan_hook_dir_t
 * */
__attribute__ ( ( unused ) )
     static int collect_openat_sd5_scan_node_before( unsigned long long pathid_unused, duf_depthinfo_t * pdi, duf_record_t * precord )
{
  const char *real_path = NULL;

  DEBUG_STARTR( r );

  real_path = duf_levinfo_path( pdi );
  DUF_TRACE( sd5, 0, "L%d; ID%-7llu  real_path=%s;", duf_pdi_depth( pdi ), pathid_unused, real_path );

  DEBUG_ENDR( r );
}

/* currently used for --same-sd5  ??? */




static duf_beginning_t final_sql = {.done = 0,
  .sql = {
          "UPDATE " DUF_DBPREF "sd5 SET dup2cnt=(SELECT COUNT(*) " /* */
          " FROM " DUF_DBPREF "sd5 AS sd " /* */
          " JOIN " DUF_DBPREF "filedatas AS fd ON (fd.md5id=sd." DUF_SQL_IDNAME ") " /* */
          " WHERE " DUF_DBPREF "sd5.sd5sum1=sd.sd5sum1 AND " DUF_DBPREF "sd5.sd5sum2=sd.sd5sum2)" /* */
          ,
#if 0
          "INSERT OR IGNORE INTO " DUF_DBPREF "pathtot_dirs (Pathid, numdirs) " /* */
          "SELECT parents." DUF_SQL_IDNAME " AS Pathid, COUNT(*) AS numdirs " /* */
          " FROM " DUF_DBPREF "paths " /* */
          " JOIN " DUF_DBPREF "paths AS parents ON (parents." DUF_SQL_IDNAME "=paths.parentid) " /* */
          " GROUP BY parents." DUF_SQL_IDNAME "" /* */
          ,

          "UPDATE " DUF_DBPREF "pathtot_dirs SET " /* */
          " numdirs=(SELECT COUNT(*) AS numdirs " /* */
          " FROM " DUF_DBPREF "paths AS p " /* */
          " WHERE p.ParentId=" DUF_DBPREF "pathtot_dirs.Pathid )" /* */
          ,
#endif

          NULL,
          }
};



duf_scan_callbacks_t duf_collect_openat_sd5_callbacks = {
  .title = "collect sd5",
  .name = "sd5",
  .init_scan = NULL,
  .def_opendir = 1,
  /* .dirent_dir_scan_before = NULL, */
  /* .dirent_file_scan_before = NULL, */
  /* .node_scan_before = collect_openat_sd5_scan_node_before, */
  /*  .leaf_scan =  collect_openat_sd5_scan_leaf, */
  /* .leaf_scan_fd = duf_scan_dirent_content, */
  .leaf_scan_fd2 = duf_scan_dirent_sd5_content2,
  .leaf = {.fieldset = "fn.Pathid AS dirid " /* */
           ", fd." DUF_SQL_IDNAME " AS filedataid, fd.inode AS inode " /* */
           ", fn.name AS filename, fd.size AS filesize " /* */
           ", uid, gid, nlink, inode, strftime('%s',mtim) AS mtime, md.dup5cnt AS nsame " /* */
           ", fn." DUF_SQL_IDNAME " AS filenameid " /* */
           ", fd.mode AS filemode, md.md5sum1, md.md5sum2 " /* */
           ", fd.md5id AS md5id" /* */
           ,
           /* .selector = "SELECT %s FROM " DUF_DBPREF "filenames AS fn " (* *)       */
           /*       " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " (* *) */
           /*       " LEFT JOIN " DUF_DBPREF "md5 AS md ON (md." DUF_SQL_IDNAME "=fd.md5id)" (* *)         */
           /*       " LEFT JOIN " DUF_DBPREF "sizes as sz ON (sz.size=fd.size)" (* *)      */
           /*       "    WHERE "            (* *)                                          */
           /*       " fd.sd5id IS NULL AND "    (* *)                                      */
           /*       " sz.size > 0 AND "                                                    */
           /*       (* " sz.dupzcnt > 1 AND "  (* *) *)                                    */
           /*       " fn.Pathid='%llu' "    (* *)                                          */
           /*       ,                                                                      */
           .selector2 =         /* */
           /* "SELECT %s " */
           " FROM " DUF_DBPREF "filenames AS fn " /* */
           " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "md5 AS md ON (md." DUF_SQL_IDNAME "=fd.md5id)" /* */
           " LEFT JOIN " DUF_DBPREF "sizes as sz ON (sz.size=fd.size)" /* */
           "    WHERE "         /* */
           " fd.sd5id IS NULL AND " /* */
           " sz.size > 0 AND "
           /* "       sz.dupzcnt > 1 AND "  (* *) */
           " fn.Pathid=:parentdirID " /* */
           ,
           .selector_total2 =   /* */
           " FROM " DUF_DBPREF "filenames AS fn " /* */
           " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "md5 AS md ON (md." DUF_SQL_IDNAME "=fd.md5id)" /* */
           " LEFT JOIN " DUF_DBPREF "sizes as sz ON (sz.size=fd.size)" /* */
           "    WHERE "         /* */
           " fd.sd5id IS NULL AND " /* */
           /* "       sz.dupzcnt > 1 AND "      (* *) */
           " sz.size > 0  "},
  .node = {.fieldset = "pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname,  pt.ParentId " /* */
           ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize" /* */
           ,
           .selector2 =         /* */
           " FROM " DUF_DBPREF "paths AS pt " /* */
           " LEFT JOIN " DUF_SQL_TABLES_PATHTOT_DIRS_FULL "  AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_SQL_TABLES_PATHTOT_FILES_FULL " AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
#if 0
           " LEFT JOIN " DUF_DBPREF "pathtot_dirs AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "pathtot_files AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
#endif
           " WHERE pt.ParentId=:parentdirID AND ( :dirName IS NULL OR dirname=:dirName )" /* */
           },
  .final_sql_argv = &final_sql,
};
