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

#include "duf_cli_types.h"

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
  duf_ufilter_t u;
  duf_config_cli_t cli;
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
typedef int ( *duf_scan_callback_init_t ) ( struct duf_scan_callbacks_s * cb );

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
  const char *fieldset;
  duf_scan_callback_init_t init_scan;
  duf_scan_callback_dir_t directory_scan;
  duf_scan_callback_file_t file_scan;
};

typedef struct duf_scan_callbacks_s duf_scan_callbacks_t;




#endif
