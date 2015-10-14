#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <openssl/md5.h>




#include "duf_maintenance.h"

#include "duf_config_ref.h"


#include "duf_pdi_ref.h"
#include "duf_pdi_stmt.h"

#include "duf_levinfo_ref.h"


#include "duf_sql_stmt_defs.h"

#include "duf_sql_defs.h"
#include "duf_sql_field.h"

#include "duf_sql_bind.h"
#include "duf_sql_prepared.h"

/* #include "duf_dbg.h" */

#include "sql_beginning_tables.h"
/* ########################################################################################## */
static int sd5_dirent_content2( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi );

/* ########################################################################################## */
static duf_sql_sequence_t final_sql = /* */
{
  .name = "final @ sd5",
  .done = 0,
  .sql = {
          "UPDATE " DUF_SQL_TABLES_SD5_FULL " SET dup2cnt=(SELECT COUNT(*) " /* */
          " FROM " DUF_SQL_TABLES_SD5_FULL " AS sd " /* */
          " JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fd.sd5id=sd." DUF_SQL_IDFIELD ") " /* */
          " WHERE " DUF_SQL_TABLES_SD5_FULL "." DUF_SQL_IDFIELD "=sd." DUF_SQL_IDFIELD ")" /* */
          /* " WHERE " DUF_SQL_TABLES_SD5_FULL ".sd5sum1=sd.sd5sum1 AND " DUF_SQL_TABLES_SD5_FULL ".sd5sum2=sd.sd5sum2)" (* *) */
          ,
          NULL}
};

/* ########################################################################################## */


