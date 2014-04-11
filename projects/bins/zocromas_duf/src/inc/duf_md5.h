#ifndef MAS_DUF_MD5_H
#  define MAS_DUF_MD5_H

int duf_update_md5_path( const char *path, duf_ufilter_t u );

/* int duf_print_md5( unsigned long long limit ); */
int duf_print_md5_same( unsigned long long dupcnt_min, unsigned long long limit );


unsigned long long duf_insert_md5( unsigned long long *md64, size_t fsize );

#endif
