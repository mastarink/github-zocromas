#include <stdarg.h>
#include <string.h>
#include <getopt.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

/* #include <mastar/tools/mas_arg_tools.h> */

#include "duf_types.h"
#include "duf_utils.h"

#include "duf_config.h"

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

static int
duf_limits( const char *s, unsigned long long *pmin, unsigned long long *pmax )
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
duf_parse_option( int opt, const char *optarg, int longindex, const duf_longval_extended_t * extended )
{
  int r = 0;
  __attribute__ ( ( unused ) ) size_t loptarg = 0;

  if ( optarg )
    loptarg = strlen( optarg );
  switch ( opt )
  {
  case DUF_OPTION_HELP:
  case DUF_OPTION_SMART_HELP:
  case DUF_OPTION_EXAMPLES:
    r = opt;
    break;
  case DUF_OPTION_TEST:
    DUF_PRINTF( 0, "This is test option output; optarg:%s", optarg ? optarg : "-" );
    break;
/* */
    DUF_OPTION_CASE_ACQUIRE_NUM( OUTPUT, /*             */ level, /*     */ cli.output );


/* debug etc. */
    DUF_OPTION_CASE_ACQUIRE_NUM( VERBOSE, /*            */ verbose, /*         */ cli.dbg );
    DUF_OPTION_CASE_ACQUIRE_NUM( DEBUG, /*              */ debug, /*           */ cli.dbg );
    DUF_OPTION_CASE_ACQUIRE_NUM( MIN_DBGLINE, /*        */ min_line, /*        */ cli.dbg );
    DUF_OPTION_CASE_ACQUIRE_NUM( MAX_DBGLINE, /*        */ max_line, /*        */ cli.dbg );

    DUF_OPTION_CASE_ACQUIRE_FLAG( DRY_RUN, /*           */ dry_run, /*         */ cli );
    DUF_OPTION_CASE_ACQUIRE_FLAG( NOOPENAT, /*          */ noopenat, /*        */ cli );


/* db */
    DUF_OPTION_CASE_ACQUIRE_STR( DB_DIRECTORY, /*       */ dir, /*             */ db );
    DUF_OPTION_CASE_ACQUIRE_STR( DB_NAME_MAIN, /*       */ main.name, /*       */ db );
    DUF_OPTION_CASE_ACQUIRE_STR( DB_NAME_ADM, /*        */ adm.name, /*        */ db );

    DUF_OPTION_CASE_ACQUIRE_FLAG( REMOVE_DATABASE, /*   */ remove_database, /* */ cli.act );
    DUF_OPTION_CASE_ACQUIRE_FLAG( DROP_TABLES, /*       */ drop_tables, /*     */ cli.act );
    DUF_OPTION_CASE_ACQUIRE_FLAG( CREATE_TABLES, /*     */ create_tables, /*   */ cli.act );
    DUF_OPTION_CASE_ACQUIRE_FLAG( ADD_PATH, /*          */ add_path, /*        */ cli.act );
    DUF_OPTION_CASE_ACQUIRE_FLAG( FILEDATA, /*          */ filedata, /*        */ cli.act );
    DUF_OPTION_CASE_ACQUIRE_FLAG( FILENAMES, /*         */ filenames, /*       */ cli.act );
    DUF_OPTION_CASE_ACQUIRE_FLAG( VACUUM, /*            */ vacuum, /*          */ cli.act );

/* actions */
    DUF_OPTION_CASE_ACQUIRE_FLAG( PROGRESS, /*          */ progress, /*        */ cli.act );
    DUF_OPTION_CASE_ACQUIRE_FLAG( SUMMARY, /*           */ summary, /*         */ cli.act );

    DUF_OPTION_CASE_ACQUIRE_NUM( SAMPLE, /*             */ sample, /*          */ cli.act );
    DUF_OPTION_CASE_ACQUIRE_NUM( SAMPUPD, /*            */ sampupd, /*         */ cli.act );

    DUF_OPTION_CASE_ACQUIRE_FLAG( MDPATH, /*            */ mdpath, /*          */ cli.act );
    DUF_OPTION_CASE_ACQUIRE_FLAG( DIRENT, /*            */ dirent, /*          */ cli.act );
    DUF_OPTION_CASE_ACQUIRE_FLAG( SD5, /*               */ sd5, /*             */ cli.act );
    DUF_OPTION_CASE_ACQUIRE_FLAG( MD5, /*               */ md5, /*             */ cli.act );
    DUF_OPTION_CASE_ACQUIRE_FLAG( CRC32, /*             */ crc32, /*           */ cli.act );
    DUF_OPTION_CASE_ACQUIRE_FLAG( MIME, /*              */ mime, /*            */ cli.act );

    DUF_OPTION_CASE_ACQUIRE_FLAG( COLLECT, /*           */ collect, /*         */ cli.act );
    DUF_OPTION_CASE_ACQUIRE_FLAG( INTEGRITY, /*         */ integrity, /*       */ cli.act );
    DUF_OPTION_CASE_ACQUIRE_FLAG( PRINT, /*             */ print, /*           */ cli.act );
    DUF_OPTION_CASE_ACQUIRE_FLAG( TREE, /*              */ tree, /*            */ cli.act );
    DUF_OPTION_CASE_ACQUIRE_FLAG( DIRS, /*              */ dirs, /*            */ cli.act );
    DUF_OPTION_CASE_ACQUIRE_FLAG( FILES, /*             */ files, /*           */ cli.act );

    DUF_OPTION_CASE_ACQUIRE_FLAG( DISABLE_CALCULATE, /* */ calculate, /*       */ cli.disable );
    DUF_OPTION_CASE_ACQUIRE_FLAG( DISABLE_INSERT, /*    */ insert, /*          */ cli.disable );
    DUF_OPTION_CASE_ACQUIRE_FLAG( DISABLE_UPDATE, /*    */ update, /*          */ cli.disable );

    DUF_OPTION_CASE_ACQUIRE_FLAG( RECURSIVE, /*         */ recursive, /*       */ u );

    DUF_OPTION_CASE_ACQUIRE_NUM( SD5ID, /*              */ sd5id, /*           */ u );
    DUF_OPTION_CASE_ACQUIRE_NUM( MD5ID, /*              */ md5id, /*           */ u );
    DUF_OPTION_CASE_ACQUIRE_NUM( CRC32ID, /*            */ crc32id, /*         */ u );
    DUF_OPTION_CASE_ACQUIRE_NUM( MIMEID, /*             */ mimeid, /*          */ u );

/* limits, filters, selectors */
  case DUF_OPTION_SIZE:
    r = duf_limits( optarg, &duf_config->u.minsize, &duf_config->u.maxsize );
    break;
    DUF_OPTION_CASE_ACQUIRE_NUM( MAXSIZE, /*            */ maxsize, /*     */ u );
    DUF_OPTION_CASE_ACQUIRE_NUM( MINSIZE, /*            */ minsize, /*     */ u );

  case DUF_OPTION_SAME:
    r = duf_limits( optarg, &duf_config->u.minsame, &duf_config->u.maxsame );
    break;
    DUF_OPTION_CASE_ACQUIRE_NUM( MAXSAME, /*            */ maxsame, /*     */ u );
    DUF_OPTION_CASE_ACQUIRE_NUM( MINSAME, /*            */ minsame, /*     */ u );

    DUF_OPTION_CASE_ACQUIRE_NUM( MAXDIRFILES, /*        */ maxdirfiles, /*     */ u );
    DUF_OPTION_CASE_ACQUIRE_NUM( MINDIRFILES, /*        */ mindirfiles, /*     */ u );

    DUF_OPTION_CASE_ACQUIRE_NUM( MAXSEQ, /*             */ maxseq, /*     */ u );
    DUF_OPTION_CASE_ACQUIRE_NUM( MAXDEPTH, /*           */ maxreldepth, /*     */ u );

    DUF_OPTION_CASE_ACQUIRE_NUM( MAXITEMS, /*           */ maxitems.total, /*     */ u );
    DUF_OPTION_CASE_ACQUIRE_NUM( MAXITEMS_FILES, /*     */ maxitems.files, /*     */ u );
    DUF_OPTION_CASE_ACQUIRE_NUM( MAXITEMS_DIRS, /*      */ maxitems.dirs, /*     */ u );

    DUF_OPTION_CASE_ACQUIRE_FLAG( UNI_SCAN, /*          */ uni_scan, /*     */ cli.act );

    DUF_OPTION_CASE_ACQUIRE_ARG( GLOB_INCLUDE_FILES, /* */ glob.include_files, /*     */ u );
    DUF_OPTION_CASE_ACQUIRE_ARG( GLOB_EXCLUDE_FILES, /* */ glob.exclude_files, /*     */ u );


/* trace */
    DUF_OPTION_CASE_ACQUIRE_NUM( TRACE_NONEW, /*        */ nonew, /*     */ cli.trace );

    DUF_OPTION_CASE_ACQUIRE_TRACE( SEQ, /*              */ seq );
    DUF_OPTION_CASE_ACQUIRE_TRACE( CALLS, /*            */ calls );
    DUF_OPTION_CASE_ACQUIRE_TRACE( ANY, /*              */ any );
    DUF_OPTION_CASE_ACQUIRE_TRACE( CURRENT, /*          */ current );
    DUF_OPTION_CASE_ACQUIRE_TRACE( ACTION, /*           */ action );
    DUF_OPTION_CASE_ACQUIRE_TRACE( ERROR, /*            */ error );
    DUF_OPTION_CASE_ACQUIRE_TRACE( SCAN, /*             */ scan );
    DUF_OPTION_CASE_ACQUIRE_TRACE( PATH, /*             */ path );
    DUF_OPTION_CASE_ACQUIRE_TRACE( FS, /*               */ fs );
    DUF_OPTION_CASE_ACQUIRE_TRACE( DELETED, /*          */ deleted );
    DUF_OPTION_CASE_ACQUIRE_TRACE( SAMPUPD, /*          */ sampupd );
    DUF_OPTION_CASE_ACQUIRE_TRACE( SAMPLE, /*           */ sample );
    DUF_OPTION_CASE_ACQUIRE_TRACE( MDPATH, /*           */ mdpath );
    DUF_OPTION_CASE_ACQUIRE_TRACE( DIRENT, /*           */ dirent );
    DUF_OPTION_CASE_ACQUIRE_TRACE( SD5, /*              */ sd5 );
    DUF_OPTION_CASE_ACQUIRE_TRACE( MD5, /*              */ md5 );
    DUF_OPTION_CASE_ACQUIRE_TRACE( CRC32, /*            */ crc32 );
    DUF_OPTION_CASE_ACQUIRE_TRACE( MIME, /*             */ mime );
    DUF_OPTION_CASE_ACQUIRE_TRACE( COLLECT, /*          */ collect );
    DUF_OPTION_CASE_ACQUIRE_TRACE( INTEGRITY, /*        */ integrity );
    DUF_OPTION_CASE_ACQUIRE_TRACE( SQL, /*              */ sql );
    DUF_OPTION_CASE_ACQUIRE_TRACE( SELECT, /*           */ select );
    DUF_OPTION_CASE_ACQUIRE_TRACE( INSERT, /*           */ insert );
    DUF_OPTION_CASE_ACQUIRE_TRACE( UPDATE, /*           */ update );

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
    break;
  case DUF_OPTION_TRACE_FILE:
    r = duf_open_special( optarg, &duf_config->cli.trace.file, &duf_config->cli.trace.out );
    break;

  case DUF_OPTION_OUTPUT_FILE:
    r = duf_open_special( optarg, &duf_config->cli.output.file, &duf_config->cli.output.out );
    break;

/* combined */
  case DUF_OPTION_ALL_TRACE:
    if ( optarg && *optarg )
      duf_config->cli.trace.sql = duf_config->cli.trace.select = duf_config->cli.trace.insert = duf_config->cli.trace.update =
            duf_config->cli.trace.collect = duf_config->cli.trace.dirent = duf_config->cli.trace.sd5 = duf_config->cli.trace.md5 =
            duf_config->cli.trace.crc32 = duf_config->cli.trace.mime = duf_config->cli.trace.sample = duf_config->cli.trace.deleted =
            duf_config->cli.trace.scan = strtol( optarg, NULL, 10 );
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
      duf_config->cli.trace.dirent++;
      duf_config->cli.trace.sample++;
      duf_config->cli.trace.deleted++;
      duf_config->cli.trace.scan++;
    }
    break;



  case DUF_OPTION_FLAG_ZERO_DB:
    DUF_OPTION_ACQUIRE_FLAG( cli.act.create_tables );
    duf_config->cli.act.create_tables = 1;

  case DUF_OPTION_TREE_TO_DB:
    /* -ORifd5 
     * i.e.
     *  --create-tables --uni-scan --recursive ...
     *  */
    duf_config->cli.act.create_tables = duf_config->cli.act.add_path = duf_config->cli.act.uni_scan = duf_config->u.recursive =
          duf_config->cli.act.files = duf_config->cli.act.dirs = duf_config->cli.act.dirent =
          duf_config->cli.act.sd5 = duf_config->cli.act.md5 = duf_config->cli.act.crc32 = duf_config->cli.act.filedata =
          duf_config->cli.act.filenames = 1;
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
      /*       duf_config->cli.format.filesize = (* *) */
      /*       duf_config->cli.format.mtime = (* *)    */
      /*       duf_config->cli.format.filename = (* *) */
      /*       duf_config->cli.format.md5 = (* *)      */
      /*       duf_config->cli.format.md5id = (* *)    */
      /*       duf_config->cli.format.nsame = 0;       */


      char *const tokens[] = {
        [DUF_FORMAT_DATAID] = "dataid",
        [DUF_FORMAT_DIRID] = "dirid",
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
        [DUF_FORMAT_MODE] = "mode",
        [DUF_FORMAT_MTIME] = "mtime",
        [DUF_FORMAT_NLINK] = "nlink",
        [DUF_FORMAT_NSAME] = "nsame",
        [DUF_FORMAT_OFFSET] = "offset",
        [DUF_FORMAT_PREFIX] = "prefix",
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
        case DUF_FORMAT_DIRID:
          duf_config->cli.format.dirid = value == NULL ? 1 : nvalue;
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


  case ':':
    r = DUF_ERROR_OPTION_VALUE;
    break;
  case '?':
    r = DUF_ERROR_OPTION;
    break;
  }
  if ( r )
  {
    char *ona = NULL;

    ona = duf_option_description( r );
    DUF_ERROR( "returns: %d = ( %s )", r, ona );
    mas_free( ona );
  }

  DUF_TEST_R( r );
  return r;
}
