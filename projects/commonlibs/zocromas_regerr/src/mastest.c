#define R_GOOD(_r) (_r>=0)
#include "masregerr_defs.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <limits.h>
#include <stdlib.h>

#include <mastar/exam/masexam.h>

#include "masregerr.h"

int
function_one( void )
{
  RGE;
  return 0;
}

int
function_two( void )
{
  RGE;
  return 0;
}

int
function_three( void )
{
  RGE;
  return 0;
}

int
function_four( void )
{
  int r = 0;

  RGESR( r = open( "something_not_exist.txt", O_RDONLY ) );
  if ( r >= 0 )
    close( r );
  return r;
}

int
main( int argc __attribute__ ( ( unused ) ), char *argv[] __attribute__ ( ( unused ) ) )
{
  int counter = 35;
  int nfunctions = 4;

  for ( int i = 0; i < counter; i++ )
  {
    int nf = 0;

    function_one(  );
    nf++;
    EXAM( masregerrs_count_all( NULL ), ( size_t ) ( 1 + i * nfunctions ), "%ld:%ld" );
    function_two(  );
    nf++;
    EXAM( masregerrs_count_all( NULL ), ( size_t ) ( 2 + i * nfunctions ), "%ld:%ld" );
    function_three(  );
    nf++;
    EXAM( masregerrs_count_all( NULL ), ( size_t ) ( 3 + i * nfunctions ), "%ld:%ld" );
    function_four(  );
    nf++;
    EXAM( masregerrs_count_all( NULL ), ( size_t ) ( 4 + i * nfunctions ), "%ld:%ld" );
    EXAM( nf, nfunctions, "%d:%d" );
  }
  EXAM( masregerrs_count_all( NULL ), ( size_t ) ( counter * nfunctions ), "%ld:%ld" );
  fprintf( stderr, "----------------last:\n" );
  masregerr_print_simple_last( NULL, NULL );
  fprintf( stderr, "----------------forward:\n" );
  masregerr_print_simple_all( NULL, NULL, 0 );
  fprintf( stderr, "----------------back (%ld:%ld):\n", masregerrs_count_all( NULL ), ( size_t ) ( counter * 4 ) );
  masregerr_print_simple_all_back( NULL, NULL );
  fprintf( stderr, "----------------\n" );
  EXAM( masregerrs_count_all( NULL ), ( size_t ) ( counter * nfunctions ), "%ld:%ld" );
  return 0;
}
