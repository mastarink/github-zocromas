#define DUF_SQL_PDI_STMT

#include <string.h>
#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_maintenance.h"

#include "duf_config_ref.h"

#include "duf_sccb.h"

/* ###################################################################### */
#include "duf_prepare_actions.h"
/* ###################################################################### */



typedef struct
{
  duf_config_act_flags_combo_t on;
  duf_config_act_flags_combo_t off;
  duf_scan_callbacks_t *sccb;
} duf_action_table_t;

extern duf_scan_callbacks_t duf_integrity_callbacks __attribute( ( weak ) ),
      duf_directories_callbacks __attribute( ( weak ) ),
      duf_filedata_callbacks __attribute( ( weak ) ),
      duf_filenames_callbacks __attribute( ( weak ) ),
      duf_collect_openat_crc32_callbacks __attribute( ( weak ) ),
      duf_collect_openat_sd5_callbacks __attribute( ( weak ) ),
      duf_collect_openat_md5_callbacks __attribute( ( weak ) ),
      duf_collect_mime_callbacks __attribute( ( weak ) ),
      duf_collect_exif_callbacks __attribute( ( weak ) ),
      duf_collect_mdpath_callbacks __attribute( ( weak ) ),
      duf_print_md5_callbacks __attribute( ( weak ) ),
      duf_print_tree_callbacks __attribute( ( weak ) ),
      duf_print_dir_callbacks __attribute( ( weak ) ),
      duf_bubububububububububububububububububububububububububububububububububububu __attribute( ( weak ) );

static duf_action_table_t act_table[] = {
  {.sccb = &duf_integrity_callbacks,
   .on.flag = {.integrity = 1}
   },
  {.sccb = &duf_directories_callbacks,
   .on.flag = {.collect = 1,.dirent = 1,.dirs = 1}
   },
  {.sccb = &duf_filedata_callbacks,
   .on.flag = {.collect = 1,.dirent = 1,.filedata = 1}
   },
  {.sccb = &duf_filenames_callbacks,
   .on.flag = {.collect = 1,.dirent = 1,.filenames = 1}
   },
  {.sccb = &duf_collect_openat_crc32_callbacks,
   .on.flag = {.collect = 1,.crc32 = 1}
   },
  {.sccb = &duf_collect_openat_sd5_callbacks,
   .on.flag = {.collect = 1,.sd5 = 1}
   },
  {.sccb = &duf_collect_openat_md5_callbacks,
   .on.flag = {.collect = 1,.md5 = 1}
   },
  {.sccb = &duf_collect_mime_callbacks,
   .on.flag = {.collect = 1,.mime = 1}
   },
  {.sccb = &duf_collect_exif_callbacks,
   .on.flag = {.collect = 1,.exif = 1}
   },
  /* {.sccb = &duf_collect_mdpath_callbacks, */
  /*  .on.flag = {.mdpath = 1}},             */

  {.sccb = &duf_print_tree_callbacks,
   .on.flag = {.print = 1,.tree = 1},
   .off.flag = {.md5 = 1}
   },
  {.sccb = &duf_print_dir_callbacks,
   .on.flag = {.print = 1},
   .off.flag = {.md5 = 1,.tree = 1}
   },
  {.sccb = &duf_print_md5_callbacks,
   .on.flag = {.print = 1,.md5 = 1},
   },
};

