#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"
#include "duf_config.h"
#include "duf_utils.h"
#include "duf_service.h"


#include "duf_sql_def.h"
#include "duf_sql_field.h"

#include "duf_sql.h"
#include "duf_sql1.h"

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_filedata.h"
/* ###################################################################### */

/* run --create-tables --zero-filedata   */
/* run --create-tables --update-filedata */



unsigned long long
duf_insert_filedata_uni( const struct stat *pst_file, int need_id, int *pr )
{
  int r = 0;
  unsigned long long resd = 0;

  if ( pst_file )
  {
    int r;
    int changes = 0;

    DEBUG_START(  );
    {
      r = duf_sql( "INSERT OR IGNORE INTO duf_filedatas " " (dev,  inode, size, mode, nlink, uid,  gid,  blksize, blocks, "
                   " atim,atimn,mtim, mtimn,ctim,  ctimn,  md5id) " "VALUES "
                   " ('%lu','%lu', '%lu','%lu','%lu', '%lu','%lu','%lu',   '%lu', "
                   "'%lu','%lu','%lu','%lu','%lu', '%lu',  0)", &changes,
                   ( unsigned long ) pst_file->st_dev, ( unsigned long ) pst_file->st_ino, ( unsigned long ) pst_file->st_size,
                   ( unsigned long ) pst_file->st_mode, ( unsigned long ) pst_file->st_nlink, ( unsigned long ) pst_file->st_uid,
                   ( unsigned long ) pst_file->st_gid, ( unsigned long ) pst_file->st_blksize, ( unsigned long ) pst_file->st_blocks,
                   ( unsigned long ) pst_file->st_atim.tv_sec, ( unsigned long ) pst_file->st_atim.tv_nsec,
                   ( unsigned long ) pst_file->st_mtim.tv_sec, ( unsigned long ) pst_file->st_mtim.tv_nsec,
                   ( unsigned long ) pst_file->st_ctim.tv_sec, ( unsigned long ) pst_file->st_ctim.tv_nsec );
    }
    /* DUF_TRACE( current, 0, "<changes> : %d", changes ); */
    if ( r == DUF_SQL_CONSTRAINT && !changes )
    {
      if ( need_id )
      {
        duf_scan_callbacks_t sccb = {.fieldset = "dataid" };
        r = duf_sql_select( duf_sel_cb_field_by_sccb, &resd, STR_CB_DEF, STR_CB_UDATA_DEF,
                            ( duf_depthinfo_t * ) NULL, &sccb,
                            "SELECT id AS dataid " " FROM duf_filedatas " " WHERE dev='%lu' AND inode='%lu'", pst_file->st_dev,
                            pst_file->st_ino );
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
  DEBUG_ENDULL( resd );
  return resd;
}
