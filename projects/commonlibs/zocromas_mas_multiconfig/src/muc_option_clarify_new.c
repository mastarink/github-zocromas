/* #undef MAS_TRACING */
/* #define MAST_TRACE_CONFIG muc_get_cli_options_trace_config(cli) */
#include "muc_tracen_defs_preset.h"
#include "muc_config_trace_enum.h"

#include "muc_errorn_defs_preset.h"

#include <assert.h>                                                  /* assert */
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ♣ */
#include <mastar/tools/mas_expandable.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs.h>                             /* MASE_TEST_R; MASE_TEST_R_LOWERE; ... */
#include <mastar/error/mas_error_defs_make.h>                        /* MASE_MAKE_ERROR; MASE_MAKE_ERRORFL; MASE_MAKE_ERRORM  ... */

#include "muc_se_only.h"                                             /* Only muc_DR; muc_SR; muc_ER; muc_CR; muc_QSTR; QERRIND; QERRNAME etc. ♠ */

#include "muc_option_config.h"                                       /* muc_get_cli_options_trace_config ♠ */

#include "muc_option_stage.h"                                        /* muc_optstage_name ♠ */
#include "muc_option_source.h"                                       /* muc_optsource_name ♠ */

#include "muc_option_clarify.h"

/* ###################################################################### */
#include "muc_option_clarify_new.h"
/* ###################################################################### */

static
muc_SR( OPTIONS, split_string_od, muc_config_cli_t * cli, muc_option_data_t * pod )
{
  const char *barg = NULL;
  const char *endn = NULL;
  char *name = NULL;
  char *arg = NULL;
  char *string;

  string = mas_strdup( pod->string_copy );
  pod->has_arg = 0;
  switch ( pod->value_separator )
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
#if 0                                                                /* 20160113.153243 */
  if ( endn )
    name = mas_strndup( string, endn - string );
  else
    name = mas_strdup( string );
#else
  name = endn ? mas_strndup( string, endn - string ) : mas_strdup( string );
#endif
  if ( barg )
  {
    mas_expandable_string_t cs_x = { 0, 0, NULL, NULL,.protected_prefix = '?',.varfunc = muc_cli_options_varfunc( cli ) };

    cs_x.value = mas_strdup( barg );
    arg = mas_strdup( mas_string_expanded( &cs_x ) );
    mas_expandable_string_delete( &cs_x );
  }
  pod->has_arg = ( ( pod->has_arg && arg ) || ( arg && *arg ) );
  pod->name = name;
  pod->optarg = arg;
/* muc_QT( "@0$$$$ n:%s; a:%s(%d:%d); s:%s -- (has_arg:%d)", pod->name, pod->optarg ? pod->optarg : "", pod->optarg ? 1 : 0, */
/*    ( pod->optarg && *( pod->optarg ) ) ? 1 : 0, pod->string_copy, pod->has_arg );                                         */
  for ( unsigned i = 0; i < strlen( string ); i++ )
    string[i] = '@';
  mas_free( string );
  muc_ER( OPTIONS, split_string_od, muc_config_cli_t * cli, muc_option_data_t * pod );
}

muc_SR( OPTIONS, soption_xclarify_new_register, muc_config_cli_t * cli, const muc_longval_extended_t * xtended, int soft, muc_option_data_t * pod )
{
  size_t alloc_step = 10;
  size_t index = 0;

/* muc_QT( "@@i name_offset:%lu - '%s' (%s) noo:%c", pod->name_offset, pod->name, xtended->o.name, pod->noo ? '+' : '-' ); */
/* muc_QT( "$$$$ SOFT:%d; n:%s; a:%s; s:%s -- o.n:%s (has_arg:%d) from %s", soft, pod->name, pod->optarg, pod->string_copy, xtended->o.name, pod->has_arg, */
/*    muc_optsource_name(cli, pod->source ) );                                                                                                       */

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
/* muc_QT( "@@@(%c%c)%s", pod->xfound.xarray[pod->doindex].noo ? '+' : '-', pod->noo ? '+' : '-', xtended->o.name ); */
  pod->xfound.xarray[index].noo = pod->noo;
/* muc_QT( "@@(%c%c)%s", pod->xfound.xarray[pod->doindex].noo ? '+' : '-', pod->noo ? '+' : '-', xtended->o.name ); */
  pod->xfound.xarray[index].xvtable = pod->xvtable;

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
  muc_ER( OPTIONS, soption_xclarify_new_register, muc_config_cli_t * cli, const muc_longval_extended_t * xtended, int soft, muc_option_data_t * pod );
}

