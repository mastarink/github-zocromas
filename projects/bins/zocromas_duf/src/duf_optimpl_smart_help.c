#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"


#include "duf_config_ref.h"
#include "duf_config_defs.h"
#include "duf_config_util.h"

#include "duf_action_table.h"
#include "duf_sccb.h"

#include "duf_xtended_table.h"

#include "duf_options_file.h"   /* duf_options_infilepath */

#include "duf_option_descr.h"
#include "duf_option_stage.h"
#include "duf_option_extended.h"
#include "duf_option_extended2string.h"
#include "duf_option_lfind.h"

#include "duf_option_names.h"
#include "duf_option_class.h"
#include "duf_option_vtype.h"

/* ###################################################################### */
#include "duf_optimpl_smart_help.h"
/* ###################################################################### */

static const char *oclass_titles[DUF_OPTION_CLASS_MAX + 1] = {
  [DUF_OPTION_CLASS_HELP] = "Help",
  [DUF_OPTION_CLASS_NO_HELP] = "No help",
  [DUF_OPTION_CLASS_SYSTEM] = "System",
  [DUF_OPTION_CLASS_CONTROL] = "Control",
  [DUF_OPTION_CLASS_DB] = "Database",
  [DUF_OPTION_CLASS_FS] = "File system",
  [DUF_OPTION_CLASS_SCCB] = "SCCB system",
  [DUF_OPTION_CLASS_REFERENCE] = "Reference",
  [DUF_OPTION_CLASS_COLLECT] = "Collect",
  [DUF_OPTION_CLASS_SCAN] = "Scan",
  [DUF_OPTION_CLASS_FILTER] = "Filter",
  [DUF_OPTION_CLASS_UPDATE] = "Update",
  [DUF_OPTION_CLASS_REQUEST] = "Request",
  [DUF_OPTION_CLASS_PRINT] = "Print",
  [DUF_OPTION_CLASS_TRACE] = "Trace",
  [DUF_OPTION_CLASS_OBSOLETE] = "Obsolete",
  [DUF_OPTION_CLASS_OTHER] = "Other",
  [DUF_OPTION_CLASS_NONE] = "None",
  [DUF_OPTION_CLASS_DEBUG] = "DEBUG",
  [DUF_OPTION_CLASS_NODESC] = "No desc",
};

mas_error_code_t
duf_option_O_smart_help_all( duf_option_class_t oclass )
{
  DEBUG_STARTR( r );

  if ( oclass == DUF_OPTION_CLASS_ALL )
  {
    for ( duf_option_class_t oc = DUF_OPTION_CLASS_MIN + 1; oc < DUF_OPTION_CLASS_MAX; oc++ )
    {
      DOR( r, duf_option_O_smart_help( oc ) );
    }
  }
  DEBUG_ENDR( r );
}

static void
duf_show_option_description_x( const duf_longval_extended_t * extended )
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

    /* duf_option_class_t hclass; */

    /* hclass = duf_help_option2class( codeval ); */
    s = duf_xoption_description_d( extended, "\t", " // " );
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

static void
duf_show_option_description( int ilong )
{
  int look = 1;
  const char *name;
  duf_option_code_t codeval;

  name = DUF_CONFIGG( cli.longopts_table )[ilong].name;
  codeval = DUF_CONFIGG( cli.longopts_table )[ilong].val;

  /* DUF_PRINTF( 0, "<><><><><><cnd:%d> %d: ie:%d oc:%d '%s'; %u", cnd, ilong, ie, oclass, name, codeval ); */
  if ( DUF_CONFIGG( help_string ) )
  {
    char *s = DUF_CONFIGG( help_string );

    look = ( ( s && *s && !s[1] && codeval == *s ) || ( 0 == strcmp( s, name ) ) /* OR: else if ( strstr( name, s ) ) */  );
  }
  if ( look )
  {
    char *s = NULL;

    /* duf_option_class_t hclass; */

    /* hclass = duf_help_option2class( codeval ); */
    s = duf_loption_description_d( ilong, "\t", " // " );
    /* s = mas_strcat_x( s, " ...................." ); */
    if ( s )
    {
      /* if ( shown >= 0 )                    */
      /*   DUF_PRINTF( 0, " ## %d;", shown ); */
      /* DUF_PRINTF( 0, "(%d;s[%d]=%d) %d. [%u] \t%s", shown, ie, ashown[ie], ilong, codeval, s ); */
      DUF_PRINTF( 0, "%d. [%u] \t%s", ilong, codeval, s );
      mas_free( s );
    }
    else
    {
      DUF_PRINTF( 0, " ??? %s", name );
    }
  }
}

