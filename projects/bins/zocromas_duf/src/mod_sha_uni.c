#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <openssl/sha.h>




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
static int sha_dirent_content2( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi );

/* ########################################################################################## */
static duf_sql_sequence_t final_sql = /* */
{
  .name = "final @ sha",
  .done = 0,
  .sql = {
          "UPDATE " DUF_SQL_TABLES_SHA_FULL " SET dupshacnt=(SELECT COUNT(*) " /* */
          " FROM " DUF_SQL_TABLES_SHA_FULL " AS sh " /* */
          " JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fd.shaid=sh." DUF_SQL_IDNAME ") " /* */
          " WHERE " DUF_SQL_TABLES_SHA_FULL "." DUF_SQL_IDNAME "=sh." DUF_SQL_IDNAME ")" /* */
          ,
          NULL,
          }
};

/* ########################################################################################## */

duf_scan_callbacks_t duf_sha_callbacks = {
  .title = "collect sha",
  .name = "sha",
  .init_scan = NULL,
  .def_opendir = 1,
  /* .dirent_dir_scan_before = NULL, */
  /* .dirent_file_scan_before = NULL, */
  /* .node_scan_before = collect_openat_sha_node_before, */
  /*  .leaf_scan =  collect_openat_sha_leaf, */
  /* .leaf_scan_fd = duf_dirent_sha_content, */
  .leaf_scan_fd2 = sha_dirent_content2,

/* TODO : exp;ain values of use_std_leaf and use_std_node TODO */
  .use_std_leaf = 0,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node = 0,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .leaf = {
           .name = "sha leaf",
           .type = DUF_NODE_LEAF,
           .fieldset =          /* */
           "#sha",
           .fieldsets = {
                         "#basic",
                         "#plus",
                         "#shax",
                         NULL}
           ,
           .selector2 =         /* */
           "#sha-leaf",
           .matcher = " fn.Pathid=:parentdirID " /* */
           ,                    /* */
           .filter =            /* */
           " ( fd.shaid   IS NULL OR sha." DUF_SQL_IDNAME " IS NULL ) " /*                */ " AND " /* */
           " sz.size > 0 " /*                                                            */ " AND " /* */
           "(  :fFast IS NULL OR sz.size IS NULL OR sz.dupzcnt > 1 ) " /*                */ " AND " /* */
           "(  :fFast IS NULL OR sd." DUF_SQL_IDNAME " IS NULL OR sd.dup2cnt > 1 ) " /*  */ " AND " /* */
           " 1 "                /* */
           ,
           .count_aggregate = "DISTINCT fd." DUF_SQL_IDNAME}
  ,                             /* */
  .node = {                     /* */
           .name = "sha node",
           .type = DUF_NODE_NODE,
           .expand_sql = 1,     /* */
           .fieldset =          /* */
           /* "'sha-node' AS fieldset_id, " (* *) */
           " pt." DUF_SQL_IDNAME " AS dirid" /* */
           ", pt." DUF_SQL_IDNAME " AS nameid " /* */
           ", pt.dirname, pt.dirname AS dfname,  pt.ParentId " /* */
           ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize" /* */
           ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks, STRFTIME( '%s', pt.mtim ) AS mtime " /* */
           ,
           .selector2 =         /* */
           /* "SELECT     pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname,  pt.ParentId "                  */
           /* ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " */
           " FROM " DUF_SQL_TABLES_PATHS_FULL " AS pt " /* */
           " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL "  AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL " AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           ,
           .matcher = " pt.ParentId=:parentdirID AND ( :dirName IS NULL OR dirname=:dirName )" /* */
           ,
           .filter = NULL       /* */
           },
  .final_sql_seq = &final_sql,
};

