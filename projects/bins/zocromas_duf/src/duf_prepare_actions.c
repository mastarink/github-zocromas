#define DUF_SQL_PDI_STMT

#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"
#include "duf_option_names.h"

#include "duf_sccb.h"

/* ###################################################################### */
#include "duf_prepare_actions.h"
/* ###################################################################### */



extern duf_scan_callbacks_t duf_integrity_callbacks __attribute( ( weak ) ),
      duf_directories_callbacks __attribute( ( weak ) ),
      duf_filedata_callbacks __attribute( ( weak ) ),
      duf_filenames_callbacks __attribute( ( weak ) ),
      duf_collect_openat_crc32_callbacks __attribute( ( weak ) ),
      duf_collect_openat_sd5_callbacks __attribute( ( weak ) ),
      duf_collect_openat_md5_callbacks __attribute( ( weak ) ),
      duf_collect_mime_callbacks __attribute( ( weak ) ), duf_collect_exif_callbacks __attribute( ( weak ) ),
      /* duf_collect_mdpath_callbacks __attribute( ( weak ) ), */
      /* duf_print_md5_callbacks __attribute( ( weak ) ),      */
 
      duf_print_tree_callbacks __attribute( ( weak ) ),
      duf_print_dir_callbacks __attribute( ( weak ) ),
      duf_bpbpbpbp_$_pbpbpbpbpbpbpbpbpbpbpbpbpbpbpbpbpbpbpbpbpbpbpbpbpbpbpbpbpbpbp __attribute( ( weak ) );

static duf_action_table_t actions_table[] = {
  {.tovector = 1,
   .sccb = &duf_integrity_callbacks,
   .on.flag = {.integrity = 1}
   },
  {.tovector = 1,
   .sccb = &duf_directories_callbacks,
   .on.flag = {.collect = 1,.dirent = 1,.dirs = 1}
   },
  {.tovector = 1,
   .sccb = &duf_filedata_callbacks,
   .on.flag = {.collect = 1,.dirent = 1,.filedata = 1}
   },
  {.tovector = 1,
   .sccb = &duf_filenames_callbacks,
   .on.flag = {.collect = 1,.dirent = 1,.filenames = 1}
   },
  {.tovector = 1,
   .sccb = &duf_collect_openat_crc32_callbacks,
   .on.flag = {.collect = 1,.crc32 = 1}
   },
  {.tovector = 1,
   .sccb = &duf_collect_openat_sd5_callbacks,
   .on.flag = {.collect = 1,.sd5 = 1}
   },
  {.tovector = 1,
   .sccb = &duf_collect_openat_md5_callbacks,
   .on.flag = {.collect = 1,.md5 = 1}
   },
  {.tovector = 1,
   .sccb = &duf_collect_mime_callbacks,
   .on.flag = {.collect = 1,.mime = 1}
   },
  {.tovector = 1,
   .sccb = &duf_collect_exif_callbacks,
   .on.flag = {.collect = 1,.exif = 1}
   },
  /* {.sccb = &duf_collect_mdpath_callbacks, */
  /*  .on.flag = {.mdpath = 1}},             */

  {.tovector = 0,
   .sccb = &duf_print_tree_callbacks,
#if 0
   .on.flag = {.print = 1,.tree = 1},
#endif
   .off.flag = {.md5 = 1}
   },
  {.tovector = 0,
   .sccb = &duf_print_dir_callbacks,
#if 0
   .on.flag = {.print = 1},
#endif
   .off.flag = {.md5 = 1
#if 0
     ,.tree = 1
#endif
   }
   },
  /* {.sccb = &duf_print_md5_callbacks, */
  /*  .on.flag = {.print = 1,.md5 = 1}, */
  /*  },                                */
  {.sccb = NULL},
};


duf_action_table_t *
duf_action_table( void )
{
  return actions_table;
}

/*
 * "convert" «duf_config» representation of tasks/actions into «sccb» list (duf_scan_callbacks_t),
 *         return list length (# of items)
 * ********************************************************************************************
 *  ppscan_callbacks (duf_scan_callbacks_t **) buffer must be inited for enough number of sccb (duf_scan_callbacks_t) pointers
 * */
int
duf_config2sccb_vector( duf_scan_callbacks_t ** ppscan_callbacks, int max_asteps )
{
  int asteps = 0;

  if ( ppscan_callbacks )
  {
    ppscan_callbacks[asteps] = NULL;
    for ( duf_action_table_t * act = duf_action_table(  ); act->sccb && asteps < max_asteps; act++ )
    {
      duf_scan_callbacks_t *sccb = act->sccb;

      if ( act->tovector && ( duf_config->cli.act.v.bit & act->on.bit ) == act->on.bit && ( duf_config->cli.act.v.bit & act->off.bit ) == 0 )
      {
        if ( sccb )
        {
          ppscan_callbacks[asteps++] = sccb;
          ppscan_callbacks[asteps] = NULL;
          DUF_TRACE( action, 0, "action prepared: %s", duf_uni_scan_action_title( sccb ) );
        }
        else
        {
          DUF_TRACE( action, 0, "action not prepared (no sccb): %s : %x on:%x off:%x", duf_uni_scan_action_title( sccb ),
                     duf_config->cli.act.v.bit, act->on.bit, act->off.bit );
        }
      }
      else
      {
        DUF_TRACE( action, 0, "action not prepared: %s : %x on:%x off:%x", duf_uni_scan_action_title( sccb ),
                   duf_config->cli.act.v.bit, act->on.bit, act->off.bit );
      }
    }
  }
  return asteps;
}

int
duf_config2sccb_vector_sample( duf_scan_callbacks_t ** ppscan_callbacks, int max_asteps )
{
  int asteps = 0;

  if ( duf_config->cli.act.sample )
  {
    extern duf_scan_callbacks_t duf_sample_callbacks /* __attribute( ( weak ) ) */ ;

    DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( SAMPLE ) );
    assert( asteps + duf_config->cli.act.sample < max_asteps );
    for ( int i = 0; i < duf_config->cli.act.sample && asteps < max_asteps; i++ )
    {
      duf_scan_callbacks_t *sccb = &duf_sample_callbacks;

      DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪sample #%d", asteps );
      DUF_TRACE( action, 0, "#%d action prepared: %s", asteps, duf_uni_scan_action_title( sccb ) );
      ppscan_callbacks[asteps++] = sccb;
    }
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option or no callback", DUF_OPT_NAME( SAMPLE ) );
  }

  assert( asteps < max_asteps );
  if ( duf_config->cli.act.sampupd )
  {
    extern duf_scan_callbacks_t duf_sampupd_callbacks /* __attribute( ( weak ) ) */ ;
    duf_scan_callbacks_t *sccb = &duf_sampupd_callbacks;

    DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( SAMPUPD ) );
    DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪sampupd≫ #%d", asteps );
    DUF_TRACE( action, 0, "#%d action prepared: %s", asteps, duf_uni_scan_action_title( sccb ) );
    ppscan_callbacks[asteps++] = sccb;
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option or no callback", DUF_OPT_NAME( SAMPUPD ) );
  }
  return asteps;
}
