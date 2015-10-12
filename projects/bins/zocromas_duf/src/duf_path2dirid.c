/* 20150901.122226 */
#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"


#include "duf_utils_path.h"

#include "duf_levinfo_ref.h"

#include "duf_pdi.h"
#include "duf_pdi_stmt.h"

#include "duf_sql_defs.h"
#include "duf_sql_stmt_defs.h"
#include "evsql_selector.h"

/* #include "duf_sql.h" */
#include "duf_sql_bind.h"
#include "duf_sql_prepared.h"
#include "duf_sql_field.h"

#include "sql_beginning_tables.h"

/* ###################################################################### */
#include "duf_path2dirid.h"
/* ###################################################################### */

/* 20150901.121302
 * use temporary inited pdi
 * to get dirid for path
 * */
unsigned long long
duf_path2dirid( const char *path, int *pr )
{
  int rpr = 0;
  char *real_path;
  unsigned long long dirid = 0;

  real_path = duf_realpath( path, &rpr );
  if ( DUF_NOERROR( rpr ) )
  {
    duf_depthinfo_t di = { 0 };

#if 0
    DOR( rpr, DUF_WRAPPED( duf_pdi_init ) ( &di, NULL /* pu */ , real_path, NULL /* sql_set */ , 0 /* caninsert */ , 1 /* recursive */ ,
                                            0 /* opendir */  ) );
#else
    DOR( rpr, duf_pdi_init_min( &di, real_path ) );
#endif



    if ( DUF_NOERROR( rpr ) )
      dirid = duf_levinfo_dirid( &di );
    /* xchanges = di.changes; --- needless!? */
    duf_pdi_shut( &di );
  }
  mas_free( real_path );
  if ( pr )
    *pr = rpr;
  return dirid;
}

static char *
_duf_dirid2name_existed( duf_depthinfo_t * pdi, const char *sqlv, unsigned long long dirid, unsigned long long *pparentid, int *pr )
{
  int rpr = 0;
  char *name = NULL;

  DEBUG_START(  );
  DUF_SQL_START_STMT( pdi, dirid2name_existed, sqlv, rpr, pstmt );
  {
    DUF_SQL_BIND_LL( dirID, dirid, rpr, pstmt );
    DUF_SQL_STEP( rpr, pstmt );
    if ( DUF_IS_ERROR_N( rpr, DUF_SQL_ROW ) )
    {
      /* rpr = 0; */
      DUF_TRACE( select, 0, "<selected> %s", sqlv );
      name = mas_strdup( DUF_GET_SFIELD2( name ) );
      if ( pparentid )
        *pparentid = DUF_GET_UFIELD2( parentid );
    }
    else
    {
      DUF_TRACE( select, 10, "<NOT selected> (%d)", rpr );
    }
  }
  DUF_SQL_END_STMT( pdi, dirid2name_existed, rpr, pstmt );
  if ( pr )
    *pr = rpr;
  DEBUG_ENDS( name );
}

static char *
duf_dirid2name_existed( duf_depthinfo_t * pdi, unsigned long long dirid, unsigned long long *pparentid, int *pr )
{
  char *name = NULL;

  DEBUG_START(  );

  char *sqlv = NULL;

  duf_sql_set_t def_node_set = {
    .fieldset = "pt.parentid AS parentid, pt." DUF_SQL_DIRNAMEFIELD " AS name " /* */
          ,
    .selector2 = " FROM " DUF_SQL_TABLES_PATHS_FULL " AS pt " /* */
          " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL " AS td ON (td.pathid=pt." DUF_SQL_IDFIELD ") " /*      */
          " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL " AS tf ON (tf.pathid=pt." DUF_SQL_IDFIELD ") " /*      */
    " WHERE pt." DUF_SQL_IDFIELD "=:dirID"
  };

  assert( pdi );

  {
    sqlv = duf_selector2sql( &def_node_set, pdi->pdi_name, pr );
    if ( sqlv )
    {
      name = _duf_dirid2name_existed( pdi, sqlv, dirid, pparentid, pr );

      mas_free( sqlv );
    }
  }
  DEBUG_ENDS( name );
}

char *
duf_dirid2path( unsigned long long dirid, int *pr )
{
  char *path = NULL;

  DEBUG_START(  );
  int r = 0;
  int done = 0;
  int depth = 0;

  duf_depthinfo_t DUF_UNUSED di = { 0 };
#if 0
  DOR( r, DUF_WRAPPED( duf_pdi_init ) ( &di, NULL /* pu */ , NULL /* real_path */ , NULL /* sql_set */ , 0 /* caninsert */ , 1 /* recursive */ ,
                                        0 /* opendir */  ) );
#else
  DOR( r, duf_pdi_init_min( &di, NULL /* real_path */  ) );
#endif
  do
  {
    char *name = NULL;

    name = duf_dirid2name_existed( &di, dirid, &dirid, &r );
    if ( name )
    {
      char *t;

      t = path;
      /* (name && !*name) means "/" */
      path = mas_strdup( name );
#if 0
      path = mas_strcat_x( path, "/" );
#else
      path = duf_normalize_path( path );
#endif
      path = mas_strcat_x( path, t );
      mas_free( t );
      depth++;
    }
    else
      done = 1;
    mas_free( name );
  }
  while ( !done );
  duf_pdi_shut( &di );

  if ( pr )
    *pr = r;
  DEBUG_ENDS( path );
}