mas_error_code_t
duf_option_O_smart_help( duf_option_class_t oclass )
{
  DEBUG_STARTR( r );

  int *ashown;
  size_t ss;
  int tbcount;

  tbcount = duf_longindex_extended_count( duf_extended_table_multi(  ) );
  ss = DUF_OPTION_VAL_MAX_LONG * sizeof( int );

  ashown = mas_malloc( ss );
  memset( ( void * ) ashown, 0, ss );
  /* for ( int ilong = 0; DUF_CONFIGG(cli.longopts_table)[ilong].name && ilong < lo_extended_count; ilong++ ) */
  /* {                                                                                                   */
  /* }                                                                                                   */
  if ( oclass <= DUF_OPTION_CLASS_MAX && oclass_titles[oclass] && *oclass_titles[oclass] )
    DUF_PRINTF( 0, "-=-=-=-=- %s (%s) -=-=-=-=-", oclass_titles[oclass], duf_optclass_name( oclass ) );
  else
    DUF_PRINTF( 0, "-=-=-=-=- <no title set for %d> -=-=-=-=-", oclass );
  for ( int ilong = 0; DUF_NOERROR( r ) && DUF_CONFIGG( cli.longopts_table )[ilong].name && ilong < tbcount; ilong++ )
  {
    duf_option_code_t codeval;
    const duf_longval_extended_t *extd;
    int ie;

    codeval = DUF_CONFIGG( cli.longopts_table )[ilong].val;
    /* extended = _duf_find_longval_extended( codeval ); */
    extd = duf_loption_xfind_at_stdx( ilong, ( const duf_longval_extended_table_t ** ) NULL, NULL /* &no */  );
    /* ie = extended ? extended - &lo_extended[0] : -1; */
    ie = codeval;
    if ( codeval && DUF_NOERROR( r ) )
    {
      int cnd = 0;

      cnd = ( !extd && ( oclass == DUF_OPTION_CLASS_ANY || oclass == DUF_OPTION_CLASS_NODESC ) )
            || ( extd && ( oclass == DUF_OPTION_CLASS_ANY || oclass == extd->oclass ) );
      if ( cnd )
      {
        int shown = -1;

        if ( ie >= 0 )
          shown = ashown[ie];
        if ( shown <= 0 )
          duf_show_option_description( ilong );
        if ( ie >= 0 )
          ashown[ie]++;
      }
    }
  }
  mas_free( ashown );

  DEBUG_ENDR( r );
}

