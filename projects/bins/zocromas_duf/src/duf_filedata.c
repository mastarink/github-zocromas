#define DUF_SQL_PDI_STMT

#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"

#include "duf_config_ref.h"

#include "duf_levinfo_ref.h"

#include "duf_pdi.h"
#include "duf_pdi_ref.h"
#include "duf_pdi_stmt.h"

#include "duf_sql_defs.h"
#include "duf_sql_stmt_defs.h"
#include "duf_sql_prepared.h"
#include "duf_sql.h"
#include "duf_sql_bind.h"
#include "duf_sql_field.h"
#include "duf_sql_prepared.h"

#include "sql_beginning_tables.h"


/* ###################################################################### */
#include "duf_filedata.h"
/* ###################################################################### */

unsigned long long
duf_pdistat2file_dataid_existed( duf_depthinfo_t * pdi, int *pr )
{
  int rpr = 0;
  unsigned long long dataid = 0;
  const char *sql =
        "SELECT " DUF_SQL_IDNAME " AS dataid FROM " DUF_SQL_TABLES_FILEDATAS_FULL
        " INDEXED BY " DUF_SQL_TABLES_FILEDATAS "_uniq WHERE dev=:Dev AND inode=:iNode";

  DEBUG_START(  );

  DUF_SQL_START_STMT( pdi, select_filedata, sql, rpr, pstmt );
  DUF_TRACE( select, 3, "S:%s", sql );
  DUF_SQL_BIND_LL( Dev, duf_levinfo_stat_dev( pdi ), rpr, pstmt );
  DUF_SQL_BIND_LL( iNode, duf_levinfo_stat_inode( pdi ), rpr, pstmt );
  DUF_SQL_STEP( rpr, pstmt );
  if ( rpr == MAS_SQL_ROW )
  {
    DUF_TRACE( select, 10, "<selected>" );
#if 0
    dataid = duf_sql_column_long_long( pstmt, 0 );
#else
    dataid = DUF_GET_UFIELD2( dataid );
#endif
    rpr = 0;
  }
  else
  {
    /* DUF_TEST_R( rpr ); */
    DUF_TRACE( select, 10, "<NOT selected> (%d)", rpr );
  }
  /* DUF_TEST_R( rpr ); */
  DUF_SQL_END_STMT( select_filedata, rpr, pstmt );
  if ( pr )
    *pr = rpr;
  DEBUG_ENDULL( dataid );
  return dataid;
}

unsigned long long
duf_pdistat2file_dataid( duf_depthinfo_t * pdi, /* const struct stat *pst_file, */ int need_id, int *pr )
{
  int rpr = 0;
  unsigned long long dataid = 0;

  /* const struct stat *pst_file = NULL; */

  DEBUG_START(  );

  /* pst_file = duf_levinfo_stat( pdi ); */
  /* if ( pst_file ) */
  {
    int changes = 0;

    if ( !DUF_CONFIGG( cli.disable.flag.insert ) )
    {
      const char *sql = "INSERT OR IGNORE INTO " DUF_SQL_TABLES_FILEDATAS_FULL /* */
            " (dev,   inode,  size,  mode,  nlink,  uid,  gid,  blksize,  blocks,  atim,  atimn,  mtim,  mtimn,  ctim,  ctimn) " /* */
            " VALUES "          /* */
            " (:Dev, :iNode, :Size, :Mode, :nLink, :UID, :GID, :blkSize, :Blocks, " /* */
            "datetime(:aTim, 'unixepoch'), :aTimn, " "datetime(:mTim, 'unixepoch'), :mTimn, " "datetime(:cTim, 'unixepoch'), :cTimn) " /* */ ;

      DUF_SQL_START_STMT( pdi, insert_filedata, sql, rpr, pstmt );
      DUF_TRACE( insert, 0, "S:%s", sql );

      DUF_SQL_BIND_LL( Dev, duf_levinfo_stat_dev( pdi ), rpr, pstmt );
      DUF_SQL_BIND_LL( iNode, duf_levinfo_stat_inode( pdi ), rpr, pstmt );
      DUF_SQL_BIND_LL( Mode, duf_levinfo_stat_mode( pdi ), rpr, pstmt );
      DUF_SQL_BIND_LL( Size, duf_levinfo_stat_size( pdi ), rpr, pstmt );
      DUF_SQL_BIND_LL( nLink, duf_levinfo_stat_nlink( pdi ), rpr, pstmt );
      DUF_SQL_BIND_LL( UID, duf_levinfo_stat_uid( pdi ), rpr, pstmt );
      DUF_SQL_BIND_LL( GID, duf_levinfo_stat_gid( pdi ), rpr, pstmt );
      DUF_SQL_BIND_LL( blkSize, duf_levinfo_stat_blksize( pdi ), rpr, pstmt );
      DUF_SQL_BIND_LL( Blocks, duf_levinfo_stat_blocks( pdi ), rpr, pstmt );

      DUF_SQL_BIND_LL( aTim, duf_levinfo_stat_asec( pdi ), rpr, pstmt );
      DUF_SQL_BIND_LL( mTim, duf_levinfo_stat_msec( pdi ), rpr, pstmt );
      DUF_SQL_BIND_LL( cTim, duf_levinfo_stat_csec( pdi ), rpr, pstmt );

      DUF_SQL_BIND_LL( aTimn, duf_levinfo_stat_ansec( pdi ), rpr, pstmt );
      DUF_SQL_BIND_LL( mTimn, duf_levinfo_stat_mnsec( pdi ), rpr, pstmt );
      DUF_SQL_BIND_LL( cTimn, duf_levinfo_stat_cnsec( pdi ), rpr, pstmt );

      DUF_SQL_STEP( rpr, pstmt );
      DUF_SQL_CHANGES( changes, rpr, pstmt );
      DUF_SQL_END_STMT( insert_filedata, rpr, pstmt );
    }
    DUF_TRACE( select, 2, "<changes> : %d", changes );
    if ( need_id )
    {
      if ( ( rpr == MAS_SQL_CONSTRAINT || DUF_NOERROR( rpr ) ) && !changes )
      {
        dataid = duf_pdistat2file_dataid_existed( pdi, pr );
      }
      else if ( DUF_NOERROR( rpr ) /* assume SQLITE_OK */  && changes )
      {
        dataid = duf_sql_last_insert_rowid(  );
        if ( !dataid )
        {
          DUF_SHOW_ERROR( "(2) no dataid by parentid=%llu", duf_levinfo_dirid_up( pdi ) );
          if ( DUF_NOERROR( rpr ) )
            DUF_MAKE_ERROR( rpr, DUF_ERROR_NOT_IN_DB );
        }
        else
        {
        }
        assert( dataid );
        DUF_TRACE( collect, 1, "inserted (SQLITE_OK) dataid=%llu", dataid );
      }
      DUF_TEST_R( rpr );
    }
  }
  /* else                                   */
  /* {                                      */
  /*   DUF_SHOW_ERROR( "Wrong data" );      */
  /*   DUF_MAKE_ERROR( rpr, DUF_ERROR_DATA ); */
  /* }                                      */
  if ( pr )
    *pr = rpr;
  /* DUF_TRACE( select, 0, "%llu", dataid ); */
  assert( !need_id || dataid );
  DEBUG_ENDULL( dataid );
  return dataid;
}

int
duf_pdistat2file( duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  ( void ) /* dataid= */ duf_pdistat2file_dataid( pdi, 0 /*need_id */ , &r );
  DEBUG_ENDR( r );
}
