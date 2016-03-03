#include <assert.h>                                                  /* assert */
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include "duf_tracen_defs_preset.h"

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ♣ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ♣ */
#include <mastar/trace/mas_trace.h>

#include <mastar/multiconfig/muc_option_names.h>

#include "duf_tracen_defs.h"                                         /* T; TT; TR ♠ */
#include "duf_errorn_defs.h"                                         /* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ */
#include "duf_config_output_weak.h"                                  /* mas_dry_run */

#include "duf_start_end.h"                                           /* DUF_STARTR ; DUF_ENDR ♠ */
#include "duf_dodefs.h"                                              /* DOR ♠ */

#include "duf_printn_defs.h"                                         /* DUF_PRINTF etc. ♠ */

#include "duf_status_ref.h"                                          /* global_status */

/* #include "duf_config_ref.h" */
#include "duf_config.h"                                              /* duf_get_config ♠ */
#include "duf_config_defs.h"                                         /* DUF_CONF... ♠ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ♠ */
#include "duf_config_db.h"                                           /* duf_config_make_db_paths */
/* #include "duf_config_output_util.h" */

/* #include "duf_expandable.h" */

#include "duf_sys_wrap.h"                                            /* duf_unlink() ♠ */

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_path for TRACE */
#include "duf_sql_open.h"                                            /* duf_sql_open() ♠ */
#include "duf_pdi_global.h"                                          /* duf_pdi_global() ♠ */

#include "duf_maindb_info.h"                                         /* duf_main_db_info() ♠ */

#include "duf_evsql_begfin.h"

#include "sql_beginning_common.h"
#include "sql_beginning_tables.h"                                    /* DUF_SQL_TABLES... etc. ♠ */
#include "sql_beginning_vacuum.h"
#include "sql_beginning_create.h"

#include "duf_optimpl_defs.h"
#include "duf_optimpl_enum.h"                                        /* duf_option_code_t ♠ */

/* ###################################################################### */
#include "duf_maindb.h"                                              /* duf_main_db; duf_main_db_open; duf_main_db_close ♠ */
/* ###################################################################### */

static int
duf_main_db_locate( void )
{
  DUF_STARTR( r );

  DOR( r, duf_config_make_db_paths(  ) );

  DUF_ENDR( r );
}

static int
duf_main_db_remove_files( void )
{
  DUF_STARTR( r );
  if ( DUF_CONFIGG( db.main.fpath ) )
    DORF( r, duf_unlink, DUF_CONFIGG( db.main.fpath ) );
  DUF_ENDR( r );
}

static int
duf_main_db_optionally_remove_files( void )
{
  DUF_STARTR( r );
  if ( DUF_ACTG_FLAG( allow_remove_database ) )
  {
    MAST_TRACE( explain, 0, "     option %s, removing database", DUF_OPT_FLAG_NAME( duf_get_config_cli(  ), ALLOW_REMOVE_DATABASE ) );
    DORF( r, duf_main_db_remove_files );
  }
  else
  {
    MAST_TRACE( explain, 1, "no %s option, not removing database", DUF_OPT_FLAG_NAME( duf_get_config_cli(  ), ALLOW_REMOVE_DATABASE ) );
  }
  DUF_ENDR( r );
}

static int
duf_main_db_create_tables( void )
{
  DUF_STARTR( r );
/* DOR( r, duf_check_tables(  ) ); */
  if ( mas_dry_run(  ) )
    DUF_PRINTF( 0, "DRY %s : action '%s'", DUF_OPT_FLAG_NAME( duf_get_config_cli(  ), DRY_RUN ),
                DUF_OPT_FLAG_NAME2( duf_get_config_cli(  ), ALLOW_CREATE_TABLES ) );
  else
  {
    DORF( r, duf_eval_sqlsq, &sql_beginning_create_one, 0, ( const char * ) NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL,
          ( const char * ) NULL /* selected.db */  );
    DORF( r, duf_eval_sqlsq, &sql_beginning_create_two, 0, ( const char * ) NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL,
          ( const char * ) NULL /* selected.db */  );
    DORF( r, duf_eval_sqlsq, &sql_beginning_create_three, 0, ( const char * ) NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL,
          ( const char * ) NULL /* selected.db */  );
    DORF( r, duf_eval_sqlsq, &sql_beginning_create_four, 0, ( const char * ) NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL,
          ( const char * ) NULL /* selected.db */  );
  }
  DUF_ENDR( r );
}

