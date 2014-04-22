#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_types.h"

#include "duf_config.h"
#include "duf_utils.h"
#include "duf_cli_options.h"

#include "duf_sql.h"
#include "duf_create.h"
/* #include "duf_utils.h" */
#include "duf_path.h"
#include "duf_mdpath.h"
/* #include "duf_file.h" */
/* #include "duf_file_md5id.h" */
/* #include "duf_file_pathid.h" */
#include "duf_finddup.h"

#include "duf_add.h"
/* #include "duf_update_path.h" */

#include "duf_filedata.h"
#include "duf_exif.h"

/* #include "duf_dir_scan.h" */

#include "duf_uni_scan.h"

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf.h"
/* ###################################################################### */

/* 20140420
 SEGFAULT:
A. -good
run    --db-name=test$$.db  --uni-scan -Z
run    --db-name=test$$.db  --uni-scan -OP /home/mastar/a/down/
run    --db-name=test$$.db  --uni-scan -Rid /home/mastar/a/down/

B. -segfault
run    --db-name=test$$.db  --uni-scan -Z
run    --db-name=test$$.db  --uni-scan -OPRid /home/mastar/a/down/

*********************
sqlite with uflag: debug :::


mastar@mastar .../bins/zocromas_duf $ run    --db-name=test$$.db  --uni-scan -OPRid5 /home/mastar/a/down/ 
. ○ make ok ○ . ○ installed ○ ------ to run 'zocromas_duf --db-name=test19639.db ...' -------
------------------------------------20140420.102643----

qargs: '--db-name=test19639.db' '--uni-scan' '-OPRid5' '/home/mastar/a/down/'
zocromas_duf: sqlite3.c:44875: sqlite3PagerAcquire: Assertion `(*ppPage)->pPager==pPager || (*ppPage)->pPager==0' failed.
Aborted (core dumped)

real	0m0.083s
user	0m0.014s
sys	0m0.006s
returned 134

◀  ●  ▶
#50  ▷[2430s]◁ /bin/bash;/bin/bash #19639 
₍Sun₎ ◆ 53.335s ▷ 20140420.102550 ▷ 20140420.102500 ▷ 20140417.145442
Apr 20 2014 ◆ 10:26:44 $19639 [xterm/25]  L1 /bind:roxterm@devel.0 (1/2/2/1)
mastar@mastar .../bins/zocromas_duf $



*********************
 */









