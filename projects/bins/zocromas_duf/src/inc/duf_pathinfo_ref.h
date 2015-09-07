#ifndef DUF_PATHINFO_REF_H
#  define DUF_PATHINFO_REF_H

#  include "duf_levinfo_types.h"




duf_levinfo_t *duf_pathinfo_ptr_d( const duf_pathinfo_t * pi, int d );
duf_levinfo_t *duf_pathinfo_ptr( const duf_pathinfo_t * pi );
duf_levinfo_t *duf_pathinfo_ptr_up( const duf_pathinfo_t * pi );

int duf_pathinfo_is_leaf_d( const duf_pathinfo_t * pi, int d );
int duf_pathinfo_is_leaf( const duf_pathinfo_t * pi );
int duf_pathinfo_is_leaf_up( const duf_pathinfo_t * pi );

int duf_pathinfo_item_deleted_d( const duf_pathinfo_t * pi, int d );
int duf_pathinfo_item_deleted( const duf_pathinfo_t * pi );
int duf_pathinfo_item_deleted_up( const duf_pathinfo_t * pi );

const char *duf_pathinfo_itemshowname_d( const duf_pathinfo_t * pi, int d );
const char *duf_pathinfo_itemshowname( const duf_pathinfo_t * pi );
const char *duf_pathinfo_itemshowname_up( const duf_pathinfo_t * pi );
const char *duf_pathinfo_itemshowname_q( const duf_pathinfo_t * pi, const char *q );

const char *duf_pathinfo_itemtruename_d( const duf_pathinfo_t * pi, int d );
const char *duf_pathinfo_itemtruename( const duf_pathinfo_t * pi );
const char *duf_pathinfo_itemtruename_up( const duf_pathinfo_t * pi );
const char *duf_pathinfo_itemtruename_q( const duf_pathinfo_t * pi, const char *q );

void duf_pathinfo_set_dirid_d( duf_pathinfo_t * pi, unsigned long long dirid, int d );
void duf_pathinfo_set_dirid( duf_pathinfo_t * pi, unsigned long long dirid );
void duf_pathinfo_set_dirid_up( duf_pathinfo_t * pi, unsigned long long dirid );

unsigned long long duf_pathinfo_dirid_d( const duf_pathinfo_t * pi, int d );
unsigned long long duf_pathinfo_dirid( const duf_pathinfo_t * pi );
unsigned long long duf_pathinfo_dirid_up( const duf_pathinfo_t * pi );

unsigned long long duf_pathinfo_nodedirid_d( const duf_pathinfo_t * pi, int d );
unsigned long long duf_pathinfo_nodedirid( const duf_pathinfo_t * pi );
unsigned long long duf_pathinfo_nodedirid_up( const duf_pathinfo_t * pi );

unsigned long long duf_pathinfo_nameid_d( const duf_pathinfo_t * pi, int d );
unsigned long long duf_pathinfo_nameid( const duf_pathinfo_t * pi );
unsigned long long duf_pathinfo_nameid_up( const duf_pathinfo_t * pi );

unsigned long long duf_pathinfo_items_files_d( const duf_pathinfo_t * pi, int d );
unsigned long long duf_pathinfo_items_files( const duf_pathinfo_t * pi );
unsigned long long duf_pathinfo_items_files_up( const duf_pathinfo_t * pi );

void duf_pathinfo_set_items_files_d( duf_pathinfo_t * pi, unsigned long long n, int d );
void duf_pathinfo_set_items_files( duf_pathinfo_t * pi, unsigned long long n );
void duf_pathinfo_set_items_files_up( duf_pathinfo_t * pi, unsigned long long n );

int duf_pathinfo_numdir_d( const duf_pathinfo_t * pi, int d );
int duf_pathinfo_numdir( const duf_pathinfo_t * pi );
int duf_pathinfo_numdir_up( const duf_pathinfo_t * pi );

void duf_pathinfo_set_context_d( duf_pathinfo_t * pi, void *ctx, int d );
void duf_pathinfo_set_context( duf_pathinfo_t * pi, void *ctx );
void duf_pathinfo_set_context_up( duf_pathinfo_t * pi, void *ctx );

void *duf_pathinfo_context_d( const duf_pathinfo_t * pi, int d );
void *duf_pathinfo_context( const duf_pathinfo_t * pi );
void *duf_pathinfo_context_up( const duf_pathinfo_t * pi );


int duf_pathinfo_dfd_d( const duf_pathinfo_t * pi, int d );
int duf_pathinfo_dfd( const duf_pathinfo_t * pi );
int duf_pathinfo_dfd_up( const duf_pathinfo_t * pi );
int duf_pathinfo_dfd_top( const duf_pathinfo_t * pi );

int duf_pathinfo_source_d( const duf_pathinfo_t * pi, int d );
int duf_pathinfo_source( const duf_pathinfo_t * pi );
int duf_pathinfo_source_up( const duf_pathinfo_t * pi );

duf_dirhandle_t *duf_pathinfo_pdh_d( const duf_pathinfo_t * pi, int d );
duf_dirhandle_t *duf_pathinfo_pdh( const duf_pathinfo_t * pi );
duf_dirhandle_t *duf_pathinfo_pdh_up( const duf_pathinfo_t * pi );

struct stat *duf_pathinfo_stat_d( const duf_pathinfo_t * pi, int d );
struct stat *duf_pathinfo_stat( const duf_pathinfo_t * pi );
struct stat *duf_pathinfo_stat_up( const duf_pathinfo_t * pi );

typedef long long_t;

