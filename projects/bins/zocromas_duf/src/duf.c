#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_config_ref.h"
#include "duf_config.h"
#include "duf_utils.h"

#include "duf_cli_options.h"
#include "duf_options.h"

#include "duf_service.h"

#include "duf_dbg.h"

#include "duf_action.h"
#include "duf_maindb.h"

/* צאַצקע */
#include "duf_trace_defs.h"

/* ###################################################################### */
#include "duf.h"
/* ###################################################################### */

int
duf_test_help( int argc, char **argv, int opt )
{
  int r = 0;

  switch ( opt )
  {
  case DUF_OPTION_SMART_HELP:
    duf_option_smart_help(  );
    break;
  case DUF_OPTION_VERSION:
    {
      extern int __MAS_LINK_DATE__, __MAS_LINK_TIME__;
      char *sargv1, *sargv2;

      sargv1 = mas_argv_string( argc, argv, 1 );
      sargv2 = duf_restore_options( argv[0] );

      DUF_PRINTF( 0, "CFLAGS:          (%s)", MAS_CFLAGS );
      DUF_PRINTF( 0, "LDFLAGS:         (%s)", MAS_LDFLAGS );
      DUF_PRINTF( 0, "args:            (%s)", sargv1 );
      DUF_PRINTF( 0, "restored opts:   (%s)", sargv2 );
      DUF_PRINTF( 0, "configire        (%s)", MAS_CONFIG_ARGS );
      DUF_PRINTF( 0, "UUID             %s", MAS_UUID );
      DUF_PRINTF( 0, "cli.      [%2lu]   %x", sizeof( duf_config->cli.v.bit ), duf_config->cli.v.bit );
      DUF_PRINTF( 0, "u.        [%2lu]   %x", sizeof( duf_config->u.v.bit ), duf_config->u.v.bit );
      DUF_PRINTF( 0, "cli.act.  [%2lu]   %x", sizeof( duf_config->cli.act.v.bit ), duf_config->cli.act.v.bit );
      {
        unsigned u = duf_config->cli.act.v.bit;

        DUF_PRINTF( 0, ".cli.act.  [%2lu]   ", sizeof( duf_config->cli.act.v.bit ) );
        while ( u )
        {
          DUF_PRINTF( 0, ".%u", u & 0x80000000 ? 1 : 0 );
          u <<= 1;
        }
        DUF_PUTSL( 0 );
      }
      DUF_PRINTF( 0, "prefix    [%2lu]   %s", sizeof( MAS_CONFIG_PREFIX ), MAS_CONFIG_PREFIX );
      DUF_PRINTF( 0, "O.        [%2lu]  %s", sizeof( MAS_OSVER ), MAS_OSVER );
      DUF_PRINTF( 0, "U.        [%2lu]   %s", sizeof( MAS_UNAME ), MAS_UNAME );
      DUF_PRINTF( 0, "V.        [%2lu]   %s", sizeof( PACKAGE_STRING ), PACKAGE_STRING );
      DUF_PRINTF( 0, "d.        [%2lu]   %s", sizeof( MAS_C_DATE ), MAS_C_DATE );
      DUF_PRINTF( 0, "Link d.   [%lu+%lu]  %lx.%lx", sizeof( ( unsigned long ) & __MAS_LINK_DATE__ ),
                  sizeof( ( unsigned long ) & __MAS_LINK_TIME__ ), ( unsigned long ) &__MAS_LINK_DATE__,
                  ( unsigned long ) &__MAS_LINK_TIME__ );
#ifdef MAS_SPLIT_DB
      DUF_PRINTF( 0, "MAS_SPLIT_DB" );
#endif
      mas_free( sargv2 );
      mas_free( sargv1 );
    }
    break;
  case DUF_OPTION_HELP:
    DUF_PRINTF( 0, "Usage: %s [OPTION]... [PATH]...", argv[0] );
    DUF_PRINTF( 0, "  -h, --help			[%s]", duf_find_longval_help( DUF_OPTION_HELP ) );
    DUF_PRINTF( 0, "  -x, --example			[%s]", duf_find_longval_help( DUF_OPTION_EXAMPLES ) );
    DUF_PRINTF( 0, "  --output-level		[%s]", duf_find_longval_help( DUF_OPTION_OUTPUT_LEVEL ) );
    DUF_PRINTF( 0, "Database ----------" );
    DUF_PRINTF( 0, "  -N, --db-name=%s", duf_config->db.main.name );
    DUF_PRINTF( 0, "  -D, --db-directory=%s", duf_config->db.dir );
    DUF_PRINTF( 0, "  --drop-tables		DANGEROUS!" );
    DUF_PRINTF( 0, "  --remove-database		DANGEROUS!" );
    DUF_PRINTF( 0, "  --create-tables" );
    DUF_PRINTF( 0, "  -V, --vacuum		vacuum" );
    DUF_PRINTF( 0, "Manipulations --------------" );
    DUF_PRINTF( 0, "  --add-path" );
    DUF_PRINTF( 0, "  -R, --recursive		recursive" );
    DUF_PRINTF( 0, "  --uni-scan" );
    DUF_PRINTF( 0, "    --md5" );
    DUF_PRINTF( 0, "    -f, --files" );
    DUF_PRINTF( 0, "    --tree" );
    DUF_PRINTF( 0, "    --print" );
    DUF_PRINTF( 0, "    --sample" );
    DUF_PRINTF( 0, "    --collect" );
    DUF_PRINTF( 0, "Old options" );
    DUF_PRINTF( 0, "  --zero-duplicates" );
    /* DUF_PRINTF(0,  "  --update-duplicates"  ); */
    DUF_PRINTF( 0, "  --zero-filedatas" );
    DUF_PRINTF( 0, "  --update-filedatas" );
    DUF_PRINTF( 0, "  --update-mdpaths" );
#ifdef DUF_COMPILE_EXPIRED
    DUF_PRINTF( 0, "  --update-exif" );
#endif
    DUF_PRINTF( 0, "  --same-md5" );
    DUF_PRINTF( 0, "Debug ----------" );
    DUF_PRINTF( 0, "  --debug" );
    DUF_PRINTF( 0, "  -v, --verbose=%d", duf_config->cli.dbg.verbose );
    DUF_PRINTF( 0, "  --min-dbg-lines=%lu", duf_config->cli.dbg.max_line );
    DUF_PRINTF( 0, "  --max-dbg-lines=%lu", duf_config->cli.dbg.min_line );
    DUF_PRINTF( 0, "Trace ----------" );
    DUF_PRINTF( 0, "  -A, --trace-action=%d", duf_config->cli.trace.action );
    DUF_PRINTF( 0, "  -C, --trace-current=%d", duf_config->cli.trace.current );
    DUF_PRINTF( 0, "  -S, --trace-scan=%d", duf_config->cli.trace.scan );
    DUF_PRINTF( 0, "  -Q, --trace-sql=%d", duf_config->cli.trace.sql );
    DUF_PRINTF( 0, "  --trace-sample=%d", duf_config->cli.trace.sample );
    DUF_PRINTF( 0, "  --trace-path=%d", duf_config->cli.trace.path );
    DUF_PRINTF( 0, "  -F, --trace-collect=%d", duf_config->cli.trace.collect );
    DUF_PRINTF( 0, "----------------" );
    r = 1;
    break;
  case DUF_OPTION_EXAMPLES:
    DUF_PRINTF( 0, "Examples" );
    DUF_PRINTF( 0, "  run  --db-name=test20140412  --drop-tables --create-tables" );
    DUF_PRINTF( 0, "  run  --db-name=test20140412  --add-path /home/mastar/a/down/  --uni-scan -R --md5 --file" );
    DUF_PRINTF( 0,
                "  run  --db-name=test20140412  --drop-tables --create-tables  --add-path /home/mastar/a/down/  --uni-scan -R --collect --dirent --files" );
    DUF_PRINTF( 0, "  run  --db-name=test20140412  --uni-scan /mnt/new_media/media/down/   --sample -f -R" );
    DUF_PRINTF( 0, "  run  --db-name=test20140412  --uni-scan /mnt/new_media/media/down/chromium/   --print    -R --files" );
    DUF_PRINTF( 0, "  run  --db-name=test20140412  --uni-scan /mnt/new_media/media/down/   --print  --tree  -R --files" );
    DUF_PRINTF( 0, "  run  --db-name=test20140412  --uni-scan /mnt/new_media/media/down/   --print   -R --files" );
    DUF_PRINTF( 0, "----------------" );
    DUF_PRINTF( 0, "  run  --db-name=test20140412 --zero-duplicates --update-duplicates" );
#ifdef DUF_COMPILE_EXPIRED
    DUF_PRINTF( 0, "  run  --db-name=test20140412 --zero-duplicates --update-exif" );
#endif
    DUF_PRINTF( 0, "=============================================" );
    DUF_PRINTF( 0, "  run  --db-name=test`datem`.db  --uni-scan /mnt/new_media/media/down/   --sample -f -R" );
    DUF_PRINTF( 0, "  run  --db-name=test`datem`.db  --uni-scan /mnt/new_media/media/down/   --sample -f -d -R" );
    DUF_PRINTF( 0, "  run  --db-name=test`datem`.db  --uni-scan /mnt/new_media/media/down/   --sample -d -R" );
    DUF_PRINTF( 0, "  run  --db-name=test`datem`.db  --uni-scan  --sample -d -R --trace-sample /mnt/new_media/media/down/ --max-seq=10" );
    DUF_PRINTF( 0, "  run  --db-name=test`datem`.db  --uni-scan  --sample -d -R --trace-sample=2 /mnt/new_media/media/down/ --max-seq=10" );
    DUF_PRINTF( 0, "  run  --db-name=test`datem`.db  --uni-scan -Ri5fd /home/mastar/a/down/ --trace-md5 --trace-new=0 --trace-stdout" );

    DUF_PRINTF( 0, "===== Transition ============================" );
#ifdef DUF_COMPILE_EXPIRED
    DUF_PRINTF( 0, "  run --update-duplicates --update-mdpath --update-filedata --update-exif" );
#endif
    DUF_PRINTF( 0, "  run --db-name=test`datem`.db  --uni-scan  --mdpath --collect --dirent -dfR  /mnt/new_media/media/down/" );
    DUF_PRINTF( 0, "=============================================" );
    DUF_PRINTF( 0, "Collect basic file info recursively to db" );
    DUF_PRINTF( 0, "  run  --db-name=test`datem`.db   --uni-scan -R --collect --dirent --files   /home/mastar/a/down/" );
    DUF_PRINTF( 0, "Calculate checksums recursively for files in db" );
    DUF_PRINTF( 0, "  run  --db-name=test`datem`.db  --uni-scan -R --collect --dirent --md5 --files --dirs /home/mastar/a/down/" );
    DUF_PRINTF( 0, "List files recursively from db" );
    DUF_PRINTF( 0, "  run  --db-name=test`datem`.db  --uni-scan   --print -R -d  --files /home/mastar/a/down/google-chrome/ " );
    DUF_PRINTF( 0, "=============================================" );
    DUF_PRINTF( 0, "  run --db-name=test.db  -AAPB   /mnt/new_media/media/down/ --summary" );
    DUF_PRINTF( 0, "    same as prev:" );
    DUF_PRINTF( 0, "  run --db-name=test.db --trace-action=2 --create-tables --uni-scan --recursive --collect --dirent"
                " --files --dirs --md5 /mnt/new_media/media/down/ --summary" );
    DUF_PRINTF( 0, "=============================================" );
    DUF_PRINTF( 0,
                "  run  --db-name=test20140416.db  --uni-scan --print  --md5 --noopenat -Rdf --max-dirs=300  --min-dirfiles=5 --min-size=10" );
    DUF_PRINTF( 0, "=============================================" );

    DUF_PRINTF( 0, "  run  --db-name=test20140416.db  --uni-scan   --print  -Rdf --max-seq=26 --max-depth=6 --summary"
                " --format=filesize,seq,filename,md5" );
    DUF_PRINTF( 0, "  run  --db-name=test20140416.db  --uni-scan   --print  -Rdf --max-seq=26 --max-depth=6"
                " --summary --format=filesize,seq,filename,md5 --tree" );
    DUF_PRINTF( 0, "  run  --trace-any=0  --db-name=test20140416.db  --uni-scan   --print  -df --max-seq=76 --max-depth=6 --summary"
                " --format=filesize,seq,filename,nlink,mtime,mode,gid,uid /mnt/new_media/media/down" );
    DUF_PRINTF( 0, "=============================================" );
    DUF_PRINTF( 0, "  run  --db-name=test$$.db  --uni-scan --print  --md5 --noopenat  -Rdf --max-dirs=300 --max-items=200 "
                " --min-dirfiles=5 --min-size=10 --noopenat" );

    DUF_PUTS( 0, "" );
    DUF_PUTS( 0, NULL );
    DUF_PUTSL( 0 );
    DUF_PRINTF( 0, "=== Working with the tree as for 20140429 ===================" );
    DUF_PRINTF( 0, "  run --remove-database" );
    DUF_PRINTF( 0, "  run -OPA /home/mastar/a/down/chromium/" );
    DUF_PRINTF( 0, "  run --uni-scan -RdifE5 /home/mastar/a/down/chromium/" );
    DUF_PRINTF( 0, "  run -RdifE5 /home/mastar/a/down/chromium/" );
    DUF_PRINTF( 0, "  run  --uni-scan   --print  --md5  -Rdf" );
    DUF_PRINTF( 0, "========================= as for 20140429 ===================" );
    DUF_PRINTF( 0, "  run --sample=2 -Rdf /home/mastar/a/down/////  --printf --max-seq=2000" );
    DUF_PRINTF( 0, "========================= as for 20140501 ===================" );
    DUF_PRINTF( 0, "  run -PRdifE5 /home/mastar/a/down/chromium/ -SSS" );
    DUF_PRINTF( 0, "  run  --uni-scan   --print  -Rd --summary  --tree   /home/mastar/a/down/" );
    DUF_PRINTF( 0, "  run  --uni-scan   --print  --md5  -Rdf --max-seq=1000" );
    DUF_PRINTF( 0, "========================= as for 20140504 ===================" );
    DUF_PRINTF( 0, "= full - cycle collect in one command =======================" );
    DUF_PRINTF( 0, "  run  --remove-database -OP  -RdifE5 /home/mastar/a/down/chromium/"
                " --exclude='*.doc' --exclude='*.jpg' --exclude='*.css'" " --exclude='*.html' --exclude='*.png'  --exclude='*.gif'"
                " --exclude='*.js'  --exclude='*.php'" );
    DUF_PRINTF( 0, "  run  --uni-scan   --print  -Rfd  /home/mastar/a/down/" );
    DUF_PRINTF( 0, "=============================================================" );
    DUF_PRINTF( 0, "  run  --uni-scan   --print  -Rfd  /home/mastar/a/down/  --size=-10 --size=+0" );
    DUF_PRINTF( 0, "  run  --uni-scan   --print  -Rfd  /home/mastar/a/down/  --size=-1b" );
    DUF_PRINTF( 0, "  run  --uni-scan   --print  -Rfd  /home/mastar/a/down/  --size=+1G" );
    DUF_PRINTF( 0, "=============================================================" );
    DUF_PRINTF( 0, "  run --trace-any  --print    -Rdf /home/mastar/a/down/ --same=+1 --format=filename,md5id" );
    DUF_PRINTF( 0, "  run --trace-any  --print    -Rdf /home/mastar/a/down/ --md5id=1285" );
    DUF_PRINTF( 0, "========================= as for 20140505 ===================" );
    DUF_PRINTF( 0, "  run --print  -Rfd /mnt/new_media/media/video/ --md5id=121549" " --format=human,inode=0,dataid=0" );
    DUF_PRINTF( 0, "  run --print  -Rfd /mnt/new_media/media/video/" " --format=human,inode=0,dataid=0,seq=0,prefix=0"
                " --include='*Speckled*'" );
    DUF_PRINTF( 0, "========================= as for 20140506 ===================" );
    DUF_PRINTF( 0, "  run -O -Rdif --mime /mnt/new_media/media/photo --max-seq=2000 (%gsec)", 2.8 );
    DUF_PRINTF( 0, "========================= as for 20140509 ===================" );
    DUF_PRINTF( 0, "  run --remove-database" );
    DUF_PRINTF( 0, "  run   -O						- %s", "creates DB" );
    DUF_PRINTF( 0, "  run   -P     /mnt/new_media/media/photo		- %s", "adds initial path" );
    DUF_PRINTF( 0, "  run   -OP    /mnt/new_media/media/photo		- %s", "creates DB AND adds initial path" );
    DUF_PRINTF( 0, "  run   -RdE   /mnt/new_media/media/photo		- %s", "no-action status" );
    DUF_PRINTF( 0, "  run   -Rd    /mnt/new_media/media/photo		- %s", "no-action status" );
    DUF_PRINTF( 0, "  run   -Rdf   /mnt/new_media/media/photo		- %s", "no-action status" );
    DUF_PRINTF( 0, "  run   -Rid   /mnt/new_media/media/photo		- %s", "no-action status" );
    DUF_PRINTF( 0, "  run   -RidE  /mnt/new_media/media/photo		- %s", "collect directories information" );
    DUF_PRINTF( 0, "  run   -RiDE  /mnt/new_media/media/photo		- %s", "        files data (nameless) information" );
    DUF_PRINTF( 0, "  run   -RinE  /mnt/new_media/media/photo		- %s", "        files names information" );
    DUF_PRINTF( 0, "  run   -Rif5  /mnt/new_media/media/photo		- %s", "        files md5 information (-f pre-open file)" );
    DUF_PRINTF( 0, "  run   -Rife  /mnt/new_media/media/photo		- %s", "        files mime information (-f pre-open file)" );
    DUF_PRINTF( 0, "  run   -pRTd  /mnt/new_media/media/photo		- %s", "print tree w/o files" );
    DUF_PRINTF( 0, "  run   -pRTdf /mnt/new_media/media/photo		- %s", "      tree with files" );
    DUF_PRINTF( 0, "  run   -pRTd  /mnt/new_media/media/photo --max-depth=2	- %s", "      tree with files limited depth" );
    DUF_PRINTF( 0, "  run   -pRdf  /mnt/new_media/media/photo		- %s", "      files (full paths) with dir headers linear" );
    DUF_PRINTF( 0, "  run   -pRd   /mnt/new_media/media/photo		- %s", "      dirs (full paths) linear" );
    DUF_PRINTF( 0, "  run   -pRf   /mnt/new_media/media/photo		- %s", "      files (full paths) linear" );
    DUF_PRINTF( 0, "========================= as for 20140510 ===================" );
    DUF_PRINTF( 0, "  run   -Rdf --sample  --output /mnt/new_media/media/photo/" );
    DUF_PRINTF( 0, "  run   -Rif5  /mnt/new_media/media/photo --progress" );
    DUF_PRINTF( 0, "========================= as for 20140513 ===================" );
    DUF_PRINTF( 0, "  run   -RifX  /mnt/new_media/media/photo		- %s", "        files exif information (-f pre-open file)" );
    DUF_PRINTF( 0, "=============================================================" );

    r = 1;
    break;
  }
  return r;
}

