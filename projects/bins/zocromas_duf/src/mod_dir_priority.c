#include <string.h>
#include <unistd.h>






#include "duf_maintenance.h"

#include "duf_config_ref.h"




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

#include "duf_tags.h"

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

/* ########################################################################################## */

static int
set_dir_priority_node_before2( duf_stmnt_t * pstmt DUF_UNUSED, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  DUF_TRACE( mod, 1, "%d %llu %s : %s: %s", DUF_CONFIGG( dir_priority ), duf_levinfo_dirid( pdi ), duf_levinfo_relpath( pdi ), duf_levinfo_itemshowname( pdi ),
             pstmt ? DUF_GET_SFIELD2( dname ) : "-" );
  DEBUG_ENDR( r );
}

static int
set_dir_priority_node_middle2( duf_stmnt_t * pstmt DUF_UNUSED, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 1, "%d %llu %s : %s: %s", DUF_CONFIGG( dir_priority ), duf_levinfo_dirid( pdi ), duf_levinfo_relpath( pdi ), duf_levinfo_itemshowname( pdi ),
             pstmt ? DUF_GET_SFIELD2( dname ) : "-" );
  DEBUG_ENDR( r );
}


static int
set_dir_priority_node_after2( duf_stmnt_t * pstmt DUF_UNUSED, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( mod, 1, "%d %llu %s : %s: %s", DUF_CONFIGG( dir_priority ), duf_levinfo_dirid( pdi ), duf_levinfo_relpath( pdi ), duf_levinfo_itemshowname( pdi ),
             pstmt ? DUF_GET_SFIELD2( dname ) : "-" );
  DEBUG_ENDR( r );
}
