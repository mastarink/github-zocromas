/* #undef MAS_TRACING */
#define MAST_TRACE_CONFIG duf_get_cli_options_trace_config(cli)
#include "duf_errorn_defs_preset.h" 

#include <assert.h>                                                  /* assert */
#include <stdio.h>                                                   /* FILE */
#include <string.h>
#include <sys/stat.h>                                                /* struct stat */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_types.h>                           /* mas_limits_t; mas_limitsll_t */
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */
#include <mastar/tools/mas_convert.h>                                /* mas_strtol_suff; mas_strtoll_suff; etc. ♣ */
#include <mastar/tools/mas_expandable.h>
#include <mastar/tools/mas_time.h>                                   /* mas_(|double_|xlocal|xgm|xvstrf|xvstrftime_|(|t)strflocal|strfgm)time ; strtime2long; etc. ♣ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ♣ */
#include <mastar/tools/mas_utils_path.h>                             /* mas_normalize_path; mas_pathdepth; mas_realpath etc. ♣ */
# include <mastar/error/mas_error_defs_ctrl.h>
# include <mastar/error/mas_error_defs.h>                            /* MASE_TEST_R; MASE_TEST_R_LOWERE; ... */
# include <mastar/error/mas_error_defs_make.h>                       /* MASE_MAKE_ERROR; MASE_MAKE_ERRORFL; MASE_MAKE_ERRORM  ... */


#include "duf_tracen_defs.h"                                         /* MAST_TRACE ♠ */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ *) */

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ♠ */

#include "duf_option_stage.h"                                        /* duf_optstage_name ♠ */
#include "duf_option_source.h"                                       /* duf_optsource_name ♠ */
#include "duf_option_config.h"                                       /* duf_get_cli_options_trace_config ♠ */

#include "duf_optmod_tmpdb.h"

/* ###################################################################### */
#include "duf_option_typed_defs.h"
#include "duf_option_typed_gen.h"
/* ###################################################################### */

static 
#if 0
  FILE *
