#ifndef MAS_DUF_UPDATE_PATHID_H
#  define MAS_DUF_UPDATE_PATHID_H

unsigned long long duf_update_pathid( unsigned long long pathid, unsigned long long parentid, int recursive, int dofiles );
unsigned long long duf_update_pathid_down( unsigned long long parentid, int recursive, int dofiles, unsigned long long pathid );

#endif
