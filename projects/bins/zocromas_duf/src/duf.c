#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_types.h"

#include "duf_sql.h"
#include "duf_create.h"
/* #include "duf_utils.h" */
#include "duf_path.h"
#include "duf_file.h"
#include "duf_file_md5id.h"
#include "duf_file_pathid.h"
#include "duf_md5.h"
#include "duf_finddup.h"
#include "duf_update.h"
#include "duf_update_path.h"
#include "duf_exif.h"
#include "duf_dir_scan.h"


/* ###################################################################### */
#include "duf.h"
/* ###################################################################### */



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

/*
 * run tables drop
 * run tables check
 * run paths add <dir path>
 * sqf "select * from duf_group;"
 * -- groups argument,updated --
 * run paths print [+-]<group>
 * run paths id <dir path>
 * TODO : run paths update <path> - to make "add" not updating
 * TODO : run files print <pathgroup> - add pathgroup
 * run md5 update
 * run md5 same
 * run md5 print
 * run group <dir path>
 * */

int
duf_action( int argc, char **argv )
{
  int r = 0;

  for ( int ia = 1; ia < argc; ia++ )
  {
    /* fprintf( stderr, "ia:%d of %d\n\n\n", ia, argc ); */
    if ( 0 )
    {
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "data" ) && 0 == strcmp( argv[ia + 1], "prepare" ) )
    {
      ia++;
      duf_check_tables(  );
      for ( ia = ia + 1; ia < argc; ia++ )
      {
        duf_add_path( argv[ia], "argument" );
        duf_update_path_down( argv[ia], 0 /* parentid */ , DUF_RECURSIVE_YES, DUF_TRUE /* dofiles */  );
      }
      duf_update_md5_path( NULL );
      duf_update_duplicates(  );
      duf_update_mdpaths(  );
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "tables" ) && 0 == strcmp( argv[ia + 1], "drop" ) )
    {
      ia++;
      duf_clear_tables(  );
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "tables" ) && 0 == strcmp( argv[ia + 1], "check" ) )
    {
      ia++;
      duf_check_tables(  );
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "paths" ) && 0 == strcmp( argv[ia + 1], "add" ) )
    {
      ia++;
      for ( ia = ia + 1; ia < argc; ia++ )
        duf_add_path( argv[ia], "argument" );
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "paths" ) && 0 == strcmp( argv[ia + 1], "update" ) )
    {
      ia++;
      for ( ia = ia + 1; ia < argc; ia++ )
        duf_update_path_down( argv[ia], 0 /* parentid */ , DUF_RECURSIVE_YES, DUF_TRUE /* dofiles */  );
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "paths" ) && 0 == strcmp( argv[ia + 1], "print" ) )
    {
      ia++;
      if ( ia + 1 < argc )
        duf_print_paths( argv[++ia] );
      else
        duf_print_paths( NULL );
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "paths" ) && 0 == strcmp( argv[ia + 1], "id" ) )
    {
      unsigned long long prev = 0;
      char *notfound = NULL;

      ia++;
      for ( ia = ia + 1; ia < argc; ia++ )
      {
        unsigned long long pathid = duf_path_to_pathid_x( argv[ia], &prev, &notfound );
        char *path = duf_pathid_to_path( pathid );
        char *lastpath = duf_pathid_to_path( prev );

        if ( pathid )
        {
          printf( "#%lld:\t\t'%s' => '%s'\nprev: #%lld =>\t'%s'\t\t; n/f '%s'\n", pathid, argv[ia], path ? path : "", prev, lastpath,
                  notfound ? notfound : "" );
        }
        else
        {
        }
        mas_free( path );
        mas_free( lastpath );
      }
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "files" ) && 0 == strcmp( argv[ia + 1], "print" ) )
    {
      ia++;
      for ( ia = ia + 1; ia < argc; ia++ )
      {
        const char *path = argv[ia];

        /* fprintf( stderr, "argv[%d]='%s'\n", ia, argv[ia] ); */
        if ( *path == '+' )
          duf_print_files( ++path, DUF_RECURSIVE_YES );
        else if ( *path == '-' )
          duf_print_files( ++path, DUF_RECURSIVE_NO );
        else
          duf_print_files( path, DUF_RECURSIVE_NO );
      }
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "dirs" ) && 0 == strcmp( argv[ia + 1], "print" ) )
    {
      ia++;
      for ( ia = ia + 1; ia < argc; ia++ )
      {
        const char *path = argv[ia];

        /* fprintf( stderr, "argv[%d]='%s'\n", ia, argv[ia] ); */
        if ( *path == '+' )
          duf_print_dirs( ++path, DUF_RECURSIVE_YES );
        else if ( *path == '-' )
          duf_print_dirs( ++path, DUF_RECURSIVE_NO );
        else
          duf_print_dirs( path, DUF_RECURSIVE_NO );
      }
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "files" ) && 0 == strcmp( argv[ia + 1], "same" ) )
    {
      ia++;
      if ( ia + 1 >= argc )
        fprintf( stderr, "argument needed %d %d\n", ia, argc );
      for ( ia = ia + 1; ia < argc; ia++ )
      {
        /* fprintf( stderr, "argv[%d]='%s'\n", ia, argv[ia] ); */
        duf_print_files_same( argv[ia] );
      }
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "md5" ) && 0 == strcmp( argv[ia + 1], "print" ) )
    {
      ia++;
      duf_print_md5( 0 );
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "md5" ) && 0 == strcmp( argv[ia + 1], "update" ) )
    {
      ia++;
      fprintf( stderr, "ia:%d argc:%d\n", ia, argc );
      if ( ia + 1 >= argc )
        duf_update_md5_path( NULL );
      else
        for ( ia = ia + 1; ia < argc; ia++ )
          duf_update_md5_path( argv[ia] );
      ia++;
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "md5" ) && 0 == strcmp( argv[ia + 1], "same" ) )
    {
      unsigned long long limit = 0;

      ia++;
      /* fprintf( stderr, "ia:%d of %d\n", ia, argc ); */
      if ( ia + 1 >= argc || sscanf( argv[ia + 1], "%llu", &limit ) )
        duf_print_md5_same( 1, limit );
      ia++;
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "group" ) )
    {
      ia += 1;
      if ( ia < argc )
      {
        char *group;

        group = argv[ia++];
        for ( ia = ia; ia < argc; ia++ )
        {
          const char *path = argv[ia];

          duf_paths_group( group, path );
        }
      }
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "exif" ) && 0 == strcmp( argv[ia + 1], "update" ) )
    {
      duf_update_exif(  );
      ia++;
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "duplicates" ) && 0 == strcmp( argv[ia + 1], "zero" ) )
    {
      duf_zero_duplicates(  );
      ia++;
    }
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "duplicates" ) && 0 == strcmp( argv[ia + 1], "update" ) )
    {
      duf_update_duplicates(  );
      ia++;
    }
    /* else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "duplicates" ) && 0 == strcmp( argv[ia + 1], "print" ) ) */
    /* {                                                                                                              */
    /*   print_duplicates( 5000 );                                                                                    */
    /*   ia++;                                                                                                        */
    /* }                                                                                                              */
    else if ( ia < ( argc - 1 ) && 0 == strcmp( argv[ia], "mdline" ) && 0 == strcmp( argv[ia + 1], "update" ) )
    {
      duf_update_mdpaths(  );
      ia++;
    }
    else
    {
      fprintf( stderr, "wrong command\n" );
    }
  }
  fprintf( stderr, "\n\n\n" );
  return r;
}

/* file:///usr/share/doc/sqlite-3.7.15.2/html/docs.html */
int
main( int argc, char **argv )
{
  char *dbdir;

  dbdir = getenv( "MAS_SHN_PROJECTS_DIR" );
  /* fprintf( stderr, "Dir: %s\n", dbdir ); */
  if ( dbdir )
  {
    char *dbfile;

    dbfile = mas_strdup( dbdir );
    dbfile = mas_strcat_x( dbfile, "/../duf_db/duf-photo.sqlite3" );
    /* fprintf( stderr, "File: %s\n", dbfile ); */
    /* fprintf(stderr, "File: %s\n", "/mnt/new_misc/develop/autotools/zoc-new/duf_db/duf-photo.sqlite3" ); */

    duf_sql_open( dbfile );
    duf_sql_exec( "PRAGMA synchronous = OFF" );
    duf_action( argc, argv );
    duf_sql_close(  );

    mas_free( dbfile );
  }
#ifdef MAS_TRACEMEM
  print_memlist( FL, stderr );
#endif

  return 0;
}