muc_SR( OPTIONS, soption_xclarify_new_at_xarr_od, muc_config_cli_t * cli, const muc_longval_extended_t * xarr, muc_option_data_t * pod )
{
  for ( const muc_longval_extended_t * xtended = xarr; xtended->o.name; xtended++ )
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
    /* muc_QT( "@0a name_offset:%lu - %s ? %s", pod->name_offset, pod->name,  xtended->o.name ); */
    }
    else
    {
      pod->noo = 0;
      pod->name_offset = 0;
    /* muc_QT( "@0b name_offset:%lu - %s ? %s", pod->name_offset, pod->name,  xtended->o.name ); */
    }
  /* 
   * unset works at option_typed_gen.c / MUC_OPTION_VTYPE_FLAG; not here:

   if ( xtended->unset )
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
      /* muc_QT( "@@1 name_offset:%lu - '%s' (%s|%s) noo:%c", pod->name_offset, pod->name, match, xtended->o.name, pod->noo ? '+' : '-' ); */
        assert( len_tb == len_ask );
        muc_CR( soption_xclarify_new_register, cli, xtended, 0 /* soft */ , pod );
      }
      else if ( 0 == strncmp( match, xtended->o.name, len_ask ) )
      {
      /* muc_QT( "@@2 name_offset:%lu - '%s' (%s|%s) noo:%c", pod->name_offset, pod->name, match, xtended->o.name, pod->noo ? '+' : '-' ); */
        assert( len_tb > len_ask );
        muc_CR( soption_xclarify_new_register, cli, xtended, len_tb - len_ask /* soft */ , pod );
      }
    }
  /* muc_QT( "@3 name_offset:%lu - '%s' (%s|%s) noo:%d; %d; %d (%ld:%ld)", pod->name_offset, pod->name, match, xtended->o.name, pod->noo, arg_error, 
     pod->has_arg, pod->xfound.count_hard, pod->xfound.count_soft );                                                                         */
  }

  muc_ER( OPTIONS, soption_xclarify_new_at_xarr_od, muc_config_cli_t * cli, const muc_longval_extended_t * xarr, muc_option_data_t * pod );
}

muc_SR( OPTIONS, soption_xclarify_new_at_xtable_od, muc_config_cli_t * cli, const muc_longval_extended_vtable_t * xvtable, muc_option_data_t * pod )
{
  pod->xvtable = xvtable;
  muc_CR( soption_xclarify_new_at_xarr_od, cli, xvtable->xlist, pod );

  muc_ER( OPTIONS, soption_xclarify_new_at_xtable_od, muc_config_cli_t * cli, const muc_longval_extended_vtable_t * xvtable,
          muc_option_data_t * pod );
}

