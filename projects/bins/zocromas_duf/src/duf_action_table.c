#include <string.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"
#include "duf_option_names.h"

#include "duf_sccb.h"

/* ###################################################################### */
#include "duf_action_table.h"
/* ###################################################################### */



static duf_action_table_t actions_table[] = {
  {.tovector = 1,.in_use = 1,
   .sccb = &duf_directories_callbacks,
   .on.flag = {.collect_obs = 1,.dirent = 1,.allow_dirs = 1}
   },
  {.tovector = 1,.in_use = 1,
   .sccb = &duf_filedata_callbacks,
   .on.flag = {.collect_obs = 1,.dirent = 1,.filedata = 1}
   },
  {.tovector = 1,.in_use = 1,
   .sccb = &duf_filenames_callbacks,
   .on.flag = {.collect_obs = 1,.dirent = 1,.filenames = 1}
   },
  {.tovector = 1,.in_use = 1,
   .sccb = &duf_collect_openat_crc32_callbacks,
   .on.flag = {.collect_obs = 1,.crc32_obs = 1}
   },
  {.tovector = 1,.in_use = 1,
   .sccb = &duf_collect_openat_sd5_callbacks,
   .on.flag = {.collect_obs = 1,.sd5_obs = 1}
   },
  {.tovector = 1,.in_use = 1,
   .sccb = &duf_collect_openat_md5_callbacks,
   .on.flag = {.collect_obs = 1,.md5_obs = 1}
   },
  {.tovector = 1,.in_use = 1,
   .sccb = &duf_collect_mime_callbacks,
   .on.flag = {.collect_obs = 1,.mime_obs = 1}
   },
  {.tovector = 1,.in_use = 1,
   .sccb = &duf_collect_exif_callbacks,
   .on.flag = {.collect_obs = 1,.exif_obs = 1}
   },
  /* {.sccb = &duf_collect_mdpath_callbacks, */
  /*  .on.flag = {.mdpath = 1}},             */
  {.tovector = 0,.in_use = 1,
   .sccb = &duf_tagit_callbacks,
   },
  {.tovector = 0,.in_use = 1,
   .sccb = &duf_save_to_callbacks,
   },
  {.tovector = 0,.in_use = 1,
   .sccb = &duf_dummy_callbacks,
   },
  {.tovector = 0,.in_use = 1,
   .sccb = &duf_dumplet_callbacks,
   },
  {.tovector = 0,.in_use = 1,
   .sccb = &duf_dialog_callbacks,
   },

  {.tovector = 0,.in_use = 1,
   .sccb = &duf_print_tree_callbacks,
#if 0
   .on.flag = {.print = 1,.tree = 1},
#endif
   .off.flag = {.md5_obs = 1}
   },
  {.tovector = 0,.in_use = 1,
   .sccb = &duf_print_dir_callbacks,
#if 0
   .on.flag = {.print = 1},
#endif
   .off.flag = {.md5_obs = 1
#if 0
                ,.tree = 1
#endif
                }
   },
  /* {.sccb = &duf_print_md5_callbacks, */
  /*  .on.flag = {.print = 1,.md5 = 1}, */
  /*  },                                */
  {.sccb = NULL,.end_of_table = 1},
};


duf_action_table_t *
duf_action_table( void )
{
  return actions_table;
}
