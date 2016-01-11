#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_options.h"

/* #include "duf_status_ref.h" */
#include "duf_tmp_types.h"

#include "duf_option_find.h"
#include "duf_option_extended.h"
#include "duf_option_names.h"

/* ###################################################################### */
#include "duf_option_descr.h"
/* ###################################################################### */


static const char *
_duf_find_longval_help( duf_option_code_t codeval, int *pr )
{
  const char *ph = NULL;
  const duf_longval_extended_t *extended;

  extended = duf_find_codeval_extended_std( codeval, NULL, pr );
  /* extended = &lo_extended[ilong]; */

  if ( extended )
    ph = extended->help;
  return extended ? ( ph ? ph : "no description" ) : "not found";
}

const char *
duf_find_longval_help( duf_option_code_t codeval, int *pr )
{
  const char *ph = _duf_find_longval_help( codeval, pr );

  return ph ? ph : "-";
}

char *
duf_option_description_xd( const duf_longval_extended_t * extended, const char *delimh, const char *delim )
{
  char *s = NULL;

  if ( extended )
  {
    duf_option_code_t codeval;

    codeval = extended->o.val;
    s = duf_option_names_d( codeval, delim );
    if ( s )
    {
      const char *h;

      h = extended->help;
      if ( h )
      {
        s = mas_strcat_x( s, delimh ? delimh : " :: " );
        s = mas_strcat_x( s, h );
      }
    }
  }
  return s;
}

char *
duf_option_description_d( int longindex, const char *delimh, const char *delim )
{
  char *p = NULL;
  const duf_longval_extended_t *extended;

  extended = duf_longindex2extended( longindex, ( const duf_longval_extended_table_t ** ) NULL, NULL /* &no */  );
  if ( extended )
    p = duf_option_description_xd( extended, delimh, delim );
  return p;
}

static char *
duf_option_description_x( const duf_longval_extended_t * extended )
{
  return duf_option_description_xd( extended, NULL, NULL );
}

/* char *                                                          */
/* duf_option_description( int longindex, int *pr )                */
/* {                                                               */
/*   return duf_option_description_d( longindex, NULL, NULL, pr ); */
/* }                                                               */
/*                                                                 */

static duf_tmp_t *cnames_tmp = NULL;

const char *
duf_option_description_x_tmp( int index, const duf_longval_extended_t * extended )
{
  const char *x = NULL;

  if ( index < 0 )
  {
    index = cnames_tmp->tmp_index++;
    if ( cnames_tmp->tmp_index >= DUF_TMP_INDEX_MAX )
      cnames_tmp->tmp_index = 0;
  }

  if ( index >= 0 && index < DUF_TMP_INDEX_MAX )
  {
    mas_free( cnames_tmp->tmp_string[index] );
    cnames_tmp->tmp_string[index] = NULL;
    cnames_tmp->tmp_string[index] = duf_option_description_x( extended );
    x = cnames_tmp->tmp_string[index];
  }
  return x;
}
static void constructor_tmp( void ) __attribute__ ( ( constructor( 101 ) ) );
static void
constructor_tmp( void )
{
  cnames_tmp = mas_malloc( sizeof( duf_tmp_t ) );
  memset( cnames_tmp, 0, sizeof( duf_tmp_t ) );
}

static void destructor_tmp( void ) __attribute__ ( ( destructor( 101 ) ) );
static void
destructor_tmp( void )
{
  if ( cnames_tmp )
  {
    for ( int i = 0; i < DUF_TMP_INDEX_MAX; i++ )
    {
      mas_free( cnames_tmp->tmp_string[i] );
      cnames_tmp->tmp_string[i] = NULL;
    }
    mas_free( cnames_tmp );
  }
}