static
muc_SR( OPTIONS, soption_xclarify_new_at_multix_od, muc_config_cli_t * cli, muc_longval_extended_vtable_t ** xvtables, muc_option_data_t * pod )
{
/* int doindex = -1; */

  pod->doindex = -1;
  for ( ; *xvtables; xvtables++ )
  {
    const muc_longval_extended_vtable_t *xvtable = *xvtables;

    muc_CR( soption_xclarify_new_at_xtable_od, cli, xvtable, pod );
  /* muc_QT( "@**%ld : %ld", pod->xfound.count_hard, pod->xfound.count_soft ); */
/* if not found : clear error */
  }
  if ( pod->xfound.count_hard == 1 )
  {
    if ( pod->xfound.xarray[pod->xfound.hard_index].soft == 0
         && MUC_OPTION_CHECK_STAGE( cli, pod->stage, pod->xfound.xarray[pod->xfound.hard_index].xtended, pod->xfound.xarray[0].xvtable ) )
    {
      pod->doindex = pod->xfound.hard_index;
    /* muc_QT( "@@name_offset:%lu - (%s|%s) noo:%c%c", pod->name_offset, pod->name, pod->xfound.xarray[pod->doindex].xtended->o.name, */
    /*    pod->xfound.xarray[pod->doindex].noo ? '+' : '-', pod->noo ? '+' : '-' );                                              */
    }
  /* muc_QT( "@**%ld - %ld : %ld %s", pod->doindex, pod->xfound.count_hard, pod->xfound.count_soft, pod->name ); */
    if ( !( pod->doindex >= 0 || pod->stage < MUC_OPTION_STAGE_LOOP ) )
    {
      muc_QT( "@not match stage; %ld %s:%s hard:%lu; soft:%lu : \"%s\"", pod->doindex, muc_optstage_name( cli, pod->stage ),
              muc_optsource_name( cli, pod->source ), pod->xfound.count_hard, pod->xfound.count_soft, pod->name );
    }
  /* assert( pod->doindex >= 0 || pod->stage < MUC_OPTION_STAGE_LOOP ); */
  }
  else if ( pod->xfound.count_hard == 0 )
  {
    if ( pod->xfound.count_soft == 1 )
    {
    /* muc_QT( "@@name_offset:%lu - (%s) noo:%c", pod->name_offset, pod->name, pod->noo ? '+' : '-' ); */
      if ( pod->xfound.xarray[pod->xfound.soft_index].soft > 0
           && MUC_OPTION_CHECK_STAGE( cli, pod->stage, pod->xfound.xarray[pod->xfound.hard_index].xtended, pod->xfound.xarray[0].xvtable ) )
      {
        pod->doindex = pod->xfound.hard_index;
      /* muc_QT( "@@name_offset:%lu - (%s|%s) noo:%c%c", pod->name_offset, pod->name, pod->xfound.xarray[pod->doindex].xtended->o.name, */
      /*    pod->xfound.xarray[pod->doindex].noo ? '+' : '-', pod->noo ? '+' : '-' );                                              */
      }
    /* muc_QT( "@**%ld - %ld : %ld %s", pod->doindex, pod->xfound.count_hard, pod->xfound.count_soft, pod->name ); */
    }
    else if ( pod->xfound.count_soft > 1 )
    {
    /* MULTI-ERROR : may show list to choose */
    /* muc_QT( "@ERR %s", pod->string_copy ); */
      SERRM( OPTION_NEW_MULTIPLE, "'--%s' '--%s' (from %s)", pod->string_copy, pod->name, muc_optsource_name( cli, pod->source ) );
    }
    else                                                             /* if ( pod->xfound.count_soft < 1 ) */
    {
    /* NOT-FOUND-ERROR */
#if 0
      fprintf( stderr, "_______NOT FOUND_______ %s {%s:%s}\n", pod->name, muc_optstage_name( cli, pod->stage ),
               muc_optsource_name( cli, pod->source ) );
#endif
      muc_QT( "@ERR %s:%s", muc_optstage_name( cli, pod->stage ), muc_optsource_name( cli, pod->source ) );
      if ( pod->stage != MUC_OPTION_STAGE_BOOT )
        SERRM( OPTION_NEW_NOT_FOUND, "'--%s' '--%s' (from %s)", pod->string_copy, pod->name, muc_optsource_name( cli, pod->source ) );
    /* assert(0); */
    }
  /* muc_QT( "@**%ld - %ld : %ld %s", pod->doindex, pod->xfound.count_hard, pod->xfound.count_soft, pod->name ); */
  }
  else
  {
  /* BAD-MULTI-ERROR : same command defined, needs fixing tables */
  /* muc_QT( "@ERR %s", pod->string_copy ); */
    SERRM( OPTION_NEW_DUPLICATE, "'--%s' '--%s' (from %s)", pod->string_copy, pod->name, muc_optsource_name( cli, pod->source ) );
  }
/* muc_QT( "@%ld : %s", pod->doindex, pod->name ); */
  if ( QNOERR && pod->doindex >= 0 && pod->clarifier && pod->stage != MUC_OPTION_STAGE_BOOT )
  {
  /* muc_QT( "@Do it : (%c%c)%s", pod->xfound.xarray[pod->doindex].noo ? '+' : '-', pod->noo ? '+' : '-', */
  /*    pod->xfound.xarray[pod->doindex].xtended->o.name );                                          */
    {
      char *oa;

      {
        mas_expandable_string_t cs_x = { 0, 0, NULL, NULL,.protected_prefix = '?',.varfunc = muc_cli_options_varfunc( cli ) };

        cs_x.value = mas_strdup( pod->optarg );
        oa = mas_strdup( mas_string_expanded( &cs_x ) );
        mas_expandable_string_delete( &cs_x );
      }
      muc_CRV( ( pod->clarifier ), cli, pod->xfound.xarray[pod->doindex].xtended, oa, pod->xfound.xarray[pod->doindex].xvtable,
               pod->xfound.xarray[pod->doindex].noo, pod->stage, pod->source );
      pod->clarified[pod->stage] = 1;
      mas_free( oa );
    }
  }
/* if ( pod->source.sourcecode == MUC_OPTION_SOURCE_CLI )                                               */
/*   muc_QT( "@==== [%s:%s:%s] ==== %d:%s\n", pod->string_copy, pod->name, pod->optarg, QERRIND, QERRNAME ); */
#if 0
  muc_QT( "@-------- %lu:%lu ----------", pod->xfound.count_hard, pod->xfound.count_soft );
  for ( size_t n = 0; n < pod->xfound.count_hard + pod->xfound.count_soft; n++ )
  {
    muc_QT( "@@ ========== s:%d; ch:%lu; cs:%lu; doi:%d {%d} --%s='%s' =======================", pod->xfound.xarray[n].soft, pod->xfound.count_hard,
            pod->xfound.count_soft, doindex, pod->clarifier ? 1 : 0, pod->xfound.xarray[n].xtended->o.name, pod->optarg );
  }
#endif
/* mas_free( pod->xfound.xarray ); */
/* pod->xfound.xarray = NULL; */
/* mas_free( pod->name );   */
/* pod->name = NULL;        */
/* mas_free( pod->optarg ); */
/* pod->optarg = NULL;      */
  muc_QTR;

  muc_ER( OPTIONS, soption_xclarify_new_at_multix_od, muc_config_cli_t * cli, muc_longval_extended_vtable_t ** xvtables, muc_option_data_t * pod );
}

