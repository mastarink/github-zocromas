/* #undef MAS_TRACING */
#include <assert.h>                                                  /* assert */
#include <stddef.h>                                                  /* NULL */
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <openssl/md5.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

/*  #include "duf_tracen_defs.h"  (*  T; TT; TR ♠  *) */
/*  #include "duf_errorn_defs.h"  (*  DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠  *) */

/*  #include "duf_start_end.h"  (*  DUF_STARTR ; DUF_ENDR ♠  *) */
/*  #include "duf_dodefs.h"  (*  DOR ♠  *) */

#include "duf_sccb_types.h"                                          /* duf_scan_callbacks_t ✗ */

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */
/* #include "duf_config_ref.h" */
/* #include "duf_config_defs.h"                                         (* DUF_CONF... ♠ *) */

#include "duf_pdi_ref.h"
#include "duf_pdi_stmt.h"                                            /* duf_pdi_find_statement_by_id; etc. ✗ */

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */

/* #include "duf_sql_stmt_defs.h"                                       (* DUF_SQL_BIND_S_OPT etc. ♠ *) */
#include "duf_sql_se_stmt_defs.h"                                    /* DUF_SQL_SE_BIND_S_OPT etc. ✗ */

#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ✗ */
#include "duf_sql_field.h"                                           /* __duf_sql_str_by_name2 for DUF_GET_UFIELD2 etc. ✗ */

#include "duf_sql_bind.h"                                            /* duf_sql_... for DUF_SQL_BIND_... etc. ✗ */
#include "duf_sql_prepared.h"                                        /* duf_sql_(prepare|step|finalize) ✗ */

/* #include "duf_dbg.h" */

#include "sql_beginning_tables.h"                                    /* DUF_SQL_TABLES... etc. ✗ */
/* ########################################################################################## */
static int duf_digest_dirent_content2( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi );

#define ADIGEST_DELTA 0

/* ########################################################################################## */
#define MOD_DIGEST_LENGTH MD5_DIGEST_LENGTH
#define MOD_DIGEST_TABLE DUF_SQL_TABLES_MD5_FULL
#define MOD_DIGEST_DATA_S "md5"
#define MOD_DIGEST_CTX MD5_CTX
#define MOD_DIGEST_Init MD5_Init
#define MOD_DIGEST_Update MD5_Update
#define MOD_DIGEST_Final MD5_Final
#define FILTER_DATA "fd." MOD_DIGEST_DATA_S "id IS NULL"

static duf_sql_sequence_t final_sql =                                /* */
{
  .name = "final-" MOD_DIGEST_DATA_S,
  .done = 0,
  .sql = {
          "UPDATE " MOD_DIGEST_TABLE " SET dup5cnt=(SELECT COUNT(*) " /* */
          " FROM " MOD_DIGEST_TABLE " AS md "                        /* */
          " JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fd." MOD_DIGEST_DATA_S "id=md." DUF_SQL_IDFIELD ") " /* */
          " WHERE " MOD_DIGEST_TABLE "." DUF_SQL_IDFIELD "=md." DUF_SQL_IDFIELD ")" /* */
          ,
#if 0
          "DELETE FROM path_pairs"                                   /* */
          ,
          "INSERT OR IGNORE INTO path_pairs (samefiles, Pathid1, Pathid2) SELECT COUNT(*), fna.Pathid AS Pathid1, fnb.Pathid  AS Pathid2" /* */
          " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fna"           /* */
          "   JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fda ON (fna.dataid=fda." DUF_SQL_IDFIELD ")" /* */
          "   JOIN " MOD_DIGEST_TABLE " AS mda ON (fda." MOD_DIGEST_DATA_S "id=mda." DUF_SQL_IDFIELD ")" /* */
          "   JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fdb ON (fdb." MOD_DIGEST_DATA_S "id=mda." DUF_SQL_IDFIELD ")" /* */
          "   JOIN " DUF_SQL_TABLES_FILENAMES_FULL " AS fnb ON (fdb." DUF_SQL_IDFIELD "=fnb.dataid)" /* */
          " WHERE Pathid1 < Pathid2 AND fna." DUF_SQL_FILENAMEFIELD "=fnb." DUF_SQL_FILENAMEFIELD "" /* */
          " GROUP BY Pathid1, Pathid2"                               /* */
          ,
#endif
          NULL,
          }
};

/* ########################################################################################## */

