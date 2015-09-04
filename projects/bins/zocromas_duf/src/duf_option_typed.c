#include <assert.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_utils.h"

#include "duf_config_ref.h"

#include "duf_pdi.h"
#include "duf_levinfo_ref.h"

#include "duf_option_defs.h"
#include "duf_option_find.h"
#include "duf_option_tmpdb.h"

/* ###################################################################### */
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
    DOR( rpr, DUF_ERROR_OPTION_VALUE );
  }
  else
  {
    newfile = fopen( pname, mode );
    if ( !newfile )
    {
      DUF_SHOW_ERROR( "can't open file %s\n", pname );
      DOR( rpr, DUF_ERROR_OPTION_VALUE );
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
duf_clarify_opt_typed( const duf_longval_extended_t * extended, const char *optargg, duf_option_stage_t istage,
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
#define DUF_NUMOPT( _no, _rt, _typ, _dopls, _conv) \
      if (_rt>=0) \
      { \
	if ( !_no ) \
	{ \
	  int __rl = 0; \
	  _typ *p; \
	  p = ( _typ * ) byteptr; /* byteptr only valid if extended->m_hasoff == 1 */ \
	  if ( extended->m_hasoff == 1 && optargg ) /* if  extended->m_hasoff == 1, then mcfg_offset is offset */ \
	  { \
	    _typ __v; \
	    if (*optargg=='?') \
	    { DUF_PRINTF(0, "%s=%lld", extended->o.name, (unsigned long long)( *p )); } \
	    else \
	    { \
	      __v = _conv( optargg, &__rl ); \
	      if ( __rl < 0 ) \
	      { DOR(_rt, DUF_ERROR_OPTION_VALUE); } \
	      else \
	      { \
		if ( _dopls && ( *optargg=='-' || *optargg=='+' ) ) \
		  ( *p ) += __v; \
		else \
		  ( *p ) = __v; \
		DUF_TRACE( options, 7, "%s : number set:%llu", extended->o.name, (unsigned long long) __v ); \
	      } \
	    } \
	  } \
	  else if ( _dopls ) \
	  { \
	    ( *p )++; \
	  } \
	} \
	else \
	{ \
	  DOR(_rt, DUF_ERROR_OPTION); \
	} \
      }
#if 0
#  define DUF_PNUMOPT( _no, _rt, _typ ) \
      if (_rt>=0) \
      { \
	if ( !_no ) \
	{ \
	  _typ *p; \
	  p = ( _typ * ) byteptr; /* byteptr only valid if extended->m_hasoff == 1 */ \
	  if ( extended->m_hasoff == 1 ) /* if  extended->m_hasoff == 1, then mcfg_offset is offset */ \
	  { \
	    DUF_PRINTF(0, "??? %lld", (unsigned long long)( *p )); \
	  } \
	} \
	else \
	{ \
	  DOR(_rt, DUF_ERROR_OPTION); \
	} \
      }
#  define DUF_QPNUMOPT( _no, _rt, _typ, _conv ) \
      if (_rt>=0) \
      { \
	if ( !_no ) \
	{ \
	  if ( optargg && *optargg ) \
	  { \
	    DUF_NUMOPT(_no, _rt, _typ,0,_conv); \
	    if (_rt==DUF_ERROR_OPTION_VALUE) { DUF_CLEAR_ERROR(_rt, DUF_ERROR_OPTION_VALUE); DUF_PNUMOPT(_no, _rt, _typ); } \
	  } \
	  else \
	  {  DUF_PNUMOPT(_no, _rt, _typ); } \
	} \
	else \
	{ \
	  DOR(_rt, DUF_ERROR_OPTION); \
	} \
      }
#endif
#define DUF_MINMAXOPT(_no, _rt, _typ,_conv) \
      if (_rt>=0) \
      { \
	if ( !_no ) \
	{ \
	  int __rl = 0; \
	  _typ *mm; \
	  mm= ( _typ * ) byteptr; /* byteptr only valid if extended->m_hasoff == 1 */ \
	  if ( extended->m_hasoff == 1 ) /* if  extended->m_hasoff == 1, then mcfg_offset is offset */ \
	  { \
	    if( optargg ) \
	    { \
	      _typ __v; \
	      char c=0; \
	      mm->flag = 1; \
	      if ( *optargg == '+' ) \
		c = *optargg++; \
	      else if ( *optargg == '-' ) \
		c = *optargg++; \
	      __v.min = _conv( optargg, &__rl ); \
	      if ( __rl < 0 ) \
	      { DOR(_rt, DUF_ERROR_OPTION_VALUE); } \
	      else \
	      { \
		if ( c=='+' ) \
		{ \
		  mm->min = __v.min; \
		  mm->max = 0; \
		} \
		else if ( c=='-' ) \
		{ \
		  mm->min = 0; \
		  mm->max = __v.min; \
		} \
		else \
		{ \
		  mm->min = mm->max = __v.min; \
		} \
		DUF_TRACE( options, 1, "%s: min/max set:%llu", extended->o.name, (unsigned long long) __v.min ); \
	      } \
	    } \
	    else \
	    { \
	      mm->flag = 1; \
	      mm->min = extended->call.value.u; \
	      mm->max = extended->call.value2.u; \
	      DUF_TRACE( options, 1, "%s: min/max set:%llu:%llu", extended->o.name, \
		  (unsigned long long) extended->call.value.u,  (unsigned long long) extended->call.value2.u ); \
	    } \
	  } \
	} \
	else \
	{ \
	  DOR(_rt, DUF_ERROR_OPTION); \
	} \
      }
#define DUF_MOPT(_no, _rt,_typ,_mix,_conv) \
      if(_rt>=0) \
      { \
	if ( !_no ) \
	{ \
	  int __rl = 0; \
	  _typ *mm; \
	  mm= ( _typ * ) byteptr; /* byteptr only valid if extended->m_hasoff == 1 */ \
	  if ( extended->m_hasoff == 1 && optargg ) /* if  extended->m_hasoff == 1, then mcfg_offset is offset */ \
	  { \
	    mm->flag = 1; \
	    mm->_mix = _conv( optargg, &__rl ); \
	    if ( __rl < 0 ) \
	    {  DOR(_rt, DUF_ERROR_OPTION_VALUE); } \
	    else \
	    { DUF_TRACE( options, 1, #_mix "%s: set:%llu", extended->o.name, (unsigned long long) mm->_mix ); } \
	  } \
	} \
	else \
	{ \
	  DOR(_rt, DUF_ERROR_OPTION); \
	} \
      }

#define DUF_OUTPUTFILE_A(_no, _rt, _typ, _defoptarg, _defout) \
      if(_rt>=0) \
      { \
	if( !_no ) \
	{ \
	  int __rl = 0; \
	  const char *s; \
	  _typ *mm; \
	  s=optargg ? optargg : _defoptarg; \
	  mm= ( _typ * ) byteptr; /* byteptr only valid if extended->m_hasoff == 1 */ \
	  if ( extended->m_hasoff == 1 /* && (s || extended->call.value.u */ ) /* if  extended->m_hasoff == 1, then mcfg_offset is offset */ \
	  { \
	    DOR(__rl, duf_set_file_special( s, &mm->file, &mm->out, _defout, extended->call.value.u )); \
	    if ( __rl < 0 ) \
	    {  DOR(_rt, DUF_ERROR_OPTION_VALUE); } \
	  } \
	} \
	else \
	{ \
	  DOR(_rt, DUF_ERROR_OPTION); \
	} \
      }
#define DUF_OUTPUTFILE(_no, _rt, _typ, _defout) DUF_OUTPUTFILE_A(_no, _rt, _typ, NULL, _defout)

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
      case DUF_OPTION_VTYPE_UPLUS:
        DUF_TRACE( options, +3, "vtype UPLUS" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
          doplus = 1;
      case DUF_OPTION_VTYPE_NUM:
        DUF_TRACE( options, +3, "vtype NUM" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_NUMOPT( noo, r, unsigned, doplus, duf_strtol_suff );

        break;
#if 0
      case DUF_OPTION_VTYPE_PNUM:
        DUF_TRACE( options, +3, "vtype PNUM" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
        {
          DUF_QPNUMOPT( noo, r, unsigned, duf_strtol_suff );
        }
        break;
#endif
      case DUF_OPTION_VTYPE_NL:
        DUF_TRACE( options, +3, "vtype NL" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_NUMOPT( noo, r, unsigned long, 0, duf_strtol_suff );

        break;
#if 0
      case DUF_OPTION_VTYPE_PNL:
        DUF_TRACE( options, +3, "vtype PNL" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_QPNUMOPT( noo, r, unsigned long, duf_strtol_suff );

        break;
#endif
      case DUF_OPTION_VTYPE_NLL:
        DUF_TRACE( options, +3, "vtype NLL" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
        {
          DUF_NUMOPT( noo, r, unsigned long long, 0, duf_strtoll_suff );
        }
        break;
#if 0
      case DUF_OPTION_VTYPE_PNLL:
        DUF_TRACE( options, +3, "vtype PNLL" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_QPNUMOPT( noo, r, unsigned long long, duf_strtoll_suff );

        break;
#endif
      case DUF_OPTION_VTYPE_MIN:
        DUF_TRACE( options, +3, "vtype MIN" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MOPT( noo, r, duf_limits_t, min, duf_strtol_suff );
        break;
      case DUF_OPTION_VTYPE_MAX:
        DUF_TRACE( options, +3, "vtype MAX" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MOPT( noo, r, duf_limits_t, max, duf_strtol_suff );
        break;
      case DUF_OPTION_VTYPE_MINMAX:
        DUF_TRACE( options, +3, "vtype MINMAX" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MINMAXOPT( noo, r, duf_limits_t, duf_strtol_suff );
        break;
      case DUF_OPTION_VTYPE_MINLL:
        DUF_TRACE( options, +3, "vtype MINLL" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MOPT( noo, r, duf_limitsll_t, min, duf_strtol_suff );
        break;
      case DUF_OPTION_VTYPE_MAXLL:
        DUF_TRACE( options, +3, "vtype MAXLL" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MOPT( noo, r, duf_limitsll_t, max, duf_strtoll_suff );
        break;
      case DUF_OPTION_VTYPE_MINMAXLL:
        DUF_TRACE( options, +3, "vtype MINMAXLL" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MINMAXOPT( noo, r, duf_limitsll_t, duf_strtoll_suff );
        break;
        /* case DUF_OPTION_CLASS_DEBUG: */
        /* DUF_PRINTF( 0, "------------ %lu", extended->mcfg_offset ); */
        /* break; */
      case DUF_OPTION_VTYPE_NOFLAG:
        DUF_TRACE( options, +3, "vtype NOFLAG" );
        nof = !nof;
      case DUF_OPTION_VTYPE_FLAG:
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
      case DUF_OPTION_VTYPE_SFLAG:
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
      case DUF_OPTION_VTYPE_NOSFLAG:
        DUF_TRACE( options, +3, "vtype NOSFLAG" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
        {
          unsigned *pi;

          pi = ( unsigned * ) byteptr;
          ( *pi ) &= ~extended->afl.sbit;
          DUF_TEST_R( r );
        }
        break;
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
      case DUF_OPTION_VTYPE_STR:
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
      case DUF_OPTION_VTYPE_ARGV:
      case DUF_OPTION_VTYPE_PAA:
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
      case DUF_OPTION_VTYPE_TDB:
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
          {
            duf_tmpdb_add( extended->o.val, extended->o.name, optargg );
          }
#if 0
          else if ( extended->m_hasoff == 1 ) /* if  extended->m_hasoff == 1, then mcfg_offset is offset */
          {
            char *str = duf_tmpdb_select( extended->o.name );

            DUF_PRINTF( 0, "%s", ( str ) );
            mas_free( str );
          }
#endif
        }
        break;
#if 0
      case DUF_OPTION_VTYPE_PDISTR:
        DUF_TRACE( options, +3, "vtype PDISTR" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
        {
          if ( optargg && *optargg )
          {
            duf_depthinfo_t *pdi = duf_config->pdi;

            DOR( r,
                 duf_pdi_reinit_anypath( *( ( duf_depthinfo_t ** ) byteptr ), optargg,
                                         NULL /* const char *node_selector2 *//* , duf_config->pu,  recursive */  ) );
            DUF_TRACE( path, 0, "@ levinfo_path: %s", duf_levinfo_path( *( duf_depthinfo_t ** ) byteptr ) );
          }
          else
            DUF_PRINTF( 0, "%s", duf_levinfo_path( *( ( duf_depthinfo_t ** ) byteptr ) ) );
        }
        break;
      case DUF_OPTION_VTYPE_PDINUM:
        DUF_TRACE( options, +3, "vtype PDINUM" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) && extended->m_hasoff == 1 )
        {
          /* if ( optargg && *optargg )                                                                                              */
          /*   DOR( r, duf_pdi_reinit_anypath( *( ( duf_depthinfo_t ** ) byteptr ), optargg, &duf_config->u, 0 (* recursive *)  ) ); */
          /* else                                                                                                                    */
          DUF_PRINTF( 0, "%d", *( ( int * ) byteptr ) );
        }
        break;
      case DUF_OPTION_VTYPE_PDISCCB:
        DUF_TRACE( options, +3, "vtype PDISCCB" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
        {
          duf_sccb_handle_t csccbh = {
            .pdi = duf_config->pdi,
            .sccb = extended->call.fdesc.hi.sccb,
          };
          duf_sccb_handle_t *sccbh = &csccbh;

/* #include "duf_sccb_way.h" */
          DOR( r, duf_sccbh_eval_pdi_and_summary( sccbh ) );
        }
        break;
/* DOR( r, duf_make_sccb( sccb ) );                            */
/* DOR( r, duf_sccbh_each_path( duf_sccb_handle_t * sccbh ) ); */
#endif
      case DUF_OPTION_VTYPE_DATETIME:
        DUF_TRACE( options, +3, "vtype DATETIME" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_NUMOPT( noo, r, unsigned long long, 0, duf_strtime2long );

        break;
      case DUF_OPTION_VTYPE_MINMAXDATETIME:
        DUF_TRACE( options, +3, "vtype MINMAXDATETIME" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MINMAXOPT( noo, r, duf_limitsll_t, duf_strtime2long );
        break;
      case DUF_OPTION_VTYPE_MINDATETIME:
        DUF_TRACE( options, +3, "vtype MINDATETIME" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MOPT( noo, r, duf_limitsll_t, min, duf_strtime2long );
        break;
      case DUF_OPTION_VTYPE_MAXDATETIME:
        DUF_TRACE( options, +3, "vtype MAXDATETIME" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        DUF_MOPT( noo, r, duf_limitsll_t, max, duf_strtime2long );
        break;
      case DUF_OPTION_VTYPE_VIA_CALL:
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
      case DUF_OPTION_VTYPE_VSA_CALL:
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
      case DUF_OPTION_VTYPE_A_CALL:
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
      case DUF_OPTION_VTYPE_AA_CALL:
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

      case DUF_OPTION_VTYPE_VV_CALL:
        DUF_TRACE( options, +3, "vtype VV_CALL" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
        {
          if ( extended->call.fdesc.vv.func )
            ( extended->call.fdesc.vv.func ) (  );
        }
        break;
      case DUF_OPTION_VTYPE_N_CALL:
        DUF_TRACE( options, +3, "vtype N_CALL" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
        {
          if ( extended->call.fdesc.n.func )
            ( extended->call.fdesc.n.func ) ( duf_strtol_suff( optargg, &r ) );
        }
        break;
      case DUF_OPTION_VTYPE_TN_CALL:
        DUF_TRACE( options, +3, "vtype TN_CALL" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
        {
          if ( extended->call.fdesc.tn.func )
            ( extended->call.fdesc.tn.func ) ( &duf_config->targ.argc, &duf_config->targ.argv, duf_strtol_suff( optargg, &r ) );
        }
        break;
      case DUF_OPTION_VTYPE_TS_CALL:
        DUF_TRACE( options, +3, "vtype TS_CALL" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
        {
          if ( extended->call.fdesc.ts.func )
            ( extended->call.fdesc.ts.func ) ( &duf_config->targ.argc, &duf_config->targ.argv, optargg );
        }
        break;
      case DUF_OPTION_VTYPE_S_CALL:
        DUF_TRACE( options, +3, "vtype S_CALL" );
        if ( noo )
          DOR( r, DUF_ERROR_OPTION_NOT_PARSED );
        if ( DUF_NOERROR( r ) )
        {
          if ( extended->call.fdesc.s.func )
            ( extended->call.fdesc.s.func ) ( optargg );
        }
        break;
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
    }
    else
    {
      DUF_TRACE( options, 0, "@@NOT switching by extended->vtype=%d; istage=%d (%d:%d); stage.flag:%d", extended->vtype, istage, extended->stage.min,
                 extended->stage.max, extended->stage.flag );
      /* DUF_MAKE_ERROR( r, DUF_ERROR_OPTION_NOT_FOUND ); */
    }
  }
  DEBUG_ENDR( r );
  /* DEBUG_ENDR_YES( r, DUF_ERROR_OPTION_NOT_FOUND ); */
}
