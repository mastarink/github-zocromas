/* #undef MAS_TRACING */
#define MAST_TRACE_CONFIG duf_get_cli_options_trace_config(cli)
#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_z.h"
/* #include "duf_maintenance_tracen.h" */
/* #include "duf_maintenance_errors.h" */

#include "duf_config_util.h"

#include "duf_tmp_types.h"

#include "duf_option_config.h"
/* ###################################################################### */
#include "duf_option_names.h"
/* ###################################################################### */

char *
duf_coption_names_d( duf_config_cli_t * cli, duf_option_code_t codeval, const char *delim )
{
  duf_longval_extended_vtable_t **xvtables;
  const duf_longval_extended_vtable_t *xtable;
  char *names = NULL;
  int cnt = 0;
  static const char *wrap[2] = { "〈", "〉" };

#if 0
  xvtables = duf_cli_options_config(  )->xvtable_multi;
#else
  xvtables = duf_cli_options_xvtable_multi( cli );
#endif

  while ( ( xtable = *xvtables++ ) )
  {
    const duf_longval_extended_t *xtended = xtable->xlist;

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

#if 0
char *
duf_coption_names( duf_option_code_t codeval )
{
  return duf_coption_names_d( codeval, NULL );
}
#endif

static duf_tmp_t *cnames_tmp = NULL;

const char *
duf_coption_cnames_tmp( duf_config_cli_t * cli, int index, duf_option_code_t codeval, const char *delim )
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
    cnames_tmp->tmp_string[index] = duf_coption_names_d( cli, codeval, delim );
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
