/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_seq_structs.h"

#include "duf_pathinfo_credel.h"                                     /* duf_pi_shut; duf_pi_copy; duf_pi_levinfo_create; duf_pi_levinfo_delete etc. ✗ */

#include "duf_sccb_structs.h"                                        /* duf_scan_callbacks_s; duf_sccb_data_row_s; duf_scanner_fun_s; ✗ */

#include "duf_sql_prepared.h"                                        /* duf_sql_prepare; duf_sql_step; duf_sql_finalize; ✗ */
#include "duf_sql_positional.h"                                      /* duf_sql_column_long_long etc. ✗ */

/* ###################################################################### */
#include "duf_sccb_row.h"                                            /* datarow_* ✗ */
/* ###################################################################### */

static MAS_UNUSED
SR( OTHER, datarow_init_field_i, duf_sccb_data_value_t * field, const char *name, duf_sqltype_t typ, const char *svalue, unsigned long long nvalue )
{
  field->typ = typ;
  field->name = mas_strdup( name );
  field->svalue = mas_strdup( svalue );
  field->value.n = nvalue;
  ER( OTHER, datarow_init_field_i, duf_sccb_data_value_t * field, const char *name, duf_sqltype_t typ, const char *value, unsigned long long nvalue );
}

static
SR( OTHER, datarow_init_field, duf_sccb_data_value_t * field, duf_stmnt_t * pstmt_arg, int pos )
{
#if 0
  field->typ = CRX( sql_column_type, pstmt_arg, pos );
  field->name = mas_strdup( CRX( sql_column_name, pstmt_arg, pos ) );
  field->svalue = mas_strdup( CRX( sql_column_string, pstmt_arg, pos ) );
#else
  duf_sqltype_t typ;
  const char *name;

  typ = CRX( sql_column_type, pstmt_arg, pos );
  name = CRX( sql_column_name, pstmt_arg, pos );

  assert( !CRX( sql_column_string, pstmt_arg, pos ) || 0 != strcmp( "std-node", CRX( sql_column_string, pstmt_arg, pos ) ) );
#endif
  switch ( typ )
  {
  case DUF_SQLTYPE_NONE:
#if 0
#else
    CRX( datarow_init_field_i, field, name, typ, CRX( sql_column_string, pstmt_arg, pos ), 0 );
#endif
    break;
  case DUF_SQLTYPE_INTEGER:
#if 0
    field->value.n = CRX( sql_column_long_long, pstmt_arg, pos );
#else
    CRX( datarow_init_field_i, field, name, typ, CRX( sql_column_string, pstmt_arg, pos ), CRX( sql_column_long_long, pstmt_arg, pos ) );
#endif
    break;
  case DUF_SQLTYPE_FLOAT:
    assert( 0 );
    break;
  case DUF_SQLTYPE_TEXT:
#if 0
    field->value.n = CRX( sql_column_long_long, pstmt_arg, pos );
#else
    CRX( datarow_init_field_i, field, name, typ, CRX( sql_column_string, pstmt_arg, pos ), CRX( sql_column_long_long, pstmt_arg, pos ) );
#endif
    break;
  case DUF_SQLTYPE_BLOB:
    assert( 0 );
    break;
  case DUF_SQLTYPE_NULL:
#if 0
#else
    CRX( datarow_init_field_i, field, name, typ, NULL, 0 );
#endif
    break;
  }

  ER( OTHER, datarow_init_field, duf_sccb_data_value_t * field, duf_stmnt_t * pstmt_arg, int pos );
}

