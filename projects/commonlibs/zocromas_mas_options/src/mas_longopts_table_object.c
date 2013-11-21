#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "mas_longopts_table_object.h"

struct option *
mas_cli_longopts_table_create_old( const mas_optionx_t * table, size_t num )
{
  struct option *longtab = NULL;

  longtab = mas_malloc( sizeof( struct option ) * num );
  for ( int i = 0; i < num; i++ )
  {
    longtab[i] = table[i].longopt;
    longtab[i].name = mas_strdup( table[i].longopt.name );
  }
  return longtab;
}

struct option *
mas_cli_longopts_table_create( const mas_optionx_t * table )
{
  const mas_optionx_t *ptable;
  struct option *longtab = NULL;
  size_t indx = 0;
  int nz = 0;

  ptable = table;
  indx = 0;
  do
  {
    nz = ptable->longopt.name ? 1 : 0;
    ptable++;
    indx++;
  }
  while ( nz );
  longtab = mas_malloc( sizeof( struct option ) * indx );
  memset( longtab, indx, 0 );
  /* for ( int i = 0; i < num; i++ ) */

  ptable = table;
  indx = 0;
  do
  {
    nz = ptable->longopt.name ? 1 : 0;
    longtab[indx] = ptable->longopt;
    longtab[indx].name = mas_strdup( ptable->longopt.name );
    ptable++;
    indx++;
  }
  while ( nz );
  return longtab;
}

void
mas_cli_longopts_table_delete_old( struct option *longtab, size_t num )
{
  for ( int i = 0; i < num; i++ )
  {
    mas_free( ( char * ) longtab[i].name );
  }
  mas_free( longtab );
}

void
mas_cli_longopts_table_delete( struct option *longtab )
{
  struct option *ptable = longtab;
  size_t indx = 0;
  int nz;

  do
  {
    nz = ptable->name ? 1 : 0;
    mas_free( ( char * ) ptable->name );
    ptable->name = NULL;
    ptable++;
    indx++;
  }
  while ( nz );

  mas_free( longtab );
}