#if 1
int
duf_set_actions( duf_scan_callbacks_t ** ppscan_callbacks, int max_asteps )
{
  int asteps = 0;
  int num;

  num = sizeof( act_table ) / sizeof( act_table[0] );
  for ( int iac = 0; iac < num; iac++ )
  {
    duf_scan_callbacks_t *sccb = act_table[iac].sccb;

    if ( ( duf_config->cli.act.v.bit & act_table[iac].on.bit ) == act_table[iac].on.bit
         && ( duf_config->cli.act.v.bit & act_table[iac].off.bit ) == 0 )
    {
      if ( sccb )
      {
        ppscan_callbacks[asteps++] = sccb;
        DUF_TRACE( action, 0, "#%d (%d) of %d action prepared: %s", iac, asteps, num, duf_uni_scan_action_title( sccb ) );
      }
      else
      {
        DUF_TRACE( action, 0, "#%d of %d action not prepared (no sccb): %s : %x on:%x off:%x", iac, num, duf_uni_scan_action_title( sccb ),
                   duf_config->cli.act.v.bit, act_table[iac].on.bit, act_table[iac].off.bit );
      }
    }
    else
    {
      DUF_TRACE( action, 0, "#%d of %d action not prepared: %s : %x on:%x off:%x", iac, num, duf_uni_scan_action_title( sccb ),
                 duf_config->cli.act.v.bit, act_table[iac].on.bit, act_table[iac].off.bit );
    }
  }
  return asteps;
}
#else
static int
duf_set_actions( duf_scan_callbacks_t ** ppscan_callbacks, int max_asteps )
{
  int asteps = 0;

  /*
   * 1. DUF_ACT_FLAG( integrity ) => duf_integrity_callbacks
   * 
   * 2. DUF_ACT_FLAG( collect )
   *   2.1 DUF_ACT_FLAG( dirent )
   *     2.1.1.    DUF_ACT_FLAG( dirs )  => duf_directories_callbacks
   *     2.1.2    DUF_ACT_FLAG( filedata ) => duf_filedata_callbacks
   *     2.1.3    DUF_ACT_FLAG( filenames ) => duf_filenames_callbacks
   *   2.2.    DUF_ACT_FLAG( crc32 )     => duf_collect_openat_crc32_callbacks
   *   2.3.    DUF_ACT_FLAG( sd5 )       => duf_collect_openat_sd5_callbacks
   *   2.4.    DUF_ACT_FLAG( md5 )       => duf_collect_openat_md5_callbacks
   *   2.5.    DUF_ACT_FLAG( mime )      => duf_collect_mime_callbacks
   *   2.6.    DUF_ACT_FLAG( exif )      => duf_collect_exif_callbacks
   *
   * */
  if ( DUF_ACT_FLAG( integrity ) )
  {
    extern duf_scan_callbacks_t duf_integrity_callbacks /* __attribute( ( weak ) ) */ ;
    duf_scan_callbacks_t *sccb = &duf_integrity_callbacks;

    DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( FLAG_INTEGRITY ) );
    DUF_TRACE( action, 2, "prep integrity ..." );
    DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪integrity≫ #%d", asteps );
    DUF_TRACE( action, 0, "#%d action prepared: %s", asteps, duf_uni_scan_action_title( sccb ) );
    ppscan_callbacks[asteps++] = sccb;
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option", DUF_OPT_NAME( FLAG_INTEGRITY ) );
  }
  assert( asteps < max_asteps );
  if ( DUF_ACT_FLAG( collect ) )
  {
    DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( FLAG_COLLECT ) );
    if ( DUF_ACT_FLAG( dirent ) )
    {
      DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( FLAG_DIRENT ) );
      {
        extern duf_scan_callbacks_t duf_directories_callbacks __attribute( ( weak ) );

        if ( &duf_directories_callbacks && DUF_ACT_FLAG( dirs ) )
        {
          duf_scan_callbacks_t *sccb = &duf_directories_callbacks;

          DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( FLAG_DIRS ) );
          DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪directories≫ #%d", asteps );
          DUF_TRACE( action, 0, "#%d action prepared: %s", asteps, duf_uni_scan_action_title( sccb ) );
          ppscan_callbacks[asteps++] = sccb;
        }
        else
        {
          DUF_TRACE( explain, 1, "no %s option or no callback", DUF_OPT_NAME( FLAG_DIRS ) );
        }
      }
      assert( asteps < max_asteps );
      {
        extern duf_scan_callbacks_t duf_filedata_callbacks __attribute( ( weak ) );


        if ( &duf_filedata_callbacks && DUF_ACT_FLAG( filedata ) )
        {
          duf_scan_callbacks_t *sccb = &duf_filedata_callbacks;

          DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( FLAG_FILEDATA ) );
          DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪filedata≫ #%d", asteps );
          DUF_TRACE( action, 0, "#%d action prepared: %s", asteps, duf_uni_scan_action_title( sccb ) );
          ppscan_callbacks[asteps++] = sccb;
        }
        else
        {
          DUF_TRACE( explain, 1, "no %s option or no callback", DUF_OPT_NAME( FLAG_FILEDATA ) );
        }
      }
      assert( asteps < max_asteps );
      {
        extern duf_scan_callbacks_t duf_filenames_callbacks __attribute( ( weak ) );


        if ( &duf_filenames_callbacks && DUF_ACT_FLAG( filenames ) )
        {
          duf_scan_callbacks_t *sccb = &duf_filenames_callbacks;

          DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( FLAG_FILENAMES ) );
          DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪filenames≫ #%d", asteps );
          DUF_TRACE( action, 0, "#%d action prepared: %s", asteps, duf_uni_scan_action_title( sccb ) );
          ppscan_callbacks[asteps++] = sccb;
        }
        else
        {
          DUF_TRACE( explain, 1, "no %s option or no callback", DUF_OPT_NAME( FLAG_FILENAMES ) );
        }
      }
    }
    assert( asteps < max_asteps );
    {
      extern duf_scan_callbacks_t duf_collect_openat_crc32_callbacks __attribute( ( weak ) );


      if ( &duf_collect_openat_crc32_callbacks && DUF_ACT_FLAG( crc32 ) )
      {
        duf_scan_callbacks_t *sccb = &duf_collect_openat_crc32_callbacks;

        DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( FLAG_CRC32 ) );
        DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪collect_openat_crc32≫ #%d", asteps );
        DUF_TRACE( action, 0, "#%d action prepared: %s", asteps, duf_uni_scan_action_title( sccb ) );
        ppscan_callbacks[asteps++] = sccb;
      }
      else
      {
        DUF_TRACE( explain, 1, "no %s option or no callback", DUF_OPT_NAME( FLAG_CRC32 ) );
      }
    }
    assert( asteps < max_asteps );
    {
      extern duf_scan_callbacks_t duf_collect_openat_sd5_callbacks __attribute( ( weak ) );


      if ( &duf_collect_openat_sd5_callbacks && DUF_ACT_FLAG( sd5 ) )
      {
        duf_scan_callbacks_t *sccb = &duf_collect_openat_sd5_callbacks;

        DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( FLAG_SD5 ) );
        DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪collect_openat_sd5≫ #%d", asteps );
        DUF_TRACE( action, 0, "#%d action prepared: %s", asteps, duf_uni_scan_action_title( sccb ) );
        ppscan_callbacks[asteps++] = sccb;
      }
      else
      {
        DUF_TRACE( explain, 1, "no %s option or no callback", DUF_OPT_NAME( FLAG_SD5 ) );
      }
    }
    assert( asteps < max_asteps );
    {
      extern duf_scan_callbacks_t duf_collect_openat_md5_callbacks __attribute( ( weak ) );


      if ( &duf_collect_openat_md5_callbacks && DUF_ACT_FLAG( md5 ) )
      {
        duf_scan_callbacks_t *sccb = &duf_collect_openat_md5_callbacks;

        DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( FLAG_MD5 ) );
        DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪collect_openat_md5≫ #%d", asteps );
        DUF_TRACE( action, 0, "#%d action prepared: %s", asteps, duf_uni_scan_action_title( sccb ) );
        ppscan_callbacks[asteps++] = sccb;
      }
      else
      {
        DUF_TRACE( explain, 1, "no %s option or no callback", DUF_OPT_NAME( FLAG_MD5 ) );
      }
    }
    assert( asteps < max_asteps );
    {
      extern duf_scan_callbacks_t duf_collect_mime_callbacks __attribute( ( weak ) );

      if ( &duf_collect_mime_callbacks && DUF_ACT_FLAG( mime ) )
      {
        duf_scan_callbacks_t *sccb = &duf_collect_mime_callbacks;

        DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( FLAG_MIME ) );
        DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪collect_mime≫ #%d", asteps );
        DUF_TRACE( action, 0, "#%d action prepared: %s", asteps, duf_uni_scan_action_title( sccb ) );
        ppscan_callbacks[asteps++] = sccb;
      }
      else
      {
        DUF_TRACE( explain, 1, "no %s option or no callback", DUF_OPT_NAME( FLAG_MIME ) );
      }
    }
    assert( asteps < max_asteps );
    {
      extern duf_scan_callbacks_t duf_collect_exif_callbacks __attribute( ( weak ) );

      if ( &duf_collect_exif_callbacks && DUF_ACT_FLAG( exif ) )
      {
        duf_scan_callbacks_t *sccb = &duf_collect_exif_callbacks;

        DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( FLAG_EXIF ) );
        DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪collect_exif≫ #%d", asteps );
        DUF_TRACE( action, 0, "#%d action prepared: %s", asteps, duf_uni_scan_action_title( sccb ) );
        ppscan_callbacks[asteps++] = sccb;
      }
      else
      {
        DUF_TRACE( explain, 1, "no %s option or no callback", DUF_OPT_NAME( FLAG_EXIF ) );
      }
    }
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option", DUF_OPT_NAME( FLAG_COLLECT ) );
  }

  assert( asteps < max_asteps );
  if (  /* DUF_ACT_FLAG(update )&& */ DUF_ACT_FLAG( mdpath ) )
  {
    extern duf_scan_callbacks_t duf_collect_mdpath_callbacks /* __attribute( ( weak ) ) */ ;
    duf_scan_callbacks_t *sccb = &duf_collect_mdpath_callbacks;

    DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( FLAG_MDPATH ) );
    DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪collect_mdpath≫ #%d", asteps );
    DUF_TRACE( action, 0, "#%d action prepared: %s", asteps, duf_uni_scan_action_title( sccb ) );
    ppscan_callbacks[asteps++] = sccb;
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option", DUF_OPT_NAME( FLAG_MDPATH ) );
  }
  assert( asteps < max_asteps );
  if ( DUF_ACT_FLAG( print ) )
  {
    DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( FLAG_PRINT ) );
    if ( DUF_ACT_FLAG( md5 ) )
    {
      extern duf_scan_callbacks_t duf_print_md5_callbacks /* __attribute( ( weak ) ) */ ;
      duf_scan_callbacks_t *sccb = &duf_print_md5_callbacks;

      DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( FLAG_MD5 ) );
      DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪print_md5≫ #%d", asteps );
      DUF_TRACE( action, 0, "#%d action prepared: %s", asteps, duf_uni_scan_action_title( sccb ) );
      ppscan_callbacks[asteps++] = sccb;
    }
    else
    {
      assert( asteps < max_asteps );
      DUF_TRACE( explain, 1, "no %s option", DUF_OPT_NAME( FLAG_MD5 ) );
      if ( DUF_ACT_FLAG( tree ) )
      {
        extern duf_scan_callbacks_t duf_print_tree_callbacks /* __attribute( ( weak ) ) */ ;
        duf_scan_callbacks_t *sccb = &duf_print_tree_callbacks;

        DUF_TRACE( explain, 0, "     option %s", DUF_OPT_NAME( FLAG_TREE ) );
        DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪print_tree≫ #%d", asteps );
        DUF_TRACE( action, 0, "#%d action prepared: %s", asteps, duf_uni_scan_action_title( sccb ) );
        ppscan_callbacks[asteps++] = sccb;
      }
      else
      {
        assert( asteps < max_asteps );
        extern duf_scan_callbacks_t duf_print_dir_callbacks /* __attribute( ( weak ) ) */ ;
        duf_scan_callbacks_t *sccb = &duf_print_dir_callbacks;

        DUF_TRACE( explain, 1, "no %s option", DUF_OPT_NAME( FLAG_TREE ) );
        DUF_TRACE( explain, 0, "◇◇◇◇◇◇ set action ≪print_dir≫ #%d", asteps );
        DUF_TRACE( action, 0, "#%d action prepared: %s", asteps, duf_uni_scan_action_title( sccb ) );
        ppscan_callbacks[asteps++] = sccb;
      }
    }
  }
  else
  {
    DUF_TRACE( explain, 1, "no %s option", DUF_OPT_NAME( FLAG_PRINT ) );
  }
  return asteps;
}
#endif
int
duf_set_actions_sample( duf_scan_callbacks_t ** ppscan_callbacks, int max_asteps )
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
