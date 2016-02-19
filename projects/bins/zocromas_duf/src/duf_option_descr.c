#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_z.h"
/* #include "duf_maintenance_tracen.h" */
/* #include "duf_maintenance_errors.h" */

#include "duf_tmp_types.h"

/* #include "duf_option_find.h" */
#include "duf_option_cfind.h"
#include "duf_option_lfind.h"

#include "duf_option_names.h"

/* ###################################################################### */
#include "duf_option_descr.h"
/* ###################################################################### */

/* TODO to new option system , see duf_option_clarify_new.c
 * use soption_xclarify_new_at_stdx with specific `duf_xclarifier_t clarifier`
 *
 * */

/* -= by duf_option_code_t codeval =- */
static const char *
_duf_coption_xfind_help_at_stdx( duf_option_code_t codeval, int *pr )
{
  const char *ph = NULL;
  const duf_longval_extended_t *extended;

  extended = duf_coption_xfind_at_stdx( codeval, NULL, pr );
  /* extended = &lo_extended[ilong]; */
  if ( extended )
    ph = extended->help;
  return extended ? ( ph ? ph : "no description" ) : "not found";
}

const char *
duf_coption_xfind_desc_at_stdx( duf_option_code_t codeval, int *pr )
{
  const char *ph = _duf_coption_xfind_help_at_stdx( codeval, pr );

  return ph ? ph : "-";
}

#if 0
/* -= by int longindex =- */
char *
duf_loption_description_d( int longindex, const char *delimh, const char *delim )
{
  char *p = NULL;
  const duf_longval_extended_t *extended;

  extended = duf_loption_xfind_at_stdx( longindex, ( const duf_longval_extended_vtable_t ** ) NULL, NULL /* &no */  );
  if ( extended )
    p = duf_xoption_description_d( extended, delimh, delim );
  return p;
}
#endif

/* -= by duf_longval_extended_t * extended =- */
char *
duf_xoption_description_d( const duf_longval_extended_t * extended, const char *delimh, const char *delim )
{
  char *s = NULL;

  if ( extended )
  {
    s = duf_coption_names_d( extended->o.val, delim );
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

static char *
duf_xoption_description( const duf_longval_extended_t * extended )
{
  return duf_xoption_description_d( extended, NULL, NULL );
}

static duf_tmp_t *cnames_tmp = NULL;

const char *
duf_xoption_description_tmp( int tmp_index, const duf_longval_extended_t * extended )
{
  const char *x = NULL;

  if ( tmp_index < 0 )
  {
    tmp_index = cnames_tmp->tmp_index++;
    if ( cnames_tmp->tmp_index >= DUF_TMP_INDEX_MAX )
      cnames_tmp->tmp_index = 0;
  }

  if ( tmp_index >= 0 && tmp_index < DUF_TMP_INDEX_MAX )
  {
    mas_free( cnames_tmp->tmp_string[tmp_index] );
    cnames_tmp->tmp_string[tmp_index] = NULL;
    cnames_tmp->tmp_string[tmp_index] = duf_xoption_description( extended );
    x = cnames_tmp->tmp_string[tmp_index];
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
