#define R_GOOD(_r) ((_r)>=0)

#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>
#include <mastar/regerr/masregerr_defs.h>

#include <mastar/mulconfnt/mulconfnt_structs.h>
#include <mastar/mulconfnt/source_defaults.h>
#include <mastar/mulconfnt/mulconfnt_enums.h>
#include <mastar/mulconfnt/option_tablist_base.h>
#include <mastar/mulconfnt/source_list_base.h>
#include <mastar/mulconfnt/source_list.h>
#include <mastar/mulconfnt/source.h>

#include "dufnx.h"

int
dufnx( int argc __attribute__ ( ( unused ) ), char *argv[] __attribute__ ( ( unused ) ) )
{
  INFO( "dufnx" );
  int v_int0 _uUu_ = 99992;

  mucs_option_t options[] = {
    {"num0", 0, MUCS_RTYP_INT, &v_int0}
  };
  mucs_option_table_list_t gen_tablist = {
    .next = NULL,.count = ( sizeof( options ) / sizeof( options[0] ) ),.name = "dufnx-gen-table",.options = options, /* */
  };
  mucs_source_list_t *plist = mucs_source_list_create(  );

  {
    mucs_source_t *osrc = mucs_source_list_add_source( plist, MUCS_SOURCE_ARGV, argc, argv, NULL, "=", NULL );

    mucs_source_lookup_all( osrc, &gen_tablist );
    INFO( "v_int0:%d\n", v_int0 );
  }
  {
    mucs_source_t *osrc = mucs_source_list_add_source( plist, MUCS_SOURCE_LIBCONFIG, 0, NULL, NULL, "=", NULL );

    WARN( "1 data_ptr:%s", ( char * ) osrc->data_ptr );
    mucs_source_lookup_all( osrc, &gen_tablist );
    INFO( "v_int0:%d\n", v_int0 );
  }
  {
    mucs_source_t *osrc = mucs_source_list_add_source( plist, MUCS_SOURCE_CONFIG, 0, MULCONFNT_ETC_CONFIG, NULL, "=", NULL );

    WARN( "1 data_ptr:%s", ( char * ) osrc->data_ptr );
    mucs_source_lookup_all( osrc, &gen_tablist );
    INFO( "v_int0:%d\n", v_int0 );
  }

  mucs_source_list_delete( plist );
  mucs_config_option_tablist_reset( &gen_tablist );

  WARN( "MAS_CONFIG_PREFIX:%s;", MAS_CONFIG_PREFIX );
  WARN( "MAS_CONFIG_SYSCONFDIR:%s;", MAS_CONFIG_SYSCONFDIR );
  WARN( "MAS_SYSCONFDIR:%s;", MAS_SYSCONFDIR );
  WARN( "MAS_DATADIR:%s;", MAS_DATADIR );
  {
    const mucs_source_t *osrc = mucs_source_default( MUCS_SOURCE_LIBCONFIG );

    WARN( "MUCS_SOURCE_LIBCONFIG data_ptr:%s", ( char * ) osrc->data_ptr );
  }

  return 0;
}

int
main( int argc, char *argv[] )
{
  return dufnx( argc, argv );
}

static void constructor_main(  ) __attribute__ ( ( constructor( 2011 ) ) );
static void
constructor_main( void )
{
  masregerrs_set_max_print( 15 );
  INFO( "START" );
}
