#include <string.h>
#include <dlfcn.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"
#include "duf_option_names.h"

#include "duf_sccb.h"

/* ###################################################################### */
#include "duf_action_table.h"
/* ###################################################################### */



static duf_action_table_t actions_table[] = {
  /* {.tovector = 1,.in_use = 1,                                        */
  /*  .sccb = &duf_dirs_callbacks,                               */
  /*  .on.flag = { (* .collect_obs = 1, *) .dirent = 1,.allow_dirs = 1} */
  /*  },                                                                */
  /* {.tovector = 1,.in_use = 1,                                              */
  /*  .sccb = &duf_filedata_callbacks,                                        */
  /*  .on.flag = { (* .collect_obs = 1, *) .dirent = 1 (* ,.filedata = 1 *) } */
  /*  },                                                                      */
  /* {.tovector = 1,.in_use = 1,                                               */
  /*  .sccb = &duf_filenames_callbacks,                                        */
  /*  .on.flag = { (* .collect_obs = 1, *) .dirent = 1 (* ,.filenames = 1 *) } */
  /*  },                                                                       */
  /* {.tovector = 1,.in_use = 1,                                */
  /*  .sccb = &duf_crc32_callbacks,                             */
  /*  .on.flag = { (* .collect_obs = 1, *)(*.crc32_obs = 1 *) } */
  /*  },                                                        */
  /* {.tovector = 1,.in_use = 1,                              */
  /*  .sccb = &duf_sd5_callbacks,                             */
  /*  .on.flag = { (* .collect_obs = 1, *)(*.sd5_obs = 1 *) } */
  /*  },                                                      */
  /* {.tovector = 1,.in_use = 1,                              */
  /*  .sccb = &duf_md5_callbacks,                             */
  /*  .on.flag = { (* .collect_obs = 1, *)(*.md5_obs = 1 *) } */
  /*  },                                                      */
  /* {.tovector = 1,.in_use = 1,                               */
  /*  .sccb = &duf_sha1_callbacks,                             */
  /*  .on.flag = { (* .collect_obs = 1, *)(*.sha1_obs = 1 *) } */
  /*  },                                                       */
  /* {.tovector = 1,.in_use = 1,                                */
  /*  .sccb = &duf_mime_callbacks,                              */
  /*  .on.flag = { (* .collect_obs = 1, *)(* .mime_obs = 1 *) } */
  /*  },                                                        */
  /* {.tovector = 1,.in_use = 1,                                */
  /*  .sccb = &duf_exif_callbacks,                              */
  /*  .on.flag = { (* .collect_obs = 1, *)(* .exif_obs = 1 *) } */
  /*  },                                                        */
  /* {.sccb = &duf_mdpath_callbacks, */
  /*  .on.flag = {.mdpath = 1}},             */
  /* {.tovector = 0,.in_use = 1,    */
  /*  .sccb = &duf_tagit_callbacks, */
  /*  },                            */
  /* {.tovector = 0,.in_use = 1,      */
  /*  .sccb = &duf_save_to_callbacks, */
  /*  },                              */
  {.tovector = 0,.in_use = 1,
   .sccb = &duf_dummy_callbacks,
   },
  /* {.tovector = 0,.in_use = 1,      */
  /*  .sccb = &duf_dumplet_callbacks, */
  /*  },                              */
  /* {.tovector = 0,.in_use = 1,     */
  /*  .sccb = &duf_dialog_callbacks, */
  /*  },                             */

#if 0
  {.tovector = 0,.in_use = 1,
   .sccb = &duf_tree_callbacks,
#if 0
   .on.flag = {.print = 1,.tree = 1},
#endif
   .off.flag = { /*.md5_obs = 1 */ }
   },
#endif
/*   {.tovector = 0,.in_use = 1,                     */
/*    .sccb = &duf_listing_callbacks,                */
/* #if 0                                             */
/*    .on.flag = {.print = 1},                       */
/* #endif                                            */
/*    .off.flag = {                (*.md5_obs = 1 *) */
/* #if 0                                             */
/*                 ,.tree = 1                        */
/* #endif                                            */
/*                 }                                 */
/*    },                                             */
  /* {.sccb = &duf_print_md5_callbacks, */
  /*  .on.flag = {.print = 1,.md5 = 1}, */
  /*  },                                */
  {.sccb = NULL,.end_of_table = 1},
};

static void
init_list( void )
{
  static unsigned inited = 0;

  if ( !inited )
  {
    duf_action_table_t *prev = NULL;

    for ( duf_action_table_t * act = actions_table; act && act->sccb; act++ )
    {
      if ( act->in_use )
      {
        if ( prev )
          prev->sccb->next = act->sccb;
        prev = act;
      }
    }
    /* TODO additionally load dynamic here */
  }
}

/* duf_action_table_t *      */
/* _duf_action_table( void ) */
/* {                         */
/*   init_list(  );          */
/*   return actions_table;   */
/* }                         */

duf_scan_callbacks_t *
duf_first_sccb( void )
{
  duf_action_table_t *at = NULL;

  init_list(  );
  at = actions_table;
  return at ? at->sccb : NULL;
}

__attribute__ ( ( destructor( 101 ) ) )
     static void duf_sccb_dlclose( void )
{
  for ( duf_scan_callbacks_t * sccb = duf_first_sccb(  ); sccb; sccb = sccb->next )
  {
    if ( sccb->dlhan )
      dlclose( sccb->dlhan );
    sccb->dlhan = NULL;
  }
}
