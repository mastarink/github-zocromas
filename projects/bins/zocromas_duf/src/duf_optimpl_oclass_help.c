#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include <mastar/multiconfig/muc_options_file.h>
/* #include <mastar/multiconfig/muc_option_names.h> */
#include <mastar/multiconfig/muc_option_descr.h>
#include <mastar/multiconfig/muc_option_vtype.h>
#include <mastar/multiconfig/muc_option_stage.h>
#include <mastar/multiconfig/muc_option_lfind.h>
#include <mastar/multiconfig/muc_option_config.h>

#include "duf_optimpl_class.h"

/* #include "duf_tracen_defs.h"                                         (* MAST_TRACE ♠ *) */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ *) */

/* #include "duf_start_end.h"                                           (* DUF_STARTR ; DUF_ENDR ♠ *) */
/* #include "duf_dodefs.h"                                              (* DOR ♠ *) */

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

/* #include "duf_output_defs.h" */
#include "duf_printn_defs.h"                                         /* DUF_PRINTF etc. ✗ */

#include "duf_config.h"                                              /* duf_get_config ✗ */
/* #include "duf_config_ref.h" */
#include "duf_config_defs.h"                                         /* DUF_CONF... ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */
/* #include "duf_config_output_util.h" */

/* #include "duf_action_table.h"                                        (* duf_actions_table_std ✗ *) */
/* #include "duf_sccb.h" */

/* #include "duf_xtended_table.h" */

/* #include "duf_optimpl_extended2string.h" */
#include "duf_optimpl_enum.h"                                        /* duf_option_code_t ✗ */

/* ###################################################################### */
#include "duf_optimpl_oclass_help.h"
/* ###################################################################### */

SR( SNIPPET_OPTION, optimpl_O_oclass_help_all, duf_optimpl_class_t oclass )
{
/* DUF_STARTR( r ); */

  if ( oclass == DUF_OPTIMPL_CLASS_ALL )
  {
    for ( duf_optimpl_class_t oc = DUF_OPTIMPL_CLASS_MIN + 1; oc < DUF_OPTIMPL_CLASS_MAX; oc++ )
    {
      CR( optimpl_O_oclass_help, oc );
    }
  }
  else
  {
    CR( optimpl_O_oclass_help, oclass );
  }
/* DUF_ENDR( r ); */
  ER( SNIPPET_OPTION, optimpl_O_oclass_help_all, duf_optimpl_class_t oclass );
}

#if 0
static void
duf_show_loption_description( int nserial )
{
  int look = 1;
  const char *name;
  duf_option_code_t codeval;

  name = DUF_CONFIGG( pcli->longopts_table )[nserial].name;
  codeval = DUF_CONFIGG( pcli->longopts_table )[nserial].val;

/* DUF_PRINTF( 0, "<><><><><><cnd:%d> %d: ie:%d oc:%d '%s'; %u", cnd, nserial, ie, oclass, name, codeval ); */
  if ( DUF_CONFIGG( help_string ) )
  {
    char *s = DUF_CONFIGG( help_string );

    look = ( ( s && *s && !s[1] && codeval == *s ) || ( 0 == strcmp( s, name ) ) /* OR: else if ( strstr( name, s ) ) */  );
  }
  if ( look )
  {
    char *s = NULL;

  /* duf_optimpl_class_t hclass; */

  /* hclass = duf_help_option2class( codeval ); */
    s = duf_loption_description_d( nserial, "\t", " // " );
  /* s = mas_strcat_x( s, " ...................." ); */
    if ( s )
    {
    /* if ( shown >= 0 )                    */
    /*   DUF_PRINTF( 0, " ## %d;", shown ); */
    /* DUF_PRINTF( 0, "(%d;s[%d]=%d) %d. [%u] \t%s", shown, ie, ashown[ie], nserial, codeval, s ); */
      DUF_PRINTF( 0, " %02d. [%u] \t%s", nserial, codeval, s );
      mas_free( s );
    }
    else
    {
      DUF_PRINTF( 0, " ??? %s", name );
    }
  }
}
#endif
static void
duf_show_xoption_description( const muc_longval_extended_t * extended, int nserial, int shown )
{
  int look = 1;
  const char *name;
  duf_option_code_t codeval;

/* name = DUF_CONFIGG( pcli->longopts_table )[nserial].name; */
  name = extended->o.name;
/* codeval = DUF_CONFIGG( pcli->longopts_table )[nserial].val; */
  codeval = extended->o.val;

/* DUF_PRINTF( 0, "<><><><><><cnd:%d> %d: ie:%d oc:%d '%s'; %u", cnd, nserial, ie, oclass, name, codeval ); */
  if ( DUF_CONFIGG( help_string ) )
  {
    char *s = DUF_CONFIGG( help_string );

    look = ( ( s && *s && !s[1] && codeval == *s ) || ( 0 == strcmp( s, name ) ) /* OR: else if ( strstr( name, s ) ) */  );
  }
  if ( look )
  {
    char *s = NULL;

  /* duf_optimpl_class_t hclass; */

  /* hclass = duf_help_option2class( codeval ); */
    s = muc_xoption_description_d( duf_get_config_cli(  ), extended, "\t", " // " );
  /* s = mas_strcat_x( s, " ...................." ); */
    if ( s )
    {
    /* if ( shown >= 0 )                    */
    /*   DUF_PRINTF( 0, " ## %d;", shown ); */
    /* DUF_PRINTF( 0, "(%d;s[%d]=%d) %d. [%u] \t%s", shown, ie, ashown[ie], nserial, codeval, s ); */
      if ( shown >= 0 )
        DUF_PRINTF( 0, "%c  %02d. [%u] \t%s", shown > 0 ? '*' : ' ', nserial, codeval, s );
      mas_free( s );
    }
    else
    {
      DUF_PRINTF( 0, " ??? %s", name );
    }
  }
}