/* ########################################################################################## */
unsigned long long
duf_pdistat2file_shaid_existed( duf_depthinfo_t * pdi, unsigned long shasum1, unsigned long shasum2, unsigned long shasum3, int *pr )
{
  int rpr = 0;
  unsigned long long shaid = 0;
  const char *sql =
        "SELECT " DUF_SQL_IDNAME " AS shaid FROM " DUF_SQL_TABLES_SHA_FULL " WHERE shasum1=:shaSum1 AND shasum2=:shaSum2 AND shasum3=:shaSum3"
        /* " INDEXED BY " DUF_SQL_TABLES_SD5 "_uniq WHERE  shasum1=:shaSum1 AND shasum2=:shaSum2 AND shasum3=:shaSum3 */
        ;

  DEBUG_START(  );

  DUF_SQL_START_STMT( pdi, select_sha, sql, rpr, pstmt );
  DUF_TRACE( select, 3, "S:%s", sql );
  DUF_SQL_BIND_LL( shaSum1, shasum1, rpr, pstmt );
  DUF_SQL_BIND_LL( shaSum2, shasum2, rpr, pstmt );
  DUF_SQL_BIND_LL( shaSum3, shasum3, rpr, pstmt );
  DUF_SQL_STEP( rpr, pstmt );
  if ( rpr == MAS_SQL_ROW )
  {
    DUF_TRACE( select, 10, "<selected>" );
    /* shaid = duf_sql_column_long_long( pstmt, 0 ); */
    shaid = DUF_GET_UFIELD2( shaid );
    rpr = 0;
  }
  else
  {
    /* DUF_TEST_R( rpr ); */
    DUF_TRACE( select, 10, "<NOT selected> (%d)", rpr );
  }
  /* DUF_TEST_R( rpr ); */
  DUF_SQL_END_STMT( select_sha, rpr, pstmt );
  if ( pr )
    *pr = rpr;
  DEBUG_ENDULL( shaid );
}

