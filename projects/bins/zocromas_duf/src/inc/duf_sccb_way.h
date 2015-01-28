#ifndef MAS_DUF_SCCB_WAY_H
#  define MAS_DUF_SCCB_WAY_H

#  define PDI sccbh->pdi
#  define SCCB sccbh->sccb
#  define SCCBX sccbh
#  define DSCCBX struct duf_sccb_handle_s * sccbh
#  define TOTFILES sccbh->total_files
#  define PU sccbh->pu

#endif
