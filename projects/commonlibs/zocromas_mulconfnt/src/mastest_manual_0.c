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

#include "option_tablist_base.h"

#include "source.h"
#include "source_list_base.h"
#include "source_list.h"
/* #include "parse.h" */

#include "option_interface_base.h"
#include "option_interface.h"

int
test_manual_0( int argc, const char *argv[], int nseries _uUu_, const char *series_suffix _uUu_, int variant _uUu_ )
{
  long app_flags = 0x00000000ffff0000L;

  mucs_option_t options[] = {
    {.s={"xor", 'X', MUCS_RTYP_LONG | MUCS_RTYP_BW_XOR, &app_flags,.def_string_value = "0xfffe0101",.val = 0, "app_flags",.argdesc = "value",.flags = MUCS_FLAG_OPTIONAL_VALUE | MUCS_FLAG_USE_DEF_SVALUE}}, /* */
    {.s={.name = NULL,.shortn = 0,.restype = 0,.cust_ptr = NULL,.def_string_value = NULL,.val = 0,.desc = NULL,.argdesc = NULL}} /* */
  };

#if 0
  mucs_option_table_list_t test_tablist = {
    .next = NULL,.count = ( sizeof( options ) / sizeof( options[0] ) ),.name = "test-table",.coptions = options, /* */
  };
#else
  mucs_option_table_list_t *test_tablist = mucs_config_option_tabnode_add( NULL, "test-table", options, 0 );
#endif
  mucs_source_list_t *plist = mucs_source_list_create(  );

  mucs_source_list_add_source_x( plist, MUCS_SOURCE_ENV, 0, "MAS_TEST_ENV", ":", "=", NULL );
  mucs_source_list_add_source_x( plist, MUCS_SOURCE_ARGV, argc, argv, NULL, "=", NULL /* pref_ids */  );

  mucs_source_list_lookup_all( plist, test_tablist, NULL );

  mucs_source_list_delete( plist );
  plist = NULL;
  mucs_config_option_tablist_delete( test_tablist );
  test_tablist = NULL;
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  INFO( "app_flags (--xor):%lx", app_flags );
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  masregerr_print_simple_all_default( NULL, NULL, 0 );
  masregerrs_delete_default( NULL );
  return 0;
}
