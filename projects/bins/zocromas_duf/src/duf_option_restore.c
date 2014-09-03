#include <assert.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"


#include "duf_option_extended.h"

/* ###################################################################### */
#include "duf_option_restore.h"
/* ###################################################################### */



static int
_duf_restore_option_i( char *ptr, duf_option_code_t codeval, int value, int maxlen )
{
  for ( int ilong = 0; ilong < lo_extended_count; ilong++ )
  {
    if ( duf_config->longopts_table[ilong].val == codeval )
    {
      snprintf( ptr, maxlen, " --%s='%d'", duf_config->longopts_table[ilong].name, value );
      break;
    }
  }
  return 0;
}

static int
_duf_restore_option_s( char *ptr, duf_option_code_t codeval, const char *value, int maxlen )
{
  if ( value )
    for ( int ilong = 0; ilong < lo_extended_count; ilong++ )
    {
      if ( duf_config->longopts_table[ilong].val == codeval )
      {
        snprintf( ptr, maxlen, " --%s='%s'", duf_config->longopts_table[ilong].name, value );
        break;
      }
    }
  return 0;
}

static int
_duf_restore_option_b( char *ptr, duf_option_code_t codeval, int value, int maxlen )
{
  if ( value )
    for ( int ilong = 0; ilong < lo_extended_count; ilong++ )
    {
      if ( duf_config->longopts_table[ilong].val == codeval )
      {
        snprintf( ptr, maxlen, " --%s", duf_config->longopts_table[ilong].name );
        break;
      }
    }
  return 0;
}