static int
duf_main( int argc, char **argv )
{
  int r = 0;

  /* DUF_TRACE( any, 0, "r=%d", r ); */
  {
    extern int mas_mem_disable_print_usage __attribute__ ( ( weak ) );

    if ( &mas_mem_disable_print_usage )
    {
      mas_mem_disable_print_usage = 7;
    }
  }
  r = duf_config_create(  );
  /* duf_config->cli.trace.explain = 1; */
  /* DUF_TRACE( any, 0, "r=%d", r ); */
  if ( r >= 0 )
  {
    int er = 0, fr = 0, or = 0;

    DUF_TRACE( any, 1, "any test" );
    if ( r >= 0 )
      er = r = duf_env_options( argc, argv );
    DUF_TRACE( explain, 0, "got env options; r:%d (%c)", r, r > ' ' && r < 'z' ? r : '-' );
    if ( r >= 0 )
      fr = r = duf_infile_options( argc, argv );
    DUF_TRACE( explain, 0, "got infile options; r:%d (%c)", r, r > ' ' && r < 'z' ? r : '-' );
    /* duf_config->cli.dbg.verbose = 4; */
    if ( r >= 0 )
      or = r = duf_cli_options( argc, argv );

    DUF_TRACE( explain, 0, "got cli options; r:%d (%c)", r, r > ' ' && r < 'z' ? r : '-' );

    if ( r >= 0 )
      r = duf_show_options( argv[0] );
    /* {                                 */
    /*   char c;                         */
    /*   char *s = NULL;                 */
    /*                                   */
    /*   c = *s;                         */
    /*   fprintf( stderr, "%c\n", c );   */
    /*   DUF_TRACE( any, 0, "r=%d", r ); */
    /* }                                 */
    {
      extern int dbgfunc_enabled __attribute__ ( ( weak ) );

      if ( &dbgfunc_enabled )
         /**/ dbgfunc_enabled = 1;
    }
    DUF_TEST_R( r );
    DUF_TRACE( explain, 0, "or: %d; fr: %d; er: %d", or, fr, er );
    if ( !duf_test_help( argc, argv, or ) && !duf_test_help( argc, argv, fr ) && !duf_test_help( argc, argv, er ) )
    {
      if ( r == 0 && duf_config && duf_config->db.dir )
      {
        DUF_TRACE( explain, 0, "to run main_db( argc, argv )" );
        r = main_db( argc, argv );
        DUF_TEST_R( r );
      }
      DUF_PUTS( 0, "--------------------------------------------------" );
      DUF_PRINTF( 0, " main_db ended                                                       [%s] (#%d)", duf_error_name( r ), r );
      DUF_PUTS( 0, "--------------------------------------------------" );
    }
    duf_config_delete(  );
  }
  else
  {
    DUF_ERROR( "(%d) %s", r, argv[0] );
  }
#ifdef MAS_TRACEMEM
  print_memlist( FL, stderr );
#endif
  DUF_ERRORR( r, "-" );
  return r < 0 && r != DUF_ERROR_MAX_REACHED ? 31 : 0;
}

int
main( int argc, char **argv )
{
  return duf_main( argc, argv );
}
