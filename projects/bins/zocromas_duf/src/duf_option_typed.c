#include <assert.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_utils.h"

#include "duf_config_ref.h"

/* #include "duf_pdi.h" */
#include "duf_levinfo_ref.h"

#include "duf_option_defs.h"
#include "duf_option_find.h"
#include "duf_option_tmpdb.h"

/* ###################################################################### */
#include "duf_option_typed_defs.h"
#include "duf_option_typed.h"
/* ###################################################################### */


static FILE *
duf_open_file_special( const char *pname, char **popenedname, int *pr )
{
  int rpr = 0;
  FILE *newfile = NULL;
  int overw = 0;
  const char *mode = "w";
  struct stat st;

  DUF_TRACE( io, 0, "to open %s", pname );
  if ( *pname == '@' )
  {
    overw = 1;
    pname++;
  }
  else if ( *pname == '+' )
  {
    mode = "a";
    pname++;
  }
  *popenedname = mas_strdup( pname );
  if ( 0 == stat( pname, &st ) && ( ( !S_ISCHR( st.st_mode ) || !( st.st_mode & S_IWUSR ) ) && ( !overw && *mode != 'a' ) ) )
  {
    DUF_SHOW_ERROR( "can't open file %s for writing file exists %llu / %llu chr:%d\n", pname, ( unsigned long long ) st.st_dev,
                    ( unsigned long long ) st.st_rdev, S_ISCHR( st.st_mode ) );
    DOR( rpr, DUF_ERROR_FILE_EXISTS );
  }
  else
  {
    newfile = fopen( pname, mode );
    if ( !newfile )
    {
      DUF_SHOW_ERROR( "can't open file %s\n", pname );
      DOR( rpr, DUF_ERROR_OPEN );
    }
  }
  if ( pr )
    *pr = rpr;
  return newfile;
}

static int
duf_set_file_special( const char *pname, char **pfilename, FILE ** pfile, FILE * defout, int handleid )
{
  DEBUG_STARTR( r );
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
      newout = duf_open_file_special( pname, pfilename, &r );
    if ( !newout )
      newout = defout;
    if ( newout )
    {
      if ( *pfile && *pfile != stderr && *pfile != stdout )
        fclose( *pfile );
      *pfile = newout;
    }
  }
  DEBUG_ENDR( r );
}

