#include <assert.h>                                                  /* assert */
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs.h>
#include <mastar/error/mas_error_defs_make.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ▤ */

#include <mastar/multiconfig/muc_option_names.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_printn_defs.h"                                         /* DUF_PRINTF etc. ✗ */

#include "duf_status_ref.h"                                          /* global_status */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */
#include "duf_config_db.h"                                           /* duf_config_make_db_paths */
#include "duf_config_db_get.h"

#include "duf_sys_wrap.h"                                            /* duf_unlink ✗ */

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */
#include "duf_sql_open.h"                                            /* duf_sql_open ✗ */
#include "duf_pdi_global.h"                                          /* duf_pdi_global ✗ */

#include "duf_maindb_info.h"                                         /* duf_main_db_info ✗ */

#include "duf_evsql_begfin.h"                                        /* duf_eval_sqlsq ✗ */

#include "sql_beginning_common.h"
#include "sql_beginning_tables.h"                                    /* DUF_SQL_TABLES... etc. ✗ */
#include "sql_beginning_vacuum.h"
#include "sql_beginning_create.h"

#include "duf_pdi_structs.h"

/* ###################################################################### */
#include "duf_maindb.h"                                              /* duf_main_db; duf_main_db_open; duf_main_db_close ✗ */
/* ###################################################################### */

static
SR( TOP, main_db_locate, void )
{
  CR( config_make_db_paths );
  ER( TOP, main_db_locate, void );
}

static
SR( TOP, main_db_remove_files, void )
{
#if 0
  if ( duf_get_config_db_main_fpath(  ) )
    CR( unlink, duf_get_config_db_main_fpath(  ) );
#else
  {
    const char *fp;

    fp = duf_get_config_db_main_fpath(  );
    if ( fp )
      CR( unlink, fp );
  }
#endif

  ER( TOP, main_db_remove_files, void );
}

static
SR( TOP, main_db_optionally_remove_files, void )
{
  if ( duf_get_config_flag_act_allow_remove_database(  ) )
  {
    MAST_TRACE( explain, 0, "     option %s, removing database", duf_get_config_flagname_act_allow_remove_database( -1 ) );
    CR( main_db_remove_files );
  }
  else
  {
    MAST_TRACE( explain, 1, "no %s option, not removing database", duf_get_config_flagname_act_allow_remove_database( -1 ) );
  }

  ER( TOP, main_db_optionally_remove_files, void );
}

static
SR( TOP, main_db_create_tables, void )
{                                                                    /* DOR( r, duf_check_tables(  ) ); */
  if ( duf_get_config_flag_flow_dry_run(  ) )
    DUF_PRINTF( 0, "DRY %s : action '%s'", duf_get_config_flagname_flow_dry_run( -1 ), duf_get_config_flagname_act_allow_create_tables( 2 ) );
  else
  {
    CR( eval_sqlsq, &sql_beginning_create_one, 0, ( const char * ) NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL,
        ( const char * ) NULL /* selected.db */  );
    CR( eval_sqlsq, &sql_beginning_create_two, 0, ( const char * ) NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL,
        ( const char * ) NULL /* selected.db */  );
    CR( eval_sqlsq, &sql_beginning_create_three, 0, ( const char * ) NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL,
        ( const char * ) NULL /* selected.db */  );
    CR( eval_sqlsq, &sql_beginning_create_four, 0, ( const char * ) NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL,
        ( const char * ) NULL /* selected.db */  );
  }

  ER( TOP, main_db_create_tables, void );
}

