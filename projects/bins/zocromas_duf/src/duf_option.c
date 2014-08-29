#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

/* #include <getopt.h> */
#include <sys/types.h>
#include <sys/stat.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>
/* #include <mastar/tools/mas_arg_tools.h> */

#include "duf_maintenance.h"


/* #include "duf_types.h" */
#include "duf_utils.h"

/* #include "duf_config.h" */
#include "duf_config_ref.h"

#include "duf_options.h"

/* ###################################################################### */
#include "duf_option.h"
/* ###################################################################### */


static long
duf_strtol( const char *s, int *pr )
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

DUF_UNUSED static int
duf_limits( const char *s, unsigned *pmin, unsigned *pmax )
{
  int r = 0;

  if ( s )
  {
    long n;
    char c = 0;

    if ( *s == '+' )
      c = *s++;
    else if ( *s == '-' )
      c = *s++;
    n = duf_strtol( s, &r );
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
    DUF_ERROR( "[%c] %d - %d", c, pmin ? *pmin : 0, pmax ? *pmax : 0 );
  }
  return r;
}

DUF_UNUSED static int
duf_limitsll( const char *s, unsigned long long *pmin, unsigned long long *pmax )
{
  int r = 0;

  if ( s )
  {
    long n;
    char c = 0;

    if ( *s == '+' )
      c = *s++;
    else if ( *s == '-' )
      c = *s++;
    n = duf_strtol( s, &r );
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
    DUF_ERROR( "[%c] %lld - %lld", c, pmin ? *pmin : 0, pmax ? *pmax : 0 );
  }
  return r;
}

int
duf_open_special( const char *pname, char **pfilename, FILE ** pfile )
{
  int r = 0;

  if ( pname && *pname && pfilename && pfile )
  {
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
    if ( *pfilename )
      mas_free( *pfilename );
    *pfilename = mas_strdup( pname );
    if ( 0 == stat( pname, &st ) && ( ( !S_ISCHR( st.st_mode ) || !( st.st_mode & S_IWUSR ) ) && ( !overw && *mode != 'a' ) ) )
    {
      DUF_ERROR( "can't open file %s for writing file exists %llu / %llu chr:%d\n", pname, ( unsigned long long ) st.st_dev,
                 ( unsigned long long ) st.st_rdev, S_ISCHR( st.st_mode ) );
      r = DUF_ERROR_OPTION_VALUE;
    }
    else
    {
      FILE *out;

      out = fopen( *pfilename, mode );
      if ( out )
      {
        if ( *pfile && *pfile != stderr && *pfile != stdout )
          fclose( *pfile );
        *pfile = out;
      }
      else
      {
        DUF_ERROR( "can't open file %s\n", pname );
        r = DUF_ERROR_OPTION_VALUE;
      }
    }
  }
  return r;
}

int
duf_parse_option( duf_option_code_t codeval, int longindex, const char *optarg )
{
  int r = -1;

  assert( ( int ) codeval >= 0 );
  /* short always corresponds long (in my case) - find it */
  if ( longindex < 0 )
    longindex = duf_find_long( codeval );
  if ( longindex >= 0 )
  {
    r = duf_parse_option_long( longindex, optarg );
    DUF_TRACE( explain, 2, "cli options r: %d", r );
  }
  else
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
      DUF_TRACE( explain, 3, "cli options r: %d; codeval:%d; longindex:%d", r, codeval, longindex );
      break;
    }
  DUF_TRACE( explain, 3, "cli options r: %d; codeval:%d; longindex:%d", r, codeval, longindex );
  return r;
}

