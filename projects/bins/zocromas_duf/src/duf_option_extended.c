#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_options.h"

/* #include "duf_options_config.h" */
#include "duf_option_names.h"   /* duf_optstage_name */
#include "duf_xtended_table.h"
/* ###################################################################### */
#include "duf_option_extended.h"
/* ###################################################################### */

static void
duf_print_stage( int use_stage, int use_stage_mask, duf_limits_stage_t stage, unsigned long stage_mask, unsigned eol )
{
  if ( use_stage || use_stage_mask )
  {
    DUF_PRINTF( 0, ".  " );
    if ( use_stage )
    {
      DUF_PRINTF( 0, ".stage(%s,%s) ", duf_optstage_name( stage.min ), duf_optstage_name( stage.max ) );
    }
    if ( use_stage_mask )
    {
      unsigned long msk = stage_mask;
      unsigned n = 0;

      DUF_PRINTF( 0, ".mask(%lu; { ", stage_mask );
      while ( msk )
      {
        if ( msk & 1 )
        {
          DUF_PRINTF( 0, ".%s; ", duf_optstage_name( n ) );
        }
        n++;
        msk = msk >> 1;
      }
      DUF_PRINTF( 0, ".} )" );
    }
    if ( eol )
      DUF_PUTSL( 0 );
  }
}

const char *
duf_extended_code2string( duf_option_code_t code )
{
  const char *rs = NULL;

  switch ( code )
  {
#define ENUM_WRAP(_n)       case DUF_OPTION_VAL_ ## _n: rs= #_n; break;
#define ENUM_WRAP_V(_n, _v) case DUF_OPTION_VAL_ ## _n: rs= #_n; break;
#include "duf_options_enum.def"
#undef ENUM_WRAP
#undef ENUM_WRAP_V
  }
  return rs;
}

const char *
duf_extended_class2string( duf_option_class_t oclass )
{
  const char *rs = NULL;

  switch ( oclass )
  {
#define ENUM_WRAP(_n)       case DUF_OPTION_CLASS_ ## _n: rs= #_n; break;
#define ENUM_WRAP_V(_n, _v) case DUF_OPTION_CLASS_ ## _n: rs= #_n; break;
#include "duf_option_class_enum.def"
#undef ENUM_WRAP
#undef ENUM_WRAP_V
  }
  return rs;
}

const char *
duf_extended_vtype2string( duf_option_vtype_t ovtype )
{
  const char *rs = NULL;

  switch ( ovtype )
  {
#define ENUM_WRAP(_n)       case DUF_OPTION_VTYPE_ ## _n: rs= #_n; break;
#define ENUM_WRAP_V(_n, _v) case DUF_OPTION_VTYPE_ ## _n: rs= #_n; break;
#include "duf_option_vtype_enum.def"
#undef ENUM_WRAP
#undef ENUM_WRAP_V
  }
  return rs;
}

