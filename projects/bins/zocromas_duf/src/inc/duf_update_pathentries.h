#ifndef MAS_DUF_UPDATE_PATHENTRIES_H
#  define MAS_DUF_UPDATE_PATHENTRIES_H

int duf_update_realpath_entries( const char *real_path, struct stat *pst_dir, unsigned long long dir_id, int recursive, int dofiles );

#endif