duf_scan_callbacks_t duf_collect_openat_sd5_callbacks = {
  .title = "collect sd5",
  .name = "sd5",
  .init_scan = NULL,
  .def_opendir = 1,





  .leaf_scan_fd2 = sd5_dirent_content2,

/* TODO : exp;ain values of use_std_leaf and use_std_node TODO */
  .use_std_leaf = 0,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node = 0,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .leaf = {
           .name = "sd leaf",
           .type = DUF_NODE_LEAF,
           .fieldset =          /* */
#if 0
           /* "'sd5-leaf' AS fieldset_id, " (* *) */
           " fn.Pathid AS dirid " /* */
           ", 0 AS ndirs, 0 AS nfiles" /* */
           " , fd." DUF_SQL_IDFIELD " AS filedataid " /* */
           " , fd." DUF_SQL_IDFIELD " AS dataid " /* */
           " , fd.inode AS inode " /* */
           " , fn." DUF_SQL_FILENAMEFIELD " AS fname, fn." DUF_SQL_FILENAMEFIELD " AS dfname, fd.size AS filesize " /* */
           " , fd.dev, fd.uid, fd.gid, fd.nlink, STRFTIME('%s',fd.mtim) AS mtime, fd.rdev, fd.blksize, fd.blocks " /* */
           " , sd.dup2cnt AS nsame " /* */
           " , md.dup5cnt            AS dup5cnt " /* */
           " , sz.dupzcnt            AS dupzcnt " /* */
           " , fn." DUF_SQL_IDFIELD " AS filenameid " /* */
           " , fn." DUF_SQL_IDFIELD " AS nameid " /* */
           " , fd.mode AS filemode, md.md5sum1, md.md5sum2 " /* */
           " , fd.md5id AS md5id" /* */
#else
           "#sd5"
#endif
           ,
           .selector2 =         /* */
#if 0
           " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn " /* */
           " LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fn.dataid=fd." DUF_SQL_IDFIELD ") " /* */
           " LEFT JOIN " DUF_SQL_TABLES_SIZES_FULL " AS sz ON (sz.size=fd.size)" /* */
           " LEFT JOIN " DUF_SQL_TABLES_MD5_FULL " AS md ON (md." DUF_SQL_IDFIELD "=fd.md5id)" /* */
           " LEFT JOIN " DUF_SQL_TABLES_SD5_FULL " AS sd ON (sd." DUF_SQL_IDFIELD "=fd.sd5id)" /* */
#else
           "#md5-leaf"
#endif
           ,
           .matcher = " fn.Pathid=:parentdirID " /* */
           ,                    /* */
           .filter =            /* */
           "( fd.sd5id IS NULL OR sd." DUF_SQL_IDFIELD " IS NULL ) " /*                           */ " AND " /* */
           "( sz.size  IS NULL OR sz.size > 0 ) " /*                                             */ " AND " /* */
           "(  :fFast  IS NULL OR sz.size IS NULL OR sz.dupzcnt IS NULL OR sz.dupzcnt > 1 ) " /* */ " AND " /* */
           " 1 "                /* */
           ,
           .count_aggregate = "DISTINCT fd." DUF_SQL_IDFIELD}
  ,
  .node = {
           .name = "sd node",
           .type = DUF_NODE_NODE,
           .expand_sql = 1,     /* */
           .fieldset =          /* */
           /* "'sd5-node' AS fieldset_id, " (* *) */
           " pt." DUF_SQL_IDFIELD " AS dirid" /* */
           ", pt." DUF_SQL_IDFIELD " AS nameid " /* */
           ", pt." DUF_SQL_DIRNAMEFIELD " AS dname, pt." DUF_SQL_DIRNAMEFIELD " AS dfname,  pt.ParentId " /* */
           ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize" /* */
           ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks, STRFTIME( '%s', pt.mtim ) AS mtime " /* */
           ,
           .selector2 =         /* */
           " FROM " DUF_SQL_TABLES_PATHS_FULL " AS pt " /* */
           " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL "  AS td ON (td.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
           " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL " AS tf ON (tf.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
#if 0
           " LEFT JOIN " DUF_DBPREF "pathtot_dirs AS td ON (td.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
           " LEFT JOIN " DUF_DBPREF "pathtot_files AS tf ON (tf.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
#endif
           ,
           .matcher = "pt.ParentId=:parentdirID AND ( :dirName IS NULL OR dname=:dirName )" /* */
           ,                    /* */
           .filter = NULL       /* */
           ,
#if 0
           .selector_total2 =   /* */
           " /* sd5 */ FROM " DUF_SQL_TABLES_PATHS_FULL " AS p " /* */
#endif
           },
  .final_sql_seq = &final_sql,
};

/* ########################################################################################## */
unsigned long long
duf_pdistat2file_sd5id_existed( duf_depthinfo_t * pdi, unsigned long sd5sum1, unsigned long sd5sum2, int *pr )
{
  int rpr = 0;
  unsigned long long sd5id = 0;
  const char *sql = "SELECT " DUF_SQL_IDFIELD " AS sd5id FROM " DUF_SQL_TABLES_SD5_FULL " WHERE sd5sum1=:sd5Sum1 AND sd5sum2=:sd5Sum2"
        /* " INDEXED BY " DUF_SQL_TABLES_SD5 "_uniq WHERE  sd5sum1=:sd5Sum1 AND sd5sum2=:sd5Sum2 */
        ;

  DEBUG_START(  );

  DUF_SQL_START_STMT( pdi, select_sd5, sql, rpr, pstmt );
  DUF_TRACE( select, 3, "S:%s", sql );
  DUF_SQL_BIND_LL( sd5Sum1, sd5sum1, rpr, pstmt );
  DUF_SQL_BIND_LL( sd5Sum2, sd5sum2, rpr, pstmt );
  DUF_SQL_STEP( rpr, pstmt );
  if ( DUF_IS_ERROR_N( rpr, DUF_SQL_ROW ) )
  {
    DUF_TRACE( select, 10, "<selected>" );
    /* sd5id = duf_sql_column_long_long( pstmt, 0 ); */
    sd5id = DUF_GET_UFIELD2( sd5id );
    /* rpr = 0; */
  }
  else
  {
    /* DUF_TEST_R( rpr ); */
    DUF_TRACE( select, 10, "<NOT selected> (%d)", rpr );
  }
  /* DUF_TEST_R( rpr ); */
  DUF_SQL_END_STMT( pdi, select_sd5, rpr, pstmt );
  if ( pr )
    *pr = rpr;
  DEBUG_ENDULL( sd5id );
}

static unsigned long long
duf_insert_sd5_uni( duf_depthinfo_t * pdi, unsigned long long *sd64, const char *msg, int need_id, int *pr )
{
  unsigned long long sd5id = -1;
  int lr = 0;
  int changes = 0;

#ifdef MAS_TRACING
  const char *real_path = duf_levinfo_path( pdi );
#endif

  DEBUG_START(  );
  if ( sd64 && sd64[1] && sd64[0] )
  {
    if ( !DUF_CONFIGG( opt.disable.flag.insert ) )
    {
      static const char *sql = "INSERT OR IGNORE INTO " DUF_SQL_TABLES_SD5_FULL " ( sd5sum1, sd5sum2 ) VALUES ( :sd5sum1, :sd5sum2 )";

      DUF_TRACE( sd5, 0, "%016llx%016llx %s%s", sd64[1], sd64[0], real_path, msg );
      DUF_SQL_START_STMT( pdi, insert_sd5, sql, lr, pstmt );
      DUF_TRACE( insert, 0, "S:%s", sql );
      DUF_SQL_BIND_LL( sd5sum1, sd64[1], lr, pstmt );
      DUF_SQL_BIND_LL( sd5sum2, sd64[0], lr, pstmt );
      DUF_SQL_STEPC( lr, pstmt );
      DUF_SQL_CHANGES( changes, lr, pstmt );
      DUF_SQL_END_STMT( pdi, insert_sd5, lr, pstmt );
    }
    duf_pdi_reg_changes( pdi, changes );
    if ( ( DUF_IS_ERROR_N( lr, DUF_SQL_CONSTRAINT ) || !lr ) && !changes )
    {
      if ( need_id )
        sd5id = duf_pdistat2file_sd5id_existed( pdi, sd64[1], sd64[0], &lr );
    }
    else if ( DUF_NOERROR( lr ) /* assume SQLITE_OK */  )
    {
      if ( need_id && changes )
      {
        sd5id = duf_sql_last_insert_rowid(  );
      }
    }
    /* else                                     */
    /* {                                        */
    /*   DUF_SHOW_ERROR( "insert sd5 %d", lr ); */
    /* }                                        */
  }
  else
  {
    /* DUF_SHOW_ERROR( "Wrong data" ); */
    DUF_MAKE_ERROR( lr, DUF_ERROR_DATA );
    /* DUF_TEST_R( lr ); */
  }

  if ( pr )
    *pr = lr;

  DEBUG_ENDULL( sd5id );
  return sd5id;
}

static int
duf_make_sd5_uni( int fd, unsigned char *pmd )
{
  DEBUG_STARTR( r );
  size_t bufsz = 256 * 1;
  MD5_CTX ctx;

  memset( &ctx, 0, sizeof( ctx ) );
  DUF_TRACE( sd5, 0, "make" );
  {
    char *buffer;

    buffer = mas_malloc( bufsz );
    if ( buffer )
    {
      if ( !DUF_CONFIGG( opt.disable.flag.calculate ) && ( MD5_Init( &ctx ) != 1 ) )
        DUF_MAKE_ERROR( r, DUF_ERROR_MD5 );
      DUF_TEST_R( r );
      {
        int maxcnt = 2;
        int cnt = 0;

        while ( DUF_NOERROR( r ) && ( maxcnt == 0 || cnt++ < maxcnt ) )
        {
          int rr;

          DUF_TRACE( sd5, 10, "read fd:%u", fd );
          rr = read( fd, buffer, bufsz );
          DUF_TRACE( sd5, 10, "read rr:%u", rr );
          if ( rr < 0 )
          {
            DUF_ERRSYS( "read file" );

            DUF_MAKE_ERROR( r, DUF_ERROR_READ );
            DUF_TEST_R( r );
            break;
          }
          if ( rr > 0 && !DUF_CONFIGG( opt.disable.flag.calculate ) )
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
  if ( !DUF_CONFIGG( opt.disable.flag.calculate ) && MD5_Final( pmd, &ctx ) != 1 )
    DUF_MAKE_ERROR( r, DUF_ERROR_MD5 );
  DEBUG_ENDR( r );
}

/* 20150820.143755 */
static int
sd5_dirent_content2( duf_stmnt_t * pstmt, /* const struct stat *pst_file_needless, */ duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  unsigned char amd5r[MD5_DIGEST_LENGTH];
  unsigned char amd5[MD5_DIGEST_LENGTH];


  DUF_SFIELD2( fname );
  DUF_TRACE( sd5, 0, "+ %s", fname );

  memset( amd5, 0, sizeof( amd5 ) );
  DUF_TRACE( sd5, 0, "+ %s", fname );
  if ( !DUF_CONFIGG( opt.disable.flag.calculate ) )
    DOR( r, duf_make_sd5_uni( duf_levinfo_dfd( pdi ), amd5 ) );
  DUF_TRACE( sd5, 0, "+ %s", fname );
  DUF_TEST_R( r );
  /* reverse */
  for ( unsigned i = 0; i < sizeof( amd5 ) / sizeof( amd5[0] ); i++ )
    amd5r[i] = amd5[sizeof( amd5 ) / sizeof( amd5[0] ) - i - 1];

  if ( DUF_NOERROR( r ) )
  {
    unsigned long long sd5id = 0;
    unsigned long long *pmd;

    pmd = ( unsigned long long * ) &amd5r;
    DUF_TRACE( sd5, 0, "insert %s", fname );
    if ( DUF_CONFIGG( opt.disable.flag.calculate ) )
      pmd[0] = pmd[1] = duf_levinfo_dirid( pdi ) + 74; /* FIXME What is it? */
    sd5id = duf_insert_sd5_uni( pdi, pmd, fname /* for dbg message only */ , 1 /*need_id */ , &r );
    if ( sd5id )
    {
      int changes = 0;

      if ( !DUF_CONFIGG( opt.disable.flag.update ) )
      {
        DUF_UFIELD2( filedataid );
#if 0
        DOR( r, duf_sql( "UPDATE " DUF_SQL_TABLES_FILEDATAS_FULL " SET sd5id='%llu' WHERE " DUF_SQL_IDFIELD "='%lld'", &changes, sd5id, filedataid ) );
#else
        const char *sql = "UPDATE " DUF_SQL_TABLES_FILEDATAS_FULL " SET sd5id=:sd5Id WHERE " DUF_SQL_IDFIELD " =:dataId ";

        DUF_SQL_START_STMT( pdi, update_sd5id, sql, r, pstmt );
        DUF_TRACE( mod, 3, "S:%s", sql );
        DUF_SQL_BIND_LL( sd5Id, sd5id, r, pstmt );
        DUF_SQL_BIND_LL( dataId, filedataid, r, pstmt );
        DUF_SQL_STEPC( r, pstmt );
        DUF_SQL_CHANGES( changes, r, pstmt );
        DUF_SQL_END_STMT( pdi, update_sd5id, r, pstmt );
#endif

      }
      duf_pdi_reg_changes( pdi, changes );
    }
    DUF_TRACE( sd5, 0, "%016llx%016llx : sd5id: %llu", pmd[1], pmd[0], sd5id );
    /* DUF_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan 5    * %016llx%016llx : %llu", duf_pdi_depth( pdi ), pmd[1], pmd[0], sd5id ); */
  }
  DEBUG_ENDR( r );
}
