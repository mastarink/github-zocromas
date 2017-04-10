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
#include "option_tablist.h"

#include "option_tablist_base.h"

/*
 *
 * mucs_config_option_tablist_...
 *
 * */

mucs_option_t *
mucs_config_aoptions_clone( const mucs_option_t * options, size_t count )
{
  mucs_option_t *options_clone = mas_calloc( count + 1, sizeof( mucs_option_t ) );

/* for ( const mucs_option_t * opt = tabnode->options; opt && opt->name; opt++ ) */
  for ( unsigned no = 0; no < count; no++ )
  {
    options_clone[no] = options[no];
    options_clone[no].name = mas_strdup( options[no].name );
  }
  return options_clone;
}

void
mucs_config_aoptions_reset( mucs_option_t * options, size_t count )
{
  for ( unsigned no = 0; no < count; no++ )
  {
    mas_free( options[no].name );
    options[no].name = NULL;
  }
}

void
mucs_config_aoptions_delete( mucs_option_t * options, size_t count )
{
  mucs_config_aoptions_reset( options, count );
  mas_free( options );
}

void
mucs_config_option_tabnode_init( mucs_option_table_list_t * tablist )
{
  memset( tablist, 0, sizeof( mucs_option_table_list_t ) );
}

mucs_option_table_list_t *
mucs_config_option_tabnode_create( void )
{
  mucs_option_table_list_t *tablist = mas_calloc( 1, sizeof( mucs_option_table_list_t ) );

  mucs_config_option_tabnode_init( tablist );
  tablist->allocated = 1;
  return tablist;
}

mucs_option_table_list_t *
mucs_config_option_tabnode_add( mucs_option_table_list_t * tablist, const char *name, const mucs_option_t * options, size_t count )
{
  mucs_option_table_list_t *tbnew = mucs_config_option_tabnode_create(  );

  if ( tbnew )
  {
    if ( !count )
    {
      for ( const mucs_option_t * o = options; o && o->name && !mucs_config_option_flag( o, MUCS_FLAG_LAST_IN_TABLE ); o++ )
        count++;
    /* WARN( "COUNT:%ld", ( long ) count ); */
    }
    if ( tablist )
    {
      mucs_option_table_list_t *tb = tablist;

      while ( tb->next )
        tb = tb->next;
      tb->next = tbnew;
    }
    else
    {
      tablist = tbnew;
    }
    {
      if ( tbnew->allocated )
      {
        tbnew->name = mas_strdup( name );
        tbnew->voptions = mucs_config_aoptions_clone( options, count );
      }
      else /* really never happens !? */
        tbnew->coptions = options;
      tbnew->count = count;
    }
  }
  return tablist;
}

void
mucs_config_option_tabnode_reset( mucs_option_table_list_t * tabnode )
{
  if ( tabnode )
  {
  /* TODO: tbnew->count */
    for ( const mucs_option_t * opt = mucs_config_option_tabnode_aoptions( tabnode ); opt && opt->name; opt++ )
    {
      if ( opt && opt->cust_ptr && ( mucs_config_option_flag( opt, MUCS_FLAG_AUTOFREE ) ) )
      {
        switch ( opt->restype & ~MUCS_RTYP_FLAG_ALL )
        {
        case MUCS_RTYP_STRING:
          mas_free( *( ( char ** ) opt->cust_ptr ) );
          break;
        case MUCS_RTYP_TARG:
          mas_argvc_delete( ( mas_argvc_t * ) opt->cust_ptr );
          break;
        default:
          break;
        }
      }
    }
    if ( tabnode->allocated && tabnode->name )
    {
      mucs_config_aoptions_delete( tabnode->voptions, tabnode->count );
      mas_free( tabnode->name );
      tabnode->name = NULL;
    }
  }
}

void
mucs_config_option_tabnode_delete( mucs_option_table_list_t * tabnode )
{
  if ( tabnode )
  {
    mucs_config_option_tabnode_reset( tabnode );
    mas_free( tabnode );
  }
}

void
mucs_config_option_tablist_reset( mucs_option_table_list_t * tablist )
{
  while ( tablist )
  {
    mucs_option_table_list_t *t = tablist;

    tablist = tablist->next;
    mucs_config_option_tabnode_reset( t );
  }
}

void
mucs_config_option_tablist_delete( mucs_option_table_list_t * tablist )
{
  while ( tablist )
  {
    mucs_option_table_list_t *t = tablist;

    tablist = tablist->next;
    mucs_config_option_tabnode_delete( t );
  }
}
