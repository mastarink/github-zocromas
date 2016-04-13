#ifndef MAS_DUF_SCCBH_SHORTCUTS_H
# define MAS_DUF_SCCBH_SHORTCUTS_H

/* #  define H_PDI (sccbh->pdi) */
# define H_PDI CRX( sccbh_pdi, sccbh )
# define H_PDI_SET(_pdi) CRX( sccbh_set_pdi, sccbh, _pdi )
# define H_PDICLONED (sccbh->pdi_cloned)

/* # define H_SCCBA (sccbh->sccb_array) */
# define H_SCCBA duf_sccbh_sccbarray(sccbh)
/* # define H_SCCBI (sccbh->sccb_index) */
# define H_SCCBI duf_sccbh_sccbindex(sccbh)
# define H_SCCBIv (*duf_sccbh_psccbindex(sccbh))

# if 0
#  define H_SCCB (sccbh->sccb)
# else
#  define H_SCCB (H_SCCBA[H_SCCBI])
# endif
/* # define H_TOTITEMS (sccbh->total_items) */
# define H_TOTITEMS duf_sccbh_total_items(sccbh)
# define H_TOTITEMSv (*duf_sccbh_ptotal_items(sccbh))
/* # define H_TOTCOUNTED (sccbh->total_counted) */
# define H_TOTCOUNTED duf_sccbh_total_counted(sccbh)
# define H_TOTCOUNTED_SET duf_sccbh_set_total_counted(sccbh, 1)
# if 0
#  define H_PU (sccbh->pu_x)
# else
#  define H_PU CRX( pdi_pu, H_PDI )
#  define H_PY CRX( pdi_py, H_PDI )
# endif
/* # define H_HCHANGES (sccbh->changes) */
# define H_HCHANGES duf_sccbh_changes(sccbh)
# define H_HCHANGES_SET(_changes) duf_sccbh_set_changes(sccbh, _changes)
# define H_HCHANGES_CLEAR H_HCHANGES_SET(0)
# define H_PARGC (sccbh->parg.argc)
# define H_PARGV (sccbh->parg.argv)

#endif
