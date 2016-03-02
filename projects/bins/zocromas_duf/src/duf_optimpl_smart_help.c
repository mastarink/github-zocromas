#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ♣ */
#include <mastar/trace/mas_trace.h>

#include <mastar/multiconfig/muc_options_file.h>
#include <mastar/multiconfig/muc_option_class.h>
/* #include <mastar/multiconfig/muc_option_names.h> */
#include <mastar/multiconfig/muc_option_descr.h>
#include <mastar/multiconfig/muc_option_vtype.h>
#include <mastar/multiconfig/muc_option_stage.h>
#include <mastar/multiconfig/muc_option_lfind.h>
#include <mastar/multiconfig/muc_option_config.h>

/* #include "duf_tracen_defs.h"                                         (* MAST_TRACE ♠ *) */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

#include "duf_output_defs.h"
#include "duf_printn_defs.h"                                         /* DUF_PRINTF etc. ♠ */

#include "duf_config.h"                                              /* duf_get_config ♠ */
#include "duf_config_ref.h"
#include "duf_config_defs.h"                                         /* DUF_CONF... ♠ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ♠ */
#include "duf_config_output_util.h"

#include "duf_action_table.h"
#include "duf_sccb.h"

#include "duf_xtended_table.h"

#include "duf_optimpl_extended2string.h"
#include "duf_optimpl_enum.h"                                        /* duf_option_code_t ♠ */

/* ###################################################################### */
#include "duf_optimpl_smart_help.h"
/* ###################################################################### */

static const char *oclass_titles[MUC_OPTION_CLASS_MAX + 1] = {
  [MUC_OPTION_CLASS_HELP] = "Help",
  [MUC_OPTION_CLASS_NO_HELP] = "No help",
  [MUC_OPTION_CLASS_SYSTEM] = "System",
  [MUC_OPTION_CLASS_CONTROL] = "Control",
  [MUC_OPTION_CLASS_DB] = "Database",
  [MUC_OPTION_CLASS_FS] = "File system",
  [MUC_OPTION_CLASS_SCCB] = "SCCB system",
  [MUC_OPTION_CLASS_REFERENCE] = "Reference",
  [MUC_OPTION_CLASS_COLLECT] = "Collect",
  [MUC_OPTION_CLASS_SCAN] = "Scan",
  [MUC_OPTION_CLASS_FILTER] = "Filter",
  [MUC_OPTION_CLASS_UPDATE] = "Update",
  [MUC_OPTION_CLASS_REQUEST] = "Request",
  [MUC_OPTION_CLASS_PRINT] = "Print",
  [MUC_OPTION_CLASS_TRACE] = "Trace",
  [MUC_OPTION_CLASS_OBSOLETE] = "Obsolete",
  [MUC_OPTION_CLASS_OTHER] = "Other",
  [MUC_OPTION_CLASS_NONE] = "None",
  [MUC_OPTION_CLASS_DEBUG] = "DEBUG",
  [MUC_OPTION_CLASS_NODESC] = "No desc",
};

mas_error_code_t
duf_optimpl_O_smart_help_all( muc_option_class_t oclass )
{
  DUF_STARTR( r );

  if ( oclass == MUC_OPTION_CLASS_ALL )
  {
    for ( muc_option_class_t oc = MUC_OPTION_CLASS_MIN + 1; oc < MUC_OPTION_CLASS_MAX; oc++ )
    {
      DOR( r, duf_optimpl_O_smart_help( oc ) );
    }
  }
  DUF_ENDR( r );
}

