#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"
#include "duf_utils.h"
#include "duf_utils_path.h"

#include "duf_config_ref.h"

#include "duf_pdi.h"
#include "duf_sccb.h"
#include "duf_levinfo.h"
#include "duf_levinfo_ref.h"

#include "duf_option_find.h"
#include "duf_option_descr.h"
#include "duf_option_extended.h"

/* ###################################################################### */
#include "duf_option.h"
/* ###################################################################### */


static long
duf_strtol_suff( const char *s, int *pr )
{
  int r = 0;
  long l = 0;
  char *pe = NULL;

  l = strtol( s, &pe, 10 );
  if ( pe )
  {
    switch ( *pe )
    {
    case 'G':
      l *= 1024 * 1024 * 1024;
      break;
    case 'M':
      l *= 1024 * 1024;
      break;
    case 'k':
      l *= 1024;
      break;
    case 'w':
      l *= 2;
      break;
    case 'c':
    case '\0':
      break;
    case 'b':
      l *= 512;
      break;
    default:
      r = DUF_ERROR_OPTION_VALUE;
      l = 0;
      break;
    }
  }
  else
  {
    r = DUF_ERROR_OPTION_VALUE;
    l = 0;
  }
  if ( pr )
    *pr = r;
  return l;
}

static long long
duf_strtoll_suff( const char *s, int *pr )
{
  int r = 0;
  long l = 0;
  char *pe = NULL;

  l = strtoll( s, &pe, 10 );
  if ( pe )
  {
    switch ( *pe )
    {
    case 'G':
      l *= 1024 * 1024 * 1024;
      break;
    case 'M':
      l *= 1024 * 1024;
      break;
    case 'k':
      l *= 1024;
      break;
    case 'w':
      l *= 2;
      break;
    case 'c':
    case '\0':
      break;
    case 'b':
      l *= 512;
      break;
    default:
      r = DUF_ERROR_OPTION_VALUE;
      l = 0;
      break;
    }
  }
  else
  {
    r = DUF_ERROR_OPTION_VALUE;
    l = 0;
  }
  if ( pr )
    *pr = r;
  return l;
}

DUF_UNUSED static int
duf_limits( const char *s, unsigned *pmin, unsigned *pmax )
{
  DEBUG_STARTR( r );

  if ( s )
  {
    long n;
    char c = 0;

    if ( *s == '+' )
      c = *s++;
    else if ( *s == '-' )
      c = *s++;
    n = duf_strtol_suff( s, &r );
    if ( r >= 0 )
    {
      if ( c == '+' )
      {
        if ( pmin )
          *pmin = n + 1;
      }
      else if ( c == '-' )
      {
        if ( pmax )
          *pmax = n - 1;
      }
      else
      {
        if ( pmin )
          *pmin = n;
        if ( pmax )
          *pmax = n;
      }
    }
    else
      r = DUF_ERROR_OPTION_VALUE;
    DUF_ERROR( "[%c] %d - %d", c, pmin ? *pmin : 0, pmax ? *pmax : 0 );
  }
  DEBUG_ENDR( r );
}

DUF_UNUSED static int
duf_limitsll( const char *s, unsigned long long *pmin, unsigned long long *pmax )
{
  DEBUG_STARTR( r );

  if ( s )
  {
    long n;
    char c = 0;

    if ( *s == '+' )
      c = *s++;
    else if ( *s == '-' )
      c = *s++;
    n = duf_strtol_suff( s, &r );
    if ( r >= 0 )
    {
      if ( c == '+' )
      {
        if ( pmin )
          *pmin = n + 1;
      }
      else if ( c == '-' )
      {
        if ( pmax )
          *pmax = n - 1;
      }
      else
      {
        if ( pmin )
          *pmin = n;
        if ( pmax )
          *pmax = n;
      }
    }
    else
      r = DUF_ERROR_OPTION_VALUE;
    DUF_ERROR( "[%c] %lld - %lld", c, pmin ? *pmin : 0, pmax ? *pmax : 0 );
  }
  DEBUG_ENDR( r );
}

static FILE *
duf_open_file_special( const char *pname, char **popenedname, int *pr )
{
  int r = 0;
  FILE *newfile;
  int overw = 0;
  const char *mode = "w";
  struct stat st;

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
    DUF_ERROR( "can't open file %s for writing file exists %llu / %llu chr:%d\n", pname, ( unsigned long long ) st.st_dev,
               ( unsigned long long ) st.st_rdev, S_ISCHR( st.st_mode ) );
    r = DUF_ERROR_OPTION_VALUE;
  }
  else
  {
    newfile = fopen( pname, mode );
    if ( !newfile )
    {
      DUF_ERROR( "can't open file %s\n", pname );
      r = DUF_ERROR_OPTION_VALUE;
    }
  }
  if ( pr )
    *pr = r;
  return newfile;
}

static int
duf_set_file_special( const char *pname, char **pfilename, FILE ** pfile, FILE * newout, int handleid )
{
  DEBUG_STARTR( r );

  if ( pfile )
  {
    if ( !newout && handleid )
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
    if ( pfilename )
    {
      if ( *pfilename )
        mas_free( *pfilename );
      *pfilename = NULL;
      if ( pname && *pname )
        newout = duf_open_file_special( pname, pfilename, &r );
    }
    if ( newout )
    {
      if ( *pfile && *pfile != stderr && *pfile != stdout )
        fclose( *pfile );
      *pfile = newout;
    }
  }
  DEBUG_ENDR( r );
}

static int
duf_parse_option_long_x_wrap( const duf_longval_extended_t * extended, const char *optargg, int stage )
{
  DEBUG_STARTR( r );

  if ( extended )
  {
/* 
 * duf_parse_option_long return 
 *            =0  for other option
 *   errorcode<0  for error
 * */
    DOR( r, duf_parse_option_long_typed( extended, optargg, stage ) );
    DUF_TRACE( explain, 0, "parsed typed:`%s`   %s", extended->o.name, duf_error_name( r ) );
    if ( r == DUF_ERROR_OPTION_NOT_PARSED )
      DOZR( r, duf_parse_option_long_old( extended, optargg, stage ) );
    DUF_TRACE( explain, 2, "cli options r: %d", r );
    DUF_TRACE( explain, 1, "parsed CLI option:  %s  %s", duf_option_description_x_tmp( -1, extended, NULL ), duf_error_name( r ) );
  }
  DEBUG_ENDR( r );
}


