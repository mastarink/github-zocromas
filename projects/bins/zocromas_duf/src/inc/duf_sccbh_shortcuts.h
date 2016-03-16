#ifndef MAS_DUF_SCCBH_SHORTCUTS_H
#  define MAS_DUF_SCCBH_SHORTCUTS_H

#  define H_PDI (sccbh->pdi)
#  define H_PDICLONED (sccbh->pdi_cloned)
#  define H_SCCBA (sccbh->sccb_array)
#  define H_SCCBI (sccbh->sccb_index)
#if 0
#  define H_SCCB (sccbh->sccb)
#else
#  define H_SCCB (H_SCCBA[H_SCCBI])
#endif
#  define H_TOTITEMS (sccbh->total_items)
#  define H_TOTCOUNTED (sccbh->total_counted)
#if 0
#  define H_PU (sccbh->pu_x)
#else
#  define H_PU duf_pdi_pu(H_PDI)
#  define H_PY duf_pdi_py(H_PDI)
#endif
#  define H_HCHANGES (sccbh->changes)
#  define H_PARGC (sccbh->parg.argc)
#  define H_PARGV (sccbh->parg.argv)

#endif
