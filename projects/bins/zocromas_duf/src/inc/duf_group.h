#ifndef MAS_DUF_GROUP_H
#  define MAS_DUF_GROUP_H

unsigned long long duf_insert_group( const char *name );
unsigned long long duf_insert_path_group( unsigned long long groupid, unsigned long long pathid );

unsigned long long duf_group_to_groupid( const char *group );

void duf_pathid_group( const char *group, unsigned long long pathid, int add_remove );
void duf_paths_group( const char *group, const char *path, int add_remove );

/* int duf_print_paths( const char *groupname ); */

#endif