int
duf_parse_option_long( int longindex, const char *optarg )
{
  int r = 0;
  int done = 0;
  duf_option_code_t codeval;
  const duf_longval_extended_t *extended;

  extended = duf_longindex_extended( longindex );
  codeval = extended->o.val;
  codeval = duf_config->longopts_table[longindex].val;
  /* __attribute__ ( ( unused ) ) size_t loptarg = 0; */
  duf_option_class_t oclass = DUF_OPTION_CLASS_BAD;

  /* if ( optarg )                 */
  /*   loptarg = strlen( optarg ); */

  oclass = duf_help_option2class( codeval );

  if (  /* oclass != DUF_OPTION_CLASS_NO_HELP && */ oclass != DUF_OPTION_CLASS_BAD )
  {
    if ( optarg )
    {
      if ( duf_config->help_string )
        mas_free( duf_config->help_string );
      duf_config->help_string = mas_strdup( optarg );
    }
    r = codeval;
    done = 1;
  }
  if ( !done )
  {
    if ( extended->mf == 1 )
    {
      unsigned *pi;
      unsigned long long *pll;
      unsigned short *pis;
      char **pstr;
      unsigned doplus = 0;

      pi = ( unsigned * ) ( ( ( char * ) duf_config ) + extended->m );
      pis = ( unsigned short * ) ( ( ( char * ) duf_config ) + extended->m );
      pll = ( unsigned long long * ) ( ( ( char * ) duf_config ) + extended->m );
      pstr = ( char ** ) ( ( ( char * ) duf_config ) + extended->m );
      switch ( extended->vtype )
      {
      case DUF_OPTION_VTYPE_UPLUS:
        doplus = 1;
      case DUF_OPTION_VTYPE_NUM:
        {
          int rl = 0;

          {
            if ( optarg )
            {
              ( *pi ) = duf_strtol( optarg, &rl );
              done = 1;
            }
            else if ( doplus )
            {
              ( *pi )++;
              done = 1;
            }
          }                     /* r = codeval; */
          /* DUF_PRINTF( 0, "T------------oc:%d; %d %lu/%lu '%s' :: %d/%d r(%d)", extended->oclass, extended->mf, extended->m, */
          /*             offsetof( duf_config_cli_trace_t, scan ), extended->o.name, *pi, duf_config->cli.trace.scan, r );     */
        }
        break;
      case DUF_OPTION_VTYPE_NLL:
        {
          int rl = 0;

          {
            if ( optarg )
            {
              ( *pll ) = duf_strtol( optarg, &rl );
              done = 1;
            }
            else if ( doplus )
            {
              ( *pll )++;
              done = 1;
            }
          }                     /* r = codeval; */
          /* DUF_PRINTF( 0, "T------------oc:%d; %d %lu/%lu '%s' :: %d/%d r(%d)", extended->oclass, extended->mf, extended->m, */
          /*             offsetof( duf_config_cli_trace_t, scan ), extended->o.name, *pi, duf_config->cli.trace.scan, r );     */
        }
        break;
      case DUF_OPTION_VTYPE_MINMAX:
        {
          int rl = 0;

          {
            if ( optarg )
            {
              unsigned n;

              n = duf_strtol( optarg, &rl );
              *pi++ = n;
              *pi = n;
              done = 1;
            }
            /* DUF_PRINTF( 0, "%u - %u", duf_config->u.same.min, duf_config->u.same.max ); */
          }                     /* r = codeval; */
        }
        break;
      case DUF_OPTION_VTYPE_MINMAXLL:
        {
          int rl = 0;

          {
            if ( optarg )
            {
              unsigned long long n;

              n = duf_strtol( optarg, &rl );
              *pll++ = n;
              *pll = n;
              done = 1;
            }
            /* DUF_PRINTF( 0, "%llu - %llu", duf_config->u.same.min, duf_config->u.same.max ); */
          }                     /* r = codeval; */
        }
        break;
        /* case DUF_OPTION_CLASS_DEBUG: */
        /* DUF_PRINTF( 0, "------------ %lu", extended->m ); */
        /* break; */
      case DUF_OPTION_VTYPE_FLAG:
        {
          ( *pi ) |= extended->afl.bit;
          done = 1;
        }
        break;
      case DUF_OPTION_VTYPE_SFLAG:
        {
          ( *pis ) |= extended->afl.sbit;
          done = 1;
        }
        break;
      case DUF_OPTION_VTYPE_STR:
        {
          if ( pstr && *pstr )
            mas_free( *pstr );
          *pstr = NULL;
          if ( optarg )
            *pstr = mas_strdup( optarg );
          done = 1;
        }
        break;
      }
    }
  }
  /* DUF_PRINTF( 0, "############################### %x", extended->afl.bit ); */
  if ( !done )
    switch ( codeval )
    {
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
    case DUF_OPTION_HELP:
    case DUF_OPTION_EXAMPLES:
    case DUF_OPTION_VERSION:
      if ( optarg )
      {
        if ( duf_config->help_string )
          mas_free( duf_config->help_string );
        duf_config->help_string = mas_strdup( optarg );
      }
      r = codeval;
      done = 1;
      break;
    case DUF_OPTION_TEST:
      DUF_PRINTF( 0, "This is test option output; optarg:%s", optarg ? optarg : "-" );
      done = 1;
      break;
/* */
      DUF_OPTION_CASE_ACQUIRE_NUM( OUTPUT_LEVEL, /*             */ level, /*     */ cli.output );


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

      DUF_OPTION_CASE_ACQUIRE_U_NUM( SD5ID, /*              */ sd5id /*           */  );
      DUF_OPTION_CASE_ACQUIRE_U_NUM( MD5ID, /*              */ md5id /*           */  );
      DUF_OPTION_CASE_ACQUIRE_U_NUM( CRC32ID, /*            */ crc32id /*         */  );
      DUF_OPTION_CASE_ACQUIRE_U_NUM( EXIFID, /*             */ exifid /*          */  );
      DUF_OPTION_CASE_ACQUIRE_U_NUM( MIMEID, /*             */ mimeid /*          */  );

/* limits, filters, selectors */
    /* case DUF_OPTION_SIZE:                                                           */
    /*   r = duf_limitsll( optarg, &duf_config->u.size.min, &duf_config->u.size.max ); */
    /*   done = 1;                                                                     */
    /*   break;                                                                        */
      /* DUF_OPTION_CASE_ACQUIRE_U_NUM( MAXSIZE, (*            *) size.max (*     *)  ); */
      /* DUF_OPTION_CASE_ACQUIRE_U_NUM( MINSIZE, (*            *) size.min (*     *)  ); */

      /* case DUF_OPTION_SAME:                                                         */
      /*   r = duf_limits( optarg, &duf_config->u.same.min, &duf_config->u.same.max ); */
      /*   done = 1;                                                                   */
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


      DUF_OPTION_CASE_ACQUIRE_U_ARG( GLOB_INCLUDE_FILES, /* */ glob.include_files /*     */  );
      DUF_OPTION_CASE_ACQUIRE_U_ARG( GLOB_EXCLUDE_FILES, /* */ glob.exclude_files /*     */  );


/* trace */
      /* DUF_OPTION_CASE_ACQUIRE_NUM( TRACE_NONEW, (*        *) nonew, (*     *) cli.trace ); */

      /* DUF_OPTION_CASE_ACQUIRE_TRACE( DRY_RUN, (*           *) dry_run ); */
      /* DUF_OPTION_CASE_ACQUIRE_TRACE( EXPLAIN, (*           *) explain ); */

      DUF_OPTION_CASE_ACQUIRE_TRACE( SEQ, /*              */ seq );
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
      if ( duf_config->cli.trace.out )
      {
        if ( duf_config->cli.trace.out != stderr && duf_config->cli.trace.out != stdout )
          fclose( duf_config->cli.trace.out );
        duf_config->cli.trace.out = NULL;
      }
      if ( duf_config->cli.trace.file )
      {
        mas_free( duf_config->cli.trace.file );
        duf_config->cli.trace.file = NULL;
      }
      duf_config->cli.trace.out = stderr;
      done = 1;
      break;
    case DUF_OPTION_TRACE_STDOUT:
      if ( duf_config->cli.trace.out )
      {
        if ( duf_config->cli.trace.out != stderr && duf_config->cli.trace.out != stdout )
          fclose( duf_config->cli.trace.out );
        duf_config->cli.trace.out = NULL;
      }
      if ( duf_config->cli.trace.file )
      {
        mas_free( duf_config->cli.trace.file );
        duf_config->cli.trace.file = NULL;
      }
      duf_config->cli.trace.out = stdout;
      done = 1;
      break;
    case DUF_OPTION_TRACE_FILE:
      r = duf_open_special( optarg, &duf_config->cli.trace.file, &duf_config->cli.trace.out );
      done = 1;
      break;

    case DUF_OPTION_OUTPUT_FILE:
      r = duf_open_special( optarg, &duf_config->cli.output.file, &duf_config->cli.output.out );
      done = 1;
      break;

/* combined */
    case DUF_OPTION_ALL_TRACE:
      if ( optarg && *optarg )
        duf_config->cli.trace.sql = duf_config->cli.trace.select = duf_config->cli.trace.insert = duf_config->cli.trace.update =
              duf_config->cli.trace.collect = duf_config->cli.trace.dirent = duf_config->cli.trace.sd5 = duf_config->cli.trace.md5 =
              duf_config->cli.trace.crc32 = duf_config->cli.trace.mime = duf_config->cli.trace.exif = duf_config->cli.trace.sample =
              duf_config->cli.trace.deleted = duf_config->cli.trace.scan = duf_config->cli.trace.temp = strtol( optarg, NULL, 10 );
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
      done = 1;
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
      done = 1;
      break;

/* specific */
    case DUF_OPTION_FORMAT:
      {
        char *coptarg, *poptarg;
        unsigned nvalue;
        char *value;


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
          [DUF_FORMAT_GID] = "gid",
          [DUF_FORMAT_HUMAN] = "human",
          [DUF_FORMAT_INODE] = "inode",
          [DUF_FORMAT_SD5ID] = "sd5id",
          [DUF_FORMAT_SD5] = "sd5",
          [DUF_FORMAT_MD5ID] = "md5id",
          [DUF_FORMAT_MD5] = "md5",
          [DUF_FORMAT_CRC32ID] = "crc32id",
          [DUF_FORMAT_CRC32] = "crc32",
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
          [DUF_FORMAT_SEQ] = "seq",
          [DUF_FORMAT_TRUEPATH] = "truepath",
          [DUF_FORMAT_UID] = "uid",
          [DUF_FORMAT_MAX] = NULL,
        };
        poptarg = coptarg = mas_strdup( optarg );
        /* coptarg = mas_strdup( optarg ); */
        value = NULL;
        DUF_TRACE( action, 0, "--format=%s", coptarg );
        while ( poptarg && *poptarg )
        {
          char *hlp;

          hlp = poptarg;
          DUF_TRACE( any, 0, "hlp:%s", hlp );
          r = getsubopt( &poptarg, tokens, &value );
          DUF_TRACE( any, 0, "%d: (%s) '%s'", r, hlp, value ? value : "nil" );
          nvalue = value ? strtol( value, NULL, 10 ) : -1;
          switch ( r )
          {
          case DUF_FORMAT_SEQ:
            duf_config->cli.format.seq = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_PREFIX:
            duf_config->cli.format.prefix = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_SUFFIX:
            duf_config->cli.format.suffix = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_DIRID:
            duf_config->cli.format.dirid = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_DIRID_SPACE:
            duf_config->cli.format.dirid_space = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_NFILES:
            duf_config->cli.format.nfiles = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_NFILES_SPACE:
            duf_config->cli.format.nfiles_space = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_NDIRS:
            duf_config->cli.format.ndirs = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_NDIRS_SPACE:
            duf_config->cli.format.ndirs_space = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_TRUEPATH:
            duf_config->cli.format.truepath = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_REALPATH:
            duf_config->cli.format.realpath = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_INODE:
            duf_config->cli.format.inode = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_MODE:
            duf_config->cli.format.mode = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_NLINK:
            duf_config->cli.format.nlink = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_UID:
            duf_config->cli.format.uid = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_GID:
            duf_config->cli.format.gid = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_FILESIZE:
            duf_config->cli.format.filesize = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_MTIME:
            duf_config->cli.format.mtime = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_FILENAME:
            duf_config->cli.format.filename = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_SD5:
            duf_config->cli.format.sd5 = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_SD5ID:
            duf_config->cli.format.sd5id = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_MD5:
            duf_config->cli.format.md5 = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_MD5ID:
            duf_config->cli.format.md5id = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_CRC32:
            duf_config->cli.format.crc32 = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_CRC32ID:
            duf_config->cli.format.crc32id = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_MIMEID:
            duf_config->cli.format.mimeid = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_EXIFID:
            duf_config->cli.format.exifid = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_HUMAN:
            duf_config->cli.format.human = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_DATAID:
            duf_config->cli.format.dataid = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_NSAME:
            duf_config->cli.format.nsame = value == NULL ? 1 : nvalue;
            break;
          case DUF_FORMAT_OFFSET:
            duf_config->cli.format.offset = value == NULL ? 0 : nvalue;
            break;
          }
          DUF_TRACE( any, 0, "%d: (%s) '%s'", r, hlp, value ? value : "nil" );
          /* DUF_TRACE( any, 0, "r:%d", r ); */
          if ( r < 0 )
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
      done = 1;
      break;
    case DUF_OPTION_MEMUSAGE:
      {
        extern int mas_mem_disable_print_usage __attribute__ ( ( weak ) );

        if ( &mas_mem_disable_print_usage )
        {
          mas_mem_disable_print_usage = 0;
        }
      }
      done = 1;
      break;
    default:
      r = DUF_ERROR_OPTION;
      break;
    }
  /* DUF_TRACE( explain, 0, "parse option opt:%d r: %d", codeval, r ); */
  if ( r )
  {
    char *ona = NULL;

    ona = duf_option_description( longindex, NULL );
    /* DUF_ERROR( "returns: %d = ( %s )", r, ona ); */
    mas_free( ona );
  }
  /* DUF_PRINTF( 0, "xxxxxxxxxxxxxxxxxx %s -> %lu:%lu", optarg, duf_config->cli.dbg.min_line, duf_config->cli.dbg.max_line ); */

  DUF_TEST_RN( r );
  return r;
}
