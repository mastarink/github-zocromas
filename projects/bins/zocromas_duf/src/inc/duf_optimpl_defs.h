#ifndef MAS_DUF_OPTIMPL_DEFS_H
#  define MAS_DUF_OPTIMPL_DEFS_H



# define DUF_OPT_NAME_D(_cli, _n,_d) duf_coption_cnames_tmp( _cli, -1, DUF_OPTION_VAL_ ## _n, _d )
# define DUF_OPT_NAME(_cli,_n)  DUF_OPT_NAME_D(_cli,_n,NULL)

# define DUF_OPT_FLAG_NAME_D(_cli, _n, _d) duf_coption_cnames_tmp( _cli, -1, DUF_OPTION_VAL_FLAG_ ## _n,_d )
# define DUF_OPT_FLAG_NAME(_cli,_n)  DUF_OPT_FLAG_NAME_D(_cli, _n,NULL)

# define DUF_OPT_NAME1(_cli,_n) duf_coption_cnames_tmp( _cli,1, DUF_OPTION_VAL_ ## _n )

/* #  define DUF_OPT_FLAG_NAME1_D(_n,_d) duf_coption_cnames_tmp(_cli, 1, DUF_OPTION_FLAG_ ## _n, _d ) */
/* #  define DUF_OPT_FLAG_NAME1(_n) DUF_OPT_FLAG_NAME1_D(_n,NULL) */

/* #  define DUF_OPT_NAME2_D(_n,_d) duf_coption_cnames_tmp(_cli, 2, DUF_OPTION_VAL_ ## _n,_d ) */
/* #  define DUF_OPT_NAME2(_n) DUF_OPT_NAME2_D(_n,NULL) */

# define DUF_OPT_FLAG_NAME2_D(_cli,_n,_d) duf_coption_cnames_tmp(_cli, 2, DUF_OPTION_VAL_FLAG_ ## _n,_d )
# define DUF_OPT_FLAG_NAME2(_cli,_n) DUF_OPT_FLAG_NAME2_D(_cli,_n,NULL)



#endif

/*
vi: ft=c
*/

