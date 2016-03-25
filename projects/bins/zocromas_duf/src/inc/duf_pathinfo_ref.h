#ifndef MAS_DUF_PATHINFO_REF_H
# define MAS_DUF_PATHINFO_REF_H

/* # include <sys/types.h> */
# include <sys/stat.h>
/* # include <unistd.h> */

# include "duf_levinfo_types.h"                                      /* duf_levinfo_t ✗ */
# include "duf_pathinfo_types.h"                                     /* duf_pathinfo_t ✗ */
# include "duf_levinfo_dirhandle_types.h"                            /* duf_dirhandle_t ✗ */
# include "duf_scan_types.h"                                         /* duf_node_type_t; duf_scanstage_t ✗ */

duf_levinfo_t *duf_pi_ptr_d( const duf_pathinfo_t * pi, int d );
duf_levinfo_t *duf_pi_ptr( const duf_pathinfo_t * pi );
duf_levinfo_t *duf_pi_ptr_up( const duf_pathinfo_t * pi );

int duf_pi_item_deleted_d( const duf_pathinfo_t * pi, int d );
int duf_pi_item_deleted( const duf_pathinfo_t * pi );
int duf_pi_item_deleted_up( const duf_pathinfo_t * pi );

const char *duf_pi_itemshowname_d( const duf_pathinfo_t * pi, int d );
const char *duf_pi_itemshowname( const duf_pathinfo_t * pi );
const char *duf_pi_itemshowname_up( const duf_pathinfo_t * pi );
const char *duf_pi_itemshowname_q( const duf_pathinfo_t * pi, const char *q );

const char *duf_pi_itemtruename_d( const duf_pathinfo_t * pi, int d );
const char *duf_pi_itemtruename( const duf_pathinfo_t * pi );
const char *duf_pi_itemtruename_up( const duf_pathinfo_t * pi );
const char *duf_pi_itemtruename_q( const duf_pathinfo_t * pi, const char *q );

/*
void duf_pi_set_dirid_d( duf_pathinfo_t * pi, unsigned long long dirid, int d );
void duf_pi_set_dirid( duf_pathinfo_t * pi, unsigned long long dirid );
void duf_pi_set_dirid_up( duf_pathinfo_t * pi, unsigned long long dirid );

unsigned long long duf_pi_dirid_d( const duf_pathinfo_t * pi, int d );
unsigned long long duf_pi_dirid( const duf_pathinfo_t * pi );
unsigned long long duf_pi_dirid_up( const duf_pathinfo_t * pi );

unsigned long long duf_pi_nodedirid_d( const duf_pathinfo_t * pi, int d );
unsigned long long duf_pi_nodedirid( const duf_pathinfo_t * pi );
unsigned long long duf_pi_nodedirid_up( const duf_pathinfo_t * pi );

unsigned long long duf_pi_nameid_d( const duf_pathinfo_t * pi, int d );
unsigned long long duf_pi_nameid( const duf_pathinfo_t * pi );
unsigned long long duf_pi_nameid_up( const duf_pathinfo_t * pi );
*/

unsigned long long duf_pi_items_files_d( const duf_pathinfo_t * pi, int d );
unsigned long long duf_pi_items_files( const duf_pathinfo_t * pi );
unsigned long long duf_pi_items_files_up( const duf_pathinfo_t * pi );

void duf_pi_set_items_files_d( duf_pathinfo_t * pi, unsigned long long n, int d );
void duf_pi_set_items_files( duf_pathinfo_t * pi, unsigned long long n );
void duf_pi_set_items_files_up( duf_pathinfo_t * pi, unsigned long long n );

int duf_pi_numdir_d( const duf_pathinfo_t * pi, int d );
int duf_pi_numdir( const duf_pathinfo_t * pi );
int duf_pi_numdir_up( const duf_pathinfo_t * pi );

void duf_pi_set_context_d( duf_pathinfo_t * pi, void *ctx, int d );
void duf_pi_set_context( duf_pathinfo_t * pi, void *ctx );
void duf_pi_set_context_up( duf_pathinfo_t * pi, void *ctx );

void *duf_pi_context_d( const duf_pathinfo_t * pi, int d );
void *duf_pi_context( const duf_pathinfo_t * pi );
void *duf_pi_context_up( const duf_pathinfo_t * pi );

int duf_pi_dfd_d( const duf_pathinfo_t * pi, int d );
int duf_pi_dfd( const duf_pathinfo_t * pi );
int duf_pi_dfd_up( const duf_pathinfo_t * pi );
int duf_pi_dfd_top( const duf_pathinfo_t * pi );

int duf_pi_source_d( const duf_pathinfo_t * pi, int d );
int duf_pi_source( const duf_pathinfo_t * pi );
int duf_pi_source_up( const duf_pathinfo_t * pi );

duf_dirhandle_t *duf_pi_pdh_d( const duf_pathinfo_t * pi, int d );
duf_dirhandle_t *duf_pi_pdh( const duf_pathinfo_t * pi );
duf_dirhandle_t *duf_pi_pdh_up( const duf_pathinfo_t * pi );

struct stat *duf_pi_stat_d( const duf_pathinfo_t * pi, int d );
struct stat *duf_pi_stat( const duf_pathinfo_t * pi );
struct stat *duf_pi_stat_up( const duf_pathinfo_t * pi );

typedef long long_t;

