#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_options.h"

#include "duf_status.h"

#include "duf_option_find.h"
#include "duf_option_cfind.h"
#include "duf_option_lfind.h"

#include "duf_option_descr.h"

#include "duf_option_typed.h"

#include "duf_option_stage.h"
#include "duf_option_source.h"



#include "duf_xtended_table.h"
#include "duf_option_clarify.h"

/* ###################################################################### */
#include "duf_option_clarify_new.h"
/* ###################################################################### */

SR( OPTIONS, split_string_od, duf_option_data_t * pod DUF_UNUSED )
{
  char *barg = NULL;
  char *endn = NULL;
  char *name = NULL;
  char *arg = NULL;
  char *string;

  string = mas_strdup( pod->string );
  pod->has_arg = 0;
  switch ( pod->vseparator )
  {
  case ' ':
    barg = endn = strpbrk( string, "\t\r\n " );
    while ( barg && *barg && strchr( "\t\r\n ", *barg ) )
      barg++;
    break;
  case 0:
    barg = endn = strpbrk( string, "\t\r\n =" );
    while ( barg && *barg && strchr( "\t\r\n =", *barg ) )
    {
      pod->has_arg = ( *barg == '=' );
      barg++;
    }
    break;
  default:
    barg = endn = strchr( string, '=' );
    if ( barg )
    {
      pod->has_arg = 1;
      barg++;
    }
    break;
  }
#if 0                           /* 20160113.153243 */
  if ( endn )
    name = mas_strndup( string, endn - string );
  else
    name = mas_strdup( string );
#else
  name = endn ? mas_strndup( string, endn - string ) : mas_strdup( string );
#endif
  if ( barg )
  {
    /* arg = mas_strdup( barg ); */
    /* T( ">>>> barg:'%s'", barg ); */
    arg = duf_string_options_expand( barg, '?' );
    /* T( ">> arg:'%s' => %s", barg, arg ); */
  }
  pod->has_arg = ( ( pod->has_arg && arg ) || ( arg && *arg ) );
  pod->name = name;
  pod->optarg = arg;
  /* T( "@0$$$$ n:%s; a:%s(%d:%d); s:%s -- (has_arg:%d)", pod->name, pod->optarg ? pod->optarg : "", pod->optarg ? 1 : 0, */
  /*    ( pod->optarg && *( pod->optarg ) ) ? 1 : 0, pod->string, pod->has_arg );                                         */
  mas_free( string );
  ER( OPTIONS, split_string_od, duf_option_data_t * pod );
}

SR( OPTIONS, soption_xclarify_new_register, const duf_longval_extended_t * xtended, int soft, duf_option_data_t * pod )
{
  size_t alloc_step = 10;
  size_t index = 0;

  /* T( "$$$$ SOFT:%d; n:%s; a:%s; s:%s -- o.n:%s (has_arg:%d) from %s", soft, pod->name, pod->optarg, pod->string, xtended->o.name, pod->has_arg, */
  /*    duf_optsource_name( pod->source ) );                                                                                                       */

  index = pod->xfound.count_hard + pod->xfound.count_soft;
  if ( pod->xfound.allocated_size == 0 )
  {
    assert( pod->xfound.array == NULL );
    pod->xfound.array = mas_malloc( alloc_step * sizeof( pod->xfound.array[0] ) );

    memset( pod->xfound.array + pod->xfound.allocated_size * sizeof( pod->xfound.array[0] ), 0, alloc_step * sizeof( pod->xfound.array[0] ) );
    pod->xfound.allocated_size += alloc_step;
  }
  else if ( pod->xfound.allocated_size == index )
  {
    assert( pod->xfound.array );
    pod->xfound.array = mas_realloc( pod->xfound.array, ( pod->xfound.allocated_size + alloc_step ) * sizeof( pod->xfound.array[0] ) );

    memset( pod->xfound.array + pod->xfound.allocated_size * sizeof( pod->xfound.array[0] ), 0, alloc_step * sizeof( pod->xfound.array[0] ) );
    pod->xfound.allocated_size += alloc_step;
  }
  pod->xfound.array[index].xtended = xtended;
  pod->xfound.array[index].soft = soft;
  pod->xfound.array[index].noo = pod->noo;
  pod->xfound.array[index].xtable = pod->xtable;

  if ( soft )
  {
    pod->xfound.count_soft++;
    pod->xfound.soft_index = index;
  }
  else
  {
    pod->xfound.count_hard++;
    pod->xfound.hard_index = index;
  }
  ER( OPTIONS, soption_xclarify_new_register, const duf_longval_extended_t * xtended, int soft, duf_option_data_t * pod );
}

