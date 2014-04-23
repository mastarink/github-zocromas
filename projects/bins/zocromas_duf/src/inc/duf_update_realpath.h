#ifndef MAS_DUF_UPDATE_REALPATH_H
#  define MAS_DUF_UPDATE_REALPATH_H

/* unsigned long long duf_update_realpath_down_filter( const char *real_path, unsigned long long parentid, duf_filter_t * pfilter ); */
/* unsigned long long duf_update_realpath_down( const char *real_path, unsigned long long parentid, int recursive, int level, int maxdepth, */
/*                                              int *pseq, int maxseq, int dofiles );                                                       */


unsigned long long duf_update_realpath_self_up( const char *real_path, const char *group, int notup, int need_id, int *pr );

#endif