/* 20140412                                                                                            */
/* reset:                                                                                              */
/*    run  --db-name=test20140412  --drop-tables --create-tables  --add-path /home/mastar/a/down/      */
/* test:                                                                                               */
/*    sqlite3 /mnt/new_misc/develop/autotools/zoc-new/duf_db/test20140412 .tables                      */
/*    sqlite3 /mnt/new_misc/develop/autotools/zoc-new/duf_db/test20140412 'select * from duf_paths'    */
/* fill:                                                                                               */
/*    run  --db-name=test20140412  --uni-scan /mnt/new_media/media/down/  -R  --fill -v --trace-fill=1 */


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
 * run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_dab/ -N aaaaa a b c
 * run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_dab/ -N aaaaa --drop-tables a b c
 * run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_dab/ -N temp.sqlite3 --drop-tables --create-tables
 * run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_db/ -N aaaaa a b c
 * run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_db/ -N temp.sqlite3 --drop-tables 
 * run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_db/ -N temp.sqlite3 --drop-tables --create-tables
 * run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_db/ -N temp.sqlite3 --drop-tables --create-tables --add-path .
 * run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_db/ -N temp.sqlite3 --drop-tables --create-tables --add-path 
 * 				/mnt/tall/htclegend/ /mnt/old_home/mastar/.mas/HTC_Legend/ /mnt/new_media/media/photo/Pictures
 * 				/mnt/new_media/media/photo/Pictures.R.20120207.164339 
 * run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_db/ -N temp.sqlite3 --drop-tables --create-tables --add-path 
 * 				/mnt/tall/htclegend/ /mnt/old_home/mastar/.mas/HTC_Legend/ /mnt/new_media/media/photo/Pictures
 * 				/mnt/new_media/media/photo/Pictures.R.20120207.164339
 * run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_db/ -N temp.sqlite3 --drop-tables --create-tables --add-path --update-path
 * 				/mnt/tall/htclegend/ /mnt/old_home/mastar/.mas/HTC_Legend/ /mnt/new_media/media/photo/Pictures
 * 				/mnt/new_media/media/photo/Pictures.R.20120207.164339 
 * run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_db/ -N temp.sqlite3 --drop-tables --create-tables --add-path --update-path
 * 				--print-dirs 
 * run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_db/ -N temp.sqlite3 --drop-tables --create-tables --add-path --update-path
 * 				--print-dirs --recursive .
 * run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_db/ -N temp.sqlite3 --drop-tables --create-tables --add-path --update-path
 * 				--print-files --print-dirs --recursive .
 * run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_db/ -N temp.sqlite3 --drop-tables --create-tables --add-path --update-path 
 * 				--print-files --recursive .
 * run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_db/ -N temp.sqlite3 --drop-tables --create-tables --add-path --update-path 
 * 				--print-paths .
 * run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_db/ -N temp.sqlite3 --drop-tables --create-tables --add-path --update-path 
 * 				--print-paths . /mnt/new_media/media/photo/Pictures.R.20120207.164339 
 * run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_db/ -N temp.sqlite3 --drop-tables --create-tables --add-path --update-path 
 * 				--print-paths /mnt/tall/htclegend/ /mnt/old_home/mastar/.mas/HTC_Legend/ /mnt/new_media/media/photo/Pictures 
 * 				/mnt/new_media/media/photo/Pictures.R.20120207.164339 
 * run -D /mnt/new_misc/develop/autotools/zoc-new/duf_db/
 * run -D /mnt/new_misc/develop/autotools/zoc-new/duf_db/duf-photo
 * run -D /mnt/new_misc/develop/autotools/zoc-new/duf_db/ -N 
 * run -D /mnt/new_misc/develop/autotools/zoc-new/duf_db/ -N aaaaa
 * run -D /mnt/new_misc/develop/autotools/zoc-new/duf_db/ -N aaaaa 
 * run -D /mnt/new_misc/develop/autotools/zoc-new/duf_db/ -N aaaaa a b c
 * run -N temp.sqlite3 --add-path --update-path --print-dirs --recursive .
 * run -N temp.sqlite3 --add-to-group --group=aaaaaa mased/ ; . shsq/libsql.bash ; sqf_group temp
 * run -N temp.sqlite3 --add-to-group --group=bbbbbb mased/ ; . shsq/libsql.bash ; sqf_group temp
 * run -N temp.sqlite3 --group=aaaaaa mased/
 * run -N temp.sqlite3 --group aaaaaa mased/
 * run -N temp.sqlite3 --group=aaaaaa mased/ ; . shsq/libsql.bash ; sqf_group temp
 * run -N temp.sqlite3 --group=bbbbbb mased/ ; . shsq/libsql.bash ; sqf_group temp
 * run -N temp.sqlite3 --print-dirs --recursive .
 * run -N temp.sqlite3 --remove-from-group --group=aaaaaa mased/ ; . shsq/libsql.bash ; sqf_group temp
 * run -N temp.sqlite3 --remove-from-group --group=bbbbbb mased/ ; . shsq/libsql.bash ; sqf_group temp
 * run -N temp.sqlite3 --update-mad5 --print-dirs --recursive .
 * run -N temp.sqlite3 --update-mad5 --print-dirs --recursive a
 * run -N temp.sqlite3 --update-md5 --print-dirs --recuarsive a
 * run -N temp.sqlite3 --update-md5 --print-dirs --recursive .
 * run -N temp.sqlite3 --update-md5 --print-dirs --recursive a
 * run --limit=2 --same-md5  --recursive 
 * run --print-dirs --recursive
 * run --print-dirs --recursive 
 * run --print-dirs --recursive /mnt/new_media/media/photo/Pictures.R.20120207.164339
 * run --print-dirs --recursive /mnt/new_media/media/photo/Pictures.R.20120207.164339/Picasa
 * run --print-files  --recursive  --limit=10
 * run --print-files  --recursive  --limit=10 /mnt/new_media/media/photo/Pictures/
 * run --print-paths
 * run --same-files  --limit=10 /mnt/new_media/media/photo/
 * run --same-files  --limit=10 /mnt/new_media/media/photo/Pictures/all/mov/
 * run --same-files  --limit=10 /mnt/new_media/media/photo/Pictures/all/mov/bbbfc0b7c5e0b0547474cfac27fbb7b2.mov
 * run --same-files /mnt/new_media/media/photo/Pictures/all/mov/bbbfc0b7c5e0b0547474cfac27fbb7b2.mov
 * run --same-files --recursive 
 * run --same-files  --recursive  --limit=10 /mnt/new_media/media/photo/
 * run --same-files  --recursive  --limit=10 /mnt/new_media/media/photo/Pictures/
 * run --same-files --recursive  /mnt/new_media/media/photo
 * run --same-files --recursive  /mnt/new_media/media/photo/Pictures.R.20120207.164339
 * run --same-md5  --limit=10 
 * run --same-md5 --limit=5 --recursive 
 * run --same-md5 --recursive 
 * run --same-md5  --recursive --limit=10
 * 
 * run --update-exif
 * 
 * lshn && run --update-mdpath "'/mnt/new_media/media/photo/Pictures/unsorted/kodak/Kodak Pictures/08-14-2007'"
 * 
 * 
 * run --drop-tables --create-tables --add-path --update-path --update-md5 --recursive /mnt/tall/htclegend/ /mnt/old_home/mastar/.mas/HTC_Legend/ 
 * 
 * run --drop-tables --create-tables --add-path --update-path --update-md5 --update-duplicates --update-mdpath --update-filedata --update-exif
 * 		--recursive /mnt/tall/htclegend/ /mnt/old_home/mastar/.mas/HTC_Legend/ 
 * 		/mnt/new_media/media/photo/Pictures /mnt/new_media/media/photo/Pictures.R.20120207.164339/
 * 
 * run
 * */


