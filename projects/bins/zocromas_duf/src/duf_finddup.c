#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

/* #include "duf_path.h" */
#include "duf_sql.h"
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
duf_sql_update_duplicates( duf_record_t * precord, va_list args, void *sel_cb_udata,
                           duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_dirinfo_t * pdi, duf_scan_callbacks_t * cb )
{
  unsigned long long cnt;
  unsigned long long md5id;

  cnt = strtoll( precord->presult[1], NULL, 10 );
  md5id = strtoll( precord->presult[0], NULL, 10 );
  /* fprintf( stderr, "%u. %llu dupcnt:%llu\x1b[K\r", nrow, md5id, cnt ); */
  return duf_sql( "UPDATE duf_md5 SET dupcnt='%llu' WHERE id='%llu'", cnt, md5id );
}

int
duf_update_duplicates( void )
{
  int r;

  fprintf( stderr, "Find duplicates\x1b[K\n" );
  r = duf_sql_select( duf_sql_update_duplicates, SEL_CB_UDATA_DEF, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_dirinfo_t * ) NULL,
                      ( duf_scan_callbacks_t * ) NULL /*  sccb */ ,
                      "SELECT " " duf_md5.id, COUNT(*) as cnt " " FROM duf_md5 "
                      " LEFT JOIN duf_filedatas ON (duf_md5.id=duf_filedatas.md5id) "
                      " LEFT JOIN duf_filenames ON (duf_filedatas.id=duf_filenames.dataid) "
                      " LEFT JOIN duf_paths ON (duf_filenames.pathid=duf_paths.id) " " WHERE " " dupcnt is null "
                      " GROUP BY md5sum1,md5sum2 " " HAVING cnt>1 " " ORDER BY cnt ASC" );
  fprintf( stderr, "End find duplicates\x1b[K\n" );
  return r;
}
