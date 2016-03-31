#ifndef MAS_DUF_SQL_FIELD_DEFS_H
# define MAS_DUF_SQL_FIELD_DEFS_H

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

# define DUF_GET_STMT_SFIELD2(_pstmt, _name)		DUF_GET_STMT_SFIELD2Q(_pstmt, _name, 0)
# define DUF_GET_STMT_UFIELD2(_pstmt, _name)		DUF_GET_STMT_UFIELD2Q(_pstmt, _name, 0)

# define DUF_GET_STMT_SFIELD2OPT(_pstmt, _name)	DUF_GET_STMT_SFIELD2Q(_pstmt, _name, 1)
# define DUF_GET_STMT_UFIELD2OPT(_pstmt, _name)	DUF_GET_STMT_UFIELD2Q(_pstmt, _name, 1)

# define DUF_SET_STMT_SFIELD2(_pstmt, _name)		_name = DUF_GET_STMT_SFIELD2(_pstmt, _name)
# define DUF_SET_STMT_UFIELD2(_pstmt, _name)		_name = DUF_GET_STMT_UFIELD2(_pstmt, _name)

# define DUF_STMT_SFIELD2(_pstmt, _name)		const char*         DUF_SET_STMT_SFIELD2(_pstmt, _name)
# define DUF_STMT_UFIELD2(_pstmt, _name)		unsigned long long  DUF_SET_STMT_UFIELD2(_pstmt, _name)

# define DUF_SET_STMT_SFIELD2OPT(_pstmt, _name)	_name = DUF_GET_STMT_SFIELD2OPT(_pstmt, _name)
# define DUF_SET_STMT_UFIELD2OPT(_pstmt, _name)	_name = DUF_GET_STMT_UFIELD2OPT(_pstmt, _name)

# define DUF_STMT_SFIELD2OPT(_pstmt, _name)		const char*         DUF_SET_STMT_SFIELD2OPT(_pstmt, _name)
# define DUF_STMT_UFIELD2OPT(_pstmt, _name)		unsigned long long  DUF_SET_STMT_UFIELD2OPT(_pstmt, _name)

#endif
