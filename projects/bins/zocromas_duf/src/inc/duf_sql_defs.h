#ifndef MAS_DUF_SQL_DEFS_H
#  define MAS_DUF_SQL_DEFS_H


/* #  define DUF_USE_IDCOL */
#  define DUF_SQL_IDNAME "rowid"
#define DUF_COMMA_AS(name, alias) , #name " AS " #alias


/* ###################################################################### */

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


#  define DUF_SET_SFIELD2(name) name = __duf_sql_str_by_name2( pstmt, #name, 0 )
#  define DUF_SET_UFIELD2(name) name = __duf_sql_ull_by_name2( pstmt, #name, 0 )

#  define DUF_SFIELD2(name) const char*  DUF_SET_SFIELD2(name)
#  define DUF_UFIELD2(name) unsigned long long  DUF_SET_UFIELD2(name)

#  define DUF_SET_SFIELD2OPT(name) name = __duf_sql_str_by_name2( pstmt, #name, 1 )
#  define DUF_SET_UFIELD2OPT(name) name = __duf_sql_ull_by_name2( pstmt, #name, 1 )

#  define DUF_SFIELD2OPT(name) const char*  DUF_SET_SFIELD2OPT(name)
#  define DUF_UFIELD2OPT(name) unsigned long long  DUF_SET_UFIELD2OPT(name)


#endif
