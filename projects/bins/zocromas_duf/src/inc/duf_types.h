#ifndef MAS_DUF_TYPES_H
#  define MAS_DUF_TYPES_H
#  include <sys/stat.h>
#  include <stdarg.h>

#  define DUF_FALSE 0
#  define DUF_TRUE 1

/* #  define DUF_TRACE_NO DUF_FALSE */
/* #  define DUF_TRACE_YES DUF_TRUE */

#  define DUF_RECURSIVE_NO DUF_FALSE
#  define DUF_RECURSIVE_YES DUF_TRUE

#  define DUF_CONSTRAINT_IGNORE_NO DUF_FALSE
#  define DUF_CONSTRAINT_IGNORE_YES DUF_TRUE

#  define SEL_CB_DEF ((duf_sql_select_cb_t)NULL)
#  define SEL_CB_UDATA_DEF (NULL)

#  define STR_CB_DEF ((duf_scan_callback_file_t)NULL)
#  define STR_CB_UDATA_DEF (NULL)

#  define DUF_STAT_DEF ((struct stat *) NULL)

#  define DUF_SFIELD(name) int duf_have_field_##name; const char* name = __duf_sql_str_by_name( #name, precord, &duf_have_field_##name, 0 )
#  define DUF_UFIELD(name) int duf_have_field_##name; unsigned long long name = __duf_sql_ull_by_name( #name, precord, &duf_have_field_##name, 0 )

#  define DUF_SFIELD_OPT(name) int duf_have_field_##name; const char* name = __duf_sql_str_by_name( #name, precord, &duf_have_field_##name, 1 )
#  define DUF_UFIELD_OPT(name) int duf_have_field_##name; unsigned long long name = __duf_sql_ull_by_name( #name, precord, &duf_have_field_##name, 1 )

#  define DUF_IF_TRACE(name) (duf_config && duf_config->cli.trace.new && duf_config->cli.trace.name )
#  define DUF_IF_TRACEN(name,min) (duf_config && duf_config->cli.trace.new && duf_config->cli.trace.name>min )
#  define DUF_TRACE( name, min, ...) \
    duf_trace( #name, ((duf_config && duf_config->cli.trace.new) ? duf_config->cli.trace.name: 0), min, \
		__func__,__LINE__, \
			duf_config->cli.trace.out?duf_config->cli.trace.out:stdout, __VA_ARGS__ )
#  define DUF_TRACE_SCAN( min, ...) DUF_TRACE( scan, min, __VA_ARGS__)
#  define DUF_TRACE_SAMPLE( min, ...) DUF_TRACE( sample, min, __VA_ARGS__)

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
  duf_ufilter_t u;
  duf_config_cli_t cli;
  char *db_dir;
  char *db_name;
  char *group;
  int targc;
  char **targv;
} duf_config_t;
typedef enum
{
  DUF_NODE_LEAF = 100,
  DUF_NODE_NODE,
} duf_node_type_t;

typedef struct
{
  unsigned eod:1;
  unsigned long long dirid;
  /* const char *name; */
  unsigned long long items;
  unsigned long long ndirs;
  unsigned long long nfiles;
  void *context;
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
} duf_dirinfo_t;

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
  unsigned long long dupcnt;
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
typedef int ( *duf_scan_hook_dir_t ) ( unsigned long long pathid, unsigned long long items, duf_dirinfo_t * pdi, duf_record_t * precord );

/* this is callback of type: duf_scan_hook_file_t (first range; str_cb) */
typedef int ( *duf_scan_hook_file_t ) ( void *str_cb_udata, duf_dirinfo_t * pdi, duf_record_t * precord );





/* this is callback of type: duf_scan_callback_file_t (first range; str_cb) */
typedef int ( *duf_scan_callback_file_t ) ( void *str_cb_udata, duf_dirinfo_t * pdi,
                                            struct duf_scan_callbacks_s * sccb, duf_record_t * precord );



typedef int ( *duf_sql_select_cb_t ) ( duf_record_t * precord, va_list args,
                                       void *sel_cb_udata, duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_dirinfo_t * pdi,
                                       struct duf_scan_callbacks_s * sccb );
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
