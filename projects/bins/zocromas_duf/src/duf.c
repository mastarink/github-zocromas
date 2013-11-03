#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_types.h"

#include "duf_config.h"
#include "duf_cli_options.h"

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


/***
run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_dab/ -F aaaaa a b c
run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_dab/ -F aaaaa --drop-tables a b c
run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_dab/ -F temp.sqlite3 --drop-tables --create-tables
run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_db/ -F aaaaa a b c
run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_db/ -F temp.sqlite3 --drop-tables 
run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_db/ -F temp.sqlite3 --drop-tables --create-tables
run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_db/ -F temp.sqlite3 --drop-tables --create-tables --add-path .
run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_db/ -F temp.sqlite3 --drop-tables --create-tables --add-path 
				/mnt/tall/htclegend/ /mnt/old_home/mastar/.mas/HTC_Legend/ /mnt/new_media/media/photo/Pictures
				/mnt/new_media/media/photo/Pictures.R.20120207.164339 
run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_db/ -F temp.sqlite3 --drop-tables --create-tables --add-path 
				/mnt/tall/htclegend/ /mnt/old_home/mastar/.mas/HTC_Legend/ /mnt/new_media/media/photo/Pictures
				/mnt/new_media/media/photo/Pictures.R.20120207.164339
run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_db/ -F temp.sqlite3 --drop-tables --create-tables --add-path --update-path
				/mnt/tall/htclegend/ /mnt/old_home/mastar/.mas/HTC_Legend/ /mnt/new_media/media/photo/Pictures
				/mnt/new_media/media/photo/Pictures.R.20120207.164339 
run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_db/ -F temp.sqlite3 --drop-tables --create-tables --add-path --update-path
				--print-dirs 
run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_db/ -F temp.sqlite3 --drop-tables --create-tables --add-path --update-path
				--print-dirs --recursive .
run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_db/ -F temp.sqlite3 --drop-tables --create-tables --add-path --update-path
				--print-files --print-dirs --recursive .
run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_db/ -F temp.sqlite3 --drop-tables --create-tables --add-path --update-path 
				--print-files --recursive .
run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_db/ -F temp.sqlite3 --drop-tables --create-tables --add-path --update-path 
				--print-paths .
run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_db/ -F temp.sqlite3 --drop-tables --create-tables --add-path --update-path 
				--print-paths . /mnt/new_media/media/photo/Pictures.R.20120207.164339 
run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_db/ -F temp.sqlite3 --drop-tables --create-tables --add-path --update-path 
				--print-paths /mnt/tall/htclegend/ /mnt/old_home/mastar/.mas/HTC_Legend/ /mnt/new_media/media/photo/Pictures 
				/mnt/new_media/media/photo/Pictures.R.20120207.164339 
run -D /mnt/new_misc/develop/autotools/zoc-new/duf_db/
run -D /mnt/new_misc/develop/autotools/zoc-new/duf_db/duf-photo
run -D /mnt/new_misc/develop/autotools/zoc-new/duf_db/ -F 
run -D /mnt/new_misc/develop/autotools/zoc-new/duf_db/ -F aaaaa
run -D /mnt/new_misc/develop/autotools/zoc-new/duf_db/ -F aaaaa 
run -D /mnt/new_misc/develop/autotools/zoc-new/duf_db/ -F aaaaa a b c
run -D /mnt/new_misc/develop/autotools/zoc-new/duf_db/ -F aaaaa -N
run -D /mnt/new_misc/develop/autotools/zoc-new/duf_db/ -N aaaaa
run -F temp.sqlite3 --add-path --update-path --print-dirs --recursive .
run -F temp.sqlite3 --add-to-group --group=aaaaaa mased/ ; . shsq/libsql.bash ; sqf_group temp
run -F temp.sqlite3 --add-to-group --group=bbbbbb mased/ ; . shsq/libsql.bash ; sqf_group temp
run -F temp.sqlite3 --group=aaaaaa mased/
run -F temp.sqlite3 --group aaaaaa mased/
run -F temp.sqlite3 --group=aaaaaa mased/ ; . shsq/libsql.bash ; sqf_group temp
run -F temp.sqlite3 --group=bbbbbb mased/ ; . shsq/libsql.bash ; sqf_group temp
run -F temp.sqlite3 --print-dirs --recursive .
run -F temp.sqlite3 --remove-from-group --group=aaaaaa mased/ ; . shsq/libsql.bash ; sqf_group temp
run -F temp.sqlite3 --remove-from-group --group=bbbbbb mased/ ; . shsq/libsql.bash ; sqf_group temp
run -F temp.sqlite3 --update-mad5 --print-dirs --recursive .
run -F temp.sqlite3 --update-mad5 --print-dirs --recursive a
run -F temp.sqlite3 --update-md5 --print-dirs --recuarsive a
run -F temp.sqlite3 --update-md5 --print-dirs --recursive .
run -F temp.sqlite3 --update-md5 --print-dirs --recursive a
run --limit=2 --same-md5  --recursive 
run --print-dirs --recursive
run --print-dirs --recursive 
run --print-dirs --recursive /mnt/new_media/media/photo/Pictures.R.20120207.164339
run --print-dirs --recursive /mnt/new_media/media/photo/Pictures.R.20120207.164339/Picasa
run --print-files  --recursive  --limit=10
run --print-files  --recursive  --limit=10 /mnt/new_media/media/photo/Pictures/
run --print-paths
run --same-files  --limit=10 /mnt/new_media/media/photo/
run --same-files  --limit=10 /mnt/new_media/media/photo/Pictures/all/mov/
run --same-files  --limit=10 /mnt/new_media/media/photo/Pictures/all/mov/bbbfc0b7c5e0b0547474cfac27fbb7b2.mov
run --same-files /mnt/new_media/media/photo/Pictures/all/mov/bbbfc0b7c5e0b0547474cfac27fbb7b2.mov
run --same-files --recursive 
run --same-files  --recursive  --limit=10 /mnt/new_media/media/photo/
run --same-files  --recursive  --limit=10 /mnt/new_media/media/photo/Pictures/
run --same-files --recursive  /mnt/new_media/media/photo
run --same-files --recursive  /mnt/new_media/media/photo/Pictures.R.20120207.164339
run --same-md5  --limit=10 
run --same-md5 --limit=5 --recursive 
run --same-md5 --recursive 
run --same-md5  --recursive --limit=10


lshn && run --update-mdpath "'/mnt/new_media/media/photo/Pictures/unsorted/kodak/Kodak Pictures/08-14-2007'"



***/

