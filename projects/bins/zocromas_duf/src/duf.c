#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



/* #include <sqlite3.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>


/* #include "duf_def.h" */
#include "duf_sql.h"
#include "duf_create.h"
#include "duf_utils.h"
#include "duf_md5.h"
#include "duf_finddup.h"
#include "duf_update.h"
#include "duf_exif.h"

#include "duf.h"


/* error codes */
/* - /usr/include/errno.h                  */
/* + /usr/include/bits/errno.h             */
/*   /usr/include/features.h               */
/* - /usr/include/linux/errno.h            */
/* - /usr/include/asm/errno.h              */
/* + /usr/include/asm-generic/errno.h      */
/* + /usr/include/asm-generic/errno-base.h */



/*
 * /mnt/new_media/media/Photo/
 * /mnt/new_media/media/Pictures.R.20120207.164339/
 * /mnt/miscn/video/Pictures/ /home/mastar/a/to-attach/
 * /home/mastar/a/zaurus/sd/Documents/image/
 * /home/mastar/.local/share/Trash/
 * /mnt/old_home/mastar/.mas/HTC_Legend/
 * /mnt/tall/htclegend/
 * /mnt/tall/atalk/backup/
 *
 * /mnt/new_media/media/down/
 * */

/* run tables drop tables check path add /mnt/new_misc/develop/autotools/zoc-new/projects/bins/zocromas_duf/
 * sqlite3 -header -column /mnt/new_misc/develop/autotools/zoc-new/duf_db/duf-photo.sqlite3  \
 * 				'SELECT id, dirname, parentid  FROM duf_paths order by id limit 20'
 * */


/* tables drop       */
/* tables check      */
/* paths add ...     */
/* md5 update        */
/* exif update       */
/* duplicates update */
/* mdline update     */
/* duplicates  print */

int
duf_action( int argc, char **argv )
{
  int r = 0;
  int ia;

  for ( ia = 1; ia < argc; ia++ )
  {
    fprintf( stderr, "ia:%d\n\n\n", ia );
    if ( 0 )
    {
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "tables" ) && 0 == strcmp( argv[ia + 1], "drop" ) )
    {
      ia++;
      clear_tables(  );
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "tables" ) && 0 == strcmp( argv[ia + 1], "check" ) )
    {
      ia++;
      check_tables(  );
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "paths" ) && 0 == strcmp( argv[ia + 1], "add" ) )
    {
      ia++;
      for ( ia = ia + 1; ia < argc; ia++ )
        update_path( argv[ia], 0 /* parentid */ , 1 /* recurse */ , 1 /* dofiles */ , 1 /* added */  );
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "paths" ) && 0 == strcmp( argv[ia + 1], "print" ) )
    {
      ia++;
      print_paths(  );
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "md5" ) && 0 == strcmp( argv[ia + 1], "update" ) )
    {
      update_md5(  );
      ia++;
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "exif" ) && 0 == strcmp( argv[ia + 1], "update" ) )
    {
      update_exif(  );
      ia++;
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "duplicates" ) && 0 == strcmp( argv[ia + 1], "zero" ) )
    {
      zero_duplicates(  );
      ia++;
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "duplicates" ) && 0 == strcmp( argv[ia + 1], "update" ) )
    {
      update_duplicates(  );
      ia++;
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "duplicates" ) && 0 == strcmp( argv[ia + 1], "print" ) )
    {
      print_duplicates( 5 );
      ia++;
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "mdline" ) && 0 == strcmp( argv[ia + 1], "update" ) )
    {
      update_mdline(  );
      ia++;
    }
  }
  fprintf( stderr, "\n\n\n" );
  /* if ( 1 )                                      */
  /* {                                             */
  /*   check_tables(  );                           */
  /*   fprintf( stderr, "Scan %d\x1b[K\n", argc ); */
  /*   for ( int ia = 1; ia < argc; ia++ )         */
  /*     update_path( argv[ia], -1, 1, 1 );        */
  /*   update_md5(  );                             */
  /*   (* update_exif(  ); *)                      */
  /* }                                             */
  /* update_duplicates(  );                        */
  /* update_mdline(  );                            */
  /*                                               */
  /*                                               */
  /* (* print_hardlinks(  ); *)                    */
  /* if ( 0 )                                      */
  /* {                                             */
  /*                                               */
  /*   copy_jpeg_by_date(  );                      */
  /*                                               */
  /*   (* print_duplicate_dirs(  ); *)             */
  /*   print_duplicates(  );                       */
  /* }                                             */
  return r;
}

/* file:///usr/share/doc/sqlite-3.7.15.2/html/docs.html */
int
main( int argc, char **argv )
{
  char *dbdir;

  dbdir = getenv( "MAS_SHN_PROJECTS_DIR" );
  fprintf( stderr, "Dir: %s\n", dbdir );
  if ( dbdir )
  {
    char *dbfile;

    dbfile = mas_strdup( dbdir );
    dbfile = mas_strcat_x( dbfile, "/../duf_db/duf-photo.sqlite3" );
    fprintf( stderr, "File: %s\n", dbfile );
    /* fprintf(stderr, "File: %s\n", "/mnt/new_misc/develop/autotools/zoc-new/duf_db/duf-photo.sqlite3" ); */

    duf_sql_open( dbfile );
    duf_sql_exec( "PRAGMA synchronous = OFF" );
    duf_action( argc, argv );
    duf_sql_close(  );

    mas_free( dbfile );
  }
  return 0;
}
