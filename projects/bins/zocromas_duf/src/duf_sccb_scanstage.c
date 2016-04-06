/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
/* #include "duf_errorn_defs_preset.h" */

#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ▤ */
#include <mastar/trace/mas_trace.h>
/* #include <mastar/error/mas_error_defs_ctrl.h> */
/* #include <mastar/error/mas_error_defs_make.h> */
/* #include <mastar/error/mas_error_defs.h>      */

#include "duf_sccb.h"
#include "duf_sccb_structs.h"

/* ###################################################################### */
#include "duf_sccb_scanstage.h"                                      /* duf_scanstage_name; duf_scanstage_scanner; ✗ */
/* ###################################################################### */

/* 20151013.113751 */
duf_scanner_fun_t
duf_sccb_scanstage_scanner( const duf_scan_callbacks_t * sccb, duf_scanstage_t scanstage, int deleted, duf_node_type_t nt )
{
  duf_scanner_fun_t scanner = NULL;

  assert( deleted == 1 || deleted == 0 );
  switch ( scanstage )
  {
  case DUF_SCANSTAGE_FS_ITEMS:
    if ( !deleted )
    {
      assert( nt == DUF_NODE_LEAF || nt == DUF_NODE_NODE );
      if ( nt == DUF_NODE_NODE )
      {
        scanner = sccb->dirent_dir_scan_before2;
      /* T( "scanner: %d %s (%s)", scanner ? 1 : 0, duf_uni_scan_action_title( sccb ), duf_scanstage_name( scanstage ) ); */
      }
      else if ( nt == DUF_NODE_LEAF )
      {
        scanner = sccb->dirent_file_scan_before2;
      /* T( "scanner: %d %s (%s)", scanner ? 1 : 0, duf_uni_scan_action_title( sccb ), duf_scanstage_name( scanstage ) ); */
      }
    }
    break;
  case DUF_SCANSTAGE_DB_LEAVES:
    break;
  case DUF_SCANSTAGE_DB_LEAVES_NOFD:
    assert( nt == DUF_NODE_LEAF );
    if ( nt == DUF_NODE_LEAF )
      scanner = deleted ? sccb->leaf_scan2_deleted : sccb->leaf_scan2;
  /* T( "scanner: %d %s (%s)", scanner ? 1 : 0, duf_uni_scan_action_title( sccb ), duf_scanstage_name( scanstage ) ); */
    break;
  case DUF_SCANSTAGE_DB_LEAVES_FD:
    assert( nt == DUF_NODE_LEAF );
    if ( nt == DUF_NODE_LEAF )
      scanner = deleted ? sccb->leaf_scan_fd2_deleted : sccb->leaf_scan_fd2;
  /* T( "scanner: %d %s (%s)", scanner ? 1 : 0, duf_uni_scan_action_title( sccb ), duf_scanstage_name( scanstage ) ); */
    break;
  case DUF_SCANSTAGE_DB_SUBNODES:
  /* T( "scanner: %d %s (%s)", scanner ? 1 : 0, duf_uni_scan_action_title( sccb ), duf_scanstage_name( scanstage ) ); */
    break;
  case DUF_SCANSTAGE_NODE_BEFORE:
    if ( nt == DUF_NODE_NODE )
      scanner = deleted ? sccb->node_scan_before2_deleted : sccb->node_scan_before2;
  /* T( "scanner: %d %s (%s)", scanner ? 1 : 0, duf_uni_scan_action_title( sccb ), duf_scanstage_name( scanstage ) ); */
    break;
  case DUF_SCANSTAGE_NODE_MIDDLE:
    if ( nt == DUF_NODE_NODE )
      scanner = deleted ? sccb->node_scan_middle2_deleted : sccb->node_scan_middle2;
  /* T( "scanner: %d %s (%s)", scanner ? 1 : 0, duf_uni_scan_action_title( sccb ), duf_scanstage_name( scanstage ) ); */
    break;
  case DUF_SCANSTAGE_NODE_AFTER:
    if ( nt == DUF_NODE_NODE )
      scanner = deleted ? sccb->node_scan_after2_deleted : sccb->node_scan_after2;
  /* T( "scanner: %d %s (%s)", scanner ? 1 : 0, duf_uni_scan_action_title( sccb ), duf_scanstage_name( scanstage ) ); */
    break;
  case DUF_SCANSTAGE_NONE:
  /* case DUF_SCANSTAGE_MIN: */
  /* case DUF_SCANSTAGE_MAX: */
  /* T( "scanner: %d %s (%s)", scanner ? 1 : 0, duf_uni_scan_action_title( sccb ), duf_scanstage_name( scanstage ) ); */
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
    rs = "fs_items";                                                 /* F */
    break;
  case DUF_SCANSTAGE_DB_LEAVES:
    rs = "db_leaves_any";                                            /* M */
    break;
  case DUF_SCANSTAGE_DB_LEAVES_NOFD:
    rs = "db_leaves_nofd";                                           /* L */
    break;
  case DUF_SCANSTAGE_DB_LEAVES_FD:
    rs = "db_leaves_fd";                                             /* D */
    break;
  case DUF_SCANSTAGE_DB_SUBNODES:
    rs = "db_subnodes";                                              /* U */
    break;
  case DUF_SCANSTAGE_NODE_BEFORE:
    rs = "before";                                                   /* B */
    break;
  case DUF_SCANSTAGE_NODE_MIDDLE:
    rs = "middle";                                                   /* M */
    break;
  case DUF_SCANSTAGE_NODE_AFTER:
    rs = "after";                                                    /* A */
    break;
  case DUF_SCANSTAGE_NONE:
    rs = "none";                                                     /* N */
    break;
  /* case DUF_SCANSTAGE_MIN:               */
  /*   rs = "min";                 (* I *) */
  /*   break;                              */
  /* case DUF_SCANSTAGE_MAX:               */
  /*   rs = "max";                 (* X *) */
  /*   break;                              */
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
    rs = "F";                                                        /* F */
    break;
  case DUF_SCANSTAGE_DB_LEAVES:
    rs = "M";                                                        /* M */
    break;
  case DUF_SCANSTAGE_DB_LEAVES_NOFD:
    rs = "L";                                                        /* L */
    break;
  case DUF_SCANSTAGE_DB_LEAVES_FD:
    rs = "D";                                                        /* D */
    break;
  case DUF_SCANSTAGE_DB_SUBNODES:
    rs = "U";                                                        /* U */
    break;
  case DUF_SCANSTAGE_NODE_BEFORE:
    rs = "B";                                                        /* B */
    break;
  case DUF_SCANSTAGE_NODE_MIDDLE:
    rs = "M";                                                        /* M */
    break;
  case DUF_SCANSTAGE_NODE_AFTER:
    rs = "A";                                                        /* A */
    break;
  case DUF_SCANSTAGE_NONE:
    rs = "N";                                                        /* N */
    break;
  /* case DUF_SCANSTAGE_MIN:               */
  /*   rs = "i";                   (* I *) */
  /*   break;                              */
  /* case DUF_SCANSTAGE_MAX:               */
  /*   rs = "X";                   (* X *) */
  /*   break;                              */
  }
  return rs;
}
