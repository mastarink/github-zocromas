#define R_GOOD(_r) ((_r)>=0)
#include "mulconfnt_defs.h"
#include <string.h>
#include <math.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_argvc_tools.h>
#include <mastar/minierr/minierr.h>

#include "mulconfnt_structs.h"

#include "source_list_base.h"
#include "option_tablist_base.h"

#include "option.h"
#include "source.h"
#include "source_list.h"

#include "option_interface_base.h"

int
mucs_config_option_interface_callback( mucs_option_t * opt, void *userdata _uUu_ )
{
  if ( opt )
  {
    mucs_source_list_t *source_list = mucs_source_list_create(  );

    char *fpath = mucs_source_absfile( opt->d.extra_cb.source, opt->d.string_value );

    char *t = canonicalize_file_name( opt->d.string_value );
    char *t1 = realpath( opt->d.string_value, NULL );

  /* WARN( "oooooooooooooooo:%s = '%s'/'%s'/'%s' :: %s", opt->s.name, opt->d.string_value, t, t1, fpath ); */
    free( t1 );
    free( t );

    mucs_source_list_add_source( source_list, MUCS_SOURCE_FILE, 0, fpath, 0 /* min_pass */ );

    mucs_source_list_lookup_all( source_list, opt->d.extra_cb.tablist, NULL );

    if ( fpath )
      mas_free( fpath );
    mucs_source_list_delete( source_list );
    source_list = NULL;
  }
  return 0;
}

const mucs_option_static_t mucs_interface_options[] = {
  {.name = "mucs-include",.shortn = '\0',.restype = MUCS_RTYP_STRING,.callback = mucs_config_option_interface_callback},
  {.name = NULL,.shortn = 0,.restype = 0,.cust_ptr = NULL,.def_string_value = NULL,.val = 0,.desc = NULL,.argdesc = NULL} /* */
};

void
mucs_config_option_interface_init( mucs_option_interface_t * option_interface )
{
  memset( option_interface, 0, sizeof( mucs_option_interface_t ) );
}

mucs_option_interface_t *
mucs_config_option_interface_create( void )
{
  mucs_option_interface_t *option_interface = mas_calloc( 1, sizeof( mucs_option_interface_t ) );

  mucs_config_option_interface_init( option_interface );
  return option_interface;
}

/* mucs_option_interface_t *                                                                                                                         */
/* mucs_config_option_interface_create_setup_o( const char *name, const mucs_option_t * options, int special_options )                               */
/* {                                                                                                                                                 */
/*   mucs_option_interface_t *interface = mucs_config_option_interface_create(  );                                                                   */
/*                                                                                                                                                   */
/*   interface->source_list = mucs_source_list_create(  );                                                                                           */
/*   if ( special_options )                                                                                                                          */
/*   {                                                                                                                                               */
/*     interface->tablist = mucs_config_soption_tabnode_add( interface->tablist, "interface-table", mucs_interface_options, 0 (* count=<auto> *)  ); */
/*   }                                                                                                                                               */
/*   interface->tablist = mucs_config_option_tabnode_add( interface->tablist, name, options, 0 (* count=<auto> *)  );                                */
/*   return interface;                                                                                                                               */
/* }                                                                                                                                                 */

mucs_option_interface_t *
mucs_config_soption_interface_create_setup( const char *name, const mucs_option_static_t * soptions, int special_options )
{
  mucs_option_interface_t *interface = mucs_config_option_interface_create(  );

  interface->source_list = mucs_source_list_create(  );
  if ( special_options )
  {
    interface->tablist = mucs_config_soption_tabnode_add( interface->tablist, "interface-table", mucs_interface_options, 0 /* count=<auto> */  );
  }
  interface->tablist = mucs_config_soption_tabnode_add( interface->tablist, name, soptions, 0 /* count=<auto> */  );
  return interface;
}

/* mucs_option_interface_t *                                                                                                        */
/* mucs_config_option_interface_tabnode_add( mucs_option_interface_t * interface, const char *name, const mucs_option_t * options ) */
/* {                                                                                                                                */
/*   interface->tablist = mucs_config_option_tabnode_add( interface->tablist, name, options, 0 (* count=<auto> *)  );               */
/*   return interface;                                                                                                              */
/* }                                                                                                                                */

mucs_option_interface_t *
mucs_config_soption_interface_tabnode_add( mucs_option_interface_t * interface, const char *name, const mucs_option_static_t * soptions )
{
  interface->tablist = mucs_config_soption_tabnode_add( interface->tablist, name, soptions, 0 /* count=<auto> */  );
  return interface;
}

void
mucs_config_option_interface_reset( mucs_option_interface_t * interface )
{
  if ( interface )
  {
    mucs_source_list_delete( interface->source_list );
    mucs_config_option_tablist_delete( interface->tablist );
  }
}

void
mucs_config_option_interface_delete( mucs_option_interface_t * interface )
{
  mucs_config_option_interface_reset( interface );
  mas_free( interface );
}
