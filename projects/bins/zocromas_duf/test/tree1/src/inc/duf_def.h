#ifndef MAS_DUF_DEF_H
#  define MAS_DUF_DEF_H

/* #  include <sqlite3.h> */
/* extern sqlite3 *pDb; */
extern char *errmsg;

#  define SQL_ERREQ(rsl,sql,errmsg) { if ( rsl != SQLITE_OK ) fprintf( stderr, "E<<%s:%d>> r=%d '%s' [%s]\n", __FILE__, __LINE__, rsl, errmsg ? errmsg : "-", sql?sql:"?" ); }
#  define SQL_ERREQ_CO(rsl,sql,errmsg) { if ( rsl != SQLITE_OK  && rsl != SQLITE_CONSTRAINT ) fprintf( stderr, "E<<%s:%d>> r=%d '%s' [%s]\n", __FILE__, __LINE__, rsl, errmsg ? errmsg : "-", sql?sql:"?" ); }
#  define SQL_ERRE(rsl,errmsg) SQL_ERREQ(rsl,NULL,errmsg)
#  define SQL_ERR(rsl) SQL_ERRE(rsl,errmsg)
#  define SQL_ERRQ(rsl,sql) SQL_ERREQ(rsl,sql,errmsg)
#  define SQL_EXECC(sql) { int re; char *errmsg = NULL; re = sqlite3_exec( pDb, sql, NULL, NULL, &errmsg ); SQL_ERREQ(re,sql,errmsg) }
#  define SQL_EXEC(sqlf,...) { int re; char *errmsg = NULL; char *sql; sql=sqlite3_mprintf(sqlf, __VA_ARGS__); \
  	re = sqlite3_exec( pDb, sql, NULL, NULL, &errmsg ); SQL_ERREQ(re,sql,errmsg) ; sqlite3_free( sql );  }
#  define SQL_EXEC_CO(sqlf,...) { int re; char *errmsg = NULL; char *sql; sql=sqlite3_mprintf(sqlf, __VA_ARGS__); \
  	re = sqlite3_exec( pDb, sql, NULL, NULL, &errmsg ); SQL_ERREQ_CO(re,sql,errmsg) ; sqlite3_free( sql );  }

#endif

