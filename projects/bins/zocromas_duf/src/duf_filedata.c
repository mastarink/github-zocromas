/* #undef MAS_TRACING */
#include <assert.h>                                                  /* assert */
#include <stddef.h>                                                  /* NULL */

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

/* #include "duf_tracen_defs.h"                                         (* T; TT; TR ✗ *) */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ✗ *) */

/* #include "duf_start_end.h"                                           (* DUF_STARTR ; DUF_ENDR ✗ *) */
/* #include "duf_dodefs.h"                                              (* DOR ✗ *) */

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */
/* #include "duf_config_ref.h" */
/* #include "duf_config_defs.h"                                         (* DUF_CONF... ✗ *) */

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */

#include "duf_pdi_ref.h"
#include "duf_pdi_stmt.h"                                            /* duf_pdi_find_statement_by_id; etc. ✗ */

#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ✗ */
/* #include "duf_sql_stmt_defs.h"                                       (* DUF_SQL_BIND_S_OPT etc. ✗ *) */
#include "duf_sql_se_stmt_defs.h"                                    /* DUF_SQL_SE_BIND_S_OPT etc. ✗ */
#include "duf_sql_prepared.h"                                        /* duf_sql_(prepare|step|finalize) ✗ */
#include "duf_sql_bind.h"                                            /* duf_sql_... for DUF_SQL_BIND_... etc. ✗ */
#include "duf_sql_field.h"                                           /* __duf_sql_str_by_name2 for DUF_GET_UFIELD2 etc. ✗ */
#include "duf_sql_prepared.h"                                        /* duf_sql_(prepare|step|finalize) ✗ */

#include "sql_beginning_tables.h"                                    /* DUF_SQL_TABLES... etc. ✗ */

/* ###################################################################### */
#include "duf_filedata.h"
/* ###################################################################### */

SRP( PDI, unsigned long long, dataid, 0, pdistat2file_dataid_existed, duf_depthinfo_t * pdi )
{
/* int rpr = 0; */
/* unsigned long long dataid = 0; */
  const char *sql =
          "SELECT " DUF_SQL_IDFIELD " AS dataid FROM " DUF_SQL_TABLES_FILEDATAS_FULL
          " INDEXED BY " DUF_SQL_TABLES_FILEDATAS "_uniq WHERE dev=:Dev AND inode=:iNode";

/* DUF_START(  ); */

  DUF_SQL_SE_START_STMT( pdi, select_filedata, sql, pstmt );
  MAST_TRACE( select, 3, "S:%s", sql );
  DUF_SQL_SE_BIND_LL( Dev, ( unsigned long long ) duf_levinfo_stat_dev( pdi ), pstmt );
  DUF_SQL_SE_BIND_LL( iNode, ( unsigned long long ) duf_levinfo_stat_inode( pdi ), pstmt );
  DUF_SQL_SE_STEP( pstmt );
  if ( QISERR1_N( SQL_ROW ) )
  {
    MAST_TRACE( select, 10, "<selected>" );
#if 0
    dataid = duf_sql_column_long_long( pstmt, 0 );
#else
    dataid = DUF_GET_UFIELD2( dataid );
#endif
    assert( dataid > 0 );
  /* rpr = 0; */
  }
  else
  {
  /* DUF_TEST_R( rpr ); */
    MAST_TRACE( select, 0, "@<NOT selected> (%d)", QERRIND );
    assert( 0 );
  }
/* DUF_TEST_R( rpr ); */
  DUF_SQL_SE_END_STMT( pdi, select_filedata, pstmt );
/* if ( pr ) */
/* *pr = rpr; */
  assert( dataid > 0 );
/* DUF_ENDULL( dataid ); */
/* return dataid; */
  ERP( PDI, unsigned long long, dataid, 0, pdistat2file_dataid_existed, duf_depthinfo_t * pdi );
}