void
duf_extended_table_print( const duf_longval_extended_table_t * xtable, const char *name )
{
  DUF_PRINTF( 0, ".@@@ [%s]", xtable->name );
  duf_print_stage( xtable->use_stage, xtable->use_stage_mask, xtable->stage, xtable->stage_mask, 0 );
  DUF_PUTSL( 0 );

  for ( const duf_longval_extended_t * xtended = xtable->table; xtended->o.name; xtended++ )
  {
    if ( !name || strstr( xtended->o.name, name ) )
    {
      DUF_PRINTF( 0, "cmd %s", xtended->o.name );
      DUF_PRINTF( 0, "{" );
      if ( xtended->o.has_arg == 0 )
        DUF_PRINTF( 0, "  optarg: %s", "no" );
      else if ( xtended->o.has_arg == 1 )
        DUF_PRINTF( 0, "  optarg: %s", "optional" );
      else if ( xtended->o.has_arg == 2 )
        DUF_PRINTF( 0, "  optarg: %s", "required" );
      else
        DUF_PRINTF( 0, "  optarg: %s", "error" );
      if ( xtended->can_no || xtended->invert || xtended->m_hasoff )
      {
        DUF_PRINTF( 0, ".%s", "  " );
        DUF_PRINTF( 0, ".%s", xtended->can_no ? "can_no " : "" );
        DUF_PRINTF( 0, ".%s", xtended->invert ? "invert " : "" );
        if ( xtended->m_hasoff )
          DUF_PRINTF( 0, ".offset(%lu,%d) ", xtended->m_offset, xtended->relto );
        DUF_PUTSL( 0 );
      }
      duf_print_stage( xtended->use_stage, xtended->use_stage_mask, xtended->stage, xtended->stage_mask, 1 );
      {
        DUF_PRINTF( 0, ".%s", "  " );
        DUF_PRINTF( 0, ".code:%s(%d)", duf_extended_code2string( xtended->o.val ), xtended->o.val );
        DUF_PUTSL( 0 );
      }
      DUF_PRINTF( 0, "  class:%s(%d)", duf_extended_class2string( xtended->oclass ), xtended->oclass );
      DUF_PRINTF( 0, "  vtype:%s(%d)", duf_extended_vtype2string( xtended->vtype ), xtended->vtype );
      DUF_PRINTF( 0, "  funcname:%s", xtended->call.funcname );
      switch ( xtended->vtype )
      {
      case DUF_OPTION_VTYPE_EIA_CALL:
        DUF_PRINTF( 0, "  func:%p(%d)", xtended->call.fdesc.eia.func, xtended->call.fdesc.eia.arg );
        break;
      case DUF_OPTION_VTYPE_EV_CALL:
        DUF_PRINTF( 0, "  func:%p( )", xtended->call.fdesc.ev.func );
        break;
      case DUF_OPTION_VTYPE_SAS_CALL:
        DUF_PRINTF( 0, "  func:%p(%s)", xtended->call.fdesc.sas.func, xtended->call.fdesc.sas.arg );
        break;
      case DUF_OPTION_VTYPE_SAN_CALL:
        DUF_PRINTF( 0, "  func:%p(%ld)", xtended->call.fdesc.san.func, xtended->call.fdesc.san.arg );
        break;
      case DUF_OPTION_VTYPE_A_CALL:
        DUF_PRINTF( 0, "  func:%p(duf_cli_options_get_carg(  )->argc,duf_cli_options_get_carg(  )->argv)", xtended->call.fdesc.a.func );
        break;
      case DUF_OPTION_VTYPE_N_CALL:
        /* int rxx DUF_UNUSED = 0; */
        /* DUF_PRINTF( 0, "  func:%p(%ld)", xtended->call.fdesc.n.func, duf_strtol_suff( optargg, &rxx ) ); */
        DUF_PRINTF( 0, "  func:%p(duf_strtol_suff( optargg, &r ))", xtended->call.fdesc.n.func );
        break;
      case DUF_OPTION_VTYPE_S_CALL:
        DUF_PRINTF( 0, "  func:%p(optargg)", xtended->call.fdesc.s.func );
        break;
      case DUF_OPTION_VTYPE_AA_CALL:
        DUF_PRINTF( 0, "  func:%p(duf_cli_options_get_carg(  ))", xtended->call.fdesc.aa.func );
        break;
      case DUF_OPTION_VTYPE_TN1_CALL:
        DUF_PRINTF( 0, "  func:%p(duf_cli_options_get_targ(  ), duf_strtol_suff( optargg, &r ))", xtended->call.fdesc.tn1.func );
        break;
      case DUF_OPTION_VTYPE_TS1_CALL:
        DUF_PRINTF( 0, "  func:%p(duf_cli_options_get_targ(  ), optargg)", xtended->call.fdesc.ts1.func );
        break;
      case DUF_OPTION_VTYPE_TS2_CALL:
        DUF_PRINTF( 0, "  func:%p(duf_cli_options_get_ptargc(  ), duf_cli_options_get_ptargv(  ), optargg)", xtended->call.fdesc.ts2.func );
        break;
      default:
        break;
      }
      DUF_PRINTF( 0, "  help(%s)", xtended->help );
      DUF_PRINTF( 0, "}" );
    }
  }
}

void
duf_extended_table_multi_print( const duf_longval_extended_table_t ** xtables, const char *name )
{
  for ( const duf_longval_extended_table_t ** xt = xtables; xt && *xt; xt++ )
  {
    char *se = NULL;
    char *matchtab = NULL;
    char *matchopt = NULL;

    if ( name )
    {
      se = strchr( name, ':' );
      if ( se )
      {
        matchtab = mas_strndup( name, se - name );
        matchopt = mas_strdup( se + 1 );
      }
      else
        matchtab = mas_strdup( name );
    }
    /* if ( ( !matchtab || !*matchtab || 0 == strncmp( ( *xt )->name, matchtab, strlen( matchtab ) ) ) ) */
    if ( ( !matchtab || !*matchtab || strstr( ( *xt )->name, matchtab ) ) )
      duf_extended_table_print( *xt, matchopt );
    mas_free( matchtab );
    mas_free( matchopt );
  }
}

void
duf_extended_table_multi_std_print( const char *name )
{
  duf_extended_table_multi_print( duf_extended_table_multi(  ), name );
}

#if 0
const char *
duf_longindex_extended_name( int longindex, int *pr )
{
  const duf_longval_extended_t *extended = NULL;

  extended = duf_longindex2extended( longindex, pr );
  return extended ? extended->o.name : NULL;
}

duf_option_code_t
duf_longindex_extended_codeval( int longindex, int *pr )
{
  const duf_longval_extended_t *extended = NULL;

  extended = duf_longindex2extended( longindex, pr );
  return extended ? extended->o.val : 0;
}
#endif
const duf_longval_extended_t *
_duf_longindex2extended( int longindex, const duf_longval_extended_table_t * xtable, int *ptbcount, int no )
{
  const duf_longval_extended_t *extended = NULL;

  for ( const duf_longval_extended_t * xtended = xtable->table; *ptbcount <= longindex && xtended->o.name; xtended++ )
  {
    DUF_TRACE( options, 5, "@li2ex %d [%s]", *ptbcount, xtended->o.name );
    if ( no )
    {
      if ( xtended->can_no )
      {
        if ( *ptbcount == longindex )
        {
          extended = xtended;
          break;
        }
        ( *ptbcount )++;
      }
    }
    else
    {
      if ( *ptbcount == longindex )
      {
        extended = xtended;
        /* break; */
      }
      ( *ptbcount )++;
    }
  }

  return extended;
}

