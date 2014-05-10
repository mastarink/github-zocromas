#ifndef MAS_DUF_TYPES_H
#  define MAS_DUF_TYPES_H
#  include <sys/stat.h>
#  include <stdarg.h>

#  include "duf_sql_def.h"


/* ###################################################################### */

#  define DUF_FL __func__,__LINE__
#  define DUF_UNUSED __attribute__(( unused ))
/* ###################################################################### */

#  define DUF_FALSE 0
#  define DUF_TRUE 1

/* ###################################################################### */

#  define DUF_RECURSIVE_NO DUF_FALSE
#  define DUF_RECURSIVE_YES DUF_TRUE

#  define DUF_CONSTRAINT_IGNORE_NO DUF_FALSE
#  define DUF_CONSTRAINT_IGNORE_YES DUF_TRUE

#  ifdef MAS_SPLIT_DB
#    define DUF_DBPREF "main."
#    define DUF_DBADMPREF "adm."
#  else
#    define DUF_DBPREF "main."
#    define DUF_DBADMPREF "main."
#  endif

/* ###################################################################### */

#  define SEL_CB_DEF ((duf_sel_cb_t)NULL)
#  define SEL_CB_UDATA_DEF (NULL)

#  define STR_CB_DEF ((duf_str_cb_t)NULL)
#  define STR_CB_UDATA_DEF (NULL)

#  define DUF_STAT_DEF ((struct stat *) NULL)

/* ###################################################################### */

#  define DUF_SET_SFIELD(name) name = __duf_sql_str_by_name( #name, precord, NULL, 0 )
#  define DUF_SET_UFIELD(name) name = __duf_sql_ull_by_name( #name, precord, NULL, 0 )

#  define DUF_SFIELD(name) const char*  DUF_SET_SFIELD(name)
#  define DUF_UFIELD(name) unsigned long long  DUF_SET_UFIELD(name)

