#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */
#include <sys/stat.h>
#include <time.h>


#include <openssl/md5.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_utils.h"
#include "duf_service.h"
#include "duf_config.h"

#include "duf_pdi.h"
#include "duf_levinfo.h"

#include "duf_sql_field.h"

/* #include "duf_path.h" */

#include "duf_sql.h"

#include "duf_dbg.h"

/* ###################################################################### */
/* #include "duf_tree_print_uni.h" */
/* ###################################################################### */


static int
duf_sql_print_tree_prefix_uni( duf_depthinfo_t * pdi, int is_file )
{
  int r = 0;
  int max = duf_pdi_depth( pdi );

  /* if ( pdi->depth <= 0 )                                             */
  /*   DUF_ERROR( "depth should not be %d at this point", pdi->depth ); */
  if ( duf_pdi_reldepth( pdi ) >= 0 )
  {

    DEBUG_START(  );
    if ( pdi )
    {
      int d0;

      d0 = duf_pdi_topdepth( pdi );
      if ( d0 > 0 )
        d0--;
      d0 = 0;
      for ( int d = d0; d <= max; d++ )
      {
        int du = d - 1;
        unsigned flags = 0;
        long ndu = duf_levinfo_numdir_d( pdi, du );
        char nduc = ndu > 0 ? '+' : ( ndu < 0 ? '-' : 'z' );
        int leaf = duf_levinfo_is_leaf_d( pdi, d );
        char leafc = leaf ? 'L' : 'N';
        int eod = duf_levinfo_eod_d( pdi, d );
        char eodc = eod ? 'E' : '-';
        char is_filec = is_file ? 'F' : '-';

        if ( ndu > 0 )
          flags |= 0x1;
        if ( ndu < 0 )
          flags |= 0x2;
        if ( leaf )
          flags |= 0x4;
        if ( eod )
          flags |= 0x8;
        if ( d == max )
          flags |= 0x10;
        if ( d >= pdi->maxdepth -1)
          flags |= 0x20;
        /* if ( is_file )   */
        /*   flags |= 0x40; */
        if ( duf_config->cli.dbg.debug )
        {
          DUF_PRINTF( 0, ".L%-2d", d );
          DUF_PRINTF( 0, ".M%-2d", pdi->maxdepth );
          DUF_PRINTF( 0, ".rd%d", duf_pdi_reldepth( pdi ) );
          /* DUF_PRINTF( 0, ".(%3ld)", ndu ); */
          DUF_PRINTF( 0, ".%c%c%c%c", eodc, nduc, leafc, is_filec );
          /* DUF_PRINTF( 0, ".0x%02x",  flags ); */
        }
        if ( pdi->levinfo )
        {
          if ( 1 )
          {
            /* if ( duf_levinfo_is_leaf_d( pdi, d ) ) */
            /*   DUF_PRINTF( 0, ".[  ◇ ]" );        */
            /* else                                   */
            switch ( flags )
            {
            case 0x14:
            case 0x34:
            case 0x35:
              DUF_PRINTF( 0, ".  ◇ " );
              break;
            case 0x15:
              DUF_PRINTF( 0, ".│ ◇ " );
              break;
            case 0x10:
              DUF_PRINTF( 0, ".└─── " );
              break;
            case 0x11:
              DUF_PRINTF( 0, ".├─── " );
              break;
            case 0x1:
              DUF_PRINTF( 0, ".│    " );
              break;
            case 0x8:
              DUF_PRINTF( 0, ".     " );
              break;
            case 0x2:
            case 0x0:
              /* DUF_PRINTF( 0, ".     " ); */
              break;
            default:
              DUF_PRINTF( 0, ".  0x%02x", flags );
              break;
            }
          }
          else
          {
            if ( duf_levinfo_numdir_d( pdi, du ) > 0 )
            {
              if ( d == max )
              {
                if ( !eod )
                  DUF_PRINTF( 0, ".[├─── ]" );
                else
                  DUF_PRINTF( 0, ".[ *%-3d]", __LINE__ );
              }
              else
              {
                if ( !eod )
                  DUF_PRINTF( 0, ".[│    ]" );
                else
                  DUF_PRINTF( 0, ".[ *%-3d]", __LINE__ );
              }
            }
            else if ( duf_levinfo_numdir_d( pdi, du ) == 0 )
            {
              if ( d == max )
              {
                if ( !eod )
                  DUF_PRINTF( 0, ".[└─── ]" );
                else
                  DUF_PRINTF( 0, ".[ *%-3d]", __LINE__ );
              }
              else
              {
                DUF_PRINTF( 0, ".[ *%-3d]", __LINE__ );
              }
            }
            else
            {
              if ( eod )
                DUF_PRINTF( 0, ".[ %-4d]", __LINE__ );
              else
                DUF_PRINTF( 0, ".[ %-4d]", __LINE__ );
            }
          }
        }
        else
        {
          DUF_PRINTF( 0, ".??? " );
        }
      }
    }
  }
  DEBUG_ENDR( r );
  return r;
}

