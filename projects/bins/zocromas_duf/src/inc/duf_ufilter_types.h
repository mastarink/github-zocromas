#ifndef MAS_DUF_UFILTER_TYPES_H
#  define MAS_DUF_UFILTER_TYPES_H

#  include <mastar/tools/mas_argvc_types.h>

#  include "duf_base_types.h"
#  include "duf_item_types.h"
#  include "duf_flags_types.h"

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

typedef struct globx
{
  mas_argvc_t include_fs_files;
  mas_argvc_t exclude_fs_files;
} duf_filter_globx_t;


typedef struct
{
  unsigned long long dirid;
  char *name;
} duf_filepath_t;

typedef struct
{
  char *type;
} duf_ufilter_mime_t;
typedef struct
{
  char *camera;
} duf_ufilter_exif_t;

typedef struct
{
#  if 0
  duf_limitsll_t md5;
  duf_limitsll_t sha1;
  duf_limitsll_t exif;
  duf_limitsll_t mime;
  duf_limitsll_t data;
  duf_limitsll_t size;
#  else
#    define  ENUM_WRAPMM(_id, _ty, _rf, _rf2, ...) duf_limitsll_t _rf;
#    include "duf_options_enum_filter_same_minmax.def"
#    undef   ENUM_WRAPMM
#  endif
} duf_same_limits_t;

typedef struct
{
  char *md5;
  char *sha1;
  char *exif;
} duf_same_as_t;

typedef struct
{
  int use_format;
  duf_rfilter_flags_combo_t v;
  unsigned max_rel_depth;
  unsigned long long max_seq;
  unsigned long long std_leaf_set;
  unsigned long long std_node_set;
  duf_items_t maxitems;
  duf_limits_t dirfiles;
  duf_filter_globx_t globx;
  /* unsigned long long sd5id; */
  /* unsigned long long md5id; */
  /* unsigned long long crc32id; */
  /* unsigned long long mimeid; */
  /* unsigned long long exifid; */
#  if 0
  duf_limitsll_t size;
  duf_limitsll_t nameid;
  duf_limitsll_t dataid;
  duf_limitsll_t dirid;
  duf_limitsll_t mtime;
  duf_limitsll_t atime;
  duf_limitsll_t ctime;
  duf_limitsll_t exifdt;
  duf_limitsll_t inode;
  duf_limitsll_t md5id;
  duf_limitsll_t sha1id;
  duf_limitsll_t sd5id;
  duf_limitsll_t crc32id;
  duf_limitsll_t mimeid;
  duf_limitsll_t exifid;
#  else
#    define  ENUM_WRAPMM(_id, _ty, _rf, _rf2, ...) duf_limitsll_t _rf;
#    include "duf_options_enum_filter_minmax.def"
#    undef   ENUM_WRAPMM
#  endif
  duf_same_limits_t same;
  duf_itemtag_t tag;
  char *filename;
  char *glob_db;
  char *glob_db_include;
  char *glob_db_exclude;
  duf_same_as_t same_as;
  duf_ufilter_mime_t mime;
  duf_ufilter_exif_t exif;
  unsigned long testnum;
  /* unsigned long long filter_id; */
} duf_ufilter_t;

typedef struct
{
  unsigned long long topdirid;
} duf_yfilter_t;

#endif

/*
vi: ft=c
*/
