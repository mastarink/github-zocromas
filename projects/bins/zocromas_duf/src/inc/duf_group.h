#ifndef MAS_DUF_GROUP_H
#  define MAS_DUF_GROUP_H

unsigned long long duf_insert_group( const char *name );
unsigned long long duf_insert_path_group( unsigned long long groupid, unsigned long long pathid );

unsigned long long duf_group_to_groupid( const char *group );

#endif
