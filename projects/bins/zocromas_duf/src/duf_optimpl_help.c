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
#include <mastar/multiconfig/muc_option_names.h>
#include <mastar/multiconfig/muc_option_descr.h>
#include <mastar/multiconfig/muc_option_config.h>

/* #include "duf_tracen_defs.h"                                         (* MAST_TRACE ♠ *) */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ *) */

/* #include "duf_start_end.h"                                           (* DUF_STARTR ; DUF_ENDR ♠ *) */
/* #include "duf_dodefs.h"                                              (* DOR ♠ *) */

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

/* #include "duf_output_defs.h" */
#include "duf_printn_defs.h"                                         /* DUF_PRINTF etc. ✗ */

/* #include "duf_expandable.h"                                          (* duf_expandable_string_t; duf_string_expanded ♠ *) */

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_ref.h"
#include "duf_config_defs.h"                                         /* DUF_CONF... ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */
/* #include "duf_config_output_util.h" */

/* #include "duf_action_table.h" */
/* #include "duf_sccb.h" */

/* #include "duf_xtended_table.h" */

/* #include "duf_option_descr.h" */
/* #include "duf_option_stage.h"                                        (* duf_optstage_name ♠ *) */
/* #include "duf_option_extended.h" */

/* #include "duf_option_names.h"                                        (* duf_coption_names_d etc... ♠ *) */
/* #include "duf_option_class.h"                                        (* duf_optclass2string ♠ *) */
/* #include "duf_option_config.h"                                       (* duf_get_cli_options_trace_config ♠ *) */

#include "duf_optimpl_enum.h"                                        /* duf_option_code_t ✗ */
#include "duf_optimpl_extended2string.h"

/* ###################################################################### */
#include "duf_optimpl_help.h"
/* ###################################################################### */

#if 0
# define DUF_H2C( codename, val ) case DUF_OPTION_HELP_ ## codename: val=DUF_OPTIMPL_CLASS_ ## codename;
/*
 * if arg is help option
 * return class id for options to display the help
 * */
static duf_optimpl_class_t __attribute__ ( ( unused ) ) duf_help_option2class( duf_option_code_t codeval )
{
  duf_optimpl_class_t rv = DUF_OPTIMPL_CLASS_BAD;

/*
  DUF_OPTIMPL_CLASS_NONE,

  DUF_OPTIMPL_CLASS_HELP,
  DUF_OPTIMPL_CLASS_TEST,
  DUF_OPTIMPL_CLASS_SYSTEM,
  DUF_OPTIMPL_CLASS_CONTROL,
  DUF_OPTIMPL_CLASS_REFERENCE,
  DUF_OPTIMPL_CLASS_COLLECT,
  DUF_OPTIMPL_CLASS_SCAN,
  DUF_OPTIMPL_CLASS_UPDATE,
  DUF_OPTIMPL_CLASS_REQUEST,
  DUF_OPTIMPL_CLASS_PRINT,
  
  DUF_OPTIMPL_CLASS_TRACE,
  DUF_OPTIMPL_CLASS_DEBUG,
  DUF_OPTIMPL_CLASS_OBSOLETE,
  DUF_OPTIMPL_CLASS_NODESC,
  DUF_OPTIMPL_CLASS_OTHER,

*/

  switch ( codeval )
  {
    DUF_H2C( NONE, rv );
    break;
    DUF_H2C( HELP, rv );
    break;
    DUF_H2C( SYSTEM, rv );
    break;
    DUF_H2C( CONTROL, rv );
    break;
    DUF_H2C( REFERENCE, rv );
    break;
    DUF_H2C( COLLECT, rv );
    break;
    DUF_H2C( SCAN, rv );
    break;
    DUF_H2C( UPDATE, rv );
    break;
    DUF_H2C( REQUEST, rv );
    break;
    DUF_H2C( PRINT, rv );
    break;
    DUF_H2C( TRACE, rv );
    break;
    DUF_H2C( DEBUG, rv );
    break;
    DUF_H2C( OBSOLETE, rv );
    break;
    DUF_H2C( NODESC, rv );
    break;
    DUF_H2C( OTHER, rv );
    break;
    DUF_H2C( ALL, rv );
    break;
  case DUF_OPTION_OCLASS_HELP:
    rv = DUF_OPTIMPL_CLASS_ANY;
    break;
  default:
    break;
  }
  return rv;
}
#endif

