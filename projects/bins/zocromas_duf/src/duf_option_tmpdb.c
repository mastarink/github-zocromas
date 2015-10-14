#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance_options.h"

#include "evsql_begfin.h"
#include "duf_sql_stmt_defs.h"
#include "duf_sql_bind.h"

#include "sql_beginning_tables.h"

/* ###################################################################### */
#include "duf_option_tmpdb.h"
/* ###################################################################### */
/* #define DUF_SQL_TMP_TEMPORARY_STRING  "TEMPORARY" */


static int
duf_bind_tmp( duf_stmnt_t * pstmt, const duf_ufilter_t * pu_unused DUF_UNUSED, const duf_yfilter_t * py_unused DUF_UNUSED, const mas_argvc_t * ttarg )
{
  DEBUG_STARTR( r );
  if ( ttarg && ttarg->argc >= 2 && ttarg->argv )
  {
    /* DUF_TRACE( temp, 0, "%d:0 >>>> %s", ttarg->argc, ttarg->argv[0] ); */
    /* DUF_TRACE( temp, 0, "%d:1 >>>> %s", ttarg->argc, ttarg->argv[1] ); */
    /* DUF_TRACE( temp, 0, "%d:2 >>>> %s", ttarg->argc, ttarg->argv[2] ); */
    DUF_SQL_BIND_S_OPT( optVal, ttarg->argv[0], r, pstmt );
    DUF_SQL_BIND_S_OPT( optName, ttarg->argv[1], r, pstmt );
    DUF_SQL_BIND_S_OPT( optArg, ttarg->argv[2], r, pstmt );
    DUF_TRACE( sql, 0, "@@@@bind: argv[]={'%s', '%s', '%s'}", ttarg->argv[0], ttarg->argv[1], ttarg->argv[2] );
  }
  DEBUG_ENDR( r );
}

int
duf_tmpdb_add( int oval, const char *optname, const char *optargg )
{
  DEBUG_STARTR( r );
  static duf_sql_sequence_t tmpseq = { /* */
    .name = "tmpseq",
    .sql = {
            /* "DELETE FROM " DUF_SQL_TABLES_TMP_TDB_OPTIONS_FULL " WHERE oval=:optVal AND name=:optName AND arg=:optArg" (* *) , */
            "INSERT OR IGNORE INTO " DUF_SQL_TABLES_TMP_TDB_OPTIONS_FULL " (oval, name, arg) VALUES (:optVal, :optName, :optArg) " /* */ ,
            NULL}
  };
  char optval[512];
  mas_argvc_t ttarg = {.argc = 0,.argv = NULL };
  snprintf( optval, sizeof( optval ), "%d", oval );
  ttarg.argc = mas_add_argv_arg( ttarg.argc, &ttarg.argv, optval );
  ttarg.argc = mas_add_argv_arg( ttarg.argc, &ttarg.argv, optname );
  ttarg.argc = mas_add_argv_arg( ttarg.argc, &ttarg.argv, optargg );

  DOR( r, duf_eval_sqlsq_cb( &tmpseq, NULL /* title */ , ( duf_ufilter_t * ) NULL , ( duf_yfilter_t * ) NULL , duf_bind_tmp, &ttarg, NULL /* selected_db */  ) );
  mas_del_argv( ttarg.argc, ttarg.argv, 0 );
  DEBUG_ENDR( r );
}
