#ifndef MAS_DUF_MD5_H
#  define MAS_DUF_MD5_H

int duf_update_md5( void );
int duf_print_md5( unsigned long long limit );
int duf_print_md5_same( unsigned long long dupcnt_min, unsigned long long limit );

#endif