SR( SNIPPET_OPTION, optimpl_O_help, /* int argc, char *const *argv */ void )
{
/* DUF_STARTR( r ); */

  DUF_PRINTF( 0, "Usage: %s [OPTION]... [PATH]...", DUF_CONFIGG( pcli->carg.argv )[0] );
  DUF_PRINTF( 0, "  -H, --help			[%s]", muc_coption_xfind_desc_at_stdx( duf_get_config_cli(  ), DUF_OPTION_VAL_HELP, QPERRIND ) );
  DUF_PRINTF( 0, "  -h, --help-class-help	[%s]", muc_coption_xfind_desc_at_stdx( duf_get_config_cli(  ), DUF_OPTION_VAL_OCLASS_HELP, QPERRIND ) );
  DUF_PRINTF( 0, "  -x, --example		[%s]", muc_coption_xfind_desc_at_stdx( duf_get_config_cli(  ), DUF_OPTION_VAL_EXAMPLES, QPERRIND ) );
  DUF_PRINTF( 0, "  --output-level		[%s]", muc_coption_xfind_desc_at_stdx( duf_get_config_cli(  ), DUF_OPTION_VAL_OUTPUT_LEVEL, QPERRIND ) );
  DUF_PRINTF( 0, "Database ----------" );
  DUF_PRINTF( 0, "  -N, --db-name=%s", DUF_CONFIGGSP( db.main.name_x ) );
  DUF_PRINTF( 0, "  -D, --db-directory=%s", DUF_CONFIGGSP( db.dir_x ) );
  DUF_PRINTF( 0, "  --allow-drop-tables		DANGEROUS!" );
  DUF_PRINTF( 0, "  --allow-remove-database		DANGEROUS!" );
  DUF_PRINTF( 0, "  --allow-create-tables" );
  DUF_PRINTF( 0, "  -V, --allow-vacuum		vacuum" );
  DUF_PRINTF( 0, "Manipulations --------------" );
  DUF_PRINTF( 0, "  --add-path" );
  DUF_PRINTF( 0, "  -R, --recursive		recursive" );
  DUF_PRINTF( 0, "  --uni-scan" );
  DUF_PRINTF( 0, "    --md5" );
  DUF_PRINTF( 0, "    -f, --files" );
  DUF_PRINTF( 0, "    --tree" );
  DUF_PRINTF( 0, "    --print" );
  DUF_PRINTF( 0, "    --sample" );
  DUF_PRINTF( 0, "    --collect" );
  DUF_PRINTF( 0, "Old options" );
  DUF_PRINTF( 0, "  --zero-duplicates" );
/* DUF_PRINTF(0,  "  --update-duplicates"  ); */
  DUF_PRINTF( 0, "  --zero-filedatas" );
  DUF_PRINTF( 0, "  --update-filedatas" );
  DUF_PRINTF( 0, "  --update-mdpaths" );
#ifdef DUF_COMPILE_EXPIRED
  DUF_PRINTF( 0, "  --update-exif" );
#endif
  DUF_PRINTF( 0, "  --same-md5" );
  DUF_PRINTF( 0, "Debug ----------" );
  DUF_PRINTF( 0, "  --debug" );
  DUF_PRINTF( 0, "  -v, --verbose=%d", MASE_STD_VERBOSE_LEVEL );
  DUF_PRINTF( 0, "  --min-dbg-lines=%lu", DUF_CONFIGG( opt.dbg.max_line ) );
  DUF_PRINTF( 0, "  --max-dbg-lines=%lu", DUF_CONFIGG( opt.dbg.min_line ) );
  DUF_PRINTF( 0, "Trace ----------" );
#if 0
  DUF_PRINTF( 0, "  -A, --trace-action=%d", DUF_CONFIGG( opt.ptracecfg->action ) );
  DUF_PRINTF( 0, "  -C, --trace-temporary=%d", DUF_CONFIGG( opt.ptracecfg->temporary ) );
  DUF_PRINTF( 0, "  -S, --trace-scan=%d", DUF_CONFIGG( opt.ptracecfg->scan ) );
  DUF_PRINTF( 0, "  -Q, --trace-sql=%d", DUF_CONFIGG( opt.ptracecfg->sql ) );
  DUF_PRINTF( 0, "  --trace-sample=%d", DUF_CONFIGG( opt.ptracecfg->sample ) );
  DUF_PRINTF( 0, "  --trace-path=%d", DUF_CONFIGG( opt.ptracecfg->path ) );
  DUF_PRINTF( 0, "  -F, --trace-collect=%d", DUF_CONFIGG( opt.ptracecfg->collect ) );
#else
  DUF_PRINTF( 0, "  -A, --trace-action=%d", DUF_CONFIGG( opt.ptracecfg->class_levels[DUF_TRACE_LEVEL_action] ) );
  DUF_PRINTF( 0, "  -C, --trace-temporary=%d", DUF_CONFIGG( opt.ptracecfg->class_levels[DUF_TRACE_LEVEL_temporary] ) );
  DUF_PRINTF( 0, "  -S, --trace-scan=%d", DUF_CONFIGG( opt.ptracecfg->class_levels[DUF_TRACE_LEVEL_scan] ) );
  DUF_PRINTF( 0, "  -Q, --trace-sql=%d", DUF_CONFIGG( opt.ptracecfg->class_levels[DUF_TRACE_LEVEL_sql] ) );
  DUF_PRINTF( 0, "  --trace-sample=%d", DUF_CONFIGG( opt.ptracecfg->class_levels[DUF_TRACE_LEVEL_sample] ) );
  DUF_PRINTF( 0, "  --trace-path=%d", DUF_CONFIGG( opt.ptracecfg->class_levels[DUF_TRACE_LEVEL_path] ) );
  DUF_PRINTF( 0, "  -F, --trace-collect=%d", DUF_CONFIGG( opt.ptracecfg->class_levels[DUF_TRACE_LEVEL_collect] ) );
#endif
  DUF_PRINTF( 0, "----------------" );

/* DUF_ENDR( r ); */
  ER( SNIPPET_OPTION, optimpl_O_help, /* int argc, char *const *argv */ void );
}