/*
 * return 
 *               =0  for normal options
 * or errorcode (<0) for error
 * */
int
duf_parse_option( duf_option_code_t codeval, int longindex, const char *optargg, int stage )
{
  DEBUG_STARTR( r );
  const duf_longval_extended_t *extended = NULL;

  assert( ( int ) codeval >= 0 );
  /* short always corresponds long (in my case) - find it */
  if ( longindex < 0 )
  {
    DORN( r, duf_find_codeval_long_std( codeval ) );
    if ( r >= 0 )
    {
      longindex = r;
      r = 0;
    }
  }
  DUF_TRACE( explain, 1, "duf_parse_option codeval: %d (%c) longindex:%d (%s)", codeval, codeval > ' '
             && codeval <= 'z' ? codeval : '?', longindex, duf_error_name( r ) );
  if ( r >= 0 )
  {
    /* if ( longindex < 0 )                                       */
    /*   extended = duf_find_codeval_extended_std( codeval, &r ); */
    /* else                                                       */
    /*   extended = duf_longindex_extended( longindex );          */

    extended = ( longindex < 0 ) ? duf_find_codeval_extended_std( codeval, &r ) : duf_longindex_extended( longindex );

    DUF_TEST_R( r );
  }
  if ( r >= 0 )
  {
    if ( extended )
      r = duf_parse_option_long_x_wrap( extended, optargg, stage );
    else /* switch is useless !... */
      switch ( ( int ) codeval )
      {
      case ':':
        r = DUF_ERROR_OPTION_VALUE;
        DUF_TRACE( explain, 3, "cli options r: %d", r );
        break;
      case '?':
        r = DUF_ERROR_OPTION;
        DUF_TRACE( explain, 3, "cli options r: %d", r );
        break;
      default:
        r = DUF_ERROR_OPTION;
        DUF_TRACE( explain, 3, "cli options r: %d; codeval:%d;", r, codeval );
        break;
      }
  }
  DUF_TRACE( explain, 3, "cli options r: %d; codeval:%d;", r, codeval );
  DEBUG_ENDR( r );
}

unsigned long long
duf_strtime2long( const char *s, int *pr )
{
  int r = 0;
  time_t t = 0;
  char *p = NULL;
  struct tm tm;

  {
    if ( !p )
    {
      memset( &tm, 0, sizeof( tm ) );
      p = strptime( s, "%Y-%m-%d %H:%M:%S", &tm );
    }
    if ( !p )
    {
      memset( &tm, 0, sizeof( tm ) );
      p = strptime( s, "%Y-%m-%d", &tm );
    }
    if ( !p )
    {
      memset( &tm, 0, sizeof( tm ) );
      p = strptime( s, "%Y%m%d.%H%M%S", &tm );
    }
    if ( !p )
    {
      memset( &tm, 0, sizeof( tm ) );
      p = strptime( s, "%Y%m%d.%H%M", &tm );
    }
    if ( !p )
    {
      memset( &tm, 0, sizeof( tm ) );
      p = strptime( s, "%Y%m%d.%H", &tm );
    }
    if ( !p )
    {
      memset( &tm, 0, sizeof( tm ) );
      p = strptime( s, "%Y%m%d", &tm );
    }
    tm.tm_zone = tzname[daylight];
    tm.tm_isdst = daylight;
    if ( p )
      t = timelocal( &tm );
    else
      r = DUF_ERROR_OPTION_VALUE;
  }
  if ( pr )
    *pr = r;
  return ( unsigned long long ) t;
}

