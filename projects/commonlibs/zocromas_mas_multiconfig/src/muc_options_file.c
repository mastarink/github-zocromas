/* #undef MAS_TRACING */
/* #define MAST_TRACE_CONFIG muc_get_cli_options_trace_config(cli) */
#include "muc_tracen_defs_preset.h"
#include "muc_config_trace_enum.h"

#include "muc_errorn_defs_preset.h"

#include <assert.h>                                                  /* assert */
#include <string.h>
#include <errno.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ♣ */
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs.h>                             /* MASE_TEST_R; MASE_TEST_R_LOWERE; ... */
#include <mastar/error/mas_error_defs_make.h>                        /* MASE_MAKE_ERROR; MASE_MAKE_ERRORFL; MASE_MAKE_ERRORM  ... */

#include <mastar/tools/mas_utils_path.h>                             /* mas_normalize_path; mas_pathdepth; mas_realpath etc. ♣ */

#include "muc_se_only.h"                                             /* Only muc_DR; muc_SR; muc_ER; muc_CR; muc_QSTR; QERRIND; QERRNAME etc. ♠ */

#include "muc_option_config.h"                                       /* muc_get_cli_options_trace_config ♠ */
#include "muc_option_stage.h"                                        /* muc_optstage_name ♠ */
#include "muc_option_source.h"                                       /* muc_optsource_name ♠ */

#include "muc_option_clarify_batch.h"

#include "muc_config_cli_structs.h"
/* ###################################################################### */
#include "muc_options_file.h"
/* ###################################################################### */

/* TODO: remove static; possibly to «muc_config_cli_t * cli»  */
static char *config_file_path = NULL;

static void destructor_options_file( void ) __attribute__ ( ( destructor( 101 ) ) );
static void
destructor_options_file( void )
{
  mas_free( config_file_path );
}

const char *
muc_options_infilepath( muc_config_cli_t * cli MAS_UNUSED )
{
#if 0
  return DUF_CONFIGG( config_file_path );
#else
  return config_file_path;                                           /* TODO => ..._config_cli_t *cli */
#endif
}

/*
 * open configuration file
 *         with path combined from dir and filename, possibly prefixed by dot
 * ***********************
 * ?! replaces DUF_CONFIGG( config_file_path )
 * return FILE *, optionally indirectly return error code (errno)
 * */
static FILE *
muc_infilepath( muc_config_cli_t * cli MAS_UNUSED, const char *filepath, int *pry )
{
  FILE *f = NULL;

  f = fopen( filepath, "r" );
#if 0
  mas_free( DUF_CONFIGG( config_file_path ) );
  DUF_CONFIGWS( config_file_path, mas_strdup( filepath ) );
  MAST_TRACE( options, 230, "opened conf file %s %s", DUF_CONFIGG( config_file_path ), f ? "Ok" : "FAIL" );
#else
  mas_free( config_file_path );
  config_file_path = mas_strdup( filepath );
  MAST_TRACE( options, 250, "opened conf file %s %s", config_file_path, f ? "Ok" : "FAIL" );
#endif
  if ( pry )
    *pry = f ? 0 : -errno;
  return f;
}

static
#if 0
FILE *
muc_infile( muc_config_cli_t * cli MAS_UNUSED, int dot, const char *at, const char *filename, int *pr )
{
  int rpr = 0;
  int ry = 0;
  FILE *f = NULL;
  char *cfgpath = NULL;

  cfgpath = mas_normalize_path( at );

  assert( cfgpath );
  if ( dot )
    cfgpath = mas_strcat_x( cfgpath, "." );
  assert( cfgpath );
  cfgpath = mas_strcat_x( cfgpath, filename );
  assert( cfgpath );
  f = muc_infilepath( cli, cfgpath, &ry );
  if ( ry < 0 )
    DUF_MAKE_ERRORM( rpr, DUF_ERROR_OPEN, "file %s", cfgpath );
/* ERRMAKE_M(); */
  if ( pr )
    *pr = rpr;
  mas_free( cfgpath );
  return f;
}
#else
muc_SRP( OPTIONS, FILE *, f, NULL, infile, muc_config_cli_t * cli MAS_UNUSED, int dot, const char *at, const char *filename )
{
  int ry = 0;

  char *cfgpath = NULL;

  cfgpath = mas_normalize_path( at );

  assert( cfgpath );
  if ( dot )
    cfgpath = mas_strcat_x( cfgpath, "." );
  assert( cfgpath );
  cfgpath = mas_strcat_x( cfgpath, filename );
  assert( cfgpath );
  f = muc_infilepath( cli, cfgpath, &ry );
  if ( ry < 0 )
    ERRMAKE_M( OPEN, "file %s", cfgpath );
  mas_free( cfgpath );

  muc_ERP( OPTIONS, FILE *, f, NULL, infile, muc_config_cli_t * cli MAS_UNUSED, int dot, const char *at, const char *filename );
}
#endif