static unsigned long long
duf_insert_sha_uni( duf_depthinfo_t * pdi, unsigned long long *sha, const char *msg, int need_id, int *pr )
{
  unsigned long long shaid = -1;
  int lr = 0;
  int changes = 0;

#ifdef MAS_TRACING
  const char *real_path = duf_levinfo_path( pdi );
#endif

  DEBUG_START(  );

  assert( sizeof( unsigned long long ) == 8 );
  assert( SHA_DIGEST_LENGTH == 2 * sizeof( unsigned long long ) + 4 );
  assert( SHA_DIGEST_LENGTH == 2 * 64 / 8 + 4 );
  if ( sha && sha[2] && sha[1] && sha[0] )
  {
    if ( !DUF_CONFIGG( cli.disable.flag.insert ) )
    {
      static const char *sql =
            "INSERT OR IGNORE INTO " DUF_SQL_TABLES_SHA_FULL " ( shasum1, shasum2, shasum3 ) VALUES ( :shasum1, :shasum2, :shasum3 )";

      DUF_TRACE( sha, 0, "%08llx%016llx%016llx %s%s", sha[2], sha[1], sha[0], real_path, msg );
      DUF_SQL_START_STMT( pdi, insert_sha, sql, lr, pstmt );
      DUF_TRACE( insert, 0, "S:%s", sql );
      DUF_SQL_BIND_LL( shasum1, sha[2], lr, pstmt );
      DUF_SQL_BIND_LL( shasum2, sha[1], lr, pstmt );
      DUF_SQL_BIND_LL( shasum3, sha[0], lr, pstmt );
      DUF_SQL_STEP( lr, pstmt );
      DUF_SQL_CHANGES( changes, lr, pstmt );
      DUF_SQL_END_STMT( insert_sha, lr, pstmt );
    }
    duf_pdi_reg_changes( pdi, changes );
    if ( ( lr == MAS_SQL_CONSTRAINT || !lr ) && !changes )
    {
      if ( need_id )
        shaid = duf_pdistat2file_shaid_existed( pdi, sha[2], sha[1], sha[0], &lr );
    }
    else if ( !lr /* assume SQLITE_OK */  )
    {
      if ( need_id && changes )
      {
        shaid = duf_sql_last_insert_rowid(  );
      }
    }
    else
    {
      DUF_SHOW_ERROR( "insert sha %d", lr );
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

  DEBUG_ENDULL( shaid );
  return shaid;
}

static int
duf_make_sha_uni( int fd, unsigned char *pmd )
{
  DEBUG_STARTR( r );
  size_t bufsz = 1024 * 1024 * 1;
  SHA_CTX ctx;

  memset( &ctx, 0, sizeof( ctx ) );
  DUF_TRACE( sha, 0, "make" );
  {
    char *buffer;

    buffer = mas_malloc( bufsz );
    if ( buffer )
    {
      if ( !DUF_CONFIGG( cli.disable.flag.calculate ) && ( SHA1_Init( &ctx ) != 1 ) )
        DUF_MAKE_ERROR( r, DUF_ERROR_SHA );
      DUF_TEST_R( r );
      {
        int maxcnt = 0;
        int cnt = 0;

        while ( DUF_NOERROR( r ) && ( maxcnt == 0 || cnt++ < maxcnt ) )
        {
          int ry;

          DUF_TRACE( sha, 10, "read fd:%u", fd );
          ry = read( fd, buffer, bufsz );
          DUF_TRACE( sha, 10, "read ry:%u", ry );
          if ( ry < 0 )
          {
            DUF_ERRSYS( "read file" );

            DUF_MAKE_ERROR( r, DUF_ERROR_READ );
            DUF_TEST_R( r );
            break;
          }
          if ( ry > 0 && !DUF_CONFIGG( cli.disable.flag.calculate ) )
          {
            if ( SHA1_Update( &ctx, buffer, ry ) != 1 )
              DUF_MAKE_ERROR( r, DUF_ERROR_SHA );
          }
          if ( ry <= 0 )
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
  if ( !DUF_CONFIGG( cli.disable.flag.calculate ) && SHA1_Final( pmd, &ctx ) != 1 )
    DUF_MAKE_ERROR( r, DUF_ERROR_SHA );
  DEBUG_ENDR( r );
}

static int
duf_make_shar_uni( duf_depthinfo_t * pdi, unsigned char *pmdr )
{
  DEBUG_STARTR( r );
  unsigned char asha[SHA_DIGEST_LENGTH];
  int fd;

  memset( asha, 0, sizeof( asha ) );
  fd = duf_levinfo_dfd( pdi );
  DOR( r, duf_make_sha_uni( fd, asha ) );
  /* reverse */
  for ( int i = 0; i < sizeof( asha ) / sizeof( asha[0] ); i++ )
    pmdr[i] = asha[sizeof( asha ) / sizeof( asha[0] ) - i - 1];


  DEBUG_ENDR( r );
}

static int
sha_dirent_content2( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
#define ASHA_DELTA 4
  unsigned char ashar[SHA_DIGEST_LENGTH + ASHA_DELTA];

  DUF_SFIELD2( filename );
  DUF_TRACE( sha, 0, "+ %s", filename );
  memset( ashar, 0, sizeof( ashar ) );
  if ( !DUF_CONFIGG( cli.disable.flag.calculate ) )
    DOR( r, duf_make_shar_uni( pdi, ashar ) );

  assert( sizeof( ashar ) == 20 + ASHA_DELTA );
  if ( DUF_NOERROR( r ) )
  {
    unsigned long long shaid = 0;
    unsigned long long *pmd = ( unsigned long long * ) &ashar;

    DUF_TRACE( sha, 0, "insert %s", filename );


    shaid = duf_insert_sha_uni( pdi, pmd, filename /* for dbg message only */ , 1 /*need_id */ , &r );
    if ( shaid )
    {
      int changes = 0;

      pdi->cnts.dirent_content2++;
      if ( !DUF_CONFIGG( cli.disable.flag.update ) )
      {
        DUF_UFIELD2( filedataid );
#if 0
        DOR( r, duf_sql( "UPDATE " DUF_SQL_TABLES_FILEDATAS_FULL " SET shaid='%llu' WHERE " DUF_SQL_IDNAME "='%lld'", &changes, shaid, filedataid ) );
#else
        const char *sql = "UPDATE " DUF_SQL_TABLES_FILEDATAS_FULL " SET shaid=:shaId WHERE " DUF_SQL_IDNAME " =:dataId ";

        DUF_SQL_START_STMT( pdi, update_shaid, sql, r, pstmt );
        DUF_TRACE( mod, 3, "S:%s", sql );
        DUF_SQL_BIND_LL( shaId, shaid, r, pstmt );
        DUF_SQL_BIND_LL( dataId, filedataid, r, pstmt );
        DUF_SQL_STEP( r, pstmt );
        DUF_SQL_CHANGES( changes, r, pstmt );
        DUF_SQL_END_STMT( update_shaid, r, pstmt );
#endif
      }
      duf_pdi_reg_changes( pdi, changes );
      DUF_TEST_R( r );
    }
    DUF_TRACE( sha, 4, "%02x:%2x:%2x:%2x:%2x:%2x", ashar[SHA_DIGEST_LENGTH - 0], ashar[SHA_DIGEST_LENGTH - 1], ashar[SHA_DIGEST_LENGTH - 2],
               ashar[SHA_DIGEST_LENGTH - 3], ashar[SHA_DIGEST_LENGTH - 4], ashar[SHA_DIGEST_LENGTH - 5] );
    DUF_TRACE( sha, 0, "%08llx%016llx%016llx : shaid: %llu", pmd[2], pmd[1], pmd[0], shaid );
    /* DUF_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan 5    * %016llx%016llx : %llu", duf_pdi_depth( pdi ), pmd[1], pmd[0], shaid ); */
  }
  DEBUG_ENDR( r );
#undef  ASHA_DELTA
}