ino_t duf_pi_stat_inode( const duf_pathinfo_t * pi );
dev_t duf_pi_stat_dev( const duf_pathinfo_t * pi );
mode_t duf_pi_stat_mode( const duf_pathinfo_t * pi );
off_t duf_pi_stat_size( const duf_pathinfo_t * pi );
nlink_t duf_pi_stat_nlink( const duf_pathinfo_t * pi );
uid_t duf_pi_stat_uid( const duf_pathinfo_t * pi );
gid_t duf_pi_stat_gid( const duf_pathinfo_t * pi );
blkcnt_t duf_pi_stat_blocks( const duf_pathinfo_t * pi );
blksize_t duf_pi_stat_blksize( const duf_pathinfo_t * pi );
time_t duf_pi_stat_asec( const duf_pathinfo_t * pi );
time_t duf_pi_stat_msec( const duf_pathinfo_t * pi );
time_t duf_pi_stat_csec( const duf_pathinfo_t * pi );
long_t duf_pi_stat_ansec( const duf_pathinfo_t * pi );
long_t duf_pi_stat_mnsec( const duf_pathinfo_t * pi );
long_t duf_pi_stat_cnsec( const duf_pathinfo_t * pi );

struct stat *duf_pi_dbstat_d( const duf_pathinfo_t * pi, int d );
struct stat *duf_pi_dbstat( const duf_pathinfo_t * pi );
struct stat *duf_pi_dbstat_up( const duf_pathinfo_t * pi );

ino_t duf_pi_dbstat_inode( const duf_pathinfo_t * pi );
dev_t duf_pi_dbstat_dev( const duf_pathinfo_t * pi );
mode_t duf_pi_dbstat_mode( const duf_pathinfo_t * pi );
off_t duf_pi_dbstat_size( const duf_pathinfo_t * pi );
nlink_t duf_pi_dbstat_nlink( const duf_pathinfo_t * pi );
uid_t duf_pi_dbstat_uid( const duf_pathinfo_t * pi );
gid_t duf_pi_dbstat_gid( const duf_pathinfo_t * pi );
blkcnt_t duf_pi_dbstat_blocks( const duf_pathinfo_t * pi );
blksize_t duf_pi_dbstat_blksize( const duf_pathinfo_t * pi );
time_t duf_pi_dbstat_asec( const duf_pathinfo_t * pi );
time_t duf_pi_dbstat_msec( const duf_pathinfo_t * pi );
time_t duf_pi_dbstat_csec( const duf_pathinfo_t * pi );
long_t duf_pi_dbstat_ansec( const duf_pathinfo_t * pi );
long_t duf_pi_dbstat_mnsec( const duf_pathinfo_t * pi );
long_t duf_pi_dbstat_cnsec( const duf_pathinfo_t * pi );

const char *duf_pi_path_d( const duf_pathinfo_t * pi, int d );
const char *duf_pi_path( const duf_pathinfo_t * pi );
const char *duf_pi_path_up( const duf_pathinfo_t * pi );
const char *duf_pi_path_top( const duf_pathinfo_t * pi );

const char *duf_pi_relpath_d( const duf_pathinfo_t * pi, int d );
const char *duf_pi_relpath( const duf_pathinfo_t * pi );
const char *duf_pi_relpath_up( const duf_pathinfo_t * pi );

duf_node_type_t duf_pi_node_type_d( const duf_pathinfo_t * pi, int d );
duf_node_type_t duf_pi_node_type( const duf_pathinfo_t * pi );
duf_node_type_t duf_pi_node_type_up( const duf_pathinfo_t * pi );

const char *duf_pi_path_q( const duf_pathinfo_t * pi, const char *q );
char *duf_pi_path_qdup( const duf_pathinfo_t * pi, const char *q );

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/

int duf_pi_if_statat_dh_d( duf_pathinfo_t * pi, int d );
int duf_pi_if_statat_dh( duf_pathinfo_t * pi );
int duf_pi_if_statat_dh_up( duf_pathinfo_t * pi );

int duf_pi_statat_dh_d( duf_pathinfo_t * pi, int d );
int duf_pi_statat_dh( duf_pathinfo_t * pi );
int duf_pi_statat_dh_up( duf_pathinfo_t * pi );

int duf_pi_if_openat_dh_d( duf_pathinfo_t * pi, int d );
int duf_pi_if_openat_dh( duf_pathinfo_t * pi );
int duf_pi_if_openat_dh_up( duf_pathinfo_t * pi );

/* int duf_pi_open_dh( duf_pathinfo_t * pi, const char *path ); */

int duf_pi_openat_dh_d( duf_pathinfo_t * pi, int d );
int duf_pi_openat_dh( duf_pathinfo_t * pi );
int duf_pi_openat_dh_up( duf_pathinfo_t * pi );

int duf_pi_opened_dh_d( duf_pathinfo_t * pi, int d );
int duf_pi_opened_dh( duf_pathinfo_t * pi );
int duf_pi_opened_dh_up( duf_pathinfo_t * pi );

int duf_pi_closeat_dh_d( duf_pathinfo_t * pi, int d );
int duf_pi_closeat_dh( duf_pathinfo_t * pi );
int duf_pi_closeat_dh_up( duf_pathinfo_t * pi );

int duf_pi_depth( const duf_pathinfo_t * pi );
void duf_pi_set_topdepth( duf_pathinfo_t * pi );

int duf_pi_topdepth( const duf_pathinfo_t * pi );
int duf_pi_maxdepth( const duf_pathinfo_t * pi );
duf_levinfo_t *duf_pi_levinfo( const duf_pathinfo_t * pi );

int duf_pi_deltadepth_d( const duf_pathinfo_t * pi, int d );
int duf_pi_deltadepth( const duf_pathinfo_t * pi );
int duf_pi_deltadepth_up( const duf_pathinfo_t * pi );

#endif
