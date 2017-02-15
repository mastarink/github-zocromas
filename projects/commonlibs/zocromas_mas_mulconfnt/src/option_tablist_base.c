#include <string.h>
#include <math.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_argvc_tools.h>

#include "mulconfnt_defs.h"
#include "mulconfnt_structs.h"

#include "mulconfnt_error.h"

#include "option.h"

#include "option_tablist_base.h"

void
mulconfnt_config_option_tablist_init( config_option_table_list_t * tablist )
{
  memset( tablist, 0, sizeof( config_option_table_list_t ) );
}

config_option_table_list_t *
mulconfnt_config_option_tablist_create( void )
{
  config_option_table_list_t *tablist = mas_malloc( sizeof( config_option_table_list_t ) );

  mulconfnt_config_option_tablist_init( tablist );
  return tablist;
}

config_option_table_list_t *
mulconfnt_config_option_tablist_add( config_option_table_list_t * tablist, const char *name, config_option_t * options, size_t count )
{
  if ( tablist )
  {
    config_option_table_list_t *tb = tablist;

    while ( tb->next )
      tb = tb->next;
    tb->next = mulconfnt_config_option_tablist_create(  );
    tb->name = mas_strdup( name );
    tb->options = options;
    tb->count = count;
  }
  else
  {
    tablist = mulconfnt_config_option_tablist_create(  );
    tablist->name = mas_strdup( name );
    tablist->options = options;
    tablist->count = count;
  }
  return tablist;
}

void
mulconfnt_config_option_tablist_close( config_option_table_list_t * tablist )
{
  while ( tablist )
  {
    for ( config_option_t * opt = tablist->options; opt->name; opt++ )
    {
      if ( opt && opt->ptr && ( opt->restype & MULCONF_RTYP_FLAG_AUTOFREE ) )
      {
        switch ( opt->restype & ~MULCONF_RTYP_FLAG_ALL )
        {
        case MULCONF_RTYP_STRING:
          mas_free( *( ( char ** ) opt->ptr ) );
          break;
        case MULCONF_RTYP_TARG:
          mas_argvc_delete( ( mas_argvc_t * ) opt->ptr );
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
mulconfnt_config_option_tablist_delete( config_option_table_list_t * tablist )
{
  mulconfnt_config_option_tablist_close( tablist );
  mas_free( tablist );
}
