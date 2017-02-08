#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <popt.h>

#include <mastar/wrap/mas_memory.h>

#include "mulconfnt_structs.h"

#include "parse.h"

static void constructor_main( int argc, char **argv, char **envp ) __attribute__ ( ( constructor( 2001 ) ) );
static void
constructor_main( int argc __attribute__ ( ( unused ) ), char **argv __attribute__ ( ( unused ) ), char **envp __attribute__ ( ( unused ) ) )
{
/* configure my zocromas_mas_wrap library (malloc/free wrapper) not to print memory usage map; may be enabled later */
#ifdef MAS_TRACEMEM
  {
    extern int mas_mem_disable_print_usage __attribute__ ( ( weak ) );

    if ( &mas_mem_disable_print_usage )
    {
      mas_mem_disable_print_usage = 0;
    }
  }

#endif
}

void
usage( poptContext optCon, int exitcode, char *error, char *addl )
{
  poptPrintUsage( optCon, stderr, 0 );
  if ( error )
    fprintf( stderr, "%s: %s0", error, addl );
  exit( exitcode );
}

/* getopt_long */
int
main( int argc, const char *argv[] )
{
//mas_strdup( "abrakadabra" );
  fprintf( stderr, "START\n" );
  {
    int speed = 0;                                                   /* used in argument parsing to set speed */
    int raw = 0;                                                     /* raw mode? */
    long bitwise = 0xffff0000;

    config_option_t options[] = {
      {"bps", 'b', MULCONF_RESTYPE_INT, &speed, 0, "signaling rate in bits-per-second", "BPS"}, /* */
      {"bwi-", 'z', MULCONF_RESTYPE_LONG | MULCONF_BITWISE_NOT | MULCONF_BITWISE_AND, &bitwise, 0, "bitwise", "value"}, /* */
      {"bwi+", 'z', MULCONF_RESTYPE_LONG | MULCONF_BITWISE_OR, &bitwise, 0, "bitwise", "value"}, /* */
      {"crnl", 'c', 0, 0, 'c', "expand cr characters to cr/lf sequences", NULL}, /* */
      {"hwflow", 'h', 0, 0, 'h', "use hardware (RTS/CTS) flow control", NULL}, /* */
      {"noflow", 'n', 0, 0, 'n', "use no flow control", NULL},       /* */
      {"raw", 'r', 0, &raw, 0, "don't perform any character conversions", NULL}, /* */
      {"swflow", 's', 0, 0, 's', "use software (XON/XOF) flow control", NULL}, /* */
      {.name = NULL,.shortname = 0,.restype = 0,.ptr = NULL,.val = 0,.desc = NULL,.argdesc = NULL} /* */
    };
    config_option_table_list_t test_tablist = {
      .next = NULL,.count = ( sizeof( options ) / sizeof( options[0] ) ),.name = "test-table",.options = options, /* */
    };

    mulconfnt_parse( argc, argv, &test_tablist );
    fprintf( stderr, "**** Options  chosen: " );
    if ( raw )
      fprintf( stderr, "-r - %d", raw );
    if ( bitwise )
      fprintf( stderr, "-z - %lx", bitwise );
    if ( speed )
      fprintf( stderr, "-b %d ", speed );
  }
  fprintf( stderr, "\n%0x - %0x - %0x - %0x\n", MULCONF_BITWISE_AND, MULCONF_BITWISE_OR, MULCONF_BITWISE_XOR, MULCONF_BITWISE_NOT );
  fprintf( stderr, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n" );
  if ( 1 )
  {
    char c __attribute__ ( ( unused ) );                             /* used for argument parsing */
    int i __attribute__ ( ( unused ) ) = 0;                          /* used for tracking options */
    const char *portname;
    int speed = 0;                                                   /* used in argument parsing to set speed */
    int raw = 0;                                                     /* raw mode? */
    long bitwise = 0;
    int j __attribute__ ( ( unused ) );
    char buf[BUFSIZ + 1] __attribute__ ( ( unused ) );
    poptContext optCon __attribute__ ( ( unused ) );                 /* context for parsing command-line options */

    struct poptOption optionsTable[] = {
      {"bps", 'b', POPT_ARG_INT, &speed, 0,
       "signaling rate in bits-per-second", "BPS"},
      {"bwi", 'z', POPT_ARG_INT | POPT_ARGFLAG_OR, &bitwise, 0, "bitwise", "value"}, /* */
      {"crnl", 'c', 0, 0, 'c',
       "expand cr characters to cr/lf sequences", NULL},
      {"hwflow", 'h', 0, 0, 'h',
       "use hardware (RTS/CTS) flow control", NULL},
      {"noflow", 'n', 0, 0, 'n',
       "use no flow control", NULL},
      {"raw", 'r', 0, &raw, 0,
       "don't perform any character conversions", NULL},
      {"swflow", 's', 0, 0, 's',
       "use software (XON/XOF) flow control", NULL},
      POPT_AUTOHELP {NULL, 0, 0, NULL, 0}
    };

    optCon = poptGetContext( NULL, argc, argv, optionsTable, 0 );
    poptSetOtherOptionHelp( optCon, "[OPTIONS]* <port>" );

    if ( argc < 2 )
    {
      poptPrintUsage( optCon, stderr, 0 );
      exit( 1 );
    }

  /* Now do options processing, get portname */
    while ( ( c = poptGetNextOpt( optCon ) ) >= 0 )
    {
      switch ( c )
      {
      case 'c':
        buf[i++] = 'c';
        break;
      case 'h':
        buf[i++] = 'h';
        break;
      case 's':
        buf[i++] = 's';
        break;
      case 'n':
        buf[i++] = 'n';
        break;
      }
    }
    portname = poptGetArg( optCon );
    if ( ( portname == NULL ) || !( poptPeekArg( optCon ) == NULL ) )
      usage( optCon, 1, "Specify a single port", ".e.g., /dev/cua0" );

    if ( c < -1 )
    {
    /* an error occurred during option processing */
      fprintf( stderr, "%s: %s\n", poptBadOption( optCon, POPT_BADOPTION_NOALIAS ), poptStrerror( c ) );
      return 1;
    }

  /* Print out options, portname chosen */
    fprintf( stderr, "Options  chosen: " );
    for ( j = 0; j < i; j++ )
      fprintf( stderr, "-%c ", buf[j] );
    if ( raw )
      fprintf( stderr, "-r - %d", raw );
    if ( bitwise )
      fprintf( stderr, "-z - %lx", bitwise );
    if ( speed )
      fprintf( stderr, "-b %d ", speed );
    fprintf( stderr, "\nPortname chosen: %s\n", portname );

    poptFreeContext( optCon );
  }
  return 0;
}
