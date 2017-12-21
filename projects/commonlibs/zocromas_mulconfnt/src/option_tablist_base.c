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
#include "aoptions.h"

#include "option_tablist_base.h"

/*
 *
 * mucs_config_option_tablist_...
 *
 * */

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

/* mucs_option_table_list_t *                                                                                                          */
/* mucs_config_option_tabnode_add( mucs_option_table_list_t * tablist, const char *name, const mucs_option_t * options, size_t optcount ) */
/* {                                                                                                                                   */
/*   mucs_option_table_list_t *tbnew = mucs_config_option_tabnode_create(  );                                                          */
/*                                                                                                                                     */
/*   if ( tbnew )                                                                                                                      */
/*   {                                                                                                                                 */
/*     if ( !optcount )                                                                                                                   */
/*     {                                                                                                                               */
/*       for ( const mucs_option_t * o = options; o && o->s.name && !mucs_config_option_flag( o, MUCS_FLAG_LAST_IN_TABLE ); o++ )      */
/*         optcount++;                                                                                                                    */
/*     (* WARN( "COUNT:%ld", ( long ) optcount ); *)                                                                                      */
/*     }                                                                                                                               */
/*     if ( tablist )                                                                                                                  */
/*     {                                                                                                                               */
/*       mucs_option_table_list_t *tb = tablist;                                                                                       */
/*                                                                                                                                     */
/*       while ( tb->next )                                                                                                            */
/*         tb = tb->next;                                                                                                              */
/*       tb->next = tbnew;                                                                                                             */
/*     }                                                                                                                               */
/*     else                                                                                                                            */
/*     {                                                                                                                               */
/*       tablist = tbnew;                                                                                                              */
/*     }                                                                                                                               */
/*     {                                                                                                                               */
/*       if ( tbnew->allocated )                                                                                                       */
/*       {                                                                                                                             */
/*         tbnew->name = mas_strdup( name );                                                                                           */
/*         tbnew->voptions = mucs_config_aoptions_clone( options, optcount );                                                             */
/*       }                                                                                                                             */
/*       else                                                           (* really never happens !? *)                                  */
/*         tbnew->coptions = options;                                                                                                  */
/*       tbnew->optcount = optcount;                                                                                                         */
/*     }                                                                                                                               */
/*   }                                                                                                                                 */
/*   return tablist;                                                                                                                   */
/* }                                                                                                                                   */

mucs_option_table_list_t *
mucs_config_soption_tabnode_add( mucs_option_table_list_t * tablist, const char *name, const mucs_option_static_t * soptions, size_t optcount )
{
  mucs_option_table_list_t *tbnew = mucs_config_option_tabnode_create(  );

  if ( tbnew )
  {
    if ( !optcount )
    {
      for ( const mucs_option_static_t * sopt = soptions; sopt && sopt->name && !mucs_config_soption_flag( sopt, MUCS_FLAG_LAST_IN_TABLE ); sopt++ )
        optcount++;
    /* WARN( "COUNT:%ld", ( long ) optcount ); */
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
        tbnew->voptions = mucs_config_soptions2options( soptions, optcount );
      }
      else                                                           /* really never happens !? */
      {
        QRGM( -1, "wrong call" );
      /* optscan->errors |= MUCS_ERROR_WRONG_CALL; */
      }
      tbnew->optcount = optcount;
    }
  }
  return tablist;
}

void
mucs_config_option_tabnode_reset_cust( mucs_option_table_list_t * tabnode, int fautofree )
{
  if ( tabnode )
  {
    const mucs_option_t *aoptions = mucs_config_option_tabnode_aoptions( tabnode );

    for ( unsigned no = 0; no < tabnode->optcount; no++ )
    {
      const mucs_option_t *opt = aoptions + no;

      if ( opt && opt->s.cust_ptr && ( fautofree && mucs_config_option_flag( opt, MUCS_FLAG_AUTOFREE ) ) )
      {
        switch ( opt->s.restype & ~MUCS_RTYP_FLAG_ALL )
        {
        case MUCS_RTYP_STRING:
          mas_free( *( ( char ** ) opt->s.cust_ptr ) );
          *( ( char ** ) opt->s.cust_ptr ) = NULL;
          break;
        case MUCS_RTYP_TARG:
          mas_argvc_delete( ( mas_argvc_t * ) opt->s.cust_ptr );
          break;
        default:
          break;
        }
      }
    }
  }
}

void
mucs_config_option_tabnode_reset( mucs_option_table_list_t * tabnode, int fautofree )
{
  mucs_config_option_tabnode_reset_cust( tabnode, fautofree );
  if ( tabnode )
  {
    if ( tabnode->allocated && tabnode->name )
    {
      mucs_config_aoptions_delete( tabnode->voptions, tabnode->optcount );
      mas_free( tabnode->name );
      tabnode->name = NULL;
    }
  }
}

void
mucs_config_option_tabnode_delete( mucs_option_table_list_t * tabnode, int fautofree )
{
  if ( tabnode )
  {
    mucs_config_option_tabnode_reset( tabnode, fautofree );
    mas_free( tabnode );
  }
}

void
mucs_config_option_tablist_reset_cust( mucs_option_table_list_t * tablist, int fautofree )
{
  while ( tablist )
  {
    mucs_option_table_list_t *t = tablist;

    tablist = tablist->next;
    mucs_config_option_tabnode_reset_cust( t, fautofree );
  }
}

void
mucs_config_option_tablist_reset( mucs_option_table_list_t * tablist, int fautofree )
{
  while ( tablist )
  {
    mucs_option_table_list_t *t = tablist;

    tablist = tablist->next;
    mucs_config_option_tabnode_reset( t, fautofree );
  }
}

void
mucs_config_option_tablist_delete( mucs_option_table_list_t * tablist, int fautofree )
{
  while ( tablist )
  {
    mucs_option_table_list_t *t = tablist;

    tablist = tablist->next;
    mucs_config_option_tabnode_delete( t, fautofree );
  }
}
