#define R_GOOD(_r) ((_r)>=0)
#include "mulconfnt_defs.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <popt.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>
#include <mastar/exam/masexam.h>
#include <mastar/minierr/minierr.h>

#include "mulconfnt_structs.h"

#include "global.h"

#include "option_tablist_base.h"

#include "source.h"
#include "source_list_base.h"
#include "source_list.h"
/* #include "parse.h" */

#include "option_interface_base.h"
#include "option_interface.h"

int
test_manual( int argc, const char *argv[], int nseries _uUu_, const char *series_suffix _uUu_, int variant _uUu_ )
{
  long app_flags = 0x00ffff1111111111L;
  unsigned long app_uflags = 0x0L;

  mucs_option_static_t soptions[] = {
    {"xor", 'X', MUCS_RTYP_LONG | MUCS_RTYP_BW_XOR, &app_flags,.def_string_value = "0xfffe0101",.val = 0, "app_flags",.argdesc = "value",.flags = MUCS_FLAG_OPTIONAL_VALUE | MUCS_FLAG_USE_DEF_SVALUE}, /* */
    {"or1", 'O', MUCS_RTYP_ULONG | MUCS_RTYP_BW_OR, &app_uflags,.def_nvalue.v_ulong = 0x101,.val = 0, "app_uflags",.argdesc = "value",.flags = MUCS_FLAG_NO_VALUE | MUCS_FLAG_USE_DEF_NVALUE}, /* */
    {"or2", 'o', MUCS_RTYP_ULONG | MUCS_RTYP_BW_OR, &app_uflags,.def_nvalue.v_ulong = 0x404,.val = 0, "app_uflags",.argdesc = "value",.flags = MUCS_FLAG_NO_VALUE | MUCS_FLAG_USE_DEF_NVALUE}, /* */
    {.name = NULL,.shortn = 0,.restype = 0,.cust_ptr = NULL,.def_string_value = NULL,.val = 0,.desc = NULL,.argdesc = NULL} /* */
  };
  mucs_set_global_flags( MUCS_FLAG_CONTINUE_ON_UNRECOGNIZED /* | MUCS_FLAG_USE_TTY */  );
  mucs_option_interface_t *interface = mucs_config_soption_interface_create_setup( "table-manual", soptions, TRUE );

  mucs_option_interface_add_source( interface, MUCS_SOURCE_LIBCONFIG, 0, NULL, 0 );
  mucs_option_interface_add_source( interface, MUCS_SOURCE_CONFIG, 0, MULCONFNT_ETC_CONFIG, 0 );
  mucs_option_interface_add_source( interface, MUCS_SOURCE_ENV, 0, "MAS_TEST_ENV", 0 );
  mucs_option_interface_add_source( interface, MUCS_SOURCE_STDIN, 0, NULL, 0 );
  mucs_option_interface_add_source( interface, MUCS_SOURCE_ARGV, argc, argv, 0 );
  mucs_option_interface_lookup_all( interface, NULL );
  INFO( "app_flags (--or):%lx", app_uflags );
  INFO( "app_flags (--xor):%lx", app_flags );
  app_flags = 0x00ffff1111111111L;
  app_uflags = 0x0L;

  mucs_option_interface_lookup_all( interface, NULL );

  mucs_config_option_interface_delete( interface );
  interface = NULL;
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  INFO( "app_flags (--or):%lx", app_uflags );
  INFO( "app_flags (--xor):%lx", app_flags );
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  masregerr_print_simple_all_default( NULL, NULL, 0 );
  masregerrs_delete_default( NULL );
  return 0;
}
