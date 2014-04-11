#ifndef MAS_DUF_UPDATE_PATH_H
#  define MAS_DUF_UPDATE_PATH_H

unsigned long long duf_update_path( const char *path, unsigned long long parentid, duf_ufilter_t u, int level, int *pseq, int dofiles );

unsigned long long duf_update_path_down_filter( const char *path, unsigned long long parentid, duf_filter_t * pfilter );
unsigned long long duf_update_path_down_filter_uni( const char *path, unsigned long long parentid, duf_filter_t * pfilter );

unsigned long long duf_update_path_down( const char *path, unsigned long long parentid, duf_ufilter_t u, int level, int *pseq,
                                         int dofiles );

#endif
