#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_config.h"
#include "duf_utils.h"
#include "duf_cli_options.h"
#include "duf_service.h"

#include "duf_dbg.h"

#include "duf_action.h"
#include "duf_maindb.h"

/* ###################################################################### */
#include "duf.h"
/* ###################################################################### */


int
main( int argc, char **argv )
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
  /* DUF_TRACE( any, 0, "r=%d", r ); */
  if ( r >= 0 )
  {
    extern int dbgfunc_enabled;
    int or;

    DUF_TRACE( any, 0, "X" );
    if ( r >= 0 )
      r = duf_infile_options( argc, argv );
    /* duf_config->cli.dbg.verbose = 4; */
    if ( r >= 0 )
      or = r = duf_cli_options( argc, argv );
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
    dbgfunc_enabled = 1;
    DUF_TEST_R( r );
    if ( r == 0 && duf_config && duf_config->db.dir )
    {
      r = main_db( argc, argv );
      DUF_TEST_R( r );
    }
    {
      switch ( or )
      {
      case DUF_OPTION_HELP:
        printf( "Usage: %s [OPTION]... [PATH]...\n", argv[0] );
        printf( "  -h, --help			help\n" );
        printf( "Database ----------" "\n" );
        printf( "  -N, --db-name=%s\n", duf_config->db.name );
        printf( "  -D, --db-directory=%s\n", duf_config->db.dir );
        printf( "  --drop-tables		DANGEROUS!" "\n" );
        printf( "  --remove-database		DANGEROUS!" "\n" );
        printf( "  --create-tables" "\n" );
        printf( "Manipulations --------------" "\n" );
        printf( "  --add-path\n" );
        printf( "  -R, --recursive		recursive" "\n" );
        printf( "  --uni-scan" "\n" );
        printf( "    --md5" "\n" );
        printf( "    -f, --files" "\n" );
        printf( "    --tree" "\n" );
        printf( "    --print" "\n" );
        printf( "    --sample" "\n" );
        printf( "    --fill" "\n" );
        printf( "Old options" "\n" );
        printf( "  --zero-duplicates" "\n" );
        /* printf( "  --update-duplicates" "\n" ); */
        printf( "  --zero-filedatas" "\n" );
        printf( "  --update-filedatas" "\n" );
        printf( "  --update-mdpaths" "\n" );
        printf( "  --update-exif" "\n" );
        printf( "  --same-md5" "\n" );
        printf( "Debug ----------" "\n" );
        printf( "  --debug" "\n" );
        printf( "  -v, --verbose=%d" "\n", duf_config->cli.dbg.verbose );
        printf( "  --min-dbg-lines=%lu" "\n", duf_config->cli.dbg.max_line );
        printf( "  --max-dbg-lines=%lu" "\n", duf_config->cli.dbg.min_line );
        printf( "Trace ----------" "\n" );
        printf( "  -A, --trace-action=%d" "\n", duf_config->cli.trace.action );
        printf( "  -S, --trace-scan=%d" "\n", duf_config->cli.trace.scan );
        printf( "  -Q, --trace-sql=%d" "\n", duf_config->cli.trace.sql );
        printf( "  --trace-sample=%d" "\n", duf_config->cli.trace.sample );
        printf( "  --trace-path=%d" "\n", duf_config->cli.trace.path );
        printf( "  -F, --trace-fill=%d" "\n", duf_config->cli.trace.fill );
        printf( "----------------" "\n" );
        r = 0;
        break;
      case DUF_OPTION_HELP_EXAMPLES:
        printf( "Examples" "\n" );
        printf( "  run  --db-name=test20140412  --drop-tables --create-tables" "\n" );
        printf( "  run  --db-name=test20140412  --add-path /home/mastar/a/down/  --uni-scan -R --md5 --file" "\n" );
        printf( "  run  --db-name=test20140412  --drop-tables --create-tables  --add-path /home/mastar/a/down/  --uni-scan -R --fill --files" "\n" );
        printf( "  run  --db-name=test20140412  --uni-scan /mnt/new_media/media/down/   --sample -f -R" "\n" );
        printf( "  run  --db-name=test20140412  --uni-scan /mnt/new_media/media/down/chromium/   --print    -R --files" "\n" );
        printf( "  run  --db-name=test20140412  --uni-scan /mnt/new_media/media/down/   --print  --tree  -R --files" "\n" );
        printf( "  run  --db-name=test20140412  --uni-scan /mnt/new_media/media/down/   --print   -R --files" "\n" );
        printf( "----------------" "\n" );
        printf( "  run  --db-name=test20140412 --zero-duplicates --update-duplicates" "\n" );
        printf( "  run  --db-name=test20140412 --zero-duplicates --update-exif" "\n" );
        printf( "=============================================" "\n" );
        printf( "  run  --db-name=test`datem`.db  --uni-scan /mnt/new_media/media/down/   --sample -f -R" "\n" );
        printf( "  run  --db-name=test`datem`.db  --uni-scan /mnt/new_media/media/down/   --sample -f -d -R" "\n" );
        printf( "  run  --db-name=test`datem`.db  --uni-scan /mnt/new_media/media/down/   --sample -d -R" "\n" );
        printf( "  run  --db-name=test`datem`.db  --uni-scan  --sample -d -R --trace-sample /mnt/new_media/media/down/ --max-seq=10" "\n" );
        printf( "  run  --db-name=test`datem`.db  --uni-scan  --sample -d -R --trace-sample=2 /mnt/new_media/media/down/ --max-seq=10"
                "\n" );
        printf( "  run  --db-name=test`datem`.db  --uni-scan -Ri5fd /home/mastar/a/down/ --trace-md5 --trace-new=0 --trace-stdout" "\n" );

        printf( "===== Transition ============================" "\n" );
        printf( "  run --update-duplicates --update-mdpath --update-filedata --update-exif" "\n" );
        printf( "  run --db-name=test`datem`.db  --uni-scan  --mdpath --fill -dfR  /mnt/new_media/media/down/" "\n" );
        printf( "=============================================" "\n" );
        printf( "Collect basic file info recursively to db\n" );
        printf( "  run  --db-name=test`datem`.db   --uni-scan -R --fill --files   /home/mastar/a/down/" "\n" );
        printf( "Calculate checksums recursively for files in db\n" );
        printf( "  run  --db-name=test`datem`.db  --uni-scan -R --fill --md5 --files --dirs /home/mastar/a/down/" "\n" );
        printf( "List files recursively from db\n" );
        printf( "  run  --db-name=test`datem`.db  --uni-scan   --print -R -d  --files /home/mastar/a/down/google-chrome/ " "\n" );
        printf( "=============================================" "\n" );
        printf( "  run --db-name=test.db  -AAPB   /mnt/new_media/media/down/ --totals" "\n" );
        printf( "    same as prev:" "\n" );
        printf( "  run --db-name=test.db --trace-action=2 --create-tables --uni-scan --recursive --fill"
                " --files --dirs --md5 /mnt/new_media/media/down/ --totals" "\n" );

        printf( "=============================================" "\n" );
        printf( "  run  --db-name=test20140416.db  --uni-scan   --print  --md5 --noopenat  -Rdf --max-dirs=300  --min-dirfiles=5 --min-size=10" "\n" );
        printf( "=============================================" "\n" );

        printf( "  run  --db-name=test20140416.db  --uni-scan   --print  -Rdf --max-seq=26 --max-depth=6 --totals"
                " --format=filesize,seq,filename,md5" "\n" );
        printf( "  run  --db-name=test20140416.db  --uni-scan   --print  -Rdf --max-seq=26 --max-depth=6"
                " --totals --format=filesize,seq,filename,md5 --tree" "\n" );

        printf( "  run  --trace-any=0  --db-name=test20140416.db  --uni-scan   --print  -df --max-seq=76 --max-depth=6 --totals"
                " --format=filesize,seq,filename,nlink,mtime,mode,gid,uid /mnt/new_media/media/down" "\n" );

        printf( "=============================================" "\n" );
        printf( "  run  --db-name=test$$.db  --uni-scan --print  --md5 --noopenat  -Rdf --max-dirs=300 --max-items=200 "
                " --min-dirfiles=5 --min-size=10 --noopenat" "\n" );


        printf( "=============================================" "\n" );
        r = 0;
        break;
      default:
        break;
      }
    }
    if ( r < 0 )
      DUF_ERROR( "(%d) %s", r, argv[0] );

    duf_config_delete(  );
  }
  else
  {
    DUF_ERROR( "(%d) %s", r, argv[0] );
  }
#ifdef MAS_TRACEMEM
  print_memlist( FL, stderr );
#endif

  return 0;
}
