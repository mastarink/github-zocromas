#ifndef MAS_DUF_UFILTER_TYPES_H
#  define MAS_DUF_UFILTER_TYPES_H

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
  duf_argvc_t include_fs_files;
  duf_argvc_t exclude_fs_files;
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
  duf_rfilter_flags_combo_t v;
  unsigned max_rel_depth;
  unsigned long long max_seq;
  duf_items_t maxitems;
  duf_limits_t dirfiles;
  duf_filter_globx_t globx;
  /* unsigned long long sd5id; */
  /* unsigned long long md5id; */
  /* unsigned long long crc32id; */
  /* unsigned long long mimeid; */
  /* unsigned long long exifid; */
  duf_limitsll_t size;
  duf_limitsll_t same;
  duf_limitsll_t exifsame;
  duf_limitsll_t mimesame;
  duf_limitsll_t nameid;
  duf_limitsll_t dataid;
  duf_limitsll_t dirid;
  duf_limitsll_t mtime;
  duf_limitsll_t atime;
  duf_limitsll_t ctime;
  duf_limitsll_t exifdt;
  duf_limitsll_t inode;
  duf_limitsll_t md5id;
  duf_limitsll_t sd5id;
  duf_limitsll_t crc32id;
  duf_limitsll_t mimeid;
  duf_limitsll_t exifid;
  duf_itemtag_t tag;
  char *glob_db;
  char *glob_db_include;
  char *glob_db_exclude;
  char *same_md5;
  duf_ufilter_mime_t mime;
  duf_ufilter_exif_t exif;
  /* unsigned long long filter_id; */
} duf_ufilter_t;

#endif

/*
vi: ft=c
*/
