#include "masregerr_defs.h"
#include <malloc.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>

#include <limits.h>
#include <stdlib.h>

#include <mastar/wrap/mas_memory.h>

#include "masregerr_structs.h"
#include "masregerr_base.h"

#include "masregerr.h"

static int reg_errors_max_print = 0;
static masregerrs_t *reg_errors = NULL;

static void destructor_main(  ) __attribute__ ( ( destructor( 2001 ) ) );
static void
destructor_main(  )
{
  masregerr_print_simple_all_default( reg_errors, NULL, reg_errors_max_print );
  masregerrs_delete_default( reg_errors );
}

void
masregerrs_set_max_print( int num )
{
  reg_errors_max_print = num;
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

void
masregerrs_delete_default( masregerrs_t * regerrs )
{
  if ( !regerrs )
  {
    regerrs = reg_errors;
    reg_errors = NULL;
  }
  masregerrs_delete( regerrs );
}

static masregerr_t *
masregerr_start_default( masregerrs_t * regerrs )
{
  if ( !regerrs )
    regerrs = masregerrs_default( regerrs );
  return regerrs ? regerrs->err_array : NULL;
}

static masregerr_t *
masregerr_end( masregerrs_t * regerrs )
{
  if ( !regerrs )
    regerrs = masregerrs_default( regerrs );
/* fprintf( stderr, "%p (%ld)\n", masregerr_start_default( regerrs ),  regerrs ->size ); */
  return masregerr_start_default( regerrs ) + regerrs->size;
}

static masregerr_t *
masregerr_after( masregerrs_t * regerrs, masregerr_t * rge )
{
  rge++;
  if ( rge >= masregerr_end( regerrs ) )
    rge = masregerr_start_default( regerrs );
  return rge;
}

static masregerr_t *
masregerr_before( masregerrs_t * regerrs, masregerr_t * rge )
{
  rge--;
  if ( rge < masregerr_start_default( regerrs ) )
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
  return rge && rge->mark ? rge : NULL;
}

static masregerr_t *
masregerr_valid_after( masregerrs_t * regerrs, masregerr_t * rge )
{
  masregerr_t *rge0 = rge;

  do
  {
    rge = masregerr_after( regerrs, rge );
  } while ( rge && !rge->mark && rge != rge0 );
  return rge && rge->mark ? rge : NULL;
}

static masregerr_t *
masregerr_next( masregerrs_t * regerrs )
{
  if ( !regerrs )
    regerrs = masregerrs_default( regerrs );
  masregerrs_default( regerrs )->current = masregerr_after( regerrs, masregerrs_default( regerrs )->current );
  return masregerrs_default( regerrs )->current;
}

static _uUu_ masregerr_t *
masregerr_prev( masregerrs_t * regerrs )
{
  if ( !regerrs )
    regerrs = masregerrs_default( regerrs );
  masregerrs_default( regerrs )->current = masregerr_before( regerrs, masregerrs_default( regerrs )->current );
  return masregerrs_default( regerrs )->current;
}

void
masregerr_reg( masregerrs_t * regerrs, const char *func, int line, const char *file, const char *func1, const char *func2, const char *package,
               int *perrno, int sys, const char *fmt, ... )
{
  if ( !regerrs )
    regerrs = masregerrs_default( regerrs );
  if ( regerrs )
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
      if ( fmt )
      {
        char msg[1024 * 10];
        va_list args;

        va_start( args, fmt );
        vsnprintf( msg, sizeof( msg ), fmt, args );
        va_end( args );
        rge->msg = mas_strdup( msg );
      }
      rge->serial = ++( regerrs->serial );
      if ( perrno )
        *perrno = 0;
    /* assert( 0 ); */
    }
  }
}

void
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
    fprintf( stderr, "\x1b[0;1;41;33m%s\x1b[0m - ", msg );
    if ( rge->sys )
      fprintf( stderr, "#%-3ld %3d:%-30s %-10s @ %-30s\x1b[7;1;41;33m  E%02d:%s\x1b[0m (sys)\n", rge->serial, rge->line, rge->func, rge->package,
               sfile, rge->err_no, rge->msg ? rge->msg : ( rge->err_no ? strerror( rge->err_no ) : "" ) );
    else
      fprintf( stderr, "#%-3ld %3d:%-30s %-10s @ %-30s\x1b[7;1;41;33m  %s  \x1b[0m\n", rge->serial, rge->line, rge->func, rge->package, sfile,
               rge->msg ? rge->msg : "-" );
  }
}

void
masregerr_print_simple_all_back_default( masregerrs_t * regerrs, const char *msg )
{
  if ( !regerrs )
    regerrs = masregerrs_default( regerrs );

  int num = 0;

  if ( regerrs )
  {
    masregerr_t *rge = regerrs->current;

    do
    {
      num++;
    /* fprintf( stderr, "%2d. ", num ); */
      masregerr_print_simple( rge, msg );
      rge = masregerr_valid_before( regerrs, rge );
    } while ( rge && rge->mark && rge != regerrs->current );
  }
}

void
masregerr_print_simple_all_default( masregerrs_t * regerrs, const char *msg, int max_print )
{
  if ( !regerrs )
    regerrs = masregerrs_default( regerrs );
  int num = 0;

  if ( regerrs )
  {
    masregerr_t *rge = regerrs->current;

    do
    {
      num++;
      rge = masregerr_valid_after( regerrs, rge );
      if ( rge )
      {
      /* fprintf( stderr, "%2d. %p %p\n", num, rge,  regerrs->current ); */
        masregerr_print_simple( rge, msg );
      }
    } while ( rge && rge != regerrs->current && ( !max_print || num <= max_print ) );
    if ( max_print && num > max_print )
    {
      fprintf( stderr, "... ...\n" );
    }
  }
}

unsigned
masregerrs_count_all_default( masregerrs_t * regerrs, int count_hidden )
{
  size_t num = 0;

  if ( !regerrs )
    regerrs = masregerrs_default( regerrs );
  if ( regerrs )
  {
    masregerr_t *rge = regerrs->current;

    do
    {
      rge = masregerr_valid_after( regerrs, rge );
      if ( rge && ( count_hidden || !rge->hidden ) )
        num++;
    } while ( rge && rge != regerrs->current );
  }
  return num;
}

void
masregerr_print_simple_last_default( masregerrs_t * regerrs, const char *msg )
{
  if ( !regerrs )
    regerrs = masregerrs_default( regerrs );
  return regerrs ? masregerr_print_simple( regerrs->current, msg ) : 0;
}

const char *
masregerr_last_msg_default( masregerrs_t * regerrs )
{
  masregerr_t *rge = NULL;

  if ( !regerrs )
    regerrs = masregerrs_default( regerrs );
  if ( regerrs )
    rge = regerrs->current;

  return rge ? rge->msg : NULL;
}