int
duf_parse_option_long_typed( const duf_longval_extended_t * extended, const char *optargg, int stage )
{
  DEBUG_STARTR( r );
  duf_option_code_t codeval = DUF_OPTION_NONE;

  if ( extended )
    codeval = extended->o.val;
  else
    r = DUF_ERROR_OPTION;

  DUF_TRACE( explain, 0, "parsing typed:`%s`; cv:%d;   %s", extended->o.name, codeval, duf_error_name( r ) );
/*
 * if arg is help option
 * return class id for options to display the help
 * */

  if ( codeval && r >= 0 )
  {
    unsigned doplus = 0;
    char *byteptr = ( ( ( char * ) duf_config ) + extended->m );

#define DUF_NUMOPT(typ,dopls,conv) \
      { \
        int rl = 0; \
	typ *p; \
	p = ( typ * ) byteptr; /* byteptr only valid if extended->mf == 1 */ \
	if ( extended->mf == 1 && optargg ) /* if  extended->mf == 1, then m is offset */ \
	{ \
	  ( *p ) = conv( optargg, &rl ); \
	  if ( rl < 0 ) \
	    r = DUF_ERROR_OPTION_VALUE; \
	} \
	else if ( dopls ) \
	{ \
	  ( *p )++; \
	} \
      }
#define DUF_MINMAXOPT(typ,conv) \
      { \
        int rl = 0; \
	typ *mm; \
	mm= ( typ * ) byteptr; /* byteptr only valid if extended->mf == 1 */ \
	if ( extended->mf == 1 ) /* if  extended->mf == 1, then m is offset */ \
	{ \
	  if( optargg ) \
	  { \
	    mm->flag = 1; \
	    mm->min = mm->max = conv( optargg, &rl ); \
	    if ( rl < 0 ) \
	      r = DUF_ERROR_OPTION_VALUE; \
	  } \
	  else \
	  { \
	    mm->flag = 1; \
	    mm->min = extended->call.value.u; \
	    mm->max = extended->call.value2.u; \
	  } \
	} \
      }
#define DUF_MOPT(typ,mix,conv) \
      { \
        int rl = 0; \
	typ *mm; \
	mm= ( typ * ) byteptr; /* byteptr only valid if extended->mf == 1 */ \
	if ( extended->mf == 1 && optargg ) /* if  extended->mf == 1, then m is offset */ \
	{ \
	  mm->flag = 1; \
	  mm->mix = conv( optargg, &rl ); \
	  if ( rl < 0 ) \
	    r = DUF_ERROR_OPTION_VALUE; \
	} \
      }

#define DUF_OUTPUTFILE_A(typ, defoptarg) \
      { \
        int rl = 0; \
	const char *s; \
	typ *mm; \
	s=optargg ? optargg : defoptarg; \
	mm= ( typ * ) byteptr; /* byteptr only valid if extended->mf == 1 */ \
	if ( extended->mf == 1 /* && (s || extended->call.value.u */ ) /* if  extended->mf == 1, then m is offset */ \
	{ \
	  duf_set_file_special( s, &mm->file, &mm->out, NULL, extended->call.value.u ); \
	  if ( rl < 0 ) \
	    r = DUF_ERROR_OPTION_VALUE; \
	} \
      }
#define DUF_OUTPUTFILE(typ) DUF_OUTPUTFILE_A(typ, NULL)


    if ( extended->stage == stage )
      switch ( extended->vtype )
      {
      case DUF_OPTION_VTYPE_NONE:
        r = DUF_ERROR_OPTION_NOT_PARSED;
        /* DUF_TEST_R( r ); */
        break;
      case DUF_OPTION_VTYPE_UPLUS:
        doplus = 1;
      case DUF_OPTION_VTYPE_NUM:
        {
          DUF_NUMOPT( unsigned, doplus, duf_strtol_suff );

          DUF_TEST_R( r );
        }
        break;
      case DUF_OPTION_VTYPE_NL:
        {
          DUF_NUMOPT( unsigned long, 0, duf_strtol_suff );

          DUF_TEST_R( r );
        }
        break;
      case DUF_OPTION_VTYPE_NLL:
        {
          DUF_NUMOPT( unsigned long long, 0, duf_strtoll_suff );

          DUF_TEST_R( r );
        }
        break;
      case DUF_OPTION_VTYPE_MIN:
        {
          DUF_MOPT( duf_limits_t, min, duf_strtol_suff );
          DUF_TEST_R( r );
        }
        break;
      case DUF_OPTION_VTYPE_MAX:
        {
          DUF_MOPT( duf_limits_t, max, duf_strtol_suff );
          DUF_TEST_R( r );
        }
        break;
      case DUF_OPTION_VTYPE_MINMAX:
        {
          DUF_MINMAXOPT( duf_limits_t, duf_strtol_suff );
          DUF_TEST_R( r );
        }
        break;
      case DUF_OPTION_VTYPE_MINLL:
        {
          DUF_MOPT( duf_limitsll_t, min, duf_strtol_suff );
          DUF_TEST_R( r );
        }
        break;
      case DUF_OPTION_VTYPE_MAXLL:
        {
          DUF_MOPT( duf_limitsll_t, max, duf_strtoll_suff );
          DUF_TEST_R( r );
        }
        break;
      case DUF_OPTION_VTYPE_MINMAXLL:
        {
          DUF_MINMAXOPT( duf_limitsll_t, duf_strtoll_suff );
          DUF_TEST_R( r );
        }
        break;
        /* case DUF_OPTION_CLASS_DEBUG: */
        /* DUF_PRINTF( 0, "------------ %lu", extended->m ); */
        /* break; */
      case DUF_OPTION_VTYPE_FLAG:
        {
          unsigned *pi;

          pi = ( unsigned * ) byteptr;
          ( *pi ) |= extended->afl.bit;
          DUF_TEST_R( r );
        }
        break;
      case DUF_OPTION_VTYPE_NOFLAG:
        {
          unsigned *pi;

          pi = ( unsigned * ) byteptr;
          ( *pi ) &= ~extended->afl.bit;
          DUF_TEST_R( r );
        }
        break;
      case DUF_OPTION_VTYPE_SFLAG:
        {
          unsigned short *pis;

          pis = ( unsigned short * ) byteptr;
          ( *pis ) |= extended->afl.sbit;
          DUF_TEST_R( r );
        }
        break;
      case DUF_OPTION_VTYPE_STR:
        {
          char **pstr;

          pstr = ( char ** ) byteptr;
          if ( pstr && *pstr )
            mas_free( *pstr );
          *pstr = NULL;
          if ( optargg )
            *pstr = mas_strdup( optargg );
          DUF_TEST_R( r );
        }
        break;
      case DUF_OPTION_VTYPE_PDISTR:
        {
          duf_depthinfo_t **ppdi;

          ppdi = ( duf_depthinfo_t ** ) byteptr;
          {
            duf_depthinfo_t *pdi = NULL;

            if ( ppdi )
              pdi = *ppdi;
            assert( pdi );

            if ( optargg )
            {
              char *path = NULL;
              char *real_path = NULL;

              if ( *optargg == '/' )
                path = mas_strdup( optargg );
              else
              {
                path = mas_strdup( duf_levinfo_path( pdi ) );
                if ( !( optargg[0] == '.' && !optargg[1] ) )
                {
                  path = mas_strcat_x( path, "/" );
                  path = mas_strcat_x( path, optargg );
                }
              }
              real_path = duf_realpath( path, &r );
              if ( r >= 0 )
              {
                duf_pdi_close( pdi );
                duf_pdi_reinit( pdi, real_path, &duf_config->u, 0 /* recursive */ );
              }
              mas_free( path );
              mas_free( real_path );
            }
          }
        }
        DUF_TEST_R( r );
        break;
      case DUF_OPTION_VTYPE_PDISCCB:
        {
          duf_depthinfo_t **ppdi;

          ppdi = ( duf_depthinfo_t ** ) byteptr;
          {
            duf_depthinfo_t *pdi = NULL;

            if ( ppdi )
              pdi = *ppdi;
            assert( pdi );
            T( " duf_config->targc:%d", duf_config->targc );
            T( " duf_config->pdi:%p", duf_config->pdi );
            T( " pdi:%p", pdi );

            assert( pdi->levinfo );
            {
              DUF_UNUSED extern duf_scan_callbacks_t duf_print_dir_callbacks __attribute( ( weak ) );

              T( "trace.scan:%d", duf_config->cli.trace.scan );

              T( "SCCB %s : %p - %p", duf_levinfo_path( pdi ), extended->call.fdesc.hi.sccb, &duf_print_dir_callbacks );
              r = duf_sccb_pdi( pdi, extended->call.fdesc.hi.sccb );
              T( "/SCCB %s : %p - %p", duf_levinfo_path( pdi ), extended->call.fdesc.hi.sccb, &duf_print_dir_callbacks );
            }
          }
        }
        break;
      case DUF_OPTION_VTYPE_DATETIME:
        {
          DUF_NUMOPT( unsigned long long, 0, duf_strtime2long );

          DUF_TEST_R( r );
        }
        break;
      case DUF_OPTION_VTYPE_MINMAXDATETIME:
        {
          DUF_MINMAXOPT( duf_limitsll_t, duf_strtime2long );

          DUF_TEST_R( r );
        }
        break;
      case DUF_OPTION_VTYPE_MINDATETIME:
        {
          DUF_MOPT( duf_limitsll_t, min, duf_strtime2long );

          DUF_TEST_R( r );
        }
        break;
      case DUF_OPTION_VTYPE_MAXDATETIME:
        {
          DUF_MOPT( duf_limitsll_t, max, duf_strtime2long );

          DUF_TEST_R( r );
        }
        break;
      case DUF_OPTION_VTYPE_VIFUN:
        {
          if ( extended->call.fdesc.vi.func )
            ( extended->call.fdesc.vi.func ) ( extended->call.fdesc.vi.arg );

          DUF_TEST_R( r );
        }
        break;
      case DUF_OPTION_VTYPE_AFUN:
        {
          assert( duf_config->cargv );
          if ( extended->call.fdesc.a.func )
            ( extended->call.fdesc.a.func ) ( duf_config->cargc, duf_config->cargv );

          DUF_TEST_R( r );
        }
        break;
      case DUF_OPTION_VTYPE_FILE:
        {
          DUF_OUTPUTFILE( duf_config_output_t );
          /* {                                                                                 */
          /*   char start_time[128] = "??";                                                    */
          /*                                                                                   */
          /*   duf_strflocaltime( start_time, sizeof( start_time ), "%Y%m%d.%H:%M:%S", NULL ); */
          /*   PF( "%s", start_time );                                                         */
          /* }                                                                                 */

          /* r = DUF_ERROR_OPTION_NOT_PARSED; */
          DUF_TEST_R( r );
        }
        break;
      }
  }
  DEBUG_ENDR( r );
}