static void
duf_show_option_description_x( const muc_longval_extended_t * extended )
{
  int look = 1;
  const char *name;
  duf_option_code_t codeval;

  name = extended->o.name;
  codeval = extended->o.val;

/* DUF_PRINTF( 0, "<><><><><><cnd:%d> %d: ie:%d oc:%d '%s'; %u", cnd, ilong, ie, oclass, name, codeval ); */
  if ( DUF_CONFIGG( help_string ) )
  {
    char *s = DUF_CONFIGG( help_string );

    look = ( ( s && *s && !s[1] && codeval == *s ) || ( 0 == strcmp( s, name ) ) /* OR: else if ( strstr( name, s ) ) */  );
  }
  if ( look )
  {
    char *s = NULL;

  /* muc_option_class_t hclass; */

  /* hclass = duf_help_option2class( codeval ); */
    s = muc_xoption_description_d( duf_get_config_cli(  ), extended, "\t", " // " );
  /* s = mas_strcat_x( s, " ...................." ); */
    if ( s )
    {
    /* if ( shown >= 0 )                    */
    /*   DUF_PRINTF( 0, " ## %d;", shown ); */
    /* DUF_PRINTF( 0, "(%d;s[%d]=%d) %d. [%u] \t%s", shown, ie, ashown[ie], ilong, codeval, s ); */
      DUF_PRINTF( 0, "[%u] \t%s", codeval, s );
      mas_free( s );
    }
    else
    {
      DUF_PRINTF( 0, " ??? %s", name );
    }
  }
}

#if 0
static void
duf_show_loption_description( int ilong )
{
  int look = 1;
  const char *name;
  duf_option_code_t codeval;

  name = DUF_CONFIGG( pcli->longopts_table )[ilong].name;
  codeval = DUF_CONFIGG( pcli->longopts_table )[ilong].val;

/* DUF_PRINTF( 0, "<><><><><><cnd:%d> %d: ie:%d oc:%d '%s'; %u", cnd, ilong, ie, oclass, name, codeval ); */
  if ( DUF_CONFIGG( help_string ) )
  {
    char *s = DUF_CONFIGG( help_string );

    look = ( ( s && *s && !s[1] && codeval == *s ) || ( 0 == strcmp( s, name ) ) /* OR: else if ( strstr( name, s ) ) */  );
  }
  if ( look )
  {
    char *s = NULL;

  /* muc_option_class_t hclass; */

  /* hclass = duf_help_option2class( codeval ); */
    s = duf_loption_description_d( ilong, "\t", " // " );
  /* s = mas_strcat_x( s, " ...................." ); */
    if ( s )
    {
    /* if ( shown >= 0 )                    */
    /*   DUF_PRINTF( 0, " ## %d;", shown ); */
    /* DUF_PRINTF( 0, "(%d;s[%d]=%d) %d. [%u] \t%s", shown, ie, ashown[ie], ilong, codeval, s ); */
      DUF_PRINTF( 0, " %02d. [%u] \t%s", ilong, codeval, s );
      mas_free( s );
    }
    else
    {
      DUF_PRINTF( 0, " ??? %s", name );
    }
  }
}
#endif
static void MAS_UNUSED
duf_show_xoption_description( const muc_longval_extended_t * extended, int ilong )
{
  int look = 1;
  const char *name;
  duf_option_code_t codeval;

/* name = DUF_CONFIGG( pcli->longopts_table )[ilong].name; */
  name = extended->o.name;
/* codeval = DUF_CONFIGG( pcli->longopts_table )[ilong].val; */
  codeval = extended->o.val;

/* DUF_PRINTF( 0, "<><><><><><cnd:%d> %d: ie:%d oc:%d '%s'; %u", cnd, ilong, ie, oclass, name, codeval ); */
  if ( DUF_CONFIGG( help_string ) )
  {
    char *s = DUF_CONFIGG( help_string );

    look = ( ( s && *s && !s[1] && codeval == *s ) || ( 0 == strcmp( s, name ) ) /* OR: else if ( strstr( name, s ) ) */  );
  }
  if ( look )
  {
    char *s = NULL;

  /* muc_option_class_t hclass; */

  /* hclass = duf_help_option2class( codeval ); */
    s = muc_xoption_description_d( duf_get_config_cli(  ), extended, "\t", " // " );
  /* s = mas_strcat_x( s, " ...................." ); */
    if ( s )
    {
    /* if ( shown >= 0 )                    */
    /*   DUF_PRINTF( 0, " ## %d;", shown ); */
    /* DUF_PRINTF( 0, "(%d;s[%d]=%d) %d. [%u] \t%s", shown, ie, ashown[ie], ilong, codeval, s ); */
      DUF_PRINTF( 0, " %02d. [%u] \t%s", ilong, codeval, s );
      mas_free( s );
    }
    else
    {
      DUF_PRINTF( 0, " ??? %s", name );
    }
  }
}