static void
duf_xtable2options( duf_option_t ** plongopts_ptr, const duf_longval_extended_t * xtended, int no )
{
  while ( xtended->o.name )
  {
    if ( no )
    {
      if ( xtended->can_no )
      {
        char *s = NULL;

        *( *plongopts_ptr ) = xtended->o;
        s = mas_strdup( "no-" );
        s = mas_strcat_x( s, ( *plongopts_ptr )->name );
        ( *plongopts_ptr )->name = s;
        /* ( *plongopts_ptr )->has_arg = xtended->o.has_arg; */
        /* ( *plongopts_ptr )->val = xtended->o.val;         */
        ( *plongopts_ptr )++;
      }
    }
    else
    {
      *( *plongopts_ptr ) = xtended->o;
      ( *plongopts_ptr )->name = mas_strdup( ( *plongopts_ptr )->name );
      /* ( *plongopts_ptr )->has_arg = xtended->o.has_arg; */
      /* ( *plongopts_ptr )->val = xtended->o.val;         */
      ( *plongopts_ptr )++;
    }
    xtended++;
  }
}

void
duf_options_delete_longopts_table( duf_option_t * longopts )
{
  for ( duf_option_t * o = longopts; o->name; o++ )
    mas_free( ( char * ) o->name );
  mas_free( longopts );
}

duf_option_t *
duf_options_create_longopts_table( const duf_longval_extended_table_t ** pxtable )
{
  duf_option_t *longopts = NULL;

  {
    duf_option_t *longopts_ptr;
    size_t tbsize = 0;

    tbsize = duf_longindex_extended_count( pxtable ) * ( sizeof( duf_longval_extended_t ) + 1 );

    longopts = longopts_ptr = mas_malloc( tbsize );
    memset( longopts, 0, tbsize );

    for ( const duf_longval_extended_table_t ** xtables = pxtable; *xtables; xtables++ )
      duf_xtable2options( &longopts_ptr, ( *xtables )->table, 0 );
    for ( const duf_longval_extended_table_t ** xtables = pxtable; *xtables; xtables++ )
      duf_xtable2options( &longopts_ptr, ( *xtables )->table, 1 );
  }
  return longopts;
}

const duf_longval_extended_t *
duf_longindex2extended( int longindex, const duf_longval_extended_table_t ** pxtable, int *pno )
{
  const duf_longval_extended_t *extended = NULL;
  int tbcount = 0;

  for ( const duf_longval_extended_table_t ** xtables = duf_extended_table_multi(  ); tbcount <= longindex && *xtables; xtables++ )
  {
    const duf_longval_extended_table_t *xtable = *xtables;

#if 0

    for ( const duf_longval_extended_t * xtended = xtable->table; tbcount <= longindex && xtended->o.name; xtended++ )
    {
      DUF_TRACE( options, 5, "@li2ex %d [%s]", tbcount, xtended->o.name );
      if ( tbcount == longindex )
      {
        extended = xtended;
        if ( pxtable )
          *pxtable = xtable;
        /* break; */
      }
      tbcount++;
    }
#else
    extended = _duf_longindex2extended( longindex, xtable, &tbcount, 0 /* no */  );
    if ( extended )
    {
      if ( pxtable )
        *pxtable = xtable;
      break;
    }
#endif
  }
  if ( !extended )
  {
    /* continue with tbcount */
    for ( const duf_longval_extended_table_t ** xtables = duf_extended_table_multi(  ); tbcount <= longindex && *xtables; xtables++ )
    {
      const duf_longval_extended_table_t *xtable = *xtables;

      extended = _duf_longindex2extended( longindex, xtable, &tbcount, 1 /* no */  );
      if ( extended )
      {
        if ( pxtable )
          *pxtable = xtable;
        if ( pno )
          *pno = !*pno;
        break;
      }
    }
  }

  return extended;
}

int
duf_longindex_extended_count( const duf_longval_extended_table_t ** xtables )
{
  int xcount = 0;

#if 0
  const duf_longval_extended_table_t *xtable;

  while ( ( xtable = *xtables++ ) )
  {
    const duf_longval_extended_t *xtended = xtable->table;

    while ( xtended->o.name )
    {
      xcount++;
      xtended++;
      if ( xtended->can_no )
        xcount++;
    }
  }
#else
  for ( const duf_longval_extended_table_t ** pxtables = xtables; *pxtables; pxtables++ )
  {
    const duf_longval_extended_t *xtended = NULL;

    xtended = ( *pxtables )->table;
    while ( xtended->o.name )
    {
      xcount++;
      xtended++;
      if ( xtended->can_no )
        xcount++;
    }
  }
#endif
  return xcount;
}