duf_open_file_special( duf_config_cli_t * cli, const char *pname, char **popenedname, int overwrite, int *pr )
{
  int rpr = 0;
  FILE *newfile = NULL;
  int overw = 0;
  const char *mode = "w";
  struct stat st;
  int skipchar = 0;
  int ry;

  MAST_TRACE( io, 0, "to open %s", pname );
  if ( ( skipchar = ( *pname == '@' ) ) || overwrite )
    overw = 1;
  else if ( ( skipchar = ( *pname == '+' ) ) )
    mode = "a";
  if ( skipchar )
    pname++;
  *popenedname = mas_strdup( pname );
  ry = stat( pname, &st );
  if ( ry >= 0 && ( ( !S_ISCHR( st.st_mode ) || !( st.st_mode & S_IWUSR ) ) && ( !overw && *mode != 'a' ) ) )
  {
    DUF_MAKE_ERRORM( rpr, DUF_ERROR_FILE_EXISTS, "can't open file %s for writing file exists %llu / %llu chr:%d", pname,
                     ( unsigned long long ) st.st_dev, ( unsigned long long ) st.st_rdev, S_ISCHR( st.st_mode ) );
  }
  else
  {
    newfile = fopen( pname, mode );
    if ( !newfile )
    {
      DUF_MAKE_ERRORM( rpr, DUF_ERROR_OPEN, "can't open file %s", pname );
    }
  }
  if ( pr )
    *pr = rpr;
  return newfile;
}
#else
SRP( OPTIONS, FILE *, newfile = NULL, open_file_special, duf_config_cli_t * cli, const char *pname, char **popenedname, int overwrite )
{
  /* int rpr = 0; */
  /* FILE *newfile = NULL; */
  int overw = 0;
  const char *mode = "w";
  struct stat st;
  int skipchar = 0;
  int ry;

  MAST_TRACE( io, 0, "to open %s", pname );
  if ( ( skipchar = ( *pname == '@' ) ) || overwrite )
    overw = 1;
  else if ( ( skipchar = ( *pname == '+' ) ) )
    mode = "a";
  if ( skipchar )
    pname++;
  *popenedname = mas_strdup( pname );
  ry = stat( pname, &st );
  if ( ry >= 0 && ( ( !S_ISCHR( st.st_mode ) || !( st.st_mode & S_IWUSR ) ) && ( !overw && *mode != 'a' ) ) )
  {
    ERRMAKE_M(  FILE_EXISTS, "can't open file %s for writing file exists %llu / %llu chr:%d", pname,
                     ( unsigned long long ) st.st_dev, ( unsigned long long ) st.st_rdev, S_ISCHR( st.st_mode ) );
  }
  else
  {
    newfile = fopen( pname, mode );
    if ( !newfile )
    {
      ERRMAKE_M( OPEN, "can't open file %s", pname );
    }
  }
  ERP( OPTIONS, FILE *, newfile, open_file_special, duf_config_cli_t * cli, const char *pname, char **popenedname, int overwrite );
}
#endif
/*  20150924.120214 for DUF_OUTPUTFILE DUF_OUTPUTFILE_A */
static
SR( OPTIONS, set_file_special, duf_config_cli_t * cli, const char *pname, int overwrite, char **pfilename, FILE ** pfile, FILE * defout,
    int handleid )
{

  FILE *newout;

  newout = defout;
  MAST_TRACE( io, 0, "to set file special %s", pname );
  if ( pfile )
  {
    *pfile = NULL;

    MAST_TRACE( io, 0, "handleid %d", handleid );
    if ( handleid )
    {
      switch ( handleid )
      {
      case 1:
        newout = stdout;
        break;
      case 2:
        newout = stderr;
        break;
      }
    }
    MAST_TRACE( io, 0, "pname %s", pname );
    if ( pfilename )
    {
      if ( *pfilename )
        mas_free( *pfilename );
      *pfilename = NULL;
    }
    if ( pname && *pname )
      newout = duf_open_file_special( cli, pname, pfilename, overwrite, QPERRIND );
    if ( !newout )
      newout = defout;
    if ( newout )
    {
      if ( *pfile && *pfile != stderr && *pfile != stdout )
        fclose( *pfile );
      *pfile = newout;
    }
  }

  ER( OPTIONS, set_file_special, duf_config_cli_t * cli, const char *pname, int overwrite, char **pfilename, FILE ** pfile, FILE * defout,
      int handleid );
}

static void *
duf_get_offset( void *ptr, unsigned long off )
{
  assert( ptr );
  return ptr ? ( void * ) ( ( ( char * ) ptr ) + off ) : NULL;
}

static void *
duf_xoption_clarify_typed_byteptr( duf_config_cli_t * cli, const duf_longval_extended_t * extended )
{
  void *byte_ptr = NULL;

  MAST_TRACE( options, 60, "to switch by extended->relto=%d", extended->relto );
/* TODO relto=..._get_config_puz_offset etc. */
  switch ( extended->relto )
  {
  case DUF_OFFSET_none:
    assert( extended->m_offset == 0 );
    break;
  case DUF_OFFSET_varptr:
    byte_ptr = duf_get_offset( extended->reltoptr, extended->m_offset );
    break;
  case DUF_OFFSET_funcptr:
    {
      void *aptr;

      aptr = ( ( duf_pvoid_void_func_t ) extended->reltoptr ) (  );
      byte_ptr = duf_get_offset( aptr, extended->m_offset );
    }
    break;
  }
  return byte_ptr;
}