static
SR( TOP, main_db_pre_action, void )
{
  if ( QNOERR && duf_get_config_flag_act_allow_drop_tables(  ) )
  {
    MAST_TRACE( explain, 0, "drop (zero) tables: option %s", duf_get_config_flagname_act_allow_drop_tables( -1 ) );
    if ( duf_get_config_flag_flow_dry_run(  ) )
      DUF_PRINTF( 0, "DRY %s : action '%s'", duf_get_config_flagname_flow_dry_run( -1 ), duf_get_config_flagname_act_allow_drop_tables( 2 ) );
    else
      CR( eval_sqlsq, &sql_beginning_drop, 0, NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL,
          NULL /* selected.db */  );
  /* global_status.actions_done++; */
  }
  else
  {
    MAST_TRACE( explain, 1, "no %s option, not dropping tables", duf_get_config_flagname_act_allow_drop_tables( -1 ) );
  }

  if ( QNOERR && duf_get_config_flag_act_allow_clean_tables(  ) )
  {
    MAST_TRACE( explain, 0, "clean (zero) tables: option %s", duf_get_config_flagname_act_allow_clean_tables( -1 ) );
    if ( duf_get_config_flag_flow_dry_run(  ) )
      DUF_PRINTF( 0, "DRY %s : action '%s'", duf_get_config_flagname_flow_dry_run( -1 ), duf_get_config_flagname_act_allow_clean_tables( 2 ) );
    else
      CR( eval_sqlsq, &sql_beginning_clean, 0, NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL,
          NULL /* selected.db */  );
  /* global_status.actions_done++; */
  }
  else
  {
    MAST_TRACE( explain, 1, "no %s option, not dropping tables", duf_get_config_flagname_act_allow_drop_tables( -1 ) );
  }

  if ( QNOERR && duf_get_config_flag_act_allow_create_tables(  ) /* && duf_get_config_flag_act_allow_create_database() */  )
  {
    MAST_TRACE( explain, 0, "     option %s : to check / create db tables", duf_get_config_flagname_act_allow_create_tables( -1 ) );
    CR( main_db_create_tables );
  /* global_status.actions_done++; */
  }
  else
  {
    MAST_TRACE( explain, 1, "no %s option", duf_get_config_flagname_act_allow_create_tables( -1 ) );
  }

  if ( QNOERR && duf_get_config_flag_act_allow_vacuum(  ) )
  {
  /* static const char *sql = "VACUUM"; */

  /* MAST_TRACE( explain, 0, "[ %s ]  option %s", sql, duf_get_config_flagname_act_allow_vacuum(-1) ); */
    if ( duf_get_config_flag_flow_dry_run(  ) )
      DUF_PRINTF( 0, "DRY %s : action '%s'", duf_get_config_flagname_flow_dry_run( -1 ), duf_get_config_flagname_act_allow_vacuum( 2 ) );
    else
      CR( eval_sqlsq, &sql_beginning_vacuum, 0, NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL,
          NULL /* selected.db */  );
  /* global_status.actions_done++; */
  }
  else
  {
    MAST_TRACE( explain, 1, "no %s option", duf_get_config_flagname_act_allow_vacuum( -1 ) );
  }

  CR( eval_sqlsq, &sql_beginning_tables, 0, NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL, NULL /* selected.db */  );

  ER( TOP, main_db_pre_action, void );
}

#ifdef DUF_ATTACH_TTABLES_PATTERN
# ifdef DUF_SQL_TTABLES_TEMPORARY
#  error "Wrong DUF_ATTACH_SELECTED_PATTERN / DUF_SQL_SELECTED_TEMPORARY : add include sql_tables_defs.h"
# endif
#else
# ifndef DUF_SQL_TTABLES_TEMPORARY
#  error "Wrong DUF_ATTACH_SELECTED_PATTERN / DUF_SQL_SELECTED_TEMPORARY : add include sql_tables_defs.h"
# endif
#endif

static
SR( TOP, main_db_tune, void )
{
#ifdef MAS_SPLIT_DB
  if ( duf_get_config_db_adm_fpath(  ) )
  {
    static const char *sql = "ATTACH DATABASE '" DUF_ATTACH_COMMON_PATTERN "adm.db' AS " DUF_DBADMALIAS;

    CR( eval_sql_one, sql, ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL, DUF_DBADMALIAS, NULL /* &changes */  );
    MAST_TRACE( db, 0, "(%d) %s", QERRIND, sql );
  }

# ifndef DUF_SQL_TTABLES_TEMPORARY
  if ( duf_get_config_db_tempo_fpath(  ) )
  {
    static const char *sql = "ATTACH DATABASE '" DUF_ATTACH_TTABLES_PATTERN "temp.db' AS " DUF_DBTEMPALIAS;

    CR( eval_sql_one, sql, ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL, DUF_DBTEMPALIAS, NULL /* &changes */  );
    MAST_TRACE( db, 0, "(%d) %s", QERRIND, sql );
  }
# endif

#endif

/* TODO : part to only after possible tables creation */
  CR( eval_sqlsq, &sql_beginning_common, 0, NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL, NULL /* selected.db */  ); /* PRAGMAs etc. */

  ER( TOP, main_db_tune, void );
}

