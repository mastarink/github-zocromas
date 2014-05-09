#define DUF_SQL_PDI_STMT

#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"
#include "duf_config.h"
#include "duf_utils.h"

#include "duf_pdi.h"

#include "duf_sql.h"
#include "duf_sql2.h"

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_filedata.h"
/* ###################################################################### */

/* run --create-tables --zero-filedata   */
/* run --create-tables --update-filedata */


unsigned long long
duf_insert_filedata_uni( duf_depthinfo_t * pdi, const struct stat *pst_file, int need_id, int *pr )
{
  int r = 0;
  unsigned long long dataid = 0;

  if ( pst_file )
  {
    int r = 0;
    int changes = 0;

    DEBUG_START(  );
    if ( !duf_config->cli.disable.insert )
    {
      const char *sql = "INSERT OR IGNORE INTO duf_filedatas  " /* */
            " (dev,   inode,  size,  mode,  nlink,  uid,  gid,  blksize,  blocks,  atim,  atimn,  mtim,  mtimn,  ctim,  ctimn,  md5id) " /* */
            " VALUES "          /* */
            " (:dev, :inode, :size, :mode, :nlink, :uid, :gid, :blksize, :blocks, :atim, :atimn, :mtim, :mtimn, :ctim, :ctimn, :md5id) " /* */
            ;

      DUF_SQL_START_STMT( pdi, insert_filedata, sql, r, pstmt );
      DUF_SQL_BIND_LL( dev, pst_file->st_dev, r, pstmt );
      DUF_SQL_BIND_LL( inode, pst_file->st_ino, r, pstmt );
      DUF_SQL_BIND_LL( size, pst_file->st_size, r, pstmt );
      DUF_SQL_BIND_LL( mode, pst_file->st_mode, r, pstmt );
      DUF_SQL_BIND_LL( nlink, pst_file->st_nlink, r, pstmt );
      DUF_SQL_BIND_LL( uid, pst_file->st_uid, r, pstmt );
      DUF_SQL_BIND_LL( gid, pst_file->st_gid, r, pstmt );
      DUF_SQL_BIND_LL( blksize, pst_file->st_blksize, r, pstmt );
      DUF_SQL_BIND_LL( blocks, pst_file->st_blocks, r, pstmt );
      DUF_SQL_BIND_LL( atim, pst_file->st_atim.tv_sec, r, pstmt );
      DUF_SQL_BIND_LL( atimn, pst_file->st_atim.tv_nsec, r, pstmt );
      DUF_SQL_BIND_LL( mtim, pst_file->st_mtim.tv_sec, r, pstmt );
      DUF_SQL_BIND_LL( mtimn, pst_file->st_mtim.tv_nsec, r, pstmt );
      DUF_SQL_BIND_LL( ctim, pst_file->st_ctim.tv_sec, r, pstmt );
      DUF_SQL_BIND_LL( ctimn, pst_file->st_ctim.tv_nsec, r, pstmt );
      DUF_SQL_BIND_LL( md5id, 0, r, pstmt );
      DUF_SQL_STEP( r, pstmt );
      DUF_SQL_CHANGES( changes, r, pstmt );
      DUF_SQL_END_STMT( r, pstmt );
    }
    DUF_TRACE( current, 0, "<changes> : %d", changes );
    if ( ( r == DUF_SQL_CONSTRAINT || !r ) && !changes )
    {
      if ( need_id )
      {
        const char *sql = "SELECT id AS dataid FROM duf_filedatas INDEXED BY filedatas_uniq WHERE dev=:dev AND inode=:inode";

        DUF_SQL_START_STMT( pdi, select_filedata, sql, r, pstmt );
        DUF_SQL_BIND_LL( dev, pst_file->st_dev, r, pstmt );
        DUF_SQL_BIND_LL( inode, pst_file->st_ino, r, pstmt );
        DUF_SQL_STEP( r, pstmt );
        if ( r == DUF_SQL_ROW )
        {
          DUF_TRACE( current, 0, "<selected>" );
          dataid = duf_sql_column_long_long( pstmt, 0 );
          r = 0;
        }
        else
        {
          DUF_TEST_R( r );
          DUF_TRACE( current, 0, "<NOT selected> (%d)", r );
        }
        DUF_TEST_R( r );
        DUF_SQL_END_STMT( r, pstmt );
      }
    }
    else if ( !r /* assume SQLITE_OK */  )
    {
      if ( need_id )
      {
        dataid = duf_sql_last_insert_rowid(  );
        DUF_TRACE( collect, 1, "inserted (SQLITE_OK) dataid=%llu", dataid );
      }
    }
    else
    {
      DUF_ERROR( "insert filedata %d [%s]", r, duf_error_name( r ) );
    }
  }
  else
  {
    DUF_ERROR( "Wrong data" );
    r = DUF_ERROR_DATA;
  }
  if ( pr )
    *pr = r;
  /* DUF_TRACE( current, 0, "%llu", dataid ); */
  assert( !need_id || dataid );
  DEBUG_ENDULL( dataid );
  return dataid;
}
