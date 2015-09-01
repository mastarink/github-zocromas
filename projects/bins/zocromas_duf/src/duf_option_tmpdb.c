#include <assert.h>
#include <string.h>

#include <mastar/wrap/mas_std_def.h>
/* #include <mastar/wrap/mas_memory.h> */
#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

#include "duf_begfin.h"
#include "duf_sql2.h"

#include "sql_beginning_tables.h"

/* ###################################################################### */
#include "duf_option_tmpdb.h"
/* ###################################################################### */
/* #define DUF_SQL_TMP_TEMPORARY_STRING  "TEMPORARY" */


static int
duf_bind_tmp( duf_sqlite_stmt_t * pstmt, const duf_argvc_t * ttarg )
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
  }

  DEBUG_ENDR( r );
}

int
duf_tmpdb_add( int oval, const char *optname, const char *optargg )
{
  DEBUG_STARTR( r );
  static duf_sql_sequence_t DUF_UNUSED tmpseq = { /* */
    .sql = {
            "INSERT OR IGNORE INTO " DUF_SQL_TABLES_TMP_TDB_OPTIONS " (oval, name, arg) VALUES (:optVal, :optName, :optArg) " /* */
            , NULL}
  };
  char optval[512];
  duf_argvc_t ttarg = {.argc = 0,.argv = NULL };

  snprintf( optval, sizeof( optval ), "%d", oval );
  ttarg.argc = mas_add_argv_arg( ttarg.argc, &ttarg.argv, optval );
  ttarg.argc = mas_add_argv_arg( ttarg.argc, &ttarg.argv, optname );
  ttarg.argc = mas_add_argv_arg( ttarg.argc, &ttarg.argv, optargg );

  DOR( r, duf_eval_sql_sequence_cb( &tmpseq, NULL, duf_bind_tmp, &ttarg ) );
  mas_del_argv( ttarg.argc, ttarg.argv, 0 );
  DEBUG_ENDR( r );
}