#  define DUF_SFIELD_CHECK(name) int duf_have_field_##name; const char* name = __duf_sql_str_by_name( #name, precord, &duf_have_field_##name, 0 ); \
  	duf_check_field(#name, duf_have_field_##name)
#  define DUF_UFIELD_CHECK(name) int duf_have_field_##name; unsigned long long name = __duf_sql_ull_by_name( #name, precord, &duf_have_field_##name, 0 ); \
  	duf_check_field(#name, duf_have_field_##name)

#  define DUF_SFIELD_OPT(name) int duf_have_field_##name; const char* name = __duf_sql_str_by_name( #name, precord, &duf_have_field_##name, 1 )
#  define DUF_UFIELD_OPT(name) int duf_have_field_##name; unsigned long long name = __duf_sql_ull_by_name( #name, precord, &duf_have_field_##name, 1 )


#  define DUF_SET_SFIELD2(name) name = __duf_sql_str_by_name2( pstmt, #name )
#  define DUF_SET_UFIELD2(name) name = __duf_sql_ull_by_name2( pstmt, #name )

#  define DUF_SFIELD2(name) const char*  DUF_SET_SFIELD2(name)
#  define DUF_UFIELD2(name) unsigned long long  DUF_SET_UFIELD2(name)

/* ###################################################################### */

#  define DUF_PUTS( min, str) \
    duf_puts( duf_config ? duf_config->cli.output.level:0, min, \
		DUF_FL, duf_config && duf_config->cli.output.out?duf_config->cli.output.out:stdout, str)
#  define DUF_PUTSL( min) \
    duf_puts( duf_config ? duf_config->cli.output.level:0, min, \
		DUF_FL, duf_config && duf_config->cli.output.out?duf_config->cli.output.out:stdout, NULL)


#  define DUF_PRINTF( min, ...) \
    duf_printf( duf_config ? duf_config->cli.output.level:0, min, 0, \
		DUF_FL, duf_config && duf_config->cli.output.out?duf_config->cli.output.out:stdout, __VA_ARGS__ )

#  define DUF_DIE( min, ...) \
    duf_printf( duf_config ? duf_config->cli.output.level:0, min, 1, \
		DUF_FL, duf_config && duf_config->cli.output.out?duf_config->cli.output.out:stdout, __VA_ARGS__ )


/* ###################################################################### */
#  define DUF_IF_TR_WHAT_C(  cfg, what, name )		(( cfg && !cfg->cli.trace.nonew) ? cfg->what.name: 1 )
#  define DUF_IF_TR_WHAT( what, name )			DUF_IF_TR_WHAT_C( duf_config,  what,      name )
#  define DUF_IF_TR( name )				DUF_IF_TR_WHAT(		  cli.trace, name)

#  define DUF_IF_TR_WHATN_C( cfg, what, name, min )	( cfg && !cfg->cli.trace.nonew && cfg->what.name > min )
#  define DUF_IF_TR_WHATN( what, name, min )		DUF_IF_TR_WHATN_C( duf_config, what,      name, min )
#  define DUF_IF_TRN( name, min )			DUF_IF_TR_WHATN(		  cli.trace, name, min)

#  define DUF_TR_WHAT_C( cfg, what, name, min, ... )	if ( DUF_IF_TR_WHATN_C( cfg, what, name, min )) { __VA_ARGS__ ; }
#  define DUF_TR_WHAT( what, name, min, ... )		DUF_TR_WHAT_C( duf_config, what, name, min, __VA_ARGS__ )

#  define DUF_TR_WHATSYS(  what, name, min, ...)	DUF_TR_WHATSYSE( errno,  what, name, min, __VA_ARGS__ )

#  define DUF_TR_C( cfg,name, ... )			DUF_TR_WHAT_C( cfg, cli.trace, name, __VA_ARGS__ )
#  define DUF_TR(name, ...)				DUF_TR_C( duf_config, __VA_ARGS__ )

/* ###################################################################### */

/* #  define DUF_IF_TRACE_WHAT(what,name) (duf_config && !duf_config->cli.trace.nonew && duf_config->what.name ) */
#  define DUF_IF_TRACE_WHAT_C(  cfg, what, name )	(( cfg && !cfg->cli.trace.nonew) ? cfg->what.name: 1 )
#  define DUF_IF_TRACE_WHAT( what, name )		DUF_IF_TRACE_WHAT_C( duf_config,  what,      name )
#  define DUF_IF_TRACE( name )				DUF_IF_TRACE_WHAT(		  cli.trace, name)

#  define DUF_IF_TRACE_WHATN_C( cfg, what, name, min )  ( cfg && !cfg->cli.trace.nonew && cfg->what.name > min )
#  define DUF_IF_TRACE_WHATN( what, name, min )		DUF_IF_TRACE_WHATN_C( duf_config, what,      name, min )
#  define DUF_IF_TRACEN( name, min )			DUF_IF_TRACE_WHATN(		  cli.trace, name, min)

#  define DUF_TRACE_FILE_C( cfg ) ( cfg && cfg->cli.trace.out ? cfg->cli.trace.out : stdout )

#  define DUF_TRACE_WHAT_C( cfg, what, name, min, ...)	duf_trace( DUF_TRACE_MODE_ ## name, #name, \
		    	DUF_IF_TRACE_WHAT_C( cfg, what, name ), min, \
			DUF_FL, cfg?cfg->loadtime:0, 0, 0, DUF_TRACE_FILE_C( cfg ), __VA_ARGS__ )
#  define DUF_TRACE_WHAT( what, name, min, ...)		DUF_TRACE_WHAT_C( duf_config,	  what,	     name, min, __VA_ARGS__ )

#  define DUF_TRACE_C( cfg, name, ... )			DUF_TRACE_WHAT_C( cfg,		  cli.trace, name, __VA_ARGS__ )
#  define DUF_TRACE( name, ... )			DUF_TRACE_C(	  duf_config,		     name, __VA_ARGS__ )

/* ###################################################################### */

#  define DUF_TRACE_WHATSYSE_C( cfg, ern, what, name, min, ... ) \
		duf_trace( DUF_TRACE_MODE_ ## name, #name, \
			DUF_IF_TRACE_WHAT_C( cfg, what, name ), min, \
			DUF_FL, cfg?cfg->loadtime:0, DUF_TRACE_FLAG_SYSTEM, ern, DUF_TRACE_FILE_C( cfg ), __VA_ARGS__ )

#  define DUF_TRACE_WHATSYSE( ern, what, name, min, ... ) \
							DUF_TRACE_WHATSYSE_C( duf_config, ern, what, name, min, __VA_ARGS__ )

#  define DUF_TRACE_WHATSYS( what, name, min, ... )	DUF_TRACE_WHATSYSE( errno,  what, name, min, __VA_ARGS__ )


#  define DUF_TRACESYS(name, ...)			DUF_TRACE_WHATSYS( cli.trace, name, __VA_ARGS__ )
#  define DUF_TRACESYSE(ern, name, ...)			DUF_TRACE_WHATSYSE( ern, cli.trace, name, __VA_ARGS__ )

/* ###################################################################### */

#  define DUF_IF_DEBUG( lev )				DUF_IF_TRACE_WHAT( cli.dbg, debug )
#  define DUF_IF_DEBUGN( lev )				DUF_IF_TRACE_WHAT( cli.dbg, debug, lev )
#  define DUF_DEBUG(lev, ...)				if ( DUF_IF_DEBUG( lev ) ) {  __VA_ARGS__ ; }

/* ###################################################################### */

#  define DUF_IF_VERBOSE()				DUF_IF_TRACE_WHAT( cli.dbg, verbose )
#  define DUF_IF_VERBOSEN( lev )			DUF_IF_TRACE_WHATN( cli.dbg, verbose, lev )
#  define DUF_VERBOSE( lev, ... )			DUF_TRACE_WHAT( cli.dbg, verbose, lev, __VA_ARGS__ )

/* ###################################################################### */

#  define DUF_ERROR( ... )				DUF_TRACE( error, 0, __VA_ARGS__ )
#  define DUF_ERRORR( r, ... )				DUF_TRACE( errorr, r, __VA_ARGS__ )
#  define DUF_ERRORiV( v )				DUF_ERROR( #v ":%d" , v )
#  define DUF_ERRSYS( ... )				DUF_TRACESYS( error, 0, __VA_ARGS__ )
#  define DUF_ERRSYSE( ern, ... )			DUF_TRACESYSE( ern, error, 0, __VA_ARGS__ )

/* ###################################################################### */

#  define DUF_TEST_RX(val)	if (val) DUF_ERROR( " - - - - - -> [%s] (#%d)", val<0?duf_error_name(val):"-", val )

#  define DUF_TEST_R(val)	if ( val!=DUF_ERROR_MAX_REACHED ) DUF_TEST_RX( val )
#  define DUF_TEST_RR(val)	if ( val!=DUF_SQL_ROW && val!=DUF_SQL_DONE ) DUF_TEST_R( val )
#  define DUF_TEST_R3(val)	if (val \
    			&& (val)!=SQLITE_ROW \
    			&& (val)!=SQLITE_DONE \
    					)		\
					DUF_ERROR( " - - - - - -> sqlite3 [%s] (#%d)", \
					    DUF_SQLITE_ERROR_CODE(val) < 0 ? duf_error_name(DUF_SQLITE_ERROR_CODE(val)) : "-", \
					    DUF_SQLITE_ERROR_CODE(val) \
					    )

