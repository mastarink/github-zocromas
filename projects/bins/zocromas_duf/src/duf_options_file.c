/* #undef MAS_TRACING */
#define MAST_TRACE_CONFIG duf_get_cli_options_trace_config(cli)
#include <assert.h>                                                  /* assert */
#include <string.h>
#include <errno.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ♣ */

#include <mastar/tools/mas_utils_path.h>                             /* mas_normalize_path; mas_pathdepth; mas_realpath etc. ♣ */

#include "duf_tracen_defs.h"                                         /* DUF_TRACE ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

/* #include "duf_start_end.h"                                           (* DUF_STARTR ; DUF_ENDR ♠ *) */
/* #include "duf_dodefs.h"                                              (* DOR ♠ *) */

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ♠ */

#include "duf_option_config.h"                                       /* duf_get_cli_options_trace_config ♠ */
#include "duf_options_enum.h"                                        /* duf_option_code_t ♠ */
#include "duf_option_stage.h"                                        /* duf_optstage_name ♠ */
#include "duf_option_source.h"                                       /* duf_optsource_name ♠ */

#include "duf_option_clarify_batch.h"

/* ###################################################################### */
#include "duf_options_file.h"
/* ###################################################################### */

/* TODO: remove static; possibly to «duf_config_cli_t * cli»  */
static char *config_file_path = NULL;

static void destructor_options_file( void ) __attribute__ ( ( destructor( 101 ) ) );
static void
destructor_options_file( void )
{
  mas_free( config_file_path );
}