duf_option_code_t
duf_flag2code( duf_config_act_flags_combo_t fset )
{
  duf_option_code_t rc = DUF_OPTION_VAL_NONE;

  typedef struct
  {
    duf_config_act_flags_combo_t test;
    const char *name;
    duf_option_code_t id;
  } duf_chk_act_flags_t;

#define CHECK_FLAG_ID_ROW(_l, _u) {{.flag._l = 1}, .name=#_l, .id=DUF_OPTION_VAL_FLAG_ ## _u}
  duf_chk_act_flags_t tab[] = {
    CHECK_FLAG_ID_ROW( info, INFO ),
    CHECK_FLAG_ID_ROW( allow_vacuum, ALLOW_VACUUM ),
    CHECK_FLAG_ID_ROW( allow_remove_database, ALLOW_REMOVE_DATABASE ),
    CHECK_FLAG_ID_ROW( allow_drop_tables, ALLOW_DROP_TABLES ),
    CHECK_FLAG_ID_ROW( allow_clean_tables, ALLOW_CLEAN_TABLES ),
    CHECK_FLAG_ID_ROW( allow_create_database, ALLOW_CREATE_DB ),
    CHECK_FLAG_ID_ROW( allow_create_tables, ALLOW_CREATE_TABLES ),

  /* 20150907.122929 renamed  FILES -> ALLOW_FILES */
    CHECK_FLAG_ID_ROW( allow_dirs, ALLOW_DIRECTORIES ),
    CHECK_FLAG_ID_ROW( allow_sub, ALLOW_SUB ),
  /* 20150907.122929 renamed  DIRS -> ALLOW_DIRS; 20151013.095014 -> ALLOW_DIRECTORIES */
    CHECK_FLAG_ID_ROW( allow_files, ALLOW_FILES ),

  /* CHECK_FLAG_ID_ROW( progress, PROGRESS ), */
  /* CHECK_FLAG_ID_ROW( use_binformat, USE_BINFORMAT ), */
    CHECK_FLAG_ID_ROW( summary, SUMMARY ),
    CHECK_FLAG_ID_ROW( interactive, INTERACTIVE ),

  /* CHECK_FLAG_ID_ROW( do_sccbs, DO_SCCBS ), */
    CHECK_FLAG_ID_ROW( fast, FAST ),
    CHECK_FLAG_ID_ROW( fresh, FRESH ),

    CHECK_FLAG_ID_ROW( add_path, ADD_PATH ),

    CHECK_FLAG_ID_ROW( dirent, DIRENT ),
  /* CHECK_FLAG_ID_ROW( filedata, FILEDATA ),   */
  /* CHECK_FLAG_ID_ROW( filenames, FILENAMES ), */

  /* CHECK_FLAG_ID_ROW( collect_obs, COLLECT ), */
  /* CHECK_FLAG_ID_ROW( sd5_obs, SD5 ),     */
  /* CHECK_FLAG_ID_ROW( md5_obs, MD5 ),     */
  /* CHECK_FLAG_ID_ROW( sha1_obs, MD5 ),     */
  /* CHECK_FLAG_ID_ROW( crc32_obs, CRC32 ), */
  /* CHECK_FLAG_ID_ROW( mime_obs, MIME ),   */
  /* CHECK_FLAG_ID_ROW( exif_obs, EXIF ),   */
  /* CHECK_FLAG_ID_ROW( mdpath_obs, MSPATH ),   */

  };
#if 0
  for ( typeof( fset.bit ) uf = 1; uf != 0; uf = uf << 1 )
  {
    duf_config_act_flags_combo_t tst;

    tst.bit = uf;
  /* if ( tst.bit == fset.bit ) */
    {
      DUF_PRINTF( 0, "%lx/%lx - %lx", ( unsigned long ) uf, ( unsigned long ) tst.bit, ( unsigned long ) fset.bit );
    }
  }
#endif
  for ( unsigned i = 0; i < sizeof( tab ) / sizeof( tab[0] ); i++ )
  {
    if ( tab[i].test.bit == fset.bit )
      rc = tab[i].id;
  }
  return rc;
}