SR( SNIPPET_OPTION, oclass_extended_help, const muc_longval_extended_t * extended, duf_optimpl_class_t oclass, int nserial, int *pshown )
{
/* int ie; */
/* duf_option_code_t codeval; */

/* codeval = DUF_CONFIGG( pcli->longopts_table )[nserial].val; */
  assert( extended );
/* assert( codeval == extended->o.val ); */
/* extended = _duf_find_longval_extended( codeval ); */
/* ie = extended ? extended - &lo_extended[0] : -1; */
/* ie = codeval; */
  if ( extended->o.val && QNOERR )
  {
    int cnd = 0;

    cnd = ( !extended && ( oclass == DUF_OPTIMPL_CLASS_ANY || oclass == DUF_OPTIMPL_CLASS_NODESC ) )
            || ( extended && ( oclass == DUF_OPTIMPL_CLASS_ANY || oclass == extended->oclass ) );
    if ( cnd )
    {
    /* int shown = -1; */

    /* if ( codeval >= 0 ) */
    /* shown = ashown[codeval]; */
      if ( *pshown <= 0 )
#if 0
        duf_show_loption_description( nserial );
#else
        duf_show_xoption_description( extended, nserial, *pshown );
#endif
    /* if ( codeval >= 0 ) */
      ( *pshown )++;
    }
    else
    {
    }
  }
  ER( SNIPPET_OPTION, oclass_extended_help, const muc_longval_extended_t * extended, duf_optimpl_class_t oclass, int nserial, int *pshown );
}

/* */
SR( SNIPPET_OPTION, optimpl_O_oclass_help, duf_optimpl_class_t oclass )
{
/* DUF_STARTR( r ); */

  int *ashown;
  size_t ss;
  int nserial = 0;

#if 0
  ss = DUF_OPTION_VAL_MAX_LONG * sizeof( int );
#else
  ss = ( muc_cli_options_get_maxcodeval( duf_get_config_cli(  ) ) + 1 ) * sizeof( int );
#endif
  ashown = mas_malloc( ss );
  memset( ( void * ) ashown, 0, ss );
/* for ( int nserial = 0; DUF_CONFIGG(pcli->longopts_table)[nserial].name && nserial < lo_extended_count; nserial++ ) */
/* {                                                                                                   */
/* }                                                                                                   */
  if ( oclass <= DUF_OPTIMPL_CLASS_MAX && duf_optclass_title( oclass ) )
    DUF_PRINTF( 0, "-=-=-=-=- %s (%s,%s) -=-=-=-=-", duf_optclass_title( oclass ), duf_optclass_name( oclass ), duf_optclass2string( oclass ) );
  else
    DUF_PRINTF( 0, "-=-=-=-=- <no title set for %d> -=-=-=-=-", oclass );
  {
    for ( muc_longval_extended_vtable_t ** xtables = muc_cli_options_xvtable_multi( duf_get_config_cli(  ) ); *xtables; xtables++ )
      for ( const muc_longval_extended_t * xtended = ( *xtables )->xlist; xtended->o.name; nserial++, xtended++ )
      {
      /* const muc_longval_extended_t *extended; */

      /* extended = muc_loption_xfind_at_stdx( duf_get_config_cli(  ), nserial, ( const muc_longval_extended_vtable_t ** ) NULL, NULL (* &no *)  ); */
      /* assert( xtended == extended ); */
        CR( oclass_extended_help, xtended, oclass, nserial, &( ashown[xtended->o.val] ) );
      }
  }
  mas_free( ashown );

/* DUF_ENDR( r ); */
  ER( SNIPPET_OPTION, optimpl_O_oclass_help, duf_optimpl_class_t oclass );
}

SR( SNIPPET_OPTION, optimpl_O_oclass_help_str, const char *osclass )
{
  QT( "%s: %d %d (%d)", osclass, duf_optclass_name2code( osclass ), duf_optclass_title2code( osclass ), DUF_OPTIMPL_CLASS_NONE  );
  ER( SNIPPET_OPTION, optimpl_O_oclass_help_str, const char *osclass );
}
