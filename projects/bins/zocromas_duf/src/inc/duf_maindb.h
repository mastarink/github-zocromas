#ifndef MAS_DUF_MAINDB_H
# define MAS_DUF_MAINDB_H

# include "duf_pdi_types.h"                                          /* duf_depthinfo_t ✗ */

# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

DR( TOP, main_db ) __attribute__ ( ( warn_unused_result ) );

DR( TOP, main_db_open, duf_depthinfo_t * pdi ) __attribute__ ( ( warn_unused_result ) );
DR( TOP, main_db_close, duf_depthinfo_t * pdi ) __attribute__ ( ( warn_unused_result ) );

/* int duf_main_db_pre_action( void ); */

#endif
