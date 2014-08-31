#ifndef DUF_LEVINFO_REF_H
#  define DUF_LEVINFO_REF_H

#  include "duf_levinfo_types.h"

duf_levinfo_t *duf_levinfo_ptr_d( const duf_depthinfo_t * pdi, int d );
duf_levinfo_t *duf_levinfo_ptr( const duf_depthinfo_t * pdi );
duf_levinfo_t *duf_levinfo_ptr_up( const duf_depthinfo_t * pdi );

int duf_levinfo_is_leaf_d( const duf_depthinfo_t * pdi, int d );
int duf_levinfo_is_leaf( const duf_depthinfo_t * pdi );
int duf_levinfo_is_leaf_up( const duf_depthinfo_t * pdi );

int duf_levinfo_item_deleted_d( const duf_depthinfo_t * pdi, int d );
int duf_levinfo_item_deleted( const duf_depthinfo_t * pdi );
int duf_levinfo_item_deleted_up( const duf_depthinfo_t * pdi );

const char *duf_levinfo_itemname_d( const duf_depthinfo_t * pdi, int d );
const char *duf_levinfo_itemname( const duf_depthinfo_t * pdi );
const char *duf_levinfo_itemname_up( const duf_depthinfo_t * pdi );
const char *duf_levinfo_itemname_q( const duf_depthinfo_t * pdi, const char *q );

void duf_levinfo_set_dirid_d( duf_depthinfo_t * pdi, unsigned long long dirid, int d );
void duf_levinfo_set_dirid( duf_depthinfo_t * pdi, unsigned long long dirid );
void duf_levinfo_set_dirid_up( duf_depthinfo_t * pdi, unsigned long long dirid );

unsigned long long duf_levinfo_items_files_d( const duf_depthinfo_t * pdi, int d );
unsigned long long duf_levinfo_items_files( const duf_depthinfo_t * pdi );
unsigned long long duf_levinfo_items_files_up( const duf_depthinfo_t * pdi );

void duf_levinfo_set_items_files_d( duf_depthinfo_t * pdi, unsigned long long n, int d );
void duf_levinfo_set_items_files( duf_depthinfo_t * pdi, unsigned long long n );
void duf_levinfo_set_items_files_up( duf_depthinfo_t * pdi, unsigned long long n );

unsigned long long duf_levinfo_dirid_d( const duf_depthinfo_t * pdi, int d );
unsigned long long duf_levinfo_dirid( const duf_depthinfo_t * pdi );
unsigned long long duf_levinfo_dirid_up( const duf_depthinfo_t * pdi );

int duf_levinfo_numdir_d( const duf_depthinfo_t * pdi, int d );
int duf_levinfo_numdir( const duf_depthinfo_t * pdi );
int duf_levinfo_numdir_up( const duf_depthinfo_t * pdi );

void duf_levinfo_set_context_d( duf_depthinfo_t * pdi, void *ctx, int d );
void duf_levinfo_set_context( duf_depthinfo_t * pdi, void *ctx );
void duf_levinfo_set_context_up( duf_depthinfo_t * pdi, void *ctx );

void *duf_levinfo_context_d( const duf_depthinfo_t * pdi, int d );
void *duf_levinfo_context( const duf_depthinfo_t * pdi );
void *duf_levinfo_context_up( const duf_depthinfo_t * pdi );


int duf_levinfo_opened_dh_d( duf_depthinfo_t * pdi, int d );
int duf_levinfo_opened_dh( duf_depthinfo_t * pdi );
int duf_levinfo_opened_dh_up( duf_depthinfo_t * pdi );

int duf_levinfo_closeat_dh_d( duf_depthinfo_t * pdi, int d );
int duf_levinfo_closeat_dh( duf_depthinfo_t * pdi );
int duf_levinfo_closeat_dh_up( duf_depthinfo_t * pdi );

int duf_levinfo_dfd_d( const duf_depthinfo_t * pdi, int d );
int duf_levinfo_dfd( const duf_depthinfo_t * pdi );
int duf_levinfo_dfd_up( const duf_depthinfo_t * pdi );

duf_dirhandle_t *duf_levinfo_pdh_d( const duf_depthinfo_t * pdi, int d );
duf_dirhandle_t *duf_levinfo_pdh( const duf_depthinfo_t * pdi );
duf_dirhandle_t *duf_levinfo_pdh_up( const duf_depthinfo_t * pdi );

struct stat *duf_levinfo_stat_d( const duf_depthinfo_t * pdi, int d );
struct stat *duf_levinfo_stat( const duf_depthinfo_t * pdi );
struct stat *duf_levinfo_stat_up( const duf_depthinfo_t * pdi );

const char *duf_levinfo_path_d( const duf_depthinfo_t * pdi, int d );
const char *duf_levinfo_path( const duf_depthinfo_t * pdi );
const char *duf_levinfo_path_up( const duf_depthinfo_t * pdi );
const char *duf_levinfo_path_q( const duf_depthinfo_t * pdi, const char *q );


#endif