SRP( PDI, unsigned long long, dataid, 0, pdistat2file_dataid, duf_depthinfo_t * pdi, int need_id )
{
/* int rpr = 0; */
/* unsigned long long dataid = 0; */

/* DUF_START(  ); */

  {
    int changes = 0;

    if ( !duf_get_config_flag_disable_insert() )
    {
      const char *sql = "INSERT OR IGNORE INTO " DUF_SQL_TABLES_FILEDATAS_FULL /* */
              " (dev,   inode,  size,  mode,  nlink,  uid,  gid,  blksize,  blocks,  atim,  atimn,  mtim,  mtimn,  ctim,  ctimn) " /* */
              " VALUES "                                             /* */
              " (:Dev, :iNode, :Size, :Mode, :nLink, :UID, :GID, :blkSize, :Blocks, " /* */
              "datetime(:aTim, 'unixepoch'), :aTimn, " "datetime(:mTim, 'unixepoch'), :mTimn, " "datetime(:cTim, 'unixepoch'), :cTimn) " /* */ ;

      DUF_SQL_SE_START_STMT( pdi, insert_filedata, sql, pstmt );
      MAST_TRACE( insert, 0, "S:%s", sql );

      DUF_SQL_SE_BIND_LL( Dev, ( unsigned long long ) duf_levinfo_stat_dev( pdi ), pstmt );
      DUF_SQL_SE_BIND_LL( iNode, ( unsigned long long ) duf_levinfo_stat_inode( pdi ), pstmt );
      DUF_SQL_SE_BIND_LL( Mode, ( unsigned long long ) duf_levinfo_stat_mode( pdi ), pstmt );
      DUF_SQL_SE_BIND_LL( Size, ( unsigned long long ) duf_levinfo_stat_size( pdi ), pstmt );
      DUF_SQL_SE_BIND_LL( nLink, ( unsigned long long ) duf_levinfo_stat_nlink( pdi ), pstmt );
      DUF_SQL_SE_BIND_LL( UID, ( unsigned long long ) duf_levinfo_stat_uid( pdi ), pstmt );
      DUF_SQL_SE_BIND_LL( GID, ( unsigned long long ) duf_levinfo_stat_gid( pdi ), pstmt );
      DUF_SQL_SE_BIND_LL( blkSize, ( unsigned long long ) duf_levinfo_stat_blksize( pdi ), pstmt );
      DUF_SQL_SE_BIND_LL( Blocks, ( unsigned long long ) duf_levinfo_stat_blocks( pdi ), pstmt );

      DUF_SQL_SE_BIND_LL( aTim, ( unsigned long long ) duf_levinfo_stat_asec( pdi ), pstmt );
      DUF_SQL_SE_BIND_LL( mTim, ( unsigned long long ) duf_levinfo_stat_msec( pdi ), pstmt );
      DUF_SQL_SE_BIND_LL( cTim, ( unsigned long long ) duf_levinfo_stat_csec( pdi ), pstmt );

      DUF_SQL_SE_BIND_LL( aTimn, ( unsigned long long ) duf_levinfo_stat_ansec( pdi ), pstmt );
      DUF_SQL_SE_BIND_LL( mTimn, ( unsigned long long ) duf_levinfo_stat_mnsec( pdi ), pstmt );
      DUF_SQL_SE_BIND_LL( cTimn, ( unsigned long long ) duf_levinfo_stat_cnsec( pdi ), pstmt );

      DUF_SQL_SE_STEPC( pstmt );
      DUF_SQL_SE_CHANGES( changes, pstmt );
      DUF_SQL_SE_END_STMT( pdi, insert_filedata, pstmt );
    }
    MAST_TRACE( select, 2, "<changes> : %d", changes );
    if ( need_id )
    {
      if ( ( QISERR1_N( SQL_CONSTRAINT ) || QNOERR ) && !changes )
      {
        dataid = duf_pdistat2file_dataid_existed( pdi, QPERRIND );
      }
      else if ( QNOERR /* assume SQLITE_OK */  && changes )
      {
        dataid = duf_sql_last_insert_rowid(  );
        if ( !dataid )
        {
          if ( QNOERR )
            ERRMAKE_M( NOT_IN_DB, "(2) no dataid by parentid=%llu", duf_levinfo_dirid_up( pdi ) );
        }
        else
        {
        }
        assert( dataid );
        MAST_TRACE( collect, 1, "inserted (SQLITE_OK) dataid=%llu", dataid );
      }
    /* DUF_TEST_R( rpr ); */
    }
  }
/* else                                   */
/* {                                      */
/*   DUF_SHOW_ERROR( "Wrong data" );      */
/*   ERRMAKE(DATA ); */
/* }                                      */
/* if ( pr ) */
/* *pr = rpr; */
  assert( !need_id || dataid );
/* DUF_ENDULL( dataid ); */
/* return dataid; */
  ERP( PDI, unsigned long long, dataid, 0, pdistat2file_dataid, duf_depthinfo_t * pdi, int need_id );
}

SR( PDI, pdistat2file, duf_depthinfo_t * pdi )
{
/* DUF_STARTR( r ); */

  ( void ) /* dataid= */ duf_pdistat2file_dataid( pdi, 0 /*need_id */ , QPERRIND );
/* DUF_ENDR( r ); */
  ER( PDI, pdistat2file, duf_depthinfo_t * pdi );
}