SRX( OTHER, duf_sccb_data_row_t *, row, NULL, datarow_create, duf_stmnt_t * pstmt_arg, const duf_pathinfo_t * pi, const seq_t * seqq MAS_UNUSED )
{
/* if ( pstmt_arg ) */
  {
    row = mas_malloc( sizeof( duf_sccb_data_row_t ) );
    memset( row, 0, sizeof( duf_sccb_data_row_t ) );
/* prow=mas_malloc(sizeof(duf_sccb_data_row_t)); */
    row->nfields = pstmt_arg ? CRX( sql_column_count, pstmt_arg ) : 0;

    if ( seqq )
      row->reserved = sizeof( *seqq ) / sizeof( seqq->gen );
    row->fields = mas_malloc( ( row->nfields + row->reserved ) * sizeof( duf_sccb_data_value_t ) );
    memset( row->fields, 0, ( row->nfields + row->reserved ) * sizeof( duf_sccb_data_value_t ) );

    CRX( pi_copy, &row->pathinfo, pi, 0 /* no_li */ , 0 /* no_copy */  );

    if ( row->reserved > 0 )
      CRX( datarow_init_field_i, &row->fields[row->nfields + 0], "seq_gen", DUF_SQLTYPE_INTEGER, NULL, seqq->gen );
    if ( row->reserved > 1 )
      CRX( datarow_init_field_i, &row->fields[row->nfields + 1], "seq_leaf", DUF_SQLTYPE_INTEGER, NULL, seqq->leaf );
    if ( row->reserved > 2 )
      CRX( datarow_init_field_i, &row->fields[row->nfields + 2], "seq_node", DUF_SQLTYPE_INTEGER, NULL, seqq->node );
    if ( row->reserved > 3 )
      CRX( datarow_init_field_i, &row->fields[row->nfields + 3], "seq_row", DUF_SQLTYPE_INTEGER, NULL, seqq->row );

/* QT( "@X %d : %d", pi->levinfo[17].node_type, row->pathinfo.levinfo[17].node_type ); */
    for ( size_t i = 0; pstmt_arg && i < row->nfields; i++ )         /* sqlite3_column_count( pstmt_arg ) */
    {
#if 0
      row->fields[i].typ = CRX( sql_column_type, pstmt_arg, i );
      row->fields[i].name = mas_strdup( CRX( sql_column_name, pstmt_arg, i ) );
      row->fields[i].svalue = mas_strdup( CRX( sql_column_string, pstmt_arg, i ) );
/*     if ( 0 == strcmp( row->fields[i].name, "mtime" ) )                                                                                     */
/*     {                                                                                                                                      */
/* #include <mastar/sqlite/mas_sqlite.h>                                                                                                      */
/*                                                                                                                                            */
/*     (* assert( strcmp( row->fields[i].name, "mtime" ) ); *)                                                                                */
/*       QT( "@%s : %d : %s", CRX( sql_column_name, pstmt_arg, i ), mas_sqlite_column_type( pstmt_arg, i ), mas_sqlite_column_decltype( pstmt_arg, i ) ); */
/*     }                                                                                                                                      */
      switch ( row->fields[i].typ )
      {
      case DUF_SQLTYPE_NONE:
        break;
      case DUF_SQLTYPE_INTEGER:
        row->fields[i].value.n = CRX( sql_column_long_long, pstmt_arg, i );
      /* QT( "field %lu: '%s' = %lld", i, row->fields[i].name, row->fields[i].value.n ); */
        break;
      case DUF_SQLTYPE_FLOAT:
        break;
      case DUF_SQLTYPE_TEXT:
        row->fields[i].value.n = CRX( sql_column_long_long, pstmt_arg, i );
        break;
      case DUF_SQLTYPE_BLOB:
        break;
      case DUF_SQLTYPE_NULL:
        break;
      }
#else
      CR( datarow_init_field, &row->fields[i], pstmt_arg, i );
#endif
      {
#if 0
        const char *s;
        const char *n;
        const char *st;
        duf_sqltype_t it;

        n = CRX( sql_column_name, pstmt_arg, i );                    /* sqlite3_column_name */
        s = CRX( sql_column_string, pstmt_arg, i );                  /* sqlite3_column_text( pstmt_arg, i ) */
        st = CRX( sql_column_decltype, pstmt_arg, i );
        it = CRX( sql_column_type, pstmt_arg, i );
        if ( s )
          QT( "@%lu. %s/%d %s='%s'", i, st, it, n, s );
        else
          QT( "@%lu. %s/%d %s=NULL", i, st, it, n );
#endif
      }
    }
  }
  ERX( OTHER, duf_sccb_data_row_t *, row, NULL, datarow_create, duf_stmnt_t * pstmt_arg, const duf_pathinfo_t * pi, const seq_t * seqq );
}

SRN( OTHER, void, datarow_delete, duf_sccb_data_row_t * row )
{
  if ( row )
  {
    for ( size_t i = 0; i < row->nfields + row->reserved; i++ )
    {
      mas_free( row->fields[i].svalue );
      mas_free( row->fields[i].name );
    }
    CRX( pi_levinfo_delete, &row->pathinfo );
    mas_free( row->fields );
    mas_free( row );
  }
  ERN( OTHER, void, datarow_delete, duf_sccb_data_row_t * row );
}

