#define R_GOOD(_r) ((_r)>=0)
#include "mulconfnt_defs.h"
#include <string.h>
#include <math.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_argvc_tools.h>
#include <mastar/minierr/minierr.h>

#include "mulconfnt_structs.h"

/* #include "mulconfnt_error.h" */

#include "option.h"

#include "option_tablist_base.h"

/*
 *
 * mucs_config_option_tablist_...
 *
 * */

void
mucs_config_option_tablist_init( mucs_option_table_list_t * tablist )
{
  memset( tablist, 0, sizeof( mucs_option_table_list_t ) );
}

mucs_option_table_list_t *
mucs_config_option_tablist_create( void )
{
  mucs_option_table_list_t *tablist = mas_calloc( 1, sizeof( mucs_option_table_list_t ) );

  mucs_config_option_tablist_init( tablist );
  return tablist;
}

mucs_option_table_list_t *
mucs_config_option_tablist_add( mucs_option_table_list_t * tablist, const char *name, const mucs_option_t * options, size_t count )
{
  if ( tablist )
  {
    mucs_option_table_list_t *tb = tablist;

    while ( tb->next )
      tb = tb->next;
    tb->next = mucs_config_option_tablist_create(  );
    tb->name = mas_strdup( name );
    tb->options = options;
    if ( !count )
    {
      for ( const mucs_option_t * o = options; o->name && !mucs_config_option_flag( o, MUCS_FLAG_LAST_IN_TABLE ); o++ )
        count++;
      WARN( "COUNT:%ld", ( long ) count );
    }
    tb->count = count;
  }
  else
  {
    tablist = mucs_config_option_tablist_create(  );
    tablist->name = mas_strdup( name );
    tablist->options = options;
    tablist->count = count;
  }
  return tablist;
}

void
mucs_config_option_tablist_reset( mucs_option_table_list_t * tablist )
{
  while ( tablist )
  {
    for ( const mucs_option_t * opt = tablist->options; opt->name; opt++ )
    {
      if ( opt && opt->argptr && ( mucs_config_option_flag( opt, MUCS_FLAG_AUTOFREE ) ) )
      {
        switch ( opt->restype & ~MUCS_RTYP_FLAG_ALL )
        {
        case MUCS_RTYP_STRING:
          mas_free( *( ( char ** ) opt->argptr ) );
          break;
        case MUCS_RTYP_TARG:
          mas_argvc_delete( ( mas_argvc_t * ) opt->argptr );
          break;
        default:
          break;
        }
      }
    }
    tablist = tablist->next;
  }
}

void
mucs_config_option_tablist_delete( mucs_option_table_list_t * tablist )
{
  mucs_config_option_tablist_reset( tablist );
  mas_free( tablist->name );
  {
    mucs_option_table_list_t *tb = tablist;

    while ( tb )
    {
      mucs_option_table_list_t *t = tb;

      tb = tb->next;
      mas_free( t );
    }
  }
}
