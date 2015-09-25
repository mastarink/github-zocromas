#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_utils_path.h"     /* duf_pathdepth */

#include "duf_option_defs.h"


#include "duf_levinfo_ref.h"
#include "duf_pdi_ref.h"

#include "duf_pdi_credel.h"
/* ###################################################################### */
#include "duf_pdi.h"
#include "duf_pdi_reinit.h"
/* ###################################################################### */



/* 20150904.090827 */
static int
duf_pdi_reinit( duf_depthinfo_t * pdi, const char *real_path, const duf_ufilter_t * pu,
                const duf_sql_set_t * sql_set, int caninsert, int frecursive, int opendir )
{
  DEBUG_STARTR( r );
  int frec = 0;

  assert( pdi );
  assert( !real_path || *real_path == '/' );
  frec = frecursive < 0 ? duf_pdi_recursive( pdi ) : frecursive;
  DOR( r, duf_pdi_shut( pdi ) );
  DUF_TRACE( pdi, 0, "@@@frecursive:%d; duf_pdi_recursive( pdi ):%d; frec:%d; reinit real_path:%s", frecursive, duf_pdi_recursive( pdi ), frec,
             real_path );
  DUF_TRACE( pdi, 0, "@@[%p] sql_beginning_done:%d", pdi, duf_pdi_root( pdi )->sql_beginning_done );
  DOR( r, DUF_WRAPPED( duf_pdi_init ) ( pdi, pu, real_path, sql_set, caninsert, frec, opendir ) );
  /*OR: return duf_pdi_init( pdi, real_path, 0 ); */
  DEBUG_ENDR( r );
}

/* 20150914.111027
 * reinit with no sql_set, no caninsert and recursive:auto,
 * preserving real_path, ufilter and opendir flag
 * */
int
duf_pdi_reinit_min( duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  const char *rpath;

  rpath = duf_levinfo_path( pdi );
  DUF_TRACE( pdi, 0, "@@[%p] sql_beginning_done:%d", pdi, duf_pdi_root( pdi )->sql_beginning_done );
  DOR( r,
       duf_pdi_reinit( pdi, rpath, duf_pdi_pu( pdi ), NULL /* sql_set */ , 0 /* caninsert */ , -1 /* recursive:auto */ , duf_pdi_opendir( pdi ) ) );
  DEBUG_ENDR( r );
}

/* 20150914.110855
 * init pdi, makeing real_path from path, with ufilter from config, 
 * preserving opendir flag
 * */
int
duf_pdi_reinit_anypath( duf_depthinfo_t * pdi, const char *cpath, const duf_sql_set_t * sql_set, int caninsert, int frecursive )
{
  DEBUG_STARTR( r );
  char *real_path = NULL;

  if ( cpath )
  {
    real_path = duf_realpath( cpath, &r );
    {
      DUF_TRACE( pdi, 8, "@@(FREC:%d/%d) cpath:%s; real_path:%s", DUF_UG_FLAG( recursive ), duf_pdi_recursive( pdi ), cpath, real_path );
      assert( pdi->pdi_name );
      DUF_TRACE( pdi, 0, "@@@reinit_a real_path:%s", real_path );
      DUF_TRACE( pdi, 0, "@@[%p] sql_beginning_done:%d", pdi, pdi->sql_beginning_done );
      DOR( r, duf_pdi_reinit( pdi, real_path, DUF_CONFIGG( puz ), sql_set, caninsert, frecursive, duf_pdi_opendir( pdi ) ) );
      DUF_TRACE( pdi, 8, "@@@(FREC:%d/%d) cpath:%s; real_path:%s", DUF_UG_FLAG( recursive ), duf_pdi_recursive( pdi ), cpath, real_path );
    }
    mas_free( real_path );
  }
  DEBUG_ENDR( r );
}

/* 20150904.091517 */
int
duf_pdi_reinit_oldpath( duf_depthinfo_t * pdi, const duf_sql_set_t * sql_set, int frecursive, int opendir )
{
  DEBUG_STARTR( r );
  char *path = NULL;

  {
    const char *cpath = NULL;

    cpath = duf_levinfo_path( pdi );
    DUF_TRACE( pdi, 8, "@@(frecursive:%d/%d) cpath:%s", frecursive, duf_pdi_recursive( pdi ), cpath );
    if ( cpath )
      path = mas_strdup( cpath );
  }
  DUF_TRACE( pdi, 0, "@@[%p] sql_beginning_done:%d", pdi, duf_pdi_root( pdi )->sql_beginning_done );
  DOR( r, duf_pdi_reinit_anypath( pdi, path, sql_set, 0 /* canisert */ , frecursive ) );
  mas_free( path );
  DEBUG_ENDR( r );
}
