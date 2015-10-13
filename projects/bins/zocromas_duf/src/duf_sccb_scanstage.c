#include <string.h>

#include <mastar/tools/mas_arg_tools.h>

#include "duf_maintenance.h"

/* ###################################################################### */
#include "duf_sccb_scanstage.h"
/* ###################################################################### */

/* 20151013.113751 */
duf_scanner_t
duf_scanstage_scanner( const duf_scan_callbacks_t * sccb, duf_scanstage_t scanstage, int deleted, duf_node_type_t nt )
{
  duf_scanner_t scanner = NULL;

  assert( deleted == 1 || deleted == 0 );
  switch ( scanstage )
  {
  case DUF_SCANSTAGE_FS_ITEMS:
    if ( !deleted )
    {
      assert( nt == DUF_NODE_LEAF || nt == DUF_NODE_NODE );
      if ( nt == DUF_NODE_NODE )
        scanner = sccb->dirent_dir_scan_before2;
      else if ( nt == DUF_NODE_LEAF )
        scanner = sccb->dirent_file_scan_before2;
    }
    break;
  case DUF_SCANSTAGE_DB_LEAVES_NOFD:
    assert( nt == DUF_NODE_LEAF );
    if ( nt == DUF_NODE_LEAF )
      scanner = deleted ? sccb->leaf_scan2_deleted : sccb->leaf_scan2;
    break;
  case DUF_SCANSTAGE_DB_LEAVES_FD:
    assert( nt == DUF_NODE_LEAF );
    if ( nt == DUF_NODE_LEAF )
      scanner = deleted ? sccb->leaf_scan_fd2_deleted : sccb->leaf_scan_fd2;
    break;
  case DUF_SCANSTAGE_DB_SUBNODES:
    break;
  case DUF_SCANSTAGE_NODE_BEFORE:
    if ( nt == DUF_NODE_NODE )
      scanner = deleted ? sccb->node_scan_before2_deleted : sccb->node_scan_before2;
    break;
  case DUF_SCANSTAGE_NODE_MIDDLE:
    if ( nt == DUF_NODE_NODE )
      scanner = deleted ? sccb->node_scan_middle2_deleted : sccb->node_scan_middle2;
    break;
  case DUF_SCANSTAGE_NODE_AFTER:
    if ( nt == DUF_NODE_NODE )
      scanner = deleted ? sccb->node_scan_after2_deleted : sccb->node_scan_after2;
    break;
  case DUF_SCANSTAGE_MIN:
  case DUF_SCANSTAGE_MAX:
    break;
  }

  return scanner;
}

/* 20151013.113755 */
const char *
duf_scanstage_name( duf_scanstage_t scanstage, duf_node_type_t nt DUF_UNUSED )
{
  const char *rs = NULL;

  switch ( scanstage )
  {
  case DUF_SCANSTAGE_FS_ITEMS:
    rs = "fs_leaves";
    break;
  case DUF_SCANSTAGE_DB_LEAVES_NOFD:
    rs = "db_leaves";
    break;
  case DUF_SCANSTAGE_DB_LEAVES_FD:
    rs = "db_leaves_fd";
    break;
  case DUF_SCANSTAGE_DB_SUBNODES:
    rs = "db_subnodes";
    break;
  case DUF_SCANSTAGE_NODE_BEFORE:
    rs = "before";
    break;
  case DUF_SCANSTAGE_NODE_MIDDLE:
    rs = "middle";
    break;
  case DUF_SCANSTAGE_NODE_AFTER:
    rs = "after";
    break;
  case DUF_SCANSTAGE_MIN:
    rs = "min";
    break;
  case DUF_SCANSTAGE_MAX:
    rs = "max";
    break;
  }
  return rs;
}

const char *
duf_nodetype_name( duf_node_type_t nt )
{
  return ( nt == DUF_NODE_LEAF ? "LEAF" : ( nt == DUF_NODE_LEAF ? "NODE" : "UNKNOWN" ) );
}
