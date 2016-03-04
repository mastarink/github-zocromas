/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ♠ */
#include "duf_errorn_defs_preset.h"

#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */
#include <mastar/tools/mas_utils_path.h>                             /* mas_normalize_path; mas_pathdepth; mas_realpath etc. ♣ */

/* #include "duf_tracen_defs.h"                                         (* MAST_TRACE ♠ *) */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ *) */

/* #include "duf_start_end.h"                                           (* DUF_STARTR ; DUF_ENDR ♠ *) */
/* #include "duf_dodefs.h"                                              (* DOR ♠ *) */

# include "duf_se_only.h"

/* #include "duf_debug_defs.h"                                          (* DUF_WRAPSTATIC; DUF_WRAPPED ...  ♠ *) */

#include "duf_config.h"                                              /* duf_get_config ♠ */
#include "duf_optimpl_defs.h"                                        /* DUF_UG_FLAG; DUF_ACT_FLAG etc. ♠ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ♠ */
#include "duf_config_defs.h"                                         /* DUF_CONF... ♠ */

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ♠ */

#include "duf_pdi.h"                                                 /* duf_pdi_init; duf_pdi_shut; duf_pdi_close ♠ */
#include "duf_pdi_filters.h"                                         /* duf_pdi_pu; etc. ♠ */
#include "duf_pdi_ref.h"
/* #include "duf_pdi_credel.h" */

/* ###################################################################### */
#include "duf_pdi_reinit.h"
/* ###################################################################### */

/* 20150904.090827 */
static
SR( PDI, pdi_reinit, duf_depthinfo_t * pdi, const char *real_path, const duf_ufilter_t * pu,
    const duf_sql_set_t * sql_set, int caninsert, int frecursive, int fallow_dirs, int flinear, int opendir )
{
/* DUF_STARTR( r ); */
  int frec = 0;
  int fwn = 0;

  assert( pdi );
  assert( !real_path || *real_path == '/' );
  frec = frecursive < 0 ? duf_pdi_recursive( pdi ) : frecursive;
  fwn = fallow_dirs < 0 ? duf_pdi_allow_dirs( pdi ) : fallow_dirs;
  flinear = flinear < 0 ? duf_pdi_linear( pdi ) : flinear;
  CR( pdi_shut, pdi );
  MAST_TRACE( pdi, 0, "@@@frecursive:%d; duf_pdi_recursive( pdi ):%d; frec:%d; reinit real_path:%s", frecursive, duf_pdi_recursive( pdi ), frec,
              real_path );
  MAST_TRACE( pdi, 0, "@@[%p] sql_beginning_done:%d", pdi, duf_pdi_root( pdi )->sql_beginning_done );
  MAST_TRACE( pdi, 0, "@@@frecursive:%d/%d; real_path:%s", frecursive, frec, real_path );
/* T( "%p real_path:%p:%s", pdi->pathinfo.levinfo ? pdi->pathinfo.levinfo->fullpath : 0, real_path, real_path ); */
  CR( pdi_init, pdi, pu, real_path, sql_set, caninsert, frec, fwn, flinear, opendir );
/*OR: return duf_pdi_init( pdi, real_path, 0 ); */
/* DUF_ENDR( r ); */

  ER( PDI, pdi_reinit, duf_depthinfo_t * pdi, const char *real_path, const duf_ufilter_t * pu,
      const duf_sql_set_t * sql_set, int caninsert, int frecursive, int fallow_dirs, int flinear, int opendir );
}

/* 20150914.111027
 * reinit with no sql_set, no caninsert and recursive:auto,
 * preserving real_path, ufilter and opendir flag
 * */
SR( PDI, pdi_reinit_min, duf_depthinfo_t * pdi )
{
/* DUF_STARTR( r ); */
  char *rpath;

  rpath = mas_strdup( duf_levinfo_path( pdi ) );
  MAST_TRACE( pdi, 0, "@@[%p] sql_beginning_done:%d", pdi, duf_pdi_root( pdi )->sql_beginning_done );
/* T( "%p rpath:%p:%s", pdi->pathinfo.levinfo ? pdi->pathinfo.levinfo->fullpath : 0, rpath, rpath ); */
  CR( pdi_reinit, pdi, rpath, duf_pdi_pu( pdi ), ( const duf_sql_set_t * ) NULL /* sql_set */ , 0 /* caninsert */ , -1 /* recursive:auto */ ,
      -1 /* allow_dirs:auto */ , -1 /* linear:auto */ , duf_pdi_opendir( pdi ) );
  mas_free( rpath );
/* DUF_ENDR( r ); */
  ER( PDI, pdi_reinit_min, duf_depthinfo_t * pdi );
}

/* 20150914.110855
 * init pdi, makeing real_path from path, with ufilter from config, 
 * preserving opendir flag
 * */
SR( PDI, pdi_reinit_anypath, duf_depthinfo_t * pdi, const char *cpath, const duf_ufilter_t * pu, const duf_sql_set_t * sql_set,
    int caninsert, int frecursive, int fallow_dirs, int flinear )
{
/* DUF_STARTR( r ); */
  char *real_path = NULL;

  if ( cpath )
  {
    real_path = mas_realpath( cpath /*, &r */  );
    if ( !real_path )
      ERRMAKE( PATH );
    {
    /* FIXME : does'nt copy pu - so mod_dialog not work ???  */
      MAST_TRACE( pdi, 0, "@@@reinit_a real_path:%s : %llu", real_path, pdi->pup ? pdi->pup->md5id.min : 0 );
      MAST_TRACE( pdi, 8, "@@(FREC:%d/%d) cpath:%s; real_path:%s", DUF_UG_FLAG( recursive ), duf_pdi_recursive( pdi ), cpath, real_path );
      assert( pdi->pdi_name );
      MAST_TRACE( pdi, 0, "@@[%p] sql_beginning_done:%d", pdi, pdi->sql_beginning_done );

      CR( pdi_reinit, pdi, real_path, pu ? pu : DUF_CONFIGG( vars.puz ), sql_set, caninsert, frecursive, fallow_dirs, flinear,
          duf_pdi_opendir( pdi ) );
      MAST_TRACE( pdi, 8, "@@@(FREC:%d/%d) cpath:%s; real_path:%s", DUF_UG_FLAG( recursive ), duf_pdi_recursive( pdi ), cpath, real_path );
      MAST_TRACE( pdi, 0, "@@@reinit_a real_path:%s : %llu", real_path, pdi->pup ? pdi->pup->md5id.min : 0 );
    }
    mas_free( real_path );
  }
/* DUF_ENDR( r ); */
  ER( PDI, pdi_reinit_anypath, duf_depthinfo_t * pdi, const char *cpath, const duf_ufilter_t * pu, const duf_sql_set_t * sql_set,
      int caninsert, int frecursive, int fallow_dirs, int flinear );
}

SR( PDI, pdi_reinit_defflags_anypath, duf_depthinfo_t * pdi, const char *cpath, const duf_ufilter_t * pu, const duf_sql_set_t * sql_set )
{
  CR( pdi_reinit_anypath, pdi, cpath, pu, sql_set, 1 /* caninsert */ , duf_pdi_recursive( pdi ),
      duf_pdi_allow_dirs( pdi ), duf_pdi_linear( pdi ) );
  ER( PDI, pdi_reinit_defflags_anypath, duf_depthinfo_t * pdi, const char *cpath, const duf_ufilter_t * pu, const duf_sql_set_t * sql_set );
}
