#include <string.h>
#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_defs.h"
#include "duf_maintenance.h"

/* #include "duf_config.h" */
#include "duf_config_ref.h"
#include "duf_utils.h"
#include "duf_service.h"

#include "duf_cli_options.h"
#include "duf_options.h"

#include "duf_sql_defs.h"
#include "duf_sql.h"
#include "duf_sql2.h"

#include "duf_create.h"

#include "duf_add.h"

#include "duf_filedata.h"
#include "duf_uni_scan.h"

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_action.h"
/* ###################################################################### */





static int
duf_store_filters( void )
{
  int r = 0;

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
            " AND ifnull(minsize,0)=ifnull(:minSize,0) " /* */
            " AND ifnull(maxsize,0)=ifnull(:maxSize,0) " /* */
            " AND ifnull(mindups,0)=ifnull(:minDups,0) " /* */
            " AND ifnull(maxdups,0)=ifnull(:maxDups,0) " /* */
            " AND ifnull(glob_include,'')=ifnull(:glob_include,'')" /* */
            " AND ifnull(glob_exclude,'')=ifnull(:glob_exclude,'')" /* */
            ,
      "INSERT INTO " DUF_DBADMPREF "filefilter (type,minsize,maxsize,mindups,maxdups,glob_include,glob_exclude) " /* */
            " VALUES ("         /* */
            " 'cli', :minSize, :maxSize, :minDups, :maxDups, :glob_include, :glob_exclude " /* */
            ")",
      "SELECT " DUF_SQL_IDNAME " FROM " DUF_DBADMPREF "filefilter " /* */
            " WHERE type='cli' " /* */
            " AND ifnull(minsize,0)=ifnull(:minSize,0) " /* */
            " AND ifnull(maxsize,0)=ifnull(:maxSize,0) " /* */
            " AND ifnull(mindups,0)=ifnull(:minDups,0) " /* */
            " AND ifnull(maxdups,0)=ifnull(:maxDups,0) " /* */
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
            if ( r >= 0 && DUF_U_NUM( size.min ) )
              r = duf_sql_bind_long_long_nz( pstmt, ":minSize", DUF_U_NUM( size.min ) );
            DUF_TEST_R( r );
            if ( r >= 0 && DUF_U_NUM( size.max ) )
              r = duf_sql_bind_long_long_nz( pstmt, ":maxSize", DUF_U_NUM( size.max ) );
            /* DUF_TRACE( action, 0, "BIND maxsize %lld", DUF_U_NUM(maxsize )); */
            DUF_TEST_R( r );
            if ( r >= 0 && DUF_U_NUM( same.min ) )
              r = duf_sql_bind_long_long_nz( pstmt, ":minDups", DUF_U_NUM( same.min ) );
            DUF_TEST_R( r );
            if ( r >= 0 && DUF_U_NUM( same.max ) )
              r = duf_sql_bind_long_long_nz( pstmt, ":maxDups", DUF_U_NUM( same.max ) );
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

              /* if ( r == DUF_SQL_ROW )                          */
              /* {                                                */
              /*   long long filtid;                              */
              /*                                                  */
              /*   filtid = duf_sql_column_long_long( pstmt, 0 ); */
              /*   duf_config->u.filter_id = filtid;              */
              /* }                                                */
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
  return r;
}

static int
duf_store_log( int argc, char *const argv[] )
{
  int r = 0;
  char *sargv1, *sargv2;
  int changes = 0;

  sargv1 = mas_argv_string( argc, argv, 1 );
  sargv2 = duf_restore_some_options( argv[0] );
  DUF_TRACE( any, 0, "restored optd:%s", sargv2 );
  {
    static const char *sql = "INSERT OR IGNORE INTO " DUF_DBADMPREF "log (args, restored_args, msg) VALUES (:Args, :restoredArgs, '')";

    DUF_SQL_START_STMT_NOPDI( sql, r, pstmt );
    DUF_SQL_BIND_S( Args, sargv1, r, pstmt );
    DUF_SQL_BIND_S( restoredArgs, sargv2, r, pstmt );
    DUF_SQL_STEP( r, pstmt );
    DUF_SQL_CHANGES_NOPDI( changes, r, pstmt );
    DUF_SQL_END_STMT_NOPDI( r, pstmt );
    /* if ( r == DUF_ERROR_SQL_NO_TABLE ) */
    /*   r = 0;                           */
  }
  DUF_TRACE( action, 0, "LOG inserted %d/%d [%s] - %d", changes, r, sargv1, argc );
  mas_free( sargv2 );
  mas_free( sargv1 );
  return r;
}

