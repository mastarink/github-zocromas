#ifndef MAS_DUF_UFILTER_STRUCTS_H
# define MAS_DUF_UFILTER_STRUCTS_H

# include <mastar/types/mas_common_types.h>
# include <mastar/tools/mas_argvc_types.h>                           /* mas_argvc_t; mas_cargvc_t; ▤ */

# include "duf_base_types.h"
# include "duf_item_types.h"                                         /* duf_items_t ✗ */
# include "duf_config_puz_types.h"                                   /* duf_config_puz_flags_combo_t; duf_config_puz_flags_t; duf_config_puz_flags_enum_t; ✗ */

# include "duf_ufilter_types.h"                                      /* duf_ufilter_t; duf_yfilter_t; etc. ✗ */

struct duf_filter_globx_s
{
  mas_argvc_t include_fs_files;
  mas_argvc_t exclude_fs_files;
};

struct duf_filepath_s
{
  unsigned long long dirid;
  char *name;
};

struct duf_ufilter_mime_s
{
  char *type;
};

struct duf_ufilter_exif_s
{
  char *camera;
};

struct duf_same_limits_s
{
# if 0
  mas_limitsll_t md5;
  mas_limitsll_t sha1;
  mas_limitsll_t exif;
  mas_limitsll_t mime;
  mas_limitsll_t data;
  mas_limitsll_t size;
# else
#  define  ENUM_WRAPMM(_id, _ty, _rf, _rf2, ...) mas_limitsll_t _rf;
#  include "duf_optimpl_enum_filter_same_minmax.def"
#  undef   ENUM_WRAPMM
# endif
};

struct duf_same_as_s
{
  char *md5;
  char *sha1;
  char *exif;
};

struct duf_ufilter_s
{
  int use_format;
  duf_config_puz_flags_combo_t v;
  unsigned orderid;
  unsigned max_rel_depth;
  unsigned long long max_seq;
  unsigned long long std_node_set_num;
  char *std_node_set_name;
  unsigned long long std_leaf_set_num;
  char *std_leaf_set_name;
  duf_items_t maxitems;
  mas_limits_t dirfiles;
  duf_filter_globx_t globx;
/* unsigned long long sd5id; */
/* unsigned long long md5id; */
/* unsigned long long crc32id; */
/* unsigned long long mimeid; */
/* unsigned long long exifid; */
# if 0
  mas_limitsll_t size;
  mas_limitsll_t nameid;
  mas_limitsll_t dataid;
  mas_limitsll_t dirid;
  mas_limitsll_t mtime;
  mas_limitsll_t atime;
  mas_limitsll_t ctime;
  mas_limitsll_t exifdt;
  mas_limitsll_t inode;
  mas_limitsll_t md5id;
  mas_limitsll_t sha1id;
  mas_limitsll_t sd5id;
  mas_limitsll_t crc32id;
  mas_limitsll_t mimeid;
  mas_limitsll_t exifid;
# else
#  define  ENUM_WRAPMM(_id, _ty, _rf, _rf2, ...) mas_limitsll_t _rf;
#  include "duf_optimpl_enum_filter_minmax.def"
#  undef   ENUM_WRAPMM
# endif
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
  char *pack_field;
};

struct duf_yfilter_s
{
  unsigned long long topdirid;
};

#endif

/*
vi: ft=c
*/
