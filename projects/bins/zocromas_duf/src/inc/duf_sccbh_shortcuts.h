#ifndef MAS_DUF_SCCBH_SHORTCUTS_H
#  define MAS_DUF_SCCBH_SHORTCUTS_H

#  define PDI (sccbh->pdi)
#  define PDICLONED (sccbh->pdi_cloned)
#  define SCCB (sccbh->sccb)
#  define TOTITEMS (sccbh->total_items)
#if 0
#  define PU (sccbh->pu_x)
#else
#  define PU duf_pdi_pu(PDI)
#  define PY duf_pdi_py(PDI)
#endif
#  define HCHANGES (sccbh->changes)
#  define PARGC (sccbh->parg.argc)
#  define PARGV (sccbh->parg.argv)

#endif
