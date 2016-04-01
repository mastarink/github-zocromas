/* #define DUF_GET_FIELD_FROM_ROW */
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

#include "duf_sccb_structs.h"

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_pdi_ref.h"
#include "duf_pdi_stmt.h"                                            /* duf_pdi_find_statement_by_id; etc. ✗ */

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */

#include "duf_sql_se_stmt_defs.h"                                    /* DUF_SQL_SE_BIND_S_OPT etc. ✗ */

#include "duf_sccb_row_field_defs.h"                                 /* DUF_*FIELD2* ✗ */
#include "duf_sccb_row.h"                                            /* datarow_*; duf_sccbh_row_get_*; sccbh_rows_eval ✗ */

#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ✗ */
#include "duf_sql_field.h"                                           /* __duf_sql_str_by_name2 for DUF_GET_QUFIELD2 etc. ✗ */

#include "duf_sql_bind.h"                                            /* duf_sql_... for DUF_SQL_BIND_... etc. ✗ */
#include "duf_sql_prepared.h"                                        /* duf_sql_prepare; duf_sql_step; duf_sql_finalize; ✗ */

#include "sql_beginning_tables.h"                                    /* DUF_SQL_TABLES... etc. ✗ */

#include "duf_pdi_structs.h"
/* #include "duf_levinfo_structs.h" */
/* ########################################################################################## */
static int duf_sd5_dirent_content2( duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );

/* ########################################################################################## */
#define FILTER_DATA "fd.sd5id IS NULL"

static duf_sql_sequence_t final_sql =                                /* */
{
  .name = "final-sd5",
  .done = 0,
  .sql = {
          "UPDATE " DUF_SQL_TABLES_SD5_FULL " SET dup2cnt=(SELECT COUNT(*) " /* */
          " FROM " DUF_SQL_TABLES_SD5_FULL " AS sd "                 /* */
          " JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fd.sd5id=sd." DUF_SQL_IDFIELD ") " /* */
          " WHERE " DUF_SQL_TABLES_SD5_FULL "." DUF_SQL_IDFIELD "=sd." DUF_SQL_IDFIELD ")" /* */
        /* " WHERE " DUF_SQL_TABLES_SD5_FULL ".sd5sum1=sd.sd5sum1 AND " DUF_SQL_TABLES_SD5_FULL ".sd5sum2=sd.sd5sum2)" (* *) */
          ,
          NULL}
};

/* ########################################################################################## */

duf_scan_callbacks_t duf_mod_handler = {
  .title = "collect sd5",
  .name = "sd5",
  .init_scan = NULL,
  .def_opendir = 1,

  .leaf_scan_fd2 = duf_sd5_dirent_content2,

/* TODO : explain values of use_std_leaf_set_num and use_std_node_set_num TODO */
  .use_std_leaf_set_num = 2,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node_set_num = 2,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
/* .std_leaf_set_name = "std-leaf-no-sel", */
  .std_leaf_set_name = "std-leaf-no-sel-fd",
  .std_node_set_name = "std-node-two",
  .leaf = {
           .name = "sd-leaf",
           .type = DUF_NODE_LEAF,
           .fieldset = "#sd5",
           .selector2 = "#md5-leaf",
           .matcher = " fn.Pathid=:parentdirID "                     /* */
           ,
#if 0
           .filter =                                                 /* */
           "( " FILTER_DATA " OR sd." DUF_SQL_IDFIELD " IS NULL ) " /*                           */ " AND " /* */
           "( sz.size  IS NULL OR sz.size > 0 ) " /*                                             */ " AND " /* */
           "(  :fFast  IS NULL OR sz.size IS NULL OR sz.dupzcnt IS NULL OR sz.dupzcnt > 1 ) " /* */ " AND " /* */
           " 1 "                                                     /* */
           ,
#else
           .afilter_fresh = {FILTER_DATA " OR sd." DUF_SQL_IDFIELD " IS NULL", "sz.size  IS NULL OR sz.size > 0"},
         /* .filter_fresh = "( " FILTER_DATA " OR sd." DUF_SQL_IDFIELD " IS NULL " ")" " AND " "( sz.size  IS NULL OR sz.size > 0 )" (* *) */
         /* ,                                                                                                                              */
         /* .filter_fast = ":fFast  IS NULL OR sz.size IS NULL OR sz.dupzcnt IS NULL OR sz.dupzcnt > 1" (* *) */
         /* ,                                                                                                 */
           .afilter_fast = {"sz.size IS NULL OR sz.dupzcnt IS NULL OR sz.dupzcnt > 1"},
#endif
         /* .count_aggregate = "DISTINCT fd." DUF_SQL_IDFIELD (* *) */
           }
  ,
  .node = {
           .name = "sd-node",
           .type = DUF_NODE_NODE,
           .expand_sql = 1,                                          /* */
           .fieldset =                                               /* */
         /* "'sd5-node' AS fieldset_id, " (* *) */
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
           .matcher = "pt.ParentId=:parentdirID AND ( :dirName IS NULL OR dname=:dirName )" /* */
           ,                                                         /* */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
           .filter = " rnfiles > 0 "                                 /* */
#endif
           },
  .final_sql_seq = &final_sql,
};