mas_error_code_t
duf_option_O_help_set( const char *arg )
{
  DEBUG_STARTR( r );

  for ( const duf_longval_extended_table_t ** xtables = duf_extended_table_multi(  ); *xtables; xtables++ )
  {
    const duf_longval_extended_table_t *xtable = *xtables;
    int title_printed = 0;

    if ( xtable && xtable->name )
    {
      if ( arg )
      {
        size_t len = 0;
        const char *optname;

        len = strlen( xtable->name );
        optname = arg + len;
        if ( *optname == ':' )
          optname++;
        else
          optname = NULL;
        if ( ( 0 == strcmp( arg, "%" ) || ( 0 == strncmp( xtable->name, arg, len ) && ( arg[len] == 0 || arg[len] == ':' ) ) ) )
        {
          int *ashown;
          int ss = DUF_OPTION_VAL_MAX_LONG * sizeof( int );

          ashown = mas_malloc( ss );
          memset( ( void * ) ashown, 0, ss );
          if ( !title_printed++ )
            DUF_PRINTF( 0, "# set '%-15s'", xtable->name );
          for ( const duf_longval_extended_t * xtended = xtable->table; xtended->o.name; xtended++ )
          {
            duf_option_code_t codeval;

            codeval = xtended->o.val;
            if ( !optname || ( *optname && 0 == strcmp( optname, xtended->o.name ) ) )
            {
              char *sl = NULL;

              const char *srelto[] = {
                [DUF_OFFSET_config] = "config",
                [DUF_OFFSET_ufilter] = "ufilter",
              };
              sl = duf_optstages_list( xtended, xtable );
              if ( ashown[codeval] <= 0 )
                duf_show_option_description_x( xtended );
              ashown[codeval]++;
              DUF_PRINTF( 0, ".\t--%-20s;", xtended->o.name );
              DUF_PRINTF( 0, ". [%s] " /*"%2d( %-9s ):%6d( %-13s ): %lx" */
                          "; %d:%d * \t| %-40s; {%-10s:%-10s}", /* */
                          sl,   /* */
                          xtended->invert, xtended->can_no, /* xtended->use_stage, xtended->use_stage_mask, *//* */
                          xtended->help, /* */
                          duf_optclass_name( xtended->oclass ), oclass_titles[xtended->oclass] );
              if ( xtended->vtype != DUF_OPTION_VTYPE_NONE )
                DUF_PRINTF( 0, ". `%s`;", duf_vtype2string( xtended->vtype ) );
              if ( xtended->m_hasoff )
              {
                DUF_PRINTF( 0, ".%s + %-4lu & %x;", srelto[xtended->relto] ? srelto[xtended->relto] : "-", xtended->m_offset, xtended->afl.bit );
              }
              if ( xtended->call.funcname || xtended->calltype != DUF_OPTION_CALL_TYPE_NONE )
              {
                const char *sfargs = NULL;

                DUF_PRINTF( 0, ". %s::%s( ", duf_extended_call_type2string( xtended->calltype ), /* */
                            xtended->call.funcname ? xtended->call.funcname : "-" );

                switch ( xtended->calltype )
                {
                case DUF_OPTION_CALL_TYPE_NONE:
                  break;
                case DUF_OPTION_CALL_TYPE_EIA:
                  sfargs = "int num_from_tab";
                  break;
                case DUF_OPTION_CALL_TYPE_EV:
                  sfargs = "void";
                  break;
                case DUF_OPTION_CALL_TYPE_A:
                  sfargs = "int cargc, const char **cargv";
                  break;
                case DUF_OPTION_CALL_TYPE_AA:
                  sfargs = "mas_cargvc_t carg";
                  break;
                case DUF_OPTION_CALL_TYPE_N:
                  sfargs = "long optarg";
                  break;
                case DUF_OPTION_CALL_TYPE_S:
                  sfargs = "char *optarg";
                  break;
                case DUF_OPTION_CALL_TYPE_SAS:
                  sfargs = "const char *str_from_tab";
                  break;
                case DUF_OPTION_CALL_TYPE_SAN:
                  sfargs = "char *optarg, num_from_tab";
                  break;
                case DUF_OPTION_CALL_TYPE_TN1:
                  sfargs = "mas_argvc_t targ, long optarg_with_units";
                  break;
                case DUF_OPTION_CALL_TYPE_TS1:
                  sfargs = "mas_argvc_t targ, optarg";
                  break;
                case DUF_OPTION_CALL_TYPE_TS2:
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
        DUF_PRINTF( 0, "* set '%-15s'", xtable->name );
    }
  }

  DEBUG_ENDR( r );
}