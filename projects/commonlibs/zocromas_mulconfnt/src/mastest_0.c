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
#include "parse.h"

#include "mastest.h"

int
test_0( int argc, const char *argv[], int nseries _uUu_, const char *series_suffix _uUu_, int variant _uUu_ )
{
  int ifds _uUu_ = mastest_fds(  );
  int speed = 0;                                                     /* used in argument parsing to set speed */
  int raw = 0;                                                       /* raw mode? */
  long bitwise_a = 0x00000000ffff0000;
  long bitwise_b = 0x00000000ffff0000;
  long bitwise_c = 0x00000000ffff0000;
  long bitwise_d = 0x00000000ffff0000;

  mucs_option_t options[] = {
    {.s = {"bps", 'b', MUCS_RTYP_INT, &speed,.val = 0,.desc = "signaling rate in bits-per-second",.argdesc = "BPS"}}, /* */
    {.s = {"bwi", 'z', MUCS_RTYP_LONG | MUCS_RTYP_BW_NOT, &bitwise_a,.val = 0, "bitwise",.argdesc = "value"}}, /* */
    {.s = {"bwi-", 'y', MUCS_RTYP_LONG | MUCS_RTYP_BW_NOT | MUCS_RTYP_BW_AND, &bitwise_b,.val = 0, "bitwise",.argdesc = "value"}}, /* */
    {.s = {"bwi+", 'Y', MUCS_RTYP_LONG | MUCS_RTYP_BW_OR, &bitwise_c,.val = 0, "bitwise",.argdesc = "value"}}, /* */
    {.s = {"bwi_", 'X', MUCS_RTYP_LONG | MUCS_RTYP_BW_XOR, &bitwise_d,.val = 0, "bitwise",.argdesc = "value"}}, /* */
    {.s = {"crnl", 'c',.restype = 0,.val = 0,.desc = "expand cr characters to cr/lf sequences"}}, /* */
    {.s = {"hwflow", 'h',.restype = 0,.val = 0,.desc = "use hardware (RTS/CTS) flow control"}}, /* */
    {.s = {"noflow", 'n',.restype = 0,.val = 0,.desc = "use no flow control"}}, /* */
    {.s = {"raw", 'r',.restype = 0,.cust_ptr = &raw,.val = 0,.desc = "don't perform any character conversions"}}, /* */
    {.s = {"swflow", 's',.restype = 0,.val = 0,.val = 's',.desc = "use software (XON/XOF) flow control"}}, /* */
    {.s = {.name = NULL,.shortn = 0,.restype = 0,.cust_ptr = NULL,.val = 0,.desc = NULL,.argdesc = NULL}} /* */
  };
  mucs_option_table_list_t test_tablist = {
    .next = NULL,.optcount = ( sizeof( options ) / sizeof( options[0] ) ) - 1,.name = "table-0",.coptions = options, /* */
  };

  mucs_parse( argc, argv, &test_tablist );
/* if ( variant ) */
  {
    INFO( "**** Options  chosen: " );
    if ( raw )
      INFO( "-r - %d", raw );
    if ( bitwise_a )
      INFO( "-z - %lx", bitwise_a );
    if ( bitwise_b )
      INFO( "-y - %lx", bitwise_b );
    if ( bitwise_c )
      INFO( "-Y - %lx", bitwise_c );
    if ( bitwise_d )
      INFO( "-X - %lx", bitwise_d );
    if ( speed )
      INFO( "-b %d ", speed );
  }
  mucs_config_option_tablist_reset( &test_tablist, 1 );
  mucs_clear_global_flags(  );

  masregerr_print_simple_all_default( NULL, NULL, 0 );
  masregerrs_delete_default( NULL );
  EXAM( mastest_fds(  ), ifds, "ifds=%d ? %d" );
  return 0;
}
