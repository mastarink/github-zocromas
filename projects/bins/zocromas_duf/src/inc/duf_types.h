#ifndef MAS_DUF_TYPES_H
#  define MAS_DUF_TYPES_H
#  include <sys/stat.h>
#  include <stdarg.h>

#  define DUF_FALSE 0
#  define DUF_TRUE 1

#  define DUF_RECURSIVE_NO DUF_FALSE
#  define DUF_RECURSIVE_YES DUF_TRUE

#  define DUF_CONSTRAINT_IGNORE_NO DUF_FALSE
#  define DUF_CONSTRAINT_IGNORE_YES DUF_TRUE

#  define SEL_CB_DEF ((duf_sql_select_cb_t)NULL)
#  define SEL_CB_UDATA_DEF (NULL)

#  define STR_CB_DEF ((duf_scan_callback_file_t)NULL)
#  define STR_CB_UDATA_DEF (NULL)

#  define DUF_STAT_DEF ((struct stat *) NULL)

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


#  define DUF_IF_WHAT(what,name) (duf_config && !duf_config->cli.trace.nonew && duf_config->what.name )
#  define DUF_IF_TRACE(name)           DUF_IF_WHAT(cli.trace, name)
#  define DUF_IF_WHATN(what,name,min) (duf_config && !duf_config->cli.trace.nonew && duf_config->what.name>min )
#  define DUF_IF_TRACEN(name,min)      DUF_IF_WHATN(cli.trace, name)

#  define DUF_WHAT( what, name, min, ...) \
    duf_trace( #name, ((duf_config && !duf_config->cli.trace.nonew) ? duf_config->what.name: 1), min, \
		__func__,__LINE__, \
			duf_config && duf_config->cli.trace.out?duf_config->cli.trace.out:stdout, __VA_ARGS__ )
#  define DUF_TRACE(name, ...)           DUF_WHAT(cli.trace, name, __VA_ARGS__)

#  define DUF_TRACE_SCAN( min, ...)    DUF_TRACE( scan, min, __VA_ARGS__)
#  define DUF_TRACE_SAMPLE( min, ...)  DUF_TRACE( sample, min, __VA_ARGS__)
#  define DUF_ERROR(...)               DUF_TRACE( error, 0, __VA_ARGS__ )
#  define DUF_TEST_R(val)              if (val<0) DUF_ERROR( "rv=%d", val )

#  define DUF_VERBOSE(lev,...)           DUF_WHAT(cli.dbg,verbose,lev,__VA_ARGS__)
#  define DUF_IF_VERBOSE()               DUF_IF_WHAT(cli.dbg,verbose)
#  define DUF_IF_VERBOSEN(lev)           DUF_IF_WHATN(cli.dbg,verbose,lev)

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

typedef enum
{
  DUF_ERROR_ERROR_BASE = -30000,
  DUF_ERROR_UNKNOWN,
  DUF_ERROR_MAIN,
  DUF_ERROR_PTR,
  DUF_ERROR_DATA,
  DUF_ERROR_NOT_OPEN,
  DUF_ERROR_OPENAT,
  DUF_ERROR_OPEN,
  DUF_ERROR_CLOSE,
  DUF_ERROR_OPTION,
  DUF_ERROR_SCANDIR,
  DUF_ERROR_CHECK_TABLES,
  DUF_ERROR_CLEAR_TABLES,
  DUF_ERROR_NO_DIRID,
  DUF_ERROR_NO_STR_CB,
  DUF_ERROR_MAX_REACHED,
  DUF_ERROR_GET_FIELD,
  DUF_ERROR_NO_FIELD,
  DUF_ERROR_NO_FIELD_OPTIONAL,
  DUF_ERROR_INSERT_MDPATH,
  DUF_ERROR_STAT,
} duf_error_code_t;

typedef struct
{
  unsigned long long files;
  unsigned long long dirs;
  unsigned long long total;
} duf_items_t;

typedef struct
{
  unsigned recursive:1;
  unsigned noself_dir:1;
  unsigned noupper_dirs:1;
  unsigned maxdepth;
  unsigned long long maxseq;
  duf_items_t maxitems;
  unsigned long long mindirfiles;
  unsigned long long maxdirfiles;
  unsigned long long minsize;
  unsigned long long maxsize;
} duf_ufilter_t;
typedef struct
{
  char *dir;
  char *name;
  char *fpath;
} duf_db_config_t;
typedef struct
{
  duf_ufilter_t u;
  duf_config_cli_t cli;
  duf_db_config_t db;
  char *group;
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


typedef struct
{
  unsigned eod:1;
  unsigned long long dirid;
  /* const char *name; */
  unsigned long long items;
  unsigned long long ndirs;
  unsigned long long nfiles;
  void *context;
  duf_dirhandle_t dh;
} duf_levinfo_t;

typedef struct
{
  int depth;
  duf_node_type_t node_type;
  duf_levinfo_t *levinfo;
  unsigned long long seq;
  unsigned long long seq_leaf;
  unsigned long long seq_node;
  duf_items_t items;
  duf_ufilter_t u;
} duf_depthinfo_t;

typedef struct
{
  const char *name;
  struct stat st;
  unsigned long long md5sum1;
  unsigned long long md5sum2;
} duf_fileinfo_t;


typedef struct
{
  int depth;
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
  unsigned long long size;
  unsigned long long nduplicates;
} md5_std_data_t;

typedef struct
{
  int nrow;
  int nrows;
  int ncolumns;
  const char *const *pnames;
  const char *const *presult;
}
duf_record_t;

struct duf_scan_callbacks_s;
typedef int ( *duf_scan_hook_init_t ) ( struct duf_scan_callbacks_s * cb );


/* this is callback of type: duf_scan_hook_dir_t (second range; ; sel_cb): */
typedef int ( *duf_scan_hook_dir_t ) ( unsigned long long pathid, const duf_dirhandle_t * pdh, duf_depthinfo_t * pdi,
                                       duf_record_t * precord );

/* this is callback of type: duf_scan_hook_file_t (first range; str_cb) */
typedef int ( *duf_scan_hook_file_t ) ( void *str_cb_udata, duf_depthinfo_t * pdi, duf_record_t * precord );


typedef int ( *duf_anyhook_t ) ( void );



/* this is callback of type: duf_scan_callback_file_t (first range; str_cb) */
typedef int ( *duf_scan_callback_file_t ) ( void *str_cb_udata, duf_depthinfo_t * pdi,
                                            struct duf_scan_callbacks_s * sccb, duf_record_t * precord, const duf_dirhandle_t * pdhu );



typedef int ( *duf_sql_select_cb_t ) ( duf_record_t * precord, va_list args,
                                       void *sel_cb_udata, duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_depthinfo_t * pdi,
                                       struct duf_scan_callbacks_s * sccb, const duf_dirhandle_t * pdhu );
struct duf_scan_callbacks_s
{
  const char *title;
  const char *fieldset;
  const char *dir_selector;
  const char *file_selector;
  duf_scan_hook_init_t init_scan;
  duf_scan_hook_dir_t directory_scan_before;
  duf_scan_hook_dir_t directory_scan_middle;
  duf_scan_hook_dir_t directory_scan_after;
  duf_scan_hook_file_t file_scan;
};

typedef struct duf_scan_callbacks_s duf_scan_callbacks_t;



#endif
