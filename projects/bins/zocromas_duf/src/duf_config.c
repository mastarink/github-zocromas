#include <stdarg.h>
#include <string.h>
#include <sys/time.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_utils.h"
#include "duf_service.h"

#include "duf_error_types.h"
#include "duf_debug_defs.h"
#include "duf_trace_defs.h"
#include "duf_trace.h"
#include "duf_errors.h"

#include "duf_dbg.h"
#include "duf_print_defs.h"


#include "duf_pdi.h"
#include "duf_levinfo.h"
#include "duf_levinfo_ref.h"

#include "duf_ufilter.h"


#include "duf_config_ref.h"
/* ###################################################################### */
#include "duf_config.h"
/* ###################################################################### */

duf_config_t *duf_config = NULL;
const duf_config_t *duf_config4trace = NULL;

duf_config_t *
duf_config_create( void )
{
  duf_config_t *cfg = NULL;

  assert( !cfg );
  cfg = mas_malloc( sizeof( duf_config_t ) );
  memset( cfg, 0, sizeof( cfg ) );
  cfg->pu = duf_ufilter_create(  );
  if ( 0 )
  {
    cfg->db.dir = mas_strdup( getenv( "MSH_SHN_PROJECTS_DIR" ) );
    if ( cfg->db.dir )
    {
      cfg->db.dir = mas_strcat_x( cfg->db.dir, "/../duf_db" );
    }
  }
  {
    int rt;
    struct timeval tv;

    rt = gettimeofday( &tv, NULL );
    if ( rt >= 0 )
      cfg->loadtime = ( ( double ) tv.tv_sec ) + ( ( double ) tv.tv_usec ) / 1.0E6;
  }
  cfg->db.main.name = mas_strdup( "duf-main.db" );
  cfg->db.adm.name = mas_strdup( "duf-adm.db" );
  cfg->cli.trace.any = cfg->cli.trace.error = 1;
  /* cfg->cli.trace.fs = 1; */

  {
    /* extern const duf_option_t *duf_longopts; */
    /* extern const int duf_longopts_count; */
    extern const duf_longval_extended_t *lo_extended;
    extern unsigned lo_extended_count;

    {
      size_t tbsize;

      tbsize = lo_extended_count * ( sizeof( duf_longval_extended_t ) + 1 );
      cfg->longopts_table = mas_malloc( tbsize );
      memset( cfg->longopts_table, 0, tbsize );
      for ( int i = 0; i < lo_extended_count; i++ )
      {
        cfg->longopts_table[i].name = lo_extended[i].o.name;
        cfg->longopts_table[i].has_arg = lo_extended[i].o.has_arg;
        cfg->longopts_table[i].val = lo_extended[i].o.val;
      }
    }
    /* assert(cfg->longopts_table); */
    /* {                                                                                                                                          */
    /*   DUF_PRINTF( 0, "%u -- %u", lo_extended_count, duf_longopts_count );                                                                      */
    /*   for ( int i = 0; i < duf_longopts_count && duf_longopts[i].name && cfg->longopts_table[i].name; i++ )                             */
    /*   {                                                                                                                                        */
    /*     if ( 0 != strcmp( duf_longopts[i].name, cfg->longopts_table[i].name )                                                           */
    /*          || duf_longopts[i].has_arg != cfg->longopts_table[i].has_arg || duf_longopts[i].val != cfg->longopts_table[i].val ) */
    /*     {                                                                                                                                      */
    /*       DUF_PRINTF( 0, "%d: %30s :: %30s", i, duf_longopts[i].name, cfg->longopts_table[i].name );                                    */
    /*     }                                                                                                                                      */
    /*   }                                                                                                                                        */
    /* }                                                                                                                                          */
  }
  cfg->pdi = duf_pdi_create(  );
  return cfg;
}

void
duf_config_delete( duf_config_t * cfg )
{
  if ( cfg )
  {
    duf_pdi_close( cfg->pdi );
    mas_free( cfg->pdi );
    cfg->pdi = NULL;

    duf_ufilter_delete( cfg->pu );
    cfg->pu = NULL;

    mas_free( cfg->config_path );
    cfg->config_path = NULL;

    mas_free( cfg->db.dir );
    cfg->db.dir = NULL;

    mas_free( cfg->longopts_table );
    cfg->longopts_table = NULL;

    mas_free( cfg->help_string );
    cfg->help_string = NULL;

    mas_free( cfg->db.main.name );
    cfg->db.main.name = NULL;

    mas_free( cfg->db.main.fpath );
    cfg->db.main.fpath = NULL;

    mas_free( cfg->db.adm.name );
    cfg->db.adm.name = NULL;

    mas_free( cfg->db.adm.fpath );
    cfg->db.adm.fpath = NULL;

    /* mas_free( cfg->group ); */
    /* cfg->group = NULL;      */

    mas_del_argv( cfg->targc, cfg->targv, 0 );
    cfg->targc = 0;
    cfg->targv = NULL;

    mas_free( cfg->cli.shorts );
    cfg->cli.shorts = NULL;

    mas_free( cfg->cli.trace.output.file );
    cfg->cli.trace.output.file = NULL;

    mas_free( cfg->cli.output.file );
    cfg->cli.output.file = NULL;

    mas_free( cfg );
    cfg = NULL;
  }
}

int
duf_config_show( void )
{
  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  if ( duf_config )
  {
    fprintf( stderr, "db.dir: %s\n", duf_config->db.dir );
  }
  for ( int ia = 0; ia < duf_config->targc; ia++ )
  {
    fprintf( stderr, "targv[%d]: %s\n", ia, duf_config->targv[ia] );
  }
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}