SR( OPTIONS, soption_xclarify_new_at_xarr_od, const duf_longval_extended_t * xarr, duf_option_data_t * pod )
{
  for ( const duf_longval_extended_t * xtended = xarr; xtended->o.name; xtended++ )
  {
    size_t len_ask, len_tb;
    const char *match = NULL;
    int is_no;
    int arg_error;

    is_no = ( 0 == strncmp( pod->name, "no-", 3 ) );
    assert( ( xtended->can_no && xtended->o.has_arg == no_argument ) || ( !xtended->can_no ) );
    if ( xtended->can_no && is_no )
    {
      pod->noo = 1;
      pod->name_offset = 3;
      /* T( "@0a name_offset:%lu - %s ? %s", pod->name_offset, pod->name,  xtended->o.name ); */
    }
    else
    {
      pod->noo = 0;
      pod->name_offset = 0;
      /* T( "@0b name_offset:%lu - %s ? %s", pod->name_offset, pod->name,  xtended->o.name ); */
    }
    if ( xtended->invert )
      pod->noo = !pod->noo;
    len_ask = strlen( pod->name ) - pod->name_offset;
    len_tb = strlen( xtended->o.name );
    match = pod->name + pod->name_offset;

    if ( xtended->o.has_arg == no_argument && pod->has_arg )
    {
    }
    else if ( xtended->o.has_arg == optional_argument )
    {
    }
    else if ( xtended->o.has_arg == required_argument && !pod->has_arg )
    {
    }
    arg_error = ( xtended->o.has_arg == no_argument && pod->has_arg ) || ( xtended->o.has_arg == required_argument && !pod->has_arg );

    if ( !arg_error )
    {
      if ( 0 == strcmp( match, xtended->o.name ) )
      {
        /* T( "@1 name_offset:%lu - '%s' (%s|%s) noo:%d", pod->name_offset, pod->name, match, xtended->o.name, pod->noo ); */
        assert( len_tb == len_ask );
        CR( soption_xclarify_new_register, xtended, 0 /* soft */ , pod );
      }
      else if ( 0 == strncmp( match, xtended->o.name, len_ask ) )
      {
        /* T( "@2 name_offset:%lu - '%s' (%s|%s) noo:%d", pod->name_offset, pod->name, match, xtended->o.name, pod->noo ); */
        assert( len_tb > len_ask );
        CR( soption_xclarify_new_register, xtended, len_tb - len_ask /* soft */ , pod );
      }
    }
    /* T( "@3 name_offset:%lu - '%s' (%s|%s) noo:%d; %d; %d", pod->name_offset, pod->name, match, xtended->o.name, pod->noo, arg_error, pod->has_arg ); */
  }

  ER( OPTIONS, soption_xclarify_new_at_xarr_od, const duf_longval_extended_t * xarr, duf_option_data_t * pod );
}

SR( OPTIONS, soption_xclarify_new_at_xtable_od, const duf_longval_extended_table_t * xtable, duf_option_data_t * pod )
{
  pod->xtable = xtable;
  CR( soption_xclarify_new_at_xarr_od, xtable->table, pod );

  ER( OPTIONS, soption_xclarify_new_at_xtable_od, const duf_longval_extended_table_t * xtable, duf_option_data_t * pod );
}