SR( OPTIONS,
    xoption_clarify_typed_gen, duf_config_cli_t * cli, const duf_longval_extended_t * extended, const char *optargg, unsigned noo,
    duf_option_stage_t istage MAS_UNUSED, duf_option_source_t source MAS_UNUSED )
{

/*
 * if arg is help option
 * return class id for options to display the help
 * */
  if ( QNOERR )
  {
    unsigned doplus = 0;
    void *byteptr = NULL;

    byteptr = duf_xoption_clarify_typed_byteptr( cli, extended );
    MAST_TRACE( options, 60, "to check stage; istage:%s", duf_optstage_name( cli, istage ) );
  /* if ( DUF_OPTION_CHECK_STAGE( istage, extended, xtable ) ) *//* moved upper */
    {
      unsigned nof;
      char *doptargg;

      nof = noo;
      MAST_TRACE( options, 60, "stage OK; switching by extended->vtype=%d", extended->vtype );

      doptargg = mas_strdup( optargg );
      switch ( extended->vtype )
      {
      case DUF_OPTION_VTYPE_NONE:
        MAST_TRACE( options, 70, "vtype NONE" );
#if 0
        if ( QNOERR )
          ERRMAKE( OPTION_NOT_PARSED );                              // Why was ???
      /* DUF_TEST_R( r ); */
#endif
        break;
      case DUF_OPTION_VTYPE_NOOP:
        MAST_TRACE( options, 70, "vtype NOOP" );
        break;
      case DUF_OPTION_VTYPE_UPLUS:                                  /* stage (?) not SETUP *//* --trace-...; --debug; --verbose */
        MAST_TRACE( options, 70, "vtype UPLUS" );
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        if ( QNOERR )
          doplus = 1;
      /* assert( 0 ); */
      /* assert( ( ( char * ) ( DUF_config->opt.ptracecfg->class_levels ) ) + ( extended->m_offset ) == ( ( char * ) byteptr ) ); */
      case DUF_OPTION_VTYPE_NUM:                                    /* stage SETUP *//* --max-...; --min-...; --output-level; --use-format; etc. (misc) */
        MAST_TRACE( options, 70, "vtype NUM --%s='%s'", extended->o.name, optargg ? optargg : "" );
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        DUF_NUMOPT( noo, unsigned, doplus, mas_strtol_suff );

        MAST_TRACE( options, 70, "(%d) vtype NUM --%s='%s'", QERRIND, extended->o.name, optargg ? optargg : "" );
        break;
      case DUF_OPTION_VTYPE_NUML:                                   /* stage SETUP *//* not used ?! */
        MAST_TRACE( options, 70, "vtype NL" );
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        DUF_NUMOPT( noo, unsigned long, 0, mas_strtol_suff );

        break;
      case DUF_OPTION_VTYPE_NUMLL:                                  /* stage SETUP *//* not used ?! */
        MAST_TRACE( options, 70, "vtype NLL" );
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        DUF_NUMOPT( noo, unsigned long long, 0, mas_strtoll_suff );

        break;
      case DUF_OPTION_VTYPE_MIN:                                    /* stage SETUP */
        MAST_TRACE( options, 70, "vtype MIN" );
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        DUF_MOPT( noo, mas_limits_t, min, mas_strtol_suff );
        break;
      case DUF_OPTION_VTYPE_MAX:                                    /* stage SETUP */
        MAST_TRACE( options, 70, "vtype MAX" );
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        DUF_MOPT( noo, mas_limits_t, max, mas_strtol_suff );
        break;
      case DUF_OPTION_VTYPE_MINMAX:                                 /* stage SETUP */
        MAST_TRACE( options, 70, "vtype MINMAX" );
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        DUF_MINMAXOPT( noo, mas_limits_t, mas_strtol_suff );
        break;

      case DUF_OPTION_VTYPE_MINLL:                                  /* stage SETUP */
        MAST_TRACE( options, 70, "vtype MINLL" );
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        DUF_MOPT( noo, mas_limitsll_t, min, mas_strtol_suff );
        break;
      case DUF_OPTION_VTYPE_MAXLL:                                  /* stage SETUP */
        MAST_TRACE( options, 70, "vtype MAXLL" );
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        DUF_MOPT( noo, mas_limitsll_t, max, mas_strtoll_suff );
        break;
      case DUF_OPTION_VTYPE_MINMAXLL:                               /* stage SETUP */
        MAST_TRACE( options, 70, "vtype MINMAXLL" );
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        DUF_MINMAXOPT( noo, mas_limitsll_t, mas_strtoll_suff );
        break;

#define DUF_OPTION_VTYPE_QBFLAG(_typ) \
	{ \
	  if (!byteptr) \
	    ERRMAKE( OPTION ); \
          if ( extended->unset ) \
            nof = !nof; \
          assert( byteptr ); \
          if ( QNOERR ) \
          { \
            _typ *pix; \
	    _typ nz=~0; \
            pix = ( _typ * ) byteptr; \
            if ( nof ) \
              ( *pix ) &= (_typ)((~(((_typ) 1)<<(extended->flag_bitnum-1))) & nz); \
            else \
              ( *pix ) |= (_typ)( ( ((_typ) 1)<<(extended->flag_bitnum-1)) & nz ); \
            MAST_TRACE( options, +140, "@@@@@@[%d] %p %p :%s: %llx %llx (%llx)", nof, byteptr, pix, extended->o.name, (unsigned long long)*pix, \
                       (unsigned long long)(1<<(extended->flag_bitnum-1)), (unsigned long long)( *pix | (1<<(extended->flag_bitnum-1)) ) ); \
            TER; \
          } \
        }
#define DUF_OPTION_VTYPE_XQBFLAG \
	if (extended->vsize==sizeof(unsigned char)) \
	{  DUF_OPTION_VTYPE_QBFLAG(unsigned char); } \
	else if (extended->vsize==sizeof(unsigned short)) \
	{  DUF_OPTION_VTYPE_QBFLAG(unsigned char); } \
	else if (extended->vsize==sizeof(unsigned )) \
	{  DUF_OPTION_VTYPE_QBFLAG(unsigned ); } \
	else if (extended->vsize==sizeof(unsigned long)) \
	{  DUF_OPTION_VTYPE_QBFLAG(unsigned long); } \
	else if (extended->vsize==sizeof(unsigned long long)) \
	{  DUF_OPTION_VTYPE_QBFLAG(unsigned long long); }

      case DUF_OPTION_VTYPE_NOBFLAG:                                /* stage SETUP */
        MAST_TRACE( options, 70, "vtype %s", QSTR( DUF_OPTION_VTYPE_NOBFLAG ) + 17 );
        nof = !nof;
      case DUF_OPTION_VTYPE_BFLAG:                                  /* stage SETUP *//*  unsigned set of flags */
        MAST_TRACE( options, 70, "vtype %s %lx", QSTR( DUF_OPTION_VTYPE_BFLAG ) + 17, ( 1L << ( extended->flag_bitnum - 1 ) ) );
        DUF_OPTION_VTYPE_QBFLAG( unsigned );

      /* assert( ( extended->flag_bitnum && !extended->afl_obsolete.bit )                                                            */
      /*         || ( ( typeof( extended->afl_obsolete.bit ) ) 1 << ( extended->flag_bitnum - 1 ) ) == extended->afl_obsolete.bit ); */

        break;
      case DUF_OPTION_VTYPE_NOBSFLAG:                               /* stage SETUP */
        MAST_TRACE( options, 70, "vtype %s", QSTR( DUF_OPTION_VTYPE_NOBSFLAG ) + 17 );
        nof = !nof;
      case DUF_OPTION_VTYPE_BSFLAG:                                 /* stage SETUP *//*  unsigned short set of flags */
        MAST_TRACE( options, 70, "vtype %s", QSTR( DUF_OPTION_VTYPE_BSFLAG ) + 17 );
        DUF_OPTION_VTYPE_QBFLAG( unsigned short );

      /* ssert( ( extended->flag_bitnum && !extended->afl_obsolete.bit )                                                             */
      /*         || ( ( typeof( extended->afl_obsolete.bit ) ) 1 << ( extended->flag_bitnum - 1 ) ) == extended->afl_obsolete.bit ); */

        break;

      case DUF_OPTION_VTYPE_NOBXFLAG:                               /* stage SETUP */
        MAST_TRACE( options, 70, "vtype %s", QSTR( DUF_OPTION_VTYPE_NOBXFLAG ) + 17 );
        nof = !nof;
      case DUF_OPTION_VTYPE_BXFLAG:                                 /* stage SETUP *//*  unsigned short set of flags */
        MAST_TRACE( options, 70, "vtype %s", QSTR( DUF_OPTION_VTYPE_BXFLAG ) + 17 );
        DUF_OPTION_VTYPE_XQBFLAG;

        break;

      case DUF_OPTION_VTYPE_XCHR:                                   /* stage SETUP */
        MAST_TRACE( options, 70, "vtype XCHR for %s='%s'", extended->o.name, optargg ? optargg : "" );
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        assert( byteptr );
        if ( QNOERR )
        {
          char *pchr;

          pchr = ( char * ) byteptr;
          if ( optargg )
          {
            unsigned cc = 0;

            sscanf( optargg, "%x", &cc );
            *pchr = ( char ) cc;
            MAST_TRACE( options, +150, "char set(%x):'%c' @%p", cc, cc, pchr );
          }
        }
        break;
      case DUF_OPTION_VTYPE_STR:                                    /* stage SETUP */
      /* FIXME DUF_OPTION_VTYPE_PSTR vs. DUF_OPTION_VTYPE_STR */
        MAST_TRACE( options, 70, "vtype STR for %s='%s'", extended->o.name, optargg ? optargg : "" );
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        assert( byteptr );
        if ( QNOERR )
        {
          char **pstr;

          pstr = ( char ** ) byteptr;
          if ( pstr && *pstr )
            mas_free( *pstr );
          *pstr = NULL;
          if ( optargg )
          {
            *pstr = mas_strdup( optargg );
            MAST_TRACE( options, +140, "string set:%s @%p", optargg, pstr );
          }
        }
        break;
      case DUF_OPTION_VTYPE_PATH:
        mas_free( doptargg );
        doptargg = mas_normalize_path( optargg );
      case DUF_OPTION_VTYPE_CSTR:                                   /* stage SETUP */
        MAST_TRACE( options, 70, "vtype %s for %s='%s'", QSTR( DUF_OPTION_VTYPE_CSTR ) + 17, extended->o.name, doptargg ? doptargg : "" );
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        assert( byteptr );
        if ( QNOERR )
        {
          mas_expandable_string_t *pcs_x;

          pcs_x = ( mas_expandable_string_t * ) byteptr;
          if ( pcs_x )
          {
            mas_expandable_string_delete( pcs_x );
            if ( doptargg )
            {
              pcs_x->value = doptargg;
              doptargg = NULL;
              MAST_TRACE( options, +140, "string set:%s @%p", doptargg, pcs_x->value );
            }
          }
        }
        break;
      case DUF_OPTION_VTYPE_ARGV:                                   /* stage SETUP */
      /* case DUF_OPTION_VTYPE_PAA: */
        MAST_TRACE( options, 70, "vtype PAA" );

        assert( byteptr );
        if ( QNOERR )
        {
          mas_argvc_t *parg;

          parg = ( mas_argvc_t * ) byteptr;
          if ( noo )
          {
            if ( parg )
              mas_argv_delete( parg->argc, parg->argv );
          }
          else if ( optargg && *optargg )
          {
            parg->argc = mas_add_argv_arg( parg->argc, &parg->argv, optargg );
          }
          else if ( extended->m_hasoff == 1 )                        /* if  extended->m_hasoff == 1, then mcfg_offset is offset */
          {
            char *str = mas_argv_string( parg->argc, parg->argv, 0 );

            DUF_PRINTF( 0, "%s", ( str ) );
            mas_free( str );
          }
        }
        break;
      case DUF_OPTION_VTYPE_TDB:                                    /* stage SETUP */
        MAST_TRACE( options, 70, "vtype TDB" );

      /* TODO: reserved: out-of-multiconfig-library, weak - too expensive to include into tmpdb_add multiconfig library */
        if ( QNOERR )
        {
          DR( OPTIONS, tmpdb_add, duf_config_cli_t * cli, int oval, const char *optname, const char *optargg )
                  __attribute__ ( ( warn_unused_result, weak ) );
          if ( noo )
          {
#if 0
            ... _tmpdb_delete( extended->o.name, optargg );
#endif
          }
          else if ( optargg && *optargg && F2N( duf_, tmpdb_add ) )
            IF_CR( tmpdb_add, cli, extended->o.val, extended->o.name, optargg );
        }
        break;
      /*
       * DATETIME, MINMAXDATETIME, MINDATETIME, MAXDATETIME
       * */
      case DUF_OPTION_VTYPE_DATETIME:                               /* stage SETUP */
        MAST_TRACE( options, 70, "vtype DATETIME" );
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        DUF_NUMOPT( noo, unsigned long long, 0, mas_strtime2long );

        break;
      case DUF_OPTION_VTYPE_MINMAXDATETIME:                         /* stage SETUP */
        MAST_TRACE( options, 70, "vtype MINMAXDATETIME" );
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        DUF_MINMAXOPT( noo, mas_limitsll_t, mas_strtime2long );
        break;
      case DUF_OPTION_VTYPE_MINDATETIME:                            /* stage SETUP */
        MAST_TRACE( options, 70, "vtype MINDATETIME" );
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        DUF_MOPT( noo, mas_limitsll_t, min, mas_strtime2long );
        break;
      case DUF_OPTION_VTYPE_MAXDATETIME:                            /* stage SETUP */
        MAST_TRACE( options, 70, "vtype MAXDATETIME" );
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        DUF_MOPT( noo, mas_limitsll_t, max, mas_strtime2long );
        break;
      /* 
       * FILE
       * */
      case DUF_OPTION_VTYPE_FILE:
        MAST_TRACE( options, 70, "vtype FILE" );
      /* T( "@############ %d :: %s :: %s #############", extended->vtype, extended->o.name, duf_optsource_name(source) ); */
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        if ( QNOERR )
        {
          MAST_TRACE( io, 0, "DUF_OUTPUTFILE (%s) : %d", extended->o.name, extended->call.value.u );
          DUF_OUTPUTFILE( cli, noo, mas_base_output_t /* _typ */ , /* _stream_dot */ , stderr /* _defout */  );

        /* ERRMAKE( OPTION_NOT_PARSED); */
        }
        break;
      }
      mas_free( doptargg );
      MAST_TRACE( options, +150, "@@@@@(%s)         this (%2d:%2d:%2d) stage; vtype=%2d; xname:%-20s; arg:'%s'; no:%d", QERRNAME,
                  istage, extended->stage_opts.stage.min, extended->stage_opts.stage.max, extended->vtype, extended ? extended->o.name : "?",
                  optargg ? optargg : "", noo );
    }
#if 0
    else
    {
      MAST_TRACE( options, 60, "@--%s='%s'; `noo`:%d : NOT for this stage; istage:%s", extended ? extended->o.name : "?", optargg ? optargg : "", noo,
                  duf_optstage_name( cli, istage ) );
    /* ERRMAKE( OPTION_NOT_FOUND ); */
    }
#endif
  }

  ER( OPTIONS,
      xoption_clarify_typed_gen, duf_config_cli_t * cli, const duf_longval_extended_t * extended, const char *optargg, unsigned noo,
      duf_option_stage_t istage MAS_UNUSED, duf_option_source_t source MAS_UNUSED );
}
