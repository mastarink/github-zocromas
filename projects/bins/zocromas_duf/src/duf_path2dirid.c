/* 20150901.122226 */
#define DUF_SQL_PDI_STMT

#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"


#include "duf_utils_path.h"

#include "duf_levinfo_ref.h"

#include "duf_pdi.h"
#include "duf_pdi_stmt.h"

#include "duf_sql_defs.h"
#include "duf_selector.h"

#include "duf_sql.h"
#include "duf_sql2.h"
#include "duf_sql_field.h"

#include "sql_beginning_tables.h"

/* ###################################################################### */
#include "duf_path2dirid.h"
/* ###################################################################### */

/* 20150901.121302 */
unsigned long long
duf_path2dirid( const char *path, const duf_sql_set_t * sql_set, int *pr )
{
  int rpr = 0;
  char *real_path;
  unsigned long long dirid = 0;

  real_path = duf_realpath( path, &rpr );
  if ( DUF_NOERROR( rpr ) )
  {
    duf_depthinfo_t di = {.depth = -1,
      .seq = 0,
      .levinfo = NULL,
      .pu = NULL,
      /* .opendir = sccb ? sccb->opendir : 0, */
      .opendir = 1,
      /* .name = real_path, */
    };
    DOR( rpr,
         DUF_WRAPPED( duf_pdi_init ) ( &di, real_path, 0 /* caninsert */ , sql_set /* node_selector2 */ , 1 /* recursive */ , 0 /* opendir */  ) );
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
  DUF_SQL_START_STMT( pdi, select_path, sqlv, rpr, pstmt );
  {
    DUF_SQL_BIND_LL( dirID, dirid, rpr, pstmt );
    DUF_SQL_STEP( rpr, pstmt );
    if ( DUF_IS_ERROR( rpr, DUF_SQL_ROW ) )
    {
      rpr = 0;
      DUF_TRACE( select, 0, "<selected> %s", sqlv );
      name = mas_strdup( DUF_GET_SFIELD2( name ) );
      if ( pparentid )
        *pparentid = DUF_GET_UFIELD2( parentid );
      DUF_TRACE( temp, 0, "%s", DUF_GET_SFIELD2( name ) );
    }
    else
    {
      DUF_TRACE( select, 10, "<NOT selected> (%d)", rpr );
    }
  }
  DUF_SQL_END_STMT( select_path, rpr, pstmt );
  if ( pr )
    *pr = rpr;
  DEBUG_ENDS( name );
}

static char *
duf_dirid2name_existed( duf_depthinfo_t * pdi, unsigned long long dirid, unsigned long long *pparentid, int *pr )
{
  char *name = NULL;

  DEBUG_START(  );

  int rpr = 0;
  char *sqlv = NULL;

  duf_sql_set_t def_node_set = {
    .fieldset = "pt.parentid AS parentid, pt.dirname as name " /* */
          ,
    .selector2 = " FROM " DUF_DBPREF "paths AS pt " /* */
          " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL " AS td ON (td.pathid=pt." DUF_SQL_IDNAME ") " /*      */
          " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL " AS tf ON (tf.pathid=pt." DUF_SQL_IDNAME ") " /*      */
    " WHERE pt." DUF_SQL_IDNAME "=:dirID"
  };

  assert( pdi );

  {
    sqlv = duf_selector2sql( &def_node_set );

    name = _duf_dirid2name_existed( pdi, sqlv, dirid, pparentid, &rpr );

    mas_free( sqlv );
  }
  if ( pr )
    *pr = rpr;
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

  duf_depthinfo_t DUF_UNUSED di = {.depth = -1,
    .seq = 0,
    .levinfo = NULL,
    .pu = NULL,
    .opendir = 1,
  };
  DOR( r, DUF_WRAPPED( duf_pdi_init ) ( &di, NULL, 0 /* caninsert */ , NULL /* node_selector2 */ , 1 /* recursive */ , 0 /* opendir */  ) );

  do
  {
    char *name = NULL;

    name = duf_dirid2name_existed( &di, dirid, &dirid, &r );
    DUF_TRACE( temp, 0, "@@%u: #%llu %s", depth, dirid, name );
    if ( name )
    {
      char *t;

      t = path;
      /* (name && !*name) means "/" */
      path = mas_strdup( name );

      path = mas_strcat_x( path, "/" );
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