static int
duf_main_db_pre_action( void )
{
  DUF_STARTR( r );
  if ( DUF_NOERROR( r ) && DUF_ACTG_FLAG( allow_drop_tables ) )
  {
    MAST_TRACE( explain, 0, "drop (zero) tables: option %s", DUF_OPT_FLAG_NAME( duf_get_config_cli(  ), ALLOW_DROP_TABLES ) );
    if ( mas_dry_run(  ) )
      DUF_PRINTF( 0, "DRY %s : action '%s'", DUF_OPT_FLAG_NAME( duf_get_config_cli(  ), DRY_RUN ),
                  DUF_OPT_FLAG_NAME2( duf_get_config_cli(  ), ALLOW_DROP_TABLES ) );
    else
      DORF( r, duf_eval_sqlsq, &sql_beginning_drop, 0, NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL,
            NULL /* selected.db */  );
  /* global_status.actions_done++; */
  }
  else
  {
    MAST_TRACE( explain, 1, "no %s option, not dropping tables", DUF_OPT_FLAG_NAME( duf_get_config_cli(  ), ALLOW_DROP_TABLES ) );
  }

  if ( DUF_NOERROR( r ) && DUF_ACTG_FLAG( allow_clean_tables ) )
  {
    MAST_TRACE( explain, 0, "clean (zero) tables: option %s", DUF_OPT_FLAG_NAME( duf_get_config_cli(  ), ALLOW_CLEAN_TABLES ) );
    if ( mas_dry_run(  ) )
      DUF_PRINTF( 0, "DRY %s : action '%s'", DUF_OPT_FLAG_NAME( duf_get_config_cli(  ), DRY_RUN ),
                  DUF_OPT_FLAG_NAME2( duf_get_config_cli(  ), ALLOW_CLEAN_TABLES ) );
    else
      DORF( r, duf_eval_sqlsq, &sql_beginning_clean, 0, NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL,
            NULL /* selected.db */  );
  /* global_status.actions_done++; */
  }
  else
  {
    MAST_TRACE( explain, 1, "no %s option, not dropping tables", DUF_OPT_FLAG_NAME( duf_get_config_cli(  ), ALLOW_DROP_TABLES ) );
  }

  if ( DUF_NOERROR( r ) && DUF_ACTG_FLAG( allow_create_tables ) /* && DUF_ACTG_FLAG( allow_create_database ) */  )
  {
    MAST_TRACE( explain, 0, "     option %s : to check / create db tables", DUF_OPT_FLAG_NAME( duf_get_config_cli(  ), ALLOW_CREATE_TABLES ) );
    DOR( r, duf_main_db_create_tables(  ) );
  /* global_status.actions_done++; */
  }
  else
  {
    MAST_TRACE( explain, 1, "no %s option", DUF_OPT_FLAG_NAME( duf_get_config_cli(  ), ALLOW_CREATE_TABLES ) );
  }

  if ( DUF_NOERROR( r ) && DUF_ACTG_FLAG( allow_vacuum ) )
  {
  /* static const char *sql = "VACUUM"; */

  /* MAST_TRACE( explain, 0, "[ %s ]  option %s", sql, DUF_OPT_FLAG_NAME(duf_get_config_cli(), ALLOW_VACUUM ) ); */
    if ( mas_dry_run(  ) )
      DUF_PRINTF( 0, "DRY %s : action '%s'", DUF_OPT_FLAG_NAME( duf_get_config_cli(  ), DRY_RUN ),
                  DUF_OPT_FLAG_NAME2( duf_get_config_cli(  ), ALLOW_VACUUM ) );
    else
      DORF( r, duf_eval_sqlsq, &sql_beginning_vacuum, 0, NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL,
            NULL /* selected.db */  );
  /* global_status.actions_done++; */
  }
  else
  {
    MAST_TRACE( explain, 1, "no %s option", DUF_OPT_FLAG_NAME( duf_get_config_cli(  ), ALLOW_VACUUM ) );
  }

  DORF( r, duf_eval_sqlsq, &sql_beginning_tables, 0, NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL, NULL /* selected.db */  );

  DUF_ENDR( r );
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

static int
duf_main_db_tune( void )
{
  DUF_STARTR( r );
#ifdef MAS_SPLIT_DB
  if ( DUF_CONFIGG( db.adm.fpath ) )
  {
    static const char *sql = "ATTACH DATABASE '" DUF_ATTACH_COMMON_PATTERN "adm.db' AS " DUF_DBADMALIAS;

    DOR( r, duf_eval_sql_one( sql, ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL, DUF_DBADMALIAS, NULL /* &changes */  ) );
    MAST_TRACE( db, 0, "(%d) %s", r, sql );
  }

# ifndef DUF_SQL_TTABLES_TEMPORARY
  if ( DUF_CONFIGG( db.tempo.fpath ) )
  {
    static const char *sql = "ATTACH DATABASE '" DUF_ATTACH_TTABLES_PATTERN "temp.db' AS " DUF_DBTEMPALIAS;

    DOR( r, duf_eval_sql_one( sql, ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL, DUF_DBTEMPALIAS, NULL /* &changes */  ) );
    MAST_TRACE( db, 0, "(%d) %s", r, sql );
  }
# endif

#endif

/* TODO : part to only after possible tables creation */
  DOR( r, duf_eval_sqlsq( &sql_beginning_common, 0, NULL /* title */ , ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL, NULL /* selected.db */  ) ); /* PRAGMAs etc. */
  DUF_ENDR( r );
}

int
duf_main_db_open( duf_depthinfo_t * pdi )
{
  DUF_STARTR( r );

  assert( pdi );
  assert( !pdi->attached_copy );
/* global_status.db.opened_name */

  MAST_TRACE( db, 5, "@@@@global_status.db.attached_selected:%s", global_status.db.attached_selected );
  MAST_TRACE( db, 5, "@@@@global_status.db.opened_name:%s => CFG->db.main.name:%s", global_status.db.opened_name, DUF_CONFIGGSP( db.main.name_x ) );

/* MAST_TRACE( temp, 0, "db:%s : %s", DUF_CONFIGGS(db.main.name), global_status.db.opened_name ); */
  if ( global_status.db.opened_name && 0 != strcmp( DUF_CONFIGGSP( db.main.name_x ), global_status.db.opened_name ) )
  {
    MAST_TRACE( db, 0, "@@@@autoclose db %s => %s", global_status.db.opened_name, DUF_CONFIGGSP( db.main.name_x ) );
    MAST_TRACE( db, 0, "@@@@global_status.db.attached_selected:%s", global_status.db.attached_selected );
    DOR( r, duf_main_db_close( pdi, 0 ) );
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
    DORF( r, duf_main_db_locate );
    DORF( r, duf_main_db_optionally_remove_files );

    MAST_TRACE( db, 0, "@@@@@to open db %s [%s]", DUF_CONFIGGSP( db.main.name_x ), DUF_CONFIGGS( db.main.fpath ) );
    if ( DUF_CONFIGGS( db.main.fpath ) )
    {
      if ( !DUF_ACTG_FLAG( allow_create_database ) )
      {
        struct stat st;
        int ry = 0;

        ry = stat( DUF_CONFIGGS( db.main.fpath ), &st );
        if ( ry < 0 )
          DUF_MAKE_ERRORM( r, DUF_ERROR_STAT, DUF_CONFIGGS( db.main.fpath ) );
      }
      DORF( r, duf_sql_open, DUF_CONFIGGS( db.main.fpath ) );
    }
    else
    {
      DUF_MAKE_ERROR( r, DUF_ERROR_PTR );
    }
#if 0
    DUF_CONFIGWN( db.opened, ( DUF_NOERROR( r ) ) );
#endif
    if ( DUF_NOERROR( r ) )
      global_status.db.opened_name = mas_strdup( DUF_CONFIGGSP( db.main.name_x ) );
    DORF( r, duf_main_db_tune );
    DORF( r, duf_main_db_pre_action );
    MAST_TRACE( pdi, 0, "pdi-global %s", duf_levinfo_path( duf_pdi_global(  ) ) );
#if 0
  // removed 20151026.200517 - BAD here
    DOR( r, duf_pdi_reinit_min( duf_pdi_global(  ) ) );
#endif
    DUF_CLEAR_ERROR( r, DUF_ERROR_NOT_IN_DB );
  /* if ( DUF_NOERROR( r ) ) */
  /*   r++; (* ???? *)       */
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
  DUF_ENDR( r );
}

int
duf_main_db_close( duf_depthinfo_t * pdi MAS_UNUSED, int ra )
{
  DUF_STARTR( r );
  assert( pdi );
  assert( !pdi->attached_copy );
  r = ra;
#if 0
  if ( DUF_CONFIGG( db.opened ) )
#else
  if ( global_status.db.opened_name )
#endif
  {
    int rt = 0;
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
      DORF( rt, duf_sql_close );
      T( "@duf_sql_close: rt:%d", rt );
      if ( r == 0 && rt < 0 )
        DOR( r, rt );
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
  DUF_ENDR( r );
}

SR( TOP, main_db, int argc MAS_UNUSED, char **argv MAS_UNUSED )
{
  if ( DUF_ACTG_FLAG( info ) )
    CR( main_db_info );

  CR( main_db_close, duf_pdi_global(  ), QERRIND );                  /* [@] */
  ER( TOP, main_db, int argc MAS_UNUSED, char **argv MAS_UNUSED );
}
