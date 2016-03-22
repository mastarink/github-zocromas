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

/* #include "duf_tracen_defs.h"                                         (* T; TT; TR ✗ *) */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ✗ *) */

/* #include "duf_start_end.h"                                           (* DUF_STARTR ; DUF_ENDR ✗ *) */
/* #include "duf_dodefs.h"                                              (* DOR ✗ *) */

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config.h"                                              /* duf_get_config ✗ */

#include "duf_pdi_credel.h"                                          /* duf_pdi_create; duf_pdi_kill ✗ */
#include "duf_pdi_reinit.h"
#include "duf_pdi_filters.h"                                         /* duf_pdi_pu; etc. ✗ */
#include "duf_pdi_ref.h"
#include "duf_pdi_pi_ref.h"
#include "duf_pdi_stmt.h"                                            /* duf_pdi_find_statement_by_id; etc. ✗ */

#include "duf_utils.h"                                               /* duf_percent;  etc. ✗ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */
/* #include "duf_config_ref.h" */
/* #include "duf_config_defs.h"                                         (* DUF_CONF... ✗ *) */

#include "duf_config_output_util.h"

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */

/* #include "duf_sql_stmt_defs.h"                                       (* DUF_SQL_BIND_S_OPT etc. ✗ *) */
#include "duf_sql_se_stmt_defs.h"                                    /* DUF_SQL_SE_BIND_S_OPT etc. ✗ */
#include "duf_sql_prepared.h"                                        /* duf_sql_(prepare|step|finalize) ✗ */
#include "duf_sql_bind.h"                                            /* duf_sql_... for DUF_SQL_BIND_... etc. ✗ */

#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ✗ */
#include "duf_sql_field.h"                                           /* __duf_sql_str_by_name2 for DUF_GET_UFIELD2 etc. ✗ */

#include "duf_sql_positional.h"                                      /* duf_sql_column_long_long etc. ✗ */

#include "duf_sccb_def.h"
#include "duf_sccb.h"

#include "duf_evsql_selector_new.h"

#include "std_mod_sets.h"

#include "duf_sccb_begfin.h"
#include "duf_ufilter_bind.h"

#include "duf_sccb_scanstage.h"

#include "duf_sccbh_shortcuts.h"
/* ###################################################################### */
#include "duf_sccb_row.h"
/* ###################################################################### */

duf_sccb_data_row_t
duf_sccb_row_create( duf_stmnt_t * pstmt )
{
  duf_sccb_data_row_t row = {.cnt = 0,.fields = NULL };
/* prow=mas_malloc(sizeof(duf_sccb_data_row_t)); */
  row.cnt = duf_sql_column_count( pstmt );
  row.fields = mas_malloc( row.cnt * sizeof( duf_sccb_data_value_t ) );
  memset( row.fields, 0, row.cnt * sizeof( duf_sccb_data_value_t ) );
  for ( size_t i = 0; i < row.cnt; i++ )                             /* sqlite3_column_count( pstmt ) */
  {
    row.fields[i].typ = duf_sql_column_type( pstmt, i );
    row.fields[i].name = mas_strdup( duf_sql_column_name( pstmt, i ) );
    row.fields[i].svalue = mas_strdup( duf_sql_column_string( pstmt, i ) );
    switch ( row.fields[i].typ )
    {
    case DUF_SQLTYPE_NONE:
      break;
    case DUF_SQLTYPE_INTEGER:
      row.fields[i].value.n = duf_sql_column_long_long( pstmt, i );
      break;
    case DUF_SQLTYPE_FLOAT:
      break;
    case DUF_SQLTYPE_TEXT:
    /* row.fields[i].value.n = duf_sql_column_long_long( pstmt, i ); */
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

      n = duf_sql_column_name( pstmt, i );                           /* sqlite3_column_name */
      s = duf_sql_column_string( pstmt, i );                         /* sqlite3_column_text( pstmt, i ) */
      st = duf_sql_column_decltype( pstmt, i );
      it = duf_sql_column_type( pstmt, i );
      if ( s )
        QT( "@%lu. %s/%d %s='%s'", i, st, it, n, s );
      else
        QT( "@%lu. %s/%d %s=NULL", i, st, it, n );
#endif
    }
  }
  return row;
}

void
duf_sccb_row_delete( duf_sccb_data_row_t row )
{
  for ( size_t i = 0; i < row.cnt; i++ )
  {
    mas_free( row.fields[i].svalue );
    mas_free( row.fields[i].name );
  }
  mas_free( row.fields );
}

duf_sccb_data_value_t *
duf_sccb_row_field_find( duf_sccb_data_row_t row, const char *name )
{
  for ( size_t i = 0; i < row.cnt; i++ )
  {
    if ( 0 == strcmp( row.fields[i].name, name ) )
      return &row.fields[i];
  }
  return NULL;
}

unsigned long long
duf_sccb_row_get_number( duf_sccb_data_row_t row, const char *name )
{
  duf_sccb_data_value_t *field;

  field = duf_sccb_row_field_find( row, name );
  assert( !field || field->typ == DUF_SQLTYPE_INTEGER );
  return field ? field->value.n : 0;
}

const char *
duf_sccb_row_get_string( duf_sccb_data_row_t row, const char *name )
{
  duf_sccb_data_value_t *field;

  field = duf_sccb_row_field_find( row, name );
  assert( !field || field->typ == DUF_SQLTYPE_TEXT || field->typ == DUF_SQLTYPE_NULL );
  return field ? field->svalue : NULL;
}

unsigned long long
duf_sccbh_row_get_number( duf_sccb_handle_t * sccbh, const char *name )
{
  return sccbh ? duf_sccb_row_get_number( sccbh->row, name ) : 0;
}

const char *
duf_sccbh_row_get_string( duf_sccb_handle_t * sccbh, const char *name )
{
  return sccbh ? duf_sccb_row_get_string( sccbh->row, name ) : NULL;
}

unsigned long long
duf_sccbh_prevrow_get_number( duf_sccb_handle_t * sccbh, const char *name )
{
  return sccbh ? duf_sccb_row_get_number( sccbh->previous_row, name ) : 0;
}

const char *
duf_sccbh_prevrow_get_string( duf_sccb_handle_t * sccbh, const char *name )
{
  return sccbh ? duf_sccb_row_get_string( sccbh->previous_row, name ) : NULL;
}