/* ########################################################################################## */
SRP( MOD, unsigned long long, sd5id, 0, pdistat2file_sd5id_existed, duf_depthinfo_t * pdi, unsigned long sd5sum1, unsigned long sd5sum2 )
{
  const char *sql = "SELECT " DUF_SQL_IDFIELD " AS sd5id FROM " DUF_SQL_TABLES_SD5_FULL " WHERE sd5sum1=:sd5Sum1 AND sd5sum2=:sd5Sum2"
        /* " INDEXED BY " DUF_SQL_TABLES_SD5 "_uniq WHERE  sd5sum1=:sd5Sum1 AND sd5sum2=:sd5Sum2 */
          ;

  DUF_SQL_SE_START_STMT( pdi, select_sd5, sql, pstmt_local );
  MAST_TRACE( select, 3, "S:%s", sql );
  DUF_SQL_SE_BIND_LL( sd5Sum1, sd5sum1, pstmt_local );
  DUF_SQL_SE_BIND_LL( sd5Sum2, sd5sum2, pstmt_local );
  DUF_SQL_SE_STEP( pstmt_local );
  if ( QISERR1_N( SQL_ROW ) )
  {
    ERRCLEAR1( SQL_ROW );
    MAST_TRACE( select, 10, "<selected>" );
  /* sd5id = duf_sql_column_long_long( pstmt_local, 0 ); */
    sd5id = DUF_GET_QUFIELD3( pstmt_local, sd5id );
  /* rpr = 0; */
  }
  else
  {
    MAST_TRACE( select, 10, "<NOT selected> (%d)", QERRIND );
  }
  DUF_SQL_SE_END_STMT( pdi, select_sd5, pstmt_local );                     /* clears SQL_ROW / SQL_DONE */
  ERP( MOD, unsigned long long, sd5id, 0, pdistat2file_sd5id_existed, duf_depthinfo_t * pdi, unsigned long sd5sum1, unsigned long sd5sum2 );
}

static
SRP( MOD, unsigned long long, sd5id, -1, insert_sd5_uni, duf_depthinfo_t * pdi, unsigned long long *sd64, const char *msg MAS_UNUSED, int need_id )
{
  int changes = 0;

#ifdef MAS_TRACING
  const char *real_path = duf_levinfo_path( pdi );
#endif

  if ( sd64 && sd64[1] && sd64[0] )
  {
    if ( !duf_get_config_flag_disable_insert(  ) )
    {
      static const char *sql = "INSERT OR IGNORE INTO " DUF_SQL_TABLES_SD5_FULL " ( sd5sum1, sd5sum2 ) VALUES ( :sd5sum1, :sd5sum2 )";

      MAST_TRACE( sd5, 0, "%016llx%016llx %s%s", sd64[1], sd64[0], real_path, msg );
      DUF_SQL_SE_START_STMT( pdi, insert_sd5, sql, pstmt_local );
      MAST_TRACE( insert, 0, "S:%s", sql );
      DUF_SQL_SE_BIND_LL( sd5sum1, sd64[1], pstmt_local );
      DUF_SQL_SE_BIND_LL( sd5sum2, sd64[0], pstmt_local );
      DUF_SQL_SE_STEPC( pstmt_local );
      DUF_SQL_SE_CHANGES( changes, pstmt_local );
      DUF_SQL_SE_END_STMT( pdi, insert_sd5, pstmt_local );                 /* clears SQL_ROW / SQL_DONE */
    }
    duf_pdi_reg_changes( pdi, changes );
    if ( ( QISERR1_N( SQL_CONSTRAINT ) || QNOERR ) && !changes )
    {
      if ( need_id )
        sd5id = duf_pdistat2file_sd5id_existed( pdi, sd64[1], sd64[0], QPERRIND );
    }
    else if ( QNOERR /* assume SQLITE_OK */  )
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
    ERRMAKE( DATA );
  }

  ERP( MOD, unsigned long long, sd5id, -1, insert_sd5_uni, duf_depthinfo_t * pdi, unsigned long long *sd64, const char *msg MAS_UNUSED, int need_id );
}