/* ###################################################################### */

#  define DUF_FUNN(af) duf_dbg_funname( ( duf_anyhook_t ) af )

/* ###################################################################### */

#  define DUF_OINV(pref) assert( duf_config->cli.noopenat || !pref opendir || ( \
    		          ( ( (int) duf_config->nopen - (int) duf_config->nclose ) ) \
	    		- (pref  depth)  == 1 ) \
    		)
 /* - (pref levinfo?pref levinfo[pref depth].is_leaf:0) */
#  define DUF_OINVC(pref) assert( duf_config->cli.noopenat || !pref opendir || ( \
    		          ( ( (int) duf_config->nopen - (int) duf_config->nclose ) ) \
	    		- (pref  depth)  == 0 ) \
    		)

#  define DUF_OINV_OPENED(pref)     assert( duf_config->cli.noopenat || !pref opendir || (pref levinfo && pref levinfo[pref depth].lev_dh.dfd ))
#  define DUF_OINV_NOT_OPENED(pref) assert( duf_config->cli.noopenat || !pref opendir || (!pref levinfo || pref levinfo[pref depth].lev_dh.dfd==0 ))

/* ###################################################################### */

#  define DUF_ACTION_TITLE_FMT "-17s"
#  define DUF_ACTION_TITLE_PFMT "%" DUF_ACTION_TITLE_FMT
#  define DUF_DEPTH_FMT "-2d"
#  define DUF_DEPTH_PFMT "L%" DUF_DEPTH_FMT

/* ###################################################################### */

#  include "duf_opt_types.h"
#  include "duf_cli_types.h"

typedef enum
{
  DBG_START = 1000,
  DBG_STEP,
  DBG_STEPS,
  DBG_STEPIS,
  DBG_STEPI2S,
  DBG_STEPULL,
  DBG_ENDR,
  DBG_ENDULL,
  DBG_ENDS,
  DBG_ENDRS,
  DBG_END,
} duf_dbgcode_t;

