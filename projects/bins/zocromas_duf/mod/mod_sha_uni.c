#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <openssl/sha.h>

#include "duf_maintenance.h"
#include "duf_sccb_types.h"                                          /* duf_scan_callbacks_t */

#include "duf_config.h"
#include "duf_config_trace.h"
#include "duf_config_util.h"
#include "duf_config_ref.h"
#include "duf_config_defs.h"
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
static int sha1_dirent_content2( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi );

#define ASHA1_DELTA 4

/* ########################################################################################## */
#define FILTER_DATA "fd.sha1id IS NULL"

static duf_sql_sequence_t final_sql =                                /* */
{
  .name = "final-sha1",
  .done = 0,
  .sql = {
          "UPDATE " DUF_SQL_TABLES_SHA1_FULL " SET dupsha1cnt=(SELECT COUNT(*) " /* */
          " FROM " DUF_SQL_TABLES_SHA1_FULL " AS sh "                /* */
          " JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fd.sha1id=sh." DUF_SQL_IDFIELD ") " /* */
          " WHERE " DUF_SQL_TABLES_SHA1_FULL "." DUF_SQL_IDFIELD "=sh." DUF_SQL_IDFIELD ")" /* */
          ,
          NULL,
          }
};

/* ########################################################################################## */

duf_scan_callbacks_t duf_sha1_callbacks = {
  .title = "collect sha1",
  .name = "sha1",
  .init_scan = NULL,
  .def_opendir = 1,
/* .dirent_dir_scan_before = NULL, */
/* .dirent_file_scan_before = NULL, */
/* .node_scan_before = collect_openat_sha1_node_before, */
/*  .leaf_scan =  collect_openat_sha1_leaf, */
/* .leaf_scan_fd = duf_dirent_sha1_content, */
  .leaf_scan_fd2 = sha1_dirent_content2,

/* TODO : explain values of use_std_leaf_set_num and use_std_node_set_num TODO */
  .use_std_leaf_set_num = 2,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node_set_num = 2,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
/* .std_leaf_set_name = "std-leaf-no-sel", */
  .std_leaf_set_name = "std-leaf-no-sel-fd",
  .std_node_set_name = "std-node-two",
  .leaf = {
           .name = "sha1-leaf",
           .type = DUF_NODE_LEAF,
           .fieldset =                                               /* */
           "#sha1",
           .fieldsets = {
                         "#basic",
                         "#plus",
                         "#sha1x",
                         NULL}
           ,
           .selector2 =                                              /* */
           "#sha1-leaf",
           .matcher = " fn.Pathid=:parentdirID "                     /* */
           ,                                                         /* */
#if 0
           .filter =                                                 /* */
           "( " FILTER_DATA " OR sh." DUF_SQL_IDFIELD " IS NULL ) " /*                                          */ " AND " /* */
           "( sz.size   IS NULL OR sz.size > 0 ) " /*                                                            */ " AND " /* */
           "(  :fFast   IS NULL OR sz.size IS NULL OR sz.dupzcnt > 1 ) " /*                                      */ " AND " /* */
           "(  :fFast   IS NULL OR sd." DUF_SQL_IDFIELD " IS NULL OR sd.dup2cnt IS NULL OR sd.dup2cnt > 1 ) " /*  */ " AND " /* */
           "(  :fFast   IS NULL OR md." DUF_SQL_IDFIELD " IS NULL OR md.dup5cnt IS NULL OR md.dup5cnt > 1 ) " /*  */ " AND " /* */
           " 1 "                                                     /* */
           ,
#else
           .afilter_fresh = {FILTER_DATA " OR sh." DUF_SQL_IDFIELD " IS NULL", "sz.size  IS NULL OR sz.size > 0"},
           .afilter_fast = {
                            "sz.size IS NULL OR sz.dupzcnt IS NULL OR sz.dupzcnt > 1",
                            "sd." DUF_SQL_IDFIELD " IS NULL OR sd.dup2cnt IS NULL OR sd.dup2cnt > 1",
                            "md." DUF_SQL_IDFIELD " IS NULL OR md.dup5cnt IS NULL OR md.dup5cnt > 1"},
#endif
         /* .count_aggregate = "DISTINCT fd." DUF_SQL_IDFIELD (* *) */
           }
  ,                                                                  /* */
  .node = {                                                          /* */
           .name = "sha1-node",
           .type = DUF_NODE_NODE,
           .expand_sql = 1,                                          /* */
           .fieldset =                                               /* */
         /* "'sha1-node' AS fieldset_id, " (* *) */
           " pt." DUF_SQL_IDFIELD " AS dirid"                        /* */
           ", pt." DUF_SQL_IDFIELD " AS nameid "                     /* */
           ", pt." DUF_SQL_DIRNAMEFIELD " AS dname, pt." DUF_SQL_DIRNAMEFIELD " AS dfname,  pt.ParentId " /* */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
           ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize" /* */
#endif
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_RNUMS )
           ", " DUF_SQL_RNUMDIRS( pt ) " AS rndirs "                 /* */
           ", (" DUF_SQL__RNUMFILES( pt ) " WHERE " FILTER_DATA ") AS rnfiles " /* */
#endif
           ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks, STRFTIME( '%s', pt.mtim ) AS mtime " /* */
           ,
           .selector2 =                                              /* */
           " FROM " DUF_SQL_TABLES_PATHS_FULL " AS pt "              /* */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
           " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_DIRS_FULL "  AS td ON (td.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
           " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_FILES_FULL " AS tf ON (tf.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
#endif
           ,
           .matcher = " pt.ParentId=:parentdirID AND ( :dirName IS NULL OR dname=:dirName )" /* */
           ,
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
           .filter = " rnfiles > 0 "                                 /* */
#endif
           },
  .final_sql_seq = &final_sql,
};

/* ########################################################################################## */
unsigned long long
duf_pdistat2file_sha1id_existed( duf_depthinfo_t * pdi, unsigned long sha1sum1, unsigned long sha1sum2, unsigned long sha1sum3, int *pr )
{
  int rpr = 0;
  unsigned long long sha1id = 0;
  const char *sql =
          "SELECT " DUF_SQL_IDFIELD " AS sha1id FROM " DUF_SQL_TABLES_SHA1_FULL
          " WHERE sha1sum1=:sha1Sum1 AND sha1sum2=:sha1Sum2 AND sha1sum3=:sha1Sum3"
        /* " INDEXED BY " DUF_SQL_TABLES_SD5 "_uniq WHERE  sha1sum1=:sha1Sum1 AND sha1sum2=:sha1Sum2 AND sha1sum3=:sha1Sum3 */
          ;

  DUF_START(  );

  DUF_SQL_START_STMT( pdi, select_sha1, sql, rpr, pstmt );
  DUF_TRACE( select, 3, "S:%s", sql );
  DUF_SQL_BIND_LL( sha1Sum1, sha1sum1, rpr, pstmt );
  DUF_SQL_BIND_LL( sha1Sum2, sha1sum2, rpr, pstmt );
  DUF_SQL_BIND_LL( sha1Sum3, sha1sum3, rpr, pstmt );
  DUF_SQL_STEP( rpr, pstmt );
  if ( DUF_IS_ERROR_N( rpr, DUF_SQL_ROW ) )
  {
    DUF_TRACE( select, 10, "<selected>" );
  /* sha1id = duf_sql_column_long_long( pstmt, 0 ); */
    sha1id = DUF_GET_UFIELD2( sha1id );
  /* rpr = 0; */
  }
  else
  {
  /* DUF_TEST_R( rpr ); */
    DUF_TRACE( select, 10, "<NOT selected> (%d)", rpr );
  }
/* DUF_TEST_R( rpr ); */
  DUF_SQL_END_STMT( pdi, select_sha1, rpr, pstmt );
  if ( pr )
    *pr = rpr;
  DUF_ENDULL( sha1id );
}

static unsigned long long
duf_insert_sha1_uni( duf_depthinfo_t * pdi, unsigned long long *sha1, const char *msg DUF_UNUSED, int need_id, int *pr )
{
  unsigned long long sha1id = -1;
  int lr = 0;
  int changes = 0;

#ifdef MAS_TRACING
  const char *real_path = duf_levinfo_path( pdi );
#endif

  DUF_START(  );

  assert( sizeof( unsigned long long ) == 8 );
  assert( SHA_DIGEST_LENGTH == 2 * sizeof( unsigned long long ) + ASHA1_DELTA );
  assert( SHA_DIGEST_LENGTH == 2 * 64 / 8 + ASHA1_DELTA );
  if ( sha1 && sha1[2] && sha1[1] && sha1[0] )
  {
    if ( !DUF_CONFIGG( opt.disable.flag.insert ) )
    {
      static const char *sql =
              "INSERT OR IGNORE INTO " DUF_SQL_TABLES_SHA1_FULL " ( sha1sum1, sha1sum2, sha1sum3 ) VALUES ( :sha1sum1, :sha1sum2, :sha1sum3 )";

      DUF_TRACE( sha1, 0, "%08llx%016llx%016llx %s%s", sha1[2], sha1[1], sha1[0], real_path, msg );
      DUF_SQL_START_STMT( pdi, insert_sha1, sql, lr, pstmt );
      DUF_TRACE( insert, 0, "S:%s", sql );
      DUF_SQL_BIND_LL( sha1sum1, sha1[2], lr, pstmt );
      DUF_SQL_BIND_LL( sha1sum2, sha1[1], lr, pstmt );
      DUF_SQL_BIND_LL( sha1sum3, sha1[0], lr, pstmt );
      DUF_SQL_STEPC( lr, pstmt );
      DUF_SQL_CHANGES( changes, lr, pstmt );
      DUF_SQL_END_STMT( pdi, insert_sha1, lr, pstmt );
    }
    duf_pdi_reg_changes( pdi, changes );
    if ( ( DUF_IS_ERROR_N( lr, DUF_SQL_CONSTRAINT ) || !lr ) && !changes )
    {
      if ( need_id )
        sha1id = duf_pdistat2file_sha1id_existed( pdi, sha1[2], sha1[1], sha1[0], &lr );
    }
    else if ( DUF_NOERROR( lr ) /* assume SQLITE_OK */  )
    {
      if ( need_id && changes )
      {
        sha1id = duf_sql_last_insert_rowid(  );
      }
    }
  /* else                                      */
  /* {                                         */
  /*   DUF_SHOW_ERROR( "insert sha1 %d", lr ); */
  /* }                                         */
  }
  else
  {
  /* DUF_SHOW_ERROR( "Wrong data" ); */
    DUF_MAKE_ERROR( lr, DUF_ERROR_DATA );
  /* DUF_TEST_R( lr ); */
  }

  if ( pr )
    *pr = lr;

  DUF_ENDULL( sha1id );
  return sha1id;
}

static int
duf_make_sha1_uni( int fd, unsigned long long *pbytes, unsigned char *pmd )
{
  DUF_STARTR( r );
  size_t bufsz = 1024 * 1024 * 1;
  SHA_CTX ctx;

  memset( &ctx, 0, sizeof( ctx ) );
  DUF_TRACE( sha1, 0, "make" );
  {
    char *buffer;

    buffer = mas_malloc( bufsz );
    if ( buffer )
    {
      if ( !DUF_CONFIGG( opt.disable.flag.calculate ) && ( SHA1_Init( &ctx ) != 1 ) )
        DUF_MAKE_ERROR( r, DUF_ERROR_SHA1 );
      DUF_TEST_R( r );
      {
        int maxcnt = 0;
        int cnt = 0;

        while ( DUF_NOERROR( r ) && ( maxcnt == 0 || cnt++ < maxcnt ) )
        {
          int ry;

          DUF_TRACE( sha1, 10, "read fd:%u", fd );
          ry = read( fd, buffer, bufsz );
        /* TODO: if (ry>0)  sscbh->bytes+=ry */
          DUF_TRACE( sha1, 10, "read ry:%u", ry );
          if ( ry < 0 )
          {
            DUF_ERRSYS( "read file" );

            DUF_MAKE_ERROR( r, DUF_ERROR_READ );
            DUF_TEST_R( r );
            break;
          }
          if ( ry > 0 )
          {
            if ( !DUF_CONFIGG( opt.disable.flag.calculate ) )
            {
              if ( pbytes )
                ( *pbytes ) += ry;
              if ( SHA1_Update( &ctx, buffer, ry ) != 1 )
                DUF_MAKE_ERROR( r, DUF_ERROR_SHA1 );
            }
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
  if ( !DUF_CONFIGG( opt.disable.flag.calculate ) && SHA1_Final( pmd, &ctx ) != 1 )
    DUF_MAKE_ERROR( r, DUF_ERROR_SHA1 );
  DUF_ENDR( r );
}

static int
duf_make_sha1r_uni( duf_depthinfo_t * pdi, unsigned char *pmdr )
{
  DUF_STARTR( r );

  unsigned char asha1[SHA_DIGEST_LENGTH];
  unsigned long long bytes = 0;
  int fd;

  memset( asha1, 0, sizeof( asha1 ) );
  fd = duf_levinfo_dfd( pdi );
  DOR( r, duf_make_sha1_uni( fd, &bytes, asha1 ) );
/* reverse */
  for ( unsigned i = 0; i < sizeof( asha1 ) / sizeof( asha1[0] ); i++ )
    pmdr[i] = asha1[sizeof( asha1 ) / sizeof( asha1[0] ) - i - 1];

  pdi->total_bytes += bytes;
  pdi->total_files++;

  DUF_ENDR( r );
}

static int
sha1_dirent_content2( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
{
  DUF_STARTR( r );
  unsigned char asha1r[SHA_DIGEST_LENGTH + ASHA1_DELTA];

  DUF_SFIELD2( fname );
  DUF_TRACE( sha1, 0, "+ %s", fname );
  memset( asha1r, 0, sizeof( asha1r ) );
  if ( !DUF_CONFIGG( opt.disable.flag.calculate ) )
    DOR( r, duf_make_sha1r_uni( pdi, asha1r ) );

  assert( sizeof( asha1r ) == 20 + ASHA1_DELTA );
  if ( DUF_NOERROR( r ) )
  {
    unsigned long long sha1id = 0;
    unsigned long long *pmd = ( unsigned long long * ) &asha1r;

    DUF_TRACE( sha1, 0, "insert %s", fname );

    sha1id = duf_insert_sha1_uni( pdi, pmd, fname /* for dbg message only */ , 1 /*need_id */ , &r );
    if ( sha1id )
    {
      int changes = 0;

      pdi->cnts.dirent_content2++;
      if ( !DUF_CONFIGG( opt.disable.flag.update ) )
      {
        DUF_UFIELD2( filedataid );
#if 0
        DOR( r,
             duf_sql( "UPDATE " DUF_SQL_TABLES_FILEDATAS_FULL " SET sha1id='%llu' WHERE " DUF_SQL_IDFIELD "='%lld'", &changes, sha1id, filedataid ) );
#else
        const char *sql = "UPDATE " DUF_SQL_TABLES_FILEDATAS_FULL " SET sha1id=:sha1Id WHERE " DUF_SQL_IDFIELD " =:dataId ";

        DUF_SQL_START_STMT( pdi, update_sha1id, sql, r, pstmt );
        DUF_TRACE( mod, 3, "S:%s", sql );
        DUF_SQL_BIND_LL( sha1Id, sha1id, r, pstmt );
        DUF_SQL_BIND_LL( dataId, filedataid, r, pstmt );
        DUF_SQL_STEPC( r, pstmt );
        DUF_SQL_CHANGES( changes, r, pstmt );
        DUF_SQL_END_STMT( pdi, update_sha1id, r, pstmt );
#endif
      }
      duf_pdi_reg_changes( pdi, changes );
      DUF_TEST_R( r );
    }
    DUF_TRACE( sha1, 4, "%02x:%2x:%2x:%2x:%2x:%2x", asha1r[SHA_DIGEST_LENGTH - 0], asha1r[SHA_DIGEST_LENGTH - 1], asha1r[SHA_DIGEST_LENGTH - 2],
               asha1r[SHA_DIGEST_LENGTH - 3], asha1r[SHA_DIGEST_LENGTH - 4], asha1r[SHA_DIGEST_LENGTH - 5] );
    DUF_TRACE( sha1, 0, "%08llx%016llx%016llx : sha1id: %llu", pmd[2], pmd[1], pmd[0], sha1id );
  /* DUF_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan 5    * %016llx%016llx : %llu", duf_pdi_depth( pdi ), pmd[1], pmd[0], sha1id ); */
  }
  DUF_ENDR( r );
}
