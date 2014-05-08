#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"
#include "duf_config.h"
#include "duf_utils.h"
#include "duf_service.h"

#include "duf_pdi.h"


#include "duf_sql_def.h"
#include "duf_sql_field.h"

#include "duf_sql.h"
#include "duf_sql1.h"
#include "duf_sql2.h"

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_filedata.h"
/* ###################################################################### */

/* run --create-tables --zero-filedata   */
/* run --create-tables --update-filedata */

#define DUF_PDI_STMT

unsigned long long
duf_insert_filedata_uni( duf_depthinfo_t * pdi, const struct stat *pst_file, int need_id, int *pr )
{
  int r = 0;
  unsigned long long resd = 0;

#ifdef DUF_PDI_STMT
  static int insert_filedata_index = -1;
#endif
  if ( pst_file )
  {
    int r = 0;
    int changes = 0;

    DEBUG_START(  );
    {
      if ( 1 )
      {
        const char *sql = "INSERT OR IGNORE INTO duf_filedatas  " /* */
              " (dev,   inode,  size,  mode,  nlink,  uid,  gid,  blksize,  blocks,  atim,  atimn,  mtim,  mtimn,  ctim,  ctimn,  md5id) " /* */
              " VALUES "        /* */
              " (:dev, :inode, :size, :mode, :nlink, :uid, :gid, :blksize, :blocks, :atim, :atimn, :mtim, :mtimn, :ctim, :ctimn, :md5id) " /* */
              ;
        duf_sqlite_stmt_t *pstmt = NULL;

#ifdef DUF_PDI_STMT
        if ( insert_filedata_index < 0 )
          insert_filedata_index = duf_pdi_prepare_statement( pdi, sql );

        if ( insert_filedata_index >= 0 )
          pstmt = duf_pdi_statement( pdi, insert_filedata_index );
        if ( !pstmt || insert_filedata_index < 0 )
          r = DUF_ERROR_PDI_SQL;
#else
        if ( r >= 0 )
          r = duf_sql_prepare( sql, &pstmt );
#endif
        if ( r >= 0 )
          r = duf_sql_bindu_long_long( pstmt, ":dev", 1, ( long long ) pst_file->st_dev );
        if ( r >= 0 )
          r = duf_sql_bindu_long_long( pstmt, ":inode", 2, ( long long ) pst_file->st_ino );
        if ( r >= 0 )
          r = duf_sql_bindu_long_long( pstmt, ":size", 3, ( long long ) pst_file->st_size );
        if ( r >= 0 )
          r = duf_sql_bindu_long_long( pstmt, ":mode", 4, ( long long ) pst_file->st_mode );
        if ( r >= 0 )
          r = duf_sql_bindu_long_long( pstmt, ":nlink", 5, ( long long ) pst_file->st_nlink );
        if ( r >= 0 )
          r = duf_sql_bindu_long_long( pstmt, ":uid", 6, ( long long ) pst_file->st_uid );
        if ( r >= 0 )
          r = duf_sql_bindu_long_long( pstmt, ":gid", 7, ( long long ) pst_file->st_gid );
        if ( r >= 0 )
          r = duf_sql_bindu_long_long( pstmt, ":blksize", 8, ( long long ) pst_file->st_blksize );
        if ( r >= 0 )
          r = duf_sql_bindu_long_long( pstmt, ":blocks", 9, ( long long ) pst_file->st_blocks );
        if ( r >= 0 )
          r = duf_sql_bindu_long_long( pstmt, ":atim", 10, ( long long ) pst_file->st_atim.tv_sec );
        if ( r >= 0 )
          r = duf_sql_bindu_long_long( pstmt, ":atimn", 11, ( long long ) pst_file->st_atim.tv_nsec );
        if ( r >= 0 )
          r = duf_sql_bindu_long_long( pstmt, ":mtim", 12, ( long long ) pst_file->st_mtim.tv_sec );
        if ( r >= 0 )
          r = duf_sql_bindu_long_long( pstmt, ":mtimn", 13, ( long long ) pst_file->st_mtim.tv_nsec );
        if ( r >= 0 )
          r = duf_sql_bindu_long_long( pstmt, ":ctim", 14, ( long long ) pst_file->st_ctim.tv_sec );
        if ( r >= 0 )
          r = duf_sql_bindu_long_long( pstmt, ":ctimn", 15, ( long long ) pst_file->st_ctim.tv_nsec );
        if ( r >= 0 )
          r = duf_sql_bindu_long_long( pstmt, ":md5id", 16, ( long long ) 0 );
        if ( r >= 0 )
          r = duf_sql_step( pstmt );
#ifdef DUF_PDI_STMT
        if ( r == DUF_SQL_DONE )
          r = 0;
        duf_sql_reset( pstmt );
        duf_sql_clear_bindings( pstmt );
#else
        {
          int rf = duf_sql_finalize( pstmt );

          pstmt = NULL;

          if ( r >= 0 || r == DUF_SQL_DONE )
            r = rf;
          DUF_TEST_R( r );
        }
#endif
        if ( r >= 0 )
          changes = duf_sql_changes(  );
        /* DUF_ERROR( "changes:%d", changes ); */
      }
    }
    /* DUF_TRACE( current, 0, "<changes> : %d", changes ); */
    if ( ( r == DUF_SQL_CONSTRAINT || !r ) && !changes )
    {
      if ( need_id )
      {
        duf_scan_callbacks_t sccb = {.fieldset = "dataid" };
        r = duf_sql_select( duf_sel_cb_field_by_sccb, &resd, STR_CB_DEF, STR_CB_UDATA_DEF,
                            ( duf_depthinfo_t * ) NULL, &sccb,
                            "SELECT id AS dataid FROM duf_filedatas WHERE dev='%lu' AND inode='%lu'", pst_file->st_dev, pst_file->st_ino );
      }
    }
    else if ( !r /* assume SQLITE_OK */  )
    {
      if ( need_id )
      {
        resd = duf_sql_last_insert_rowid(  );
        DUF_TRACE( collect, 1, "inserted (SQLITE_OK) dataid=%llu", resd );
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
  /* DUF_TRACE( current, 0, "%llu", resd ); */
  assert( resd );
  DEBUG_ENDULL( resd );
  return resd;
}
