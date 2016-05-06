/* #undef MAS_TRACING */
/* #define MAST_TRACE_CONFIG muc_get_cli_options_trace_config(cli) */
#include "muc_tracen_defs_preset.h"
#include "muc_config_trace_enum.h"

#include "muc_errorn_defs_preset.h"

#include <assert.h>                                                  /* assert */
#include <stdio.h>                                                   /* FILE */
#include <string.h>
#include <sys/stat.h>                                                /* struct stat */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/types/mas_common_types.h>                           /* mas_limits_t; mas_limitsll_t */
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */
#include <mastar/tools/mas_convert.h>                                /* mas_strtol_suff; mas_strtoll_suff; etc. ♣ */
#include <mastar/tools/mas_expandable.h>
#include <mastar/tools/mas_time.h>                                   /* mas_(|double_|xlocal|xgm|xvstrf|xvstrftime_|(|t)strflocal|strfgm)time ; strtime2long; etc. ♣ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ♣ */
#include <mastar/tools/mas_utils_path.h>                             /* mas_normalize_path; mas_pathdepth; mas_realpath etc. ♣ */
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs.h>                             /* MASE_TEST_R; MASE_TEST_R_LOWERE; ... */
#include <mastar/error/mas_error_defs_make.h>                        /* MASE_MAKE_ERROR; MASE_MAKE_ERRORFL; MASE_MAKE_ERRORM  ... */

#include "muc_se_only.h"                                             /* Only muc_DR; muc_SR; muc_ER; muc_CR; muc_QSTR; QERRIND; QERRNAME etc. ♠ */

#include "muc_option_stage.h"                                        /* muc_optstage_name ♠ */
#include "muc_option_source.h"                                       /* muc_optsource_name ♠ */
#include "muc_option_config.h"                                       /* muc_get_cli_options_trace_config ♠ */

#include "muc_optmod_tmpdb.h"

#include "muc_config_cli_structs.h"
/* ###################################################################### */
#include "muc_option_typed_defs.h"
#include "muc_option_typed_gen.h"
/* ###################################################################### */

static
#if 0
FILE *
muc_open_file_special( muc_config_cli_t * cli, const char *pname, char **popenedname, int overwrite, int *pr )
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
muc_SRP( OPTIONS, FILE *, newfile, NULL, open_file_special, muc_config_cli_t * cli, const char *pname, char **popenedname, int overwrite )
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
    ERRMAKE_M( FILE_EXISTS, "can't open file %s for writing file exists %llu / %llu chr:%d", pname,
               ( unsigned long long ) st.st_dev, ( unsigned long long ) st.st_rdev, S_ISCHR( st.st_mode ) );
  }
  else
  {
    newfile = fopen( pname, mode );
# if 0
    if ( !newfile )
    {
      ERRMAKE_M( OPEN, "can't open file %s", pname );
    }
# else
    ERRMAKE_IF_M( !newfile, OPEN, "can't open file %s", pname );
# endif
  }
  muc_ERP( OPTIONS, FILE *, newfile, NULL, open_file_special, muc_config_cli_t * cli, const char *pname, char **popenedname, int overwrite );
}
#endif
/*  20150924.120214 for MUC_OUTPUTFILE MUC_OUTPUTFILE_A */
static
muc_SR( OPTIONS, set_file_special, muc_config_cli_t * cli, const char *pname, int overwrite, char **pfilename, FILE ** pfile, FILE * defout,
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
      newout = muc_open_file_special( cli, pname, pfilename, overwrite, QPERRIND );
    if ( !newout )
      newout = defout;
    if ( newout )
    {
      if ( *pfile && *pfile != stderr && *pfile != stdout )
        fclose( *pfile );
      *pfile = newout;
    }
  }

  muc_ER( OPTIONS, set_file_special, muc_config_cli_t * cli, const char *pname, int overwrite, char **pfilename, FILE ** pfile, FILE * defout,
          int handleid );
}

