#define R_GOOD(_r) ((_r)>=0)
#include <mastar/minierr/minierr.h>
#include <mastar/regerr/masregerr.h>
#include <mastar/regerr/masregerr_defs.h>

#include <mastar/mulconfnt/mulconfnt_structs.h>

#include <mastar/mulconfnt/option_interface_base.h>
#include <mastar/mulconfnt/option_interface.h>

#include "dufnx.h"

int
dufnx( int argc __attribute__ ( ( unused ) ), char *argv[] __attribute__ ( ( unused ) ) )
{
  long app_flags = 0x00ffff1111111111L;

  INFO( "dufnx" );
  mucs_option_t options[] = {
    {"xor", 'X', MUCS_RTYP_LONG | MUCS_RTYP_BW_XOR, &app_flags,.def_string_value = "0xfffe0101",.val = 0, "app_flags",.argdesc = "value",.flags = MUCS_FLAG_OPTIONAL_VALUE | MUCS_FLAG_USE_DEF_VALUE}, /* */
    {.name = NULL,.shortn = 0,.restype = 0,.argptr = NULL,.def_string_value = NULL,.val = 0,.desc = NULL,.argdesc = NULL} /* */
  };

  mucs_option_interface_t *interface = mucs_config_option_interface_create_setup( "test-table", options, TRUE );

  mucs_option_interface_add_source( interface, MUCS_SOURCE_LIBCONFIG, 0, NULL );
  mucs_option_interface_add_source( interface, MUCS_SOURCE_CONFIG, 0, MULCONFNT_ETC_CONFIG );
  mucs_option_interface_add_source( interface, MUCS_SOURCE_ENV, 0, "MAS_TEST_ENV" );
  mucs_option_interface_add_source( interface, MUCS_SOURCE_STDIN, 0, NULL );
  mucs_option_interface_add_source( interface, MUCS_SOURCE_ARGV, argc, argv );
  mucs_option_interface_lookup_all( interface );

  /*******/

  /*******/

  mucs_config_option_interface_delete( interface );
  interface = NULL;

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
