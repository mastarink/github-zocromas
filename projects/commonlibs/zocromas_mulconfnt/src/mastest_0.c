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

#include "mulconfnt_structs.h"

#include "option_tablist_base.h"

#include "source.h"
#include "source_list_base.h"
#include "source_list.h"
#include "parse.h"

int
test_0( int argc, const char *argv[], int nseries _uUu_, const char *series_suffix _uUu_, int variant _uUu_ )
{
  int speed = 0;                                                     /* used in argument parsing to set speed */
  int raw = 0;                                                       /* raw mode? */
  long bitwise = 0xffff0000;

  mucs_option_t options[] = {
    {"bps", 'b', MUCS_RTYP_INT, &speed, 0, "signaling rate in bits-per-second", "BPS"}, /* */
    {"bwi", 'z', MUCS_RTYP_LONG | MUCS_RTYP_BW_NOT, &bitwise, 0, "bitwise", "value"}, /* */
    {"bwi-", 'z', MUCS_RTYP_LONG | MUCS_RTYP_BW_NOT | MUCS_RTYP_BW_AND, &bitwise, 0, "bitwise", "value"}, /* */
    {"bwi+", 'Z', MUCS_RTYP_LONG | MUCS_RTYP_BW_OR, &bitwise, 0, "bitwise", "value"}, /* */
    {"crnl", 'c', 0, 0, 'c', "expand cr characters to cr/lf sequences", NULL}, /* */
    {"hwflow", 'h', 0, 0, 'h', "use hardware (RTS/CTS) flow control", NULL}, /* */
    {"noflow", 'n', 0, 0, 'n', "use no flow control", NULL},         /* */
    {"raw", 'r', 0, &raw, 0, "don't perform any character conversions", NULL}, /* */
    {"swflow", 's', 0, 0, 's', "use software (XON/XOF) flow control", NULL}, /* */
    {.name = NULL,.shortn = 0,.restype = 0,.argptr = NULL,.val = 0,.desc = NULL,.argdesc = NULL} /* */
  };
  mucs_option_table_list_t test_tablist = {
    .next = NULL,.count = ( sizeof( options ) / sizeof( options[0] ) ),.name = "test-table",.options = options, /* */
  };

  mucs_parse( argc, argv, &test_tablist );
  if ( variant )
    fprintf( stderr, "**** Options  chosen: " );
  if ( raw )
    if ( variant )
      fprintf( stderr, "-r - %d", raw );
  if ( bitwise )
    if ( variant )
      fprintf( stderr, "-z - %lx", bitwise );
  if ( speed )
    if ( variant )
      fprintf( stderr, "-b %d ", speed );
  mucs_config_option_tablist_reset( &test_tablist );
  
  masregerr_print_simple_all_default( NULL, NULL, 0 );
  masregerrs_delete_default( NULL );
  return 0;
}
