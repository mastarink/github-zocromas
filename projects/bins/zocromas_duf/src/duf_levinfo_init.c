#include <string.h>

#include "duf_maintenance.h"

#include "duf_context.h"

#include "duf_sql_defs.h"
#include "duf_sql_field.h"

#include "duf_li.h"
#include "duf_levinfo_ref.h"
#include "duf_levinfo_ref_def.h"
#include "duf_levinfo_context.h"

#include "duf_pstmt_levinfo.h"

/* ###################################################################### */
#include "duf_levinfo.h"
#include "duf_levinfo_init.h"
/* ###################################################################### */

/* 20150831.000000 */
void
duf_levinfo_init_level_d( duf_depthinfo_t * pdi, const char *itemname, unsigned long long dirid, duf_node_type_t node_type, int d )
{
  assert( pdi );
  assert( d >= 0 );
  assert( pdi->pathinfo.levinfo );

  duf_levinfo_clear_level_d( pdi, d );
  assert( !pdi->pathinfo.levinfo[d].itemname );

  {
    duf_levinfo_t *pli;

    pli = &pdi->pathinfo.levinfo[d];
    pli->node_type = node_type;
    pli->db.dirid = dirid;

    if ( itemname )
    {
      assert( !pdi->pathinfo.levinfo[d].itemname );
      pdi->pathinfo.levinfo[d].itemname = mas_strdup( itemname );
    }

#ifndef DUF_NO_NUMS
    /* pdi->pathinfo.levinfo[d].numdir = ndirs;   */
    /* pdi->pathinfo.levinfo[d].numfile = nfiles; */
    /* duf_li_set_nums( pli, ndirs, nfiles ); *//* really never nz here */
#else
    /* if ( duf_levinfo_node_type_d( pdi, d ) == DUF_NODE_NODE ) */
    /*   duf_levinfo_make_childs_d( pdi, d );                    */
#endif
  }
}

/* 20150901.173353 */
/* resets levinfo  (currenl level) */
void
duf_levinfo_init_level( duf_depthinfo_t * pdi, const char *itemname, unsigned long long dirid, duf_node_type_t node_type )
{
  duf_levinfo_init_level_d( pdi, itemname, dirid, node_type, pdi->pathinfo.depth );

  assert( pdi->pathinfo.depth + 1 == duf_levinfo_count( pdi ) );
}
