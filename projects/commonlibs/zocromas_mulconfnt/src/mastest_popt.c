#include "mulconfnt_defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <popt.h>


void
usage( poptContext optCon, int _uUu_ exitcode, char *error, char *addl )
{
  poptPrintUsage( optCon, stderr, 0 );
  if ( error )
    fprintf( stderr, "%s: %s0", error, addl );
//exit( exitcode );
}

int
test_popt( int argc, const char *argv[], int nseries _uUu_, const char *series_suffix _uUu_ , int do_fprintf _uUu_)
{
  char c _uUu_;                                                      /* used for argument parsing */
  int i _uUu_ = 0;                                                   /* used for tracking options */
  const char *portname;
  int speed = 0;                                                     /* used in argument parsing to set speed */
  int raw = 0;                                                       /* raw mode? */
  long bitwise = 0;
  int j _uUu_;
  char buf[BUFSIZ + 1] _uUu_;
  poptContext optCon _uUu_;                                          /* context for parsing command-line options */

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
//  exit( 1 );
  }
  else
  {
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
    {
      usage( optCon, 1, "Specify a single port", ".e.g., /dev/cua0" );
    }
    else
    {
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
    }
  }
  poptFreeContext( optCon );
  return 0;
}