static
muc_SR( OPTIONS, infile_options_at_stream, muc_config_cli_t * cli, muc_option_stage_t istage, FILE * f, muc_option_source_t source,
        muc_option_adata_t * paod )
{

  MAST_TRACE( options, 20, "@@@@stream options; stage:%s; source:%s", muc_optstage_name( cli, istage ), muc_optsource_name( cli, source ) );
  while ( QNOERR && f && !feof( f ) )
  {
    char buffer[1024];
    char *s;

    s = fgets( buffer, sizeof( buffer ), f );
    if ( s )
    {
      s = mas_chomp( s );
      MAST_TRACE( options, 250, "@@@@read cmd '%s'", s );
      if ( s && ( ( *s == '#' ) || !*s ) )
        continue;
      MAST_TRACE( explain, 0, "read config line %s", s );
      {
        {
          if ( !MUC_IS_SOURCE( source, CFG ) && muc_optsource_labelled( cli, source ) )
          {
            MAST_TRACE( optsource, 0, "@@@@@@@@@%s:%s(%s)", muc_optstage_name( cli, istage ), muc_optsource_name( cli, source ),
                        muc_optsource_label( cli, source ) );
          }
          muc_CR( boption_xclarify_at_stdx, cli, 0 /* value_separator */ , istage, MUC_OPTION_SOURCE_DEFAULT( source, STREAM ), s, 0, paod );
        }
        muc_QTR;
      }
    }
  }
  muc_QTR;
  MAST_TRACE( options, 22, "@@@@stream options done; stage:%s; source:%s (%d:%s)", muc_optstage_name( cli, istage ),
              muc_optsource_name( cli, source ), QERRIND, QERRNAME );

  muc_ER( OPTIONS, infile_options_at_stream, muc_config_cli_t * cli, muc_option_stage_t istage, FILE * f, muc_option_source_t source,
          muc_option_adata_t * paod );
}

muc_SR( OPTIONS, infile_options_at_filepath, muc_config_cli_t * cli, muc_option_stage_t istage, const char *filepath, muc_option_adata_t * paod )
{

  int ry = 0;
  FILE *f = NULL;

  MAST_TRACE( options, 20, "@@@@filepath options; stage:%s; filepath:%s", muc_optstage_name( cli, istage ), filepath );
  f = muc_infilepath( cli, filepath, &ry );

  MAST_TRACE( options, 250, "to read config file %s", filepath );
  if ( f )
  {
    {
      muc_CR( infile_options_at_stream, cli, istage, f, MUC_OPTION_SOURCE_LABELLED( DUFFILE, filepath ), paod );
    }
    fclose( f );
    MAST_TRACE( options, 250, "read config file %s", filepath );
  }
  else
  {
    MAST_TRACE( options, 50, "@@fail to read config file %s", filepath );
    ERRMAKE_M( OPEN, "file '%s'", filepath );
  }
  MAST_TRACE( options, 22, "@@@@filepath options done; stage:%s; filepath:%s (%d:%s)", muc_optstage_name( cli, istage ), filepath, QERRIND,
              QERRNAME );

  muc_ER( OPTIONS, infile_options_at_filepath, muc_config_cli_t * cli, muc_option_stage_t istage, const char *filepath, muc_option_adata_t * paod );
}

/* try config
 *  1. at cfgdir
 *  2. at $HOME
 *  3. at . (cwd)
 * */
