#ifndef MAS_DUF_LEVINFO_REF_H
# define MAS_DUF_LEVINFO_REF_H

# include "duf_levinfo_types.h"                                      /* duf_levinfo_t ✗ */
# include "duf_pdi_types.h"                                          /* duf_depthinfo_t ✗ */
# include "duf_scan_types.h"                                         /* duf_node_type_t; duf_scanstage_t ✗ */
# include "duf_levinfo_dirhandle_types.h"                            /* duf_dirhandle_t ✗ */

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

# include "duf_defs.h"

duf_levinfo_t *duf_levinfo_ptr_d( const duf_depthinfo_t * pdi, int d );
duf_levinfo_t *duf_levinfo_ptr( const duf_depthinfo_t * pdi );
duf_levinfo_t *duf_levinfo_ptr_up( const duf_depthinfo_t * pdi );

int duf_levinfo_is_leaf_d( const duf_depthinfo_t * pdi, int d );
int duf_levinfo_is_leaf( const duf_depthinfo_t * pdi );
int duf_levinfo_is_leaf_up( const duf_depthinfo_t * pdi );

duf_node_type_t duf_levinfo_node_type_d( const duf_depthinfo_t * pdi, int d );
duf_node_type_t duf_levinfo_node_type( const duf_depthinfo_t * pdi );
duf_node_type_t duf_levinfo_node_type_up( const duf_depthinfo_t * pdi );

int duf_levinfo_deleted_d( const duf_depthinfo_t * pdi, int d );
int duf_levinfo_deleted( const duf_depthinfo_t * pdi );
int duf_levinfo_deleted_up( const duf_depthinfo_t * pdi );

int duf_levinfo_deleted_tested_d( const duf_depthinfo_t * pdi, int d );
int duf_levinfo_deleted_tested( const duf_depthinfo_t * pdi );
int duf_levinfo_deleted_tested_up( const duf_depthinfo_t * pdi );

int duf_levinfo_stat_tested_d( const duf_depthinfo_t * pdi, int d );
int duf_levinfo_stat_tested( const duf_depthinfo_t * pdi );
int duf_levinfo_stat_tested_up( const duf_depthinfo_t * pdi );

const char *duf_levinfo_itemshowname_d( const duf_depthinfo_t * pdi, int d );
const char *duf_levinfo_itemshowname( const duf_depthinfo_t * pdi );
const char *duf_levinfo_itemshowname_up( const duf_depthinfo_t * pdi );
const char *duf_levinfo_itemshowname_q( const duf_depthinfo_t * pdi, const char *q );

const char *duf_levinfo_itemtruename_d( const duf_depthinfo_t * pdi, int d );
const char *duf_levinfo_itemtruename( const duf_depthinfo_t * pdi );
const char *duf_levinfo_itemtruename_up( const duf_depthinfo_t * pdi );

/* const char *duf_levinfo_itemtruename_q( const duf_depthinfo_t * pdi, const char *q ); */

void duf_levinfo_set_dirid_d( duf_depthinfo_t * pdi, unsigned long long dirid, int d );
void duf_levinfo_set_dirid( duf_depthinfo_t * pdi, unsigned long long dirid );
void duf_levinfo_set_dirid_up( duf_depthinfo_t * pdi, unsigned long long dirid );

unsigned long long duf_levinfo_dirid_d( const duf_depthinfo_t * pdi, int d );
unsigned long long duf_levinfo_dirid( const duf_depthinfo_t * pdi );
unsigned long long duf_levinfo_dirid_up( const duf_depthinfo_t * pdi );

unsigned long long duf_levinfo_nodedirid_d( const duf_depthinfo_t * pdi, int d );
unsigned long long duf_levinfo_nodedirid( const duf_depthinfo_t * pdi );
unsigned long long duf_levinfo_nodedirid_up( const duf_depthinfo_t * pdi );

unsigned long long duf_levinfo_nameid_d( const duf_depthinfo_t * pdi, int d );
unsigned long long duf_levinfo_nameid( const duf_depthinfo_t * pdi );
unsigned long long duf_levinfo_nameid_up( const duf_depthinfo_t * pdi );

# if 0
unsigned long long duf_levinfo_childs_d( const duf_depthinfo_t * pdi, int d );
unsigned long long duf_levinfo_childs( const duf_depthinfo_t * pdi );
unsigned long long duf_levinfo_childs_up( const duf_depthinfo_t * pdi );
# endif
long long duf_levinfo_numchild_d( const duf_depthinfo_t * pdi, int d );
long long duf_levinfo_numchild( const duf_depthinfo_t * pdi );
long long duf_levinfo_numchild_up( const duf_depthinfo_t * pdi );

# if 0
void duf_levinfo_set_context_d( duf_depthinfo_t * pdi, void *ctx, int d );
void duf_levinfo_set_context( duf_depthinfo_t * pdi, void *ctx );
void duf_levinfo_set_context_up( duf_depthinfo_t * pdi, void *ctx );

void *duf_levinfo_context_d( const duf_depthinfo_t * pdi, int d );
void *duf_levinfo_context( const duf_depthinfo_t * pdi );
void *duf_levinfo_context_up( const duf_depthinfo_t * pdi );
# endif

int duf_levinfo_dfd_d( const duf_depthinfo_t * pdi, int d );
int duf_levinfo_dfd( const duf_depthinfo_t * pdi );
int duf_levinfo_dfd_up( const duf_depthinfo_t * pdi );
int duf_levinfo_dfd_top( const duf_depthinfo_t * pdi );

