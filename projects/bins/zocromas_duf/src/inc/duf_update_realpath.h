#ifndef MAS_DUF_UPDATE_REALPATH_H
#  define MAS_DUF_UPDATE_REALPATH_H

unsigned long long duf_update_realpath_down( const char *real_path, unsigned long long parentid, int recursive, int dofiles );

unsigned long long duf_update_realpath_self_up( const char *real_path, const char *group, int notup );

#endif
