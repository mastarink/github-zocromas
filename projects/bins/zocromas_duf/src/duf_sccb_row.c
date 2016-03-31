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
#include "duf_config_output_util.h"

#include "duf_sql_prepared.h"                                        /* duf_sql_prepare; duf_sql_step; duf_sql_finalize; ✗ */
#include "duf_sql_positional.h"                                      /* duf_sql_column_long_long etc. ✗ */

#include "duf_sccb_structs.h"
#include "duf_sccbh_shortcuts.h"                                     /* H_SCCB; H_PDI; H_* ... ✗ */

#include "duf_pathinfo_credel.h"                                     /* duf_pi_shut; duf_pi_copy; duf_pi_levinfo_create; duf_pi_levinfo_delete etc. ✗ */
#include "duf_pathinfo_ref.h"

#include "duf_levinfo_structs.h"

/* ###################################################################### */
#include "duf_sccb_row.h"
/* ###################################################################### */

/* duf_sccb_data_row_t *                                                           */
/* duf_datarow_create( duf_stmnt_t * pstmt, const duf_pathinfo_t * pi MAS_UNUSED ) */
SRX( OTHER, duf_sccb_data_row_t *, row, NULL, datarow_create, duf_stmnt_t * pstmt, const duf_pathinfo_t * pi MAS_UNUSED )
{
/* duf_sccb_data_row_t *row = NULL; */

  row = mas_malloc( sizeof( duf_sccb_data_row_t ) );
  memset( row, 0, sizeof( duf_sccb_data_row_t ) );
/* prow=mas_malloc(sizeof(duf_sccb_data_row_t)); */
  row->cnt = CRX( sql_column_count, pstmt );

  row->fields = mas_malloc( row->cnt * sizeof( duf_sccb_data_value_t ) );
  memset( row->fields, 0, row->cnt * sizeof( duf_sccb_data_value_t ) );

  CRX( pi_copy, &row->pathinfo, pi, 0 /* no_li */ , 0 /* no_copy */  );

/* QT( "@X %d : %d", pi->levinfo[17].node_type, row->pathinfo.levinfo[17].node_type ); */

  for ( size_t i = 0; i < row->cnt; i++ )                            /* sqlite3_column_count( pstmt ) */
  {
    row->fields[i].typ = CRX( sql_column_type, pstmt, i );
    row->fields[i].name = mas_strdup( CRX( sql_column_name, pstmt, i ) );
    row->fields[i].svalue = mas_strdup( CRX( sql_column_string, pstmt, i ) );
/*     if ( 0 == strcmp( row->fields[i].name, "mtime" ) )                                                                                     */
/*     {                                                                                                                                      */
/* #include <mastar/sqlite/mas_sqlite.h>                                                                                                      */
/*                                                                                                                                            */
/*     (* assert( strcmp( row->fields[i].name, "mtime" ) ); *)                                                                                */
/*       QT( "@%s : %d : %s", CRX( sql_column_name, pstmt, i ), mas_sqlite_column_type( pstmt, i ), mas_sqlite_column_decltype( pstmt, i ) ); */
/*     }                                                                                                                                      */
    switch ( row->fields[i].typ )
    {
    case DUF_SQLTYPE_NONE:
      break;
    case DUF_SQLTYPE_INTEGER:
      row->fields[i].value.n = CRX( sql_column_long_long, pstmt, i );
    /* QT( "field %lu: '%s' = %lld", i, row->fields[i].name, row->fields[i].value.n ); */
      break;
    case DUF_SQLTYPE_FLOAT:
      break;
    case DUF_SQLTYPE_TEXT:
      row->fields[i].value.n = CRX( sql_column_long_long, pstmt, i );
      break;
    case DUF_SQLTYPE_BLOB:
      break;
    case DUF_SQLTYPE_NULL:
      break;
    }
    {
#if 0
      const char *s;
      const char *n;
      const char *st;
      duf_sqltype_t it;

      n = CRX( sql_column_name, pstmt, i );                          /* sqlite3_column_name */
      s = CRX( sql_column_string, pstmt, i );                        /* sqlite3_column_text( pstmt, i ) */
      st = CRX( sql_column_decltype, pstmt, i );
      it = CRX( sql_column_type, pstmt, i );
      if ( s )
        QT( "@%lu. %s/%d %s='%s'", i, st, it, n, s );
      else
        QT( "@%lu. %s/%d %s=NULL", i, st, it, n );
#endif
    }
  }
/* return row; */
  ERX( OTHER, duf_sccb_data_row_t *, row, NULL, datarow_create, duf_stmnt_t * pstmt, const duf_pathinfo_t * pi MAS_UNUSED );
}

/* int                                                        */
/* duf_datarow_list_count( const duf_sccb_data_row_t * rows ) */
SRX( OTHER, int, cnt, 0, datarow_list_count, const duf_sccb_data_row_t * rows )
{
/* int cnt = 0; */

  for ( cnt = 0; rows; rows = rows->prev, cnt++ )
  {
    QT( "@=== cnt: %d - %p : %llu", cnt, rows, CRX( datarow_get_number, rows, "dataid" ) );
  }
/* return cnt; */
  ERX( OTHER, int, cnt, 0, datarow_list_count, const duf_sccb_data_row_t * rows );
}

/* void                                                    */
/* duf_datarow_list_delete_r( duf_sccb_data_row_t * rows ) */
SRN( OTHER, void, datarow_list_delete_r, duf_sccb_data_row_t * rows )
{
  if ( rows )
  {
    CRX( datarow_list_delete_r, rows->prev );
    CRX( datarow_delete, rows );
  }
  ERN( OTHER, void, datarow_list_delete_r, duf_sccb_data_row_t * rows );
}

