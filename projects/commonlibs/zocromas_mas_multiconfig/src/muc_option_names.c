/* #undef MAS_TRACING */
#define MAST_TRACE_CONFIG muc_get_cli_options_trace_config(cli)
/* #include "muc_tracen_defs_preset.h" */

#include <assert.h>                                                  /* assert */
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
/* #include <mastar/trace/mas_trace.h> */
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ♣ */

#include "muc_tmp_types.h"

#include "muc_option_config.h"                                       /* muc_get_cli_options_trace_config ♠ */
/* ###################################################################### */
#include "muc_option_names.h"
/* ###################################################################### */

char *
muc_coption_names_d( muc_config_cli_t * cli, muc_option_gen_code_t codeval, const char *delim )
{
  muc_longval_extended_vtable_t **xvtables;
  const muc_longval_extended_vtable_t *xtable;
  char *names = NULL;
  int cnt = 0;
  static const char *wrap[2] = { "〈", "〉" };

  xvtables = muc_cli_options_xvtable_multi( cli );

  while ( ( xtable = *xvtables++ ) )
  {
    const muc_longval_extended_t *xtended = xtable->xlist;

    while ( xtended->o.name )
    {
      if ( xtended->o.val == codeval )
      {
        const char *spaces = "                                                            ";
        size_t l;
        size_t mln = 14;

        if ( !cnt )
          names = mas_strcat_x( names, wrap[0] );
        else
          names = mas_strcat_x( names, delim ? delim : " | " );
        names = mas_strcat_x( names, "--" );
        names = mas_strcat_x( names, xtended->o.name );
        l = strlen( xtended->o.name );
        if ( l < mln )
          names = mas_strncat_x( names, spaces, mln - l );
        cnt++;
      }

      xtended++;
    }
  }
  if ( names && codeval && ( ( unsigned ) codeval ) < 0xff )
  {
    char sh[10] = "- ";

    sh[1] = codeval & 0xff;
    if ( !cnt )
      names = mas_strcat_x( names, wrap[1] );
    if ( cnt )
      names = mas_strcat_x( names, delim ? delim : " | " );
    names = mas_strcat_x( names, sh );
    cnt++;
  }
  if ( cnt )
    names = mas_strcat_x( names, wrap[1] );
  return names;
}

char *
muc_coption_names( muc_config_cli_t * cli, muc_option_gen_code_t codeval )
{
  return muc_coption_names_d( cli, codeval, NULL );
}

static muc_tmp_t *cnames_tmp = NULL;

const char *
muc_coption_cnames_tmp( muc_config_cli_t * cli, int index, muc_option_gen_code_t codeval, const char *delim )
{
  const char *x = NULL;

  if ( index < 0 )
  {
    index = cnames_tmp->tmp_index++;
    if ( cnames_tmp->tmp_index >= MUC_TMP_INDEX_MAX )
      cnames_tmp->tmp_index = 0;
  }

  if ( index >= 0 && index < MUC_TMP_INDEX_MAX )
  {
    mas_free( cnames_tmp->tmp_string[index] );
    cnames_tmp->tmp_string[index] = NULL;
    cnames_tmp->tmp_string[index] = muc_coption_names_d( cli, codeval, delim );
    x = cnames_tmp->tmp_string[index];
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
