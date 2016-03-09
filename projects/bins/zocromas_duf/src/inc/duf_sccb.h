#ifndef MAS_DUF_SCCB_H
# define MAS_DUF_SCCB_H

# include "duf_scan_types.h"                                         /* duf_node_type_t; duf_scanstage_t ✗ */
# include "duf_hook_types.h"                                         /* duf_action_table_t ✗ */

const char *duf_uni_scan_action_title( const duf_scan_callbacks_t * sccb );

/* const duf_scan_callbacks_t *duf_find_sccb_by_evnamen( const char *name, size_t namelen, const duf_scan_callbacks_t * first ); */
const duf_scan_callbacks_t *duf_find_or_load_sccb_by_evnamen( const char *name, size_t namelen, duf_scan_callbacks_t * first );
void duf_sccb_dlclose( duf_scan_callbacks_t * first );

/* const duf_scan_callbacks_t *duf_load_sccb_by_evnamen( const char *name, size_t namelen, duf_scan_callbacks_t * first ); */

const duf_scan_callbacks_t *duf_find_sccb_by_evname( const char *name, const duf_scan_callbacks_t * first );

/* const duf_scan_callbacks_t *duf_find_or_load_sccb_by_evname( const char *name, duf_scan_callbacks_t * first ); */

/* const duf_sql_set_t *duf_get_leaf_sql_set( const duf_scan_callbacks_t * sccb ); */
/* const duf_sql_set_t *duf_get_node_sql_set( const duf_scan_callbacks_t * sccb ); */
/* const duf_sql_set_t *duf_sccb_get_sql_set( const duf_scan_callbacks_t * sccb, duf_node_type_t node_type ); */
# if 0
/* 20160211.141253 */
const duf_sql_set_t *duf_sccb_get_sql_set_f( const duf_scan_callbacks_t * sccb, duf_node_type_t node_type, unsigned force_leaf_index,
                                             unsigned force_node_index );
# endif

#endif
