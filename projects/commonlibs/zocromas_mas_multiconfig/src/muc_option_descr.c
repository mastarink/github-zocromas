/* #undef MAS_TRACING */
/* #define MAST_TRACE_CONFIG muc_get_cli_options_trace_config(cli) */
/* #include "muc_tracen_defs_preset.h" */

#include <assert.h>                                                  /* assert */
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
/* #include <mastar/trace/mas_trace.h> */
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ♣ */

#include "muc_tmp_types.h"

#include "muc_option_cfind.h"                                        /* muc_coption_xfind_at_stdx;  ..._coption_*  etc... ♠ */

#include "muc_option_names.h"                                        /* muc_coption_names_d etc... ♠ */

#include "muc_config_cli_structs.h"
/* ###################################################################### */
#include "muc_option_descr.h"
/* ###################################################################### */

/* TODO to new option system , see ..._option_clarify_new.c
 * use soption_xclarify_new_at_stdx with specific `muc_xclarifier_t clarifier`
 *
 * */

/* -= by muc_option_gen_code_t codeval =- */
static const char *
coption_xfind_help_at_stdx( muc_config_cli_t * cli, muc_option_gen_code_t codeval, int *pr )
{
  const char *ph = NULL;
  const muc_longval_extended_t *extended;

  extended = muc_coption_xfind_at_stdx( cli, codeval, NULL, pr );
/* extended = &lo_extended[ilong]; */
  if ( extended )
    ph = extended->help;
  return extended ? ( ph ? ph : "no description" ) : "not found";
}

const char *
muc_coption_xfind_desc_at_stdx( muc_config_cli_t * cli, muc_option_gen_code_t codeval, int *pr )
{
  const char *ph = coption_xfind_help_at_stdx( cli, codeval, pr );

  return ph ? ph : "-";
}

#if 0
/* -= by int longindex =- */
char *...
_loption_description_d( int longindex, const char *delimh, const char *delim )
{
  char *p = NULL;
  const ... _longval_extended_t *extended;

  extended = ... _loption_xfind_at_stdx( longindex, ( const ... _longval_extended_vtable_t ** ) NULL, NULL /* &no */  );
  if ( extended )
    p = ... _xoption_description_d( extended, delimh, delim );
  return p;
}
#endif

/* -= by ..._longval_extended_t * extended =- */
char *
muc_xoption_description_d( muc_config_cli_t * cli, const muc_longval_extended_t * extended, const char *delimh, const char *delim )
{
  char *s = NULL;

  if ( extended )
  {
    s = muc_coption_names_d( cli, extended->o.val, delim );
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
muc_xoption_description( muc_config_cli_t * cli, const muc_longval_extended_t * extended )
{
  return muc_xoption_description_d( cli, extended, NULL, NULL );
}

static muc_tmp_t *cnames_tmp = NULL;

const char *
muc_xoption_description_tmp( muc_config_cli_t * cli, int tmp_index, const muc_longval_extended_t * extended )
{
  const char *x = NULL;

  if ( tmp_index < 0 )
  {
    tmp_index = cnames_tmp->tmp_index++;
    if ( cnames_tmp->tmp_index >= MUC_TMP_INDEX_MAX )
      cnames_tmp->tmp_index = 0;
  }

  if ( tmp_index >= 0 && tmp_index < MUC_TMP_INDEX_MAX )
  {
    mas_free( cnames_tmp->tmp_string[tmp_index] );
    cnames_tmp->tmp_string[tmp_index] = NULL;
    cnames_tmp->tmp_string[tmp_index] = muc_xoption_description( cli, extended );
    x = cnames_tmp->tmp_string[tmp_index];
  }
  return x;
}

static void constructor_tmp( void ) __attribute__ ( ( constructor( 101 ) ) );
static void
constructor_tmp( void )
{
  cnames_tmp = mas_malloc( sizeof( muc_tmp_t ) );
  memset( cnames_tmp, 0, sizeof( muc_tmp_t ) );
}

static void destructor_tmp( void ) __attribute__ ( ( destructor( 101 ) ) );
static void
destructor_tmp( void )
{
  if ( cnames_tmp )
  {
    for ( int i = 0; i < MUC_TMP_INDEX_MAX; i++ )
    {
      mas_free( cnames_tmp->tmp_string[i] );
      cnames_tmp->tmp_string[i] = NULL;
    }
    mas_free( cnames_tmp );
  }
}
