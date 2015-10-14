#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_options_table.h"
/* ###################################################################### */
#include "duf_option_extended.h"
/* ###################################################################### */



#if 0
const char *
duf_longindex_extended_name( int longindex, int *pr )
{
  const duf_longval_extended_t *extended = NULL;

  extended = duf_longindex2extended( longindex, pr );
  return extended ? extended->o.name : NULL;
}

duf_option_code_t
duf_longindex_extended_codeval( int longindex, int *pr )
{
  const duf_longval_extended_t *extended = NULL;

  extended = duf_longindex2extended( longindex, pr );
  return extended ? extended->o.val : 0;
}
#endif
const duf_longval_extended_t *
_duf_longindex2extended( int longindex, const duf_longval_extended_table_t * xtable, int *ptbcount, int no )
{
  const duf_longval_extended_t *extended = NULL;

  for ( const duf_longval_extended_t * xtended = xtable->table; *ptbcount <= longindex && xtended->o.name; xtended++ )
  {
    DUF_TRACE( options, 5, "@li2ex %d [%s]", *ptbcount, xtended->o.name );
    if ( no )
    {
      if ( xtended->can_no )
      {
        if ( *ptbcount == longindex )
        {
          extended = xtended;
          break;
        }
        ( *ptbcount )++;
      }
    }
    else
    {
      if ( *ptbcount == longindex )
      {
        extended = xtended;
        /* break; */
      }
      ( *ptbcount )++;
    }
  }

  return extended;
}

static void
duf_xtable2options( duf_option_t ** plongopts_ptr, const duf_longval_extended_t * xtended, int no )
{
  while ( xtended->o.name )
  {
    if ( no )
    {
      if ( xtended->can_no )
      {
        char *s = NULL;

        *( *plongopts_ptr ) = xtended->o;
        s = mas_strdup( "no-" );
        s = mas_strcat_x( s, ( *plongopts_ptr )->name );
        ( *plongopts_ptr )->name = s;
        /* ( *plongopts_ptr )->has_arg = xtended->o.has_arg; */
        /* ( *plongopts_ptr )->val = xtended->o.val;         */
        ( *plongopts_ptr )++;
      }
    }
    else
    {
      *( *plongopts_ptr ) = xtended->o;
      ( *plongopts_ptr )->name = mas_strdup( ( *plongopts_ptr )->name );
      /* ( *plongopts_ptr )->has_arg = xtended->o.has_arg; */
      /* ( *plongopts_ptr )->val = xtended->o.val;         */
      ( *plongopts_ptr )++;
    }
    xtended++;
  }
}

void
duf_options_delete_longopts_table( duf_option_t * longopts )
{
  for ( duf_option_t * o = longopts; o->name; o++ )
    mas_free( ( char * ) o->name );
  mas_free( longopts );
}

duf_option_t *
duf_options_create_longopts_table( const duf_longval_extended_table_t ** pxtable )
{
  duf_option_t *longopts = NULL;

  {
    duf_option_t *longopts_ptr;
    size_t tbsize = 0;

    tbsize = duf_longindex_extended_count( pxtable ) * ( sizeof( duf_longval_extended_t ) + 1 );

    longopts = longopts_ptr = mas_malloc( tbsize );
    memset( longopts, 0, tbsize );

    for ( const duf_longval_extended_table_t ** xtables = pxtable; *xtables; xtables++ )
      duf_xtable2options( &longopts_ptr, ( *xtables )->table, 0 );
    for ( const duf_longval_extended_table_t ** xtables = pxtable; *xtables; xtables++ )
      duf_xtable2options( &longopts_ptr, ( *xtables )->table, 1 );
  }
  return longopts;
}

const duf_longval_extended_t *
duf_longindex2extended( int longindex, const duf_longval_extended_table_t ** pxtable, int *pno )
{
  const duf_longval_extended_t *extended = NULL;
  int tbcount = 0;

  for ( const duf_longval_extended_table_t ** xtables = lo_extended_table_multi; tbcount <= longindex && *xtables; xtables++ )
  {
    const duf_longval_extended_table_t *xtable = *xtables;

#if 0

    for ( const duf_longval_extended_t * xtended = xtable->table; tbcount <= longindex && xtended->o.name; xtended++ )
    {
      DUF_TRACE( options, 5, "@li2ex %d [%s]", tbcount, xtended->o.name );
      if ( tbcount == longindex )
      {
        extended = xtended;
        if ( pxtable )
          *pxtable = xtable;
        /* break; */
      }
      tbcount++;
    }
#else
    extended = _duf_longindex2extended( longindex, xtable, &tbcount, 0 /* no */  );
    if ( extended )
    {
      if ( pxtable )
        *pxtable = xtable;
      break;
    }
#endif
  }
  if ( !extended )
  {
    /* continue with tbcount */
    for ( const duf_longval_extended_table_t ** xtables = lo_extended_table_multi; tbcount <= longindex && *xtables; xtables++ )
    {
      const duf_longval_extended_table_t *xtable = *xtables;

      extended = _duf_longindex2extended( longindex, xtable, &tbcount, 1 /* no */  );
      if ( extended )
      {
        if ( pxtable )
          *pxtable = xtable;
        if ( pno )
          *pno = !*pno;
        break;
      }
    }
  }

  return extended;
}

int
duf_longindex_extended_count( const duf_longval_extended_table_t ** xtables )
{
  int xcount = 0;

#if 0
  const duf_longval_extended_table_t *xtable;

  while ( ( xtable = *xtables++ ) )
  {
    const duf_longval_extended_t *xtended = xtable->table;

    while ( xtended->o.name )
    {
      xcount++;
      xtended++;
      if ( xtended->can_no )
        xcount++;
    }
  }
#else
  for ( const duf_longval_extended_table_t ** pxtables = xtables; *pxtables; pxtables++ )
  {
    const duf_longval_extended_t *xtended = NULL;

    xtended = ( *pxtables )->table;
    while ( xtended->o.name )
    {
      xcount++;
      xtended++;
      if ( xtended->can_no )
        xcount++;
    }
  }
#endif
  return xcount;
}