int
duf_action_new( void )
{
  if ( duf_config )
  {
    if ( duf_config->drop_tables )
      duf_clear_tables(  );
    if ( duf_config->create_tables )
      duf_check_tables(  );
    if ( duf_config->add_path )
      for ( int ia = 0; ia < duf_config->targc; ia++ )
        duf_add_path( duf_config->targv[ia], "argument" );
    if ( duf_config->update_path )
      for ( int ia = 0; ia < duf_config->targc; ia++ )
        duf_update_path_down( duf_config->targv[ia], 0 /* parentid */ , ( duf_config->recursive ? DUF_RECURSIVE_YES : DUF_RECURSIVE_NO ),
                              DUF_TRUE /* dofiles */  );
    if ( duf_config->update_md5 )
      duf_update_md5_path( NULL, DUF_RECURSIVE_NO );
    if ( duf_config->update_duplicates )
      duf_update_duplicates(  );
    if ( duf_config->update_mdpath )
    {
      if ( !duf_config->targc )
        duf_update_mdpaths( 0 );
      else
        for ( int ia = 0; ia < duf_config->targc; ia++ )
        {
          const char *path;
          unsigned long long pathid;

          path = duf_config->targv[ia];
          pathid = duf_path_to_pathid( path );
          if ( pathid )
            duf_update_mdpaths( pathid );
          else
            fprintf( stderr, "not found %lld : '%s'\n", pathid, path );
        }
    }
    if ( duf_config->print_paths )
      duf_print_paths( duf_config->group );
    if ( duf_config->print_dirs )
    {
      if ( duf_config->targc > 0 )
        for ( int ia = 0; ia < duf_config->targc; ia++ )
          duf_print_dirs( duf_config->targv[ia], ( duf_config->recursive ? DUF_RECURSIVE_YES : DUF_RECURSIVE_NO ) );
      else
        duf_print_dirs( NULL, ( duf_config->recursive ? DUF_RECURSIVE_YES : DUF_RECURSIVE_NO ) );
    }
    if ( duf_config->print_files )
    {
      if ( duf_config->targc > 0 )
        for ( int ia = 0; ia < duf_config->targc; ia++ )
          duf_print_files( duf_config->targv[ia], ( duf_config->recursive ? DUF_RECURSIVE_YES : DUF_RECURSIVE_NO ) );
      else
        duf_print_files( NULL, ( duf_config->recursive ? DUF_RECURSIVE_YES : DUF_RECURSIVE_NO ) );
    }
    if ( duf_config->same_md5 )
      duf_print_md5_same( 1, duf_config->limit );

    if ( duf_config->same_files )
    {
      if ( !duf_config->targc )
        fprintf( stderr, "argument needed\n" );
      else
        for ( int ia = 0; ia < duf_config->targc; ia++ )
          duf_print_files_same( duf_config->targv[ia] );
    }
    if ( duf_config->to_group )
      for ( int ia = 0; ia < duf_config->targc; ia++ )
        duf_paths_group( duf_config->group, duf_config->targv[ia], +1 );
    if ( duf_config->from_group )
      for ( int ia = 0; ia < duf_config->targc; ia++ )
        duf_paths_group( duf_config->group, duf_config->targv[ia], -1 );
  }
  return 0;
}

/*
 *
 * $ run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_db/ -F temp.sqlite3 --drop-tables --create-tables --add-path --update-path --print-dirs --recursive .
 * $ run --print-dirs --recursive
 *
 * */

/* file:///usr/share/doc/sqlite-3.7.15.2/html/docs.html */
int
main( int argc, char **argv )
{
  for ( int ia = 0; ia < argc; ia++ )
  {
    fprintf( stderr, ">>>>>>>> argv[%d]: %s\n", ia, argv[ia] );
  }
  if ( 0 == duf_config_create(  ) )
  {
    if ( 0 == duf_cli_options( argc, argv ) )
    {
      if ( duf_config && duf_config->db_dir )
      {
        char *dbfile;

        dbfile = mas_strdup( duf_config->db_dir );
        dbfile = mas_strcat_x( dbfile, "/" );
        dbfile = mas_strcat_x( dbfile, duf_config->db_name );
        duf_config_show(  );
        if ( duf_config->verbose )
          fprintf( stderr, "dbfile: %s\n", dbfile );
        if ( 0 == duf_sql_open( dbfile ) )
        {
          duf_sql_exec( "PRAGMA synchronous = OFF" );
          duf_action_new(  );
          /* duf_action( argc, argv ); */
        }
        duf_sql_close(  );

        mas_free( dbfile );
      }
    }
    duf_config_delete(  );
  }
#ifdef MAS_TRACEMEM
  print_memlist( FL, stderr );
#endif

  return 0;
}
