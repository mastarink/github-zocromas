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
  char *name DUF_UNUSED = NULL;
  char *arg DUF_UNUSED = NULL;
  char *string;

  string = mas_strdup( pod->string );
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
      barg++;
    break;
  default:
    barg = endn = strchr( string, '=' );
    if ( barg )
      barg++;
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
  pod->name = name;
  pod->optarg = arg;
  mas_free( string );
  ER( OPTIONS, split_string_od, duf_option_data_t * pod );
}

SR( OPTIONS, soption_xclarify_new_at_xarr_od, const char *string DUF_UNUSED,
    const duf_longval_extended_t * xarr DUF_UNUSED, int all_matched DUF_UNUSED, duf_option_data_t * pod DUF_UNUSED )
{
  for ( const duf_longval_extended_t * xtended = xarr; xtended->o.name; xtended++ )
  {
    if ( 0 == strcmp( pod->name, xtended->o.name ) )
    {
      T( "$$$$ %s:%s:%s -- %s", pod->name, pod->optarg, pod->string, xtended->o.name );
    }
  }

  ER( OPTIONS, soption_xclarify_new_at_xarr_od, const char *string DUF_UNUSED,
      const duf_longval_extended_t * xarr DUF_UNUSED, int all_matched DUF_UNUSED, duf_option_data_t * pod );
}

SR( OPTIONS, soption_xclarify_new_at_xtable_od, const char *string, const duf_longval_extended_table_t * xtable,
    int all_matched, duf_option_data_t * pod )
{
  CR( soption_xclarify_new_at_xarr_od, string, xtable->table, all_matched, pod );

  ER( OPTIONS, soption_xclarify_new_at_xtable_od, const char *string, const duf_longval_extended_table_t * xtable,
      int all_matched, duf_option_data_t * pod );
}

SR( OPTIONS, soption_xclarify_new_at_multix_od, const char *string, const duf_longval_extended_table_t ** xtables, int all_matched,
    duf_option_data_t * pod )
{
  for ( ; *xtables; xtables++ )
  {
    const duf_longval_extended_table_t *xtable = *xtables;

    CR( soption_xclarify_new_at_xtable_od, string, xtable, all_matched, pod );
  }
  ER( OPTIONS, soption_xclarify_new_at_multix_od, const char *string, const duf_longval_extended_table_t ** xtables, int all_matched,
      duf_option_data_t * pod );
}

SR( OPTIONS, soption_xclarify_new_at_stdx_od, const char *string, int all_matched, duf_option_data_t * pod )
{
  CR( soption_xclarify_new_at_multix_od, string, duf_extended_table_multi(  ), all_matched, pod );

  ER( OPTIONS, soption_xclarify_new_at_stdx_od, const char *string, int all_matched, duf_option_data_t * pod );
}

SR( OPTIONS, soption_xclarify_new_at_stdx, const char *string, const char *name, const char *arg, duf_xclarifier_t clarifier, char vseparator,
    duf_option_stage_t istage, int all_matched, duf_option_source_t source )
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
  CR( soption_xclarify_new_at_stdx_od, string, all_matched, &od );
  mas_free( od.name );
  mas_free( od.optarg );
  ER( OPTIONS, soption_xclarify_new_at_stdx, const char *string, const char *name, const char *arg, duf_xclarifier_t clarifier, char vseparator,
      duf_option_stage_t istage, int all_matched, duf_option_source_t source );
}

SR( OPTIONS, soption_xclarify_new, const char *string, const char *name, const char *arg, char vseparator, duf_option_stage_t istage, int all_matched,
    duf_option_source_t source )
{
  CR( soption_xclarify_new_at_stdx, string, name, arg, DUF_WRAPPED( duf_xoption_clarify ), vseparator, istage, all_matched, source );

  ER( OPTIONS, soption_xclarify_new, const char *string, const char *name, const char *arg, char vseparator, duf_option_stage_t istage,
      int all_matched, duf_option_source_t source );
}
