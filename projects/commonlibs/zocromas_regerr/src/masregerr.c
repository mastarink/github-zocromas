#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include <limits.h>
#include <stdlib.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "masregerr.h"

static masregerr_t *regerrs_end = 0;
static masregerr_t *regerrs = NULL;
static masregerr_t *regerr_current = NULL;
static long regerr_serial = 0;

static void destructor_main(  ) __attribute__ ( ( destructor( 2001 ) ) );
static void
destructor_main(  )
{
  if ( regerrs )
    mas_free( regerrs );
  regerrs = regerr_current = regerrs_end = NULL;
}

masregerr_t *
masregerr_init( size_t size )
{
  if ( !size )
    size = MASREGERR_DEFAULT_SIZE;
  if ( !regerrs )
  {
    regerrs = mas_calloc( size, sizeof( masregerr_t ) );
    regerrs_end = regerrs + size;
    regerr_current = regerrs;
  }
  return regerrs;
}

masregerr_t *
masregerr_after( masregerr_t * rge )
{
  rge++;
  if ( rge >= regerrs_end )
    rge = regerrs;
  return rge;
}

masregerr_t *
masregerr_before( masregerr_t * rge )
{
  rge--;
  if ( rge < regerrs )
    rge = regerrs_end - 1;
  return rge;
}

masregerr_t *
masregerr_valid_before( masregerr_t * rge )
{
  do
  {
    rge = masregerr_before( rge );
  } while ( rge && !rge->mark );
  return rge;
}

masregerr_t *
masregerr_valid_after( masregerr_t * rge )
{
  do
  {
    rge = masregerr_after( rge );
  } while ( rge && !rge->mark );
  return rge;
}

masregerr_t *
masregerr_next( void )
{
  if ( !regerr_current )
    masregerr_init( 0 );
  regerr_current = masregerr_after( regerr_current );
  return regerr_current;
}

masregerr_t *
masregerr_prev( void )
{
  if ( !regerr_current )
    masregerr_init( 0 );
  regerr_current = masregerr_before( regerr_current );
  return regerr_current;
}

int
masregerr_reg( const char *func, int line, const char *file, const char *package, int *perrno, int sys )
{
  masregerr_t *rge = masregerr_next(  );
  int err_no = 0;

  if ( perrno )
    err_no = *perrno;

  if ( rge )
  {
    rge->mark = 1;
    rge->func = func;
    rge->line = line;
    rge->file = file;
    rge->package = package;
    rge->err_no = err_no;
    rge->sys = sys;
    rge->serial = ++regerr_serial;
    if ( perrno )
      *perrno = 0;
  }
  return 0;
}

int
masregerr_print_simple( const masregerr_t * rge, const char *msg )
{
  if ( rge )
  {
    const char *sfile = rge->file ? strrchr( rge->file, '/' ) : NULL;

    if ( sfile )
      sfile++;
    else
      sfile = rge->file;
    if ( !msg )
      msg = "Error";
    fprintf( stderr, "%s - ", msg );
    if ( rge->sys )
      fprintf( stderr, "#%-3ld %3d:%-30s %-10s @ %-30s E%02d:%-40s \n", rge->serial, rge->line, rge->func, rge->package, sfile, rge->err_no,
               strerror( rge->err_no ) );
    else
      fprintf( stderr, "#%-3ld %3d:%-30s %-10s @ %-30s  %-44s\n", rge->serial, rge->line, rge->func, rge->package, sfile, "-" );
  }
  return 0;
}

int
masregerr_print_simple_all_back( const char *msg )
{
  masregerr_t *rge = regerr_current;

  int num = 0;

  do
  {
    num++;
  /* fprintf( stderr, "%2d. ", num ); */
    masregerr_print_simple( rge, msg );
    rge = masregerr_valid_before( rge );
  } while ( rge && rge->mark && rge != regerr_current );
  return 0;
}

int
masregerr_print_simple_all( const char *msg )
{
  masregerr_t *rge = regerr_current;
  int num = 0;

  do
  {
    num++;
    rge = masregerr_valid_after( rge );
  /* fprintf( stderr, "%2d. ", num ); */
    masregerr_print_simple( rge, msg );
  } while ( rge && rge != regerr_current );
  return 0;
}

int
masregerr_print_simple_last( const char *msg )
{
  return masregerr_print_simple( regerr_current, msg );
}