static int
duf_action_new( int argc, char **argv )
{
  int r = 0;

  DUF_TEST_R( r );
/*										*/ duf_dbgfunc( DBG_START, __func__, __LINE__ );
/* --drop-tables								*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  if ( r >= 0 && duf_config->cli.act.drop_tables )
    r = duf_clear_tables(  );
  DUF_TEST_R( r );
  if ( r >= 0 && duf_config->cli.act.vacuum )
    r = duf_sql_exec( "VACUUM", ( int * ) NULL );
  DUF_TEST_R( r );
/* --create-tables								*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  if ( r >= 0 && duf_config->cli.act.create_tables )
    r = duf_check_tables(  );
  DUF_TEST_R( r );

  if ( 1 )
  {
    char *sargv1, *sargv2;
    char *qsargv1, *qsargv2;
    int changes = 0;

    sargv1 = mas_argv_string( argc, argv, 1 );
    sargv2 = duf_restore_options( argv[0] );
    DUF_TRACE( any, 0, "restored optd:%s", sargv2 );
    qsargv1 = duf_single_quotes_2( sargv1 );
    qsargv2 = duf_single_quotes_2( sargv2 );


    /* r = duf_sql_c( "INSERT INTO duf_log (args, restored_args, msg, now) " " VALUES ('%s', '%s', '%s', datetime())", 0, ); */
    r = duf_sql( "INSERT OR IGNORE INTO duf_log (args, restored_args, msg, now) " " VALUES ('%s', '%s', '%s', datetime())", &changes,
                 qsargv1 ? qsargv1 : sargv1, qsargv2 ? qsargv2 : sargv2, "" );
    DUF_TRACE( current, 0, "log inserted %d/%d [%s] - %d", changes, r, sargv1, argc );
    mas_free( qsargv1 );
    mas_free( qsargv2 );
    mas_free( sargv1 );
    mas_free( sargv2 );
  }
  DUF_TEST_R( r );

