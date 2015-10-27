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
  case DUF_SCANSTAGE_NONE:
  case DUF_SCANSTAGE_MIN:
  case DUF_SCANSTAGE_MAX:
    break;
  }

  return scanner;
}

/* 20151013.113755 */
const char *
duf_scanstage_name( duf_scanstage_t scanstage )
{
  const char *rs = NULL;

  switch ( scanstage )
  {
  case DUF_SCANSTAGE_FS_ITEMS:
    rs = "fs_leaves"; /* F */
    break;
  case DUF_SCANSTAGE_DB_LEAVES_NOFD:
    rs = "db_leaves"; /* L */
    break;
  case DUF_SCANSTAGE_DB_LEAVES_FD:
    rs = "db_leaves_fd"; /* D */
    break;
  case DUF_SCANSTAGE_DB_SUBNODES:
    rs = "db_subnodes"; /* U */
    break;
  case DUF_SCANSTAGE_NODE_BEFORE:
    rs = "before"; /* B */
    break;
  case DUF_SCANSTAGE_NODE_MIDDLE:
    rs = "middle"; /* M */
    break;
  case DUF_SCANSTAGE_NODE_AFTER:
    rs = "after"; /* A */
    break;
  case DUF_SCANSTAGE_NONE:
    rs = "none"; /* N */
    break;
  case DUF_SCANSTAGE_MIN:
    rs = "min"; /* I */
    break;
  case DUF_SCANSTAGE_MAX:
    rs = "max"; /* X */
    break;
  }
  return rs;
}
const char *
duf_scanstage_shortname( duf_scanstage_t scanstage )
{
  const char *rs = NULL;

  switch ( scanstage )
  {
  case DUF_SCANSTAGE_FS_ITEMS:
    rs = "F"; /* F */
    break;
  case DUF_SCANSTAGE_DB_LEAVES_NOFD:
    rs = "L"; /* L */
    break;
  case DUF_SCANSTAGE_DB_LEAVES_FD:
    rs = "D"; /* D */
    break;
  case DUF_SCANSTAGE_DB_SUBNODES:
    rs = "U"; /* U */
    break;
  case DUF_SCANSTAGE_NODE_BEFORE:
    rs = "B"; /* B */
    break;
  case DUF_SCANSTAGE_NODE_MIDDLE:
    rs = "M"; /* M */
    break;
  case DUF_SCANSTAGE_NODE_AFTER:
    rs = "A"; /* A */
    break;
  case DUF_SCANSTAGE_NONE:
    rs = "N"; /* N */
    break;
  case DUF_SCANSTAGE_MIN:
    rs = "i"; /* I */
    break;
  case DUF_SCANSTAGE_MAX:
    rs = "X"; /* X */
    break;
  }
  return rs;
}
const char *
duf_nodetype_name( duf_node_type_t nt )
{
  return ( nt == DUF_NODE_LEAF ? "LEAF" : ( nt == DUF_NODE_LEAF ? "NODE" : "UNKNOWN" ) );
}