static
muc_SR( OPTIONS, infile_options_at_dir_and_file, muc_config_cli_t * cli, muc_option_stage_t istage, const char *cfgdir, const char *filename, int v,
        int optional, muc_option_source_t source, muc_option_adata_t * paod )
{

  FILE *f = NULL;
  int rt1 = 0, rt2 = 0, rt3 = 0;

  MAST_TRACE( options, 20, "@@@@dir_and_file options; stage:%s; cfgdir:%s; filename:%s; source:%s", muc_optstage_name( cli, istage ), cfgdir,
              filename, muc_optsource_name( cli, source ) );
  if ( cfgdir )
    f = muc_infile( cli, 0, cfgdir, filename, &rt1 );
  if ( !f && v > 0 )
  {
    cfgdir = getenv( "HOME" );
    MAST_TRACE( options, 250, "getting variable HOME value for config path (secondary) : %s", cfgdir );
    f = muc_infile( cli, 1, cfgdir, filename, &rt2 );
    if ( f )
      ERRRCLEAR( rt1, OPEN );
  }
  if ( !f && v > 1 )
  {
    f = muc_infile( cli, 0, ".", filename, &rt3 );
    if ( f )
      ERRRCLEAR( rt2, OPEN );
  }

  MAST_TRACE( explain, 0, "to read config file" );
  muc_QTR;
  if ( f )
  {
    muc_QTR;
    muc_CR( infile_options_at_stream, cli, istage, f, source, paod );
    muc_QTR;

    fclose( f );
    MAST_TRACE( explain, 0, "read config file" );
  }
  else if ( optional )
  {
    ERRRCLEAR( rt1, OPEN );
    ERRRCLEAR( rt2, OPEN );
    ERRRCLEAR( rt3, OPEN );
  }
  else
  {
    MAST_TRACE( explain, 0, "read config file" );
    muc_QTR;
    if ( QNOERR )
      QERRIND = rt1 < 0 ? rt1 : ( rt2 < 0 ? rt2 : ( rt3 < 0 ? rt3 : QERRIND ) );
  }
  MAST_TRACE( options, 22, "@@@@dir_and_file options done; stage:%s; cfgdir:%s; filename:%s; source:%s (%d:%s)", muc_optstage_name( cli, istage ),
              cfgdir, filename, muc_optsource_name( cli, source ), QERRIND, QERRNAME );

  muc_ER( OPTIONS, infile_options_at_dir_and_file, muc_config_cli_t * cli, muc_option_stage_t istage, const char *cfgdir, const char *filename, int v,
          int optional, muc_option_source_t source, muc_option_adata_t * paod );
}

/* 20150122
 * get options/settings from passed configuration filename
 * */
static
muc_SR( OPTIONS, infile_options_at_cfgfile, muc_config_cli_t * cli, muc_option_stage_t istage, const char *filename, int optional,
        muc_option_adata_t * paod, muc_option_source_code_t sourcecode )
{

  muc_option_source_t source = MUC_OPTION_SOURCE_LABELLED( CFG, filename );

  MAST_TRACE( optsource, 0, "@  -source:%s", muc_optsourcecode_name( cli, sourcecode ) );
  if ( sourcecode != MUC_OPTION_SOURCE_NONE )
    source.sourcecode = sourcecode;

  MAST_TRACE( options, 20, "@@@@cfgfile options; stage:%s; filename:%s", muc_optstage_name( cli, istage ), filename );

  muc_CR( infile_options_at_dir_and_file, cli, istage, muc_cli_options_config_dir( cli ), filename, 3, optional, source, paod );

  MAST_TRACE( options, 22, "@@@@cfgfile options done; stage:%s; filename:%s (%d:%s)", muc_optstage_name( cli, istage ), filename, QERRIND, QERRNAME );

  muc_ER( OPTIONS, infile_options_at_cfgfile, muc_config_cli_t * cli, muc_option_stage_t istage, const char *filename, int optional,
          muc_option_adata_t * paod, muc_option_source_code_t sourcecode );
}

/*
 * get options/settings from standard configuration file(s)
 * */
static
muc_SR( OPTIONS, incfgf_options, muc_config_cli_t * cli, muc_option_stage_t istage, const char *bfilename, const char *bsuffix, int optional,
        muc_option_adata_t * paod, muc_option_source_code_t sourcecode )
{

  char *filename;

  MAST_TRACE( optsource, 0, "@  *source:%s", muc_optsourcecode_name( cli, sourcecode ) );
  MAST_TRACE( options, 20, "@@@@cfgfile options; stage:%s; bfilename:%s", muc_optstage_name( cli, istage ), bfilename );
  filename = mas_strdup( bfilename );
  if ( bsuffix )
  {
    filename = mas_strcat_x( filename, "_" );
    filename = mas_strcat_x( filename, bsuffix );
  }
  filename = mas_strcat_x( filename, ".conf" );

  muc_CR( infile_options_at_cfgfile, cli, istage, filename, optional, paod, sourcecode );

  mas_free( filename );
  MAST_TRACE( options, 22, "@@@@cfgfile options done; stage:%s; bfilename:%s (%d:%s)", muc_optstage_name( cli, istage ), bfilename, QERRIND,
              QERRNAME );

  muc_ER( OPTIONS, incfgf_options, muc_config_cli_t * cli, muc_option_stage_t istage, const char *bfilename, const char *bsuffix, int optional,
          muc_option_adata_t * paod, muc_option_source_code_t sourcecode );
}

