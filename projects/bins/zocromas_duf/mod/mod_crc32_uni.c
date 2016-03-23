/* #undef MAS_TRACING */
#include <assert.h>                                                  /* assert */
#include <stddef.h>                                                  /* NULL */
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <zlib.h>

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

/* #include "duf_sccb_types.h"                                          (* duf_scan_callbacks_t ✗ *) */
#include "duf_sccb_structs.h"

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */
/* #include "duf_config_ref.h" */
/* #include "duf_config_defs.h"                                         (* DUF_CONF... ✗ *) */

#include "duf_pdi_ref.h"
#include "duf_pdi_stmt.h"                                            /* duf_pdi_find_statement_by_id; etc. ✗ */

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */

/* #include "duf_sql_stmt_defs.h"                                       (* DUF_SQL_BIND_S_OPT etc. ✗ *) */
#include "duf_sql_se_stmt_defs.h"                                    /* DUF_SQL_SE_BIND_S_OPT etc. ✗ */

#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ✗ */
#include "duf_sql_field.h"                                           /* __duf_sql_str_by_name2 for DUF_GET_UFIELD2 etc. ✗ */

#include "duf_sql_bind.h"                                            /* duf_sql_... for DUF_SQL_BIND_... etc. ✗ */
#include "duf_sql_prepared.h"                                        /* duf_sql_(prepare|step|finalize) ✗ */

/* #include "duf_dbg.h" */

#include "sql_beginning_tables.h"                                    /* DUF_SQL_TABLES... etc. ✗ */

#include "duf_pdi_structs.h"
/* #include "duf_levinfo_structs.h" */
/* ########################################################################################## */
static int duf_crc32_dirent_content2( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED );

/* ########################################################################################## */
#define FILTER_DATA  "fd.crc32id IS NULL"

static duf_sql_sequence_t final_sql =                                /* */
{
  .name = "final-crc32",
  .done = 0,
  .sql = {
          "UPDATE " DUF_SQL_TABLES_CRC32_FULL " SET dup32cnt=(SELECT COUNT(*) " /* */
          " FROM " DUF_SQL_TABLES_CRC32_FULL " AS c32 "              /* */
          " JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fd.crc32id=c32." DUF_SQL_IDFIELD ") " /* */
          " WHERE " DUF_SQL_TABLES_CRC32_FULL "." DUF_SQL_IDFIELD "=c32." DUF_SQL_IDFIELD ")" /* */
        /* " WHERE " DUF_SQL_TABLES_CRC32_FULL ".crc32sum=c32.crc32sum )" (* *) */
          ,
          NULL,
          }
};

