#ifndef MAS_DUF_SQL_DEFS_H
#  define MAS_DUF_SQL_DEFS_H


/* #  define DUF_USE_IDCOL */
#  define DUF_SQL_IDFIELD "rowid"
#  define DUF_COMMA_AS(name, alias) , #name " AS " #alias

#  define DUF_SQLITE2R_ERROR_CODE(_r3c) duf_sqlite2r_error_code(_r3c)
#  define DUF_SQLITE2R_ERROR_INDEX(_r3c) duf_sqlite2duf(_r3c, FL)


/* ###################################################################### */

/*
#  define DUF_SET_SFIELD(name) name = __duf_sql_str_by_name( #name, precord, NULL, 0 )
#  define DUF_SET_UFIELD(name) name = __duf_sql_ull_by_name( #name, precord, NULL, 0 )

#  define DUF_SFIELD(name) const char*  DUF_SET_SFIELD(name)
#  define DUF_UFIELD(name) unsigned long long  DUF_SET_UFIELD(name)

#  define DUF_SFIELD_CHECK(name) int duf_have_field_##name; const char* name = __duf_sql_str_by_name( #name, precord, &duf_have_field_##name, 0 ); \
  	duf_check_field(#name, duf_have_field_##name)
#  define DUF_UFIELD_CHECK(name) int duf_have_field_##name; unsigned long long name = __duf_sql_ull_by_name( #name, precord, &duf_have_field_##name, 0 ); \
  	duf_check_field(#name, duf_have_field_##name)

#  define DUF_SFIELD_OPT(name) int duf_have_field_##name; const char* name = __duf_sql_str_by_name( #name, precord, &duf_have_field_##name, 1 )
#  define DUF_UFIELD_OPT(name) int duf_have_field_##name; unsigned long long name = __duf_sql_ull_by_name( #name, precord, &duf_have_field_##name, 1 )
*/

#  define DUF_GET_SFIELD2Q(name, opt) __duf_sql_str_by_name2( pstmt, #name, opt )
#  define DUF_GET_UFIELD2Q(name, opt) __duf_sql_ull_by_name2( pstmt, #name, opt )

#  define DUF_GET_SFIELD2(name)                    DUF_GET_SFIELD2Q(name, 0)
#  define DUF_GET_UFIELD2(name)                    DUF_GET_UFIELD2Q(name, 0)

#  define DUF_GET_SFIELD2OPT(name)                 DUF_GET_SFIELD2Q(name, 1)
#  define DUF_GET_UFIELD2OPT(name)                 DUF_GET_UFIELD2Q(name, 1)

#  define DUF_SET_SFIELD2(name) name =             DUF_GET_SFIELD2(name)
#  define DUF_SET_UFIELD2(name) name =             DUF_GET_UFIELD2(name)

#  define DUF_SFIELD2(name) const char*            DUF_SET_SFIELD2(name)
#  define DUF_UFIELD2(name) unsigned long long     DUF_SET_UFIELD2(name)

#  define DUF_SET_SFIELD2OPT(name) name =          DUF_GET_SFIELD2OPT(name)
#  define DUF_SET_UFIELD2OPT(name) name =          DUF_GET_UFIELD2OPT(name)

#  define DUF_SFIELD2OPT(name) const char*         DUF_SET_SFIELD2OPT(name)
#  define DUF_UFIELD2OPT(name) unsigned long long  DUF_SET_UFIELD2OPT(name)


#  if 0
#    define DUF_SQLITE2DUF(_r, _r3) (DUF_MAKE_ERROR( _r, duf_sqlite2r_error_code( _r3 ) ), _r)
#    define DOR_SQLITE(_r, _fun)  DOR(_r, DUF_SQLITE2DUF(_r, _fun))
#    define DOR_SQLITE_LOWERE(_r, _fun, ...) DOR_LOWERE( _r, DUF_SQLITE2DUF( _r, _fun ), __VA_ARGS__ )
#    define DOR_SQLITE_LOWERE_N(_n, _r, _fun, ...) DOR_LOWERE_N( _n, _r, DUF_SQLITE2DUF( _r, _fun ), __VA_ARGS__ )
#  else
#    define DOR_SQLITE(_r, _fun)  DOR(_r, duf_sqlite2duf(_fun, FL))
#    define DOR_SQLITE_LOWERE(_r, _fun, ...) DOR_LOWERE( _r, duf_sqlite2duf( _fun, FL ), __VA_ARGS__ )
#    define DOR_SQLITE_LOWERE_N(_n, _r, _fun, ...) DOR_LOWERE_N( _n, _r, duf_sqlite2duf( _fun, FL ), __VA_ARGS__ )
#  endif
/* #  define DOR3( _rval, _x ) DOR(_rval, DUF_SQLITE2R_ERROR_CODE(_x)) */
#endif