/* callback of  duf_scan_callback_file_t */
__attribute__ ( ( unused ) )
     static int scan_leaf( duf_depthinfo_t * pdi, duf_record_t * precord /*, const duf_dirhandle_t * pdh_notused */  )
{
  int r = 0;

  DUF_SFIELD( filename );
  /* const char *filename = duf_sql_str_by_name( "filename", precord, 0 ); */

  DEBUG_START(  );

  {
    duf_fileinfo_t fi = {.st.st_uid = 0,.st.st_gid = 0 };
    duf_format_t format = {.filename = 0,.seq = 1 };
    /* fi.st.st_mode = ( mode_t ) filemode; */
    /* fi.st.st_ino = ( ino_t ) inode; */
    /* fi.st.st_mtim.tv_sec = mtim; */
    /* fi.st.st_mtim.tv_nsec = 0; */
    /* fi.st.st_uid = ( uid_t ) uid; */
    /* fi.st.st_gid = ( gid_t ) gid; */
    /* fi.st.st_nlink = ( nlink_t ) nlink; */
    /* fi.st.st_size = ( off_t ) filesize; */
    fi.name = filename;
    /* fi.md5sum1 = md5sum1; */
    /* fi.md5sum2 = md5sum2; */
    duf_print_file_info( stdout, pdi, &fi, &format );
  }

  r = duf_sql_print_tree_prefix_uni( pdi, 1 );

  /* SQL at duf_scan_files_by_dirid */

  DUF_PRINTF( 0, "%s", filename );

  DEBUG_ENDR( r );
  return r;
}

/*
 * this is callback of type: duf_scan_callback_dir_t (second range;):
 * */
__attribute__ ( ( unused ) )
     static int scan_node_before( unsigned long long pathid, /* const duf_dirhandle_t * pdh_notused, */ duf_depthinfo_t * pdi,
                                  duf_record_t * precord )
{
  /* DUF_SFIELD( dirname ); */

  DEBUG_START(  );

  int r = 0;

  {
    /* if ( pdi->levinfo && !nrow1 )                                             */
    /*   pdi->levinfo[pdi->depth] = nrows1;                                      */
    /* pdi->seq++;                                                              */
    /* (* DUF_PRINTF(0, "%4llu: (%7s) :", pathid, precord->presult[2] ? precord->presult[2] : "-" ); *) */
    if ( duf_config->cli.format.seq )
      DUF_PRINTF( 0, ".%8llu ", pdi->seq );
    if ( duf_config->cli.format.dirid )
      DUF_PRINTF( 0, ".[%8llu] ", pdi->levinfo[pdi->depth].dirid );
    r = duf_sql_print_tree_prefix_uni( pdi, 0 );
    {
      /* optimizing makes puts, segfault by NULL, therefore DUF_PRINTF(0, "%s\n", dirname  ); is not good */
      /* DUF_PRINTF( 0, "<<<%s>>>", dirname ? dirname : "-=No dirname=-" ); */
      DUF_PRINTF( 0, "%s", pdi->levinfo[pdi->depth].itemname );
    }
  }

  DEBUG_ENDR( r );
  return r;
}

 /* *INDENT-OFF*  */
duf_scan_callbacks_t duf_print_tree_callbacks = {
  .title = __FILE__ ".tree",
  .init_scan = NULL,
  .node_scan_before = scan_node_before,
  .leaf_scan = scan_leaf,
  .fieldset = "duf_filenames.pathid AS dirid " " , duf_filenames.name AS filename, duf_filedatas.size AS filesize "
        " , uid, gid, nlink, inode, mtim AS mtime "
        " , dupcnt AS nsame"
	" , duf_filenames.id AS filenameid" " , duf_filedatas.mode AS filemode",
  .leaf_selector =
        "SELECT %s FROM duf_filenames "
        " LEFT JOIN duf_filedatas on (duf_filenames.dataid=duf_filedatas.id) "
        " LEFT JOIN duf_md5 AS md on (md.id=duf_filedatas.md5id)"
	"    WHERE "
        /* "           duf_filedatas.size >= %llu AND duf_filedatas.size < %llu "            */
        /* "       AND (md.dupcnt IS NULL OR (md.dupcnt >= %llu AND md.dupcnt < %llu)) AND " */
        " duf_filenames.pathid='%llu' ",
  .node_selector =
        "SELECT duf_paths.id AS dirid, duf_paths.dirname, duf_paths.dirname AS dfname,  duf_paths.parentid "
        ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize "
        /* " ,(SELECT count(*) FROM duf_paths AS sp WHERE sp.parentid=duf_paths.id) AS ndirs "                   */
        /* " ,(SELECT count(*) FROM duf_filenames AS sfn "                                                       */
        /* "          JOIN duf_filedatas AS sfd ON (sfn.dataid=sfd.id) "                                         */
        /* "          JOIN duf_md5 AS smd ON (sfd.md5id=smd.id) "                                                */
        /* "            WHERE "                                                                                  */
        /* "                  sfn.pathid=duf_paths.id "                                                          */
        /* "              AND sfd.size >= %llu AND sfd.size < %llu "                                             */
        /* "              AND (smd.dupcnt IS NULL OR (smd.dupcnt >= %llu AND smd.dupcnt < %llu)) "               */
        /* " ) AS nfiles "                                                                                       */
        /* " ,(SELECT min(sfd.size) FROM duf_filedatas AS sfd JOIN duf_filenames AS sfn ON (sfn.dataid=sfd.id) " */
        /* "           WHERE sfn.pathid=duf_paths.id) AS minsize "                                               */
        /* " ,(SELECT max(sfd.size) FROM duf_filedatas AS sfd JOIN duf_filenames AS sfn ON (sfn.dataid=sfd.id) " */
        /* "           WHERE sfn.pathid=duf_paths.id) AS maxsize "                                               */
        " FROM duf_paths "
        " LEFT JOIN duf_pathtot_dirs AS td ON (td.pathid=duf_paths.id) "
        " LEFT JOIN duf_pathtot_files AS tf ON (tf.pathid=duf_paths.id) " " WHERE duf_paths.parentid='%llu' ",
  /* .final_sql_argv = final_sql, */
};
 /* *INDENT-ON*  */
