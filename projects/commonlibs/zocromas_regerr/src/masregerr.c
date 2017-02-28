#include "masregerr_defs.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include <limits.h>
#include <stdlib.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "masregerr_structs.h"
#include "masregerr_base.h"

#include "masregerr.h"

static masregerrs_t *reg_errors = NULL;

static void destructor_main(  ) __attribute__ ( ( destructor( 2001 ) ) );
static void
destructor_main(  )
{
  masregerr_print_simple_all( reg_errors, NULL );
  masregerrs_delete( reg_errors );
}

static masregerrs_t *
masregerrs_default( masregerrs_t * regerrs )
{
  if ( !regerrs )
  {
    if ( !reg_errors )
      reg_errors = masregerrs_create_setup( 0 );
    regerrs = reg_errors;
  }
  return regerrs;
}

static masregerr_t *
masregerr_start( masregerrs_t * regerrs )
{
  return masregerrs_default( regerrs )->err_array;
}

static masregerr_t *
masregerr_end( masregerrs_t * regerrs )
{
/* fprintf( stderr, "%p (%ld)\n", masregerr_start( regerrs ), masregerrs_default( regerrs )->size ); */
  return masregerr_start( regerrs ) + masregerrs_default( regerrs )->size;
}

static masregerr_t *
masregerr_after( masregerrs_t * regerrs, masregerr_t * rge )
{
  rge++;
  if ( rge >= masregerr_end( regerrs ) )
    rge = masregerr_start( regerrs );
  return rge;
}

static masregerr_t *
masregerr_before( masregerrs_t * regerrs, masregerr_t * rge )
{
  rge--;
  if ( rge < masregerr_start( regerrs ) )
    rge = masregerr_end( regerrs ) - 1;
  return rge;
}

static masregerr_t *
masregerr_valid_before( masregerrs_t * regerrs, masregerr_t * rge )
{
  masregerr_t *rge0 = rge;

  do
  {
    rge = masregerr_before( regerrs, rge );
  } while ( rge && !rge->mark && rge != rge0 );
  return rge->mark ? rge : NULL;
}

static masregerr_t *
masregerr_valid_after( masregerrs_t * regerrs, masregerr_t * rge )
{
  masregerr_t *rge0 = rge;

  do
  {
    rge = masregerr_after( regerrs, rge );
  } while ( rge && !rge->mark && rge != rge0 );
  return rge->mark ? rge : NULL;
}

static masregerr_t *
masregerr_next( masregerrs_t * regerrs )
{
  masregerrs_default( regerrs )->current = masregerr_after( regerrs, masregerrs_default( regerrs )->current );
  return masregerrs_default( regerrs )->current;
}

static _uUu_ masregerr_t *
masregerr_prev( masregerrs_t * regerrs )
{
  masregerrs_default( regerrs )->current = masregerr_before( regerrs, masregerrs_default( regerrs )->current );
  return masregerrs_default( regerrs )->current;
}

int
masregerr_reg( masregerrs_t * regerrs, const char *func, int line, const char *file, const char *func1, const char *func2, const char *package,
               int *perrno, int sys, const char *msg )
{
  masregerr_t *rge = masregerr_next( regerrs );
  int err_no = 0;

  if ( perrno )
    err_no = *perrno;

  if ( rge )
  {
    masregerr_reset( rge );
    rge->mark = 1;
    rge->func = func;
    rge->func1 = func1;
    rge->func2 = func2;
    rge->line = line;
    rge->file = file;
    rge->package = package;
    rge->err_no = err_no;
    rge->sys = sys;
    if ( msg )
    {
      rge->msg = mas_strdup( msg );
    }
    rge->serial = ++( masregerrs_default( regerrs )->serial );
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
               rge->msg ? rge->msg : ( rge->err_no ? strerror( rge->err_no ) : "" ) );
    else
      fprintf( stderr, "#%-3ld %3d:%-30s %-10s @ %-30s  %-44s\n", rge->serial, rge->line, rge->func, rge->package, sfile, rge->msg ? rge->msg : "-" );
  }
  return 0;
}

int
masregerr_print_simple_all_back( masregerrs_t * regerrs, const char *msg )
{
  masregerr_t *rge = masregerrs_default( regerrs )->current;

  int num = 0;

  do
  {
    num++;
  /* fprintf( stderr, "%2d. ", num ); */
    masregerr_print_simple( rge, msg );
    rge = masregerr_valid_before( regerrs, rge );
  } while ( rge && rge->mark && rge != masregerrs_default( regerrs )->current );
  return 0;
}

int
masregerr_print_simple_all( masregerrs_t * regerrs, const char *msg )
{
  masregerr_t *rge = masregerrs_default( regerrs )->current;
  int num = 0;

  do
  {
    num++;
    rge = masregerr_valid_after( regerrs, rge );
    if ( rge )
    {
      /* fprintf( stderr, "%2d. %p %p\n", num, rge, masregerrs_default( regerrs )->current ); */
      masregerr_print_simple( rge, msg );
    }
  } while ( rge && rge != masregerrs_default( regerrs )->current );
  return 0;
}

size_t
masregerrs_count_all( masregerrs_t * regerrs )
{
  masregerr_t *rge = masregerrs_default( regerrs )->current;
  size_t num = 0;

  do
  {
    rge = masregerr_valid_after( regerrs, rge );
    if ( rge )
      num++;
  } while ( rge && rge != masregerrs_default( regerrs )->current );
  return num;
}

int
masregerr_print_simple_last( masregerrs_t * regerrs, const char *msg )
{
  return masregerr_print_simple( masregerrs_default( regerrs )->current, msg );
}
