#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <popt.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "mulconfnt_defs.h"
#include "mulconfnt_structs.h"

#include "option_tablist_base.h"

#include "source.h"
#include "source_list_base.h"
#include "source_list.h"
#include "parse.h"

int
test_0( int argc, const char *argv[], int nseries _uUu_, const char *series_suffix _uUu_, int do_fprintf _uUu_ )
{
  int speed = 0;                                                     /* used in argument parsing to set speed */
  int raw = 0;                                                       /* raw mode? */
  long bitwise = 0xffff0000;

  config_option_t options[] = {
    {"bps", 'b', MULCONF_RTYP_INT, &speed, 0, "signaling rate in bits-per-second", "BPS"}, /* */
    {"bwi", 'z', MULCONF_RTYP_LONG | MULCONF_RTYP_FLAG_BITWISE_NOT, &bitwise, 0, "bitwise", "value"}, /* */
    {"bwi-", 'z', MULCONF_RTYP_LONG | MULCONF_RTYP_FLAG_BITWISE_NOT | MULCONF_RTYP_FLAG_BITWISE_AND, &bitwise, 0, "bitwise", "value"}, /* */
    {"bwi+", 'Z', MULCONF_RTYP_LONG | MULCONF_RTYP_FLAG_BITWISE_OR, &bitwise, 0, "bitwise", "value"}, /* */
    {"crnl", 'c', 0, 0, 'c', "expand cr characters to cr/lf sequences", NULL}, /* */
    {"hwflow", 'h', 0, 0, 'h', "use hardware (RTS/CTS) flow control", NULL}, /* */
    {"noflow", 'n', 0, 0, 'n', "use no flow control", NULL},         /* */
    {"raw", 'r', 0, &raw, 0, "don't perform any character conversions", NULL}, /* */
    {"swflow", 's', 0, 0, 's', "use software (XON/XOF) flow control", NULL}, /* */
    {.name = NULL,.shortname = 0,.restype = 0,.ptr = NULL,.val = 0,.desc = NULL,.argdesc = NULL} /* */
  };
  config_option_table_list_t test_tablist = {
    .next = NULL,.count = ( sizeof( options ) / sizeof( options[0] ) ),.name = "test-table",.options = options, /* */
  };

  mulconfnt_parse( argc, argv, &test_tablist );
  if ( do_fprintf )
    fprintf( stderr, "**** Options  chosen: " );
  if ( raw )
    if ( do_fprintf )
      fprintf( stderr, "-r - %d", raw );
  if ( bitwise )
    if ( do_fprintf )
      fprintf( stderr, "-z - %lx", bitwise );
  if ( speed )
    if ( do_fprintf )
      fprintf( stderr, "-b %d ", speed );
  mulconfnt_config_option_tablist_close( &test_tablist );
  return 0;
}
