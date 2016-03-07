#ifndef MAS_DUF_SQL_DEFS_H
# define MAS_DUF_SQL_DEFS_H

/* #  define DUF_USE_IDCOL */
# define DUF_SQL_IDFIELD "rowid"
# define DUF_SQL_DIRNAMEFIELD "dir_name"
# define DUF_SQL_FILENAMEFIELD "file_name"
# define DUF_COMMA_AS(_name, alias) , #_name " AS " #alias

# define DUF_SQLITE2R_ERROR_CODE(_r3c) duf_sqlite2r_error_code(_r3c)
# define DUF_SQLITE2R_ERROR_INDEX(_r3c) duf_sqlite2duf(_r3c, FL)

/* ###################################################################### */

/*
#  define DUF_SET_SFIELD(_name) _name = __duf_sql_str_by_name( #_name, precord, NULL, 0 )
#  define DUF_SET_UFIELD(_name) _name = __duf_sql_ull_by_name( #_name, precord, NULL, 0 )

#  define DUF_SFIELD(_name) const char*  DUF_SET_SFIELD(_name)
#  define DUF_UFIELD(_name) unsigned long long  DUF_SET_UFIELD(_name)

#  define DUF_SFIELD_CHECK(_name) int duf_have_field_##_name; const char* _name = __duf_sql_str_by_name( #_name, precord, &duf_have_field_##_name, 0 ); \
  	duf_check_field(#_name, duf_have_field_##_name)
#  define DUF_UFIELD_CHECK(_name) int duf_have_field_##_name; unsigned long long _name = __duf_sql_ull_by_name( #_name, precord, &duf_have_field_##_name, 0 ); \
  	duf_check_field(#_name, duf_have_field_##_name)

#  define DUF_SFIELD_OPT(_name) int duf_have_field_##_name; const char* _name = __duf_sql_str_by_name( #_name, precord, &duf_have_field_##_name, 1 )
#  define DUF_UFIELD_OPT(_name) int duf_have_field_##_name; unsigned long long _name = __duf_sql_ull_by_name( #_name, precord, &duf_have_field_##_name, 1 )
*/
# define DUF_GET_STMT_XFIELD2Q(_pstmt, _typ, _name, _opt)		__duf_sql_ ## _typ ## _by_name2( _pstmt, #_name, _opt )

# define DUF_GET_STMT_SFIELD2Q(_pstmt, _name, _opt)			__duf_sql_str_by_name2( _pstmt, #_name, _opt )
# define DUF_GET_STMT_UFIELD2Q(_pstmt, _name, _opt)			__duf_sql_ull_by_name2( _pstmt, #_name, _opt )

# define DUF_GET_SFIELD2Q(_name, _opt)			DUF_GET_STMT_SFIELD2Q( pstmt, _name, _opt)
# define DUF_GET_UFIELD2Q(_name, _opt)			DUF_GET_STMT_UFIELD2Q( pstmt, _name, _opt)

# define DUF_GET_SFIELD2(_name)			DUF_GET_SFIELD2Q(_name, 0)
# define DUF_GET_UFIELD2(_name)			DUF_GET_UFIELD2Q(_name, 0)
# define DUF_GET_STMT_SFIELD2(_pstmt, _name)		DUF_GET_STMT_SFIELD2Q(_pstmt, _name, 0)
# define DUF_GET_STMT_UFIELD2(_pstmt, _name)		DUF_GET_STMT_UFIELD2Q(_pstmt, _name, 0)

# define DUF_GET_SFIELD2OPT(_name)			DUF_GET_SFIELD2Q(_name, 1)
# define DUF_GET_UFIELD2OPT(_name)			DUF_GET_UFIELD2Q(_name, 1)
# define DUF_GET_STMT_SFIELD2OPT(_pstmt, _name)	DUF_GET_STMT_SFIELD2Q(_pstmt, _name, 1)
# define DUF_GET_STMT_UFIELD2OPT(_pstmt, _name)	DUF_GET_STMT_UFIELD2Q(_pstmt, _name, 1)

# define DUF_SET_SFIELD2(_name)			_name =	DUF_GET_SFIELD2(_name)
# define DUF_SET_UFIELD2(_name)			_name = DUF_GET_UFIELD2(_name)
# define DUF_SET_STMT_SFIELD2(_pstmt, _name)		_name = DUF_GET_STMT_SFIELD2(_pstmt, _name)
# define DUF_SET_STMT_UFIELD2(_pstmt, _name)		_name = DUF_GET_STMT_UFIELD2(_pstmt, _name)

# define DUF_SFIELD2(_name)				const char*         DUF_SET_SFIELD2(_name)
# define DUF_UFIELD2(_name)				unsigned long long  DUF_SET_UFIELD2(_name)
# define DUF_STMT_SFIELD2(_pstmt, _name)		const char*         DUF_SET_STMT_SFIELD2(_pstmt, _name)
# define DUF_STMT_UFIELD2(_pstmt, _name)		unsigned long long  DUF_SET_STMT_UFIELD2(_pstmt, _name)

# define DUF_SET_SFIELD2OPT(_name)			_name = DUF_GET_SFIELD2OPT(_name)
# define DUF_SET_UFIELD2OPT(_name)			_name = DUF_GET_UFIELD2OPT(_name)
# define DUF_SET_STMT_SFIELD2OPT(_pstmt, _name)	_name = DUF_GET_STMT_SFIELD2OPT(_pstmt, _name)
# define DUF_SET_STMT_UFIELD2OPT(_pstmt, _name)	_name = DUF_GET_STMT_UFIELD2OPT(_pstmt, _name)

# define DUF_SFIELD2OPT(_name)				const char*         DUF_SET_SFIELD2OPT(_name)
# define DUF_UFIELD2OPT(_name)				unsigned long long  DUF_SET_UFIELD2OPT(_name)
# define DUF_STMT_SFIELD2OPT(_pstmt, _name)		const char*         DUF_SET_STMT_SFIELD2OPT(_pstmt, _name)
# define DUF_STMT_UFIELD2OPT(_pstmt, _name)		unsigned long long  DUF_SET_STMT_UFIELD2OPT(_pstmt, _name)

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
#  define CRV_SQLITE( _fun, ... )  { int r3=0; r3=_fun(__VA_ARGS__); QERRIND=duf_sqlite2duf(r3, FL); }
# endif
/* #  define DOR3( _rval, _x ) DOR(_rval, DUF_SQLITE2R_ERROR_CODE(_x)) */
#endif
