#ifndef MAS_DUF_OPTION_NAMES_H
#  define MAS_DUF_OPTION_NAMES_H

#include "duf_option_types.h"

char *duf_cli_option_shorts( const duf_longval_extended_table_t ** xtables );

const char *duf_option_cnames_tmp( int index, duf_option_code_t code, const char *delim );

char *duf_option_names( duf_option_code_t codeval );
char *duf_option_names_d( duf_option_code_t codeval, const char *delim );


#  define DUF_OPT_NAME_D(n,d) duf_option_cnames_tmp( -1, DUF_OPTION_VAL_ ## n, d )
#  define DUF_OPT_NAME(n)  DUF_OPT_NAME_D(n,NULL)

#  define DUF_OPT_FLAG_NAME_D(n,d) duf_option_cnames_tmp( -1, DUF_OPTION_VAL_FLAG_ ## n,d )
#  define DUF_OPT_FLAG_NAME(n)  DUF_OPT_FLAG_NAME_D(n,NULL)

#  define DUF_OPT_NAME1(n) duf_option_cnames_tmp( 1, DUF_OPTION_VAL_ ## n )

#  define DUF_OPT_FLAG_NAME1_D(n,d) duf_option_cnames_tmp( 1, DUF_OPTION_FLAG_ ## n, d )
#  define DUF_OPT_FLAG_NAME1(n) DUF_OPT_FLAG_NAME1_D(n,NULL)

#  define DUF_OPT_NAME2_D(n,d) duf_option_cnames_tmp( 2, DUF_OPTION_VAL_ ## n,d )
#  define DUF_OPT_NAME2(n) DUF_OPT_NAME2_D(n,NULL)

#  define DUF_OPT_FLAG_NAME2_D(n,d) duf_option_cnames_tmp( 2, DUF_OPTION_VAL_FLAG_ ## n,d )
#  define DUF_OPT_FLAG_NAME2(n) DUF_OPT_FLAG_NAME2_D(n,NULL)

#  define DUF_OPTION_NAME_D(n,d)  duf_option_cnames_tmp(-1,n,d)
#  define DUF_OPTION_NAME(n)  DUF_OPTION_NAME_D(n,NULL)

#  define DUF_OPTION_LINDEX_NAME_D(n,d)  duf_option_cnames_tmp( -1, duf_longindex_extended_codeval( n ), d )
#  define DUF_OPTION_LINDEX_NAME(n)  DUF_OPTION_LINDEX_NAME_D(n,NULL)


#endif
