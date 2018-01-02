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

#include "mastest.h"

int
test_manual_0( int argc, const char *argv[], int nseries _uUu_, const char *series_suffix _uUu_, int variant _uUu_ )
{
  int ifds _uUu_ = mastest_fds(  );
  long app_flags = 0x00000000ffff0000L;

  mucs_option_static_t soptions[] = {
    {"xor", 'X', .restype=MUCS_RTYP_LONG | MUCS_RTYP_BW_XOR, .p=&app_flags,.def_string_value = "0xfffe0101",.val = 0, "app_flags",.argdesc = "value",.flags = MUCS_FLAG_OPTIONAL_VALUE | MUCS_FLAG_USE_DEF_SVALUE}, /* */
    {.name = NULL,.shortn = 0,.restype = 0,.cust_ptr = NULL,.def_string_value = NULL,.val = 0,.desc = NULL,.argdesc = NULL} /* */
  };

#if 0
  mucs_option_table_list_t test_tablist = {
    .next = NULL,.count = ( sizeof( soptions ) / sizeof( soptions[0] ) ),.name = "table-manual-0",.coptions = soptions, /* */
  };
#else
  mucs_option_table_list_t *test_tablist = mucs_config_soption_tabnode_add( NULL, "table-manual-0", soptions, 0, 0 /* more_flags */  );
#endif
  mucs_source_list_t *plist = mucs_source_list_create(  );

  mucs_source_list_add_source_x( plist, MUCS_SOURCE_ENV, NULL /*name */ , 0, "MAS_TEST_ENV", 0 /* min_pass */ , 0 /* npasses */, ":", "=", NULL );
  mucs_source_list_add_source_x( plist, MUCS_SOURCE_ARGV, NULL /*name */ , argc, argv, 0 /* min_pass */ , 0 /* npasses */, NULL, "=", NULL /* pref_ids */  );

  mucs_source_list_lookup_all( plist, test_tablist, NULL );

  mucs_source_list_delete( plist );
  plist = NULL;
  mucs_config_option_tablist_delete( test_tablist, 1 );
  test_tablist = NULL;
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  INFO( "app_flags (--xor):%lx", app_flags );
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  masregerr_print_simple_all_default( NULL, NULL, 0 );
  masregerrs_delete_default( NULL );
  WARN( "### FDs:%d", mastest_fds(  ) );
/* EXAM( mastest_fds(  ), ifds, "ifds=%d ? %d" ); */
  return 0;
}