/* #  define DUF_SQLITE_ERROR_CODE(r3c) ( int rt=(r3c);rt == SQLITE_OK ? 0 : ( rt > 0 ? DUF_SQLITE_ERROR_BASE + rt : rt ) ) */
#  define DUF_SQLITE_ERROR_CODE(r3c) duf_sqlite_error_code(r3c)
#  define  DEBUG_START() duf_dbgfunc( DBG_START, DUF_FL )
#  define  DEBUG_END() duf_dbgfunc( DBG_ENDR, DUF_FL )
#  define  DEBUG_ENDR(r)  DUF_TEST_R( r ); duf_dbgfunc( DBG_ENDR, DUF_FL, r )
#  define  DEBUG_ENDR3(r)  DUF_TEST_R( DUF_SQLITE_ERROR_CODE(r) ); duf_dbgfunc( DBG_ENDR, DUF_FL, r )
#  define  DEBUG_ENDULL(l) duf_dbgfunc( DBG_ENDULL, DUF_FL, l )
#  define  DEBUG_ENDS(l) duf_dbgfunc( DBG_ENDS, DUF_FL, l )
#  define  DEBUG_STEP() duf_dbgfunc( DBG_STEP, DUF_FL )
#  define  DEBUG_STEPS(l) duf_dbgfunc( DBG_STEPS, DUF_FL, l )
#  define  DEBUG_STEPIS(l, s) duf_dbgfunc( DBG_STEPIS, DUF_FL, l, s )
#  define  DEBUG_STEPULL(l) duf_dbgfunc( DBG_STEPULL, DUF_FL, l )

typedef enum
{
  DUF_OK,
  DUF_ERROR_ERROR_BASE = -30000,
  DUF_ERROR_UNKNOWN,
  DUF_ERROR_UNKNOWN_NODE,
  DUF_ERROR_MAIN,
  DUF_ERROR_NO_ACTIONS,
  DUF_ERROR_PTR,
  DUF_ERROR_DATA,
  DUF_ERROR_MD5,
  DUF_ERROR_NOT_OPEN,
  DUF_ERROR_PATH,
  DUF_ERROR_OPENAT,
  DUF_ERROR_OPENAT_ENOENT,
  DUF_ERROR_OPEN,
  DUF_ERROR_OPEN_ENOENT,
  DUF_ERROR_READ,
  DUF_ERROR_CLOSE,
  DUF_ERROR_UNLINK,
  DUF_ERROR_OPTION,
  DUF_ERROR_OPTION_VALUE,
  DUF_ERROR_SUBOPTION,
  DUF_ERROR_SCANDIR,
  DUF_ERROR_CHECK_TABLES,
  DUF_ERROR_CLEAR_TABLES,
  DUF_ERROR_NO_FILE_SELECTOR,
  DUF_ERROR_DB_NO_PATH,
  DUF_ERROR_NO_STR_CB,
  DUF_ERROR_BIND_NAME,
  DUF_ERROR_MAX_DEPTH,
  DUF_ERROR_MAX_REACHED,
  DUF_ERROR_GET_FIELD,
  DUF_ERROR_NOT_IN_DB,
  DUF_ERROR_NO_FIELD,
  DUF_ERROR_NO_FIELD_OPTIONAL,
  DUF_ERROR_INSERT_MDPATH,
  DUF_ERROR_STAT,
  DUF_ERROR_STATAT,
  DUF_ERROR_PDI_SQL,
  DUF_ERROR_SQL_NO_FIELDSET,
  DUF_ERROR_SQL_NO_TABLE,
  DUF_ERROR_MEMORY,
  DUF_ERROR_ERROR_MAX,
} duf_error_code_t;

typedef struct
{
  unsigned long long files;
  unsigned long long dirs;
  unsigned long long total;
} duf_items_t;

typedef struct
{
  int argc;
  char **argv;
} duf_argvc_t;
typedef struct glob
{
  duf_argvc_t include_files;
  duf_argvc_t exclude_files;
} duf_filter_glob_t;
typedef struct
{
  unsigned recursive:1;
  unsigned noself_dir:1;
  unsigned noupper_dirs:1;
  unsigned maxreldepth;
  unsigned long long maxseq;
  duf_items_t maxitems;
  unsigned long long mindirfiles;
  unsigned long long maxdirfiles;
  duf_filter_glob_t glob;
  unsigned long long md5id;
  unsigned long long mimeid;
  unsigned long long minsize;
  unsigned long long maxsize;
  unsigned long long minsame;
  unsigned long long maxsame;
  unsigned long long filter_id;
} duf_ufilter_t;
typedef struct
{
  char *name;
  char *fpath;
} duf_db_config_t;
typedef struct
{
  char *dir;
  duf_db_config_t main;
  duf_db_config_t adm;
} duf_dbs_config_t;
typedef struct
{
  double loadtime;
  int actions_done;
  duf_ufilter_t u;
  duf_config_cli_t cli;
  duf_dbs_config_t db;
  /* char *group; */
  int targc;
  char **targv;


  unsigned nopen;
  unsigned nclose;

} duf_config_t;
typedef enum
{
  DUF_NODE_LEAF = 100,
  DUF_NODE_NODE,
} duf_node_type_t;

