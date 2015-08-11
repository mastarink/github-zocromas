#define DUF_SQL_PDI_STMT

#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"

#include "duf_config_ref.h"

#include "duf_levinfo_ref.h"

#include "duf_pdi.h"

#include "duf_sql_defs.h"
#include "duf_sql.h"
#include "duf_sql2.h"


/* ###################################################################### */
#include "duf_filedata.h"
/* ###################################################################### */

unsigned long long
duf_pdistat2file_dataid_existed( duf_depthinfo_t * pdi, int *pr )
{
  int r = 0;
  unsigned long long dataid = 0;
  const char *sql = "SELECT " DUF_SQL_IDNAME " AS dataid FROM " DUF_DBPREF "filedatas INDEXED BY filedatas_uniq WHERE dev=:Dev AND inode=:iNode";

  DEBUG_START(  );

  DUF_SQL_START_STMT( pdi, select_filedata, sql, r, pstmt );
  DUF_TRACE( select, 3, "S:%s", sql );
  DUF_SQL_BIND_LL( Dev, duf_levinfo_stat_dev( pdi ), r, pstmt );
  DUF_SQL_BIND_LL( iNode, duf_levinfo_stat_inode( pdi ), r, pstmt );
  DUF_SQL_STEP( r, pstmt );
  if ( r == DUF_SQL_ROW )
  {
    DUF_TRACE( select, 0, "<selected>" );
    dataid = duf_sql_column_long_long( pstmt, 0 );
    r = 0;
  }
  else
  {
    /* DUF_TEST_R( r ); */
    DUF_TRACE( select, 0, "<NOT selected> (%d)", r );
  }
  /* DUF_TEST_R( r ); */
  DUF_SQL_END_STMT( select_filedata, r, pstmt );

  DEBUG_ENDULL( dataid );
  return dataid;
}

unsigned long long
duf_pdistat2file_dataid( duf_depthinfo_t * pdi, /* const struct stat *pst_file, */ int need_id, int *pr )
{
  int r = 0;
  unsigned long long dataid = 0;
  /* const struct stat *pst_file = NULL; */

  DEBUG_START(  );

  /* pst_file = duf_levinfo_stat( pdi ); */
  /* if ( pst_file ) */
  {
    int r = 0;
    int changes = 0;

    if ( !duf_config->cli.disable.flag.insert )
    {
      const char *sql = "INSERT OR IGNORE INTO " DUF_DBPREF "filedatas  " /* */
            " (dev,   inode,  size,  mode,  nlink,  uid,  gid,  blksize,  blocks,  atim,  atimn,  mtim,  mtimn,  ctim,  ctimn) " /* */
            " VALUES "          /* */
            " (:Dev, :iNode, :Size, :Mode, :nLink, :UID, :GID, :blkSize, :Blocks, " /* */
            "datetime(:aTim, 'unixepoch'), :aTimn, " "datetime(:mTim, 'unixepoch'), :mTimn, " "datetime(:cTim, 'unixepoch'), :cTimn) " /* */ ;

      DUF_SQL_START_STMT( pdi, insert_filedata, sql, r, pstmt );
      DUF_TRACE( insert, 0, "S:%s", sql );
      
      DUF_SQL_BIND_LL( Dev, duf_levinfo_stat_dev( pdi ), r, pstmt );
      DUF_SQL_BIND_LL( iNode, duf_levinfo_stat_inode( pdi ), r, pstmt );
      DUF_SQL_BIND_LL( Mode, duf_levinfo_stat_mode( pdi ), r, pstmt );
      DUF_SQL_BIND_LL( Size, duf_levinfo_stat_size( pdi ), r, pstmt );
      DUF_SQL_BIND_LL( nLink, duf_levinfo_stat_nlink( pdi ), r, pstmt );
      DUF_SQL_BIND_LL( UID, duf_levinfo_stat_uid( pdi ), r, pstmt );
      DUF_SQL_BIND_LL( GID, duf_levinfo_stat_gid( pdi ), r, pstmt );
      DUF_SQL_BIND_LL( blkSize, duf_levinfo_stat_blksize( pdi ), r, pstmt );
      DUF_SQL_BIND_LL( Blocks, duf_levinfo_stat_blocks( pdi ), r, pstmt );
 
      DUF_SQL_BIND_LL( aTim, duf_levinfo_stat_asec( pdi ), r, pstmt );
      DUF_SQL_BIND_LL( mTim, duf_levinfo_stat_msec( pdi ), r, pstmt );
      DUF_SQL_BIND_LL( cTim, duf_levinfo_stat_csec( pdi ), r, pstmt );
     
      DUF_SQL_BIND_LL( aTimn, duf_levinfo_stat_ansec( pdi ), r, pstmt );
      DUF_SQL_BIND_LL( mTimn, duf_levinfo_stat_mnsec( pdi ), r, pstmt );
      DUF_SQL_BIND_LL( cTimn, duf_levinfo_stat_cnsec( pdi ), r, pstmt );

      DUF_SQL_STEP( r, pstmt );
      DUF_SQL_CHANGES( changes, r, pstmt );
      DUF_SQL_END_STMT( insert_filedata, r, pstmt );
    }
    DUF_TRACE( select, 0, "<changes> : %d", changes );
    if ( need_id )
    {
      if ( ( r == DUF_SQL_CONSTRAINT || !r ) && !changes )
      {
        dataid = duf_pdistat2file_dataid_existed( pdi,  pr );
      }
      else if ( !r /* assume SQLITE_OK */  && changes )
      {
        dataid = duf_sql_last_insert_rowid(  );
        if ( !dataid )
        {
          DUF_SHOW_ERROR( "(2) no dataid by parentid=%llu", duf_levinfo_dirid_up( pdi ) );
          if ( r >= 0 )
            DUF_MAKE_ERROR( r, DUF_ERROR_NOT_IN_DB );
        }
        else
        {
        }
        assert( dataid );
        DUF_TRACE( collect, 1, "inserted (SQLITE_OK) dataid=%llu", dataid );
      }
      DUF_TEST_R( r );
    }
  }
  /* else                                   */
  /* {                                      */
  /*   DUF_SHOW_ERROR( "Wrong data" );      */
  /*   DUF_MAKE_ERROR( r, DUF_ERROR_DATA ); */
  /* }                                      */
  if ( pr )
    *pr = r;
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