muc_SR( OPTIONS, source_incfg_parse, muc_config_cli_t * cli, muc_option_stage_t istage, muc_int_void_func_t cb_do_interactive
        MAS_UNUSED, muc_cpchar_void_func_t cb_prompt_interactive MAS_UNUSED, muc_option_adata_t * paod,
        muc_option_source_code_t sourcecode MAS_UNUSED )
{

  MAST_TRACE( optsource, 0, "@   source:%s", muc_optsourcecode_name( cli, sourcecode ) );
  MAST_TRACE( options, 20, "@@@@incfg options; stage:%s", muc_optstage_name( cli, istage ) );
  muc_CR( incfgf_options, cli, istage, muc_cli_options_config_file_name( cli ), NULL, !cli->mandatory_config /* optional */ , paod,
          MUC_OPTION_SOURCE_NONE /* use default: MUC_OPTION_SOURCE_CFG */  );
  MAST_TRACE( options, 22, "@@@@incfg options done; stage:%s (%d:%s)", muc_optstage_name( cli, istage ), QERRIND, QERRNAME );

  muc_ER( OPTIONS, source_incfg_parse, muc_config_cli_t * cli, muc_option_stage_t istage, muc_int_void_func_t cb_do_interactive
          MAS_UNUSED, muc_cpchar_void_func_t cb_prompt_interactive MAS_UNUSED, muc_option_adata_t * paod,
          muc_option_source_code_t sourcecode MAS_UNUSED );
}

muc_SR( OPTIONS, source_incfg_last_parse, muc_config_cli_t * cli, muc_option_stage_t istage, muc_int_void_func_t cb_do_interactive MAS_UNUSED,
        muc_cpchar_void_func_t cb_prompt_interactive MAS_UNUSED, muc_option_adata_t * paod, muc_option_source_code_t sourcecode MAS_UNUSED )
{

  MAST_TRACE( optsource, 0, "@   source:%s", muc_optsourcecode_name( cli, sourcecode ) );
  MAST_TRACE( options, 20, "@@@@incfg `last` options; stage:%s", muc_optstage_name( cli, istage ) );
  muc_CR( incfgf_options, cli, istage, muc_cli_options_config_file_name( cli ), "last", 1 /* optional */ , paod, MUC_OPTION_SOURCE_CFGLAST );
  MAST_TRACE( options, 22, "@@@@incfg `last` options done; stage:%s (%d:%s)", muc_optstage_name( cli, istage ), QERRIND, QERRNAME );

  muc_ER( OPTIONS, source_incfg_last_parse, muc_config_cli_t * cli, muc_option_stage_t istage, muc_int_void_func_t cb_do_interactive MAS_UNUSED,
          muc_cpchar_void_func_t cb_prompt_interactive, muc_option_adata_t * paod, muc_option_source_code_t sourcecode );
}

/* per-stage config */
muc_SR( OPTIONS, source_incfg_stg_parse, muc_config_cli_t * cli, muc_option_stage_t istage, muc_int_void_func_t cb_do_interactive MAS_UNUSED,
        muc_cpchar_void_func_t cb_prompt_interactive MAS_UNUSED, muc_option_adata_t * paod, muc_option_source_code_t sourcecode MAS_UNUSED )
{

  MAST_TRACE( optsource, 0, "@@@  source:%s", muc_optsourcecode_name( cli, sourcecode ) );
  MAST_TRACE( options, 20, "@@@@incfg stg options; stage:%s", muc_optstage_name( cli, istage ) );
  {
    const char *sn;

    sn = muc_optstage_name( cli, istage );
    if ( sn )
    {
      muc_CR( incfgf_options, cli, istage, muc_cli_options_config_file_name( cli ), sn, 1 /* optional */ , paod, MUC_OPTION_SOURCE_CFGSTG );
    }
  }
  MAST_TRACE( options, 22, "@@@@incfg stg options done; stage:%s (%d:%s)", muc_optstage_name( cli, istage ), QERRIND, QERRNAME );

  muc_ER( OPTIONS, source_incfg_stg_parse, muc_config_cli_t * cli, muc_option_stage_t istage, muc_int_void_func_t cb_do_interactive MAS_UNUSED,
          muc_cpchar_void_func_t cb_prompt_interactive MAS_UNUSED, muc_option_adata_t * paod, muc_option_source_code_t sourcecode MAS_UNUSED );
}

