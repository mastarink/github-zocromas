#ifndef MAS_DUF_SCCB_WAY_H
#  define MAS_DUF_SCCB_WAY_H

#  ifdef MAS_SCCBHANDLE
#    define PDI sccbh->pdi
#    define SCCB sccbh->sccb
#    define SCCBX sccbh
#    define DSCCBX duf_sccb_handle_t * sccbh
#    define TOTFILES sccbh->total_files
#  else
#    define PDI pdi
#    define SCCB sccb
#    define SCCBX pdi, sccb
#    define DSCCBX duf_depthinfo_t * pdi, const duf_scan_callbacks_t * sccb
#    define TOTFILES global_status.total_files
#  endif

#endif
