#define DUF_SQL_PDI_STMT

#include <assert.h>
#include <string.h>


#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"


#include "duf_utils_path.h"     /* duf_realpath */

#include "duf_levinfo_ref.h"

#include "duf_pdi.h"

#include "duf_sql.h"
#include "duf_sql2.h"

#include "duf_sccb.h"

#include "duf_sccb_begfin.h"

/* ###################################################################### */
#include "duf_sccb_handle.h"
/* ###################################################################### */
#ifdef MAS_SCCBHANDLE

static unsigned long long
duf_count_total_items( const duf_scan_callbacks_t * sccb, int *pr )
{
  DEBUG_STARTULL( cnt );
  int r = DUF_ERROR_TOTALS;

  if ( sccb && sccb->leaf.selector_total2 )
  {
    char *sqlt;

    r = 0;
    sqlt = mas_strdup( "SELECT " );
    sqlt = mas_strcat_x( sqlt, "COUNT(*) AS nf" );
    sqlt = mas_strcat_x( sqlt, " " );
    sqlt = mas_strcat_x( sqlt, sccb->leaf.selector_total2 );
    {
      const char *csql;

      csql = sqlt;
      DUF_SQL_START_STMT_NOPDI( csql, r, pstmt );
      DUF_SQL_STEP( r, pstmt );
      if ( r == DUF_SQL_ROW )
      {
        cnt = duf_sql_column_long_long( pstmt, 0 );
        r = 0;
      }
      DUF_SQL_END_STMT_NOPDI( r, pstmt );
      DUF_TRACE( explain, 0, "counted %llu SIZED files in db", cnt );
    }
    mas_free( sqlt );
  }
  else
  {
    DUF_TRACE( explain, 0, "didn't count files in db" );
  }
  if ( pr )
    *pr = r;
  DEBUG_ENDULL( cnt );
}

duf_sccb_handle_t *
duf_open_sccb_handle( duf_depthinfo_t * pdi, const duf_scan_callbacks_t * sccb, int targc, char *const *targv, const duf_ufilter_t * pu )
{
  duf_sccb_handle_t *sccbh = NULL;
  int r = 0;

  sccbh = mas_malloc( sizeof( duf_sccb_handle_t ) );
  memset( sccbh, 0, sizeof( duf_sccb_handle_t ) );
  /* sccbh->targc = mas_argv_clone( &sccbh->targv, targc, targv ); */
  sccbh->targc = targc;
  sccbh->targv = targv;
  sccbh->pu = pu;
  sccbh->pdi = pdi;
  sccbh->sccb = sccb;
  {
    int rt = 0;

    sccbh->total_files = duf_count_total_items( sccbh->sccb, &rt ); /* reference */
/* total_files for progress bar only :( */
    DUF_SCCB( DUF_TRACE, action, 0, "total_files: %llu", sccbh->total_files );
    DUF_TRACE( explain, 0, "%llu files registered in db", sccbh->total_files );
  }

/*
TODO scan mode
  1. direct, like now
  2. place ID's to temporary table, then scan in certain order
*/
  if ( sccb && sccb->init_scan )
  {
    DUF_TRACE( explain, 0, "to init scan" );
    DUF_DO_TEST_R( r, sccb->init_scan(  ) );
  }
  else
  {
    DUF_TRACE( explain, 0, "no init scan" );
  }
  return sccbh;
}

int
duf_close_sccb_handle( duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );
  if ( sccbh )
  {
    DOR( r, duf_scan_final_sql( sccbh->sccb /*, changes */  ) );
    mas_free( sccbh );
  }
  DEBUG_ENDR( r );
}
#endif