muc_SR( OPTIONS, source_stdin_parse, muc_config_cli_t * cli, muc_option_stage_t istage, muc_int_void_func_t cb_do_interactive MAS_UNUSED,
        muc_cpchar_void_func_t cb_prompt_interactive MAS_UNUSED, muc_option_adata_t * paod, muc_option_source_code_t sourcecode MAS_UNUSED )
{

  static int done = 0;

  MAST_TRACE( optsource, 0, "@@@  %s source:%s", muc_optstage_name( cli, istage ), muc_optsourcecode_name( cli, sourcecode ) );
/* if ( istage == MUC_OPTION_STAGE_FIRST (* XXX ???? XXX *)  ) */
  {
    if ( !done )
    {
      MAST_TRACE( options, 40, "@@@ isatty: %d:%d:%d", isatty( STDIN_FILENO ), isatty( STDOUT_FILENO ), isatty( STDERR_FILENO ) );
      if ( !isatty( STDIN_FILENO ) /* only when stdin is NOT tty */  )
      {
        MAST_TRACE( optsource, 0, "@@@  !source:%s", muc_optsourcecode_name( cli, sourcecode ) );

        muc_CR( infile_options_at_stream, cli, istage, stdin, MUC_OPTION_SOURCE( STDIN ), paod );
        done = 1;
      }
    }
    else
    {
    /* ERRMAKE( OPEN ); */
    }
  }
  MAST_TRACE( options, 22, "@@@@stdin options done; stage:%s (%d:%s)", muc_optstage_name( cli, istage ), QERRIND, QERRNAME );

  muc_ER( OPTIONS, source_stdin_parse, muc_config_cli_t * cli, muc_option_stage_t istage, muc_int_void_func_t cb_do_interactive,
          muc_cpchar_void_func_t cb_prompt_interactive, muc_option_adata_t * paod, muc_option_source_code_t sourcecode );
}

muc_SR( OPTIONS, source_indirect_parse, muc_config_cli_t * cli, muc_option_stage_t istage, muc_int_void_func_t cb_do_interactive MAS_UNUSED,
        muc_cpchar_void_func_t cb_prompt_interactive MAS_UNUSED, muc_option_adata_t * paod, muc_option_source_code_t sourcecode MAS_UNUSED )
{

  MAST_TRACE( optsource, 0, "@   source:%s", muc_optsourcecode_name( cli, sourcecode ) );
  MAST_TRACE( options, 20, "@@@@indirect options; stage:%s", muc_optstage_name( cli, istage ) );
  MAST_TRACE( options, 250, ">> targc:%d cli.targ_offset:%d", muc_cli_options_get_targc( cli ), muc_cli_options_get_targ_offset( cli ) );
/* for ( int ia = 0; ia < DUF_CONFIGG( cli.targ_offset ); ia++ ) */
  for ( int ia = 0; ia < muc_cli_options_get_targ_offset( cli ); ia++ )
  {
    const char *cf;

  /* cf = DUF_CONFIGG( cli.targ.argv[ia] ); */
    cf = muc_cli_options_get_targi( cli, ia );
    MAST_TRACE( temp, 2, "@%s>> targv[%d]='%s'", muc_optstage_name( cli, istage ), ia, cf );
    if ( cf && *cf == '@' )
    {
      const char *fname = cf + 1;

      muc_CR( infile_options_at_dir_and_file, cli, istage, muc_cli_options_commands_dir( cli ), fname, 0, 0,
              MUC_OPTION_SOURCE_LABELLED( DUFFILE, fname ), paod );

      MAST_TRACE( temp, 2, "%s>> (%d) done targv[%d]='%s'", muc_optstage_name( cli, istage ), QERRIND, ia, cf );
    }
  }
  MAST_TRACE( options, 22, "@@@@indirect options done; stage:%s (%d:%s)", muc_optstage_name( cli, istage ), QERRIND, QERRNAME );

  muc_ER( OPTIONS, source_indirect_parse, muc_config_cli_t * cli, muc_option_stage_t istage, muc_int_void_func_t cb_do_interactive MAS_UNUSED,
          muc_cpchar_void_func_t cb_prompt_interactive MAS_UNUSED, muc_option_adata_t * paod, muc_option_source_code_t sourcecode );
}