/* void                                                              */
/* duf_datarow_list_delete_f( duf_sccb_data_row_t * rows, int skip ) */
SRN( OTHER, void, datarow_list_delete_f, duf_sccb_data_row_t * rows, int skip )
{
  duf_sccb_data_row_t *row = NULL;
  duf_sccb_data_row_t *prev = NULL;

  while ( rows )
  {
    row = rows;
    rows = row->prev;
    if ( skip-- <= 0 )
    {
      CRX( datarow_delete, row );
      if ( prev )
        prev->prev = NULL;
    }
    else
      prev = row;
  }
  ERN( OTHER, void, datarow_list_delete_f, duf_sccb_data_row_t * rows, int skip );
}

/* void                                            */
/* duf_datarow_delete( duf_sccb_data_row_t * row ) */
SRN( OTHER, void, datarow_delete, duf_sccb_data_row_t * row )
{
  for ( size_t i = 0; i < row->cnt; i++ )
  {
    mas_free( row->fields[i].svalue );
    mas_free( row->fields[i].name );
  }
  CRX( pi_levinfo_delete, &row->pathinfo );
  mas_free( row->fields );
  mas_free( row );
  ERN( OTHER, void, datarow_delete, duf_sccb_data_row_t * row );
}

/* duf_sccb_data_value_t *                                                     */
/* duf_datarow_field_find( const duf_sccb_data_row_t * row, const char *name ) */
SRX( OTHER, duf_sccb_data_value_t *, val, NULL, datarow_field_find, const duf_sccb_data_row_t * row, const char *name )
{
  for ( size_t i = 0; !val && row && i < row->cnt; i++ )
    if ( 0 == strcmp( row->fields[i].name, name ) )
      val = &row->fields[i];
/* return NULL; */
  ERX( SCCBH, duf_sccb_data_value_t *, val, NULL, datarow_field_find, const duf_sccb_data_row_t * row, const char *name );
}

/* unsigned long long                                                          */
/* duf_datarow_get_number( const duf_sccb_data_row_t * row, const char *name ) */
SRX( OTHER, unsigned long long, n, 0, datarow_get_number, const duf_sccb_data_row_t * row, const char *name )
{
  duf_sccb_data_value_t *field;

  field = CRX( datarow_field_find, row, name );
  n = field ? field->value.n : 0;
/* QT( "@found %s for %s at %p (%lld)", field ? field->name : NULL, name, row, field ? field->value.n : 0 ); */
  assert( !field || field->typ == DUF_SQLTYPE_INTEGER || field->typ == DUF_SQLTYPE_NULL || field->typ == DUF_SQLTYPE_TEXT );
/* return field ? field->value.n : 0; */
  ERX( SCCBH, unsigned long long, n, 0, datarow_get_number, const duf_sccb_data_row_t * row, const char *name );
}

/* const char *                                                          */
/* duf_datarow_get_string( duf_sccb_data_row_t * row, const char *name ) */
SRX( OTHER, const char *, s, NULL, datarow_get_string, duf_sccb_data_row_t * row, const char *name )
{
  duf_sccb_data_value_t *field;

  field = CRX( datarow_field_find, row, name );
  assert( !field || field->typ == DUF_SQLTYPE_TEXT || field->typ == DUF_SQLTYPE_NULL );
  s = field ? field->svalue : NULL;
/* return field ? field->svalue : NULL; */
  ERX( SCCBH, const char *, s, NULL, datarow_get_string, duf_sccb_data_row_t * row, const char *name );
}

#if 1
unsigned long long
duf_sccbh_row_get_number( duf_sccb_handle_t * sccbh, const char *name )
/* SRX( SCCBH, unsigned long long, n, 0, sccbh_row_get_number, duf_sccb_handle_t * sccbh, const char *name ) */
{
  unsigned long long n = 0;

  n = sccbh ? CRX( datarow_get_number, sccbh->rows, name ) : 0;
  return n;
/* ERX( SCCBH, unsigned long long, n, 0, sccbh_row_get_number, duf_sccb_handle_t * sccbh, const char *name ); */
}

const char *
duf_sccbh_row_get_string( duf_sccb_handle_t * sccbh, const char *name )
/* SRX( SCCBH, const char *, s, NULL, sccbh_row_get_string, duf_sccb_handle_t * sccbh, const char *name ) */
{
  const char *s = NULL;

  s = sccbh ? CRX( datarow_get_string, sccbh->rows, name ) : NULL;
  return s;
/* ERX( SCCBH, const char *, s, NULL, sccbh_row_get_string, duf_sccb_handle_t * sccbh, const char *name ); */
}
#endif
/* void                                             */
/* duf_sccbh_rows_eval( duf_sccb_handle_t * sccbh ) */
SRN( SCCBH, void, sccbh_rows_eval, duf_sccb_handle_t * sccbh )
{
  int n = 0;

/* TODO move to new sccb->shut callback */
  for ( duf_sccb_data_row_t * trow = sccbh->rows; trow; trow = trow->prev )
  {
    const char *path;
    const char *rpath;
    const char *iname;

    path = CRX( pi_path, &trow->pathinfo );
    rpath = CRX( pi_relpath, &trow->pathinfo );
    iname = CRX( pi_itemname, &trow->pathinfo );
    QT( "@@@@@%d. %-10s: %s : %s", n, H_SCCB->name, path, iname );
    QT( "@@@@@@%d. %-10s: %s : %s", n, H_SCCB->name, rpath, iname );
    n++;
  }
  CRX( datarow_list_delete_f, sccbh->rows, 0 );
  QTT( "@@@@ ===========================================" );
  sccbh->rows = NULL;
  ERN( SCCBH, void, sccbh_rows_eval, duf_sccb_handle_t * sccbh );
}