/* --add-path									*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  if ( r >= 0 && duf_config->cli.act.add_path )
  {
    for ( int ia = 0; r >= 0 && ia < duf_config->targc; ia++ )
      r = duf_add_path_uni( duf_config->targv[ia], "argument", &r );
    DUF_TEST_R( r );
  }
  /* ????????? */
  /*  r = duf_sql_close(  );                   */
  /*  r = duf_sql_open( duf_config->db.fpath ); */
  /* (* ????????? *)                           */

  DUF_TEST_R( r );



/*  --update-path								*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  /* if ( duf_config->update_path )                                                    */
  /*   for ( int ia = 0; ia < duf_config->targc; ia++ )                                */
  /*     duf_update_path( duf_config->targv[ia], 0 (* parentid *) , duf_config->u,     */
  /*                      0 (* level *) , NULL (* pseq *) , DUF_TRUE (* dofiles *)  ); */
/*										*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );



  /* if ( duf_config->update_md5 )                                      */
  /* {                                                                  */
  /*   if ( duf_config->targc > 0 )                                     */
  /*     for ( int ia = 0; ia < duf_config->targc; ia++ )               */
  /*       duf_update_md5_path( duf_config->targv[ia], duf_config->u ); */
  /*   else                                                             */
  /*     duf_update_md5_path( NULL, duf_config->u );                    */
  /* }                                                                  */


/* --zero-duplicates								*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  if ( r >= 0 && duf_config->cli.act.zero_duplicates )
    r = duf_zero_duplicates(  );
  DUF_TEST_R( r );
/* --update-duplicates								*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  if ( r >= 0 && duf_config->cli.act.update_duplicates )
    r = duf_update_duplicates(  );
/* --zero-filedatas								*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  if ( r >= 0 && duf_config->cli.act.zero_filedata )
    duf_zero_filedatas(  );
  DUF_TEST_R( r );
/* --update-filedatas								*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  if ( r >= 0 && duf_config->cli.act.update_filedata )
    duf_update_filedatas(  );
  DUF_TEST_R( r );
/*  --update-mdpaths								*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  if ( r >= 0 && duf_config->cli.act.update_mdpath )
    duf_update_mdpaths( 0 );
  DUF_TEST_R( r );
/*										*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  if ( r >= 0 && duf_config->cli.act.update_mdpath_selective )
  {
    if ( !duf_config->targc )
      duf_update_mdpaths( 0 );
    else
      for ( int ia = 0; r >= 0 && ia < duf_config->targc; ia++ )
      {
        const char *path;
        unsigned long long pathid;

        path = duf_config->targv[ia];
        pathid = duf_path_to_pathid( path, ( duf_depthinfo_t * ) NULL );
        if ( pathid )
          r = duf_update_mdpaths( pathid );
        else
          fprintf( stderr, "not found %lld : '%s'\n", pathid, path );
      }
/*  --update-exif								*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  }
  DUF_TEST_R( r );
  if ( r >= 0 && duf_config->cli.act.update_exif )
  {
    if ( !duf_config->targc )
      duf_update_exif( 0 );
    else
      for ( int ia = 0; ia < duf_config->targc; ia++ )
      {
        const char *path;
        unsigned long long pathid;

        path = duf_config->targv[ia];
        pathid = duf_path_to_pathid( path, ( duf_depthinfo_t * ) NULL );
        if ( pathid )
          duf_update_exif( pathid );
        else
          fprintf( stderr, "not found %lld : '%s'\n", pathid, path );
      }
/*										*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  }
  DUF_TEST_R( r );
  /* if ( duf_config->cli.act.print_paths )          */
  /*   duf_print_paths( duf_config->group ); */
