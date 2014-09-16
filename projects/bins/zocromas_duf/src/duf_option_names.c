#include <assert.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"


#include "duf_option_extended.h"

/* ###################################################################### */
#include "duf_option_names.h"
/* ###################################################################### */





char *
duf_cli_option_shorts( void )
{
  char shorts[512] = "";
  char *p = shorts;

/* 
 * 20140904.120533 : changed duf_config->longopts_table[ilong] => lo_extended[ilong].o
 * */
  for ( int ilong = 0; lo_extended[ilong].o.name && ilong < lo_extended_count; ilong++ )
  {
    if ( lo_extended[ilong].o.val < 0xFF )
    {
      /* DUF_SHOW_ERROR( "S:%c %x - %s", duf_longopts[ilong].val, duf_longopts[ilong].val, shorts ); */
      if ( !strchr( shorts, ( char ) lo_extended[ilong].o.val ) )
      {
        *p++ = ( char ) lo_extended[ilong].o.val;
        if ( lo_extended[ilong].o.has_arg == no_argument );
        else if ( lo_extended[ilong].o.has_arg == required_argument )
          *p++ = ':';
        else if ( lo_extended[ilong].o.has_arg == optional_argument )
        {
          /* *p++ = ':'; */
          /* *p++ = ':'; */
        }
        else
        {
          *p++ = ':';
          *p++ = ':';
        }
      }
      *p = 0;
    }
  }
  DUF_TRACE( explain, 0, "genereated options shorts: %s", shorts );
  return *shorts ? mas_strdup( shorts ) : NULL;
}

char *
duf_option_names_d( duf_option_code_t codeval, const char *delim )
{
  char *names = NULL;
  int cnt = 0;

  for ( int ilong = 0; duf_config->longopts_table[ilong].name && ilong < lo_extended_count; ilong++ )
  {
    if ( duf_config->longopts_table[ilong].val == codeval )
    {
      const char *spaces = "                                                            ";
      size_t l;
      size_t mln = 14;

      if ( !cnt )
        names = mas_strcat_x( names, "≪" );
      else
        names = mas_strcat_x( names, delim ? delim : " | " );
      names = mas_strcat_x( names, "--" );
      names = mas_strcat_x( names, duf_config->longopts_table[ilong].name );
      l = strlen( duf_config->longopts_table[ilong].name );
      if ( l < mln )
        names = mas_strncat_x( names, spaces, mln - l );
      cnt++;
    }
  }
  if ( names && codeval && ( ( unsigned ) codeval ) < 0xff )
  {
    char sh[10] = "- ";

    sh[1] = codeval & 0xff;
    if ( !cnt )
      names = mas_strcat_x( names, "≫" );
    if ( cnt )
      names = mas_strcat_x( names, delim ? delim : " | " );
    names = mas_strcat_x( names, sh );
    cnt++;
  }
  if ( cnt )
    names = mas_strcat_x( names, "≫" );
  return names;
}

char *
duf_option_names( duf_option_code_t codeval )
{
  return duf_option_names_d( codeval, NULL );
}

const char *
duf_option_cnames_tmp( int index, duf_option_code_t codeval, const char *delim )
{
  const char *x = NULL;

  if ( duf_config )
  {
    if ( index < 0 )
    {
      index = duf_config->tmp->explanation_index++;
      if ( duf_config->tmp->explanation_index >= DUF_TMP_EXPLANATION_MAX )
        duf_config->tmp->explanation_index = 0;
    }

    if ( index >= 0 && index < DUF_TMP_EXPLANATION_MAX )
    {
      mas_free( duf_config->tmp->option_explanation[index] );
      duf_config->tmp->option_explanation[index] = NULL;
      duf_config->tmp->option_explanation[index] = duf_option_names_d( codeval, delim );
      x = duf_config->tmp->option_explanation[index];
    }
  }
  return x;
}
