#ifndef MAS_DUF_OPTION_DEFS_H
#  define MAS_DUF_OPTION_DEFS_H

#  define DUF_CONFIG_FILE_NAME "zocromas_muc"


# define DUF_OPTION_NAME_D(_cli,_n,_d)  duf_coption_cnames_tmp(_cli,-1,_n,_d)
# define DUF_OPTION_NAME(_cli,_n)  DUF_OPTION_NAME_D(_cli,_n,NULL)

/* # define ..._OPTION_LINDEX_NAME_D(_cli,_n,_d)  ..._coption_cnames_tmp(_cli, -1, ..._longindex_extended_codeval( _n ), _d ) */
/* # define ..._OPTION_LINDEX_NAME(_cli,_n)  ..._OPTION_LINDEX_NAME_D(_cli,_n,NULL) */



#endif

/*
vi: ft=c
*/
