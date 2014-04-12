#ifndef MAS_DUF_TYPES_H
#  define MAS_DUF_TYPES_H
#  include <sys/stat.h>

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

#define DUF_OPTIONS_SHORT "hD:N:RvSFQ"
typedef enum
{
  DUF_OPTION_NONE,
  DUF_OPTION_HELP = 'h',
  DUF_OPTION_DB_DIRECTORY = 'D',
  DUF_OPTION_DB_NAME = 'N',
  DUF_OPTION_RECURSIVE = 'R',
  DUF_OPTION_VERBOSE = 'v',
  DUF_OPTION_SCAN_TRACE='S',
  DUF_OPTION_FILL_TRACE='F',
  DUF_OPTION_SQL_TRACE = 'Q',

  DUF_OPTION_LONG = 1000,

  DUF_OPTION_SAMPLE_TRACE,
  DUF_OPTION_DEBUG,

  DUF_OPTION_MIN_DBGLINE,
  DUF_OPTION_MAX_DBGLINE,
  DUF_OPTION_CREATE_TABLES,
  DUF_OPTION_DROP_TABLES,
  DUF_OPTION_ADD_PATH,
  /* DUF_OPTION_UPDATE_PATH, */
  DUF_OPTION_UPDATE_MD5,
  DUF_OPTION_ZERO_DUPLICATES,
  DUF_OPTION_UPDATE_DUPLICATES,
  DUF_OPTION_UPDATE_MDPATH,
  DUF_OPTION_UPDATE_MDPATH_SELECTIVE,
  DUF_OPTION_UPDATE_FILEDATA,
  DUF_OPTION_ZERO_FILEDATA,
  DUF_OPTION_UPDATE_EXIF,
  DUF_OPTION_MINSIZE,
  DUF_OPTION_MAXSIZE,
  DUF_OPTION_MAXDEPTH,
  DUF_OPTION_MAXSEQ,
  DUF_OPTION_SAMPLE,
  DUF_OPTION_FILL,
  DUF_OPTION_PRINT,
  DUF_OPTION_TREE,
  DUF_OPTION_FILES,
  DUF_OPTION_UNI_SCAN,
  /* DUF_OPTION_PRINT_PATHS, */
  /* DUF_OPTION_PRINT_DIRS, */
  /* DUF_OPTION_PRINT_FILES, */
  /* DUF_OPTION_PRINT_DUPLICATES, */
  DUF_OPTION_SAME_FILES,
  DUF_OPTION_SAME_EXIF,
  DUF_OPTION_SAME_MD5,
  DUF_OPTION_GROUP,
  DUF_OPTION_LIMIT,
  DUF_OPTION_ADD_TO_GROUP,
  DUF_OPTION_REMOVE_FROM_GROUP,
} duf_option_code_t;

typedef struct
{
  unsigned recursive:1;
  unsigned noself_dir:1;
  unsigned noupper_dirs:1;
  int maxdepth;
  unsigned maxseq;
  unsigned long long minsize;
  unsigned long long maxsize;
} duf_ufilter_t;

typedef struct
{
  unsigned drop_tables:1;
  unsigned create_tables:1;
  unsigned add_path:1;
  /* unsigned update_path:1; */
  unsigned update_md5:1;
  unsigned update_duplicates:1;
  unsigned update_mdpath:1;
  unsigned update_mdpath_selective:1;
  unsigned update_filedata:1;
  unsigned zero_filedata:1;
  unsigned update_exif:1;
  unsigned sample:1;
  unsigned fill:1;
  unsigned print:1;
  unsigned tree:1;
  unsigned files:1;
  unsigned uni_scan:1;
  /* unsigned print_paths:1; */
  /* unsigned print_dirs:1; */
  /* unsigned print_files:1; */
  unsigned print_duplicates:1;
  unsigned zero_duplicates:1;
  unsigned same_files:1;
  unsigned same_md5:1;
  unsigned same_exif:1;
  unsigned to_group:1;
  unsigned from_group:1;

  unsigned debug:1;
  unsigned scan_trace;
  unsigned sample_trace;
  unsigned fill_trace;
  unsigned sql_trace;
  unsigned verbose;

  unsigned long min_dbgline;
  unsigned long max_dbgline;
  unsigned long dbglines;

  duf_ufilter_t u;

  long limit;
  char *db_dir;
  char *db_name;
  char *group;
  int targc;
  char **targv;
} duf_config_t;

typedef struct
{
  int level;
  int seq;
  int *levinfo;
  const char *name;
  duf_ufilter_t u;
} duf_dirinfo_t;

typedef struct
{
  int level;
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
typedef int ( *duf_scan_callback_dir_t ) ( unsigned long long pathid, const char *name, unsigned long long items,
                                           duf_dirinfo_t * pdi, struct duf_scan_callbacks_s * cb );

typedef int ( *duf_scan_callback_file_t ) ( unsigned long long pathid, unsigned long long filenameid,
                                            const char *name, void *str_cb_udata, duf_dirinfo_t * pdi,
                                            struct duf_scan_callbacks_s * cb, duf_record_t * precord );

/* typedef int ( *duf_str_cb_t ) ( int nrow, int nrows, unsigned long long pathid, const char *path, unsigned long long filenameid, */
/*                                 const char *name, const struct stat * st, void *str_cb_udata, duf_scan_callbacks_t * cb,         */
/*                                 const char *const *presult );                                                                    */
typedef int ( *duf_sql_select_cb_t ) ( duf_record_t * precord, va_list args,
                                       void *sel_cb_udata, duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_dirinfo_t * pdi,
                                       struct duf_scan_callbacks_s * cb );
struct duf_scan_callbacks_s
{
  duf_scan_callback_dir_t directory_scan;
  duf_scan_callback_file_t file_scan;
};

typedef struct duf_scan_callbacks_s duf_scan_callbacks_t;




#endif
