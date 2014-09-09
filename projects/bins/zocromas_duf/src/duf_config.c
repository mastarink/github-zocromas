/* File 20140902.123910 */
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



#include "duf_config_ref.h"
/* ###################################################################### */
#include "duf_config.h"
/* ###################################################################### */

duf_config_t *duf_config = NULL;

duf_tmp_t *
duf_tmp_create( void )
{
  duf_tmp_t *tmp = NULL;

  tmp = mas_malloc( sizeof( duf_tmp_t ) );
  memset( tmp, 0, sizeof( duf_tmp_t ) );
  return tmp;
}

void
duf_tmp_delete( duf_tmp_t * tmp )
{
  for ( int i = 0; i < DUF_TMP_PATH_MAX; i++ )
  {
    mas_free( tmp->path[i] );
    tmp->path[i] = NULL;
  }
  for ( int i = 0; i < DUF_TMP_EXPLANATION_MAX; i++ )
  {
    mas_free( tmp->option_explanation[i] );
    tmp->option_explanation[i] = NULL;
  }
  mas_free( tmp );
}

int
duf_config_create( void )
{
  DEBUG_STARTR( r );

  assert( !duf_config );
  duf_config = mas_malloc( sizeof( duf_config_t ) );
  memset( duf_config, 0, sizeof( duf_config ) );
  duf_config->u.max_rel_depth = 100;
  if ( 0 )
  {
    duf_config->db.dir = mas_strdup( getenv( "MSH_SHN_PROJECTS_DIR" ) );
    if ( duf_config->db.dir )
    {
      duf_config->db.dir = mas_strcat_x( duf_config->db.dir, "/../duf_db" );
    }
  }
  {
    int rt;
    struct timeval tv;

    rt = gettimeofday( &tv, NULL );
    if ( rt >= 0 )
      duf_config->loadtime = ( ( double ) tv.tv_sec ) + ( ( double ) tv.tv_usec ) / 1.0E6;
  }
  duf_config->db.main.name = mas_strdup( "duf-main.db" );
  duf_config->db.adm.name = mas_strdup( "duf-adm.db" );
  duf_config->cli.trace.any = duf_config->cli.trace.error = 1;
  /* duf_config->cli.trace.fs = 1; */
  duf_config->tmp = duf_tmp_create(  );

  {
    /* extern const duf_option_t *duf_longopts; */
    /* extern const int duf_longopts_count; */
    extern const duf_longval_extended_t *lo_extended;
    extern unsigned lo_extended_count;

    {
      size_t tbsize;

      tbsize = lo_extended_count * ( sizeof( duf_longval_extended_t ) + 1 );
      duf_config->longopts_table = mas_malloc( tbsize );
      memset( duf_config->longopts_table, 0, tbsize );
      for ( int i = 0; i < lo_extended_count; i++ )
      {
        duf_config->longopts_table[i].name = lo_extended[i].o.name;
        duf_config->longopts_table[i].has_arg = lo_extended[i].o.has_arg;
        duf_config->longopts_table[i].val = lo_extended[i].o.val;
      }
    }
    /* {                                                                                                                                          */
    /*   DUF_PRINTF( 0, "%u -- %u", lo_extended_count, duf_longopts_count );                                                                      */
    /*   for ( int i = 0; i < duf_longopts_count && duf_longopts[i].name && duf_config->longopts_table[i].name; i++ )                             */
    /*   {                                                                                                                                        */
    /*     if ( 0 != strcmp( duf_longopts[i].name, duf_config->longopts_table[i].name )                                                           */
    /*          || duf_longopts[i].has_arg != duf_config->longopts_table[i].has_arg || duf_longopts[i].val != duf_config->longopts_table[i].val ) */
    /*     {                                                                                                                                      */
    /*       DUF_PRINTF( 0, "%d: %30s :: %30s", i, duf_longopts[i].name, duf_config->longopts_table[i].name );                                    */
    /*     }                                                                                                                                      */
    /*   }                                                                                                                                        */
    /* }                                                                                                                                          */
  }
  duf_config->pdi = mas_malloc( sizeof( duf_depthinfo_t ) );
  memset( duf_config->pdi, 0, sizeof( duf_depthinfo_t ) );
  PF( "CONFIG pdi:%p", duf_config->pdi );
  DEBUG_ENDR( r );
}

int
duf_config_delete( void )
{
  duf_pdi_close( duf_config->pdi );
  mas_free( duf_config->pdi );
  duf_config->pdi = NULL;

  duf_tmp_delete( duf_config->tmp );
  duf_config->tmp = NULL;

  mas_free( duf_config->config_path );
  duf_config->config_path = NULL;

  mas_free( duf_config->db.dir );
  duf_config->db.dir = NULL;

  mas_free( duf_config->longopts_table );
  duf_config->longopts_table = NULL;

  mas_free( duf_config->help_string );
  duf_config->help_string = NULL;
  mas_free( duf_config->db.main.name );
  duf_config->db.main.name = NULL;
  mas_free( duf_config->db.main.fpath );
  duf_config->db.main.fpath = NULL;

  mas_free( duf_config->db.adm.name );
  duf_config->db.adm.name = NULL;

  mas_free( duf_config->db.adm.fpath );
  duf_config->db.adm.fpath = NULL;

  mas_free( duf_config->u.glob );
  duf_config->u.glob = NULL;

  mas_free( duf_config->u.same_as );
  duf_config->u.same_as = NULL;

  /* mas_free( duf_config->group ); */
  /* duf_config->group = NULL;      */

  mas_del_argv( duf_config->u.globx.include_files.argc, duf_config->u.globx.include_files.argv, 0 );
  duf_config->u.globx.include_files.argc = 0;
  duf_config->u.globx.include_files.argv = NULL;

  mas_del_argv( duf_config->u.globx.exclude_files.argc, duf_config->u.globx.exclude_files.argv, 0 );
  duf_config->u.globx.exclude_files.argc = 0;
  duf_config->u.globx.exclude_files.argv = NULL;

  mas_del_argv( duf_config->targc, duf_config->targv, 0 );
  duf_config->targc = 0;
  duf_config->targv = NULL;

  mas_free( duf_config->cli.shorts );
  duf_config->cli.shorts = NULL;
  mas_free( duf_config->cli.trace.output.file );
  duf_config->cli.trace.output.file = NULL;
  mas_free( duf_config->cli.output.file );
  duf_config->cli.output.file = NULL;

  mas_free( duf_config );
  duf_config = NULL;
  return 0;
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
