#ifndef MAS_DUF_CONFIG_UTIL_H
# define MAS_DUF_CONFIG_UTIL_H

# include <mastar/multiconfig/muc_config_cli_types.h>                /* muc_config_cli_t */
# include "duf_config_opt_types.h"                                   /* duf_config_opt_t */
# include "duf_ufilter_types.h"                                      /* duf_ufilter_t; duf_yfilter_t; etc. ✗ */
# include "duf_se.h"                                                 /* DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

/* void *duf_get_config_offset( unsigned long off ); */
/* void *duf_get_config_puz_offset( unsigned long off ); */

muc_config_cli_t *duf_get_config_cli( void );
duf_config_opt_t *duf_get_config_opt( void );
duf_ufilter_t *duf_get_config_ufilter( void );

#define DUF_DECL_FLAG_FUNC(     _group, _rg, _set, _rs, _v, _name, _uname)   int            duf_get_config_flag_    ## _set ## _ ## _name( void )
#define DUF_DECL_FLAGNAME_FUNC( _group, _rg, _set, _rs, _v, _name, _uname)   const char    *duf_get_config_flagname_## _set ## _ ## _name( int nn )
#define DUF_DECL_NUM_FUNC(      _group, _rg, _set, _rs,     _name, _uname)   unsigned long  duf_get_config_num_     ## _set ## _ ## _name( void )

DUF_DECL_FLAG_FUNC( opt,., act,., v, interactive, INTERACTIVE );
DUF_DECL_FLAG_FUNC( opt,., act,., v, allow_files, ALLOW_FILES );
DUF_DECL_FLAG_FUNC( opt,., act,., v, allow_dirs, ALLOW_DIRS );
DUF_DECL_FLAG_FUNC( opt,., act,., v, allow_drop_tables, ALLOW_DROP_TABLES );
DUF_DECL_FLAGNAME_FUNC( opt,., act,., v, allow_drop_tables, ALLOW_DROP_TABLES );
DUF_DECL_FLAG_FUNC( opt,., act,., v, allow_clean_tables, ALLOW_CLEAN_TABLES );
DUF_DECL_FLAGNAME_FUNC( opt,., act,., v, allow_clean_tables, ALLOW_CLEAN_TABLES );
DUF_DECL_FLAG_FUNC( opt,., act,., v, allow_create_tables, ALLOW_CREATE_TABLES );
DUF_DECL_FLAGNAME_FUNC( opt,., act,., v, allow_create_tables, ALLOW_CREATE_TABLES );
DUF_DECL_FLAG_FUNC( opt,., act,., v, allow_create_database, ALLOW_CREATE_DATABASE );
DUF_DECL_FLAG_FUNC( opt,., act,., v, allow_remove_database, ALLOW_REMOVE_DATABASE );
DUF_DECL_FLAGNAME_FUNC( opt,., act,., v, allow_remove_database, ALLOW_REMOVE_DATABASE );
DUF_DECL_FLAG_FUNC( opt,., act,., v, allow_vacuum, ALLOW_VACUUM );
DUF_DECL_FLAGNAME_FUNC( opt,., act,., v, allow_vacuum, ALLOW_VACUUM );
DUF_DECL_FLAG_FUNC( opt,., act,., v, info, INFO );
DUF_DECL_FLAG_FUNC( opt,., act,., v, summary, SUMMARY );
DUF_DECL_FLAG_FUNC( opt,., act,., v, testflag, TESTFLAG );
DUF_DECL_FLAG_FUNC( opt,., act,., v, testiflag, TESTIFLAG );
DUF_DECL_FLAG_FUNC( opt,., act,., v, testnoflag, TESTNOFLAG );
DUF_DECL_FLAG_FUNC( opt,., disable,,, memusage, DISABLE_MEMUSAGE );
DUF_DECL_FLAGNAME_FUNC( opt,., disable,,, memusage, DISABLE_MEMUSAGE );
DUF_DECL_FLAG_FUNC( opt,., disable,,, testflag, TESTFLAG );
DUF_DECL_FLAG_FUNC( opt,., disable,,, testiflag, TESTIFLAG );
DUF_DECL_FLAG_FUNC( opt,., disable,,, testnoflag, TESTNOFLAG );
DUF_DECL_FLAG_FUNC( opt,., flow,., v, dry_run, DRY_RUN );
DUF_DECL_FLAGNAME_FUNC( opt,., flow,., v, dry_run, DRY_RUN );
DUF_DECL_NUM_FUNC( opt,., flow,., verbose, VERBOSE );
DUF_DECL_FLAG_FUNC( vars,., puz,->, v, testflag, TESTFLAG );
DUF_DECL_FLAG_FUNC( vars,., puz,->, v, testiflag, TESTIFLAG );
DUF_DECL_FLAG_FUNC( vars,., puz,->, v, testnoflag, TESTNOFLAG );
DUF_DECL_NUM_FUNC( ,,,, testnum, TESTNUM );

# ifdef MAS_TRACING
mas_config_trace_t *duf_get_trace_config( void );
# endif
/* char duf_option_delimiter( void ); */

char *duf_config_db_path_add_subdir( const char *path, int *pr );

const char *duf_string_options_at_string_xsdb_getvar( const char *name, const char *arg );

#endif