const char *
duf_options_infilepath( duf_config_cli_t * cli MAS_UNUSED )
{
#if 0
  return DUF_CONFIGG( config_file_path );
#else
  return config_file_path;                                           /* => static duf_config_cli_t *config_cli = NULL; @ duf_option_config.c TODO */
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
duf_infilepath( duf_config_cli_t * cli MAS_UNUSED, const char *filepath, int *pry )
{
  FILE *f = NULL;

  f = fopen( filepath, "r" );
#if 0
  mas_free( DUF_CONFIGG( config_file_path ) );
  DUF_CONFIGWS( config_file_path, mas_strdup( filepath ) );
  DUF_TRACE( options, 230, "opened conf file %s %s", DUF_CONFIGG( config_file_path ), f ? "Ok" : "FAIL" );
#else
  mas_free( config_file_path );
  config_file_path = mas_strdup( filepath );
  DUF_TRACE( options, 250, "opened conf file %s %s", config_file_path, f ? "Ok" : "FAIL" );
#endif
  if ( pry )
    *pry = f ? 0 : -errno;
  return f;
}

static FILE *
duf_infile( duf_config_cli_t * cli MAS_UNUSED, int dot, const char *at, const char *filename, int *pr )
{
  int rpr = 0;
  int ry = 0;
  FILE *f = NULL;
  char *cfgpath = NULL;

/* assert( duf_config ); */
  cfgpath = mas_normalize_path( at );

  assert( cfgpath );
  if ( dot )
    cfgpath = mas_strcat_x( cfgpath, "." );
  assert( cfgpath );
  cfgpath = mas_strcat_x( cfgpath, filename );
  assert( cfgpath );
  f = duf_infilepath( cli, cfgpath, &ry );
  if ( ry < 0 )
    DUF_MAKE_ERRORM( rpr, DUF_ERROR_OPEN, "file %s", cfgpath );
  if ( pr )
    *pr = rpr;
  mas_free( cfgpath );
  return f;
}

static
SR( OPTIONS, infile_options_at_stream, duf_config_cli_t * cli, duf_option_stage_t istage, FILE * f, duf_option_source_t source,
    duf_option_adata_t * paod )
{
/* DUF_STARTR( r ); */

  DUF_TRACE( options, 20, "@@@@stream options; stage:%s; source:%s", duf_optstage_name( cli, istage ), duf_optsource_name( cli, source ) );
  while ( QNOERR && f && !feof( f ) )
  {
    char buffer[1024];
    char *s;

    s = fgets( buffer, sizeof( buffer ), f );
    if ( s )
    {
      s = mas_chomp( s );
      DUF_TRACE( options, 250, "@@@@read cmd '%s'", s );
      if ( s && ( ( *s == '#' ) || !*s ) )
        continue;
      DUF_TRACE( explain, 0, "read config line %s", s );
      {
        {
          if ( !DUF_IS_SOURCE( source, CFG ) && duf_optsource_labelled( cli, source ) )
          {
            DUF_TRACE( optsource, 0, "@@@@@@@@@%s:%s(%s)", duf_optstage_name( cli, istage ), duf_optsource_name( cli, source ),
                       duf_optsource_label( cli, source ) );
          }
          CR( boption_xclarify_at_stdx, cli, 0 /* value_separator */ , istage, DUF_OPTION_SOURCE_DEFAULT( source, STREAM ), s, 0, paod );
        }
        TR( QERRIND );
      }
    }
  }
  TR( QERRIND );
  DUF_TRACE( options, 22, "@@@@stream options done; stage:%s; source:%s (%d:%s)", duf_optstage_name( cli, istage ), duf_optsource_name( cli, source ),
             QERRIND, QERRNAME );
/* DUF_ENDR( r ); */
  ER( OPTIONS, infile_options_at_stream, duf_config_cli_t * cli, duf_option_stage_t istage, FILE * f, duf_option_source_t source,
      duf_option_adata_t * paod );
}

SR( OPTIONS, infile_options_at_filepath, duf_config_cli_t * cli, duf_option_stage_t istage, const char *filepath, duf_option_adata_t * paod )
{
/* DUF_STARTR( r ); */
  int ry = 0;
  FILE *f = NULL;

  DUF_TRACE( options, 20, "@@@@filepath options; stage:%s; filepath:%s", duf_optstage_name( cli, istage ), filepath );
  f = duf_infilepath( cli, filepath, &ry );

  DUF_TRACE( options, 250, "to read config file %s", filepath );
  if ( f )
  {
    {
      CR( infile_options_at_stream, cli, istage, f, DUF_OPTION_SOURCE_LABELLED( DUFFILE, filepath ), paod );
    }
    fclose( f );
    DUF_TRACE( options, 250, "read config file %s", filepath );
  }
  else
  {
    DUF_TRACE( options, 50, "@@fail to read config file %s", filepath );
  /* DUF_MAKE_ERROR( r, DUF_ERROR_OPEN ); */
    DUF_MAKE_ERRORM( QERRIND, DUF_ERROR_OPEN, "file '%s'", filepath );
  /* assert(0); */
  }
  DUF_TRACE( options, 22, "@@@@filepath options done; stage:%s; filepath:%s (%d:%s)", duf_optstage_name( cli, istage ), filepath, QERRIND, QERRNAME );
/* DUF_ENDR( r ); */
  ER( OPTIONS, infile_options_at_filepath, duf_config_cli_t * cli, duf_option_stage_t istage, const char *filepath, duf_option_adata_t * paod );
}

/* try config
 *  1. at cfgdir
 *  2. at $HOME
 *  3. at . (cwd)
 * */
static
SR( OPTIONS, infile_options_at_dir_and_file, duf_config_cli_t * cli, duf_option_stage_t istage, const char *cfgdir, const char *filename, int v,
    int optional, duf_option_source_t source, duf_option_adata_t * paod )
{
/* DUF_STARTR( r ); */
  FILE *f = NULL;
  int rt1 = 0, rt2 = 0, rt3 = 0;

  DUF_TRACE( options, 20, "@@@@dir_and_file options; stage:%s; cfgdir:%s; filename:%s; source:%s", duf_optstage_name( cli, istage ), cfgdir, filename,
             duf_optsource_name( cli, source ) );
  if ( cfgdir )
    f = duf_infile( cli, 0, cfgdir, filename, &rt1 );
  if ( !f && v > 0 )
  {
    cfgdir = getenv( "HOME" );
    DUF_TRACE( options, 250, "getting variable HOME value for config path (secondary) : %s", cfgdir );
    f = duf_infile( cli, 1, cfgdir, filename, &rt2 );
    if ( f )
      DUF_CLEAR_ERROR( rt1, DUF_ERROR_OPEN );
  }
  if ( !f && v > 1 )
  {
    f = duf_infile( cli, 0, ".", filename, &rt3 );
    if ( f )
      DUF_CLEAR_ERROR( rt2, DUF_ERROR_OPEN );
  }

  DUF_TRACE( explain, 0, "to read config file" );
  TR( QERRIND );
  if ( f )
  {
    TR( QERRIND );
    CR( infile_options_at_stream, cli, istage, f, source, paod );
    TR( QERRIND );

    fclose( f );
    DUF_TRACE( explain, 0, "read config file" );
  }
  else if ( optional )
  {
    DUF_CLEAR_ERROR( rt1, DUF_ERROR_OPEN );
    DUF_CLEAR_ERROR( rt2, DUF_ERROR_OPEN );
    DUF_CLEAR_ERROR( rt3, DUF_ERROR_OPEN );
  }
  else
  {
    DUF_TRACE( explain, 0, "read config file" );
    TR( QERRIND );
    if ( QNOERR )
      QERRIND = rt1 < 0 ? rt1 : ( rt2 < 0 ? rt2 : ( rt3 < 0 ? rt3 : QERRIND ) );
  }
  DUF_TRACE( options, 22, "@@@@dir_and_file options done; stage:%s; cfgdir:%s; filename:%s; source:%s (%d:%s)", duf_optstage_name( cli, istage ),
             cfgdir, filename, duf_optsource_name( cli, source ), QERRIND, QERRNAME );
/* DUF_ENDR( r ); */
  ER( OPTIONS, infile_options_at_dir_and_file, duf_config_cli_t * cli, duf_option_stage_t istage, const char *cfgdir, const char *filename, int v,
      int optional, duf_option_source_t source, duf_option_adata_t * paod );
}

/* 20150122
 * get options/settings from passed configuration filename
 * ***********************************************
 * 1. set configuration directory from OS envirionment
 * 2. call duf_infile_options_at_dir_and_file
 * */
static
SR( OPTIONS, infile_options_at_cfgfile, duf_config_cli_t * cli, duf_option_stage_t istage, const char *filename, int optional,
    duf_option_adata_t * paod, duf_option_source_code_t sourcecode )
{
/* DUF_STARTR( r ); */

  duf_option_source_t source = DUF_OPTION_SOURCE_LABELLED( CFG, filename );

  DUF_TRACE( optsource, 0, "@  -source:%s", duf_optsourcecode_name( cli, sourcecode ) );
  if ( sourcecode != DUF_OPTION_SOURCE_NONE )
    source.sourcecode = sourcecode;
/* 
 * duf_infile_options_at_dir_and_file return codeval>0 for "help" option
 *   =0 for other option
 *   errorcode<0 for error
 * */
  DUF_TRACE( options, 20, "@@@@cfgfile options; stage:%s; filename:%s", duf_optstage_name( cli, istage ), filename );
  {
#if 0
    CR( infile_options_at_dir_and_file( istage, DUF_CONFIGGS( conf.config_dir ), filename, 3, optional, source, paod );
#else
    CR( infile_options_at_dir_and_file, cli, istage, duf_cli_options_config_dir( cli ), filename, 3, optional, source, paod );
#endif
  }
  DUF_TRACE( options, 22, "@@@@cfgfile options done; stage:%s; filename:%s (%d:%s)", duf_optstage_name( cli, istage ), filename, QERRIND, QERRNAME );

/* DUF_ENDR( r ); */
  ER( OPTIONS, infile_options_at_cfgfile, duf_config_cli_t * cli, duf_option_stage_t istage, const char *filename, int optional,
      duf_option_adata_t * paod, duf_option_source_code_t sourcecode );
}

/*
 * get options/settings from standard configuration file(s)
 *    - global variable duf_config must be created/inited
 * ***********************************************
 * 1. set configuration file name (constant)
 * 2. call duf_infile_options_at_cfgfile
 * */
static
SR( OPTIONS, incfgf_options, duf_config_cli_t * cli, duf_option_stage_t istage, const char *bfilename, const char *bsuffix, int optional,
    duf_option_adata_t * paod, duf_option_source_code_t sourcecode )
{
/* DUF_STARTR( r ); */
  char *filename;

  DUF_TRACE( optsource, 0, "@  *source:%s", duf_optsourcecode_name( cli, sourcecode ) );
  DUF_TRACE( options, 20, "@@@@cfgfile options; stage:%s; bfilename:%s", duf_optstage_name( cli, istage ), bfilename );
  filename = mas_strdup( bfilename );
  if ( bsuffix )
  {
    filename = mas_strcat_x( filename, "_" );
    filename = mas_strcat_x( filename, bsuffix );
  }
  filename = mas_strcat_x( filename, ".conf" );

  CR( infile_options_at_cfgfile, cli, istage, filename, optional, paod, sourcecode );

  mas_free( filename );
  DUF_TRACE( options, 22, "@@@@cfgfile options done; stage:%s; bfilename:%s (%d:%s)", duf_optstage_name( cli, istage ), bfilename, QERRIND,
             QERRNAME );
/* DUF_ENDR( r ); */
  ER( OPTIONS, incfgf_options, duf_config_cli_t * cli, duf_option_stage_t istage, const char *bfilename, const char *bsuffix, int optional,
      duf_option_adata_t * paod, duf_option_source_code_t sourcecode );
}

SR( OPTIONS, source_incfg_parse, duf_config_cli_t * cli, duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive
    MAS_UNUSED, duf_cpchar_void_func_t cb_prompt_interactive MAS_UNUSED, duf_option_adata_t * paod, duf_option_source_code_t sourcecode MAS_UNUSED )
{
/* DUF_STARTR( r ); */

  DUF_TRACE( optsource, 0, "@   source:%s", duf_optsourcecode_name( cli, sourcecode ) );
  DUF_TRACE( options, 20, "@@@@incfg options; stage:%s", duf_optstage_name( cli, istage ) );
  CR( incfgf_options, cli, istage, duf_cli_options_config_file_name( cli ), NULL, 0, paod,
      DUF_OPTION_SOURCE_NONE /* use default: DUF_OPTION_SOURCE_CFG */  );
  DUF_TRACE( options, 22, "@@@@incfg options done; stage:%s (%d:%s)", duf_optstage_name( cli, istage ), QERRIND, QERRNAME );

/* DUF_ENDR( r ); */
  ER( OPTIONS, source_incfg_parse, duf_config_cli_t * cli, duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive
      MAS_UNUSED, duf_cpchar_void_func_t cb_prompt_interactive MAS_UNUSED, duf_option_adata_t * paod,
      duf_option_source_code_t sourcecode MAS_UNUSED );
}

SR( OPTIONS, source_incfg_last_parse, duf_config_cli_t * cli, duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive MAS_UNUSED,
    duf_cpchar_void_func_t cb_prompt_interactive MAS_UNUSED, duf_option_adata_t * paod, duf_option_source_code_t sourcecode MAS_UNUSED )
{
/* DUF_STARTR( r ); */

  DUF_TRACE( optsource, 0, "@   source:%s", duf_optsourcecode_name( cli, sourcecode ) );
  DUF_TRACE( options, 20, "@@@@incfg `last` options; stage:%s", duf_optstage_name( cli, istage ) );
  CR( incfgf_options, cli, istage, duf_cli_options_config_file_name( cli ), "last", 1, paod, DUF_OPTION_SOURCE_CFGLAST );
  DUF_TRACE( options, 22, "@@@@incfg `last` options done; stage:%s (%d:%s)", duf_optstage_name( cli, istage ), QERRIND, QERRNAME );

/* DUF_ENDR( r ); */

  ER( OPTIONS, source_incfg_last_parse, duf_config_cli_t * cli, duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive MAS_UNUSED,
      duf_cpchar_void_func_t cb_prompt_interactive, duf_option_adata_t * paod, duf_option_source_code_t sourcecode );
}

#if 0
int
duf_incfg_src_options( duf_option_stage_t istage, duf_option_source_t src )
{
  DUF_STARTR( r );
  char *bfilename;

  static const char *tail[] = {
    [DUF_OPTION_SOURCE_NONE] = "none",
    [DUF_OPTION_SOURCE_ENV] = "env",
    [DUF_OPTION_SOURCE_STDIN] = "stdin",
    [DUF_OPTION_SOURCE_STREAM] = "stream",
    [DUF_OPTION_SOURCE_DUFFILE] = "file",
    [DUF_OPTION_SOURCE_CFG] = "cfg",
    [DUF_OPTION_SOURCE_CLI] = "cli",
    [DUF_OPTION_SOURCE_INTERACTIVE] = "interactive",
  };
  if ( istage > DUF_OPTION_SOURCE_NONE && istage < DUF_OPTION_SOURCE_MAX )
  {
    bfilename = mas_strdup( DUF_CONFIG_FILE_NAME );
    bfilename = mas_strcat_x( bfilename, "_" );
    bfilename = mas_strcat_x( bfilename, tail[src] );
    DOR( r, duf_incfgf_options( istage, bfilename, 1 /* optional */  ) );
    mas_free( bfilename );
  }

  DUF_ENDR( r );
}
#endif

/* per-stage config */
SR( OPTIONS, source_incfg_stg_parse, duf_config_cli_t * cli, duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive MAS_UNUSED,
    duf_cpchar_void_func_t cb_prompt_interactive MAS_UNUSED, duf_option_adata_t * paod, duf_option_source_code_t sourcecode MAS_UNUSED )
{
/* DUF_STARTR( r ); */

  DUF_TRACE( optsource, 0, "@@@  source:%s", duf_optsourcecode_name( cli, sourcecode ) );
  DUF_TRACE( options, 20, "@@@@incfg stg options; stage:%s", duf_optstage_name( cli, istage ) );
  {
    const char *sn;

    sn = duf_optstage_name( cli, istage );
    if ( sn )
    {
      CR( incfgf_options, cli, istage, duf_cli_options_config_file_name( cli ), sn, 1 /* optional */ ,
          paod, DUF_OPTION_SOURCE_CFGSTG );
    }
  }
  DUF_TRACE( options, 22, "@@@@incfg stg options done; stage:%s (%d:%s)", duf_optstage_name( cli, istage ), QERRIND, QERRNAME );

/* DUF_ENDR( r ); */
  ER( OPTIONS, source_incfg_stg_parse, duf_config_cli_t * cli, duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive MAS_UNUSED,
      duf_cpchar_void_func_t cb_prompt_interactive MAS_UNUSED, duf_option_adata_t * paod, duf_option_source_code_t sourcecode MAS_UNUSED );
}

/* duf_source_stdin_parse - can be executed only once (direct stdin reading!)  */
SR( OPTIONS,
    source_stdin_parse, duf_config_cli_t * cli, duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive
    MAS_UNUSED, duf_cpchar_void_func_t cb_prompt_interactive MAS_UNUSED, duf_option_adata_t * paod, duf_option_source_code_t sourcecode MAS_UNUSED )
{
/* DUF_STARTR( r ); */
  static int done = 0;

  DUF_TRACE( optsource, 0, "@@@  %s source:%s", duf_optstage_name( cli, istage ), duf_optsourcecode_name( cli, sourcecode ) );
/* if ( istage == DUF_OPTION_STAGE_FIRST (* XXX ???? XXX *)  ) */
  {
    if ( !done )
    {
      DUF_TRACE( options, 40, "@@@ isatty: %d:%d:%d", isatty( STDIN_FILENO ), isatty( STDOUT_FILENO ), isatty( STDERR_FILENO ) );
      if ( !isatty( STDIN_FILENO ) /* only when stdin is NOT tty */  )
      {
        DUF_TRACE( optsource, 0, "@@@  !source:%s", duf_optsourcecode_name( cli, sourcecode ) );

        CR( infile_options_at_stream, cli, istage, stdin, DUF_OPTION_SOURCE( STDIN ), paod );
        done = 1;
      }
    }
    else
    {
    /* DUF_MAKE_ERROR( r, DUF_ERROR_OPEN ); */
    /* assert(0); */
    }
  }
  DUF_TRACE( options, 22, "@@@@stdin options done; stage:%s (%d:%s)", duf_optstage_name( cli, istage ), QERRIND, QERRNAME );
/* DUF_ENDR( r ); */
  ER( OPTIONS,
      source_stdin_parse, duf_config_cli_t * cli, duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive
      MAS_UNUSED, duf_cpchar_void_func_t cb_prompt_interactive MAS_UNUSED, duf_option_adata_t * paod,
      duf_option_source_code_t sourcecode MAS_UNUSED );
}

SR( OPTIONS, source_indirect_parse, duf_config_cli_t * cli, duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive MAS_UNUSED,
    duf_cpchar_void_func_t cb_prompt_interactive MAS_UNUSED, duf_option_adata_t * paod, duf_option_source_code_t sourcecode MAS_UNUSED )
{
/* DUF_STARTR( r ); */

  DUF_TRACE( optsource, 0, "@   source:%s", duf_optsourcecode_name( cli, sourcecode ) );
  DUF_TRACE( options, 20, "@@@@indirect options; stage:%s", duf_optstage_name( cli, istage ) );
  DUF_TRACE( options, 250, ">> targc:%d cli.targ_offset:%d", /* DUF_CONFIGG( cli.targ.argc ) */ duf_cli_options_get_targc( cli ),
             duf_cli_options_get_targ_offset( cli ) );
/* for ( int ia = 0; ia < DUF_CONFIGG( cli.targ_offset ); ia++ ) */
  for ( int ia = 0; ia < duf_cli_options_get_targ_offset( cli ); ia++ )
  {
    const char *cf;

  /* cf = DUF_CONFIGG( cli.targ.argv[ia] ); */
    cf = duf_cli_options_get_targi( cli, ia );
    DUF_TRACE( temp, 2, "@%s>> targv[%d]='%s'", duf_optstage_name( cli, istage ), ia, cf );
    if ( cf && *cf == '@' )
    {
      const char *fname = cf + 1;

      {                                                              /* TODO conf.cmds_dir => cli.cmds_dir ?? TODO */
#if 0
        CR( infile_options_at_dir_and_file, istage, DUF_CONFIGGS( conf.cmds_dir ), fname, 0, 0, DUF_OPTION_SOURCE_LABELLED( DUFFILE, fname ), paod );
#else
        CR( infile_options_at_dir_and_file, cli, istage, duf_cli_options_commands_dir( cli ), fname, 0, 0,
            DUF_OPTION_SOURCE_LABELLED( DUFFILE, fname ), paod );
#endif
      }

      DUF_TRACE( temp, 2, "%s>> (%d) done targv[%d]='%s'", duf_optstage_name( cli, istage ), QERRIND, ia, cf );
    }
  }
  DUF_TRACE( options, 22, "@@@@indirect options done; stage:%s (%d:%s)", duf_optstage_name( cli, istage ), QERRIND, QERRNAME );

/* DUF_ENDR( r ); */
  ER( OPTIONS, source_indirect_parse, duf_config_cli_t * cli, duf_option_stage_t istage, duf_int_void_func_t cb_do_interactive MAS_UNUSED,
      duf_cpchar_void_func_t cb_prompt_interactive MAS_UNUSED, duf_option_adata_t * paod, duf_option_source_code_t sourcecode );
}
