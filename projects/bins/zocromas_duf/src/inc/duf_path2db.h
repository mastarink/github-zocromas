#ifndef MAS_DUF_PATH2DB_H
# define MAS_DUF_PATH2DB_H

# include "duf_hook_types.h"                                         /* duf_sql_set_t */
# include "duf_levinfo_types.h"                                      /* duf_depthinfo_t */

int duf_levinfo_stat2dirid( duf_depthinfo_t * pdi, int caninsert, const duf_sql_set_t * sql_set );

int duf_real_path2db( duf_depthinfo_t * pdi, int caninsert, const char *rpath, const duf_sql_set_t * sql_set );

#endif