/*  --print-dirs								*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
/*   if ( duf_config->cli.act.print_dirs )                                                                                                 */
/*   {                                                                                                                             */
/* (*                                                                              *) duf_dbgfunc( DBG_STEP, __func__, __LINE__ ); */
/*     if ( duf_config->targc > 0 )                                                                                                */
/*       for ( int ia = 0; ia < duf_config->targc; ia++ )                                                                          */
/*         duf_print_dirs( duf_config->targv[ia], duf_config->u, duf_config->cli.act.tree );                                               */
/*     else                                                                                                                        */
/*       duf_print_dirs( NULL, duf_config->u, duf_config->cli.act.tree );                                                                  */
/* (*                                                                              *) duf_dbgfunc( DBG_STEP, __func__, __LINE__ ); */
/*   }                                                                                                                             */
/*  --print-files								*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
/*   if ( duf_config->cli.act.print_files )                                                                                                */
/*   {                                                                                                                             */
/*     if ( duf_config->targc > 0 )                                                                                                */
/*       for ( int ia = 0; ia < duf_config->targc; ia++ )                                                                          */
/*         duf_print_files( duf_config->targv[ia], duf_config->u );                                                                */
/*     else                                                                                                                        */
/*       duf_print_files( NULL, duf_config->u );                                                                                   */
/* (*                                                                              *) duf_dbgfunc( DBG_STEP, __func__, __LINE__ ); */
/*   }                                                                                                                             */
/*  --uni-scan									*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  if ( r >= 0 && duf_config->cli.act.uni_scan )
  {
    r = duf_uni_scan_all(  );
    /* if ( r == DUF_ERROR_MAX_REACHED )                 */
    /*   DUF_TRACE( action, 0, "Maximum reached ...." ); */
    /* else if ( r < 0 )                                 */
    /*   DUF_ERROR( "code: %d", r );                     */
  }
/*										*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  /* if ( duf_config->cli.act.same_md5 )               */
  /*   duf_print_md5_same( 1, duf_config->cli.limit ); */

/*										*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );

/*   if ( duf_config->cli.act.same_files )                                                                                         */
/*   {                                                                                                                             */
/*     if ( !duf_config->targc )                                                                                                   */
/*       fprintf( stderr, "argument needed\n" );                                                                                   */
/*     else                                                                                                                        */
/*       for ( int ia = 0; ia < duf_config->targc; ia++ )                                                                          */
/*         duf_print_files_same( duf_config->targv[ia] );                                                                          */
/* (*                                                                              *) duf_dbgfunc( DBG_STEP, __func__, __LINE__ ); */
/*   }                                                                                                                             */
  if ( r >= 0 && duf_config->cli.act.same_exif )
    duf_print_exif_same( 1, duf_config->cli.limit );

/*										*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  if ( r >= 0 && duf_config->cli.act.to_group )
    for ( int ia = 0; ia < duf_config->targc; ia++ )
      duf_paths_group( duf_config->group, duf_config->targv[ia], +1 );
/*										*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  if ( r >= 0 && duf_config->cli.act.from_group )
    for ( int ia = 0; ia < duf_config->targc; ia++ )
      duf_paths_group( duf_config->group, duf_config->targv[ia], -1 );
/*										*/ duf_dbgfunc( DBG_END, __func__, __LINE__ );
  /* if ( r < 0 && !r == DUF_ERROR_MAX_REACHED ) */
  /*   DUF_TEST_R(r);        */
  /* else if ( r < 0 )                           */
  /*   DUF_ERROR( "r=%d", r );                   */
  return r;
}

