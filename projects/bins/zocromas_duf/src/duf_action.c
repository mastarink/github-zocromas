#include <string.h>
#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>


#include "duf_config.h"
#include "duf_utils.h"
#include "duf_service.h"

#include "duf_cli_options.h"
#include "duf_options.h"


#include "duf_sql.h"
/* #include "duf_sql1.h" */
#include "duf_sql2.h"

#include "duf_create.h"
#include "duf_path.h"

#include "duf_add.h"

/* #include "duf_filedata_expire.h" */
#include "duf_filedata.h"
/* #include "duf_exif.h" */

#include "duf_uni_scan.h"

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_action.h"
/* ###################################################################### */



int
duf_action_new( int argc, char **argv )
{
  int r = 0;


  DUF_TEST_R( r );
/*										*/ DEBUG_END(  );
/* --drop-tables								*/ DEBUG_STEP(  );
  if ( r >= 0 && duf_config->cli.act.drop_tables )
  {
    r = duf_clear_tables(  );
    duf_config->actions_done++;
  }
  DUF_TEST_R( r );
  if ( r >= 0 && duf_config->cli.act.vacuum )
  {
    static const char *sql = "VACUUM";

    DUF_SQL_START_STMT_NOPDI( sql, r, pstmt );
    DUF_SQL_STEP( r, pstmt );
    DUF_SQL_END_STMT_NOPDI( r, pstmt );
    duf_config->actions_done++;
  }
  /* r = duf_sql_exec( "VACUUM", ( int * ) NULL ); */
  DUF_TEST_R( r );
/* --create-tables								*/ DEBUG_STEP(  );
  if ( r >= 0 && duf_config->cli.act.create_tables )
  {
    r = duf_check_tables(  );
    duf_config->actions_done++;
  }
  DUF_TEST_R( r );

  {
    char *sargv1, *sargv2;
    int changes = 0;

    sargv1 = mas_argv_string( argc, argv, 1 );
    sargv2 = duf_restore_options( argv[0] );
    DUF_TRACE( any, 0, "restored optd:%s", sargv2 );
    {
      static const char *sql = "INSERT OR IGNORE INTO " DUF_DBADMPREF "log (args, restored_args, msg) VALUES (:args, :restored_args, '')";

      DUF_SQL_START_STMT_NOPDI( sql, r, pstmt );
      DUF_SQL_BIND_S( args, sargv1, r, pstmt );
      DUF_SQL_BIND_S( args, sargv2, r, pstmt );
      DUF_SQL_STEP( r, pstmt );
      DUF_SQL_CHANGES_NOPDI( changes, r, pstmt );
      DUF_SQL_END_STMT_NOPDI( r, pstmt );
      /* if ( r == DUF_ERROR_SQL_NO_TABLE ) */
      /*   r = 0;                           */
    }
    DUF_TRACE( action, 0, "LOG inserted %d/%d [%s] - %d", changes, r, sargv1, argc );
    mas_free( sargv2 );
    mas_free( sargv1 );
  }
  {
    /*--max-size= ; --min-size= ; --include= ;*/
    /* --min-with-same-md5 : --min-copies= ; --max-copies= ;  --min-duplicates; --max-duplicates; */
    /* --have-md5 ; --have-exif ; --attributes= ... */
    /* --min-with-same-size */
    /* --in-archive --on-removable */
    /* --access-before= --access-after --modification-before= --modification-after ... */
    /* see also for --filetype=  or --filemagic=                              */
    /*   mastar@mastar .../bins/zocromas_duf $ apropos libmagic              */
    /*   magic_buffer [libmagic] (3)  - Magic number recognition library     */
    /*   magic_check [libmagic] (3)  - Magic number recognition library      */
    /*   magic_close [libmagic] (3)  - Magic number recognition library      */
    /*   magic_compile [libmagic] (3)  - Magic number recognition library    */
    /*   magic_descriptor [libmagic] (3)  - Magic number recognition library */
    /*   magic_error [libmagic] (3)  - Magic number recognition library      */
    /*   magic_list [libmagic] (3)  - Magic number recognition library       */
    /*   magic_load [libmagic] (3)  - Magic number recognition library       */
    /*   magic_open [libmagic] (3)  - Magic number recognition library       */
    /*   magic_setflags [libmagic] (3)  - Magic number recognition library   */
    /*   magic_version [libmagic] (3)  - Magic number recognition library    */
    {
      const char *sqls[] = {
        "UPDATE " DUF_DBADMPREF "filefilter SET run=datetime() " /* */
              " WHERE type='cli' " /* */
              " AND ifnull(minsize,0)=ifnull(:minsize,0) " /* */
              " AND ifnull(maxsize,0)=ifnull(:maxsize,0) " /* */
              " AND ifnull(mindups,0)=ifnull(:mindups,0) " /* */
              " AND ifnull(maxdups,0)=ifnull(:maxdups,0) " /* */
              " AND ifnull(glob_include,'')=ifnull(:glob_include,'')" /* */
              " AND ifnull(glob_exclude,'')=ifnull(:glob_exclude,'')" /* */
              ,
        "INSERT INTO " DUF_DBADMPREF "filefilter (type,minsize,maxsize,mindups,maxdups,glob_include,glob_exclude) " /* */
              " VALUES ("       /* */
              " 'cli', :minsize, :maxsize, :mindups, :maxdups, :glob_include, :glob_exclude " /* */
              ")",
        "SELECT id FROM " DUF_DBADMPREF "filefilter " /* */
              " WHERE type='cli' " /* */
              " AND ifnull(minsize,0)=ifnull(:minsize,0) " /* */
              " AND ifnull(maxsize,0)=ifnull(:maxsize,0) " /* */
              " AND ifnull(mindups,0)=ifnull(:mindups,0) " /* */
              " AND ifnull(maxdups,0)=ifnull(:maxdups,0) " /* */
              " AND ifnull(glob_include,'')=ifnull(:glob_include,'')" /* */
              " AND ifnull(glob_exclude,'')=ifnull(:glob_exclude,'')" /* */
              ,
        NULL
      };
      {
        int changes = 0;
        const char **psql;

        psql = sqls;
        while ( psql && *psql )
        {
          const char *sql = *psql++;
          duf_sqlite_stmt_t *pstmt = NULL;

          if ( !changes || ( sql && 0 == strncmp( sql, "SELECT", 6 ) ) )
          {
            /* DUF_TRACE( action, 0, "to PREPARE %s", sql ); */
            if ( r >= 0 )
              r = duf_sql_prepare( sql, &pstmt );
            DUF_TEST_R( r );
            /* DUF_TRACE( action, 0, "PREPARE:%d", r ); */
            if ( r >= 0 )
            {
              if ( r >= 0 && duf_config->u.minsize )
                r = duf_sql_bind_long_long_nz( pstmt, ":minsize", duf_config->u.minsize );
              DUF_TEST_R( r );
              if ( r >= 0 && duf_config->u.maxsize )
                r = duf_sql_bind_long_long_nz( pstmt, ":maxsize", duf_config->u.maxsize );
              /* DUF_TRACE( action, 0, "BIND maxsize %lld", duf_config->u.maxsize ); */
              DUF_TEST_R( r );
              if ( r >= 0 && duf_config->u.minsame )
                r = duf_sql_bind_long_long_nz( pstmt, ":mindups", duf_config->u.minsame );
              DUF_TEST_R( r );
              if ( r >= 0 && duf_config->u.maxsame )
                r = duf_sql_bind_long_long_nz( pstmt, ":maxdups", duf_config->u.maxsame );
              DUF_TEST_R( r );
              {
                char *j;

                j = mas_argv_string( duf_config->u.glob.include_files.argc, duf_config->u.glob.include_files.argv, 0 );
                /* DUF_TRACE( action, 0, "%d GLOB INCLUDE %s", duf_config->u.glob.include_files.argc, j ); */
                if ( r >= 0 && duf_config->u.glob.include_files.argc )
                  r = duf_sql_bind_string( pstmt, ":glob_include", j );
                DUF_TEST_R( r );
                mas_free( j );
              }
              {
                char *j;

                j = mas_argv_string( duf_config->u.glob.exclude_files.argc, duf_config->u.glob.exclude_files.argv, 0 );

                /* DUF_TRACE( action, 0, "GLOB EXCLUDE %s", j ); */
                if ( r >= 0 && duf_config->u.glob.exclude_files.argc )
                  r = duf_sql_bind_string( pstmt, ":glob_exclude", j );
                DUF_TEST_R( r );
                mas_free( j );
              }
              do
              {
                if ( r == DUF_SQL_ROW )
                  r = 0;
                if ( r >= 0 )
                  r = duf_sql_step( pstmt );
                DUF_TEST_RR( r );
                if ( !changes )
                  changes = duf_sql_changes(  );

                if ( r == DUF_SQL_ROW )
                {
                  long long id;

                  id = duf_sql_column_long_long( pstmt, 0 );
                  duf_config->u.filter_id = id;
                }
              }
              while ( r == DUF_SQL_ROW );
            }
            /* DUF_TRACE( action, 0, "STEP; changes:%d; %s", changes, r < 0 && r != DUF_SQL_DONE ? "FAIL" : "" ); */
            {
              int rf = duf_sql_finalize( pstmt );

              DUF_TEST_R( rf );
              /* DUF_TRACE( action, 0, "FINALIZE %s;", rf < 0 ? "FAIL" : "" ); */

              if ( r >= 0 || r == DUF_SQL_DONE )
                r = rf;
            }
          }
        }
      }
    }
  }
  DUF_TEST_R( r );

/* --add-path									*/ DEBUG_STEP(  );
  if ( r >= 0 && duf_config->cli.act.add_path )
  {
    for ( int ia = 0; r >= 0 && ia < duf_config->targc; ia++ )
    {
      r = duf_add_path_uni( duf_config->targv[ia] );
      duf_config->actions_done++;
    }
    DUF_TEST_R( r );
  }
  /* ????????? */
  /*  r = duf_sql_close(  );                   */
  /*  r = duf_sql_open( duf_config->db.fpath ); */
  /* (* ????????? *)                           */

  DUF_TEST_R( r );



/*  --update-path								*/ DEBUG_STEP(  );
  /* if ( duf_config->update_path )                                                    */
  /*   for ( int ia = 0; ia < duf_config->targc; ia++ )                                */
  /*     duf_update_path( duf_config->targv[ia], 0 (* parentid *) , duf_config->u,     */
  /*                      0 (* level *) , NULL (* pseq *) , DUF_TRUE (* dofiles *)  ); */
/*										*/ DEBUG_STEP(  );



  /* if ( duf_config->update_md5 )                                      */
  /* {                                                                  */
  /*   if ( duf_config->targc > 0 )                                     */
  /*     for ( int ia = 0; ia < duf_config->targc; ia++ )               */
  /*       duf_update_md5_path( duf_config->targv[ia], duf_config->u ); */
  /*   else                                                             */
  /*     duf_update_md5_path( NULL, duf_config->u );                    */
  /* }                                                                  */


/* --zero-duplicates								*/ DEBUG_STEP(  );
  /* if ( r >= 0 && duf_config->cli.act.zero_duplicates ) */
  /*   r = duf_zero_duplicates(  );                       */
  DUF_TEST_R( r );
/* --update-duplicates								*/ DEBUG_STEP(  );
  /* if ( r >= 0 && duf_config->cli.act.update_duplicates ) */
  /*   r = duf_update_duplicates(  );                       */
#if 0
/* --zero-filedatas								*/ DEBUG_STEP(  );
  if ( r >= 0 && duf_config->cli.act.zero_filedata )
    duf_zero_filedatas(  );
  DUF_TEST_R( r );
/* --update-filedatas								*/ DEBUG_STEP(  );
  if ( r >= 0 && duf_config->cli.act.update_filedata )
    duf_update_filedatas(  );
  DUF_TEST_R( r );
#endif
/*  --update-mdpaths								*/ DEBUG_STEP(  );
  /* if ( r >= 0 && duf_config->cli.act.update_mdpath ) */
  /*   duf_update_mdpaths( 0 );                         */
  /* DUF_TEST_R( r ); */
#if 0
  if ( r >= 0 && duf_config->cli.act.update_exif )
  {
    if ( !duf_config->targc )
      r = duf_update_exif( 0 );
    else
      for ( int ia = 0; r >= 0 && ia < duf_config->targc; ia++ )
      {
        const char *path;
        unsigned long long dirid;

        path = duf_config->targv[ia];
        dirid = duf_path_to_dirid( path, ( duf_depthinfo_t * ) NULL, &r );
        if ( r >= 0 && dirid )
          r = duf_update_exif( dirid );
        else
          DUF_ERROR( "not found %lld : '%s'\n", dirid, path );
      }
/*										*/ DEBUG_STEP(  );
  }
#endif
  DUF_TEST_R( r );
  /* if ( duf_config->cli.act.print_paths )          */
  /*   duf_print_paths( duf_config->group ); */
/*  --print-dirs								*/ DEBUG_STEP(  );
/*   if ( duf_config->cli.act.print_dirs )                                                                                                 */
/*   {                                                                                                                             */
/* (*                                                                              *) DEBUG_STEP( ); */
/*     if ( duf_config->targc > 0 )                                                                                                */
/*       for ( int ia = 0; ia < duf_config->targc; ia++ )                                                                          */
/*         duf_print_dirs( duf_config->targv[ia], duf_config->u, duf_config->cli.act.tree );                                               */
/*     else                                                                                                                        */
/*       duf_print_dirs( NULL, duf_config->u, duf_config->cli.act.tree );                                                                  */
/* (*                                                                              *) DEBUG_STEP( ); */
/*   }                                                                                                                             */
/*  --print-files								*/ DEBUG_STEP(  );
/*   if ( duf_config->cli.act.print_files )                                                                                                */
/*   {                                                                                                                             */
/*     if ( duf_config->targc > 0 )                                                                                                */
/*       for ( int ia = 0; ia < duf_config->targc; ia++ )                                                                          */
/*         duf_print_files( duf_config->targv[ia], duf_config->u );                                                                */
/*     else                                                                                                                        */
/*       duf_print_files( NULL, duf_config->u );                                                                                   */
/* (*                                                                              *) DEBUG_STEP( ); */
/*   }                                                                                                                             */
/*  --uni-scan									*/ DEBUG_STEP(  );
  if ( r >= 0 && duf_config->cli.act.uni_scan )
  {
    r = duf_uni_scan_all(  );
    /* if ( r == DUF_ERROR_MAX_REACHED )                 */
    /*   DUF_TRACE( action, 0, "Maximum reached ...." ); */
    /* else if ( r < 0 )                                 */
    /*   DUF_ERROR( "code: %d", r );                     */
  }
/*										*/ DEBUG_STEP(  );
  /* if ( duf_config->cli.act.same_md5 )               */
  /*   duf_print_md5_same( 1, duf_config->cli.limit ); */

/*										*/ DEBUG_STEP(  );

/*   if ( duf_config->cli.act.same_files )                                                                                         */
/*   {                                                                                                                             */
/*     if ( !duf_config->targc )                                                                                                   */
/*       DUF_ERROR( "argument needed\n" );                                                                                   */
/*     else                                                                                                                        */
/*       for ( int ia = 0; ia < duf_config->targc; ia++ )                                                                          */
/*         duf_print_files_same( duf_config->targv[ia] );                                                                          */
/* (*                                                                              *) DEBUG_STEP( ); */
/*   }                                                                                                                             */
  /* if ( r >= 0 && duf_config->cli.act.same_exif )     */
  /*   duf_print_exif_same( 1, duf_config->cli.limit ); */

  /*          */ DEBUG_END(  );
  /* if ( r < 0 && !r == DUF_ERROR_MAX_REACHED ) */
  /*   DUF_TEST_R(r);        */
  /* else if ( r < 0 )                           */
  /*   DUF_ERROR( "r=%d", r );                   */
  return r;
}