duf_option_code_t
duf_uflag2code( unsigned long ufset )
{
  duf_config_act_flags_combo_t fset;
  duf_option_code_t id = DUF_OPTION_VAL_NONE;

  fset.bit = ufset;
  id = duf_flag2code( fset );
  return id;
}

static const char *
duf_uflag2cnames( unsigned long ufset )
{
  duf_option_code_t id = DUF_OPTION_VAL_NONE;

  id = duf_uflag2code( ufset );
  return id == DUF_OPTION_VAL_NONE ? "" : muc_coption_cnames_tmp( duf_get_config_cli(  ), -1, id, NULL );
}

static const char *
duf_unflag2cnames( unsigned unfset )
{
  return duf_uflag2cnames( 1 << unfset );
}

SR( SNIPPET_OPTION, optimpl_O_showflags, /* int argc, char *const *argv */ void )
{
/* DUF_STARTR( r ); */
  {
    typeof( duf_config->opt.act ) u = duf_config->opt.act;
  /* u.v.bit = 0;       */
  /* u.v.flag.info = 1; */
    /* DUF_PRINTF(0,"stage:%s; source:%s", muc_optstage_name(duf_get_config_cli()), muc_optsource_name(duf_get_config_cli())); */
    DUF_PRINTF( 0, "opt.act   [%2lu->%2lu]   %8lx :: ", sizeof( DUF_CONFIGG( opt.act.v ) ), sizeof( typeof( u.v.bit ) ),
                ( unsigned long ) DUF_CONFIGG( opt.act.v.bit ) );


      /* *INDENT-OFF*  */
    DUF_PRINTF( 0, "              ┌─  %s", duf_unflag2cnames( 26 ) );
    DUF_PRINTF( 0, "              │   ┌─  %s", duf_unflag2cnames( 24 ) );
    DUF_PRINTF( 0, "              │   │   ┌─  %s", duf_unflag2cnames( 22 ) );
    DUF_PRINTF( 0, "              │   │   │   ┌─  %s", duf_unflag2cnames( 20 ) );
    DUF_PRINTF( 0, "              │   │   │   │   ┌─  %s", duf_unflag2cnames( 18 ) );
    DUF_PRINTF( 0, "              │   │   │   │   │   ┌─  %s", duf_unflag2cnames( 16 ) );
    DUF_PRINTF( 0, "              │   │   │   │   │   │   ┌─  %s", duf_unflag2cnames( 14 ) );
    DUF_PRINTF( 0, "              │   │   │   │   │   │   │   ┌─  %s", duf_unflag2cnames( 12 ) );
    DUF_PRINTF( 0, "              │   │   │   │   │   │   │   │   ┌─  %s", duf_unflag2cnames( 10 ) );
    DUF_PRINTF( 0, "              │   │   │   │   │   │   │   │   │   ┌─  %s", duf_unflag2cnames( 8 ) );
    DUF_PRINTF( 0, "              │   │   │   │   │   │   │   │   │   │   ┌─  %s", duf_unflag2cnames( 6 ) );
    DUF_PRINTF( 0, "              │   │   │   │   │   │   │   │   │   │   │   ┌─  %s", duf_unflag2cnames( 4 ) );
    DUF_PRINTF( 0, "              │   │   │   │   │   │   │   │   │   │   │   │   ┌─  %s", duf_unflag2cnames( 2 ) );
    DUF_PRINTF( 0, "              │   │   │   │   │   │   │   │   │   │   │   │   │   ┌─  %s", duf_unflag2cnames( 0 ) );
    DUF_PRINTF( 0, "   ┌──────────┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴─┐" );
      /* *INDENT-ON*  */

    typeof( u.v.bit ) mask = ( ( typeof( u.v.bit ) ) 1 ) << ( ( sizeof( u.v.bit ) * 8 ) - 1 );

    DUF_PRINTF( 0, ".   │" );
    for ( unsigned i = 1; i < sizeof( u.v.bit ) * 8 + 1; i++ )
    {
#if 1
      DUF_PRINTF( 0, ".%s ", u.v.bit & mask ? "◆" : " " );
#else
      DUF_PRINTF( 0, "%016x %016x %d - %s", u.v.bit, mask, u.v.bit & mask ? 1 : 0, duf_unflag2cnames( i ) );
#endif
      u.v.bit <<= 1;
    }
    DUF_PRINTF( 0, "│" );
      /* *INDENT-OFF*  */
    DUF_PRINTF( 0, "   └────────────┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┘" );
    DUF_PRINTF( 0, "                │   │   │   │   │   │   │   │   │   │   │   │   └─ %s", duf_unflag2cnames( 1) );
    DUF_PRINTF( 0, "                │   │   │   │   │   │   │   │   │   │   │   └─ %s", duf_unflag2cnames( 3) );
    DUF_PRINTF( 0, "                │   │   │   │   │   │   │   │   │   │   └─ %s", duf_unflag2cnames( 5) );
    DUF_PRINTF( 0, "                │   │   │   │   │   │   │   │   │   └─ %s", duf_unflag2cnames( 7) );
    DUF_PRINTF( 0, "                │   │   │   │   │   │   │   │   └─ %s", duf_unflag2cnames( 9) );
    DUF_PRINTF( 0, "                │   │   │   │   │   │   │   └─ %s", duf_unflag2cnames( 11) );
    DUF_PRINTF( 0, "                │   │   │   │   │   │   └─ %s", duf_unflag2cnames( 13) );
    DUF_PRINTF( 0, "                │   │   │   │   │   └─ %s", duf_unflag2cnames( 15) );
    DUF_PRINTF( 0, "                │   │   │   │   └─ %s", duf_unflag2cnames( 17) );
    DUF_PRINTF( 0, "                │   │   │   └─ %s", duf_unflag2cnames( 19) );
    DUF_PRINTF( 0, "                │   │   └─ %s", duf_unflag2cnames( 21) );
    DUF_PRINTF( 0, "                │   └─ %s", duf_unflag2cnames( 23) );
    DUF_PRINTF( 0, "                └─ %s", duf_unflag2cnames( 25) );
      /* *INDENT-ON*  */
  }
  DUF_PRINTF( 0, ">>> %lx", ( ( unsigned long ) 1 ) << ( ( sizeof( unsigned long ) * 8 ) - 1 ) );

  {
    unsigned u = DUF_CONFIGG( vars.puz )->v.sbit;

    DUF_PRINTF( 0, "u   [%2lu->%2lu]   %8lx :: ", sizeof( DUF_CONFIGG( vars.puz )->v ), sizeof( typeof( u ) ),
                ( unsigned long ) DUF_CONFIGG( vars.puz )->v.sbit );

    typeof( u ) mask = ( ( typeof( u ) ) 1 ) << ( ( sizeof( u ) * 8 ) - 1 );

    DUF_PRINTF( 0, ".> > " );
    for ( unsigned i = 1; i < sizeof( u ) * 8 + 1; i++ )
    {
      DUF_PRINTF( 0, ".%c ", u & mask ? '+' : ' ' );
      u <<= 1;
    }
    DUF_PUTSL( 0 );
  }
  DUF_PRINTF( 0, "                                                                  └─ --recursive" );

  {
    unsigned u = DUF_CONFIGG( opt.disable.sbit );

    DUF_PRINTF( 0, "opt.disable   [%2lu->%2lu]   %8lx :: ", sizeof( DUF_CONFIGG( opt.disable ) ), sizeof( typeof( u ) ),
                ( unsigned long ) DUF_CONFIGG( opt.disable.sbit ) );

    typeof( u ) mask = ( ( typeof( u ) ) 1 ) << ( ( sizeof( u ) * 8 ) - 1 );

    DUF_PRINTF( 0, ".> > " );
    for ( unsigned i = 1; i < sizeof( u ) * 8 + 1; i++ )
    {
      DUF_PRINTF( 0, ".%c ", u & mask ? '+' : ' ' );
      u <<= 1;
    }
    DUF_PUTSL( 0 );
  }
  DUF_PRINTF( 0, "                                                              │ │ └─ --disable-calculate" );
  DUF_PRINTF( 0, "                                                              │ └─ --disable-insert" );
  DUF_PRINTF( 0, "                                                              └─ --disable-update" );
#if 0
  {
    duf_option_code_t id = DUF_OPTION_VAL_NONE;

    duf_config_act_flags_combo_t vv = {
      .flag.progress = 1
    };
    id = duf_flag2code( vv );
    DUF_PRINTF( 0, "id:%u - %s", id, muc_coption_cnames_tmp( -1, id, NULL ) );
    for ( unsigned u = 0; u < 30; u++ )
    {
      DUF_PRINTF( 0, "====================== %u. %s", u, duf_unflag2cnames( u ) );
    }
  }
#endif
/* DUF_ENDR( r ); */
  ER( SNIPPET_OPTION, optimpl_O_showflags, void );
}