void
duf_restore_some_option( char *ptr, duf_option_code_t codeval, size_t maxlen )
{
  /* most probably not all options mentioned here - FIXME */
  DUF_OPTION_RESTORE_NUM( codeval, ptr, VERBOSE, verbose, cli.dbg, maxlen );
  DUF_OPTION_RESTORE_NUM( codeval, ptr, DEBUG, debug, cli.dbg, maxlen );
  /* DUF_OPTION_RESTORE_TRACE(ptr, ALL, all ); */
  DUF_OPTION_RESTORE_NUM( codeval, ptr, TRACE_NONEW, nonew, cli.trace, maxlen );
  DUF_OPTION_RESTORE_NUM( codeval, ptr, OUTPUT_LEVEL, level, cli.output, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, DRY_RUN, dry_run, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, EXPLAIN, explain, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, SEQ, seq, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, OPTIONS, options, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, CALLS, calls, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, ANY, any, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, CURRENT, current, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, ACTION, action, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, ERROR, error, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, SCAN, scan, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, SCAN_DE_DIR, scan_de_dir, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, SCAN_DE_REG, scan_de_reg, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, TEMP, temp, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, PATH, path, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, FS, fs, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, SAMPUPD, sampupd, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, SAMPLE, sample, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, DELETED, deleted, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, MDPATH, mdpath, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, DIRENT, dirent, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, MD5, md5, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, SD5, sd5, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, CRC32, crc32, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, MIME, mime, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, EXIF, exif, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, COLLECT, collect, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, INTEGRITY, integrity, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, SQL, sql, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, SELECT, select, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, INSERT, insert, maxlen );
  DUF_OPTION_RESTORE_TRACE( codeval, ptr, UPDATE, update, maxlen );
  DUF_OPTION_RESTORE_NUM( codeval, ptr, MIN_DBGLINE, min_line, cli.dbg, maxlen );
  DUF_OPTION_RESTORE_NUM( codeval, ptr, MAX_DBGLINE, max_line, cli.dbg, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, PROGRESS, progress, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, SUMMARY, summary, cli.act, maxlen );
  /* DUF_OPTION_TREE_TO_DB: */
  /* DUF_OPTION_ZERO_DB: */
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, DROP_TABLES, drop_tables, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, CREATE_TABLES, create_tables, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, ADD_PATH, add_path, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, UNI_SCAN, uni_scan, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, INFO, info, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, RECURSIVE, recursive, u, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, DRY_RUN, dry_run, cli, maxlen );
  DUF_OPTION_RESTORE_NUM( codeval, ptr, SAMPLE, sample, cli.act, maxlen );
  DUF_OPTION_RESTORE_NUM( codeval, ptr, SAMPUPD, sampupd, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, MDPATH, mdpath, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, DIRENT, dirent, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, MD5, md5, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, SD5, sd5, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, CRC32, crc32, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, MIME, mime, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, EXIF, exif, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, COLLECT, collect, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, INTEGRITY, integrity, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, PRINT, print, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, TREE, tree, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, FILES, files, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAG( codeval, ptr, DIRS, dirs, cli.act, maxlen );
  DUF_OPTION_RESTORE_FLAGG( codeval, ptr, DISABLE_CALCULATE, calculate, cli,.disable, maxlen );
  DUF_OPTION_RESTORE_FLAGG( codeval, ptr, DISABLE_INSERT, insert, cli,.disable, maxlen );
  DUF_OPTION_RESTORE_FLAGG( codeval, ptr, DISABLE_UPDATE, update, cli,.disable, maxlen );
  DUF_OPTION_RESTORE_NUM( codeval, ptr, MAXSEQ, max_seq, u, maxlen );
  DUF_OPTION_RESTORE_NUM( codeval, ptr, MINSIZE, size.min, u, maxlen );
  DUF_OPTION_RESTORE_NUM( codeval, ptr, MAXSIZE, size.max, u, maxlen );
  DUF_OPTION_RESTORE_NUM( codeval, ptr, MINDIRFILES, dirfiles.min, u, maxlen );
  DUF_OPTION_RESTORE_NUM( codeval, ptr, MAXDIRFILES, dirfiles.max, u, maxlen );
  DUF_OPTION_RESTORE_NUM( codeval, ptr, MAXITEMS, maxitems.total, u, maxlen );
  DUF_OPTION_RESTORE_NUM( codeval, ptr, MAXITEMS_FILES, maxitems.files, u, maxlen );
  DUF_OPTION_RESTORE_NUM( codeval, ptr, MAXITEMS_DIRS, maxitems.dirs, u, maxlen );
  /* DUF_OPTION_RESTORE_TRACE(ptr, TRACE_STDERR, nonew, cli.trace ); */
  /* DUF_OPTION_RESTORE_TRACE(ptr, TRACE_STDOUT, nonew, cli.trace ); */
  DUF_OPTION_RESTOREV_B( codeval, ptr, TRACE_STDERR, out, cli.trace, duf_config->cli.trace.out == stderr, maxlen );
  DUF_OPTION_RESTOREV_B( codeval, ptr, TRACE_STDOUT, out, cli.trace, duf_config->cli.trace.out == stdout, maxlen );
  DUF_OPTION_RESTORE_STR( codeval, ptr, OUTPUT_FILE, file, cli.output, maxlen );
  DUF_OPTION_RESTORE_STR( codeval, ptr, TRACE_FILE, file, cli.trace, maxlen );
  DUF_OPTION_RESTORE_STR( codeval, ptr, DB_DIRECTORY, dir, db, maxlen );
  DUF_OPTION_RESTORE_STR( codeval, ptr, DB_NAME_ADM, name, db.adm, maxlen );
  DUF_OPTION_RESTORE_STR( codeval, ptr, DB_NAME_MAIN, name, db.main, maxlen );
  /* DUF_OPTION_RESTORE_STR( FILE, file, cli.trace ); */
}



char *
duf_restore_some_options( const char *a0 )
{
  char *str;

  str = mas_strdup( a0 );
  for ( duf_option_code_t codeval = DUF_OPTION_NONE; codeval < DUF_OPTION_MAX_LONG; codeval++ )
  {
#define BUFSZ 1024 * 4
    char buf[BUFSZ] = "";

    duf_restore_some_option( buf, codeval, BUFSZ );
    if ( *buf )
    {
      /* str = mas_strcat_x( str, " @[ " ); */
      str = mas_strcat_x( str, buf );
      /* str = mas_strcat_x( str, " ]@ " ); */
    }
  }
  str = mas_strcat_x( str, " --" );
  return str;
}