static
muc_SR( OPTIONS, soption_xclarify_new_at_stdx_od, muc_config_cli_t * cli, muc_option_data_t * pod )
{
  muc_CR( soption_xclarify_new_at_multix_od, cli, muc_cli_options_xvtable_multi( cli ), pod );

  muc_ER( OPTIONS, soption_xclarify_new_at_stdx_od, muc_config_cli_t * cli, muc_option_data_t * pod );
}

static muc_option_data_t *
muc_pod_from_paod_n( const muc_option_adata_t * paod, muc_option_stage_t basicstage, muc_option_source_t source, size_t index )
{
  muc_option_data_t *pod = NULL;

  pod = paod->pods + paod->source_index[basicstage][source.sourcecode] + index;
  assert( paod->pods[paod->source_index[basicstage][source.sourcecode] + index].source.sourcecode == source.sourcecode );
/* muc_QT( "@[%lu:%lu] %s/%s : %s (%s)", index, pod - paod->pods, muc_optstage_name(cli, pod->stage ), muc_optsource_name(cli, pod->source ), pod->string_copy, */
/*    pod->name );                                                                                                                                 */
  return pod;
}

size_t
muc_pod_stage_count( const muc_option_adata_t * paod, muc_option_stage_t istage )
{
  return istage >= MUC_OPTION_STAGE_MIN && istage <= MUC_OPTION_STAGE_MAX ? paod->stage_count[istage] : 0;
}

