#ifndef MAS_DUF_UFILTER_TYPES_H
#  define MAS_DUF_UFILTER_TYPES_H

#include "duf_item_types.h"

/* typedef struct                                  */
/* {                                               */
/*   int depth;                    (* signed !! *) */
/*   const struct                                  */
/*   {                                             */
/*     int *pseq;                                  */
/*     int dofiles;                                */
/*   } c;                                          */
/*   const duf_ufilter_t u;                        */
/* } duf_filter_t;                                 */

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
  unsigned long long min;
  unsigned long long max;
} duf_limits_t;

typedef struct
{
  unsigned recursive:1;
  unsigned noself_dir:1;
  unsigned noupper_dirs:1;
} duf_ufilter_flags_t;

typedef struct
{
  duf_ufilter_flags_t flag;
  unsigned maxreldepth;
  unsigned long long maxseq;
  duf_items_t maxitems;
  duf_limits_t dirfiles;
  duf_filter_glob_t glob;
  unsigned long long sd5id;
  unsigned long long md5id;
  unsigned long long crc32id;
  unsigned long long mimeid;
  unsigned long long exifid;
  duf_limits_t size;
  duf_limits_t same;
  unsigned long long filter_id;
} duf_ufilter_t;

#endif
