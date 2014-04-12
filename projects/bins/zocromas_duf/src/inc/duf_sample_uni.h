#ifndef MAS_DUF_SAMPLE_UNI_H
#  define MAS_DUF_SAMPLE_UNI_H

/* callback of type duf_scan_callback_dir_t */
int duf_sample_dir_uni( unsigned long long pathid, const char *name, unsigned long long items,
                               duf_dirinfo_t * pdi, struct duf_scan_callbacks_s *sccb );

/* callback of type duf_scan_callback_file_t */
int duf_sample_files_uni( unsigned long long pathid, unsigned long long filenameid,
                                     const char *name, void *str_cb_udata, duf_dirinfo_t * pdi,
                                     struct duf_scan_callbacks_s *sccb, duf_record_t * precord );

#endif