static
SR( MOD, make_sd5_uni, int fd, unsigned long long *pbytes, unsigned char *pmd )
{
/*   DUF_STARTR( r ) */ ;
  size_t bufsz = 256 * 1;
  MD5_CTX ctx;

  memset( &ctx, 0, sizeof( ctx ) );
  MAST_TRACE( sd5, 0, "make" );
  {
    char *buffer;

    buffer = mas_malloc( bufsz );
    if ( buffer )
    {
      if ( !duf_get_config_flag_disable_calculate(  ) && ( MD5_Init( &ctx ) != 1 ) )
        ERRMAKE( MD5 );

      {
        int maxcnt = 2;
        int cnt = 0;

        while ( QNOERR && ( maxcnt == 0 || cnt++ < maxcnt ) )
        {
          int ry;

          MAST_TRACE( sd5, 10, "read fd:%u", fd );
          ry = read( fd, buffer, bufsz );
          MAST_TRACE( sd5, 10, "read ry:%u", ry );
          if ( ry < 0 )
          {
          /* DUF_ERRSYS( "read file" ); */
            MASE_ERRSYS( "read file" );

            ERRMAKE( READ );

            break;
          }
          if ( ry > 0 )
          {
            if ( pbytes )
              ( *pbytes ) += ry;
            if ( !duf_get_config_flag_disable_calculate(  ) )
            {
              if ( MD5_Update( &ctx, buffer, ry ) != 1 )
                ERRMAKE( MD5 );
            }
          }
          if ( ry <= 0 )
            break;

        }
      }
      mas_free( buffer );
    }
    else
    {
      ERRMAKE( MEMORY );
    }
  }
  if ( !duf_get_config_flag_disable_calculate(  ) && MD5_Final( pmd, &ctx ) != 1 )
    ERRMAKE( MD5 );

  ER( MOD, make_sd5_uni, int fd, unsigned long long *pbytes, unsigned char *pmd );
}

/* 20150820.143755 */
static
SR( MOD, sd5_dirent_content2, duf_stmnt_t * pstmt_unused MAS_UNUSED, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
/*   DUF_STARTR( r ) */ ;
  unsigned char amd5r[MD5_DIGEST_LENGTH];
  unsigned char amd5[MD5_DIGEST_LENGTH];
  unsigned long long bytes = 0;

  DUF_RSFIELD2( fname );
  MAST_TRACE( sd5, 0, "+ %s", fname );

  memset( amd5, 0, sizeof( amd5 ) );
  MAST_TRACE( sd5, 0, "+ %s", fname );
  if ( !duf_get_config_flag_disable_calculate(  ) )
    CR( make_sd5_uni, duf_levinfo_dfd( pdi ), &bytes, amd5 );
  MAST_TRACE( sd5, 0, "+ %s", fname );

/* reverse */
  for ( unsigned i = 0; i < sizeof( amd5 ) / sizeof( amd5[0] ); i++ )
    amd5r[i] = amd5[sizeof( amd5 ) / sizeof( amd5[0] ) - i - 1];

  if ( QNOERR )
  {
    unsigned long long sd5id = 0;
    unsigned long long *pmd;

    pmd = ( unsigned long long * ) &amd5r;
    MAST_TRACE( sd5, 0, "insert %s", fname );
    if ( duf_get_config_flag_disable_calculate(  ) )
      pmd[0] = pmd[1] = duf_levinfo_dirid( pdi ) + 74;               /* FIXME What is it? */
    sd5id = duf_insert_sd5_uni( pdi, pmd, fname /* for dbg message only */ , 1 /*need_id */ , QPERRIND );
    if ( sd5id )
    {
      int changes = 0;

      if ( !duf_get_config_flag_disable_update(  ) )
      {
        DUF_RUFIELD2( filedataid );
#if 0
        DOR( r,
             duf_sql( "UPDATE " DUF_SQL_TABLES_FILEDATAS_FULL " SET sd5id='%llu' WHERE " DUF_SQL_IDFIELD "='%lld'", &changes, sd5id, filedataid ) );
#else
        const char *sql = "UPDATE " DUF_SQL_TABLES_FILEDATAS_FULL " SET sd5id=:sd5Id WHERE " DUF_SQL_IDFIELD " =:dataId ";

        DUF_SQL_SE_START_STMT( pdi, update_sd5id, sql, pstmt_local );
        MAST_TRACE( mod, 3, "S:%s", sql );
        DUF_SQL_SE_BIND_LL( sd5Id, sd5id, pstmt_local );
        DUF_SQL_SE_BIND_LL( dataId, filedataid, pstmt_local );
        DUF_SQL_SE_STEPC( pstmt_local );
        DUF_SQL_SE_CHANGES( changes, pstmt_local );
        DUF_SQL_SE_END_STMT( pdi, update_sd5id, pstmt_local );             /* clears SQL_ROW / SQL_DONE */
#endif

      }
      duf_pdi_reg_changes( pdi, changes );
    }
    MAST_TRACE( sd5, 0, "%016llx%016llx : sd5id: %llu", pmd[1], pmd[0], sd5id );
  /* MAST_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan 5    * %016llx%016llx : %llu", duf_pdi_depth( pdi ), pmd[1], pmd[0], sd5id ); */
  }
  pdi->total_bytes += bytes;
  pdi->total_files++;

  ER( MOD, sd5_dirent_content2, duf_stmnt_t * pstmt_unused, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );
}