typedef struct stat duf_stat_t;
typedef struct duf_dirhandle_s
{
  unsigned long long dirid;

  int dfd;
  int rs;
  struct stat st;
} duf_dirhandle_t;

typedef void ( *duf_void_voidp_t ) ( void * );
typedef struct
{
  void *ptr;
  duf_void_voidp_t destructor;
} duf_context_t;
typedef struct
{
  unsigned is_leaf:1;
  unsigned deleted:1;
  unsigned eod;
  unsigned long long dirid;
  /* const char *name; */
  duf_items_t items;
  long numdir;
  long numfile;
  char *fullpath;
  char *itemname;
  duf_context_t context;
  duf_dirhandle_t lev_dh;
} duf_levinfo_t;

typedef struct
{
  unsigned opendir:1;
  unsigned maxdepth;
  int depth;                    /* signed !! */
  int topdepth;                 /* signed !! */
  /* duf_node_type_t node_type; */
  /* char *path; */
  duf_levinfo_t *levinfo;
  unsigned long long changes;
  unsigned long long seq;
  unsigned long long seq_leaf;
  unsigned long long seq_node;
  duf_items_t items;
  duf_ufilter_t u;
  duf_context_t context;
  int num_statements;
  duf_sqlite_stmt_t **statements;
  int **xstatements;
} duf_depthinfo_t;

typedef struct
{
  const char *name;
  struct stat st;
  unsigned long long truedirid;
  unsigned long long nsame;
  unsigned long long md5id;
  unsigned long long mimeid;
  unsigned long long dataid;
  unsigned long long md5sum1;
  unsigned long long md5sum2;
} duf_fileinfo_t;


typedef struct
{
  int depth;                    /* signed !! */
  const struct
  {
    int *pseq;
    int dofiles;
  } c;
  const duf_ufilter_t u;
} duf_filter_t;

typedef struct
{
  unsigned long long md5id;
  unsigned long long mimeid;
  unsigned long long size;
  unsigned long long nduplicates;
} md5_std_data_t;

typedef struct
{
#  ifdef DUF_RECORD_WITH_NROWS
  int nrow;
  int nrows;
#  endif
  int ncolumns;
  const char *const *presult;
  const char *const *pnames;
}
duf_record_t;

struct duf_scan_callbacks_s;
typedef int ( *duf_scan_hook_init_t ) ( void );


/* this is callback of type: duf_scan_hook_dir_t : */
typedef int ( *duf_scan_hook_dir_t ) ( unsigned long long pathid, duf_depthinfo_t * pdi, duf_record_t * precord );
typedef int ( *duf_scan_hook2_dir_t ) ( duf_sqlite_stmt_t * pstmt, unsigned long long pathid, duf_depthinfo_t * pdi );

/* this is callback of type: duf_scan_hook_file_t : */
typedef int ( *duf_scan_hook_file_t ) ( duf_depthinfo_t * pdi, duf_record_t * precord );
typedef int ( *duf_scan_hook2_file_t ) ( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi );

typedef int ( *duf_scan_hook_file_fd_t ) ( int fd, const struct stat * pst_file, duf_depthinfo_t * pdi, duf_record_t * precord );
typedef int ( *duf_scan_hook2_file_fd_t ) ( duf_sqlite_stmt_t * pstmt, int fd, const struct stat * pst_file, duf_depthinfo_t * pdi );



typedef int ( *duf_scan_hook_dirent_reg_t ) ( const char *fname, const struct stat * pstat, unsigned long long dirid, duf_depthinfo_t * pdi,
                                              duf_record_t * precord );
typedef int ( *duf_scan_hook2_dirent_reg_t ) ( duf_sqlite_stmt_t * pstmt, const char *fname, const struct stat * pstat,
                                               unsigned long long dirid, duf_depthinfo_t * pdi );


