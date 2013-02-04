#include <mastar/wrap/mas_std_def.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* #include "mas_basic_def.h" */

#include <mastar/wrap/mas_memory.h>

#include "mas_arg_tools.h"

size_t
mas_wordlen( const char *s )
{
  const char *s0;

  s0 = s;
  while ( s && *s && *s > ' ' )
    s++;
  return s0 ? s - s0 : 0;
}

char *
mas_chomp( char *str )
{
  if ( str )
  {
    char *estr;

    estr = str + strlen( str );
    while ( estr && estr > str )
    {
      --estr;
      if ( *estr > ' ' )
        break;
      *estr = '\0';
    }
  }
  return str;
}

int
mas_strcmp2( const char *s1, const char *s2 )
{
  size_t len;

  len = strlen( s2 );
  return strncmp( s1, s2, len );
}

int
mas_strcmpv( const char *s1, const char *s2, char **vval )
{
  char *eqp;
  size_t len;
  int r = -1;

  eqp = strchr( s1, '=' );
  if ( eqp )
  {
    len = eqp - s1;
    r = strncmp( s1, s2, len );
    if ( vval )
      *vval = eqp + 1;
  }
  return r;
}

const char *
mas_find_eq_value( const char *s )
{
  while ( s && *s )
  {
    if ( *s == '=' )
    {
      s++;
      break;
    }
    s++;
  }
  return s;
}

const char *
mas_skip_space( const char *args )
{
  const char *p = NULL;

  p = args;
  while ( p && *p && *p <= ' ' )
    p++;
  if ( !p || !*p )
    p = NULL;
  return p;
}

static const char *
mas_skip_arg( const char *args, const char **pthis, size_t * plen, int *quot )
{
  const char *p = NULL;
  const char *s = NULL;

  if ( quot )
    *quot = 0;
  p = args;
  if ( *p == '"' )
  {
    if ( quot )
      *quot = 1;
    p++;
    s = p;
    while ( p && *p && *p != '"' )
      p++;
    if ( pthis )
      ( *pthis ) = s;
    if ( plen )
      ( *plen ) = p - s;
    if ( p && *p == '"' )
      p++;
  }
  else
  {
    s = p;
    while ( p && *p > ' ' )
      p++;
    if ( pthis )
      ( *pthis ) = s;
    if ( plen )
      ( *plen ) = p - s;
  }
  if ( !p || !*p )
    p = NULL;

  return p;
}

const char *
mas_find_next_arg( const char *args, const char **pthis, size_t * plen, int *quot )
{
  const char *p = NULL;

  p = args;
  p = mas_skip_arg( p, pthis, plen, quot );
  p = mas_skip_space( p );
  /* while ( ( p = mas_skip_space( p ) ) && ( p = mas_skip_space( p ) ) ) */
  /* {                                                                    */
  /* }                                                                    */
  if ( !p || !*p )
    p = NULL;
  return p;
}

/*
 */
/* int                                                                      */
/* __mas_make_argv( const char *args, int argc, char ***pargv, int from_a ) */
/* {                                                                        */
/*   char **targv = NULL;                                                   */
/*   char **cargv = NULL;                                                   */
/*   const char *nextarg;                                                   */
/*   const char *prevarg = NULL;                                            */
/*                                                                          */
/*   (* char **targv; *)                                                    */
/*   int rargc = 0;                                                         */
/*   int nargs;                                                             */
/*                                                                          */
/*                                                                          */
/*   nargs = argc;                                                          */
/*   nextarg = args;                                                        */
/*   while ( nextarg && *nextarg )                                          */
/*   {                                                                      */
/*     const char *this;                                                    */
/*     size_t len;                                                          */
/*                                                                          */
/*     if ( nextarg )                                                       */
/*       nargs++;                                                           */
/*     prevarg = nextarg;                                                   */
/*     len = 0;                                                             */
/*     nextarg = mas_find_next_arg( nextarg, &this, &len, NULL );           */
/*   }                                                                      */
/*                                                                          */
/*   targv = mas_calloc( nargs + 2, sizeof( char * ) );                     */
/*   memset( targv, 0, ( nargs + 2 ) * sizeof( char * ) );                  */
/*   cargv = targv;                                                         */
/*   {                                                                      */
/*     char **p;                                                            */
/*                                                                          */
/*     p = *pargv;                                                          */
/*     for ( int ia = 0; ia < argc; ia++ )                                  */
/*     {                                                                    */
/*       (* MFP( "p:%s\n", p ? *p : NULL ); *)                              */
/*       if ( p && *p )                                                     */
/*         *cargv++ = mas_strdup( *p++ );                                   */
/*       else                                                               */
/*         *cargv++ = NULL;                                                 */
/*       rargc++;                                                           */
/*     }                                                                    */
/*   }                                                                      */
/*   nextarg = args;                                                        */
/*   {                                                                      */
/*     int iarg;                                                            */
/*                                                                          */
/*     iarg = 0;                                                            */
/*     while ( nextarg && *nextarg )                                        */
/*     {                                                                    */
/*       const char *this;                                                  */
/*       size_t len;                                                        */
/*                                                                          */
/*       prevarg = nextarg;                                                 */
/*       len = 0;                                                           */
/*       nextarg = mas_find_next_arg( nextarg, &this, &len, NULL );         */
/*       if ( iarg >= from_a )                                              */
/*         *cargv++ = mas_strndup( this, len );                             */
/*       else                                                               */
/*         *cargv++ = NULL;                                                 */
/*       *cargv = NULL;                                                     */
/*       iarg++;                                                            */
/*       rargc++;                                                           */
/*     }                                                                    */
/*   }                                                                      */
/*   *cargv = NULL;                                                         */
/*   if ( rargc != cargv - targv )                                          */
/*   {                                                                      */
/*     (* EMSG( "FATAL" ); *)                                               */
/*   }                                                                      */
/*   mas_del_argv( argc, *pargv, 0 );                                       */
/*   ( *pargv ) = targv;                                                    */
/*   return rargc;                                                          */
/* }                                                                        */

