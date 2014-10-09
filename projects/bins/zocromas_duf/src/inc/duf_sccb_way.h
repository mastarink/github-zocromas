#ifndef MAS_DUF_SCCB_WAY_H
#  define MAS_DUF_SCCB_WAY_H

#  ifdef MAS_SCCBHANDLE
#    define PDI sccbh->pdi
#    define SCCB sccbh->sccb
#    define SCCBX sccbh
#    define DSCCBX struct duf_sccb_handle_s * sccbh
#    define TOTFILES sccbh->total_files
#    define PU sccbh->pu
#  else
#    define PDI pdi
#    define SCCB sccb
#    define SCCBX pdi, sccb
#    define DSCCBX duf_depthinfo_t * pdi, const struct duf_scan_callbacks_s * sccb
#    define TOTFILES global_status.total_files
#    define PU duf_config->pu
#  endif

#endif
