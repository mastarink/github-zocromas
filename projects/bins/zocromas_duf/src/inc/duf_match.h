#ifndef MAS_DUF_MATCH_H
#  define MAS_DUF_MATCH_H

#  include "duf_ufilter_types.h"

int duf_filename_match( duf_filter_glob_t * glob, const char *filename );


int duf_lim_match( duf_limits_t lim, int filesame );
int duf_lim_matchll( duf_limitsll_t lim, int filesame );

/* int duf_md5id_match( unsigned long long md5id_filter, unsigned long long md5id ); */



#endif