SRP( OTHER, int, cnt, 0, datarow_list_count, const duf_sccb_data_list_t * rowlist )
{
/* int cnt = 0; */
  duf_sccb_data_row_t *rows;

  rows = rowlist->last_row;
  for ( cnt = 0; rows; rows = rows->prev, cnt++ )
  {
    QT( "@=== cnt: %d - %p : %llu", cnt, rows, CRP( datarow_get_number, rows, "dataid" ) );
  }
/* return cnt; */
  ERP( OTHER, int, cnt, 0, datarow_list_count, const duf_sccb_data_list_t * rowlist );
}

SRX( OTHER, duf_sccb_data_row_t *, row, NULL, datarow_list_first, const duf_sccb_data_list_t * rowlist )
{
  row = rowlist ? rowlist->_first_row : NULL;
  ERX( OTHER, duf_sccb_data_row_t *, row, NULL, datarow_list_first, const duf_sccb_data_list_t * rowlist );
}

SRX( OTHER, duf_sccb_data_row_t *, row, NULL, datarow_list_last, const duf_sccb_data_list_t * rowlist )
{
  row = rowlist ? rowlist->last_row : NULL;
  ERX( OTHER, duf_sccb_data_row_t *, row, NULL, datarow_list_last, const duf_sccb_data_list_t * rowlist );
}

SRX( OTHER, duf_sccb_data_row_t *, row, NULL, datarow_list_next, const duf_sccb_data_row_t * qrow )
{
  row = qrow ? qrow->_next : NULL;
  ERX( OTHER, duf_sccb_data_row_t *, row, NULL, datarow_list_next, const duf_sccb_data_row_t * qrow );
}

SRX( OTHER, duf_sccb_data_row_t *, row, NULL, datarow_list_penult, const duf_sccb_data_list_t * rowlist )
{
  row = rowlist && rowlist->last_row ? rowlist->last_row->prev : NULL;
  assert( !row || row->_next == rowlist->last_row );
  ERX( OTHER, duf_sccb_data_row_t *, row, NULL, datarow_list_penult, const duf_sccb_data_list_t * rowlist );
}

SRX( OTHER, duf_sccb_data_row_t *, row, NULL, datarow_list_unlink, duf_sccb_data_list_t * rowlist, duf_sccb_data_row_t * previous,
     duf_sccb_data_row_t * urow )
{
  duf_sccb_data_row_t *p MAS_UNUSED = urow ? urow->prev : NULL;
  duf_sccb_data_row_t *n MAS_UNUSED = urow ? urow->_next : NULL;

  assert( !previous || previous->prev == urow );
  if ( urow == rowlist->last_row )
    rowlist->last_row = p;
  if ( urow == rowlist->_first_row )
    rowlist->_first_row = n;
  if ( p )
    p->_next = n;
  if ( n )
    n->prev = p;
  if ( previous )
  {
    assert( previous == urow->_next );
    assert( previous->prev == urow->prev );
    previous->prev = urow->prev;
  }
  urow->prev = NULL;
  assert( !p || p->_next == n );
  assert( !n || n->prev == p );
  row = urow;
  ERX( OTHER, duf_sccb_data_row_t *, row, NULL, datarow_list_unlink, duf_sccb_data_list_t * rowlist, duf_sccb_data_row_t * previous,
       duf_sccb_data_row_t * urow );
}

SRX( OTHER, duf_sccb_data_row_t *, row, NULL, datarow_list_link, duf_sccb_data_list_t * rowlist, duf_sccb_data_row_t * lrow )
{
  if ( rowlist && lrow )
  {

    lrow->prev = rowlist->last_row;
    if ( lrow->prev )
      lrow->prev->_next = lrow;
    rowlist->last_row = lrow;
    if ( !rowlist->_first_row )
      rowlist->_first_row = rowlist->last_row;
  /* lrow = NULL; */
  }

  ERX( OTHER, duf_sccb_data_row_t *, row, NULL, datarow_list_link, duf_sccb_data_list_t * rowlist, duf_sccb_data_row_t * lrow );
}

SRN( OTHER, void, datarow_list_delete_f, duf_sccb_data_list_t * rowlist, int min, int max )
{
  int pos;
  duf_sccb_data_row_t *previous = NULL;
  duf_sccb_data_row_t *rows;

  rows = rowlist->last_row;
  pos = 0;
  while ( rows )
  {
    duf_sccb_data_row_t *delrow;

    delrow = rows;
    rows = rows->prev;
    assert( !rows || !rows->prev || rows->prev->_next == rows );
    if ( pos >= min && ( !max || pos <= max ) )
    {
      delrow = CRX( datarow_list_unlink, rowlist, previous, delrow );
      CRX( datarow_delete, delrow );
    }
    else
    {
      previous = delrow;
    }
    pos++;
  }
  ERN( OTHER, void, datarow_list_delete_f, duf_sccb_data_list_t * rowlist, int min, int max );
}

