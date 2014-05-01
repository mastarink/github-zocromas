#ifndef MAS_DUF_ADD_H
#  define MAS_DUF_ADD_H

unsigned long long duf_add_path_uni( const char *path, int need_id, int *pr );

unsigned long long duf_insert_path_uni( const char *dename, dev_t dev_id, ino_t dir_ino, unsigned long long parentid, int need_id,
                                        int *pr );

#endif
