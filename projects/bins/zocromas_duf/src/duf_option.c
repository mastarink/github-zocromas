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

/* ###################################################################### */
#include "duf_option.h"
/* ###################################################################### */
#define DUF_OPT_NUM(code)  \
      if ( optarg && *optarg ) \
        duf_config->code = duf_strtol( optarg, &r); \

#define DUF_OPT_NUM_PLUS(code)  \
      if ( optarg && *optarg ) \
        duf_config->code = duf_strtol( optarg, &r); \
      else \
        duf_config->code++;

#define DUF_OPT_STR(code)  \
      if ( optarg ) \
      { \
        mas_free( duf_config->code ); \
        duf_config->code = mas_strdup( optarg ); \
      }
#define DUF_OPT_ARG(code)  \
      if ( optarg ) duf_config->code.argc=mas_add_argv_arg(duf_config->code.argc, &duf_config->code.argv, optarg)

#define DUF_OPT_FLAG(code)  \
    duf_config->code = 1;

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
  case DUF_OPTION_EXAMPLES:
    r = opt;
    break;
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
        [DUF_FORMAT_MD5ID] = "md5id",
        [DUF_FORMAT_MD5] = "md5",
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
        case DUF_FORMAT_MD5:
          duf_config->cli.format.md5 = value == NULL ? 1 : nvalue;
          break;
        case DUF_FORMAT_MD5ID:
          duf_config->cli.format.md5id = value == NULL ? 1 : nvalue;
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
  case DUF_OPTION_VERBOSE:
    DUF_OPT_NUM_PLUS( cli.dbg.verbose );
    break;
  case DUF_OPTION_DEBUG:
    DUF_OPT_NUM_PLUS( cli.dbg.debug );
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
  case DUF_OPTION_OUTPUT:
    DUF_OPT_NUM_PLUS( cli.output.level );
    break;
  case DUF_OPTION_OUTPUT_FILE:
    r = duf_open_special( optarg, &duf_config->cli.output.file, &duf_config->cli.output.out );
    break;
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
  case DUF_OPTION_ALL_TRACE:
    if ( optarg && *optarg )
      duf_config->cli.trace.sql = duf_config->cli.trace.collect = duf_config->cli.trace.dirent = duf_config->cli.trace.md5 =
            duf_config->cli.trace.mime = duf_config->cli.trace.sample = duf_config->cli.trace.deleted = duf_config->cli.trace.scan =
            strtol( optarg, NULL, 10 );
    else
    {
      duf_config->cli.trace.sql++;
      duf_config->cli.trace.collect++;
      duf_config->cli.trace.md5++;
      duf_config->cli.trace.mime++;
      duf_config->cli.trace.dirent++;
      duf_config->cli.trace.sample++;
      duf_config->cli.trace.deleted++;
      duf_config->cli.trace.scan++;
    }
    break;
  case DUF_OPTION_TRACE_NONEW:
    DUF_OPT_NUM_PLUS( cli.trace.nonew );
    break;
  case DUF_OPTION_NOOPENAT:
    DUF_OPT_FLAG( cli.noopenat );
    break;
  case DUF_OPTION_SEQ_TRACE:
    DUF_OPT_NUM_PLUS( cli.trace.seq );
    break;
  case DUF_OPTION_CALLS_TRACE:
    DUF_OPT_NUM_PLUS( cli.trace.calls );
    break;
  case DUF_OPTION_ANY_TRACE:
    DUF_OPT_NUM_PLUS( cli.trace.any );
    break;
  case DUF_OPTION_CURRENT_TRACE:
    DUF_OPT_NUM_PLUS( cli.trace.current );
    break;
  case DUF_OPTION_ACTION_TRACE:
    DUF_OPT_NUM_PLUS( cli.trace.action );
    break;
  case DUF_OPTION_ERROR_TRACE:
    DUF_OPT_NUM_PLUS( cli.trace.error );
    break;
  case DUF_OPTION_SCAN_TRACE:
    DUF_OPT_NUM_PLUS( cli.trace.scan );
    break;
  case DUF_OPTION_PATH_TRACE:
    DUF_OPT_NUM_PLUS( cli.trace.path );
    break;
  case DUF_OPTION_FS_TRACE:
    DUF_OPT_NUM_PLUS( cli.trace.fs );
    break;
  case DUF_OPTION_DELETED_TRACE:
    DUF_OPT_NUM_PLUS( cli.trace.deleted );
    break;
  case DUF_OPTION_SAMPUPD_TRACE:
    DUF_OPT_NUM_PLUS( cli.trace.sampupd );
    break;
  case DUF_OPTION_SAMPLE_TRACE:
    DUF_OPT_NUM_PLUS( cli.trace.sample );
    break;
  case DUF_OPTION_MATCH_TRACE:
    DUF_OPT_NUM_PLUS( cli.trace.match );
    break;
  case DUF_OPTION_MDPATH_TRACE:
    DUF_OPT_NUM_PLUS( cli.trace.mdpath );
    break;
  case DUF_OPTION_DIRENT_TRACE:
    DUF_OPT_NUM_PLUS( cli.trace.dirent );
    break;
  case DUF_OPTION_MD5_TRACE:
    DUF_OPT_NUM_PLUS( cli.trace.md5 );
    break;
  case DUF_OPTION_MIME_TRACE:
    DUF_OPT_NUM_PLUS( cli.trace.mime );
    break;
  case DUF_OPTION_COLLECT_TRACE:
    DUF_OPT_NUM_PLUS( cli.trace.collect );
    break;
  case DUF_OPTION_INTEGRITY_TRACE:
    DUF_OPT_NUM_PLUS( cli.trace.integrity );
    break;
  case DUF_OPTION_SQL_TRACE:
    DUF_OPT_NUM_PLUS( cli.trace.sql );
    break;
  case DUF_OPTION_MIN_DBGLINE:
    DUF_OPT_NUM( cli.dbg.min_line );
    break;
  case DUF_OPTION_MAX_DBGLINE:
    DUF_OPT_NUM( cli.dbg.max_line );
  case DUF_OPTION_TOTALS:
    DUF_OPT_FLAG( cli.act.totals );
    break;
  case DUF_OPTION_TREE_TO_DB:
    /* -ORifd5 
     * i.e.
     *  --create-tables --uni-scan --recursive --collect --dirent --files --dirs --md5 */
    duf_config->cli.act.create_tables = duf_config->cli.act.add_path = duf_config->cli.act.uni_scan = duf_config->u.recursive =
          duf_config->cli.act.collect = duf_config->cli.act.files = duf_config->cli.act.dirs =
          duf_config->cli.act.dirent = duf_config->cli.act.md5 = 1;
    break;
  case DUF_OPTION_ZERO_DB:
    DUF_OPT_FLAG( cli.act.create_tables );
    duf_config->cli.act.create_tables = 1;
  case DUF_OPTION_REMOVE_DATABASE:
    DUF_OPT_FLAG( cli.act.remove_database );
    break;
  case DUF_OPTION_DROP_TABLES:
    DUF_OPT_FLAG( cli.act.drop_tables );
    break;
  case DUF_OPTION_CREATE_TABLES:
    DUF_OPT_FLAG( cli.act.create_tables );
    break;
  case DUF_OPTION_ADD_PATH:
    DUF_OPT_FLAG( cli.act.add_path );
    break;
  case DUF_OPTION_ZERO_FILEDATA:
    DUF_OPT_FLAG( cli.act.zero_filedata );
    break;
  case DUF_OPTION_UPDATE_FILEDATA:
    DUF_OPT_FLAG( cli.act.update_filedata );
    break;
  case DUF_OPTION_UPDATE_EXIF:
    DUF_OPT_FLAG( cli.act.update_exif );
    break;
  case DUF_OPTION_RECURSIVE:
    DUF_OPT_FLAG( u.recursive );
    break;
  case DUF_OPTION_VACUUM:
    DUF_OPT_FLAG( cli.act.vacuum );
    break;
  case DUF_OPTION_SAMPLE:
    DUF_OPT_NUM_PLUS( cli.act.sample );
    break;
  case DUF_OPTION_SAMPUPD:
    DUF_OPT_FLAG( cli.act.sampupd );
    break;
  case DUF_OPTION_MDPATH:
    DUF_OPT_FLAG( cli.act.mdpath );
    break;
  case DUF_OPTION_DIRENT:
    DUF_OPT_FLAG( cli.act.dirent );
    break;
  case DUF_OPTION_MD5:
    DUF_OPT_FLAG( cli.act.md5 );
    break;
  case DUF_OPTION_MIME:
    DUF_OPT_FLAG( cli.act.mime );
    break;
  case DUF_OPTION_FILL:
    DUF_OPT_FLAG( cli.act.dirent );
    DUF_OPT_FLAG( cli.act.collect );
    break;
  case DUF_OPTION_COLLECT:
    DUF_OPT_FLAG( cli.act.collect );
    break;
  case DUF_OPTION_INTEGRITY:
    DUF_OPT_FLAG( cli.act.integrity );
    break;
  case DUF_OPTION_PRINT:
    DUF_OPT_FLAG( cli.act.print );
    break;
  case DUF_OPTION_TREE:
    DUF_OPT_FLAG( cli.act.tree );
    break;
  case DUF_OPTION_FILES:
    DUF_OPT_FLAG( cli.act.files );
    break;
  case DUF_OPTION_DISABLE_INSERT:
    DUF_OPT_FLAG( cli.disable.insert );
    break;
  case DUF_OPTION_DISABLE_UPDATE:
    DUF_OPT_FLAG( cli.disable.update );
    break;
  case DUF_OPTION_DIRS:
    DUF_OPT_FLAG( cli.act.dirs );
    break;
  case DUF_OPTION_MD5ID:
    DUF_OPT_NUM( u.md5id );
    DUF_ERROR( "%lld", duf_config->u.md5id );
    break;
  case DUF_OPTION_MIMEID:
    DUF_OPT_NUM( u.mimeid );
    DUF_ERROR( "%lld", duf_config->u.mimeid );
    break;
  case DUF_OPTION_SIZE:
    r = duf_limits( optarg, &duf_config->u.minsize, &duf_config->u.maxsize );
    break;
  case DUF_OPTION_MAXSIZE:
    DUF_OPT_NUM( u.maxsize );
    break;
  case DUF_OPTION_MINSIZE:
    DUF_OPT_NUM( u.minsize );
    break;
  case DUF_OPTION_SAME:
    r = duf_limits( optarg, &duf_config->u.minsame, &duf_config->u.maxsame );
    break;
  case DUF_OPTION_MAXSAME:
    DUF_OPT_NUM( u.maxsame );
    DUF_ERROR( "%lld - %lld", duf_config->u.minsame, duf_config->u.maxsame );
    break;
  case DUF_OPTION_MINSAME:
    DUF_OPT_NUM( u.minsame );
    DUF_ERROR( "%lld - %lld", duf_config->u.minsame, duf_config->u.maxsame );
    break;
  case DUF_OPTION_MAXDIRFILES:
    DUF_OPT_NUM( u.maxdirfiles );
    break;
  case DUF_OPTION_MINDIRFILES:
    DUF_OPT_NUM( u.mindirfiles );
    break;
  case DUF_OPTION_MAXDEPTH:
    DUF_OPT_NUM( u.maxreldepth );
    break;
  case DUF_OPTION_MAXITEMS:
    DUF_OPT_NUM( u.maxitems.total );
    break;
  case DUF_OPTION_MAXITEMS_FILES:
    DUF_OPT_NUM( u.maxitems.files );
    break;
  case DUF_OPTION_MAXITEMS_DIRS:
    DUF_OPT_NUM( u.maxitems.dirs );
    break;
  case DUF_OPTION_MAXSEQ:
    DUF_OPT_NUM( u.maxseq );
    break;
  case DUF_OPTION_UNI_SCAN:
    DUF_OPT_FLAG( cli.act.uni_scan );
    break;
    /* case DUF_OPTION_PRINT_PATHS:   */
    /*   duf_config->cli.act.print_paths = 1; */
    /*   break;                       */
    /* case DUF_OPTION_PRINT_DIRS:    */
    /*   duf_config->cli.act.print_dirs = 1;  */
    /*   break;                       */
    /* case DUF_OPTION_PRINT_FILES:   */
    /*   duf_config->cli.act.print_files = 1; */
    /*   break;                       */
    /* case DUF_OPTION_PRINT_DUPLICATES:   */
    /*   duf_config->cli.act.print_duplicates = 1; */
    /*   break;                            */
    /* case DUF_OPTION_ZERO_DUPLICATES:           */
    /*   DUF_OPT_FLAG( cli.act.zero_duplicates ); */
    /*   break;                                   */
  case DUF_OPTION_SAME_FILES:
    DUF_OPT_FLAG( cli.act.same_files );
    break;
  case DUF_OPTION_SAME_EXIF:
    DUF_OPT_FLAG( cli.act.same_exif );
    break;
  case DUF_OPTION_SAME_MD5:
    DUF_OPT_FLAG( cli.act.same_md5 );
    break;
  case DUF_OPTION_ADD_TO_GROUP:
    DUF_OPT_FLAG( cli.act.to_group );
    break;
  case DUF_OPTION_REMOVE_FROM_GROUP:
    DUF_OPT_FLAG( cli.act.from_group );
    break;
  case DUF_OPTION_GLOB_INCLUDE_FILES:
    DUF_OPT_ARG( u.glob.include_files );
    break;
  case DUF_OPTION_GLOB_EXCLUDE_FILES:
    DUF_OPT_ARG( u.glob.exclude_files );
    break;
  case DUF_OPTION_GROUP:
    DUF_OPT_STR( group );
    break;
  case DUF_OPTION_DB_DIRECTORY:
    DUF_OPT_STR( db.dir );
    break;
  case DUF_OPTION_DB_NAME:
    DUF_OPT_STR( db.name );
    break;
    /* case DUF_OPTION_LIMIT:      */
    /*   DUF_OPT_NUM( cli.limit ); */
    /*   break;                    */
  case '?':
    r = DUF_ERROR_OPTION;
    break;
  }
  DUF_TEST_R( r );
  return r;
}