int
mas_add_argv_args( int targc, char ***ptargv, const char *args, int ia_offset )
{
  const char *nextarg;
  char **targv = NULL;
  int nargs = 0, totargc = 0;


  if ( ptargv )
  {
    int ita = 0;

    targv = *ptargv;
    *ptargv = NULL;
    nextarg = args;
    while ( nextarg && *nextarg )
    {
      const char *this;
      size_t len;

      if ( nextarg )
        nargs++;
      len = 0;
      nextarg = mas_find_next_arg( nextarg, &this, &len, NULL );
    }
    totargc = targc + nargs - ia_offset;
    targv = mas_realloc( targv, ( totargc + 1 ) * sizeof( void * ) );
    nextarg = args;
    nargs = 0;
    while ( nextarg && *nextarg )
    {
      const char *this, *a;
      size_t len;
      int q = 0;

      if ( nextarg )
        nargs++;
      len = 0;
      a = nextarg;
      nextarg = mas_find_next_arg( nextarg, &this, &len, &q );
      if ( nargs > ia_offset )
      {
        targv[targc + ita] = mas_strndup( a + q, len );
        ita++;
      }
      /* fprintf( stderr, "nargs:%u; ia_offset:%u; targc:%u; ita:%u; q:%d [%lu] %s\n", nargs, ia_offset, targc, ita, q, len, a ); */
    }
    targv[totargc] = NULL;
    *ptargv = targv;
  }
  else
  {
    fprintf( stderr, "no ptargv\n" );
  }
  return totargc;
}

int
mas_add_argv_argv( int targc, char ***ptargv, int argc, char *argv[], int ia_offset )
{
  char **targv = NULL;
  int totargc = 0;

  if ( ptargv )
  {
    totargc = targc + argc - ia_offset;
    targv = *ptargv;
    *ptargv = NULL;
    targv = mas_realloc( targv, ( totargc + 1 ) * sizeof( void * ) );
    for ( int ia = ia_offset; ia < argc; ia++ )
    {
      targv[ia - ia_offset + targc] = mas_strdup( argv[ia] );
    }
    targv[totargc] = NULL;
    *ptargv = targv;
  }
  return totargc;
}

int
mas_add_argv_arg( int targc, char ***ptargv, const char *arg )
{
  if ( ptargv && arg )
  {
    char **targv;

    targv = *ptargv;
    targc++;
    if ( targv )
      targv = mas_realloc( targv, ( targc + 1 ) * sizeof( char * ) );
    else
      targv = mas_calloc( ( targc + 1 ), sizeof( char * ) );
    targv[targc - 1] = mas_strdup( arg );
    targv[targc] = NULL;
    *ptargv = targv;
  }
  return targc;
}

int
mas_del_argv( int targc, char **targv, int from_a )
{
  for ( int ia = from_a; ia < targc; ia++ )
  {
    mas_free( targv[ia] );
    targv[ia] = NULL;
  }
  mas_free( targv );
  return 0;
}

char *
mas_argv_string( int targc, char *const *targv, int from_a )
{
  char *value = NULL;

  for ( int ia = from_a; ia < targc; ia++ )
  {
    value = mas_strcat_xt( value, targv[ia] );
    if ( ia < targc - 1 )
      value = mas_strcat_xt( value, " " );
  }
  return value;
}

char *
mas_strcat_xt( char *s1, const char *s2 )
{
  char *r = NULL;
  int l, l1, l2;

  l1 = s1 ? strlen( s1 ) : 0;
  l2 = s2 ? strlen( s2 ) : 0;
  l = l1 + l2;
  if ( l > 0 )
  {
    r = mas_malloc( l + 1 );
    if ( s1 )
    {
      strcpy( r, s1 );
      free( s1 );
    }
    if ( s2 )
      strcpy( r + l1, s2 );
  }
  return r;
}

char *
mas_strncat_xt( char *s1, const char *s2, size_t maxs2 )
{
  char *r = NULL;
  int l, l1, l2;

  l1 = s1 ? strlen( s1 ) : 0;
  l2 = s2 ? strlen( s2 ) : 0;
  if ( maxs2 < l2 )
    l2 = maxs2;
  l = l1 + l2;
  if ( l > 0 )
  {
    r = mas_malloc( l + 1 );
    if ( s1 )
    {
      memcpy( r, s1, l1 );
      mas_free( s1 );
    }
    if ( s2 )
      memcpy( r + l1, s2, l2 );
    r[l] = 0;
  }
  return r;
}