int
duf_action_new( int argc, char **argv )
{
  int r = 0;

  DUF_TEST_R( r );
/* --drop-tables								*/ DEBUG_STEP(  );
  if ( r >= 0 && DUF_ACT_FLAG( drop_tables ) )
  {
    DUF_TRACE( explain, 0, "drop (zero) tables: option %s", DUF_OPT_FLAG_NAME( DROP_TABLES ) );
    if ( DUF_CLI_FLAG( dry_run ) )
      DUF_PRINTF( 0, "%s : action '%s'", DUF_OPT_FLAG_NAME( DRY_RUN ), DUF_OPT_FLAG_NAME2( DROP_TABLES ) );
    else
      r = duf_clear_tables(  );
    duf_config->actions_done++;
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option, not dropping tables", DUF_OPT_FLAG_NAME( DROP_TABLES ) );
  }
  DUF_TEST_R( r );
  if ( r >= 0 && DUF_ACT_FLAG( vacuum ) )
  {
    static const char *sql = "VACUUM";

    DUF_TRACE( explain, 0, "[ %s ]  option %s", sql, DUF_OPT_FLAG_NAME( VACUUM ) );
    if ( DUF_CLI_FLAG( dry_run ) )
      DUF_PRINTF( 0, "%s : action '%s'", DUF_OPT_FLAG_NAME( DRY_RUN ), DUF_OPT_FLAG_NAME2( VACUUM ) );
    else
    {
      DUF_SQL_START_STMT_NOPDI( sql, r, pstmt );
      DUF_SQL_STEP( r, pstmt );
      DUF_SQL_END_STMT_NOPDI( r, pstmt );
    }
    duf_config->actions_done++;
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option", DUF_OPT_FLAG_NAME( VACUUM ) );
  }
  DUF_TEST_R( r );
/* --create-tables								*/ DEBUG_STEP(  );
  if ( r >= 0 && DUF_ACT_FLAG( create_tables ) )
  {
    DUF_TRACE( explain, 0, "     option %s : to check / create db tables", DUF_OPT_FLAG_NAME( CREATE_TABLES ) );
    r = duf_check_tables(  );
    duf_config->actions_done++;
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option", DUF_OPT_FLAG_NAME( CREATE_TABLES ) );
  }

  DUF_TEST_R( r );
  if ( r >= 0 )
    r = duf_store_log( argc, argv );
  DUF_TEST_R( r );
  if ( r >= 0 )
    r = duf_store_filters(  );
  DUF_TEST_R( r );

/* --add-path									*/ DEBUG_STEP(  );
  if ( r >= 0 && DUF_ACT_FLAG( add_path ) )
  {
    DUF_TRACE( explain, 0, "     option %s", DUF_OPT_FLAG_NAME( ADD_PATH ) );
    for ( int ia = 0; r >= 0 && ia < duf_config->targc; ia++ )
    {
      DUF_TRACE( explain, 0, "to add (#%d) path %s", ia, duf_config->targv[ia] );
      if ( DUF_CLI_FLAG( dry_run ) )
        DUF_PRINTF( 0, "%s : action '%s'", DUF_OPT_FLAG_NAME( DRY_RUN ), DUF_OPT_FLAG_NAME2( ADD_PATH ) );
      else
        r = duf_add_path_uni( duf_config->targv[ia] );
      duf_config->actions_done++;
    }
    DUF_TEST_R( r );
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option, you may need it for adding initial path", DUF_OPT_FLAG_NAME( ADD_PATH ) );
  }
  /* DUF_PRINTF(0, "@@@@@@@@@@@ %u %u", duf_config->u.md5id.min, duf_config->u.md5id.max );   */
  /* DUF_PRINTF(0, "@@@@@@@@@@@ %u %u", duf_config->u.nameid.min, duf_config->u.nameid.max ); */
  /* DUF_PRINTF(0, "@@@@@@@@@@@ %u %u", duf_config->u.inode.min, duf_config->u.inode.max );   */
  /* DUF_PRINTF(0, "@@@@@@@@@@@ %llu %llu", duf_config->u.size.min, duf_config->u.size.max ); */
  DUF_TEST_R( r );
  if ( r >= 0 && DUF_ACT_FLAG( uni_scan ) )
    r = duf_make_all_sccbs_msg(  );
  return r;
}