typedef int ( *duf_scan_hook_dirent_dir_t ) ( const char *fname, const struct stat * pstat, unsigned long long dirid,
                                              duf_depthinfo_t * pdi, duf_record_t * precord );
typedef int ( *duf_scan_hook2_dirent_dir_t ) ( duf_sqlite_stmt_t * pstmt, const char *fname, const struct stat * pstat,
                                               unsigned long long dirid, duf_depthinfo_t * pdi );


typedef int ( *duf_scan_hook_dirent_parent_t ) ( const struct stat * pstat, unsigned long long dirid, duf_depthinfo_t * pdi,
                                                 duf_record_t * precord );
typedef int ( *duf_scan_hook2_dirent_parent_t ) ( duf_sqlite_stmt_t * pstmt, const struct stat * pstat, unsigned long long dirid,
                                                  duf_depthinfo_t * pdi );


typedef int ( *duf_sqexe_cb_t ) ( void *sqexe_data, int ncolumns, char **presult, char **pnames );

typedef int ( *duf_anyhook_t ) ( void );



/* this is callback of type: duf_str_cb_t (first range; str_cb) */
typedef int ( *duf_str_cb_t ) ( void *str_cb_udata, duf_depthinfo_t * pdi, struct duf_scan_callbacks_s * sccb, duf_record_t * precord );
typedef int ( *duf_str_cb2_t ) ( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi, struct duf_scan_callbacks_s * sccb );


/* this is callback of type:duf_sel_cb_t (second range; ; sel_cb) */
typedef int ( *duf_sel_cb_t ) ( duf_record_t * precord, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata, duf_depthinfo_t * pdi,
                                struct duf_scan_callbacks_s * sccb );
typedef int ( *duf_sel_cb_match_t ) ( duf_record_t * precord );

/* KNOWN duf_sel_cb_t callbacks:
 * duf_sel_cb_field_by_sccb	: str_cb_unused	, str_cb_udata_unused, pdi_unused
 * duf_sel_cb_levinfo		: str_cb_unused	, str_cb_udata_unused, xpdi_unused,	sccb_unused
 * duf_sel_cb_name_parid	: str_cb_unused	, str_cb_udata_unused, 			sccb_unused
 * duf_sel_cb_leaf		:		, sel_cb_udata_unused
 * duf_sel_cb_node		:		, sel_cb_udata_unused
*/
typedef int ( *duf_sel_cb2_t ) ( duf_sqlite_stmt_t * pstmt, duf_str_cb2_t str_cb, duf_depthinfo_t * pdi,
                                 struct duf_scan_callbacks_s * sccb );

typedef int ( *duf_sel_cb2_match_t ) ( duf_sqlite_stmt_t * pstmt );

typedef int ( *duf_pdi_cb_t ) ( duf_depthinfo_t * pdi );


struct duf_scan_callbacks_s
{
  unsigned opendir:1;
  unsigned scan_mode_2:1;
  const char *title;

  const char *node_fieldset;
  const char *node_selector;
  const char *node_selector2;

  const char *leaf_fieldset;
  const char *leaf_selector;
  const char *leaf_selector2;

  duf_scan_hook_init_t init_scan;

  duf_scan_hook_dir_t node_scan_before;
  duf_scan_hook2_dir_t node_scan_before2;
  duf_scan_hook2_dir_t node_scan_before2_deleted;

  duf_scan_hook_dir_t node_scan_middle;
  duf_scan_hook2_dir_t node_scan_middle2;
  duf_scan_hook2_dir_t node_scan_middle2_deleted;

  duf_scan_hook_dir_t node_scan_after;
  duf_scan_hook2_dir_t node_scan_after2;
  duf_scan_hook2_dir_t node_scan_after2_deleted;

  duf_scan_hook_file_t leaf_scan;
  duf_scan_hook2_file_t leaf_scan2;
  duf_scan_hook2_file_t leaf_scan2_deleted;

  duf_scan_hook_file_fd_t leaf_scan_fd;
  duf_scan_hook2_file_fd_t leaf_scan_fd2;

  duf_scan_hook_dirent_reg_t dirent_file_scan_before;
  duf_scan_hook2_dirent_reg_t dirent_file_scan_before2;

  duf_scan_hook_dirent_dir_t dirent_dir_scan_before;
  duf_scan_hook2_dirent_dir_t dirent_dir_scan_before2;

  const char **final_sql_argv;
};

typedef struct duf_scan_callbacks_s duf_scan_callbacks_t;



#endif