/* ########################################################################################## */
duf_scan_callbacks_t duf_crc32_callbacks = {
  .title = "collect crc32",
  .name = "crc32",
  .init_scan = NULL,
  .def_opendir = 1,

  .leaf_scan_fd2 = duf_crc32_dirent_content2,

/* TODO : explain values of use_std_leaf_set_num and use_std_node_set_num TODO */
  .use_std_leaf_set_num = -1,                                        /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node_set_num = 2,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
/* .std_leaf_set_name = "std-leaf-no-sel", */
  .std_leaf_set_name = "std-leaf-no-sel-fd",
  .std_node_set_name = "std-node-two",
#define DUF_FILTER
  .leaf = {                                                          /* */
           .name = "crc32-leaf",
           .type = DUF_NODE_LEAF,
           .fieldset =                                               /* */
           "#crc32",
           .fieldsets = {
                         "#basic",
                         "#plus",
                         "#crc32x",
                         NULL}
           ,
           .selector2 = "#std-ns-fd-leaf",
           .matcher = " fn.Pathid=:parentdirID "                     /* */
           ,                                                         /* */
#if 0
           .filter =                                                 /* */
           "( " FILTER_DATA " OR crc." DUF_SQL_IDFIELD " IS NULL ) " /*                          */ " AND " /* */
           "( sz.size    IS NULL OR sz.size > 0 ) " /*                                             */ " AND " /* */
           "(  :fFast    IS NULL OR sz.size IS NULL OR sz.dupzcnt IS NULL OR sz.dupzcnt > 1 ) " /* */ " AND " /* */
           " 1 "                                                     /* */
           ,                                                         /* */
#else
           .afilter = {"sz.size > 0"},
           .afilter_fresh = {FILTER_DATA " OR crc." DUF_SQL_IDFIELD " IS NULL", "sz.size  IS NULL OR sz.size > 0"},
           .afilter_fast = {"sz.size IS NULL OR sz.dupzcnt IS NULL OR sz.dupzcnt > 1"},
#endif
         /*, .group=" fd." DUF_SQL_IDFIELD */
         /* .count_aggregate = "DISTINCT fd." DUF_SQL_IDFIELD (* *) */
           }
  ,
  .node = {
           .name = "crc32-node",
           .type = DUF_NODE_NODE,
           .expand_sql = 1,                                          /* */
           .fieldset =                                               /* */
         /* "'crc32-node' AS fieldset_id, " (* *) */
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
           ", (" DUF_SQL__RNUMFILES( pt ) " WHERE " FILTER_DATA " ) AS rnfiles " /* */
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
SRP( MOD, unsigned long long, crc32id, 0, pdistat2file_crc32id_existed, duf_depthinfo_t * pdi, unsigned long crc32sum )
{
/* int rpr = 0; */
/* unsigned long long crc32id = 0; */
  const char *sql = "SELECT " DUF_SQL_IDFIELD " AS crc32id FROM " DUF_SQL_TABLES_CRC32_FULL " WHERE crc32sum=:Crc32sum"
        /* " INDEXED BY " DUF_SQL_TABLES_CRC32 "_uniq WHERE  crc32sum=:Crc32sum" */
          ;

/* DUF_START(  ); */

  DUF_SQL_SE_START_STMT( pdi, select_crc32, sql, pstmt );
  MAST_TRACE( select, 3, "S:%s", sql );
  DUF_SQL_SE_BIND_LL( Crc32sum, crc32sum, pstmt );
  DUF_SQL_SE_STEP( pstmt );
  if ( QISERR1_N( SQL_ROW ) )
  {
    MAST_TRACE( select, 10, "<selected>" );
  /* crc32id = duf_sql_column_long_long( pstmt, 0 ); */
    crc32id = DUF_GET_UFIELD2( crc32id );
  /* rpr = 0; */
  }
  else
  {
  /* DUF_TEST_R( rpr ); */
    MAST_TRACE( select, 10, "<NOT selected> (%d)", QERRIND );
  }
/* DUF_TEST_R( rpr ); */
  DUF_SQL_SE_END_STMT( pdi, select_crc32, pstmt );
/* if ( pr ) */
/* *pr = rpr; */
/* DUF_ENDULL( crc32id ); */
  ERP( MOD, unsigned long long, crc32id, 0, pdistat2file_crc32id_existed, duf_depthinfo_t * pdi, unsigned long crc32sum );
}

static
SRP( MOD, unsigned long long, crc32id, -1, insert_crc32_uni, duf_depthinfo_t * pdi, unsigned long long crc32sum, const char *filename MAS_UNUSED,
     int need_id )
{
/* unsigned long long crc32id = -1; */
/* int lr = 0; */
  int changes = 0;

#ifdef MAS_TRACING
  const char *real_path = duf_levinfo_path( pdi );
#endif

/* DUF_START(  ); */
  if ( crc32sum )
  {
    static unsigned long insert_cnt = 0;

    if ( !duf_get_config_flag_disable_insert(  ) )
    {
      static const char *sql = "INSERT OR IGNORE INTO " DUF_SQL_TABLES_CRC32_FULL " (crc32sum) VALUES (:crc32sum)";

      MAST_TRACE( crc32, 10, "%0llx %s%s", crc32sum, real_path, filename );
      DUF_SQL_SE_START_STMT( pdi, insert_crc32, sql, pstmt );
      MAST_TRACE( insert, 0, "(%ld) S:%s", insert_cnt, sql );
      DUF_SQL_SE_BIND_LL( crc32sum, crc32sum, pstmt );
      DUF_SQL_SE_STEPC( pstmt );
      DUF_SQL_SE_CHANGES( changes, pstmt );
      DUF_SQL_SE_END_STMT( pdi, insert_crc32, pstmt );
      insert_cnt++;
    }
    duf_pdi_reg_changes( pdi, changes );
    if ( ( QISERR1_N( SQL_CONSTRAINT ) || QNOERR ) && !changes )
    {
      if ( need_id )
        crc32id = duf_pdistat2file_crc32id_existed( pdi, crc32sum, QPERRIND );
    }
    else if ( QNOERR /* assume SQLITE_OK */  )
    {
      if ( need_id && changes )
      {
        crc32id = duf_sql_last_insert_rowid(  );
      }
    }
  /* else                                       */
  /* {                                          */
  /*   DUF_SHOW_ERROR( "insert crc32 %d", lr ); */
  /* }                                          */
  }
  else
  {
  /* DUF_SHOW_ERROR( "Wrong data" ); */
    ERRMAKE_M( DATA, "no crc32 sum" );
  /* DUF_TEST_R( lr ); */
  }

/* if ( pr ) */
/* *pr = lr; */

/* DUF_ENDULL( crc32id ); */
/* return crc32id; */
  ERP( MOD, unsigned long long, crc32id, -1, insert_crc32_uni, duf_depthinfo_t * pdi, unsigned long long crc32sum, const char *filename MAS_UNUSED,
       int need_id );
}

static
SR( MOD, make_crc32_uni, int fd, unsigned long long *pbytes, unsigned long long *pcrc32sum )
{
/*   DUF_STARTR( r ) */ ;
  size_t bufsz = 512 * 8;
  unsigned long crc32sum = 0;
  unsigned char *buffer;

  if ( !duf_get_config_flag_disable_calculate(  ) )
    crc32sum = crc32( 0L, Z_NULL, 0 );
/* if ( !duf_get_config_flag_disable_calculate()) */
  {
    size_t bytes = 0;

    buffer = mas_malloc( bufsz );
    if ( buffer )
    {
      int cnt = 0;
      int maxcnt = 1;

    /* lseek( fd, -bufsz * maxcnt, SEEK_END ); */
      while ( QNOERR && cnt++ < maxcnt )
      {
        int ry;

        ry = read( fd, buffer, bufsz );
        MAST_TRACE( crc32, 10, "read %d : crc32sum:%lx", ry, crc32sum );
        if ( ry < 0 )
        {
        /* DUF_ERRSYS( "read file" ); */
          MASE_ERRSYS( "read file" );
          ERRMAKE( READ );
        }
/* DUF_TEST_R( r ); */
        if ( ry > 0 )
        {
          if ( pbytes )
            ( *pbytes ) += ry;
          if ( !duf_get_config_flag_disable_calculate(  ) )
          {
            crc32sum = crc32( crc32sum, buffer, ry );
            bytes += ry;
          }
        }
        MAST_TRACE( crc32, 10, "ry:%d; r:%d; crc32sum:%lx", ry, QERRIND, crc32sum );
        if ( ry <= 0 )
          break;

      }
      MAST_TRACE( crc32, 10, "last crc32sum:%lx", crc32sum );
      mas_free( buffer );
    }
    else
    {
      ERRMAKE( MEMORY );
    }
  /* QT( "@bytes:%lu", bytes ); */
  }
  if ( pcrc32sum )
    *pcrc32sum = crc32sum;
/*  DUF_ENDR( r );*/
  ER( MOD, make_crc32_uni, int fd, unsigned long long *pbytes, unsigned long long *pcrc32sum );
}

static
SR( MOD, crc32_dirent_content2, duf_stmnt_t * pstmt, duf_depthinfo_t * pdi, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
/*   DUF_STARTR( r ) */ ;
  unsigned long long crc32sum = 0;
  static unsigned long content_cnt = 0;
  unsigned long long bytes = 0;

  DUF_SFIELD2( fname );
  MAST_TRACE( crc32, 0, "+ %s", fname );

/* assert( duf_levinfo_dbstat( pdi ) ); */
  assert( duf_levinfo_dfd( pdi ) );
  assert( duf_levinfo_stat( pdi ) );

  if ( duf_get_config_flag_disable_calculate(  ) )
    crc32sum = ( unsigned long long ) duf_levinfo_dirid( pdi );
  else
    CR( make_crc32_uni, duf_levinfo_dfd( pdi ), &bytes, &crc32sum );

  content_cnt++;
  if ( QNOERR )
  {
    unsigned long long crc32id = 0;

    MAST_TRACE( crc32, 10, "insert %s", fname );

    crc32id = duf_insert_crc32_uni( pdi, crc32sum, fname /* for dbg message only */ , 1 /*need_id */ , QPERRIND );
    if ( QNOERR && crc32id )
    {
      int changes = 0;

      pdi->cnts.dirent_content2++;
      if ( QNOERR && !duf_get_config_flag_disable_update(  ) )
      {
        DUF_UFIELD2( filedataid );
#if 0
        DOR( r,
             duf_sql( "UPDATE " DUF_SQL_TABLES_FILEDATAS_FULL " SET crc32id='%llu' WHERE " DUF_SQL_IDFIELD "='%lld'", &changes, crc32id,
                      filedataid ) );
#else
        const char *sql = "UPDATE " DUF_SQL_TABLES_FILEDATAS_FULL " SET crc32id=:crc32Id WHERE " DUF_SQL_IDFIELD " =:dataId ";

        DUF_SQL_SE_START_STMT( pdi, update_crc32id, sql, pstmt );
        MAST_TRACE( mod, 3, "S:%s", sql );
        DUF_SQL_SE_BIND_LL( crc32Id, crc32id, pstmt );
        DUF_SQL_SE_BIND_LL( dataId, filedataid, pstmt );
        DUF_SQL_SE_STEPC( pstmt );
        DUF_SQL_SE_CHANGES( changes, pstmt );
        DUF_SQL_SE_END_STMT( pdi, update_crc32id, pstmt );
#endif
      }
      duf_pdi_reg_changes( pdi, changes );
/* DUF_TEST_R( r ); */
    }
    MAST_TRACE( crc32, 0, "(%lu) %04llx : crc32id: %llu (sz:%lu) \"%s\"", content_cnt, crc32sum, crc32id, duf_levinfo_stat( pdi )->st_size, fname );
  /* MAST_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan 5    * %04lx : %llu", duf_pdi_depth( pdi ), crc32sum, crc32id ); */
  }
  pdi->total_bytes += bytes;
  pdi->total_files++;
/*  DUF_ENDR( r );*/
  ER( MOD, crc32_dirent_content2, duf_stmnt_t * pstmt, duf_depthinfo_t * pdi , duf_sccb_handle_t *sccbh MAS_UNUSED);
}
