/* #undef MAS_TRACING */
#define MAST_TRACE_CONFIG duf_get_cli_options_trace_config(cli)
#include <assert.h>                                                  /* assert */
#include <stdio.h>                                                   /* FILE */
#include <string.h>
#include <sys/stat.h>                                                /* struct stat */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */
#include <mastar/tools/mas_convert.h>                                /* mas_strtol_suff; mas_strtoll_suff; etc. ♣ */
#include <mastar/tools/mas_expandable.h>
#include <mastar/tools/mas_time.h>                                   /* mas_(|double_|xlocal|xgm|xvstrf|xvstrftime_|(|t)strflocal|strfgm)time ; strtime2long; etc. ♣ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ♣ */
#include <mastar/tools/mas_utils_path.h>                             /* mas_normalize_path; mas_pathdepth; mas_realpath etc. ♣ */

#include "duf_base_types.h"                                          /* duf_limits_t */

#include "duf_tracen_defs.h"                                         /* DUF_TRACE ♠ */
#include "duf_printn_defs.h"                                         /* DUF_PRINTF etc. ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_se.h"                                                  /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

/* #include "duf_utils.h"                                               (* duf_strtol_suff duf_strtoll_suff etc. ♠ *) */

#include "duf_option_stage.h"                                        /* duf_optstage_name ♠ */
#include "duf_option_source.h"                                       /* duf_optsource_name ♠ */
#include "duf_option_config.h"                                       /* duf_get_cli_options_trace_config ♠ */

#include "duf_option_tmpdb.h"

/* #include "duf_expandable.h"                                          (* duf_expandable_string_t; duf_string_expanded ♠ *) */

/* ###################################################################### */
#include "duf_option_typed_defs.h"
#include "duf_option_typed_gen.h"
/* ###################################################################### */

static FILE *
duf_open_file_special( duf_config_cli_t * cli, const char *pname, char **popenedname, int overwrite, int *pr )
{
  int rpr = 0;
  FILE *newfile = NULL;
  int overw = 0;
  const char *mode = "w";
  struct stat st;
  int skipchar = 0;
  int ry;

  DUF_TRACE( io, 0, "to open %s", pname );
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
  /* DUF_SHOW_ERROR( "can't open file %s for writing file exists %llu / %llu chr:%d\n", pname, ( unsigned long long ) st.st_dev, */
  /*                 ( unsigned long long ) st.st_rdev, S_ISCHR( st.st_mode ) );                                                 */
    DUF_MAKE_ERRORM( rpr, DUF_ERROR_FILE_EXISTS, "can't open file %s for writing file exists %llu / %llu chr:%d", pname,
                     ( unsigned long long ) st.st_dev, ( unsigned long long ) st.st_rdev, S_ISCHR( st.st_mode ) );
  }
  else
  {
    newfile = fopen( pname, mode );
    if ( !newfile )
    {
    /* DUF_SHOW_ERROR( "can't open file %s\n", pname ); */
      DUF_MAKE_ERRORM( rpr, DUF_ERROR_OPEN, "can't open file %s", pname );
    }
  }
  if ( pr )
    *pr = rpr;
  return newfile;
}

