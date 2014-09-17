#define DUF_SQL_PDI_STMT

#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include <mastar/tools/mas_arg_tools.h>


#include "duf_maintenance.h"


#include "duf_config_ref.h"

/* #include "duf_pdi.h" */
/* #include "duf_levinfo.h" */

#include "duf_sql_defs.h"
#include "duf_sql_field.h"

#include "duf_sql.h"
#include "duf_sql2.h"

#include "duf_path.h"

/* ###################################################################### */
#include "duf_db2path.h"
/* ###################################################################### */

#if 1
/* last function revision 20140901.204850 */
static char *
duf_pathid_to_path2_in( duf_sqlite_stmt_t * pstmt, unsigned long long dirid, const duf_depthinfo_t * pdi, int *pr )
{
  char *path = NULL;
  int r = 0;

  DEBUG_START(  );
  if ( dirid > 0 && pstmt )
  {
/* get parentid for dirid */
    duf_sql_reset( pstmt );

    DUF_SQL_BIND_LL( dirID, dirid, r, pstmt );
    DUF_SQL_STEP( r, pstmt );

    if ( r == DUF_SQL_ROW )
    {
      DUF_UFIELD2( parentid );
      DUF_SFIELD2( dirname );
      if ( parentid >= 0 )
      {
        char *parent = NULL;

        parent = duf_pathid_to_path2_in( pstmt, parentid, pdi, &r ); /* open!! */
        DUF_TEST_R( r );
        path = duf_join_path( parent, dirname );
        /* DUF_SHOW_ERROR( "%llu (%s): %s @ %llu => %s", dirid, parent, tail, parentid, path ); */
        mas_free( parent );
      }
    }
    DUF_TEST_R( r );
  }
  if ( pr )
    *pr = r;
  DEBUG_ENDS( path );
  return path;
}

char *
duf_pathid_to_path2( unsigned long long dirid, const duf_depthinfo_t * pdi, int *pr )
{
  int r = 0;
  char *path = NULL;


  const char *sql = "SELECT parentid, dirname FROM " DUF_DBPREF "paths WHERE " DUF_SQL_IDNAME "=:dirID";

  DUF_SQL_START_STMT_NOPDI( sql, r, pstmt );

  path = duf_pathid_to_path2_in( pstmt, dirid, pdi, &r );
  path = mas_strcat_x( path, "/" );

  DUF_SQL_END_STMT_NOPDI( r, pstmt );
  if ( pr )
    *pr = r;
  DUF_PRINTF( 0, "################ %s #######################", path );
  return path;
}

const char *
duf_pathid_to_path2_tmp( int index, unsigned long long dirid, const duf_depthinfo_t * pdi )
{
  const char *p = NULL;

  if ( index < DUF_TMP_PATH_MAX )
  {
    mas_free( duf_config->tmp->path[index] );
    duf_config->tmp->path[index] = duf_pathid_to_path2( dirid, pdi, NULL );
    p = duf_config->tmp->path[index];
  }
  return p;
}
#endif