ino_t duf_pathinfo_stat_inode( const duf_pathinfo_t * pi );
dev_t duf_pathinfo_stat_dev( const duf_pathinfo_t * pi );
mode_t duf_pathinfo_stat_mode( const duf_pathinfo_t * pi );
off_t duf_pathinfo_stat_size( const duf_pathinfo_t * pi );
nlink_t duf_pathinfo_stat_nlink( const duf_pathinfo_t * pi );
uid_t duf_pathinfo_stat_uid( const duf_pathinfo_t * pi );
gid_t duf_pathinfo_stat_gid( const duf_pathinfo_t * pi );
blkcnt_t duf_pathinfo_stat_blocks( const duf_pathinfo_t * pi );
blksize_t duf_pathinfo_stat_blksize( const duf_pathinfo_t * pi );
time_t duf_pathinfo_stat_asec( const duf_pathinfo_t * pi );
time_t duf_pathinfo_stat_msec( const duf_pathinfo_t * pi );
time_t duf_pathinfo_stat_csec( const duf_pathinfo_t * pi );
long_t duf_pathinfo_stat_ansec( const duf_pathinfo_t * pi );
long_t duf_pathinfo_stat_mnsec( const duf_pathinfo_t * pi );
long_t duf_pathinfo_stat_cnsec( const duf_pathinfo_t * pi );


struct stat *duf_pathinfo_dbstat_d( const duf_pathinfo_t * pi, int d );
struct stat *duf_pathinfo_dbstat( const duf_pathinfo_t * pi );
struct stat *duf_pathinfo_dbstat_up( const duf_pathinfo_t * pi );

ino_t duf_pathinfo_dbstat_inode( const duf_pathinfo_t * pi );
dev_t duf_pathinfo_dbstat_dev( const duf_pathinfo_t * pi );
mode_t duf_pathinfo_dbstat_mode( const duf_pathinfo_t * pi );
off_t duf_pathinfo_dbstat_size( const duf_pathinfo_t * pi );
nlink_t duf_pathinfo_dbstat_nlink( const duf_pathinfo_t * pi );
uid_t duf_pathinfo_dbstat_uid( const duf_pathinfo_t * pi );
gid_t duf_pathinfo_dbstat_gid( const duf_pathinfo_t * pi );
blkcnt_t duf_pathinfo_dbstat_blocks( const duf_pathinfo_t * pi );
blksize_t duf_pathinfo_dbstat_blksize( const duf_pathinfo_t * pi );
time_t duf_pathinfo_dbstat_asec( const duf_pathinfo_t * pi );
time_t duf_pathinfo_dbstat_msec( const duf_pathinfo_t * pi );
time_t duf_pathinfo_dbstat_csec( const duf_pathinfo_t * pi );
long_t duf_pathinfo_dbstat_ansec( const duf_pathinfo_t * pi );
long_t duf_pathinfo_dbstat_mnsec( const duf_pathinfo_t * pi );
long_t duf_pathinfo_dbstat_cnsec( const duf_pathinfo_t * pi );





const char *duf_pathinfo_path_d( const duf_pathinfo_t * pi, int d );
const char *duf_pathinfo_path( const duf_pathinfo_t * pi );
const char *duf_pathinfo_path_up( const duf_pathinfo_t * pi );
const char *duf_pathinfo_path_top( const duf_pathinfo_t * pi );

const char *duf_pathinfo_relpath_d( const duf_pathinfo_t * pi, int d );
const char *duf_pathinfo_relpath( const duf_pathinfo_t * pi );
const char *duf_pathinfo_relpath_up( const duf_pathinfo_t * pi );

const char *duf_pathinfo_path_q( const duf_pathinfo_t * pi, const char *q );
char *duf_pathinfo_path_qdup( const duf_pathinfo_t * pi, const char *q );




/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/

int duf_pathinfo_if_statat_dh_d( duf_pathinfo_t * pi, int d );
int duf_pathinfo_if_statat_dh( duf_pathinfo_t * pi );
int duf_pathinfo_if_statat_dh_up( duf_pathinfo_t * pi );

int duf_pathinfo_statat_dh_d( duf_pathinfo_t * pi, int d );
int duf_pathinfo_statat_dh( duf_pathinfo_t * pi );
int duf_pathinfo_statat_dh_up( duf_pathinfo_t * pi );


int duf_pathinfo_if_openat_dh_d( duf_pathinfo_t * pi, int d );
int duf_pathinfo_if_openat_dh( duf_pathinfo_t * pi );
int duf_pathinfo_if_openat_dh_up( duf_pathinfo_t * pi );

/* int duf_pathinfo_open_dh( duf_pathinfo_t * pi, const char *path ); */

int duf_pathinfo_openat_dh_d( duf_pathinfo_t * pi, int d );
int duf_pathinfo_openat_dh( duf_pathinfo_t * pi );
int duf_pathinfo_openat_dh_up( duf_pathinfo_t * pi );



int duf_pathinfo_opened_dh_d( duf_pathinfo_t * pi, int d );
int duf_pathinfo_opened_dh( duf_pathinfo_t * pi );
int duf_pathinfo_opened_dh_up( duf_pathinfo_t * pi );

int duf_pathinfo_closeat_dh_d( duf_pathinfo_t * pi, int d );
int duf_pathinfo_closeat_dh( duf_pathinfo_t * pi );
int duf_pathinfo_closeat_dh_up( duf_pathinfo_t * pi );



int duf_pathinfo_depth( const duf_pathinfo_t * pi );
void duf_pathinfo_set_topdepth( duf_pathinfo_t * pi );

int duf_pathinfo_topdepth( const duf_pathinfo_t * pi );

int duf_pathinfo_deltadepth_d( const duf_pathinfo_t * pi, int d );
int duf_pathinfo_deltadepth( const duf_pathinfo_t * pi );
int duf_pathinfo_deltadepth_up( const duf_pathinfo_t * pi );



#endif