int duf_levinfo_source_d( const duf_depthinfo_t * pdi, int d );
int duf_levinfo_source( const duf_depthinfo_t * pdi );
int duf_levinfo_source_up( const duf_depthinfo_t * pdi );

int duf_levinfo_opened_copy_d( const duf_depthinfo_t * pdi, int d );
int duf_levinfo_opened_copy( const duf_depthinfo_t * pdi );
int duf_levinfo_opened_copy_up( const duf_depthinfo_t * pdi );

duf_dirhandle_t *duf_levinfo_pdh_d( const duf_depthinfo_t * pdi, int d );
duf_dirhandle_t *duf_levinfo_pdh( const duf_depthinfo_t * pdi );
duf_dirhandle_t *duf_levinfo_pdh_up( const duf_depthinfo_t * pdi );

struct stat *duf_levinfo_stat_d( const duf_depthinfo_t * pdi, int d );
struct stat *duf_levinfo_stat( const duf_depthinfo_t * pdi );
struct stat *duf_levinfo_stat_up( const duf_depthinfo_t * pdi );

typedef long long_t;

ino_t duf_levinfo_stat_inode_d( const duf_depthinfo_t * pdi, int d );
ino_t duf_levinfo_stat_inode( const duf_depthinfo_t * pdi );
ino_t duf_levinfo_stat_inode_up( const duf_depthinfo_t * pdi );

dev_t duf_levinfo_stat_dev_d( const duf_depthinfo_t * pdi, int d );
dev_t duf_levinfo_stat_dev( const duf_depthinfo_t * pdi );
dev_t duf_levinfo_stat_dev_up( const duf_depthinfo_t * pdi );

dev_t duf_levinfo_stat_rdev( const duf_depthinfo_t * pdi );
mode_t duf_levinfo_stat_mode( const duf_depthinfo_t * pdi );
off_t duf_levinfo_stat_size( const duf_depthinfo_t * pdi );
nlink_t duf_levinfo_stat_nlink( const duf_depthinfo_t * pdi );
uid_t duf_levinfo_stat_uid( const duf_depthinfo_t * pdi );
gid_t duf_levinfo_stat_gid( const duf_depthinfo_t * pdi );
blkcnt_t duf_levinfo_stat_blocks( const duf_depthinfo_t * pdi );
blksize_t duf_levinfo_stat_blksize( const duf_depthinfo_t * pdi );
time_t duf_levinfo_stat_asec( const duf_depthinfo_t * pdi );
time_t duf_levinfo_stat_msec( const duf_depthinfo_t * pdi );
time_t duf_levinfo_stat_csec( const duf_depthinfo_t * pdi );
long_t duf_levinfo_stat_ansec( const duf_depthinfo_t * pdi );
long_t duf_levinfo_stat_mnsec( const duf_depthinfo_t * pdi );
long_t duf_levinfo_stat_cnsec( const duf_depthinfo_t * pdi );

struct stat *duf_levinfo_dbstat_d( const duf_depthinfo_t * pdi, int d );
struct stat *duf_levinfo_dbstat( const duf_depthinfo_t * pdi );
struct stat *duf_levinfo_dbstat_up( const duf_depthinfo_t * pdi );

ino_t duf_levinfo_dbstat_inode( const duf_depthinfo_t * pdi );
dev_t duf_levinfo_dbstat_dev( const duf_depthinfo_t * pdi );
mode_t duf_levinfo_dbstat_mode( const duf_depthinfo_t * pdi );
off_t duf_levinfo_dbstat_size( const duf_depthinfo_t * pdi );
nlink_t duf_levinfo_dbstat_nlink( const duf_depthinfo_t * pdi );
uid_t duf_levinfo_dbstat_uid( const duf_depthinfo_t * pdi );
gid_t duf_levinfo_dbstat_gid( const duf_depthinfo_t * pdi );
blkcnt_t duf_levinfo_dbstat_blocks( const duf_depthinfo_t * pdi );
blksize_t duf_levinfo_dbstat_blksize( const duf_depthinfo_t * pdi );
time_t duf_levinfo_dbstat_asec( const duf_depthinfo_t * pdi );
time_t duf_levinfo_dbstat_msec( const duf_depthinfo_t * pdi );
time_t duf_levinfo_dbstat_csec( const duf_depthinfo_t * pdi );
long_t duf_levinfo_dbstat_ansec( const duf_depthinfo_t * pdi );
long_t duf_levinfo_dbstat_mnsec( const duf_depthinfo_t * pdi );
long_t duf_levinfo_dbstat_cnsec( const duf_depthinfo_t * pdi );

const char *duf_levinfo_path_d( const duf_depthinfo_t * pdi, int d );
const char *duf_levinfo_path( const duf_depthinfo_t * pdi );
const char *duf_levinfo_path_up( const duf_depthinfo_t * pdi );
const char *duf_levinfo_path_top( const duf_depthinfo_t * pdi );

const char *duf_levinfo_relpath_d( const duf_depthinfo_t * pdi, int d );
const char *duf_levinfo_relpath( const duf_depthinfo_t * pdi );
const char *duf_levinfo_relpath_up( const duf_depthinfo_t * pdi );

const char *duf_levinfo_path_q( const duf_depthinfo_t * pdi, const char *q );
char *duf_levinfo_path_qdup( const duf_depthinfo_t * pdi, const char *q );

#endif