duf_scan_callbacks_t duf_md5_callbacks = {
  .title = "collect " MOD_DIGEST_DATA_S,
  .name = MOD_DIGEST_DATA_S,
  .init_scan = NULL,
  .def_opendir = 1,
/* .dirent_dir_scan_before = NULL, */
/* .dirent_file_scan_before = NULL, */
  .leaf_scan_fd2 = duf_digest_dirent_content2,

/* TODO : explain values of use_std_leaf_set_num and use_std_node_set_num TODO */
  .use_std_leaf_set_num = 2,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node_set_num = 2,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .std_leaf_set_name = "std-leaf-no-sel-fd",
  .std_node_set_name = "std-node-two",
  .leaf = {
           .name = MOD_DIGEST_DATA_S "-leaf",
           .type = DUF_NODE_LEAF,
           .fieldset = "#" MOD_DIGEST_DATA_S,                        /* from _all_fieldsets */
           .fieldsets = {
                         "#basic",
                         "#plus",
                         "#" MOD_DIGEST_DATA_S "x",
                         NULL}
           ,
         /* .selector2 = "#" MOD_DIGEST_DATA_S "-leaf",               (* *) */
           .selector2 = "#std-ns-fd-leaf",                           /* from _all_selectors */
           .matcher = " fn.Pathid=:parentdirID "                     /* */
           ,                                                         /* */
           .afilter_fresh = {FILTER_DATA " OR md." DUF_SQL_IDFIELD " IS NULL", "sz.size  IS NULL OR sz.size > 0"},
           .afilter_fast = {
                            "sz.size IS NULL OR sz.dupzcnt IS NULL OR sz.dupzcnt > 1",
                            "sd." DUF_SQL_IDFIELD " IS NULL OR sd.dup2cnt    IS NULL OR sd.dup2cnt > 1",
                            "sh." DUF_SQL_IDFIELD " IS NULL OR sh.dupsha1cnt IS NULL OR sh.dupsha1cnt > 1"},
         /* .count_aggregate = "DISTINCT fd." DUF_SQL_IDFIELD (* *) */
           }
  ,                                                                  /* */
  .node = {                                                          /* */
           .name = MOD_DIGEST_DATA_S "-node",
           .type = DUF_NODE_NODE,
           .expand_sql = 1,                                          /* */
           .fieldset =                                               /* */
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
static
SRP( MOD, unsigned long long, digestid, 0, pdistat2file_digestid_existed, duf_depthinfo_t * pdi, unsigned long digestsum1, unsigned long digestsum2 )
{
/* int rpr = 0; */
/* unsigned long long digestid = 0; */
  const char *sql = "SELECT " DUF_SQL_IDFIELD " AS digestid FROM " MOD_DIGEST_TABLE " WHERE " MOD_DIGEST_DATA_S "sum1=:digestSum1 AND " MOD_DIGEST_DATA_S "sum2=:digestSum2" /* */
        /* " INDEXED BY " DUF_SQL_TABLES_SD5 "_uniq WHERE  " MOD_DIGEST_DATA_S"sum1=:digestSum1 AND "MOD_DIGEST_DATA_S"sum2=:digestSum2 */
          ;

/* DUF_START(  ); */
  assert( sizeof( unsigned long ) == 8 );
  assert( sizeof( unsigned long long ) == 8 );

  DUF_SQL_SE_START_STMT( pdi, select_md5, sql, pstmt );
  MAST_TRACE( select, 3, "S:%s", sql );
  DUF_SQL_SE_BIND_LL( digestSum1, digestsum1, pstmt );
  DUF_SQL_SE_BIND_LL( digestSum2, digestsum2, pstmt );

  DUF_SQL_SE_STEP( pstmt );
  if ( QISERR1_N( SQL_ROW ) )
  {
    MAST_TRACE( select, 10, "<selected>" );
    digestid = DUF_GET_UFIELD2( digestid );
  /* rpr = 0; */
  }
  else
  {
  /* DUF_TEST_R( rpr ); */
    MAST_TRACE( select, 10, "<NOT selected> (%d)", QERRIND );
  }
/* DUF_TEST_R( rpr ); */
  DUF_SQL_SE_END_STMT( pdi, select_md5, pstmt );
/* if ( pr ) */
/* *pr = rpr; */
/* DUF_ENDULL( digestid ); */
  ERP( MOD, unsigned long long, digestid, 0, pdistat2file_digestid_existed, duf_depthinfo_t * pdi, unsigned long digestsum1,
       unsigned long digestsum2 );
}

static
SRP( MOD, unsigned long long, digestid, -1, insert_digest_uni, duf_depthinfo_t * pdi, unsigned long long *digest64, const char *msg MAS_UNUSED,
     int need_id )
{
/* unsigned long long digestid = -1; */
/* int lr = 0; */
  int changes = 0;

#ifdef MAS_TRACING
  const char *real_path = duf_levinfo_path( pdi );
#endif

/* DUF_START(  ); */

  assert( sizeof( unsigned long long ) == 8 );
  assert( MOD_DIGEST_LENGTH == 2 * sizeof( unsigned long long ) + ADIGEST_DELTA );
  assert( MOD_DIGEST_LENGTH == 2 * 64 / 8 + ADIGEST_DELTA );
  if ( digest64 && digest64[1] && digest64[0] )
  {
    if ( !duf_get_config_flag_disable_insert(  ) )
    {
      static const char *sql = "INSERT OR IGNORE INTO " MOD_DIGEST_TABLE " ( " MOD_DIGEST_DATA_S "sum1," MOD_DIGEST_DATA_S "sum2" /* */
            /* */
              ") VALUES ( :digestsum1, :digestsum2 "                 /* */
              " )";

      MAST_TRACE( digest, 0, "%016llx%016llx %s%s", digest64[1], digest64[0], real_path, msg );
      DUF_SQL_SE_START_STMT( pdi, insert_md5, sql, pstmt );
      MAST_TRACE( insert, 0, "S:%s", sql );
      DUF_SQL_SE_BIND_LL( digestsum1, digest64[1], pstmt );
      DUF_SQL_SE_BIND_LL( digestsum2, digest64[0], pstmt );

      DUF_SQL_SE_STEPC( pstmt );
      DUF_SQL_SE_CHANGES( changes, pstmt );
      DUF_SQL_SE_END_STMT( pdi, insert_md5, pstmt );
    }
    duf_pdi_reg_changes( pdi, changes );
    if ( ( QISERR1_N( SQL_CONSTRAINT ) || QNOERR ) && !changes )
    {
      if ( need_id )
#if 0
        digestid = duf_pdistat2file_digestid_existed( pdi, digest64[1], digest64[0], QPERRIND );
#else
        digestid = CRP( pdistat2file_digestid_existed, pdi, digest64[1], digest64[0] );
#endif
    }
    else if ( QNOERR /* assume SQLITE_OK */  )
    {
      if ( need_id && changes )
      {
        digestid = duf_sql_last_insert_rowid(  );
      }
    }
  }
  else
  {
  /* DUF_SHOW_ERROR( "Wrong data" ); */
    ERRMAKE( DATA );
  /* DUF_TEST_R( lr ); */
  }

/* if ( pr ) */
/* *pr = lr; */

/* DUF_ENDULL( digestid ); */
/* return digestid; */
  ERP( MOD, unsigned long long, digestid, -1, insert_digest_uni, duf_depthinfo_t * pdi, unsigned long long *digest64, const char *msg MAS_UNUSED,
       int need_id );
}

static
SR( MOD, make_digest_uni, int fd, unsigned long long *pbytes, unsigned char *pdgst )
{
/*   DUF_STARTR( r ) */ ;
  size_t bufsz = 1024 * 1024 * 1;
  MOD_DIGEST_CTX ctx;

  memset( &ctx, 0, sizeof( ctx ) );
  MAST_TRACE( digest, 0, "make" );
  {
    char *buffer;

    buffer = mas_malloc( bufsz );
    if ( buffer )
    {
      if ( !duf_get_config_flag_disable_calculate(  ) && ( MOD_DIGEST_Init( &ctx ) != 1 ) )
        ERRMAKE( DIGEST );
/* DUF_TEST_R( r ); */
      {
        int maxcnt = 0;
        int cnt = 0;

        while ( QNOERR && ( maxcnt == 0 || cnt++ < maxcnt ) )
        {
          int ry;

          MAST_TRACE( digest, 10, "read fd:%u", fd );
          ry = read( fd, buffer, bufsz );
          MAST_TRACE( digest, 10, "read ry:%u", ry );
        /* TODO: if (ry>0)  sscbh->bytes+=ry */
          if ( ry < 0 )
          {
          /* DUF_ERRSYS( "read file" ); */
            MASE_ERRSYS( "read file" );

            ERRMAKE( READ );
/* DUF_TEST_R( r ); */
            break;
          }
          if ( ry > 0 )
          {
            if ( pbytes )
              ( *pbytes ) += ry;
            if ( !duf_get_config_flag_disable_calculate(  ) )
            {
              if ( MOD_DIGEST_Update( &ctx, buffer, ry ) != 1 )
                ERRMAKE( DIGEST );
            }
          }
          if ( ry <= 0 )
            break;
/* DUF_TEST_R( r ); */
        }
      }
      mas_free( buffer );
    }
    else
    {
      ERRMAKE( MEMORY );
    }
  }
  if ( !duf_get_config_flag_disable_calculate(  ) && MOD_DIGEST_Final( pdgst, &ctx ) != 1 )
    ERRMAKE( DIGEST );
/*  DUF_ENDR( r );*/
  ER( MOD, make_digest_uni, int fd, unsigned long long *pbytes, unsigned char *pdgst );
}

static
SR( MOD, make_digestr_uni, duf_depthinfo_t * pdi, unsigned char *pmdr )
{
/*   DUF_STARTR( r ) */ ;
  int fd;
  unsigned char adigest[MOD_DIGEST_LENGTH];
  unsigned long long bytes = 0;

  memset( adigest, 0, sizeof( adigest ) );
  fd = duf_levinfo_dfd( pdi );
  CR( make_digest_uni, fd, &bytes, adigest );

/* reverse */
  for ( unsigned i = 0; i < sizeof( adigest ) / sizeof( adigest[0] ); i++ )
    pmdr[i] = adigest[sizeof( adigest ) / sizeof( adigest[0] ) - i - 1];

  pdi->total_bytes += bytes;
  pdi->total_files++;

/*  DUF_ENDR( r );*/
  ER( MOD, make_digestr_uni, duf_depthinfo_t * pdi, unsigned char *pmdr );
}

static
SR( MOD, digest_dirent_content2, duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
{
/*   DUF_STARTR( r ) */ ;
  unsigned char adigestr[MOD_DIGEST_LENGTH + ADIGEST_DELTA];

  DUF_SFIELD2( fname );
  MAST_TRACE( digest, 0, "+ %s", fname );
  memset( adigestr, 0, sizeof( adigestr ) );
  if ( !duf_get_config_flag_disable_calculate(  ) )
    CR( make_digestr_uni, pdi, adigestr );

  if ( QNOERR )
  {
    unsigned long long digestid = 0;
    unsigned long long *pdgst = ( unsigned long long * ) &adigestr;

    MAST_TRACE( digest, 0, "insert %s", fname );

#if 0
    digestid = duf_insert_digest_uni( pdi, pdgst, fname /* for dbg message only */ , 1 /*need_id */ , QPERRIND );
#else
    digestid = CRP( insert_digest_uni, pdi, pdgst, fname /* for dbg message only */ , 1 /*need_id */  );
#endif
    if ( digestid )
    {

      pdi->cnts.dirent_content2++;
      if ( !duf_get_config_flag_disable_update(  ) )
      {
        int changes = 0;

        DUF_UFIELD2( filedataid );
        const char *sql = "UPDATE " DUF_SQL_TABLES_FILEDATAS_FULL " SET " MOD_DIGEST_DATA_S "id=:digestId WHERE " DUF_SQL_IDFIELD " =:dataId ";

        DUF_SQL_SE_START_STMT( pdi, update_md5id, sql, pstmt );
        MAST_TRACE( mod, 3, "S:%s", sql );
        DUF_SQL_SE_BIND_LL( digestId, digestid, pstmt );
        DUF_SQL_SE_BIND_LL( dataId, filedataid, pstmt );
        DUF_SQL_SE_STEPC( pstmt );
        DUF_SQL_SE_CHANGES( changes, pstmt );
        DUF_SQL_SE_END_STMT( pdi, update_md5id, pstmt );
        duf_pdi_reg_changes( pdi, changes );
      }
/* DUF_TEST_R( r ); */
    }
    MAST_TRACE( digest, 4, "%016llx%016llx : " MOD_DIGEST_DATA_S "id: %llu", pdgst[1], pdgst[0], digestid );
  /* MAST_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan 5    * %016llx%016llx : %llu", duf_pdi_depth( pdi ), pdgst[1], pdgst[0], digestid ); */
  }
/*  DUF_ENDR( r );*/
  ER( MOD, digest_dirent_content2, duf_stmnt_t * pstmt, duf_depthinfo_t * pdi );
}
