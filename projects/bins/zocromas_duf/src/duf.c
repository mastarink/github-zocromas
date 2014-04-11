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
#include "duf_mdpath.h"
#include "duf_file.h"
#include "duf_file_md5id.h"
#include "duf_file_pathid.h"
#include "duf_md5.h"
#include "duf_finddup.h"

#include "duf_update.h"
#include "duf_update_path.h"

#include "duf_filedata.h"
#include "duf_exif.h"

#include "duf_dir_scan.h"

#include "duf_uni_scan.h"

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf.h"
/* ###################################################################### */

/**
 20140409: 
run --recursive  --update-path /mnt/old_home/mastar/.mas --max-items=40 --max-depth=3 --min-size=10000000 -vvv

run  --uni-scan /home/mastar/a/down/ -R --max-depth=4  --tree --files
run  --uni-scan /home/mastar/a/down/ --max-depth=4  --max-items=70 --files --tree
run  --uni-scan /home/mastar/a/down/ --max-depth=4  --max-items=70 -R --tree


 **/


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

run --update-exif

lshn && run --update-mdpath "'/mnt/new_media/media/photo/Pictures/unsorted/kodak/Kodak Pictures/08-14-2007'"


run --drop-tables --create-tables --add-path --update-path --update-md5 --recursive /mnt/tall/htclegend/ /mnt/old_home/mastar/.mas/HTC_Legend/ 

run --drop-tables --create-tables --add-path --update-path --update-md5 --update-duplicates --update-mdpath --update-filedata --update-exif
		--recursive /mnt/tall/htclegend/ /mnt/old_home/mastar/.mas/HTC_Legend/ 
		/mnt/new_media/media/photo/Pictures /mnt/new_media/media/photo/Pictures.R.20120207.164339/

run 

***/




int
duf_action_new( void )
{
/*										*/ duf_dbgfunc( DBG_START, __func__, __LINE__ );
/* --drop-tables								*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  if ( duf_config->drop_tables )
    duf_clear_tables(  );
/* --create-tables								*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  if ( duf_config->create_tables )
    duf_check_tables(  );
/* --add-path									*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  if ( duf_config->add_path )
    for ( int ia = 0; ia < duf_config->targc; ia++ )
      duf_add_path( duf_config->targv[ia], "argument" );
/*  --update-path								*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  if ( duf_config->update_path )
    for ( int ia = 0; ia < duf_config->targc; ia++ )
      duf_update_path( duf_config->targv[ia], 0 /* parentid */ , duf_config->u,
                       0 /* level */ , NULL /* pseq */ , DUF_TRUE /* dofiles */  );
/*										*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );



  if ( duf_config->update_md5 )
  {
    if ( duf_config->targc > 0 )
      for ( int ia = 0; ia < duf_config->targc; ia++ )
        duf_update_md5_path( duf_config->targv[ia], duf_config->u );
    else
      duf_update_md5_path( NULL, duf_config->u );
  }


/*										*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  if ( duf_config->zero_duplicates )
    duf_zero_duplicates(  );
/*										*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  if ( duf_config->update_duplicates )
    duf_update_duplicates(  );
/*										*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  if ( duf_config->zero_filedata )
    duf_zero_filedatas(  );
/*										*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  if ( duf_config->update_filedata )
    duf_update_filedatas(  );
/*										*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  if ( duf_config->update_mdpath )
    duf_update_mdpaths( 0 );
/*										*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  if ( duf_config->update_mdpath_selective )
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
/*										*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  }
  if ( duf_config->update_exif )
  {
    if ( !duf_config->targc )
      duf_update_exif( 0 );
    else
      for ( int ia = 0; ia < duf_config->targc; ia++ )
      {
        const char *path;
        unsigned long long pathid;

        path = duf_config->targv[ia];
        pathid = duf_path_to_pathid( path );
        if ( pathid )
          duf_update_exif( pathid );
        else
          fprintf( stderr, "not found %lld : '%s'\n", pathid, path );
      }
/*										*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  }
  if ( duf_config->print_paths )
    duf_print_paths( duf_config->group );
/*  --print-dirs								*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  if ( duf_config->print_dirs )
  {
/*										*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
    if ( duf_config->targc > 0 )
      for ( int ia = 0; ia < duf_config->targc; ia++ )
        duf_print_dirs( duf_config->targv[ia], duf_config->u, duf_config->tree );
    else
      duf_print_dirs( NULL, duf_config->u, duf_config->tree );
/*										*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  }
/*  --print-files								*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  if ( duf_config->print_files )
  {
    if ( duf_config->targc > 0 )
      for ( int ia = 0; ia < duf_config->targc; ia++ )
        duf_print_files( duf_config->targv[ia], duf_config->u );
    else
      duf_print_files( NULL, duf_config->u );
/*										*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  }
/*  --uni-scan									*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  if ( duf_config->uni_scan )
  {
    duf_uni_scan_all( duf_config );
  }
/*										*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  if ( duf_config->same_md5 )
    duf_print_md5_same( 1, duf_config->limit );

/*										*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );

  if ( duf_config->same_files )
  {
    if ( !duf_config->targc )
      fprintf( stderr, "argument needed\n" );
    else
      for ( int ia = 0; ia < duf_config->targc; ia++ )
        duf_print_files_same( duf_config->targv[ia] );
/*										*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  }
  if ( duf_config->same_exif )
    duf_print_exif_same( 1, duf_config->limit );

/*										*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  if ( duf_config->to_group )
    for ( int ia = 0; ia < duf_config->targc; ia++ )
      duf_paths_group( duf_config->group, duf_config->targv[ia], +1 );
/*										*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  if ( duf_config->from_group )
    for ( int ia = 0; ia < duf_config->targc; ia++ )
      duf_paths_group( duf_config->group, duf_config->targv[ia], -1 );
/*										*/ duf_dbgfunc( DBG_END, __func__, __LINE__ );
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
main_db( int argc, char **argv )
{
  char *dbfile;

/*										*/ duf_dbgfunc( DBG_START, __func__, __LINE__ );
  dbfile = mas_strdup( duf_config->db_dir );
  dbfile = mas_strcat_x( dbfile, "/" );
  dbfile = mas_strcat_x( dbfile, duf_config->db_name );
  if ( duf_config->verbose )
  {
    for ( int ia = 0; ia < argc; ia++ )
      fprintf( stderr, ">>>>>>>> argv[%d]: %s\n", ia, argv[ia] );
    duf_config_show(  );
    if ( duf_config->verbose > 1 )
      fprintf( stderr, "dbfile: %s\n", dbfile );
  }
  if ( 0 == duf_sql_open( dbfile ) )
  {
    duf_sql_exec( "PRAGMA synchronous = OFF" );
    duf_action_new(  );
    /* duf_action( argc, argv ); */
  }
  duf_sql_close(  );

  mas_free( dbfile );
/*										*/ duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}

int
main( int argc, char **argv )
{
  if ( 0 == duf_config_create(  ) )
  {
    /* duf_config->verbose = 4; */
    int r = duf_cli_options( argc, argv );
    extern int dbgfunc_enabled;

    dbgfunc_enabled = 1;
    if ( !r && duf_config && duf_config->db_dir )
      main_db( argc, argv );

    duf_config_delete(  );
  }
#ifdef MAS_TRACEMEM
  print_memlist( FL, stderr );
#endif

  return 0;
}
