#include <string.h>
#include <unistd.h>






#include "duf_maintenance.h"

#include "duf_config.h"
#include "duf_config_trace.h"
#include "duf_config_util.h"
#include "duf_config_ref.h"
#include "duf_config_defs.h"




#include "duf_levinfo_openclose.h"
#include "duf_levinfo_stat.h"
#include "duf_levinfo_ref.h"

#include "duf_sql_defs.h"
#include "duf_sql_field.h"

#include "duf_option_defs.h"

/* #include "duf_dbg.h" */
#include "duf_mod_defs.h"

#include "sql_beginning_selected.h"
#include "sql_beginning_tables.h"

#include "duf_pdi_ref.h"
#include "duf_pdi_stmt.h"

#include "duf_sql_stmt_defs.h"

#include "duf_sql_defs.h"
#include "duf_sql_field.h"

#include "duf_sql_bind.h"
#include "duf_sql_prepared.h"

/* #include "duf_tags.h" */

/* ########################################################################################## */
/* DUF_MOD_DECLARE_ALL_FUNCS( set_dir_priority ) */
static int set_dir_priority_node_before2( duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_depthinfo_t * pdi );
static int set_dir_priority_node_middle2( duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_depthinfo_t * pdi );
static int set_dir_priority_node_after2( duf_stmnt_t * pstmt_unused DUF_UNUSED, duf_depthinfo_t * pdi );


/* ########################################################################################## */

duf_scan_callbacks_t duf_set_dir_priority_callbacks = {
  .title = "set dir priority",
  .name = "set_dir_priority",
  .def_opendir = 0,             /* ?? */

  .node_scan_before2 = set_dir_priority_node_before2,
  .node_scan_after2 = set_dir_priority_node_after2,
  .node_scan_middle2 = set_dir_priority_node_middle2,

  .use_std_leaf = 2,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node = 2,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
};

/* extern duf_scan_callbacks_t set_dir_priority_cbs __attribute__ ( ( alias( "duf_set_dir_priority_callbacks" ) ) ); */

/* ########################################################################################## */

static int
set_dir_priority_node_before2( duf_stmnt_t * pstmt DUF_UNUSED, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  DUF_TRACE( mod, 1, "before %d dirid:%llu %s : %s: %s", DUF_CONFIGG( dir_priority ), duf_levinfo_dirid( pdi ), duf_levinfo_relpath( pdi ),
             duf_levinfo_itemshowname( pdi ), pstmt ? DUF_GET_SFIELD2( dname ) : "-" );

  {
    int changes = 0;
    const char *sql = "UPDATE " DUF_SQL_TABLES_PATHS_FULL " SET priority=:priority WHERE " DUF_SQL_IDFIELD " =:dirId ";

    DUF_SQL_START_STMT( pdi, update_priority, sql, r, pstmt );
    DUF_TRACE( mod, 3, "S:%s", sql );
    DUF_SQL_BIND_LL( dirId, duf_levinfo_dirid( pdi ), r, pstmt );
    DUF_SQL_BIND_LL( priority, DUF_CONFIGG( dir_priority ), r, pstmt );
    DUF_SQL_STEPC( r, pstmt );
    DUF_SQL_CHANGES( changes, r, pstmt );
    DUF_SQL_END_STMT( pdi, update_priority, r, pstmt );
    duf_pdi_reg_changes( pdi, changes );
    DUF_TEST_R( r );
  }
  DEBUG_ENDR( r );
}

static int
set_dir_priority_node_middle2( duf_stmnt_t * pstmt DUF_UNUSED, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 1, "middle %d dirid:%llu %s : %s: %s", DUF_CONFIGG( dir_priority ), duf_levinfo_dirid( pdi ), duf_levinfo_relpath( pdi ),
             duf_levinfo_itemshowname( pdi ), pstmt ? DUF_GET_SFIELD2( dname ) : "-" );
  DEBUG_ENDR( r );
}


static int
set_dir_priority_node_after2( duf_stmnt_t * pstmt DUF_UNUSED, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 1, "after  %d dirid:%llu %s : %s: %s", DUF_CONFIGG( dir_priority ), duf_levinfo_dirid( pdi ), duf_levinfo_relpath( pdi ),
             duf_levinfo_itemshowname( pdi ), pstmt ? DUF_GET_SFIELD2( dname ) : "-" );
  DEBUG_ENDR( r );
}