size_t
muc_pod_source_count( const muc_option_adata_t * paod, muc_option_stage_t istage, muc_option_source_t source )
{
  return istage >= MUC_OPTION_STAGE_MIN && istage <= MUC_OPTION_STAGE_MAX && source.sourcecode >= MUC_OPTION_SOURCE_MIN
          && source.sourcecode <= MUC_OPTION_SOURCE_MAX ? paod->source_count[istage][source.sourcecode] : 0;
}

static muc_option_data_t *
muc_pod_from_paod( const muc_option_adata_t * paod, muc_option_stage_t basicstage, muc_option_stage_t istage, muc_option_source_t source )
{
  muc_option_data_t *pod = NULL;

/* muc_QT( "@%s:%s : %lu", muc_optstage_name(cli, istage ), muc_optsource_name(cli, source ), paod->source_count[istage][source.sourcecode] ); */
  pod = muc_pod_from_paod_n( paod, basicstage, source, paod->source_count[istage][source.sourcecode] );
  return pod;
}

static
muc_SR( OPTIONS, soption_xclarify_new_at_stdx, muc_config_cli_t * cli, const char *string, const char *name, const char *arg,
        muc_xclarifier_t clarifier, char value_separator, muc_option_stage_t istage, muc_option_source_t source, muc_option_data_t * pod,
        muc_option_adata_t * paod )
{
  int pod_allocated = 0;
  const muc_option_data_t *bootpod = NULL;

  if ( paod )
  {
    if ( !paod->size )
      paod->size = 1000;
    if ( paod->size && !paod->pods )
    {
      size_t m;

      m = sizeof( muc_option_data_t ) * paod->size;
      paod->pods = mas_malloc( m );
      memset( paod->pods, 0, m );
      paod->count = 0;
    }

    if ( paod->last_stage != istage )
    {
      paod->stage_index[istage] = paod->count;
    /* new stage */
    /* muc_QT( "@%s => %s @ %s", muc_optstage_name(cli, paod->last_stage ), muc_optstage_name(cli, istage ), muc_optsource_name(cli, source ) ); */
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
  /* muc_QT( "@@%9s: %lu string  (%s)'%s'/'%s'", muc_optstage_name(cli, istage ), paod->stage_count[istage], muc_optsource_name(cli, source ), string, name ); */
    if ( istage > MUC_OPTION_STAGE_BOOT )
    {
      bootpod = muc_pod_from_paod( paod, MUC_OPTION_STAGE_BOOT, istage, source );
      assert( !bootpod || bootpod->source.sourcecode == source.sourcecode );
    /* muc_QT( "@bootpod from %s/%s", muc_optsource_name(cli, source ), muc_optsource_name(cli, bootpod->source ) ); */
    /* assert(0==strcmp()); */
      if ( bootpod )
      {
      /* muc_QT( "@@%9s: %lu string  (%s)'%s'/'%s' =(%s)'%s'/'%s'", muc_optstage_name(cli, istage ), paod->stage_count[istage], muc_optsource_name(cli, source ), */
      /*    string, name, muc_optsource_name(cli, bootpod->source ), bootpod->string_copy, bootpod->name );                                              */
#if 0
        assert( ( string == NULL && bootpod->string_copy == NULL && 0 == strcmp( name, bootpod->name ) )
                || 0 == strcmp( string, bootpod->string_copy ) );
        assert( ( !string && !bootpod->string_copy ) || ( string && bootpod->string_copy && 0 == strcmp( string, bootpod->string_copy ) ) );
        assert( ( string && !name ) || ( !name && !bootpod->name ) || ( name && bootpod->name && 0 == strcmp( name, bootpod->name ) ) );
        assert( ( string && !name ) || ( !arg && !bootpod->optarg ) || ( arg && bootpod->optarg && 0 == strcmp( arg, bootpod->optarg ) ) );
        assert( value_separator == bootpod->value_separator );
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
    assert( 0 || istage == MUC_OPTION_STAGE_INTERACTIVE );
  }

/* muc_QT( "@%s", paod->pods[0].string_copy ); */
/* muc_option_data_t od = { 0 }; */
  if ( !pod )
  {
    pod = mas_malloc( sizeof( muc_option_data_t ) );
    pod_allocated = 1;
    memset( pod, 0, sizeof( muc_option_data_t ) );
  }

  pod->value_separator = value_separator;
  pod->clarifier = clarifier;
  pod->stage = istage;

  if ( bootpod && istage > MUC_OPTION_STAGE_BOOT )
  {
    assert( bootpod );
    pod->value_separator = bootpod->value_separator;
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
      muc_CR( split_string_od, cli, pod );
      pod->string_split = 1;
    }
  }
  else
    pod->has_arg = pod->optarg ? 1 : 0;

  muc_CR( soption_xclarify_new_at_stdx_od, cli, pod );

  if ( pod_allocated )
  {
    mas_free( pod->xfound.xarray );
    pod->xfound.xarray = NULL;
    mas_free( pod->string_copy );
    pod->string_copy = NULL;
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
  muc_ER( OPTIONS, soption_xclarify_new_at_stdx, muc_config_cli_t * cli, const char *string, const char *name, const char *arg,
          muc_xclarifier_t clarifier, char value_separator, muc_option_stage_t istage, muc_option_source_t source, muc_option_data_t * pod,
          muc_option_adata_t * paod );
}

muc_SR( OPTIONS, soption_xclarify_new_at_stdx_default_with_pod, muc_config_cli_t * cli, const char *string, const char *name, const char *arg,
        muc_xclarifier_t clarifier, char value_separator, muc_option_stage_t istage, muc_option_source_t source, muc_option_data_t * pod,
        muc_option_adata_t * paod )
{
/* muc_QT( "@last_stage: %s => %s (%s) %lu [%s:%s:%s]", muc_optstage_name(cli, paod->last_stage ), muc_optstage_name(cli, istage ), muc_optsource_name(cli, source ), */
/*    paod->count, string, name, arg );                                                                                                              */
  muc_CR( soption_xclarify_new_at_stdx, cli, string, name, arg, clarifier ? clarifier : muc_xoption_clarify, value_separator, istage, source, pod,
          paod );
/* muc_QT( "@############### %s:%s : %lu:%lu", muc_optstage_name(cli, istage ), muc_optsource_name(cli, source ), paod->stage_count[istage], */
/*    paod->source_count[istage][source.sourcecode] );                                                                          */
  muc_ER( OPTIONS, soption_xclarify_new_at_stdx_default_with_pod, muc_config_cli_t * cli, const char *string, const char *name, const char *arg,
          muc_xclarifier_t clarifier, char value_separator, muc_option_stage_t istage, muc_option_source_t source, muc_option_data_t * pod,
          muc_option_adata_t * paod );
}

muc_SR( OPTIONS, soption_xclarify_snac_new_at_stdx_default, muc_config_cli_t * cli, const char *string, const char *name, const char *arg,
        muc_xclarifier_t clarifier, char value_separator, muc_option_stage_t istage, muc_option_source_t source, muc_option_adata_t * paod )
{
  muc_CR( soption_xclarify_new_at_stdx_default_with_pod, cli, string, name, arg, clarifier, value_separator, istage, source, NULL /* pod */ , paod );
  muc_ER( OPTIONS, soption_xclarify_snac_new_at_stdx_default, muc_config_cli_t * cli, const char *string, const char *name, const char *arg,
          muc_xclarifier_t clarifier, char value_separator, muc_option_stage_t istage, muc_option_source_t source, muc_option_adata_t * paod );
}

muc_SR( OPTIONS, soption_xclarify_sna_new_at_stdx_default, muc_config_cli_t * cli, const char *string, const char *name, const char *arg,
        char value_separator, muc_option_stage_t istage, muc_option_source_t source, muc_option_adata_t * paod )
{
  muc_CR( soption_xclarify_snac_new_at_stdx_default, cli, string, name, arg, ( muc_xclarifier_t ) NULL, value_separator, istage, source, paod );
  muc_ER( OPTIONS, soption_xclarify_sna_new_at_stdx_default, muc_config_cli_t * cli, const char *string, const char *name, const char *arg,
          char value_separator, muc_option_stage_t istage, muc_option_source_t source, muc_option_adata_t * paod );
}

muc_SR( OPTIONS, soption_xclarify_na_new_at_stdx_default, muc_config_cli_t * cli, const char *name, const char *arg, char value_separator,
        muc_option_stage_t istage, muc_option_source_t source, muc_option_adata_t * paod )
{
  muc_CR( soption_xclarify_sna_new_at_stdx_default, cli, ( const char * ) NULL /* string */ , name, arg, value_separator, istage, source, paod );
  muc_ER( OPTIONS, soption_xclarify_na_new_at_stdx_default, muc_config_cli_t * cli, const char *name, const char *arg, char value_separator,
          muc_option_stage_t istage, muc_option_source_t source, muc_option_adata_t * paod );
}

muc_SR( OPTIONS, soption_xclarify_s_new_at_stdx_default, muc_config_cli_t * cli, const char *string, char value_separator, muc_option_stage_t istage,
        muc_option_source_t source, muc_option_adata_t * paod )
{
  muc_CR( soption_xclarify_sna_new_at_stdx_default, cli, string, ( const char * ) NULL /* name */ , ( const char * ) NULL /* arg */ , value_separator,
          istage, source, paod );
  muc_ER( OPTIONS, soption_xclarify_s_new_at_stdx_default, muc_config_cli_t * cli, const char *string, char value_separator,
          muc_option_stage_t istage, muc_option_source_t source, muc_option_adata_t * paod );
}

muc_SR( OPTIONS, soption_xclarify_new_booted_source, muc_config_cli_t * cli, muc_option_stage_t istage, muc_option_source_t source,
        muc_option_adata_t * paod )
{
  size_t cntpod;

  cntpod = muc_pod_source_count( paod, MUC_OPTION_STAGE_BOOT, source );
  MAST_TRACE( optsource, 0, "@@@@@@@source_count: %lu %s %s", cntpod, muc_optstage_name( cli, istage ), muc_optsource_name( cli, source ) );

  if ( istage > MUC_OPTION_STAGE_BOOT )
  {
    for ( size_t npod = 0; npod < cntpod; npod++ )
    {
    /* muc_option_data_t *pod; */
    /* muc_QT( "@npod:%lu/%lu", npod, cntpod ); */
    /* pod = ..._pod_from_paod_n( paod, MUC_OPTION_STAGE_BOOT, source, npod ); */
    /* MAST_TRACE( optsource, 0,  "@#############  %lu.", npod ); */
      muc_CR( soption_xclarify_snac_new_at_stdx_default, cli, ( const char * ) NULL /* string */ , ( const char * ) NULL /* name */ ,
              ( const char * ) NULL /* arg */ , ( muc_xclarifier_t ) NULL, '\0' /* value_separator */ ,
              istage, source, paod );
    }
  }
  muc_ER( OPTIONS, soption_xclarify_new_booted_source, muc_config_cli_t * cli, muc_option_stage_t istage, muc_option_source_t source,
          muc_option_adata_t * paod );
}

/* for batch:                        */
/* delim = ..._option_delimiter(  ); */
