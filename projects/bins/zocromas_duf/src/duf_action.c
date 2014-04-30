#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>


#include "duf_config.h"
#include "duf_utils.h"
#include "duf_cli_options.h"
#include "duf_service.h"

#include "duf_sql.h"
#include "duf_create.h"
#include "duf_path.h"

#include "duf_add.h"

/* #include "duf_filedata_expire.h" */
#include "duf_filedata.h"
/* #include "duf_exif.h" */

#include "duf_uni_scan.h"

#ifdef DUF_COMPILE_EXPIRED
#include "duf_group.h"
#endif

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_action.h"
/* ###################################################################### */



int
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

    r = duf_sql( "INSERT OR IGNORE INTO duf_log (args, restored_args, msg) VALUES ('%s', '%s', '%s')", &changes,
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
      ( void ) duf_add_path_uni( duf_config->targv[ia], "argument", 0 /*need_id */ , &r );
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
  /* if ( r >= 0 && duf_config->cli.act.zero_duplicates ) */
  /*   r = duf_zero_duplicates(  );                       */
  DUF_TEST_R( r );
/* --update-duplicates								*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  /* if ( r >= 0 && duf_config->cli.act.update_duplicates ) */
  /*   r = duf_update_duplicates(  );                       */
#if 0
/* --zero-filedatas								*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  if ( r >= 0 && duf_config->cli.act.zero_filedata )
    duf_zero_filedatas(  );
  DUF_TEST_R( r );
/* --update-filedatas								*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  if ( r >= 0 && duf_config->cli.act.update_filedata )
    duf_update_filedatas(  );
  DUF_TEST_R( r );
#endif
/*  --update-mdpaths								*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  /* if ( r >= 0 && duf_config->cli.act.update_mdpath ) */
  /*   duf_update_mdpaths( 0 );                         */
  /* DUF_TEST_R( r ); */
#ifdef DUF_COMPILE_EXPIRED
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
          DUF_ERROR( "not found %lld : '%s'\n", pathid, path );
      }
/*  --update-exif                                                               */ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  }
  DUF_TEST_R( r );
#endif
#if 0
  if ( r >= 0 && duf_config->cli.act.update_exif )
  {
    if ( !duf_config->targc )
      r = duf_update_exif( 0 );
    else
      for ( int ia = 0; r >= 0 && ia < duf_config->targc; ia++ )
      {
        const char *path;
        unsigned long long pathid;

        path = duf_config->targv[ia];
        pathid = duf_path_to_pathid( path, ( duf_depthinfo_t * ) NULL, &r );
        if ( r >= 0 && pathid )
          r = duf_update_exif( pathid );
        else
          DUF_ERROR( "not found %lld : '%s'\n", pathid, path );
      }
/*										*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  }
#endif
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
/*       DUF_ERROR( "argument needed\n" );                                                                                   */
/*     else                                                                                                                        */
/*       for ( int ia = 0; ia < duf_config->targc; ia++ )                                                                          */
/*         duf_print_files_same( duf_config->targv[ia] );                                                                          */
/* (*                                                                              *) duf_dbgfunc( DBG_STEP, __func__, __LINE__ ); */
/*   }                                                                                                                             */
  /* if ( r >= 0 && duf_config->cli.act.same_exif )     */
  /*   duf_print_exif_same( 1, duf_config->cli.limit ); */

#ifdef DUF_COMPILE_EXPIRED
/*										*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  if ( r >= 0 && duf_config->cli.act.to_group )
    for ( int ia = 0; ia < duf_config->targc; ia++ )
      duf_paths_group( duf_config->group, duf_config->targv[ia], +1 );
/*										*/ duf_dbgfunc( DBG_STEP, __func__, __LINE__ );
  if ( r >= 0 && duf_config->cli.act.from_group )
    for ( int ia = 0; ia < duf_config->targc; ia++ )
      duf_paths_group( duf_config->group, duf_config->targv[ia], -1 );
#endif
  /*										*/ duf_dbgfunc( DBG_END, __func__, __LINE__ );
  /* if ( r < 0 && !r == DUF_ERROR_MAX_REACHED ) */
  /*   DUF_TEST_R(r);        */
  /* else if ( r < 0 )                           */
  /*   DUF_ERROR( "r=%d", r );                   */
  return r;
}
