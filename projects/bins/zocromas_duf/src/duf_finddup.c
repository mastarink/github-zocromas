#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

/* #include "duf_path.h" */
#include "duf_sql.h"
#include "duf_sql_field.h"
/* #include "duf_remove.h" */


/* ###################################################################### */
#include "duf_finddup.h"
/* ###################################################################### */


int
duf_zero_duplicates( void )
{
  int r;

  fprintf( stderr, "Zero duplicates\x1b[K\n" );
  r = duf_sql_exec( "UPDATE duf_md5 SET dupcnt=null" );
  return r;
}

/* 
 * sql must select pathid, filenameid, filename, md5id, size
 * duf_sql_select_cb_t:
 * */
static int
duf_sel_cb_update_duplicates( duf_record_t * precord, va_list args, void *sel_cb_udata,
                              duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_dirinfo_t * pdi, duf_scan_callbacks_t * cb )
{
  DUF_UFIELD( cnt );
  /* unsigned long long cnt = duf_sql_ull_by_name( "cnt", precord, 0 ); */
  DUF_UFIELD( md5id );
  /* unsigned long long md5id = duf_sql_ull_by_name( "md5id", precord, 0 ); */

  /* fprintf( stderr, "%u. %llu dupcnt:%llu\x1b[K\r", nrow, md5id, cnt ); */
  return duf_sql( "UPDATE duf_md5 SET dupcnt='%llu' WHERE id='%llu'", cnt, md5id );
}

int
duf_update_duplicates( void )
{
  int r;

  fprintf( stderr, "Find duplicates\x1b[K\n" );
  r = duf_sql_select( duf_sel_cb_update_duplicates, SEL_CB_UDATA_DEF, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_dirinfo_t * ) NULL,
                      ( duf_scan_callbacks_t * ) NULL /*  sccb */ ,
                      "SELECT duf_md5.id as md5id, COUNT(*) as cnt " " FROM duf_md5 "
                      " LEFT JOIN duf_filedatas ON (duf_md5.id=duf_filedatas.md5id) "
                      " LEFT JOIN duf_filenames ON (duf_filedatas.id=duf_filenames.dataid) "
                      " LEFT JOIN duf_paths ON (duf_filenames.pathid=duf_paths.id) " " WHERE " " dupcnt is null "
                      " GROUP BY md5sum1,md5sum2 " " HAVING cnt>1 " " ORDER BY cnt ASC" );
  fprintf( stderr, "End find duplicates\x1b[K\n" );
  return r;
}