int
duf_clarify_xcmd_typed( const duf_longval_extended_t * extended, const char *optargg, duf_option_stage_t istage,
                        const duf_longval_extended_table_t * xtable, int noo )
{
  DEBUG_STARTR( r );

  if ( !extended )
    DOR( r, DUF_ERROR_OPTION );

  DUF_TRACE( options, +2, "parsing typed:`%s`;   %s", extended->o.name, duf_error_name( r ) );
  DUF_TRACE( options, +2, "r:%d; xname:%s; noo:%d", r, extended ? extended->o.name : "?", noo );
/*
 * if arg is help option
 * return class id for options to display the help
 * */

  if ( DUF_NOERROR( r ) )
  {
    unsigned doplus = 0;
    char *byteptr = NULL;

    DUF_TRACE( options, +2, "switching by extended->relto=%d", extended->relto );
    switch ( extended->relto )
    {
    case DUF_OFFSET_config:
      byteptr = ( ( ( char * ) duf_config ) + extended->m_offset );
      break;
    case DUF_OFFSET_depthinfo:
      byteptr = ( ( ( char * ) duf_config->pdi ) + extended->m_offset );
      break;
    case DUF_OFFSET_ufilter:
      byteptr = ( ( ( char * ) duf_config->pu ) + extended->m_offset );
      break;
    }

    DUF_TRACE( options, +2, "to check stage; istage:%d", istage );
    if ( DUF_OPTION_CHECK_STAGE( istage, extended, xtable ) ) /* duf_check_stage */
    {
      int nof;

      nof = noo;
      DUF_TRACE( options, +3, "switching by extended->vtype=%d", extended->vtype );
      switch ( extended->vtype )
      {
      case DUF_OPTION_VTYPE_NONE:
        DUF_TRACE( options, +3, "vtype NONE" );
#if 0
        if ( DUF_NOERROR( r ) )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED ); // Why was ???
        /* DUF_TEST_R( r ); */
#endif
        break;
      case DUF_OPTION_VTYPE_NOOP:
        DUF_TRACE( options, +3, "vtype NOOP" );
        break;
      case DUF_OPTION_VTYPE_UPLUS: /* stage (?) not SETUP *//* --trace-...; --debug; --verbose */
        DUF_TRACE( options, +3, "vtype UPLUS" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
          doplus = 1;
      case DUF_OPTION_VTYPE_NUM: /* stage SETUP *//* --max-...; --min-...; --output-level; --use-format; etc. (misc) */
        DUF_TRACE( options, 0, "vtype NUM --%s='%s'", extended->o.name, optargg ? optargg : "" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_NUMOPT( noo, r, unsigned, doplus, duf_strtol_suff );

        DUF_TRACE( options, 0, "(%d) vtype NUM --%s='%s'", r, extended->o.name, optargg ? optargg : "" );
        break;
      case DUF_OPTION_VTYPE_NUML: /* stage SETUP *//* not used ?! */
        DUF_TRACE( options, +3, "vtype NL" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_NUMOPT( noo, r, unsigned long, 0, duf_strtol_suff );

        break;
      case DUF_OPTION_VTYPE_NUMLL: /* stage SETUP *//* not used ?! */
        DUF_TRACE( options, +3, "vtype NLL" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_NUMOPT( noo, r, unsigned long long, 0, duf_strtoll_suff );

        break;
      case DUF_OPTION_VTYPE_MIN: /* stage SETUP */
        DUF_TRACE( options, +3, "vtype MIN" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MOPT( noo, r, duf_limits_t, min, duf_strtol_suff );
        break;
      case DUF_OPTION_VTYPE_MAX: /* stage SETUP */
        DUF_TRACE( options, +3, "vtype MAX" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MOPT( noo, r, duf_limits_t, max, duf_strtol_suff );
        break;
      case DUF_OPTION_VTYPE_MINMAX: /* stage SETUP */
        DUF_TRACE( options, +3, "vtype MINMAX" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MINMAXOPT( noo, r, duf_limits_t, duf_strtol_suff );
        break;

      case DUF_OPTION_VTYPE_MINLL: /* stage SETUP */
        DUF_TRACE( options, +3, "vtype MINLL" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MOPT( noo, r, duf_limitsll_t, min, duf_strtol_suff );
        break;
      case DUF_OPTION_VTYPE_MAXLL: /* stage SETUP */
        DUF_TRACE( options, +3, "vtype MAXLL" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MOPT( noo, r, duf_limitsll_t, max, duf_strtoll_suff );
        break;
      case DUF_OPTION_VTYPE_MINMAXLL: /* stage SETUP */
        DUF_TRACE( options, +3, "vtype MINMAXLL" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MINMAXOPT( noo, r, duf_limitsll_t, duf_strtoll_suff );
        break;

      case DUF_OPTION_VTYPE_NOFLAG: /* stage SETUP */
        DUF_TRACE( options, +3, "vtype NOFLAG" );
        nof = !nof;
      case DUF_OPTION_VTYPE_FLAG: /* stage SETUP *//*  unsigned set of flags */
        DUF_TRACE( options, +3, "vtype FLAG %x", extended->afl.bit );
        {
          if ( extended->invert )
            nof = !nof;
          if ( DUF_NOERROR( r ) )
          {
            unsigned *pi;

            pi = ( unsigned * ) byteptr;
            if ( nof )
              ( *pi ) &= ~extended->afl.bit;
            else
              ( *pi ) |= extended->afl.bit;
            DUF_TEST_R( r );
          }
        }
        break;
#if 0
      case DUF_OPTION_VTYPE_PFLAG:
        DUF_TRACE( options, +3, "vtype PFLAG" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
        {
          unsigned *pi;

          pi = ( unsigned * ) byteptr;
          DUF_PRINTF( 0, "%s is %s", extended->o.name, ( ( *pi ) & extended->afl.bit ) ? "ON" : "OFF" );
          /* DUF_TEST_R( r ); */
        }
        break;
#endif
      case DUF_OPTION_VTYPE_NOSFLAG: /* stage SETUP */
        DUF_TRACE( options, +3, "vtype NOFLAG" );
        nof = !nof;
      case DUF_OPTION_VTYPE_SFLAG: /* stage SETUP *//*  unsigned short set of flags */
        DUF_TRACE( options, +3, "vtype SFLAG" );
        if ( DUF_NOERROR( r ) )
        {
          unsigned short *pis;

          pis = ( unsigned short * ) byteptr;
          if ( noo )
            ( *pis ) &= ~extended->afl.sbit;
          else
            ( *pis ) |= extended->afl.sbit;
          DUF_TEST_R( r );
        }
        break;
#if 0
      case DUF_OPTION_VTYPE_PSFLAG:
        DUF_TRACE( options, +3, "vtype PSFLAG" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
        {
          unsigned *pi;

          pi = ( unsigned * ) byteptr;
          DUF_PRINTF( 0, "(s) %s is %s", extended->o.name, ( ( *pi ) & extended->afl.sbit ) ? "ON" : "OFF" );
          /* DUF_TEST_R( r ); */
        }
        break;
#endif
      case DUF_OPTION_VTYPE_XCHR: /* stage SETUP */
        DUF_TRACE( options, +0, "@@vtype XCHR for %s='%s'", extended->o.name, optargg ? optargg : "" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
        {
          char *pchr;

          pchr = ( char * ) byteptr;
          if ( optargg )
          {
            unsigned cc = 0;

            sscanf( optargg, "%x", &cc );
            *pchr = ( char ) cc;
            DUF_TRACE( options, +0, "char set(%x):'%c'/'%c' @%p/%p", cc, cc, duf_config->cli.option_delimiter, &duf_config->cli.option_delimiter,
                       pchr );
          }
        }
        break;
      case DUF_OPTION_VTYPE_STR: /* stage SETUP */
        /* FIXME DUF_OPTION_VTYPE_PSTR vs. DUF_OPTION_VTYPE_STR */
        DUF_TRACE( options, +0, "@@vtype STR for %s='%s'", extended->o.name, optargg ? optargg : "" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
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
            DUF_TRACE( options, +2, "string set:%s @%p", optargg, pstr );
          }
        }
        break;
#if 0
      case DUF_OPTION_VTYPE_PSTR:
        /* FIXME DUF_OPTION_VTYPE_PSTR vs. DUF_OPTION_VTYPE_STR */
        DUF_TRACE( options, +3, "vtype PSTR" );


        if ( DUF_NOERROR( r ) )
        {
          char **pstr;

          pstr = ( char ** ) byteptr;
          if ( noo )
          {
            if ( pstr && *pstr )
              mas_free( *pstr );
            *pstr = NULL;
          }
          else if ( optargg && *optargg )
          {
            if ( pstr && *pstr )
              mas_free( *pstr );
            *pstr = mas_strdup( optargg );
            DUF_TRACE( options, 0, "pstring set:%s", optargg );
          }
          else if ( extended->m_hasoff == 1 ) /* if  extended->m_hasoff == 1, then mcfg_offset is offset */
          {
            DUF_PRINTF( 0, "%s", ( *pstr ) );
          }
        }
        break;
#endif
      case DUF_OPTION_VTYPE_ARGV: /* stage SETUP */
        /* case DUF_OPTION_VTYPE_PAA: */
        DUF_TRACE( options, +3, "vtype PAA" );

        if ( DUF_NOERROR( r ) )
        {
          duf_argvc_t *parg;

          parg = ( duf_argvc_t * ) byteptr;
          if ( noo )
          {
            if ( parg )
              mas_argv_delete( parg->argc, parg->argv );
          }
          else if ( optargg && *optargg )
          {
            parg->argc = mas_add_argv_arg( parg->argc, &parg->argv, optargg );
          }
          else if ( extended->m_hasoff == 1 ) /* if  extended->m_hasoff == 1, then mcfg_offset is offset */
          {
            char *str = mas_argv_string( parg->argc, parg->argv, 0 );

            DUF_PRINTF( 0, "%s", ( str ) );
            mas_free( str );
          }
        }
        break;
      case DUF_OPTION_VTYPE_TDB: /* stage SETUP */
        DUF_TRACE( options, +3, "vtype TDB" );

        if ( DUF_NOERROR( r ) )
        {
          if ( noo )
          {
#if 0
            duf_tmpdb_delete( extended->o.name, optargg );
#endif
          }
          else if ( optargg && *optargg )
            duf_tmpdb_add( extended->o.val, extended->o.name, optargg );
        }
        break;
        /*
         * DATETIME, MINMAXDATETIME, MINDATETIME, MAXDATETIME
         * */
      case DUF_OPTION_VTYPE_DATETIME: /* stage SETUP */
        DUF_TRACE( options, +3, "vtype DATETIME" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_NUMOPT( noo, r, unsigned long long, 0, duf_strtime2long );

        break;
      case DUF_OPTION_VTYPE_MINMAXDATETIME: /* stage SETUP */
        DUF_TRACE( options, +3, "vtype MINMAXDATETIME" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MINMAXOPT( noo, r, duf_limitsll_t, duf_strtime2long );
        break;
      case DUF_OPTION_VTYPE_MINDATETIME: /* stage SETUP */
        DUF_TRACE( options, +3, "vtype MINDATETIME" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MOPT( noo, r, duf_limitsll_t, min, duf_strtime2long );
        break;
      case DUF_OPTION_VTYPE_MAXDATETIME: /* stage SETUP */
        DUF_TRACE( options, +3, "vtype MAXDATETIME" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MOPT( noo, r, duf_limitsll_t, max, duf_strtime2long );
        break;
        /*
         * _CALL
         * */
      case DUF_OPTION_VTYPE_VV_CALL: /* stage Not? SETUP *//* call with nothing (VV:void-void) */
        DUF_TRACE( options, +3, "vtype VV_CALL" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
        {
          if ( extended->call.fdesc.vv.func )
            ( extended->call.fdesc.vv.func ) (  );
        }
        break;
      case DUF_OPTION_VTYPE_VIA_CALL: /* stage Not? SETUP *//* call with numeric (int) arg from table (VIA:void-int-arg) */
        DUF_TRACE( options, +3, "vtype VI_CALL" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
        {
          if ( extended->call.fdesc.via.func )
            ( extended->call.fdesc.via.func ) ( extended->call.fdesc.via.arg );
          else
            DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NO_FUNC );
        }
        break;
      case DUF_OPTION_VTYPE_VSA_CALL: /* stage Not? SETUP *//* call with constant string arg from table (VSA:void-string-arg) */
        DUF_TRACE( options, +3, "vtype VS_CALL" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
        {
          if ( extended->call.fdesc.vsa.func )
            ( extended->call.fdesc.vsa.func ) ( extended->call.fdesc.vsa.arg );
          else
            DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NO_FUNC );
        }
        break;
      case DUF_OPTION_VTYPE_A_CALL: /* stage Not? SETUP *//* call with carg[cv] (A:argv) */
        DUF_TRACE( options, +3, "vtype A_CALL" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
        {
          assert( duf_config->carg.argv );
          if ( extended->call.fdesc.a.func )
            ( extended->call.fdesc.a.func ) ( duf_config->carg.argc, duf_config->carg.argv );
        }
        break;
      case DUF_OPTION_VTYPE_AA_CALL: /* stage Not? SETUP *//* call with carg (AA:argv-argv) */
        DUF_TRACE( options, +3, "vtype AA_CALL" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
        {
          assert( duf_config->carg.argc );
          assert( duf_config->carg.argv );
          if ( extended->call.fdesc.aa.func )
            ( extended->call.fdesc.aa.func ) ( duf_config->carg );
        }
        break;

      case DUF_OPTION_VTYPE_N_CALL: /* stage Not? SETUP *//* call with numeric optarg (N:numeric) */
        DUF_TRACE( options, +3, "vtype N_CALL" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
        {
          if ( extended->call.fdesc.n.func )
            ( extended->call.fdesc.n.func ) ( duf_strtol_suff( optargg, &r ) );
        }
        break;
      case DUF_OPTION_VTYPE_S_CALL: /* stage Not? SETUP *//* call with string optarg (S: string) */
        DUF_TRACE( options, +3, "vtype S_CALL" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
        {
          if ( extended->call.fdesc.s.func )
            ( extended->call.fdesc.s.func ) ( optargg );
        }
        break;
      case DUF_OPTION_VTYPE_TN_CALL: /* stage Not? SETUP *//* call with targ[cv] + numeric optarg (TN: targ and numeric) */
        DUF_TRACE( options, +3, "vtype TN_CALL" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
        {
          if ( extended->call.fdesc.tn.func )
            ( extended->call.fdesc.tn.func ) ( &duf_config->targ.argc, &duf_config->targ.argv, duf_strtol_suff( optargg, &r ) );
        }
        break;
      case DUF_OPTION_VTYPE_TS_CALL: /* stage Not? SETUP *//* call with targ[cv] + string optarg (TS: targ and string) */
        DUF_TRACE( options, +3, "vtype TS_CALL" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
        {
          if ( extended->call.fdesc.ts.func )
            ( extended->call.fdesc.ts.func ) ( &duf_config->targ.argc, &duf_config->targ.argv, optargg );
        }
        break;
        /* 
         * FILE
         * */
      case DUF_OPTION_VTYPE_FILE:
        DUF_TRACE( options, +3, "vtype FILE" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
        {
          DUF_TRACE( io, 0, "DUF_OUTPUTFILE (%s)", extended->o.name );
          DUF_OUTPUTFILE( noo, r, duf_config_output_t, stderr );
          /* {                                                                                 */
          /*   char start_time[128] = "??";                                                    */
          /*                                                                                   */
          /*   duf_strflocaltime( start_time, sizeof( start_time ), "%Y%m%d.%H:%M:%S", NULL ); */
          /*   PF( "%s", start_time );                                                         */
          /* }                                                                                 */

          /* DOR(r, DUF_ERROR_OPTION_NOT_PARSED); */
        }
        break;
      }
      DUF_TRACE( options, 0, "@@(%s)         this (%2d:%2d:%2d) stage; vtype=%2d; xname:%-20s; arg:'%s'; no:%d", duf_error_name( r ),
                 istage, extended->stage.min, extended->stage.max, extended->vtype, extended ? extended->o.name : "?", optargg ? optargg : "", noo );
    }
    else
    {
      DUF_TRACE( options, 0, "@(%s) NOT for this (%2d:%2d:%2d) stage; vtype=%2d; xname:%-20s; arg:'%s'; no:%d", duf_error_name( r ), istage,
                 extended->stage.min, extended->stage.max, extended->vtype, extended ? extended->o.name : "?", optargg ? optargg : "", noo );
      /* DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_FOUND ); */
    }
  }
  DEBUG_ENDR( r );
  /* DEBUG_ENDR_YES( r, DUF_ERROR_OPTION_NOT_FOUND ); */
}