static inline void *
get_offset( void *ptr, unsigned long off )
{
  assert( ptr );
  return ptr ? ( void * ) ( ( ( char * ) ptr ) + off ) : NULL;
}

static void *
muc_xoption_clarify_typed_byteptr( muc_config_cli_t * cli, const muc_longval_extended_t * extended )
{
  void *byte_ptr = NULL;

  MAST_TRACE( options, 60, "to switch by extended->relto=%d", extended->relto );
/* TODO relto=..._get_config_puz_offset etc. */
  switch ( extended->relto )
  {
  case MUC_OFFSET_none:
    assert( extended->m_offset == 0 );
    break;
  case MUC_OFFSET_varptr:
    byte_ptr = get_offset( extended->reltoptr, extended->m_offset );
    break;
  case MUC_OFFSET_funcptr:
    {
      void *aptr;

      aptr = ( ( muc_pvoid_void_func_t ) extended->reltoptr ) (  );
      byte_ptr = get_offset( aptr, extended->m_offset );
    }
    break;
  }
  return byte_ptr;
}

muc_SR( OPTIONS, xoption_clarify_typed_gen, muc_config_cli_t * cli, const muc_longval_extended_t * extended, const char *optargg, unsigned noo,
        muc_option_stage_t istage MAS_UNUSED, muc_option_source_t source MAS_UNUSED )
{

/*
 * if arg is help option
 * return class id for options to display the help
 * */
  if ( QNOERR )
  {
    unsigned doplus = 0;
    void *byteptr = NULL;

    byteptr = muc_xoption_clarify_typed_byteptr( cli, extended );
    MAST_TRACE( options, 60, "to check stage; istage:%s", muc_optstage_name( cli, istage ) );
  /* if ( MUC_OPTION_CHECK_STAGE( istage, extended, xtable ) ) *//* moved upper */
    {
      unsigned nof;
      char *doptargg;

      nof = noo;
      MAST_TRACE( options, 60, "stage OK; switching by extended->vtype=%d", extended->vtype );

      doptargg = mas_strdup( optargg );
      switch ( extended->vtype )
      {
      case MUC_OPTION_VTYPE_NONE:
        MAST_TRACE( options, 70, "vtype NONE" );
#if 0
        if ( QNOERR )
          ERRMAKE( OPTION_NOT_PARSED );                              // Why was ???
      /* MUC_TEST_R( r ); */
#endif
        break;
      case MUC_OPTION_VTYPE_NOOP:
        MAST_TRACE( options, 70, "vtype NOOP" );
        break;
      case MUC_OPTION_VTYPE_UPLUS:                                  /* stage (?) not SETUP *//* --trace-...; --debug; --verbose */
        MAST_TRACE( options, 70, "vtype UPLUS" );
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        if ( QNOERR )
          doplus = 1;
      /* assert( 0 ); */
      /* assert( ( ( char * ) ( DUF_config->opt.ptracecfg->class_levels ) ) + ( extended->m_offset ) == ( ( char * ) byteptr ) ); */
      case MUC_OPTION_VTYPE_NUM:                                    /* stage SETUP *//* --max-...; --min-...; --output-level; --use-format; etc. (misc) */
        MAST_TRACE( options, 70, "vtype NUM --%s='%s'", extended->o.name, optargg ? optargg : "" );
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        MUC_NUMOPT( noo, unsigned, doplus, mas_strtol_suff );

        MAST_TRACE( options, 70, "(%d) vtype NUM --%s='%s'", QERRIND, extended->o.name, optargg ? optargg : "" );
        break;
      case MUC_OPTION_VTYPE_NUML:                                   /* stage SETUP *//* not used ?! */
        MAST_TRACE( options, 70, "vtype NL" );
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        MUC_NUMOPT( noo, unsigned long, 0, mas_strtol_suff );

        break;
      case MUC_OPTION_VTYPE_NUMLL:                                  /* stage SETUP *//* not used ?! */
        MAST_TRACE( options, 70, "vtype NLL" );
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        MUC_NUMOPT( noo, unsigned long long, 0, mas_strtoll_suff );

        break;
      case MUC_OPTION_VTYPE_MIN:                                    /* stage SETUP */
        MAST_TRACE( options, 70, "vtype MIN" );
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        MUC_MOPT( noo, mas_limits_t, min, mas_strtol_suff );
        break;
      case MUC_OPTION_VTYPE_MAX:                                    /* stage SETUP */
        MAST_TRACE( options, 70, "vtype MAX" );
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        MUC_MOPT( noo, mas_limits_t, max, mas_strtol_suff );
        break;
      case MUC_OPTION_VTYPE_MINMAX:                                 /* stage SETUP */
        MAST_TRACE( options, 70, "vtype MINMAX" );
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        MUC_MINMAXOPT( noo, mas_limits_t, mas_strtol_suff );
        break;

      case MUC_OPTION_VTYPE_MINLL:                                  /* stage SETUP */
        MAST_TRACE( options, 70, "vtype MINLL" );
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        MUC_MOPT( noo, mas_limitsll_t, min, mas_strtol_suff );
        break;
      case MUC_OPTION_VTYPE_MAXLL:                                  /* stage SETUP */
        MAST_TRACE( options, 70, "vtype MAXLL" );
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        MUC_MOPT( noo, mas_limitsll_t, max, mas_strtoll_suff );
        break;
      case MUC_OPTION_VTYPE_MINMAXLL:                               /* stage SETUP */
        MAST_TRACE( options, 70, "vtype MINMAXLL" );
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        MUC_MINMAXOPT( noo, mas_limitsll_t, mas_strtoll_suff );
        break;

#define MUC_OPTION_VTYPE_QBFLAG(_typ) \
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
#define MUC_OPTION_VTYPE_XQBFLAG \
	if (extended->vsize==sizeof(unsigned char)) \
	{  MUC_OPTION_VTYPE_QBFLAG(unsigned char); } \
	else if (extended->vsize==sizeof(unsigned short)) \
	{  MUC_OPTION_VTYPE_QBFLAG(unsigned char); } \
	else if (extended->vsize==sizeof(unsigned )) \
	{  MUC_OPTION_VTYPE_QBFLAG(unsigned ); } \
	else if (extended->vsize==sizeof(unsigned long)) \
	{  MUC_OPTION_VTYPE_QBFLAG(unsigned long); } \
	else if (extended->vsize==sizeof(unsigned long long)) \
	{  MUC_OPTION_VTYPE_QBFLAG(unsigned long long); }

      case MUC_OPTION_VTYPE_NOBFLAG:                                /* stage SETUP */
        MAST_TRACE( options, 70, "vtype %s", muc_QSTR( MUC_OPTION_VTYPE_NOBFLAG ) + 17 );
        nof = !nof;
      case MUC_OPTION_VTYPE_BFLAG:                                  /* stage SETUP *//*  unsigned set of flags */
        MAST_TRACE( options, 70, "vtype %s %lx", muc_QSTR( MUC_OPTION_VTYPE_BFLAG ) + 17, ( 1L << ( extended->flag_bitnum - 1 ) ) );
        MUC_OPTION_VTYPE_QBFLAG( unsigned );

      /* assert( ( extended->flag_bitnum && !extended->afl_obsolete.bit )                                                            */
      /*         || ( ( typeof( extended->afl_obsolete.bit ) ) 1 << ( extended->flag_bitnum - 1 ) ) == extended->afl_obsolete.bit ); */

        break;
      case MUC_OPTION_VTYPE_NOBSFLAG:                               /* stage SETUP */
        MAST_TRACE( options, 70, "vtype %s", muc_QSTR( MUC_OPTION_VTYPE_NOBSFLAG ) + 17 );
        nof = !nof;
      case MUC_OPTION_VTYPE_BSFLAG:                                 /* stage SETUP *//*  unsigned short set of flags */
        MAST_TRACE( options, 70, "vtype %s", muc_QSTR( MUC_OPTION_VTYPE_BSFLAG ) + 17 );
        MUC_OPTION_VTYPE_QBFLAG( unsigned short );

      /* ssert( ( extended->flag_bitnum && !extended->afl_obsolete.bit )                                                             */
      /*         || ( ( typeof( extended->afl_obsolete.bit ) ) 1 << ( extended->flag_bitnum - 1 ) ) == extended->afl_obsolete.bit ); */

        break;

      case MUC_OPTION_VTYPE_NOBXFLAG:                               /* stage SETUP */
        MAST_TRACE( options, 70, "vtype %s", muc_QSTR( MUC_OPTION_VTYPE_NOBXFLAG ) + 17 );
        nof = !nof;
      case MUC_OPTION_VTYPE_BXFLAG:                                 /* stage SETUP *//*  unsigned short set of flags */
        MAST_TRACE( options, 70, "vtype %s", muc_QSTR( MUC_OPTION_VTYPE_BXFLAG ) + 17 );
        MUC_OPTION_VTYPE_XQBFLAG;

        break;

      case MUC_OPTION_VTYPE_XCHR:                                   /* stage SETUP */
        MAST_TRACE( options, 70, "vtype XCHR for %s='%s'", extended->o.name, optargg ? optargg : "" );
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        assert( byteptr );
        if ( QNOERR )
        {
          char *pchr;

          pchr = ( char * ) byteptr;
        /* fprintf( stderr, "A-(%s)--------------------(( %s )) ------------------------\n", QERRNAME, optargg ); */
          if ( optargg )
          {
            unsigned cc = 0;

          /* fprintf( stderr, "B-(%s)--------------------(( %s )) ------------------------\n", QERRNAME, optargg ); */
            if ( optargg[0] == '\\' )
            {
              if ( optargg[1] == 'x' )
                sscanf( optargg + 2, "%x", &cc );
            }
            else if ( ( ( optargg[0] >= '0' && optargg[0] <= '9' ) || ( optargg[0] >= 'a' && optargg[0] <= 'f' )
                        || ( optargg[0] >= 'A' && optargg[0] <= 'F' ) )
                      && ( ( optargg[1] >= '0' && optargg[1] <= '9' ) || ( optargg[1] >= 'a' && optargg[1] <= 'f' )
                           || ( optargg[1] >= 'A' && optargg[1] <= 'F' ) ) )
            {
              sscanf( optargg, "%x", &cc );
            }
            else if ( optargg[0] && !optargg[1] )
            {
              cc = optargg[0];
            }
            *pchr = ( char ) cc;
          /* fprintf( stderr, "--------------------- %c ------------------------\n", cc ); */
            MAST_TRACE( options, +150, "char set(%x):'%c' @%p", cc, cc, pchr );
          }
        }
      /* fprintf( stderr, "C-(%s)--------------------(( %s )) -- %c%c ----------------------\n", QERRNAME, optargg,      */
      /*          cli->option_delimiter.env ? cli->option_delimiter.env : ' ? ',                                           */
      /*          cli->option_delimiters[MUC_OPTION_SOURCE_ENV] ? cli->option_delimiters[MUC_OPTION_SOURCE_ENV] : ' ? ' ); */
        break;
      case MUC_OPTION_VTYPE_STR:                                    /* stage SETUP */
      /* FIXME MUC_OPTION_VTYPE_PSTR vs. MUC_OPTION_VTYPE_STR */
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
      case MUC_OPTION_VTYPE_PATH:
        mas_free( doptargg );
        doptargg = mas_normalize_path( optargg );
      case MUC_OPTION_VTYPE_CSTR:                                   /* stage SETUP */
        MAST_TRACE( options, 70, "vtype %s for %s='%s'", muc_QSTR( MUC_OPTION_VTYPE_CSTR ) + 17, extended->o.name, doptargg ? doptargg : "" );
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
      case MUC_OPTION_VTYPE_ARGV:                                   /* stage SETUP */
      /* case MUC_OPTION_VTYPE_PAA: */
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

            MUC_PRINTF( 0, "%s", ( str ) );
            mas_free( str );
          }
        }
        break;
      case MUC_OPTION_VTYPE_TDB:                                    /* stage SETUP */
        MAST_TRACE( options, 70, "vtype TDB" );

      /* TODO: reserved: out-of-multiconfig-library, weak - too expensive to include into tmpdb_add multiconfig library */
        if ( QNOERR )
        {
          muc_DR( OPTIONS, tmpdb_add, muc_config_cli_t * cli, int oval, const char *optname, const char *optargg )
                  __attribute__ ( ( warn_unused_result, weak ) );
          if ( noo )
          {
#if 0
            ... _tmpdb_delete( extended->o.name, optargg );
#endif
          }
          else if ( optargg && *optargg && muc_F2N( muc_, tmpdb_add ) )
            muc_IF_CR( tmpdb_add, cli, extended->o.val, extended->o.name, optargg );
        }
        break;
      /*
       * DATETIME, MINMAXDATETIME, MINDATETIME, MAXDATETIME
       * */
      case MUC_OPTION_VTYPE_DATETIME:                               /* stage SETUP */
        MAST_TRACE( options, 70, "vtype DATETIME" );
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        MUC_NUMOPT( noo, unsigned long long, 0, mas_strtime2long );

        break;
      case MUC_OPTION_VTYPE_MINMAXDATETIME:                         /* stage SETUP */
        MAST_TRACE( options, 70, "vtype MINMAXDATETIME" );
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        MUC_MINMAXOPT( noo, mas_limitsll_t, mas_strtime2long );
        break;
      case MUC_OPTION_VTYPE_MINDATETIME:                            /* stage SETUP */
        MAST_TRACE( options, 70, "vtype MINDATETIME" );
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        MUC_MOPT( noo, mas_limitsll_t, min, mas_strtime2long );
        break;
      case MUC_OPTION_VTYPE_MAXDATETIME:                            /* stage SETUP */
        MAST_TRACE( options, 70, "vtype MAXDATETIME" );
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        MUC_MOPT( noo, mas_limitsll_t, max, mas_strtime2long );
        break;
      /* 
       * FILE
       * */
      case MUC_OPTION_VTYPE_FILE:
        MAST_TRACE( options, 70, "vtype FILE" );
      /* T( "@############ %d :: %s :: %s #############", extended->vtype, extended->o.name, muc_optsource_name(source) ); */
        if ( noo )
          ERRMAKE( OPTION_NOT_PARSED );
        if ( QNOERR )
        {
          MAST_TRACE( io, 0, "MUC_OUTPUTFILE (%s) : %d", extended->o.name, extended->call.value.u );
          MUC_OUTPUTFILE( cli, noo, mas_base_output_t /* _typ */ , /* _stream_dot */ , stderr /* _defout */  );

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
      MAST_TRACE( options, 60, "@--%s='%s'; `noo`:%d : NOT for this stage; istage:%s", extended ? extended->o.name : "?", optargg ? optargg : "",
                  noo, muc_optstage_name( cli, istage ) );
    /* ERRMAKE( OPTION_NOT_FOUND ); */
    }
#endif
  }

  muc_ER( OPTIONS, xoption_clarify_typed_gen, muc_config_cli_t * cli, const muc_longval_extended_t * extended, const char *optargg, unsigned noo,
          muc_option_stage_t istage MAS_UNUSED, muc_option_source_t source MAS_UNUSED );
}
