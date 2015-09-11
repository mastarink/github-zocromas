#ifndef MAS_DUF_SCCB_BEGFIN_H
#  define MAS_DUF_SCCB_BEGFIN_H

int duf_scan_beginning_sql( const duf_scan_callbacks_t * sccb, char *selected_db ) __attribute__ ( ( warn_unused_result ) );
int duf_scan_final_sql( const duf_scan_callbacks_t * sccb ) __attribute__ ( ( warn_unused_result ) );

#endif

/*
vi: ft=c
*/