mas_error_code_t
duf_optimpl_O_smart_help( muc_option_class_t oclass )
{
  DUF_STARTR( r );

  int *ashown;
  size_t ss;
  int ilong = 0;

  ss = DUF_OPTION_VAL_MAX_LONG * sizeof( int );

  ashown = mas_malloc( ss );
  memset( ( void * ) ashown, 0, ss );
/* for ( int ilong = 0; DUF_CONFIGG(pcli->longopts_table)[ilong].name && ilong < lo_extended_count; ilong++ ) */
/* {                                                                                                   */
/* }                                                                                                   */
  if ( oclass <= MUC_OPTION_CLASS_MAX && oclass_titles[oclass] && *oclass_titles[oclass] )
    DUF_PRINTF( 0, "-=-=-=-=- %s (%s) -=-=-=-=-", oclass_titles[oclass], muc_optclass_name( oclass ) );
  else
    DUF_PRINTF( 0, "-=-=-=-=- <no title set for %d> -=-=-=-=-", oclass );
  {
#if 0
    int tbcount;

    tbcount = muc_longindex_extended_count( duf_extended_vtable_multi(  ) );
    for ( ilong = 0; DUF_NOERROR( r ) && DUF_CONFIGG( pcli->longopts_table )[ilong].name && ilong < tbcount; ilong++ )
#else
    for ( muc_longval_extended_vtable_t ** xtables = muc_cli_options_xvtable_multi( duf_get_config_cli(  ) ); *xtables; xtables++ )
      for ( const muc_longval_extended_t * xtended = ( *xtables )->xlist; xtended->o.name; ilong++, xtended++ )
#endif
      {
        const muc_longval_extended_t *extended;

        extended = muc_loption_xfind_at_stdx( duf_get_config_cli(  ), ilong, ( const muc_longval_extended_vtable_t ** ) NULL, NULL /* &no */  );
        {
          int ie;
          duf_option_code_t codeval;

          codeval = DUF_CONFIGG( pcli->longopts_table )[ilong].val;
        /* extended = _duf_find_longval_extended( codeval ); */
        /* ie = extended ? extended - &lo_extended[0] : -1; */
          ie = codeval;
          assert( extended );
          if ( codeval && DUF_NOERROR( r ) )
          {
            int cnd = 0;

            cnd = ( !extended && ( oclass == MUC_OPTION_CLASS_ANY || oclass == MUC_OPTION_CLASS_NODESC ) )
                    || ( extended && ( oclass == MUC_OPTION_CLASS_ANY || oclass == extended->oclass ) );
            if ( cnd )
            {
              int shown = -1;

              if ( ie >= 0 )
                shown = ashown[ie];
              if ( shown <= 0 )
#if 0
                duf_show_loption_description( ilong );
#else
                duf_show_xoption_description( extended, ilong );
#endif
              if ( ie >= 0 )
                ashown[ie]++;
            }
          }
        }
      }
  }
  mas_free( ashown );

  DUF_ENDR( r );
}