SR( TOP, main_db_open, duf_depthinfo_t * pdi )
{
  assert( pdi );
  assert( !pdi->attached_copy );
/* global_status.db.opened_name */

  MAST_TRACE( db, 5, "@@@@global_status.db.attached_selected:%s", global_status.db.attached_selected );
  MAST_TRACE( db, 5, "@@@@global_status.db.opened_name:%s => CFG->db.main.name:%s", global_status.db.opened_name, duf_get_config_db_main_name(  ) );

/* MAST_TRACE( temp, 0, "db:%s : %s", DUF_CONFIGGS(db.main.name), global_status.db.opened_name ); */
  if ( global_status.db.opened_name && 0 != strcmp( duf_get_config_db_main_name(  ), global_status.db.opened_name ) )
  {
    MAST_TRACE( db, 0, "@@@@autoclose db %s => %s", global_status.db.opened_name, duf_get_config_db_main_name(  ) );
    MAST_TRACE( db, 0, "@@@@global_status.db.attached_selected:%s", global_status.db.attached_selected );
    CR( main_db_close, pdi );
    MAST_TRACE( db, 0, "@@@@global_status.db.attached_selected:%s", global_status.db.attached_selected );
  }
  MAST_TRACE( db, 5, "@@@@global_status.db.attached_selected:%s", global_status.db.attached_selected );
#if 0
  if ( !DUF_CONFIGG( db.opened ) )
#else
  if ( !global_status.db.opened_name )
#endif
  {
    assert( !global_status.db.attached_selected );
    CR( main_db_locate );
    CR( main_db_optionally_remove_files );

    MAST_TRACE( db, 0, "@@@@@to open db %s [%s]", duf_get_config_db_main_name(  ), duf_get_config_db_main_fpath(  ) );
    if ( duf_get_config_db_main_fpath(  ) )
    {
      if ( !duf_get_config_flag_act_allow_create_database(  ) )
      {
        struct stat st;
        int ry = 0;

        ry = stat( duf_get_config_db_main_fpath(  ), &st );
        if ( ry < 0 )
          ERRMAKE_M( STAT, duf_get_config_db_main_fpath(  ) );
      }
      CR( sql_open, duf_get_config_db_main_fpath(  ) );
    }
    else
    {
      ERRMAKE( PTR );
    }
#if 0
    DUF_CONFIGWN( db.opened, ( DUF_NOERROR( r ) ) );
#endif
    if ( QNOERR )
      global_status.db.opened_name = mas_strdup( duf_get_config_db_main_name(  ) );
    CR( main_db_tune );
    CR( main_db_pre_action );
    MAST_TRACE( pdi, 0, "pdi-global %s", duf_levinfo_path( duf_pdi_global(  ) ) );
#if 0
  // removed 20151026.200517 - BAD here
    DOR( r, duf_pdi_reinit_min( duf_pdi_global(  ) ) );
#endif
    ERRCLEAR( NOT_IN_DB );
  }
  {
    duf_depthinfo_t *pdis;

    pdis = global_status.pdilist;
    while ( pdis && pdis != pdi )
      pdis = pdis->next;
    if ( !pdis )
    {
      assert( !pdi->next );
      pdi->next = global_status.pdilist;
      assert( !pdi->attached_copy );
      global_status.pdilist = pdi;
      MAST_TRACE( db, 20, "@  link (%-14p=>%-14p) pdi:%-14p => %-14p", global_status.pdilist, global_status.pdilist->next, pdi, pdi->next );
    }
  }

  ER( TOP, main_db_open, duf_depthinfo_t * pdi );
}

SR( TOP, main_db_close, duf_depthinfo_t * pdi )
{
  assert( pdi );
  assert( !pdi->attached_copy );
/* QERRIND = ra; */
#if 0
  if ( DUF_CONFIGG( db.opened ) )
#else
  if ( global_status.db.opened_name )
#endif
  {
    duf_depthinfo_t *pdis = NULL;

    if ( pdi->next )
    {
      duf_depthinfo_t *prev_pdis = NULL;

      pdis = global_status.pdilist;
      while ( pdis && pdis != pdi )
      {
        prev_pdis = pdis;
        pdis = pdis->next;
      }
      if ( pdis )
      {
        if ( prev_pdis )
          prev_pdis->next = pdis->next;
        else
          global_status.pdilist = pdis->next;
        MAST_TRACE( db, 20, "@unlink (%-14p=>%-14p) pdi:%-14p => %-14p", global_status.pdilist, global_status.pdilist->next, pdi, pdi->next );
        pdis->next = NULL;
      }
    }
    if ( pdis && !global_status.pdilist )                            /* close only if opened for this pdi */
    {
      MAST_TRACE( db, 0, "@@@@closing db %s", global_status.db.opened_name );
    /* don't DOR it directly! call allways! */
      CR( sql_close );
      QT( "@duf_sql_close: errind:%d", QERRIND );
#if 0
      DUF_CONFIGWN( db.opened, !( DUF_NOERROR( r ) ) );
#else
      mas_free( global_status.db.opened_name );
      global_status.db.opened_name = NULL;
#endif
      mas_free( global_status.db.attached_selected );
      global_status.db.attached_selected = NULL;
    }
  }

  ER( TOP, main_db_close, duf_depthinfo_t * pdi MAS_UNUSED );
}

SR( TOP, main_db, int argc MAS_UNUSED, char **argv MAS_UNUSED )
{
  if ( duf_get_config_flag_act_info(  ) )
    CR( main_db_info );

  CR( main_db_close, duf_pdi_global(  ) );                           /* [@] */
  ER( TOP, main_db, int argc MAS_UNUSED, char **argv MAS_UNUSED );
}