SR( OPTIONS, soption_xclarify_new_at_multix_od, const duf_longval_extended_table_t ** xtables, duf_option_data_t * pod )
{
  int doindex = -1;

  for ( ; *xtables; xtables++ )
  {
    const duf_longval_extended_table_t *xtable = *xtables;

    CR( soption_xclarify_new_at_xtable_od, xtable, pod );
/* if not found : clear error */
  }
  if ( pod->xfound.count_hard == 1 )
  {
    if ( pod->xfound.array[pod->xfound.hard_index].soft == 0
         && DUF_OPTION_CHECK_STAGE( pod->stage, pod->xfound.array[pod->xfound.hard_index].xtended, pod->xfound.array[0].xtable ) )
    {
      doindex = pod->xfound.hard_index;
    }
  }
  else if ( pod->xfound.count_hard == 0 )
  {
    if ( pod->xfound.count_soft == 1 )
    {
      if ( pod->xfound.array[pod->xfound.soft_index].soft > 0
           && DUF_OPTION_CHECK_STAGE( pod->stage, pod->xfound.array[pod->xfound.hard_index].xtended, pod->xfound.array[0].xtable ) )
      {
        doindex = pod->xfound.hard_index;
      }
    }
    else if ( pod->xfound.count_soft > 1 )
    {
      /* MULTI-ERROR : may show list to choose */
      /* T( "@ERR %s", pod->string ); */
      SERRM( OPTION_NEW_MULTIPLE, "'--%s' '--%s' (from %s)", pod->string, pod->name, duf_optsource_name( pod->source ) );
    }
    else                        /* if ( pod->xfound.count_soft < 1 ) */
    {
      /* NOT-FOUND-ERROR */
      /* T( "@ERR %s", pod->string ); */
      SERRM( OPTION_NEW_NOT_FOUND, "'--%s' '--%s' (from %s)", pod->string, pod->name, duf_optsource_name( pod->source ) );
    }
  }
  else
  {
    /* BAD-MULTI-ERROR : same command defined, needs fixing tables */
    /* T( "@ERR %s", pod->string ); */
    SERRM( OPTION_NEW_DUPLICATE, "'--%s' '--%s' (from %s)", pod->string, pod->name, duf_optsource_name( pod->source ) );
  }
  if ( QNOERR && doindex >= 0 && pod->clarifier )
  {
    /* T( "@Do it : (%c)%s", pod->xfound.array[doindex].noo ? '-' : '+', pod->xfound.array[doindex].xtended->o.name ); */
    {
      char *oa;

      oa = duf_string_options_expand( pod->optarg, '?' );
      CRV( ( pod->clarifier ), pod->xfound.array[doindex].xtended, oa, pod->stage, pod->xfound.array[doindex].xtable, pod->noo, pod->source );
      mas_free( oa );
    }
  }
#if 0
  T( "@-------- %lu:%lu ----------", pod->xfound.count_hard, pod->xfound.count_soft );
  for ( size_t n = 0; n < pod->xfound.count_hard + pod->xfound.count_soft; n++ )
  {
    T( "@@ ========== s:%d; ch:%lu; cs:%lu; doi:%d {%d} --%s='%s' =======================", pod->xfound.array[n].soft, pod->xfound.count_hard,
       pod->xfound.count_soft, doindex, pod->clarifier ? 1 : 0, pod->xfound.array[n].xtended->o.name, pod->optarg );
  }
#endif
  mas_free( pod->xfound.array );
  pod->xfound.array = NULL;
  /* mas_free( pod->name );   */
  /* pod->name = NULL;        */
  /* mas_free( pod->optarg ); */
  /* pod->optarg = NULL;      */
  TR( QERRIND );

  ER( OPTIONS, soption_xclarify_new_at_multix_od, const duf_longval_extended_table_t ** xtables, duf_option_data_t * pod );
}

SR( OPTIONS, soption_xclarify_new_at_stdx_od, duf_option_data_t * pod )
{
  CR( soption_xclarify_new_at_multix_od, duf_extended_table_multi(  ), pod );

  ER( OPTIONS, soption_xclarify_new_at_stdx_od, duf_option_data_t * pod );
}

SR( OPTIONS, soption_xclarify_new_at_stdx, const char *string, const char *name, const char *arg, duf_xclarifier_t clarifier, char vseparator,
    duf_option_stage_t istage, duf_option_source_t source )
{
  duf_option_data_t od = { 0 };
  od.string = string;
  od.name = mas_strdup( name );
  od.optarg = mas_strdup( arg );
  od.vseparator = vseparator;
  od.clarifier = clarifier;
  od.stage = istage;
  od.source = source;

  if ( string && !od.name )
    CR( split_string_od, &od );
  else
    od.has_arg = od.optarg ? 1 : 0;
  /* T( "@1$$$$ n:%s; a:%s; s:%s -- (has_arg:%d) from %s", od.name, od.optarg ? od.optarg : "", od.string, od.has_arg, duf_optsource_name( od.source ) ); */
  CR( soption_xclarify_new_at_stdx_od, &od );
  /* T( "@2$$$$ n:%s; a:%s; s:%s -- (has_arg:%d) from %s", od.name, od.optarg ? od.optarg : "", od.string, od.has_arg, duf_optsource_name( od.source ) ); */

  mas_free( od.xfound.array );
  od.xfound.array = NULL;
  mas_free( od.name );
  od.name = NULL;
  mas_free( od.optarg );
  od.optarg = NULL;

  ER( OPTIONS, soption_xclarify_new_at_stdx, const char *string, const char *name, const char *arg, duf_xclarifier_t clarifier, char vseparator,
      duf_option_stage_t istage, duf_option_source_t source );
}

SR( OPTIONS, soption_xclarify_new_at_stdx_default, const char *string, const char *name, const char *arg, char vseparator, duf_option_stage_t istage,
    duf_option_source_t source )
{
  CR( soption_xclarify_new_at_stdx, string, name, arg, DUF_WRAPPED( duf_xoption_clarify ), vseparator, istage, source );

  ER( OPTIONS, soption_xclarify_new_at_stdx_default, const char *string, const char *name, const char *arg, char vseparator, duf_option_stage_t istage,
      duf_option_source_t source );
}

/* for batch:                        */
/* delim = duf_option_delimiter(  ); */
