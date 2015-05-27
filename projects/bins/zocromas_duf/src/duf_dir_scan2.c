#include <unistd.h>
#include <assert.h>
#include <libgen.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"
#include "duf_status_ref.h"

#include "duf_pdi.h"
#include "duf_levinfo_ref.h"

#include "duf_item_scan2.h"

#include "duf_path2db.h"


#include "duf_sql.h"
#include "duf_sql2.h"

#include "duf_dir_scan2_stages.h"

#include "duf_ufilter.h"

#include "duf_sccb.h"
#include "duf_sccb_def.h"
#include "duf_sccb_begfin.h"

/* #include "duf_item_match2.h" */

/* #include "duf_uni_scan2.h" */
#include "duf_sccbh_shortcuts.h"
/* ###################################################################### */
#include "duf_dir_scan2.h"
/* ###################################################################### */


/* duf_scan_dirs_by_pdi:
 *  -= Walk (scan) dirs from DB =-
 *
 * consecutively call various scanner stages with the sccb
 * on current dir from pdi
 * 
 * i.e. [ evaluate sccb for current dir ]
 *
 * args:
 *   pstmt: sql(ite) prepared statement
 *   pdi:  depthinfo structure ptr
 *   sccb: module callbacs structure
 *
 * */
static int
duf_scan_dirs_by_pdi( duf_sqlite_stmt_t * pstmt_selector, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );


  assert( SCCB );
  DUF_TRACE( scan, 4, "[%llu]", duf_levinfo_dirid( PDI ) );
  /* duf_scan_qbeginning_sql( SCCB ); */
/*
 * call corresponding callback (by dir/regular)
 *   for each direntry from filesystem
 *                                     -- see duf_dir_scan2_stages.c
 * */
#if 1
  duf_str_cb2_t stages[] = {
    duf_qscan_dirents2,
    duf_qscan_node_scan_before2,
    duf_qscan_files_by_dirid2,
    duf_qscan_node_scan_middle2,
    duf_qscan_dirs_by_dirid2,
    duf_qscan_node_scan_after2,
    NULL
  };
  for ( duf_str_cb2_t * pstage = stages; *pstage; pstage++ )
  {
    DOR( r, ( *pstage ) ( pstmt_selector, sccbh ) );
    /*                                                     */ DUF_TRACE( scan, 4, "[%llu]", duf_levinfo_dirid( PDI ) );
  }
#else
  /* 1. */ DOR( r, duf_qscan_dirents2 /*          */ ( pstmt_selector, sccbh ) ); /* walk FS entries (dirents) for dir at curr. level at pdi */
  /*                                                                              */ DUF_TRACE( scan, 4, "[%llu]", duf_levinfo_dirid( PDI ) );
  /* 2. */ DOR( r, duf_qscan_node_scan_before2 /* */ ( pstmt_selector, sccbh ) );
  /*                                                                              */ DUF_TRACE( scan, 4, "[%llu]", duf_levinfo_dirid( PDI ) );
  /* 3. */ DOR( r, duf_qscan_files_by_dirid2 /*   */ ( pstmt_selector, sccbh ) );
  /*                                                                              */ DUF_TRACE( scan, 4, "[%llu]", duf_levinfo_dirid( PDI ) );
  /* 4. */ DOR( r, duf_qscan_node_scan_middle2 /* */ ( pstmt_selector, sccbh ) );
  /*                                                                              */ DUF_TRACE( scan, 4, "[%llu]", duf_levinfo_dirid( PDI ) );
  /* 5. */ DOR( r, duf_qscan_dirs_by_dirid2 /*    */ ( pstmt_selector, sccbh ) );
  /*                                                                              */ DUF_TRACE( scan, 4, "[%llu]", duf_levinfo_dirid( PDI ) );
  /* 6. */ DOR( r, duf_qscan_node_scan_after2 /*  */ ( pstmt_selector, sccbh ) );
  /*                                                                              */ DUF_TRACE( scan, 4, "[%llu]", duf_levinfo_dirid( PDI ) );
#endif
  DEBUG_ENDR( r );
}

/* duf_scan_dirs_by_pdi_wrap          ( duf_scan_dirs_by_parentid )
 *  -= Walk (scan) dirs from DB =-
 *
 * see duf_scan_dirs_by_pdi
 *
 * str_cb2 (sub-item scanner) one of:
 *       duf_scan_dirs_by_pdi_maxdepth
 *     ( duf_str_cb2_leaf_scan    )
 *     ( duf_str_cb2_scan_file_fd )
 * */

int
duf_scan_dirs_by_pdi_wrap( duf_sqlite_stmt_t * pstmt_selector, duf_sccb_handle_t * sccbh )
{
  DEBUG_STARTR( r );
  unsigned long long diridpid;

  diridpid = duf_levinfo_dirid( PDI );

  /* assert( duf_scan_dirs_by_pdi_maxdepth == str_cb2_unused ); */

  DUF_TRACE( scan, 3, "[%llu]", diridpid );
  DUF_SCCB_PDI( DUF_TRACE, scan, 10 + duf_pdi_reldepth( PDI ), PDI, "** depth:%d/%d; diridpid:%llu", duf_pdi_depth( PDI ),
                duf_pdi_reldepth( PDI ), diridpid );

  DUF_TRACE( scan, 3, "[%llu]  : scan start      +" DUF_DEPTH_PFMT "", diridpid, duf_pdi_depth( PDI ) );

  DUF_SCCB_PDI( DUF_TRACE, scan, 10 + duf_pdi_reldepth( PDI ), PDI, " >>> 1." );

  {                             /* XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX XXX */
    DOR( r, duf_scan_dirs_by_pdi( pstmt_selector, /* str_cb2_unused, */ sccbh ) );
  }

  DUF_TRACE( scan, 3, "[%llu]  : scan end      +" DUF_DEPTH_PFMT "", diridpid, duf_pdi_depth( PDI ) );
  DEBUG_ENDR( r );
}