mas_error_code_t
duf_optimpl_O_help_set( const char *arg )
{
  DUF_STARTR( r );

  for ( muc_longval_extended_vtable_t * *xvtables = muc_cli_options_xvtable_multi( duf_get_config_cli(  ) ); *xvtables; xvtables++ )
  {
    const muc_longval_extended_vtable_t *xvtable = *xvtables;
    int title_printed = 0;

    if ( xvtable && xvtable->name )
    {
      if ( arg )
      {
        size_t len = 0;
        const char *optname;

        len = strlen( xvtable->name );
        optname = arg + len;
        if ( *optname == ':' )
          optname++;
        else
          optname = NULL;
        if ( ( 0 == strcmp( arg, "%" ) || ( 0 == strncmp( xvtable->name, arg, len ) && ( arg[len] == 0 || arg[len] == ':' ) ) ) )
        {
          int *ashown;
          int ss = DUF_OPTION_VAL_MAX_LONG * sizeof( int );

          ashown = mas_malloc( ss );
          memset( ( void * ) ashown, 0, ss );
          if ( !title_printed++ )
            DUF_PRINTF( 0, "# set '%-15s'", xvtable->name );
          for ( const muc_longval_extended_t * xtended = xvtable->xlist; xtended->o.name; xtended++ )
          {
            duf_option_code_t codeval;

            codeval = xtended->o.val;
            if ( !optname || ( *optname && 0 == strcmp( optname, xtended->o.name ) ) )
            {
              char *sl = NULL;

              const char *srelto[] = {
              /* [MUC_OFFSET_config] = "config",   */
              /* [MUC_OFFSET_ufilter] = "ufilter", */
                [MUC_OFFSET_funcptr] = "funcptr",
                [MUC_OFFSET_varptr] = "varptr",
              };
              sl = muc_optstages_list( duf_get_config_cli(  ), xtended, xvtable );
              if ( ashown[codeval] <= 0 )
                duf_show_option_description_x( xtended );
              ashown[codeval]++;
              DUF_PRINTF( 0, ".\t--%-20s;", xtended->o.name );
              DUF_PRINTF( 0, ". [%s] %d:%d:%d "                      /*"%2d( %-9s ):%6d( %-13s ): %lx" */
                          "; %d:%d * \t| %-40s; {%-10s:%-10s}",      /* */
                          sl,                                        /* */
                          xtended->stage_opts.stage.min, xtended->stage_opts.stage.max, xtended->stage_opts.use_stage, /* */
                          xtended->unset, xtended->can_no,           /* xtended->stage_opts.use_stage, xtended->stage_opts.use_stage_mask, *//* */
                          xtended->help,                             /* */
                          muc_optclass_name( xtended->oclass ), oclass_titles[xtended->oclass] );
              if ( xtended->vtype != MUC_OPTION_VTYPE_NONE )
                DUF_PRINTF( 0, ". `%s`;", muc_optvtype2string( xtended->vtype ) );
              if ( xtended->m_hasoff )
              {
                DUF_PRINTF( 0, ".%s + %-4lu & %lx;", srelto[xtended->relto] ? srelto[xtended->relto] : "-", xtended->m_offset,
                            1L << xtended->flag_bitnum );
              }
              if ( xtended->call.funcname || xtended->calltype != MUC_OPTION_CALL_TYPE_NONE )
              {
                const char *sfargs = NULL;

                DUF_PRINTF( 0, ". %s::%s( ", muc_extended_call_type2string( xtended->calltype ), /* */
                            xtended->call.funcname ? xtended->call.funcname : "-" );

                switch ( xtended->calltype )
                {
                case MUC_OPTION_CALL_TYPE_NONE:
                  break;
                case MUC_OPTION_CALL_TYPE_EIA:
                  sfargs = "int num_from_tab";
                  break;
                case MUC_OPTION_CALL_TYPE_EV:
                  sfargs = "void";
                  break;
                case MUC_OPTION_CALL_TYPE_A:
                  sfargs = "int cargc, const char **cargv";
                  break;
                case MUC_OPTION_CALL_TYPE_AA:
                  sfargs = "mas_cargvc_t carg";
                  break;
                case MUC_OPTION_CALL_TYPE_N:
                  sfargs = "long optarg";
                  break;
                case MUC_OPTION_CALL_TYPE_S:
                  sfargs = "char *optarg";
                  break;
                case MUC_OPTION_CALL_TYPE_SAS:
                  sfargs = "const char *str_from_tab";
                  break;
                case MUC_OPTION_CALL_TYPE_SAN:
                  sfargs = "char *optarg, num_from_tab";
                  break;
                case MUC_OPTION_CALL_TYPE_TN1:
                  sfargs = "mas_argvc_t targ, long optarg_with_units";
                  break;
                case MUC_OPTION_CALL_TYPE_TS1:
                  sfargs = "mas_argvc_t targ, optarg";
                  break;
                case MUC_OPTION_CALL_TYPE_TS2:
                  sfargs = "int targc, char **targv, optarg";
                  break;
                }
                DUF_PRINTF( 0, ".%s )", sfargs ? sfargs : "...." );
              }
              DUF_PUTSL( 0 );
              mas_free( sl );
            }
          }
          mas_free( ashown );
        }
      }
      else
        DUF_PRINTF( 0, "* set '%-15s'", xvtable->name );
    }
  }

  DUF_ENDR( r );
}
