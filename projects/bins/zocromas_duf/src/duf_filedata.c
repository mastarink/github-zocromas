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
duf_stat2file_dataid_existed( duf_depthinfo_t * pdi, const struct stat *pst_file, int *pr )
{
  int r = 0;
  unsigned long long dataid = 0;

  DEBUG_START(  );
  const char *sql = "SELECT " DUF_SQL_IDNAME " AS dataid FROM " DUF_DBPREF "filedatas INDEXED BY filedatas_uniq WHERE dev=:Dev AND inode=:iNode";

  DUF_SQL_START_STMT( pdi, select_filedata, sql, r, pstmt );
  DUF_TRACE( select, 3, "S:%s", sql );
  DUF_SQL_BIND_LL( Dev, pst_file->st_dev, r, pstmt );
  DUF_SQL_BIND_LL( iNode, pst_file->st_ino, r, pstmt );
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
duf_stat2file_dataid( duf_depthinfo_t * pdi, const struct stat *pst_file, int need_id, int *pr )
{
  int r = 0;
  unsigned long long dataid = 0;

  DEBUG_START(  );

  if ( pst_file )
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
      DUF_SQL_BIND_LL( Dev, pst_file->st_dev, r, pstmt );
      DUF_SQL_BIND_LL( iNode, pst_file->st_ino, r, pstmt );
      DUF_SQL_BIND_LL( Size, pst_file->st_size, r, pstmt );
      DUF_SQL_BIND_LL( Mode, pst_file->st_mode, r, pstmt );
      DUF_SQL_BIND_LL( nLink, pst_file->st_nlink, r, pstmt );
      DUF_SQL_BIND_LL( UID, pst_file->st_uid, r, pstmt );
      DUF_SQL_BIND_LL( GID, pst_file->st_gid, r, pstmt );
      DUF_SQL_BIND_LL( blkSize, pst_file->st_blksize, r, pstmt );
      DUF_SQL_BIND_LL( Blocks, pst_file->st_blocks, r, pstmt );
      DUF_SQL_BIND_LL( aTim, pst_file->st_atim.tv_sec, r, pstmt );
      DUF_SQL_BIND_LL( aTimn, pst_file->st_atim.tv_nsec, r, pstmt );
      DUF_SQL_BIND_LL( mTim, pst_file->st_mtim.tv_sec, r, pstmt );
      DUF_SQL_BIND_LL( mTimn, pst_file->st_mtim.tv_nsec, r, pstmt );
      DUF_SQL_BIND_LL( cTim, pst_file->st_ctim.tv_sec, r, pstmt );
      DUF_SQL_BIND_LL( cTimn, pst_file->st_ctim.tv_nsec, r, pstmt );
      DUF_SQL_STEP( r, pstmt );
      DUF_SQL_CHANGES( changes, r, pstmt );
      DUF_SQL_END_STMT( insert_filedata, r, pstmt );
    }
    DUF_TRACE( select, 0, "<changes> : %d", changes );
    if ( need_id )
    {
      if ( ( r == DUF_SQL_CONSTRAINT || !r ) && !changes )
      {
        dataid = duf_stat2file_dataid_existed( pdi, pst_file, pr );
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
  else
  {
    DUF_SHOW_ERROR( "Wrong data" );
    DUF_MAKE_ERROR( r, DUF_ERROR_DATA );
  }
  if ( pr )
    *pr = r;
  /* DUF_TRACE( select, 0, "%llu", dataid ); */
  assert( !need_id || dataid );
  DEBUG_ENDULL( dataid );
  return dataid;
}
