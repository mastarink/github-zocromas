#ifndef MAS_DUF_LI_CREDEL_H
# define MAS_DUF_LI_CREDEL_H

# include "duf_levinfo_types.h"                                      /* duf_levinfo_t ✗ */
# include "duf_scan_types.h"                                         /* duf_node_type_t; duf_scanstage_t ✗ */
# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

size_t duf_li_size_array( unsigned maxdepth, unsigned r );

void duf_li_clear( duf_levinfo_t * pli );
void duf_li_clear_array( duf_levinfo_t * pli, unsigned maxdepth );
void duf_li_init( duf_levinfo_t * pli, const char *itemname, unsigned long long dirid, duf_node_type_t node_type );

void duf_li_delete_array( duf_levinfo_t * pli, unsigned maxdepth );

duf_levinfo_t *duf_li_create_array( unsigned maxdepth );
duf_levinfo_t *duf_li_clone_array( const duf_levinfo_t * lisrc, unsigned maxdepth );

/* duf_levinfo_t *duf_dirid2li( unsigned long long dirid, const char *itemname, unsigned maxdepth, int *pr ); */
DRP( LI, duf_levinfo_t *, pli, duf_li_create_array( maxdepth ), nameid2li, unsigned long long nameid, unsigned maxdepth );

#endif
