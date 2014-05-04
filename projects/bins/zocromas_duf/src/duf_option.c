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
        duf_config->code = duf_strtol( optarg); \

#define DUF_OPT_NUM_PLUS(code)  \
      if ( optarg && *optarg ) \
        duf_config->code = duf_strtol( optarg); \
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

long
duf_strtol( const char *s )
{
  long l = 0;
  char *pe = NULL;

  l = strtol( s, &pe, 10 );
  if ( pe && *pe == 'k' )
    l *= 1024;
  else if ( pe && *pe == 'M' )
    l *= 1024 * 1024;
  else if ( pe && *pe == 'G' )
    l *= 1024 * 1024 * 1024;
  else if ( pe && *pe == 'w' )
    l *= 2;
  else if ( pe && *pe == 'c' )
    l *= 1;
  else if ( pe && *pe == 'b' )
    l *= 512;
  return l;
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
      int r = 0;
      char *coptarg, *poptarg;
      unsigned nvalue;
      char *value;

      char *const tokens[] = {
        [DUF_FORMAT_SEQ] = "seq",
        [DUF_FORMAT_OFFSET] = "offset",
        [DUF_FORMAT_DIRID] = "dirid",
        [DUF_FORMAT_INODE] = "inode",
        [DUF_FORMAT_MODE] = "mode",
        [DUF_FORMAT_NLINK] = "nlink",
        [DUF_FORMAT_UID] = "uid",
        [DUF_FORMAT_GID] = "gid",
        [DUF_FORMAT_FILESIZE] = "filesize",
        [DUF_FORMAT_MTIME] = "mtime",
        [DUF_FORMAT_FILENAME] = "filename",
        [DUF_FORMAT_MD5] = "md5",
        [DUF_FORMAT_NSAME] = "nsame",
        [DUF_FORMAT_MAX] = NULL,
      };
      poptarg = coptarg = mas_strdup( optarg );
      /* coptarg = mas_strdup( optarg ); */
      value = NULL;
      DUF_TRACE( action, 0, "--format=%s", coptarg );
      while ( poptarg && *poptarg )
      {
        /* char *hlp;                          */
        /*                                     */
        /* hlp = poptarg;                      */
        /* DUF_TRACE( any, 0, "hlp:%s", hlp ); */
        r = getsubopt( &poptarg, tokens, &value );
        /* DUF_TRACE( any, 0, "%d: (%s) '%s'", r, hlp, value ? value : "nil" ); */
        nvalue = value ? strtol( value, NULL, 10 ) : 0;
        switch ( r )
        {
        case DUF_FORMAT_SEQ:
          duf_config->cli.format.seq = 1;
          break;
        case DUF_FORMAT_DIRID:
          duf_config->cli.format.dirid = 1;
          break;
        case DUF_FORMAT_INODE:
          duf_config->cli.format.inode = 1;
          break;
        case DUF_FORMAT_MODE:
          duf_config->cli.format.mode = 1;
          break;
        case DUF_FORMAT_NLINK:
          duf_config->cli.format.nlink = 1;
          break;
        case DUF_FORMAT_UID:
          duf_config->cli.format.uid = 1;
          break;
        case DUF_FORMAT_GID:
          duf_config->cli.format.gid = 1;
          break;
        case DUF_FORMAT_FILESIZE:
          duf_config->cli.format.filesize = 1;
          break;
        case DUF_FORMAT_MTIME:
          duf_config->cli.format.mtime = 1;
          break;
        case DUF_FORMAT_FILENAME:
          duf_config->cli.format.filename = 1;
          break;
        case DUF_FORMAT_MD5:
          duf_config->cli.format.md5 = 1;
          break;
        case DUF_FORMAT_NSAME:
          duf_config->cli.format.nsame = 1;
          break;
        case DUF_FORMAT_OFFSET:
          duf_config->cli.format.offset = nvalue;
          break;
        }
        /* DUF_TRACE( any, 0, "r:%d", r ); */
        if ( r < 0 )
          break;
      }
      mas_free( coptarg );
    }
    break;
  case DUF_OPTION_VERBOSE:
    DUF_OPT_NUM_PLUS( cli.dbg.verbose );
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
    if ( optarg )
    {
      struct stat st;

      if ( duf_config->cli.output.file )
        mas_free( duf_config->cli.output.file );
      duf_config->cli.output.file = NULL;
      duf_config->cli.output.file = mas_strdup( optarg );
      if ( 0 == stat( optarg, &st ) && ( !S_ISCHR( st.st_mode ) || !( st.st_mode & S_IWUSR ) ) )
      {
        DUF_ERROR( "Can't open trace file %s - file exists %llu / %llu chr:%d\n", optarg, ( unsigned long long ) st.st_dev,
                   ( unsigned long long ) st.st_rdev, S_ISCHR( st.st_mode ) );
        exit( 4 );
      }
      else
      {
        FILE *out;

        if ( duf_config->cli.output.out )
        {
          if ( duf_config->cli.output.out != stderr && duf_config->cli.output.out != stdout )
            fclose( duf_config->cli.output.out );
          duf_config->cli.output.out = NULL;
        }

        out = fopen( duf_config->cli.output.file, "w" );
        if ( !out )
        {
          DUF_ERROR( "Can't open trace file %s\n", optarg );
          exit( 4 );
        }
        else
          duf_config->cli.output.out = out;
      }
    }
    break;
  case DUF_OPTION_DEBUG:
    duf_config->cli.dbg.debug = 1;
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
    if ( optarg )
    {
      struct stat st;

      if ( duf_config->cli.trace.file )
        mas_free( duf_config->cli.trace.file );
      duf_config->cli.trace.file = NULL;
      duf_config->cli.trace.file = mas_strdup( optarg );
      if ( 0 == stat( optarg, &st ) && ( !S_ISCHR( st.st_mode ) || !( st.st_mode & S_IWUSR ) ) )
      {
        DUF_ERROR( "Can't open trace file %s - file exists %llu / %llu chr:%d\n", optarg, ( unsigned long long ) st.st_dev,
                   ( unsigned long long ) st.st_rdev, S_ISCHR( st.st_mode ) );
        exit( 4 );
      }
      else
      {
        FILE *out;

        if ( duf_config->cli.trace.out )
        {
          if ( duf_config->cli.trace.out != stderr && duf_config->cli.trace.out != stdout )
            fclose( duf_config->cli.trace.out );
          duf_config->cli.trace.out = NULL;
        }

        out = fopen( optarg, "w" );
        if ( !out )
        {
          DUF_ERROR( "Can't open trace file %s\n", optarg );
          exit( 4 );
        }
        else
          duf_config->cli.trace.out = out;
      }
    }
    break;
  case DUF_OPTION_ALL_TRACE:
    if ( optarg && *optarg )
      duf_config->cli.trace.sql = duf_config->cli.trace.collect = duf_config->cli.trace.dirent =
            duf_config->cli.trace.md5 = duf_config->cli.trace.sample = duf_config->cli.trace.scan = strtol( optarg, NULL, 10 );
    else
    {
      duf_config->cli.trace.sql++;
      duf_config->cli.trace.collect++;
      duf_config->cli.trace.md5++;
      duf_config->cli.trace.dirent++;
      duf_config->cli.trace.sample++;
      duf_config->cli.trace.scan++;
    }
    break;
  case DUF_OPTION_TRACE_NONEW:
    DUF_OPT_NUM_PLUS( cli.trace.nonew );
    /* if ( optarg && *optarg )                                    */
    /*   duf_config->cli.trace.nonew = strtol( optarg, NULL, 10 ); */
    /* else                                                        */
    /*   duf_config->cli.trace.nonew++;                            */
    break;
  case DUF_OPTION_NOOPENAT:
    DUF_OPT_FLAG( cli.noopenat );
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
    /* if ( optarg && *optarg )                                     */
    /*   duf_config->cli.dbg.min_line = strtol( optarg, NULL, 10 ); */
    break;
  case DUF_OPTION_MAX_DBGLINE:
    DUF_OPT_NUM( cli.dbg.max_line );
    /* if ( optarg && *optarg )                                     */
    /*   duf_config->cli.dbg.max_line = strtol( optarg, NULL, 10 ); */
  case DUF_OPTION_TOTALS:
    DUF_OPT_FLAG( cli.act.totals );
    /* if ( optarg && *optarg )                               */
    /*   duf_config->cli.totals = strtol( optarg, NULL, 10 ); */
    /* else                                                   */
    /*   duf_config->cli.totals++;                            */
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
    /* --drop-tables --create-tables */
    duf_config->cli.act.create_tables = 1;
  case DUF_OPTION_REMOVE_DATABASE:
    DUF_OPT_FLAG( cli.act.remove_database );
    break;
  case DUF_OPTION_DROP_TABLES:
    DUF_OPT_FLAG( cli.act.drop_tables );
    break;
  case DUF_OPTION_CREATE_TABLES:
    DUF_OPT_FLAG( cli.act.create_tables );
    /* duf_config->cli.act.create_tables = 1; */
    break;
  case DUF_OPTION_ADD_PATH:
    DUF_OPT_FLAG( cli.act.add_path );
    /* duf_config->cli.act.add_path = 1; */
    break;
  case DUF_OPTION_ZERO_FILEDATA:
    DUF_OPT_FLAG( cli.act.zero_filedata );
    /* duf_config->cli.act.zero_filedata = 1; */
    break;
  case DUF_OPTION_UPDATE_FILEDATA:
    DUF_OPT_FLAG( cli.act.update_filedata );
    break;
  case DUF_OPTION_UPDATE_EXIF:
    DUF_OPT_FLAG( cli.act.update_exif );
    /* duf_config->cli.act.update_exif = 1; */
    break;
  case DUF_OPTION_RECURSIVE:
    DUF_OPT_FLAG( u.recursive );
    /* duf_config->u.recursive = 1; */
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
    /* DUF_OPT_FLAG( cli.act.integrity ); */
    break;
  case DUF_OPTION_DIRENT:
    DUF_OPT_FLAG( cli.act.dirent );
    break;
  case DUF_OPTION_MD5:
    DUF_OPT_FLAG( cli.act.md5 );
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
  case DUF_OPTION_DIRS:
    DUF_OPT_FLAG( cli.act.dirs );
    break;
  case DUF_OPTION_SIZE:
    {
      long n;

      if ( optarg )
      {
        const char *s = NULL;
        char c = 0;

        s = optarg;
        if ( *s == '+' )
          c = *s++;
        else if ( *s == '-' )
          c = *s++;
        n = duf_strtol( s );
        if ( c == '+' )
        {
          /* duf_config->u.maxsize = 0; */
          duf_config->u.minsize = n + 1;
        }
        else if ( c == '-' )
        {
          duf_config->u.maxsize = n - 1;
          /* duf_config->u.minsize = 0; */
        }
        else
        {
          duf_config->u.maxsize = n;
          duf_config->u.minsize = n;
        }
        DUF_ERROR( "[%c] %lld - %lld", c, duf_config->u.minsize, duf_config->u.maxsize );
      }
    }
    break;
  case DUF_OPTION_MAXSIZE:
    DUF_OPT_NUM( u.maxsize );
    break;
  case DUF_OPTION_MINSIZE:
    DUF_OPT_NUM( u.minsize );
    break;
  case DUF_OPTION_MAXSAME:
    DUF_OPT_NUM( u.maxsame );
    break;
  case DUF_OPTION_MINSAME:
    DUF_OPT_NUM( u.minsame );
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
  return r;
}