SR( SNIPPET_OPTION, optimpl_O_list_options, long n_unused MAS_UNUSED )
{
/* DUF_STARTR( r ); */

  int ntable = 0;
  int tbcount = 0;

  for ( muc_longval_extended_vtable_t ** xvtables = muc_cli_options_xvtable_multi( duf_get_config_cli(  ) ); *xvtables; xvtables++, ntable++ )
  {
    const muc_longval_extended_vtable_t *xvtable = *xvtables;

    for ( const muc_longval_extended_t * xtended = xvtable->xlist; xtended->o.name; xtended++, tbcount++ )
    {
      char *s = NULL;

      if ( xtended->o.val )
        s = muc_xoption_description_d( duf_get_config_cli(  ), xtended, "\t", " // " );
    /* DUF_TEST_R( r ); */

      MAST_TRACE( options, 5, "@li2ex %d [%s]", ntable, xtended->o.name );
    /* DUF_PRINTF( 0, "[%ld] %3d (%2d) %4d %d:%d\t--%-40s", n, tbcount, ntable, xtended->o.val, xtended->stage_opts.stage.min, xtended->stage_opts.stage.max, xtended->o.name ); */
      DUF_PRINTF( 0, "%3d (%2d) %4d %d:%d\t--%-40s - %s", tbcount, ntable, xtended->o.val, xtended->stage_opts.stage.min,
                  xtended->stage_opts.stage.max, xtended->o.name, s ? s : "" );
      mas_free( s );
    }
  }
/* DUF_ENDR( r ); */
  ER( SNIPPET_OPTION, optimpl_O_list_options, long n_unused MAS_UNUSED );
}

SR( SNIPPET_OPTION, optimpl_O_list_xtended, const char *s )
{
/* DUF_STARTR( r ); */
  duf_stdx_print( duf_get_config_cli(  ), s );
/* DUF_ENDR( r ); */
  ER( SNIPPET_OPTION, optimpl_O_list_xtended, const char *s );
}
