#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_options.h"

#include "duf_config.h"
#include "duf_config_trace.h"

#include "duf_option_types.h"

#include "duf_expandable.h"

#include "duf_option_typed.h"

#include "duf_option_stage.h"
#include "duf_option_source.h"


#include "duf_xtended_table.h"
#include "duf_option_clarify.h"

/* ###################################################################### */
#include "duf_option_clarify_new.h"
/* ###################################################################### */


SR( OPTIONS, split_string_od, duf_option_data_t * pod )
{
  const char *barg = NULL;
  const char *endn = NULL;
  char *name = NULL;
  char *arg = NULL;
  char *string;

  string = mas_strdup( pod->string_copy );
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
  /*    ( pod->optarg && *( pod->optarg ) ) ? 1 : 0, pod->string_copy, pod->has_arg );                                         */
  for ( unsigned i = 0; i < strlen( string ); i++ )
    string[i] = '@';
  mas_free( string );
  ER( OPTIONS, split_string_od, duf_option_data_t * pod );
}

SR( OPTIONS, soption_xclarify_new_register, const duf_longval_extended_t * xtended, int soft, duf_option_data_t * pod )
{
  size_t alloc_step = 10;
  size_t index = 0;

  /* T( "@@i name_offset:%lu - '%s' (%s) noo:%c", pod->name_offset, pod->name, xtended->o.name, pod->noo ? '+' : '-' ); */
  /* T( "$$$$ SOFT:%d; n:%s; a:%s; s:%s -- o.n:%s (has_arg:%d) from %s", soft, pod->name, pod->optarg, pod->string_copy, xtended->o.name, pod->has_arg, */
  /*    duf_optsource_name( pod->source ) );                                                                                                       */

  index = pod->xfound.count_hard + pod->xfound.count_soft;
  if ( pod->xfound.allocated_size == 0 )
  {
    assert( pod->xfound.xarray == NULL );
    pod->xfound.xarray = mas_malloc( alloc_step * sizeof( pod->xfound.xarray[0] ) );

    memset( pod->xfound.xarray + pod->xfound.allocated_size * sizeof( pod->xfound.xarray[0] ), 0, alloc_step * sizeof( pod->xfound.xarray[0] ) );
    pod->xfound.allocated_size += alloc_step;
  }
  else if ( pod->xfound.allocated_size == index )
  {
    assert( pod->xfound.xarray );
    pod->xfound.xarray = mas_realloc( pod->xfound.xarray, ( pod->xfound.allocated_size + alloc_step ) * sizeof( pod->xfound.xarray[0] ) );

    memset( pod->xfound.xarray + pod->xfound.allocated_size * sizeof( pod->xfound.xarray[0] ), 0, alloc_step * sizeof( pod->xfound.xarray[0] ) );
    pod->xfound.allocated_size += alloc_step;
  }
  pod->xfound.xarray[index].xtended = xtended;
  pod->xfound.xarray[index].soft = soft;
  /* T( "@@@(%c%c)%s", pod->xfound.xarray[pod->doindex].noo ? '+' : '-', pod->noo ? '+' : '-', xtended->o.name ); */
  pod->xfound.xarray[index].noo = pod->noo;
  /* T( "@@(%c%c)%s", pod->xfound.xarray[pod->doindex].noo ? '+' : '-', pod->noo ? '+' : '-', xtended->o.name ); */
  pod->xfound.xarray[index].xtable = pod->xtable;

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
    /* 
     * invert works at duf_option_typed_gen.c / DUF_OPTION_VTYPE_FLAG; not here:

       if ( xtended->invert )
         pod->noo = !pod->noo;
     */
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
        /* T( "@@1 name_offset:%lu - '%s' (%s|%s) noo:%c", pod->name_offset, pod->name, match, xtended->o.name, pod->noo ? '+' : '-' ); */
        assert( len_tb == len_ask );
        CR( soption_xclarify_new_register, xtended, 0 /* soft */ , pod );
      }
      else if ( 0 == strncmp( match, xtended->o.name, len_ask ) )
      {
        /* T( "@@2 name_offset:%lu - '%s' (%s|%s) noo:%c", pod->name_offset, pod->name, match, xtended->o.name, pod->noo ? '+' : '-' ); */
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
  /* int doindex = -1; */

  pod->doindex = -1;
  for ( ; *xtables; xtables++ )
  {
    const duf_longval_extended_table_t *xtable = *xtables;

    CR( soption_xclarify_new_at_xtable_od, xtable, pod );
/* if not found : clear error */
  }
  if ( pod->xfound.count_hard == 1 )
  {
    if ( pod->xfound.xarray[pod->xfound.hard_index].soft == 0
         && DUF_OPTION_CHECK_STAGE( pod->stage, pod->xfound.xarray[pod->xfound.hard_index].xtended, pod->xfound.xarray[0].xtable ) )
    {
      pod->doindex = pod->xfound.hard_index;
      /* T( "@@name_offset:%lu - (%s|%s) noo:%c%c", pod->name_offset, pod->name, pod->xfound.xarray[pod->doindex].xtended->o.name, */
      /*    pod->xfound.xarray[pod->doindex].noo ? '+' : '-', pod->noo ? '+' : '-' );                                              */
    }
  }
  else if ( pod->xfound.count_hard == 0 )
  {
    if ( pod->xfound.count_soft == 1 )
    {
      /* T( "@@name_offset:%lu - (%s) noo:%c", pod->name_offset, pod->name, pod->noo ? '+' : '-' ); */
      if ( pod->xfound.xarray[pod->xfound.soft_index].soft > 0
           && DUF_OPTION_CHECK_STAGE( pod->stage, pod->xfound.xarray[pod->xfound.hard_index].xtended, pod->xfound.xarray[0].xtable ) )
      {
        pod->doindex = pod->xfound.hard_index;
        /* T( "@@name_offset:%lu - (%s|%s) noo:%c%c", pod->name_offset, pod->name, pod->xfound.xarray[pod->doindex].xtended->o.name, */
        /*    pod->xfound.xarray[pod->doindex].noo ? '+' : '-', pod->noo ? '+' : '-' );                                              */
      }
    }
    else if ( pod->xfound.count_soft > 1 )
    {
      /* MULTI-ERROR : may show list to choose */
      /* T( "@ERR %s", pod->string_copy ); */
      SERRM( OPTION_NEW_MULTIPLE, "'--%s' '--%s' (from %s)", pod->string_copy, pod->name, duf_optsource_name( pod->source ) );
    }
    else                        /* if ( pod->xfound.count_soft < 1 ) */
    {
      /* NOT-FOUND-ERROR */
      /* T( "@ERR %s", pod->string_copy ); */
      SERRM( OPTION_NEW_NOT_FOUND, "'--%s' '--%s' (from %s)", pod->string_copy, pod->name, duf_optsource_name( pod->source ) );
    }
  }
  else
  {
    /* BAD-MULTI-ERROR : same command defined, needs fixing tables */
    /* T( "@ERR %s", pod->string_copy ); */
    SERRM( OPTION_NEW_DUPLICATE, "'--%s' '--%s' (from %s)", pod->string_copy, pod->name, duf_optsource_name( pod->source ) );
  }
  if ( QNOERR && pod->doindex >= 0 && pod->clarifier && pod->stage != DUF_OPTION_STAGE_BOOT )
  {
    /* T( "@Do it : (%c%c)%s", pod->xfound.xarray[pod->doindex].noo ? '+' : '-', pod->noo ? '+' : '-', */
    /*    pod->xfound.xarray[pod->doindex].xtended->o.name );                                          */
    {
      char *oa;

      oa = duf_string_options_expand( pod->optarg, '?' );
      /* T( "@########### [%s:%s:%s] %lu %s", pod->string_copy,  pod->name, pod->optarg, pod->doindex, duf_optstage_name(pod->stage) ); */
      CRV( ( pod->clarifier ), pod->xfound.xarray[pod->doindex].xtended, oa, pod->stage, pod->xfound.xarray[pod->doindex].xtable, pod->xfound.xarray[pod->doindex].noo,
           pod->source );
      pod->clarified[pod->stage] = 1;
      mas_free( oa );
    }
  }
  /* if ( pod->source.sourcecode == DUF_OPTION_SOURCE_CLI )                                               */
  /*   T( "@==== [%s:%s:%s] ==== %d:%s\n", pod->string_copy, pod->name, pod->optarg, QERRIND, QERRNAME ); */
#if 0
  T( "@-------- %lu:%lu ----------", pod->xfound.count_hard, pod->xfound.count_soft );
  for ( size_t n = 0; n < pod->xfound.count_hard + pod->xfound.count_soft; n++ )
  {
    T( "@@ ========== s:%d; ch:%lu; cs:%lu; doi:%d {%d} --%s='%s' =======================", pod->xfound.xarray[n].soft, pod->xfound.count_hard,
       pod->xfound.count_soft, doindex, pod->clarifier ? 1 : 0, pod->xfound.xarray[n].xtended->o.name, pod->optarg );
  }
#endif
  /* mas_free( pod->xfound.xarray ); */
  /* pod->xfound.xarray = NULL; */
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

duf_option_data_t *
duf_pod_from_paod_n( const duf_option_adata_t * paod, duf_option_stage_t basicstage, duf_option_source_t source, size_t index )
{
  duf_option_data_t *pod = NULL;

  pod = paod->pods + paod->source_index[basicstage][source.sourcecode] + index;

  /* T( "@[%lu:%lu] %s/%s : %s (%s)", index, pod - paod->pods, duf_optstage_name( pod->stage ), duf_optsource_name( pod->source ), pod->string_copy, */
  /*    pod->name );                                                                                                                                 */
  return pod;
}

size_t
duf_pod_stage_count( const duf_option_adata_t * paod, duf_option_stage_t istage )
{
  return istage >= DUF_OPTION_STAGE_MIN && istage <= DUF_OPTION_STAGE_MAX ? paod->stage_count[istage] : 0;
}

size_t
duf_pod_source_count( const duf_option_adata_t * paod, duf_option_stage_t istage, duf_option_source_t source )
{
  return istage >= DUF_OPTION_STAGE_MIN && istage <= DUF_OPTION_STAGE_MAX && source.sourcecode >= DUF_OPTION_SOURCE_MIN
        && source.sourcecode <= DUF_OPTION_SOURCE_MAX ? paod->source_count[istage][source.sourcecode] : 0;
}

duf_option_data_t *
duf_pod_from_paod( const duf_option_adata_t * paod, duf_option_stage_t basicstage, duf_option_stage_t istage, duf_option_source_t source )
{
  duf_option_data_t *pod = NULL;

  pod = duf_pod_from_paod_n( paod, basicstage, source, paod->source_count[istage][source.sourcecode] );
  return pod;
}

SR( OPTIONS, soption_xclarify_new_at_stdx, const char *string, const char *name, const char *arg, duf_xclarifier_t clarifier, char vseparator,
    duf_option_stage_t istage, duf_option_source_t source, duf_option_data_t * pod, duf_option_adata_t * paod )
{
  int pod_allocated = 0;
  const duf_option_data_t *bootpod = NULL;

  if ( paod )
  {
    if ( !paod->size )
      paod->size = 1000;
    if ( paod->size && !paod->pods )
    {
      size_t m;

      m = sizeof( duf_option_data_t ) * paod->size;
      paod->pods = mas_malloc( m );
      memset( paod->pods, 0, m );
      paod->count = 0;
    }

    if ( paod->last_stage != istage )
    {
      paod->stage_index[istage] = paod->count;
      /* new stage */
      /* T( "@%s => %s @ %s", duf_optstage_name( paod->last_stage ), duf_optstage_name( istage ), duf_optsource_name( source ) ); */
      paod->last_stage = istage;
    }
    if ( paod->last_source.sourcecode != source.sourcecode )
    {
      paod->source_index[istage][source.sourcecode] = paod->count;
      paod->last_source = source;
    }
    if ( paod->count < paod->size )
    {
      pod = &paod->pods[paod->count++];
    }
    /* T( "@@%9s: %lu string  (%s)'%s'/'%s'", duf_optstage_name( istage ), paod->stage_count[istage], duf_optsource_name( source ), string, name ); */
    if ( istage > DUF_OPTION_STAGE_BOOT )
    {
      bootpod = duf_pod_from_paod( paod, DUF_OPTION_STAGE_BOOT, istage, source );
      /* assert(0==strcmp()); */
      if ( bootpod )
      {
        /* T( "@@%9s: %lu string  (%s)'%s'/'%s' =(%s)'%s'/'%s'", duf_optstage_name( istage ), paod->stage_count[istage], duf_optsource_name( source ), */
        /*    string, name, duf_optsource_name( bootpod->source ), bootpod->string_copy, bootpod->name );                                              */
#if 0
        assert( ( string == NULL && bootpod->string_copy == NULL && 0 == strcmp( name, bootpod->name ) )
                || 0 == strcmp( string, bootpod->string_copy ) );
        assert( ( !string && !bootpod->string_copy ) || ( string && bootpod->string_copy && 0 == strcmp( string, bootpod->string_copy ) ) );
        assert( ( string && !name ) || ( !name && !bootpod->name ) || ( name && bootpod->name && 0 == strcmp( name, bootpod->name ) ) );
        assert( ( string && !name ) || ( !arg && !bootpod->optarg ) || ( arg && bootpod->optarg && 0 == strcmp( arg, bootpod->optarg ) ) );
        assert( vseparator == bootpod->vseparator );
        /* assert( istage == bootpod->stage ); */
        assert( source.sourcecode == bootpod->source.sourcecode );
        assert( clarifier == bootpod->clarifier );
#endif
      }
      else
      {
        assert( 0 );
      }
    }
    else
    {
      /* assert( bootpod ); */
    }
  }
  else
  {
    assert( 0 );
  }

  /* T( "@%s", paod->pods[0].string_copy ); */
  /* duf_option_data_t od = { 0 }; */
  if ( !pod )
  {
    pod = mas_malloc( sizeof( duf_option_data_t ) );
    pod_allocated = 1;
    memset( pod, 0, sizeof( duf_option_data_t ) );
  }

  pod->vseparator = vseparator;
  pod->clarifier = clarifier;
  pod->stage = istage;

  if ( bootpod && istage > DUF_OPTION_STAGE_BOOT )
  {
    assert( bootpod );
    pod->vseparator = bootpod->vseparator;
    assert( clarifier == bootpod->clarifier );
    /* pod->clarifier = bootpod->clarifier; */
    pod->string_copy = mas_strdup( bootpod->string_copy );
    pod->name = mas_strdup( bootpod->name );
    pod->optarg = mas_strdup( bootpod->optarg );
    pod->source = bootpod->source;
  }
  else
  {
    pod->string_copy = mas_strdup( string );
    pod->name = mas_strdup( name );
    pod->optarg = mas_strdup( arg );
    pod->source = source;
  }


  if ( pod->string_copy && !pod->name )
  {
    if ( !pod->string_split )
    {
      CR( split_string_od, pod );
      pod->string_split = 1;
    }
  }
  else
    pod->has_arg = pod->optarg ? 1 : 0;

  CR( soption_xclarify_new_at_stdx_od, pod );

  if ( pod_allocated )
  {
    mas_free( pod->xfound.xarray );
    pod->xfound.xarray = NULL;
    mas_free( pod->name );
    pod->name = NULL;
    mas_free( pod->optarg );
    pod->optarg = NULL;
    mas_free( pod );
  }
  pod = NULL;
  if ( paod )
  {
    paod->stage_count[istage]++;
    paod->source_count[istage][source.sourcecode]++;
  }
  ER( OPTIONS, soption_xclarify_new_at_stdx, const char *string, const char *name, const char *arg, duf_xclarifier_t clarifier, char vseparator,
      duf_option_stage_t istage, duf_option_source_t source, duf_option_data_t * pod, duf_option_adata_t * paod );
}

SR( OPTIONS, soption_xclarify_new_at_stdx_default_with_pod, const char *string, const char *name, const char *arg, char vseparator,
    duf_option_stage_t istage, duf_option_source_t source, duf_option_data_t * pod, duf_option_adata_t * paod )
{
  /* T( "@last_stage: %s => %s (%s) %lu [%s:%s:%s]", duf_optstage_name( paod->last_stage ), duf_optstage_name( istage ), duf_optsource_name( source ), */
  /*    paod->count, string, name, arg );                                                                                                              */
  CR( soption_xclarify_new_at_stdx, string, name, arg, DUF_WRAPPED( duf_xoption_clarify ), vseparator, istage, source, pod, paod );
  /* T( "@############### %s:%s : %lu:%lu", duf_optstage_name( istage ), duf_optsource_name( source ), paod->stage_count[istage], */
  /*    paod->source_count[istage][source.sourcecode] );                                                                          */
  ER( OPTIONS, soption_xclarify_new_at_stdx_default_with_pod, const char *string, const char *name, const char *arg, char vseparator,
      duf_option_stage_t istage, duf_option_source_t source, duf_option_data_t * pod, duf_option_adata_t * paod );
}

SR( OPTIONS, soption_xclarify_new_at_stdx_default, const char *string, const char *name, const char *arg, char vseparator, duf_option_stage_t istage,
    duf_option_source_t source, duf_option_adata_t * paod )
{
  CR( soption_xclarify_new_at_stdx_default_with_pod, string, name, arg, vseparator, istage, source, NULL /* pod */ , paod );
  ER( OPTIONS, soption_xclarify_new_at_stdx_default, const char *string, const char *name, const char *arg, char vseparator,
      duf_option_stage_t istage, duf_option_source_t source, duf_option_adata_t * paod );
}

SR( OPTIONS, soption_xclarify_new_booted_source, duf_option_stage_t istage, duf_option_source_t source, duf_option_adata_t * paod )
{
  size_t cntpod;

  cntpod = duf_pod_source_count( paod, DUF_OPTION_STAGE_BOOT, source );
  DUF_TRACE( optsource, 0, "@@@@@@@source_count: %lu %s %s", cntpod, duf_optstage_name( istage ), duf_optsource_name( source ) );

  if ( istage > DUF_OPTION_STAGE_BOOT )
  {
    for ( size_t npod = 0; npod < cntpod; npod++ )
    {
      /* duf_option_data_t *pod; */

      /* pod = duf_pod_from_paod_n( paod, DUF_OPTION_STAGE_BOOT, source, npod ); */
      /* T( "@###################%lu. %p", npod, pod ); */
      CR( soption_xclarify_new_at_stdx_default, ( const char * ) NULL, ( const char * ) NULL, ( const char * ) NULL, '\0' /* vseparator */ ,
          istage, source, paod );
    }
  }
  ER( OPTIONS, soption_xclarify_new_booted_source, duf_option_stage_t istage, duf_option_source_t source, duf_option_adata_t * paod );
}


/* for batch:                        */
/* delim = duf_option_delimiter(  ); */
