#ifndef MAS_DUF_SQL_DEFS_H
# define MAS_DUF_SQL_DEFS_H

/* #  define DUF_USE_IDCOL */
# define DUF_SQL_IDFIELD "rowid"
# define DUF_SQL_DIRNAMEFIELD "dir_name"
# define DUF_SQL_FILENAMEFIELD "file_name"
# define DUF_COMMA_AS(_name, alias) , #_name " AS " #alias

# define DUF_SQLITE2R_ERROR_CODE(_r3c) duf_sqlite2r_error_code(_r3c)
# define DUF_SQLITE2R_ERROR_INDEX(_r3c) duf_sqlite2duf(_r3c, FL)

# if 0
#  define DUF_SQLITE2DUF(_r, _r3) (DUF_MAKE_ERROR( _r, duf_sqlite2r_error_code( _r3 ) ), _r)
#  define DOR_SQLITE(_r, _fun)  DOR(_r, DUF_SQLITE2DUF(_r, _fun))
#  define DOR_SQLITE_LOWERE(_r, _fun, ...) DOR_LOWERE( _r, DUF_SQLITE2DUF( _r, _fun ), __VA_ARGS__ )
#  define DOR_SQLITE_LOWERE_N(_n, _r, _fun, ...) DOR_LOWERE_N( _n, _r, DUF_SQLITE2DUF( _r, _fun ), __VA_ARGS__ )
# else
#  define DOR_SQLITE(_r, _fun)			{ int r3=0; r3=_fun; DOR(_r, duf_sqlite2duf(r3, FL)); }
/* #  define DOR_SQLITE_LOWERE(_r, _fun, ...)              { int r3=0; r3=_fun; DOR_LOWERE( _r, duf_sqlite2duf( r3, FL ), __VA_ARGS__ ); } */
/* #  define DOR_SQLITE_LOWERE_N(_n, _r, _fun, ...)        { int r3=0; r3=_fun; DOR_LOWERE_N( _n, _r, duf_sqlite2duf( r3, FL ), __VA_ARGS__ ); } */
/* #  define CR_SQLITE( _fun, ... )  { int r3=0; r3=_fun(__VA_ARGS__); QERRIND=duf_sqlite2duf(r3, FL); } */
#  define CRV_SQLITE( _fun, ... )  { int r3=0; r3=_fun(__VA_ARGS__); QERRIND=duf_sqlite2duf(r3, FL, NULL, NULL); }
#  define CRV_SQLITEQ( _fun, _sql, ... )  { int r3=0; r3=_fun(__VA_ARGS__); QERRIND=duf_sqlite2duf(r3, FL, _sql, NULL); }
#  define CRV_SQLITES( _fun, _stmt, ... )  { int r3=0; r3=_fun(__VA_ARGS__); QERRIND=duf_sqlite2duf(r3, FL, NULL, _stmt); }
#  define CRV_SQLITEQpS( _fun, _sql, _pstmt, ... )  { int r3=0; r3=_fun(__VA_ARGS__); QERRIND=duf_sqlite2duf(r3, FL, _sql, *_pstmt); }
#  define CRV_SQLITEQS( _fun, _sql, _stmt, ... )  { int r3=0; r3=_fun(__VA_ARGS__); QERRIND=duf_sqlite2duf(r3, FL, _sql, _stmt); }
# endif
/* #  define DOR3( _rval, _x ) DOR(_rval, DUF_SQLITE2R_ERROR_CODE(_x)) */
#endif