int
duf_parse_option_long_old( const duf_longval_extended_t * extended, const char *optargg, int stage )
{
  DEBUG_STARTR( r );

  if ( extended )
    switch ( extended->oclass )
    {
    case DUF_OPTION_CLASS_ANY:
      /* r = 0; */
      break;
    case DUF_OPTION_CLASS_MIN:
      /* r = 0; */
      break;
    case DUF_OPTION_CLASS_NONE:
      /* r = 0; */
      break;
    case DUF_OPTION_CLASS_NO_HELP:
      /* r = 0; */
      break;
    case DUF_OPTION_CLASS_HELP:
      /* r = 0; */
      break;
    case DUF_OPTION_CLASS_SYSTEM:
      /* r = 0; */
      break;
    case DUF_OPTION_CLASS_CONTROL:
      /* r = 0; */
      break;
    case DUF_OPTION_CLASS_REFERENCE:
      /* r = 0; */
      break;
    case DUF_OPTION_CLASS_COLLECT:
      /* r = 0; */
      break;
    case DUF_OPTION_CLASS_SCAN:
      /* r = 0; */
      break;
    case DUF_OPTION_CLASS_FILTER:
      /* r = 0; */
      break;
    case DUF_OPTION_CLASS_UPDATE:
      /* r = 0; */
      break;
    case DUF_OPTION_CLASS_REQUEST:
      /* r = 0; */
      break;
    case DUF_OPTION_CLASS_PRINT:
      /* r = 0; */
      break;
    case DUF_OPTION_CLASS_TRACE:
      /* r = 0; */
      break;
    case DUF_OPTION_CLASS_DEBUG:
      /* r = 0; */
      break;
    case DUF_OPTION_CLASS_OBSOLETE:
      /* r = 0; */
      break;
    case DUF_OPTION_CLASS_NODESC:
      /* r = 0; */
      break;
    case DUF_OPTION_CLASS_OTHER:
      /* r = 0; */
      break;
    case DUF_OPTION_CLASS_MAX:
      /* r = 0; */
      break;
    case DUF_OPTION_CLASS_BAD:
      /* r = 0; */
      break;
    case DUF_OPTION_CLASS_ALL:
      /* r = 0; */
      break;
    }
  else
    r = DUF_ERROR_OPTION;

  DUF_TRACE( explain, 2, "(x%d) to parse %s (%s)  (%d)%s  cv:%d (F:%d)", extended ? 1 : 0, duf_option_description_x_tmp( -1, extended, NULL ),
             extended->o.name, r, duf_error_name( r ), extended->o.val, DUF_OPTION_FORMAT );

  if ( extended && stage == 0 )
  {
    switch ( extended->o.val )
    {
    case DUF_OPTION_NONE:
      break;
      /* case DUF_OPTION_SMART_HELP:     */
      /*                                 */
      /* case DUF_OPTION_HELP_ALL:       */
      /* case DUF_OPTION_HELP_NONE:      */
      /*                                 */
      /* case DUF_OPTION_HELP_HELP:      */
      /* case DUF_OPTION_HELP_SYSTEM:    */
      /* case DUF_OPTION_HELP_CONTROL:   */
      /* case DUF_OPTION_HELP_REFERENCE: */
      /* case DUF_OPTION_HELP_COLLECT:   */
      /* case DUF_OPTION_HELP_SCAN:      */
      /* case DUF_OPTION_HELP_UPDATE:    */
      /* case DUF_OPTION_HELP_REQUEST:   */
      /* case DUF_OPTION_HELP_PRINT:     */
      /*                                 */
      /* case DUF_OPTION_HELP_TRACE:     */
      /* case DUF_OPTION_HELP_DEBUG:     */
      /* case DUF_OPTION_HELP_OBSOLETE:  */
      /* case DUF_OPTION_HELP_NODESC:    */
/* .................................................. */
      /* case DUF_OPTION_HELP:                               */
      /* case DUF_OPTION_EXAMPLES:                           */
      /* case DUF_OPTION_VERSION:                            */
      /*   if ( optargg )                                     */
      /*   {                                                 */
      /*     if ( duf_config->help_string )                  */
      /*       mas_free( duf_config->help_string );          */
      /*     duf_config->help_string = mas_strdup( optargg ); */
      /*   }                                                 */
      /*   DUF_PRINTF( 0, "vvvvvvvvvvvv-" );                 */
      /*   break;                                            */
    case DUF_OPTION_TEST:
      DUF_PRINTF( 0, "This is test option output; optargg:%s", optargg ? optargg : "-" );
      break;
/* */
      /* DUF_OPTION_CASE_ACQUIRE_NUM( OUTPUT_LEVEL, (*             *) level, (*     *) cli.output ); */


/* debug etc. */
      /* DUF_OPTION_CASE_ACQUIRE_NUM( VERBOSE, (*            *) verbose, (*         *) cli.dbg ); */
      /* DUF_OPTION_CASE_ACQUIRE_NUM( DEBUG, (*              *) debug, (*           *) cli.dbg ); */
      /* DUF_OPTION_CASE_ACQUIRE_NUM( MIN_DBGLINE, (*        *) min_line, (*        *) cli.dbg ); */
      /* DUF_OPTION_CASE_ACQUIRE_NUM( MAX_DBGLINE, (*        *) max_line, (*        *) cli.dbg ); */



/* db */
      /* DUF_OPTION_CASE_ACQUIRE_STR( DB_DIRECTORY, (*           *) dir, (*             *) db ); */
      /* DUF_OPTION_CASE_ACQUIRE_STR( DB_NAME_MAIN, (*           *) main.name, (*       *) db ); */
      /* DUF_OPTION_CASE_ACQUIRE_STR( DB_NAME_ADM, (*            *) adm.name, (*        *) db ); */

      /* DUF_OPTION_CASE_ACQUIRE_ACT_FLAG( UNI_SCAN, (*          *) uni_scan (*     *)  ); */
      /* DUF_OPTION_CASE_ACQUIRE_ACT_FLAG( INFO, (*              *) info (*         *)  ); */

      /* DUF_OPTION_CASE_ACQUIRE_ACT_FLAG( REMOVE_DATABASE, (*   *) remove_database (* *)  ); */
      /* DUF_OPTION_CASE_ACQUIRE_ACT_FLAG( DROP_TABLES, (*       *) drop_tables (*     *)  ); */
      /* DUF_OPTION_CASE_ACQUIRE_ACT_FLAG( CREATE_TABLES, (*     *) create_tables (*   *)  ); */
      /* DUF_OPTION_CASE_ACQUIRE_ACT_FLAG( ADD_PATH, (*          *) add_path (*        *)  ); */
      /* DUF_OPTION_CASE_ACQUIRE_ACT_FLAG( FILEDATA, (*          *) filedata (*        *)  ); */
      /* DUF_OPTION_CASE_ACQUIRE_ACT_FLAG( FILENAMES, (*         *) filenames (*       *)  ); */
      /* DUF_OPTION_CASE_ACQUIRE_ACT_FLAG( VACUUM, (*            *) vacuum (*          *)  ); */

/* actions */
      /* DUF_OPTION_CASE_ACQUIRE_ACT_FLAG( PROGRESS, (*          *) progress (*        *)  ); */
      /* DUF_OPTION_CASE_ACQUIRE_ACT_FLAG( SUMMARY, (*           *) summary (*         *)  ); */

      /* DUF_OPTION_CASE_ACQUIRE_ACT_NUM_PLUS( SAMPLE, (*        *) sample (*          *)  ); */
      /* DUF_OPTION_CASE_ACQUIRE_ACT_NUM_PLUS( SAMPUPD, (*       *) sampupd (*         *)  ); */

      /* DUF_OPTION_CASE_ACQUIRE_ACT_FLAG( MDPATH, (*            *) mdpath (*          *)  ); */
      /* DUF_OPTION_CASE_ACQUIRE_ACT_FLAG( DIRENT, (*            *) dirent (*          *)  ); */
      /* DUF_OPTION_CASE_ACQUIRE_ACT_FLAG( SD5, (*               *) sd5 (*             *)  ); */
      /* DUF_OPTION_CASE_ACQUIRE_ACT_FLAG( MD5, (*               *) md5 (*             *)  ); */
      /* DUF_OPTION_CASE_ACQUIRE_ACT_FLAG( CRC32, (*             *) crc32 (*           *)  ); */
      /* DUF_OPTION_CASE_ACQUIRE_ACT_FLAG( MIME, (*              *) mime (*            *)  ); */
      /* DUF_OPTION_CASE_ACQUIRE_ACT_FLAG( EXIF, (*              *) exif (*            *)  ); */

      /* DUF_OPTION_CASE_ACQUIRE_ACT_FLAG( COLLECT, (*           *) collect (*         *)  ); */
      /* DUF_OPTION_CASE_ACQUIRE_ACT_FLAG( INTEGRITY, (*         *) integrity (*       *)  ); */
      /* DUF_OPTION_CASE_ACQUIRE_ACT_FLAG( PRINT, (*             *) print (*           *)  ); */
      /* DUF_OPTION_CASE_ACQUIRE_ACT_FLAG( TREE, (*              *) tree (*            *)  ); */
      /* DUF_OPTION_CASE_ACQUIRE_ACT_FLAG( DIRS, (*              *) dirs (*            *)  ); */
      /* DUF_OPTION_CASE_ACQUIRE_ACT_FLAG( FILES, (*             *) files (*           *)  ); */

      /* DUF_OPTION_CASE_ACQUIRE_FLAGG( DISABLE_CALCULATE, (*    *) calculate, (*      *) cli,.disable ); */
      /* DUF_OPTION_CASE_ACQUIRE_FLAGG( DISABLE_INSERT, (*       *) insert, (*         *) cli,.disable ); */
      /* DUF_OPTION_CASE_ACQUIRE_FLAGG( DISABLE_UPDATE, (*       *) update, (*         *) cli,.disable ); */

      /* DUF_OPTION_CASE_ACQUIRE_U_FLAG( RECURSIVE, (*         *) recursive (*       *)  ); */

      /* DUF_OPTION_CASE_ACQUIRE_U_NUM( SD5ID, (*              *) sd5id (*           *)  ); */
      /* DUF_OPTION_CASE_ACQUIRE_U_NUM( MD5ID, (*              *) md5id (*           *)  ); */
      /* DUF_OPTION_CASE_ACQUIRE_U_NUM( CRC32ID, (*            *) crc32id (*         *)  ); */
      /* DUF_OPTION_CASE_ACQUIRE_U_NUM( EXIFID, (*             *) exifid (*          *)  ); */
      /* DUF_OPTION_CASE_ACQUIRE_U_NUM( MIMEID, (*             *) mimeid (*          *)  ); */

/* limits, filters, selectors */
      /* case DUF_OPTION_SIZE:                                                           */
      /*   r = duf_limitsll( optargg, &duf_config->u.size.min, &duf_config->u.size.max ); */
      /*   break;                                                                        */
      /* DUF_OPTION_CASE_ACQUIRE_U_NUM( MAXSIZE, (*            *) size.max (*     *)  ); */
      /* DUF_OPTION_CASE_ACQUIRE_U_NUM( MINSIZE, (*            *) size.min (*     *)  ); */

      /* case DUF_OPTION_SAME:                                                         */
      /*   r = duf_limits( optargg, &duf_config->u.same.min, &duf_config->u.same.max ); */
      /*   break;                                                                      */
      /* DUF_OPTION_CASE_ACQUIRE_U_NUM( MAXSAME, (*            *) same.max (*     *)  ); */
      /* DUF_OPTION_CASE_ACQUIRE_U_NUM( MINSAME, (*            *) same.min (*     *)  ); */

      DUF_OPTION_CASE_ACQUIRE_U_NUM( MAXDIRFILES, /*        */ dirfiles.max /*     */  );
      DUF_OPTION_CASE_ACQUIRE_U_NUM( MINDIRFILES, /*        */ dirfiles.min /*     */  );

      /* DUF_OPTION_CASE_ACQUIRE_U_NUM( MAXSEQ, (*             *) maxseq (*     *)  ); */
      /* DUF_OPTION_CASE_ACQUIRE_U_NUM( MAXRELDEPTH, (*           *) max_rel_depth (*     *)  ); */

      DUF_OPTION_CASE_ACQUIRE_U_NUM( MAXITEMS, /*           */ maxitems.total /*     */  );
      DUF_OPTION_CASE_ACQUIRE_U_NUM( MAXITEMS_FILES, /*     */ maxitems.files /*     */  );
      DUF_OPTION_CASE_ACQUIRE_U_NUM( MAXITEMS_DIRS, /*      */ maxitems.dirs /*     */  );


      DUF_OPTION_CASE_ACQUIRE_U_ARG( GLOB_INCLUDE_FILES, /* */ globx.include_files /*     */  );
      DUF_OPTION_CASE_ACQUIRE_U_ARG( GLOB_EXCLUDE_FILES, /* */ globx.exclude_files /*     */  );


/* trace */
      /* DUF_OPTION_CASE_ACQUIRE_NUM( TRACE_NONEW, (*        *) nonew, (*     *) cli.trace ); */

      /* DUF_OPTION_CASE_ACQUIRE_TRACE( DRY_RUN, (*           *) dry_run ); */
      /* DUF_OPTION_CASE_ACQUIRE_TRACE( EXPLAIN, (*           *) explain ); */

      /* DUF_OPTION_CASE_ACQUIRE_TRACE( SEQ, (*              *) seq ); */
      /* DUF_OPTION_CASE_ACQUIRE_TRACE( OPTIONS, (*          *) options ); */
      /* DUF_OPTION_CASE_ACQUIRE_TRACE( CALLS, (*            *) calls ); */
      /* DUF_OPTION_CASE_ACQUIRE_TRACE( ANY, (*              *) any ); */
      /* DUF_OPTION_CASE_ACQUIRE_TRACE( CURRENT, (*          *) current ); */
      /* DUF_OPTION_CASE_ACQUIRE_TRACE( ACTION, (*           *) action ); */
      /* DUF_OPTION_CASE_ACQUIRE_TRACE( ERROR, (*            *) error ); */
      /* DUF_OPTION_CASE_ACQUIRE_TRACE( SCAN, (*             *) scan ); */
      /* DUF_OPTION_CASE_ACQUIRE_TRACE( SCAN_DE_DIR, (*      *) scan_de_dir ); */
      /* DUF_OPTION_CASE_ACQUIRE_TRACE( SCAN_DE_REG, (*      *) scan_de_reg ); */
      /* DUF_OPTION_CASE_ACQUIRE_TRACE( TEMP, (*             *) temp ); */
      /* DUF_OPTION_CASE_ACQUIRE_TRACE( PATH, (*             *) path ); */
      /* DUF_OPTION_CASE_ACQUIRE_TRACE( FS, (*               *) fs ); */
      /* DUF_OPTION_CASE_ACQUIRE_TRACE( DELETED, (*          *) deleted ); */
      /* DUF_OPTION_CASE_ACQUIRE_TRACE( SAMPUPD, (*          *) sampupd ); */
      /* DUF_OPTION_CASE_ACQUIRE_TRACE( SAMPLE, (*           *) sample ); */
      /* DUF_OPTION_CASE_ACQUIRE_TRACE( MDPATH, (*           *) mdpath ); */
      /* DUF_OPTION_CASE_ACQUIRE_TRACE( DIRENT, (*           *) dirent ); */
      /* DUF_OPTION_CASE_ACQUIRE_TRACE( SD5, (*              *) sd5 ); */
      /* DUF_OPTION_CASE_ACQUIRE_TRACE( MD5, (*              *) md5 ); */
      /* DUF_OPTION_CASE_ACQUIRE_TRACE( CRC32, (*            *) crc32 ); */
      /* DUF_OPTION_CASE_ACQUIRE_TRACE( MIME, (*             *) mime ); */
      /* DUF_OPTION_CASE_ACQUIRE_TRACE( EXIF, (*             *) exif ); */
      /* DUF_OPTION_CASE_ACQUIRE_TRACE( COLLECT, (*          *) collect ); */
      /* DUF_OPTION_CASE_ACQUIRE_TRACE( INTEGRITY, (*        *) integrity ); */
      /* DUF_OPTION_CASE_ACQUIRE_TRACE( SQL, (*              *) sql ); */
      /* DUF_OPTION_CASE_ACQUIRE_TRACE( SELECT, (*           *) select ); */
      /* DUF_OPTION_CASE_ACQUIRE_TRACE( INSERT, (*           *) insert ); */
      /* DUF_OPTION_CASE_ACQUIRE_TRACE( UPDATE, (*           *) update ); */

/* i/o */

    case DUF_OPTION_TRACE_STDERR:
      if ( duf_config->cli.trace.output.out )
      {
        if ( duf_config->cli.trace.output.out != stderr && duf_config->cli.trace.output.out != stdout )
          fclose( duf_config->cli.trace.output.out );
        duf_config->cli.trace.output.out = NULL;
      }
      if ( duf_config->cli.trace.output.file )
      {
        mas_free( duf_config->cli.trace.output.file );
        duf_config->cli.trace.output.file = NULL;
      }
      duf_config->cli.trace.output.out = stderr;
      r = 0;
      break;
    case DUF_OPTION_TRACE_STDOUT:
      if ( duf_config->cli.trace.output.out )
      {
        if ( duf_config->cli.trace.output.out != stderr && duf_config->cli.trace.output.out != stdout )
          fclose( duf_config->cli.trace.output.out );
        duf_config->cli.trace.output.out = NULL;
      }
      if ( duf_config->cli.trace.output.file )
      {
        mas_free( duf_config->cli.trace.output.file );
        duf_config->cli.trace.output.file = NULL;
      }
      duf_config->cli.trace.output.out = stdout;
      r = 0;
      break;
      /* case DUF_OPTION_TRACE_FILE:                                                                              */
      /*   r = duf_open_special( optargg, &duf_config->cli.trace.output.file, &duf_config->cli.trace.output.out ); */
      /*   break;                                                                                                 */
      /*                                                                                                          */
      /* case DUF_OPTION_OUTPUT_FILE:                                                                             */
      /*   r = duf_open_special( optargg, &duf_config->cli.output.file, &duf_config->cli.output.out );             */
      /*   break;                                                                                                 */

/* combined */
    case DUF_OPTION_ALL_TRACE:
      if ( optargg && *optargg )
        duf_config->cli.trace.sql = duf_config->cli.trace.select = duf_config->cli.trace.insert = duf_config->cli.trace.update =
              duf_config->cli.trace.collect = duf_config->cli.trace.dirent = duf_config->cli.trace.sd5 = duf_config->cli.trace.md5 =
              duf_config->cli.trace.crc32 = duf_config->cli.trace.mime = duf_config->cli.trace.exif = duf_config->cli.trace.sample =
              duf_config->cli.trace.deleted = duf_config->cli.trace.scan = duf_config->cli.trace.temp = strtol( optargg, NULL, 10 );
      else
      {
        duf_config->cli.trace.sql++;
        duf_config->cli.trace.select++;
        duf_config->cli.trace.insert++;
        duf_config->cli.trace.update++;
        duf_config->cli.trace.collect++;
        duf_config->cli.trace.sd5++;
        duf_config->cli.trace.md5++;
        duf_config->cli.trace.crc32++;
        duf_config->cli.trace.mime++;
        duf_config->cli.trace.exif++;
        duf_config->cli.trace.dirent++;
        duf_config->cli.trace.sample++;
        duf_config->cli.trace.deleted++;
        duf_config->cli.trace.scan++;
        duf_config->cli.trace.temp++;
      }
      break;

    case DUF_OPTION_FLAG_ZERO_DB:
      DUF_OPTION_ACQUIRE_FLAG( create_tables, cli.act );
/* no break here ! */
    case DUF_OPTION_TREE_TO_DB:
      /* -ORifd5 
       * i.e.
       *  --create-tables --uni-scan --recursive ...
       *  */
      DUF_ACT_FLAG( create_tables ) = /* */
            DUF_ACT_FLAG( add_path ) = /* */
            DUF_ACT_FLAG( uni_scan ) = /* */
            DUF_U_FLAG( recursive ) = /* */
            DUF_ACT_FLAG( files ) = /* */
            DUF_ACT_FLAG( dirs ) = /* */
            DUF_ACT_FLAG( dirent ) = /* */
            DUF_ACT_FLAG( sd5 ) = /* */
            DUF_ACT_FLAG( md5 ) = /* */
            DUF_ACT_FLAG( crc32 ) = /* */
            DUF_ACT_FLAG( filedata ) = /* */
            DUF_ACT_FLAG( filenames ) = /* */
            1;
      break;

/* specific */
    case DUF_OPTION_FORMAT:
      {
        char *coptarg, *poptarg;
        unsigned nvalue;
        char *value;

        DUF_TRACE( options, 3, "really to parse %s = %s (%s)  %s", duf_option_description_x_tmp( -1, extended, NULL ), optargg, extended->o.name,
                   duf_error_name( r ) );

        /* duf_config->cli.format.seq = (* *)            */
        /*       duf_config->cli.format.dirid = (* *)    */
        /*       duf_config->cli.format.inode = (* *)    */
        /*       duf_config->cli.format.mode = (* *)     */
        /*       duf_config->cli.format.nlink = (* *)    */
        /*       duf_config->cli.format.uid = (* *)      */
        /*       duf_config->cli.format.gid = (* *)      */
        /*       duf_config->cli.format.prefix = (* *)   */
        /*       duf_config->cli.format.suffix = (* *)   */
        /*       duf_config->cli.format.filesize = (* *) */
        /*       duf_config->cli.format.mtime = (* *)    */
        /*       duf_config->cli.format.filename = (* *) */
        /*       duf_config->cli.format.md5 = (* *)      */
        /*       duf_config->cli.format.md5id = (* *)    */
        /*       duf_config->cli.format.nsame = 0;       */


        char *const tokens[] = {
          [DUF_FORMAT_DATAID] = "dataid",
          [DUF_FORMAT_DIRID] = "dirid",
          [DUF_FORMAT_DIRID_SPACE] = "dirid_space",
          [DUF_FORMAT_NFILES] = "nfiles",
          [DUF_FORMAT_NFILES_SPACE] = "nfiles_space",
          [DUF_FORMAT_NDIRS] = "ndirs",
          [DUF_FORMAT_NDIRS_SPACE] = "ndirs_space",
          [DUF_FORMAT_FILENAME] = "filename",
          [DUF_FORMAT_FILESIZE] = "filesize",
          [DUF_FORMAT_HUMAN] = "human",
          [DUF_FORMAT_INODE] = "inode",
          [DUF_FORMAT_SD5ID] = "sd5id",
          [DUF_FORMAT_SD5] = "sd5",
          [DUF_FORMAT_MD5ID] = "md5id",
          [DUF_FORMAT_MD5] = "md5",
          [DUF_FORMAT_CRC32ID] = "crc32id",
          [DUF_FORMAT_CRC32] = "crc32",
          [DUF_FORMAT_NAMEID] = "nameid",
          [DUF_FORMAT_MIME] = "mime",
          [DUF_FORMAT_MIMEID] = "mimeid",
          [DUF_FORMAT_EXIFID] = "exifid",
          [DUF_FORMAT_MODE] = "mode",
          [DUF_FORMAT_MTIME] = "mtime",
          [DUF_FORMAT_NLINK] = "nlink",
          [DUF_FORMAT_NSAME] = "nsame",
          [DUF_FORMAT_OFFSET] = "offset",
          [DUF_FORMAT_PREFIX] = "prefix",
          [DUF_FORMAT_SUFFIX] = "suffix",
          [DUF_FORMAT_REALPATH] = "realpath",
          [DUF_FORMAT_DEPTH] = "depth",
          [DUF_FORMAT_SEQ] = "seq",
          [DUF_FORMAT_SEQ_NODE] = "seq-node",
          [DUF_FORMAT_SEQ_LEAF] = "seq-leaf",[DUF_FORMAT_GROUP] = "group",[DUF_FORMAT_USER] = "user",[DUF_FORMAT_MAX] = NULL,
        };
        poptarg = coptarg = mas_strdup( optargg );
        /* coptarg = mas_strdup( optargg ); */
        DUF_TRACE( action, 0, "--format=%s", coptarg );
        while ( poptarg && *poptarg )
        {
          char *hlp;
          int rs = 0;

          hlp = poptarg;
          DUF_TRACE( any, 0, "hlp:%s", hlp );
          value = NULL;
          DUF_TRACE( options, 2, "really to parse format item [%s]     value:%s", poptarg, value );
          rs = getsubopt( &poptarg, tokens, &value );
          DUF_TRACE( options, 2, "%d: (%s) '%s'", rs, hlp, value ? value : "nil" );
          nvalue = value ? strtol( value, NULL, 10 ) : -1;
          switch ( rs )
          {
          case DUF_FORMAT_DEPTH:
            duf_config->cli.format.v.flag.depth = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_SEQ:
            duf_config->cli.format.v.flag.seq = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_SEQ_NODE:
            duf_config->cli.format.v.flag.seq_node = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_SEQ_LEAF:
            duf_config->cli.format.v.flag.seq_leaf = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_PREFIX:
            duf_config->cli.format.v.flag.prefix = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_SUFFIX:
            duf_config->cli.format.v.flag.suffix = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_DIRID:
            duf_config->cli.format.v.flag.dirid = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_DIRID_SPACE:
            duf_config->cli.format.v.flag.dirid_space = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_NFILES:
            duf_config->cli.format.v.flag.nfiles = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_NFILES_SPACE:
            duf_config->cli.format.v.flag.nfiles_space = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_NDIRS:
            duf_config->cli.format.v.flag.ndirs = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_NDIRS_SPACE:
            duf_config->cli.format.v.flag.ndirs_space = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_REALPATH:
            duf_config->cli.format.v.flag.realpath = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_INODE:
            duf_config->cli.format.v.flag.inode = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_MODE:
            duf_config->cli.format.v.flag.mode = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_NLINK:
            duf_config->cli.format.v.flag.nlink = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_USER:
            duf_config->cli.format.v.flag.user = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_GROUP:
            duf_config->cli.format.v.flag.group = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_FILESIZE:
            duf_config->cli.format.v.flag.filesize = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_MTIME:
            duf_config->cli.format.v.flag.mtime = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_FILENAME:
            duf_config->cli.format.v.flag.filename = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_SD5:
            duf_config->cli.format.v.flag.sd5 = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_SD5ID:
            duf_config->cli.format.v.flag.sd5id = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_MD5:
            duf_config->cli.format.v.flag.md5 = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_MD5ID:
            duf_config->cli.format.v.flag.md5id = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_CRC32:
            duf_config->cli.format.v.flag.crc32 = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_CRC32ID:
            duf_config->cli.format.v.flag.crc32id = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_NAMEID:
            duf_config->cli.format.v.flag.nameid = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_MIME:
            duf_config->cli.format.v.flag.mime = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_MIMEID:
            duf_config->cli.format.v.flag.mimeid = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_EXIFID:
            duf_config->cli.format.v.flag.exifid = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_HUMAN:
            duf_config->cli.format.v.flag.human = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_DATAID:
            duf_config->cli.format.v.flag.dataid = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_NSAME:
            duf_config->cli.format.nsame = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_OFFSET:
            duf_config->cli.format.offset = value == NULL ? 0 : nvalue;
            break;
          }
          DUF_TRACE( options, 2, "rs:%d [%s:%s:%s] FORMAT bits: %llx", rs, poptarg, hlp, value ? value : "nil", duf_config->cli.format.v.bit );
          /* DUF_TRACE( any, 0, "rs:%d", rs ); */
          if ( rs < 0 )
          {
            r = DUF_ERROR_SUBOPTION;
            DUF_TEST_R( r );
            break;
          }
          else
            r = 0;
        }
        mas_free( coptarg );
      }

      DUF_TEST_R( r );
      break;
    case DUF_OPTION_MEMUSAGE:
      {
        extern int mas_mem_disable_print_usage __attribute__ ( ( weak ) );

        if ( &mas_mem_disable_print_usage )
        {
          mas_mem_disable_print_usage = 0;
        }
      }
      break;
    default:
      r = DUF_ERROR_OPTION;
      break;
    }
  }

  DEBUG_ENDR( r );
}
