#ifndef MASXFS_LEVINFO_DIGEST_H
# define MASXFS_LEVINFO_DIGEST_H

/* #include "masxfs_levinfo_types.h" */
#include "masxfs_levinfo_digest_types.h"

masxfs_digests_t *masxfs_digests_create( void );
masxfs_digests_t *masxfs_digests_create_setup( int fd, size_t readbufsize, off_t from, off_t max_blocks );
void masxfs_digests_delete( masxfs_digests_t * digests );

int masxfs_digests_add( masxfs_digests_t * digests, masxfs_digest_type_t dgtype );

int masxfs_digests_open( masxfs_digests_t * digests );
int masxfs_digests_update( masxfs_digests_t * digests );
int masxfs_digests_close( masxfs_digests_t * digests );

int masxfs_digests_compute( masxfs_digests_t * digests );

int masxfs_digests_get( const masxfs_digests_t * digests, masxfs_digest_type_t dgtype, const unsigned char **pbuf );
int masxfs_digests_getn( const masxfs_digests_t * digests, int npos, const unsigned char **pbuf );

int masxfs_digests_is_string( masxfs_digests_t * digests, int npos );

#endif