#if 0
SRN( OTHER, void, datarow_list_delete_r, duf_sccb_data_row_t * rows )
{
  if ( rows )
  {
    CRX( datarow_list_delete_r, rows->prev );
    CRX( datarow_delete, rows );
  }
  ERN( OTHER, void, datarow_list_delete_r, duf_sccb_data_row_t * rows );
}
#endif

SRX( OTHER, char *, lst, NULL, datarow_field_list, const duf_sccb_data_row_t * row )
{
  for ( size_t i = 0; row && i < row->nfields; i++ )
  {
    if ( lst )
      lst = mas_strcat_x( lst, "," );
    lst = mas_strcat_x( lst, row->fields[i].name );
  }

  ERX( OTHER, char *, lst, NULL, datarow_field_list, const duf_sccb_data_row_t * row );
}

SRP( OTHER, duf_sccb_data_value_t *, val, NULL, datarow_field_find, const duf_sccb_data_row_t * row, const char *name )
{
  if ( row )
  {
    for ( size_t i = 0; !val && row && i < row->nfields + row->reserved; i++ )
      if ( row->fields[i].name && 0 == strcmp( row->fields[i].name, name ) )
        val = &row->fields[i];
    if ( QNOERR && !val )
    {
      char *lst = NULL;

      lst = CRX( datarow_field_list, row );
      ERRMAKE_M( NO_FIELD, "Bad field name '%s' (%s)", name, lst );
    /* if ( 0 == strcmp( name, "fname" ) ) */
    /*   assert( 0 );                      */
      mas_free( lst );
    }
  }
  ERP( SCCBH, duf_sccb_data_value_t *, val, NULL, datarow_field_find, const duf_sccb_data_row_t * row, const char *name );
}

SRP( OTHER, duf_sqltype_t, typ, DUF_SQLTYPE_NONE, datarow_get_type, const duf_sccb_data_row_t * row, const char *name )
{
  if ( row )
  {
    duf_sccb_data_value_t *field;

    field = CRP( datarow_field_find, row, name );
    if ( QNOERR && field )
      typ = field->typ;
  }
  ERP( SCCBH, duf_sqltype_t, typ, DUF_SQLTYPE_NONE, datarow_get_type, const duf_sccb_data_row_t * row, const char *name );
}

SRP( OTHER, unsigned long long, n, 0, datarow_get_number, const duf_sccb_data_row_t * row, const char *name )
{
  if ( row )
  {
    duf_sccb_data_value_t *field;

    field = CRP( datarow_field_find, row, name );
    if ( QNOERR )
      n = field ? field->value.n : 0;
/* QT( "@found %s for %s at %p (%lld)", field ? field->name : NULL, name, row, field ? field->value.n : 0 ); */
    assert( !field || field->typ == DUF_SQLTYPE_INTEGER || field->typ == DUF_SQLTYPE_NULL || field->typ == DUF_SQLTYPE_TEXT );
  }
  ERP( SCCBH, unsigned long long, n, 0, datarow_get_number, const duf_sccb_data_row_t * row, const char *name );
}

SRP( OTHER, const char *, s, NULL, datarow_get_string, const duf_sccb_data_row_t * row, const char *name )
{
  if ( row )
  {
    duf_sccb_data_value_t *field;

    field = CRP( datarow_field_find, row, name );
    assert( !field || field->typ == DUF_SQLTYPE_TEXT || field->typ == DUF_SQLTYPE_NULL );
    if ( QNOERR )
      s = field ? field->svalue : NULL;
  }
  ERP( SCCBH, const char *, s, NULL, datarow_get_string, const duf_sccb_data_row_t * row, const char *name );
}

SRP( OTHER, int, isnull, 1, datarow_get_null, const duf_sccb_data_row_t * row, const char *name )
{
  if ( row )
  {
    duf_sccb_data_value_t *field;

    field = CRP( datarow_field_find, row, name );
/* assert( !field || field->typ == DUF_SQLTYPE_NULL ); */
    if ( QNOERR )
      isnull = field ? !field->svalue : 1;
  }
  ERP( OTHER, int, isnull, 1, datarow_get_null, const duf_sccb_data_row_t * row, const char *name );
}