/*
 *
 * $ run --db-directory=/mnt/new_misc/develop/autotools/zoc-new/duf_db/ -N temp.sqlite3 --drop-tables --create-tables --add-path --update-path --print-dirs --recursive .
 * $ run --print-dirs --recursive
 *
 * */

/* file://usr/share/doc/sqlite-3.7.15.2/html/docs.html */
/* file://usr/share/doc/sqlite-3.8.4.3/html/docs.html */
int
main_db( int argc, char **argv )
{
  int r = DUF_ERROR_MAIN;

  DUF_VERBOSE( 0, "verbose test 0> %d %s", 17, "hello" );
  DUF_VERBOSE( 1, "verbose test 1> %d %s", 17, "hello" );

/*										*/ duf_dbgfunc( DBG_START, __func__, __LINE__ );
  if ( duf_config->db.dir && duf_config->db.name )
  {
    r = 0;
    DUF_TRACE( action, 0, "db.dir:%s; db.name:%s", duf_config->db.dir, duf_config->db.name );
    duf_config->db.fpath = mas_strdup( duf_config->db.dir );
    duf_config->db.fpath = mas_strcat_x( duf_config->db.fpath, "/" );
    duf_config->db.fpath = mas_strcat_x( duf_config->db.fpath, duf_config->db.name );

    /* if ( r >= 0 )                                      */
    /*   r = duf_cli_option_by_string( "trace-maction=2" ); */
    DUF_TRACE( current, 0, "r:%d", r );
    if ( r >= 0 && duf_config->cli.dbg.verbose )
    {
      for ( int ia = 0; ia < argc; ia++ )
        DUF_TRACE( any, 0, "######### argv[%d]: %s", ia, argv[ia] );
      r = duf_config_show(  );
      DUF_TRACE( any, 0, "dbfile: %s", duf_config->db.fpath );
    }
    /* DUF_TRACE( any, 0, "r=%d", r ); */
    if ( r >= 0 )
    {
      r = DUF_ERROR_PTR;
      if ( duf_config->db.fpath )
        r = duf_sql_open( duf_config->db.fpath );
    }
    /* DUF_TRACE( any, 0, "r=%d", r ); */
    if ( r >= 0 )
    {
      r = duf_sql_exec( "PRAGMA synchronous = OFF", ( int * ) NULL );
      if ( r >= 0 )
        r = duf_sql_exec( "PRAGMA encoding = 'UTF-8'", ( int * ) NULL );

      DUF_TEST_R( r );
      if ( r >= 0 )
        r = duf_action_new( argc, argv );

      if ( r < 0 && r != DUF_ERROR_MAX_REACHED )
        DUF_ERROR( "action FAIL ; r=%d", r );
      /* duf_action( argc, argv ); */
      r = duf_sql_close(  );
    }
  }
  else if ( !duf_config->db.dir )
  {
    r = DUF_ERROR_PTR;
    DUF_ERROR( "db.dir not set" );
  }
  else if ( !duf_config->db.name )
  {
    r = DUF_ERROR_PTR;
    DUF_ERROR( "db.name not set" );
  }
/*										*/ duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}

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
        printf( "  --drop-tables" "\n" );
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
        printf( "  --update-duplicates" "\n" );
        printf( "  --zero-filedatas" "\n" );
        printf( "  --update-filedatas" "\n" );
        printf( "  --update-mdpaths" "\n" );
        printf( "  --update-exif" "\n" );
        printf( "  --same-md5" "\n" );
        printf( "Debug ----------" "\n" );
        printf( "  --debug" "\n" );
        printf( "  -v, --verbose=level" "\n" );
        printf( "  --min-dbg-lines" "\n" );
        printf( "  --max-dbg-lines" "\n" );
        printf( "Trace ----------" "\n" );
        printf( "  -S, --trace-scan=level" "\n" );
        printf( "  -Q, --trace-sql=level" "\n" );
        printf( "  --trace-sample=level" "\n" );
        printf( "  -F, --trace-fill=level" "\n" );
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
