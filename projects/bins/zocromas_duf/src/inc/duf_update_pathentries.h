#ifndef MAS_DUF_UPDATE_PATHENTRIES_H
#  define MAS_DUF_UPDATE_PATHENTRIES_H

int duf_update_realpath_entries_filter( unsigned long long dir_id, duf_filter_t * pfilter );
int duf_fill_entries_filter_uni( unsigned long long pathid, duf_dirinfo_t * pdi );

#endif