/*  20150924.120214 for DUF_OUTPUTFILE DUF_OUTPUTFILE_A */
static int
duf_set_file_special( duf_config_cli_t * cli, const char *pname, int overwrite, char **pfilename, FILE ** pfile, FILE * defout, int handleid )
{
  DUF_STARTR( r );
  FILE *newout;

  newout = defout;
  DUF_TRACE( io, 0, "to set file special %s", pname );
  if ( pfile )
  {
    *pfile = NULL;

    DUF_TRACE( io, 0, "handleid %d", handleid );
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
    DUF_TRACE( io, 0, "pname %s", pname );
    if ( pfilename )
    {
      if ( *pfilename )
        mas_free( *pfilename );
      *pfilename = NULL;
    }
    if ( pname && *pname )
      newout = duf_open_file_special( cli, pname, pfilename, overwrite, &r );
    if ( !newout )
      newout = defout;
    if ( newout )
    {
      if ( *pfile && *pfile != stderr && *pfile != stdout )
        fclose( *pfile );
      *pfile = newout;
    }
  }
  DUF_ENDR( r );
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

  DUF_TRACE( options, 60, "to switch by extended->relto=%d", extended->relto );
/* TODO relto=duf_get_config_puz_offset etc. */
  switch ( extended->relto )
  {
  case DUF_OFFSET_none:
    assert( extended->m_offset == 0 );
    break;
#if 0
  case DUF_OFFSET_config:
    DUF_TRACE( options, 60, "relto=%d", extended->relto );
# if 0
    byte_ptr = ( ( ( char * ) duf_config ) + extended->m_offset );
# else
    byte_ptr = duf_get_config_offset( extended->m_offset );
    if ( extended->reltoptr )
      assert( byte_ptr == ( void * ) ( ( ( char * ) ( ( duf_pvoid_void_func_t ) extended->reltoptr ) (  ) ) + extended->m_offset ) );
  /* assert( byte_ptr == ( ( ( char * ) duf_config ) + extended->m_offset ) ); */
# endif
    break;
# if 1
  /* case DUF_OFFSET_depthinfo:                                              */
  /*   DUF_TRACE( options, 60, "relto=%d", extended->relto );                */
  /*   byte_ptr = ( ( ( char * ) duf_pdi_global(  ) ) + extended->m_offset ); */
  /*   break;                                                                */
  case DUF_OFFSET_ufilter:
    DUF_TRACE( options, 60, "relto=%d", extended->relto );
  /* byte_ptr = ( ( ( char * ) DUF_CONFIGG( vars.puz ) ) + extended->m_offset ); */
    byte_ptr = duf_get_config_puz_offset( extended->m_offset );
    if ( extended->reltoptr )
      assert( byte_ptr == ( void * ) ( ( ( char * ) ( ( duf_pvoid_void_func_t ) extended->reltoptr ) (  ) ) + extended->m_offset ) );
    break;
# endif
#endif
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

int
duf_xoption_clarify_typed_gen( duf_config_cli_t * cli, const duf_longval_extended_t * extended, const char *optargg, unsigned noo,
                               duf_option_stage_t istage MAS_UNUSED, duf_option_source_t source MAS_UNUSED )
{
  DUF_STARTR( r );

/* if ( !extended )                         */
/*   DUF_MAKE_ERROR( r, DUF_ERROR_OPTION ); */

/*
 * if arg is help option
 * return class id for options to display the help
 * */
  if ( DUF_NOERROR( r ) )
  {
    unsigned doplus = 0;
    void *byteptr = NULL;

    byteptr = duf_xoption_clarify_typed_byteptr( cli, extended );
    DUF_TRACE( options, 60, "to check stage; istage:%s", duf_optstage_name( cli, istage ) );
  /* if ( DUF_OPTION_CHECK_STAGE( istage, extended, xtable ) ) *//* moved upper */
    {
      unsigned nof;
      char *doptargg;

      nof = noo;
      DUF_TRACE( options, 60, "stage OK; switching by extended->vtype=%d", extended->vtype );

      doptargg = mas_strdup( optargg );
      switch ( extended->vtype )
      {
      case DUF_OPTION_VTYPE_NONE:
        DUF_TRACE( options, 70, "vtype NONE" );
#if 0
        if ( DUF_NOERROR( r ) )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );          // Why was ???
      /* DUF_TEST_R( r ); */
#endif
        break;
      case DUF_OPTION_VTYPE_NOOP:
        DUF_TRACE( options, 70, "vtype NOOP" );
        break;
      case DUF_OPTION_VTYPE_UPLUS:                                  /* stage (?) not SETUP *//* --trace-...; --debug; --verbose */
        DUF_TRACE( options, 70, "vtype UPLUS" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
          doplus = 1;
      /* assert( 0 ); */
      /* assert( ( ( char * ) ( duf_config->opt.ptracecfg->class_levels ) ) + ( extended->m_offset ) == ( ( char * ) byteptr ) ); */
      case DUF_OPTION_VTYPE_NUM:                                    /* stage SETUP *//* --max-...; --min-...; --output-level; --use-format; etc. (misc) */
        DUF_TRACE( options, 70, "vtype NUM --%s='%s'", extended->o.name, optargg ? optargg : "" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_NUMOPT( noo, r, unsigned, doplus, mas_strtol_suff );

        DUF_TRACE( options, 70, "(%d) vtype NUM --%s='%s'", r, extended->o.name, optargg ? optargg : "" );
        break;
      case DUF_OPTION_VTYPE_NUML:                                   /* stage SETUP *//* not used ?! */
        DUF_TRACE( options, 70, "vtype NL" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_NUMOPT( noo, r, unsigned long, 0, mas_strtol_suff );

        break;
      case DUF_OPTION_VTYPE_NUMLL:                                  /* stage SETUP *//* not used ?! */
        DUF_TRACE( options, 70, "vtype NLL" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_NUMOPT( noo, r, unsigned long long, 0, mas_strtoll_suff );

        break;
      case DUF_OPTION_VTYPE_MIN:                                    /* stage SETUP */
        DUF_TRACE( options, 70, "vtype MIN" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MOPT( noo, r, duf_limits_t, min, mas_strtol_suff );
        break;
      case DUF_OPTION_VTYPE_MAX:                                    /* stage SETUP */
        DUF_TRACE( options, 70, "vtype MAX" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MOPT( noo, r, duf_limits_t, max, mas_strtol_suff );
        break;
      case DUF_OPTION_VTYPE_MINMAX:                                 /* stage SETUP */
        DUF_TRACE( options, 70, "vtype MINMAX" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MINMAXOPT( noo, r, duf_limits_t, mas_strtol_suff );
        break;

      case DUF_OPTION_VTYPE_MINLL:                                  /* stage SETUP */
        DUF_TRACE( options, 70, "vtype MINLL" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MOPT( noo, r, duf_limitsll_t, min, mas_strtol_suff );
        break;
      case DUF_OPTION_VTYPE_MAXLL:                                  /* stage SETUP */
        DUF_TRACE( options, 70, "vtype MAXLL" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MOPT( noo, r, duf_limitsll_t, max, mas_strtoll_suff );
        break;
      case DUF_OPTION_VTYPE_MINMAXLL:                               /* stage SETUP */
        DUF_TRACE( options, 70, "vtype MINMAXLL" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MINMAXOPT( noo, r, duf_limitsll_t, mas_strtoll_suff );
        break;
#if 0
# define DUF_OPTION_VTYPE_XFLAG_OBSOLETE(_typ) \
	{ \
	  if (!byteptr) \
	    DUF_MAKE_ERROR( r, DUF_ERROR_OPTION ); \
          if ( extended->unset ) \
            nof = !nof; \
          assert( byteptr ); \
          if ( DUF_NOERROR( r ) ) \
          { \
            _typ *pix; \
            pix = ( _typ * ) byteptr; \
            if ( nof ) \
              ( *pix ) &= ~extended->afl_obsolete.bit; \
            else \
              ( *pix ) |= extended->afl_obsolete.bit; \
            /* duf_config->opt.act.v.flag.info = 1; */ \
            DUF_TRACE( options, +140, "@@@@@@[%d] %p %p :%s: %x %x (%x)", nof, byteptr, pix, extended->o.name, *pix, \
                       extended->afl_obsolete.bit, ( *pix | extended->afl_obsolete.bit ) ); \
            DUF_TEST_R( r ); \
          } \
        }
      case DUF_OPTION_VTYPE_NOFLAG:                                 /* stage SETUP */
        DUF_TRACE( options, 70, "vtype %s", QSTR( DUF_OPTION_VTYPE_NOFLAG ) + 17 );
        nof = !nof;
      case DUF_OPTION_VTYPE_FLAG:                                   /* stage SETUP *//*  unsigned set of flags */
        DUF_TRACE( options, 70, "vtype %s %x", QSTR( DUF_OPTION_VTYPE_FLAG ) + 17, extended->afl_obsolete.bit );
        DUF_OPTION_VTYPE_XFLAG_OBSOLETE( unsigned );

        assert( ( !extended->flag_bitnum && extended->afl_obsolete.bit )
                || ( ( typeof( extended->afl_obsolete.bit ) ) 1 << ( extended->flag_bitnum - 1 ) ) == extended->afl_obsolete.bit );

        break;
      case DUF_OPTION_VTYPE_NOSFLAG:                                /* stage SETUP */
        DUF_TRACE( options, 70, "vtype %s", QSTR( DUF_OPTION_VTYPE_NOSFLAG ) + 17 );
        nof = !nof;
      case DUF_OPTION_VTYPE_SFLAG:                                  /* stage SETUP *//*  unsigned short set of flags */
        DUF_TRACE( options, 70, "vtype %s", QSTR( DUF_OPTION_VTYPE_SFLAG ) + 17 );
        DUF_OPTION_VTYPE_XFLAG_OBSOLETE( unsigned short );

        assert( ( !extended->flag_bitnum && extended->afl_obsolete.bit )
                || ( ( typeof( extended->afl_obsolete.bit ) ) 1 << ( extended->flag_bitnum - 1 ) ) == extended->afl_obsolete.bit );

        break;
#endif

#define DUF_OPTION_VTYPE_QBFLAG(_typ) \
	{ \
	  if (!byteptr) \
	    DUF_MAKE_ERROR( r, DUF_ERROR_OPTION ); \
          if ( extended->unset ) \
            nof = !nof; \
          assert( byteptr ); \
          if ( DUF_NOERROR( r ) ) \
          { \
            _typ *pix; \
	    _typ nz=~0; \
            pix = ( _typ * ) byteptr; \
            if ( nof ) \
              ( *pix ) &= (_typ)((~(((_typ) 1)<<(extended->flag_bitnum-1))) & nz); \
            else \
              ( *pix ) |= (_typ)( ( ((_typ) 1)<<(extended->flag_bitnum-1)) & nz ); \
            /* duf_config->opt.act.v.flag.info = 1; */ \
            DUF_TRACE( options, +140, "@@@@@@[%d] %p %p :%s: %llx %llx (%llx)", nof, byteptr, pix, extended->o.name, (unsigned long long)*pix, \
                       (unsigned long long)(1<<(extended->flag_bitnum-1)), (unsigned long long)( *pix | (1<<(extended->flag_bitnum-1)) ) ); \
            DUF_TEST_R( r ); \
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
        DUF_TRACE( options, 70, "vtype %s", QSTR( DUF_OPTION_VTYPE_NOBFLAG ) + 17 );
        nof = !nof;
      case DUF_OPTION_VTYPE_BFLAG:                                  /* stage SETUP *//*  unsigned set of flags */
        DUF_TRACE( options, 70, "vtype %s %lx", QSTR( DUF_OPTION_VTYPE_BFLAG ) + 17, ( 1L << ( extended->flag_bitnum - 1 ) ) );
        DUF_OPTION_VTYPE_QBFLAG( unsigned );

        /* assert( ( extended->flag_bitnum && !extended->afl_obsolete.bit )                                                            */
        /*         || ( ( typeof( extended->afl_obsolete.bit ) ) 1 << ( extended->flag_bitnum - 1 ) ) == extended->afl_obsolete.bit ); */

        break;
      case DUF_OPTION_VTYPE_NOBSFLAG:                               /* stage SETUP */
        DUF_TRACE( options, 70, "vtype %s", QSTR( DUF_OPTION_VTYPE_NOBSFLAG ) + 17 );
        nof = !nof;
      case DUF_OPTION_VTYPE_BSFLAG:                                 /* stage SETUP *//*  unsigned short set of flags */
        DUF_TRACE( options, 70, "vtype %s", QSTR( DUF_OPTION_VTYPE_BSFLAG ) + 17 );
        DUF_OPTION_VTYPE_QBFLAG( unsigned short );

        /* ssert( ( extended->flag_bitnum && !extended->afl_obsolete.bit )                                                             */
        /*         || ( ( typeof( extended->afl_obsolete.bit ) ) 1 << ( extended->flag_bitnum - 1 ) ) == extended->afl_obsolete.bit ); */

        break;

      case DUF_OPTION_VTYPE_NOBXFLAG:                               /* stage SETUP */
        DUF_TRACE( options, 70, "vtype %s", QSTR( DUF_OPTION_VTYPE_NOBXFLAG ) + 17 );
        nof = !nof;
      case DUF_OPTION_VTYPE_BXFLAG:                                 /* stage SETUP *//*  unsigned short set of flags */
        DUF_TRACE( options, 70, "vtype %s", QSTR( DUF_OPTION_VTYPE_BXFLAG ) + 17 );
        DUF_OPTION_VTYPE_XQBFLAG;

        break;

#if 0
      case DUF_OPTION_VTYPE_PFLAG:
        DUF_TRACE( options, 70, "vtype PFLAG" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        assert( byteptr );
        if ( DUF_NOERROR( r ) )
        {
          unsigned *pi;

          pi = ( unsigned * ) byteptr;
          DUF_PRINTF( 0, "%s is %s", extended->o.name, ( ( *pi ) & extended->afl_obsolete.bit ) ? "ON" : "OFF" );
        /* DUF_TEST_R( r ); */
        }
        break;
#endif
#if 0
      case DUF_OPTION_VTYPE_PSFLAG:
        DUF_TRACE( options, 70, "vtype PSFLAG" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        assert( byteptr );
        if ( DUF_NOERROR( r ) )
        {
          unsigned *pi;

          pi = ( unsigned * ) byteptr;
          DUF_PRINTF( 0, "(s) %s is %s", extended->o.name, ( ( *pi ) & extended->afl_obsolete.sbit ) ? "ON" : "OFF" );
        /* DUF_TEST_R( r ); */
        }
        break;
#endif
      case DUF_OPTION_VTYPE_XCHR:                                   /* stage SETUP */
        DUF_TRACE( options, 70, "vtype XCHR for %s='%s'", extended->o.name, optargg ? optargg : "" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        assert( byteptr );
        if ( DUF_NOERROR( r ) )
        {
          char *pchr;

          pchr = ( char * ) byteptr;
          if ( optargg )
          {
            unsigned cc = 0;

            sscanf( optargg, "%x", &cc );
            *pchr = ( char ) cc;
            DUF_TRACE( options, +150, "char set(%x):'%c' @%p", cc, cc, pchr );
          }
        }
        break;
      case DUF_OPTION_VTYPE_STR:                                    /* stage SETUP */
      /* FIXME DUF_OPTION_VTYPE_PSTR vs. DUF_OPTION_VTYPE_STR */
        DUF_TRACE( options, 70, "vtype STR for %s='%s'", extended->o.name, optargg ? optargg : "" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        assert( byteptr );
        if ( DUF_NOERROR( r ) )
        {
          char **pstr;

          pstr = ( char ** ) byteptr;
          if ( pstr && *pstr )
            mas_free( *pstr );
          *pstr = NULL;
          if ( optargg )
          {
            *pstr = mas_strdup( optargg );
            DUF_TRACE( options, +140, "string set:%s @%p", optargg, pstr );
          }
        }
        break;
      case DUF_OPTION_VTYPE_PATH:
        mas_free( doptargg );
        doptargg = mas_normalize_path( optargg );
      case DUF_OPTION_VTYPE_CSTR:                                   /* stage SETUP */
        DUF_TRACE( options, 70, "vtype %s for %s='%s'", QSTR( DUF_OPTION_VTYPE_CSTR ) + 17, extended->o.name, doptargg ? doptargg : "" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        assert( byteptr );
        if ( DUF_NOERROR( r ) )
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
              DUF_TRACE( options, +140, "string set:%s @%p", doptargg, pcs_x->value );
            }
          }
        }
        break;
      case DUF_OPTION_VTYPE_ARGV:                                   /* stage SETUP */
      /* case DUF_OPTION_VTYPE_PAA: */
        DUF_TRACE( options, 70, "vtype PAA" );

        assert( byteptr );
        if ( DUF_NOERROR( r ) )
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
        DUF_TRACE( options, 70, "vtype TDB" );

        if ( DUF_NOERROR( r ) )
        {
          if ( noo )
          {
#if 0
            duf_tmpdb_delete( extended->o.name, optargg );
#endif
          }
          else if ( optargg && *optargg && duf_tmpdb_add )
            DOR( r, duf_tmpdb_add( cli, extended->o.val, extended->o.name, optargg ) );
        }
        break;
      /*
       * DATETIME, MINMAXDATETIME, MINDATETIME, MAXDATETIME
       * */
      case DUF_OPTION_VTYPE_DATETIME:                               /* stage SETUP */
        DUF_TRACE( options, 70, "vtype DATETIME" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_NUMOPT( noo, r, unsigned long long, 0, mas_strtime2long );

        break;
      case DUF_OPTION_VTYPE_MINMAXDATETIME:                         /* stage SETUP */
        DUF_TRACE( options, 70, "vtype MINMAXDATETIME" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MINMAXOPT( noo, r, duf_limitsll_t, mas_strtime2long );
        break;
      case DUF_OPTION_VTYPE_MINDATETIME:                            /* stage SETUP */
        DUF_TRACE( options, 70, "vtype MINDATETIME" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MOPT( noo, r, duf_limitsll_t, min, mas_strtime2long );
        break;
      case DUF_OPTION_VTYPE_MAXDATETIME:                            /* stage SETUP */
        DUF_TRACE( options, 70, "vtype MAXDATETIME" );
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MOPT( noo, r, duf_limitsll_t, max, mas_strtime2long );
        break;
      /* 
       * FILE
       * */
      case DUF_OPTION_VTYPE_FILE:
        DUF_TRACE( options, 70, "vtype FILE" );
      /* T( "@############ %d :: %s :: %s #############", extended->vtype, extended->o.name, duf_optsource_name(source) ); */
        if ( noo )
          DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
        {
          DUF_TRACE( io, 0, "DUF_OUTPUTFILE (%s) : %d", extended->o.name, extended->call.value.u );
          DUF_OUTPUTFILE( cli, noo, r, mas_base_output_t,, stderr );
        /* {                                                                                 */
        /*   char start_time[128] = "??";                                                    */
        /*                                                                                   */
        /*   duf_strflocaltime( start_time, sizeof( start_time ), "%Y%m%d.%H:%M:%S", NULL ); */
        /*   PF( "%s", start_time );                                                         */
        /* }                                                                                 */

        /* DUF_MAKE_ERROR(r, DUF_ERROR_OPTION_NOT_PARSED); */
        }
        break;
      }
      mas_free( doptargg );
      DUF_TRACE( options, +150, "@@@@@(%s)         this (%2d:%2d:%2d) stage; vtype=%2d; xname:%-20s; arg:'%s'; no:%d", mas_error_name_i( r ),
                 istage, extended->stage_opts.stage.min, extended->stage_opts.stage.max, extended->vtype, extended ? extended->o.name : "?",
                 optargg ? optargg : "", noo );
    }
#if 0
    else
    {
      DUF_TRACE( options, 60, "@--%s='%s'; `noo`:%d : NOT for this stage; istage:%s", extended ? extended->o.name : "?", optargg ? optargg : "", noo,
                 duf_optstage_name( cli, istage ) );
    /* DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_FOUND ); */
    }
#endif
  }
  DUF_ENDR( r );
/* DUF_ENDR_YES( r, DUF_ERROR_OPTION_NOT_FOUND ); */
}
