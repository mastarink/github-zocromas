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

/* #include "duf_sql_def.h" */
/* #include "duf_sql.h" */
#include "duf_sql_field.h"

/* #include "duf_utils.h" */
#include "duf_path.h"
/* #include "duf_file.h" */
/* #include "duf_file_pathid.h" */
#include "duf_file_scan.h"

#include "duf_sql.h"

#include "duf_dbg.h"

/* ###################################################################### */
/* #include "duf_tree_print_uni.h" */
/* ###################################################################### */


static int
duf_sql_print_tree_prefix_uni( duf_depthinfo_t * pdi, int is_file )
{
  int r = 0;
  int max = pdi->depth - 1;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  if ( pdi )

    for ( int i = 0; i <= max; i++ )
    {
      if ( pdi->levinfo )
      {
        if ( pdi->levinfo[i].ndirs > 0 )
        {
          if ( i == max )
          {
            if ( is_file )
              printf( "│ ◇ " );
            else if ( !pdi->levinfo[i].eod )
              printf( "├─── " );
          }
          else
          {
            if ( is_file )
              printf( "│    " );
            else if ( !pdi->levinfo[i].eod )
              printf( "│    " );
          }
        }
        else if ( pdi->levinfo[i].ndirs == 0 )
        {
          if ( i == max )
          {
            if ( is_file )
              printf( "  ◇ " );
            else if ( !pdi->levinfo[i].eod )
              printf( "└─── " );
          }
          else
          {
            printf( "     " );
          }
        }
        else
        {
          if ( is_file )
            printf( " %3d", __LINE__ );
          else if ( pdi->levinfo[i].eod )
            printf( " %3d", __LINE__ );
          else
            printf( " %3d", __LINE__ );
        }
      }
      else
      {
        printf( "??? " );
      }
    }
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}

/* callback of  duf_scan_callback_file_t */
static int
duf_file_scan_print_tree_uni( void *str_cb_udata, duf_depthinfo_t * pdi, duf_record_t * precord )
{
  int r = 0;

  DUF_SFIELD( filename );
  /* const char *filename = duf_sql_str_by_name( "filename", precord, 0 ); */

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

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

  printf( "%s\n", filename );

  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}

/*
 * this is callback of type: duf_scan_callback_dir_t (second range;):
 * */
static int
duf_directory_scan_print_tree_uni( unsigned long long pathid, duf_dirhandle_t *pdh, duf_depthinfo_t * pdi, duf_record_t * precord )
{
  DUF_SFIELD( dirname );
  /* const char *dirname = duf_sql_str_by_name( "dirname", precord, 0 ); */

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  int r = 0;

  {
    /* if ( pdi->levinfo && !nrow1 )                                             */
    /*   pdi->levinfo[pdi->depth] = nrows1;                                      */
    /* pdi->seq++;                                                              */
    /* (* printf( "%4llu: (%7s) :", pathid, precord->presult[2] ? precord->presult[2] : "-" ); *) */
    if ( duf_config->cli.format.seq )
      printf( "%8llu ", pdi->seq );
    if ( duf_config->cli.format.dirid )
      printf( "[%8llu] ", pdi->levinfo[pdi->depth].dirid );
    r = duf_sql_print_tree_prefix_uni( pdi, 0 );
    {
      /* optimizing makes puts, segfault by NULL, therefore printf( "%s\n", dirname  ); is not good */
      printf( "%s\n", dirname ? dirname : "-=No dirname=-" );
    }
  }

  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}

duf_scan_callbacks_t duf_print_tree_callbacks = {
  .title = __FILE__ ".tree",
  .init_scan = NULL,
  .directory_scan_before = duf_directory_scan_print_tree_uni,
  .file_scan = duf_file_scan_print_tree_uni,
  .fieldset = "duf_filenames.pathid as dirid, duf_filenames.name as filename, duf_filedatas.size as filesize ",
  .file_selector =
        "SELECT %s FROM duf_filenames " " LEFT JOIN duf_filedatas on (duf_filenames.dataid=duf_filedatas.id) "
        " LEFT JOIN duf_md5 on (duf_md5.id=duf_filedatas.md5id)" " WHERE duf_filenames.pathid='%llu'",
  .dir_selector =
        "SELECT duf_paths.id as dirid, duf_paths.dirname, duf_paths.dirname as dfname, duf_paths.items, duf_paths.parentid "
        " ,(SELECT count(*) FROM duf_paths as subpaths WHERE subpaths.parentid=duf_paths.id)  as ndirs "
        " ,(SELECT count(*) FROM duf_filenames as subfilenames WHERE subfilenames.pathid=duf_paths.id) as nfiles "
        /* ", count(subpaths.id) as ndirs "       */
        /* ", count(duf_filenames.id) as nfiles " */
        " FROM duf_paths "
        /* "LEFT JOIN  duf_filenames ON (duf_filenames.pathid=duf_paths.id) "      */
        /* "LEFT JOIN  duf_paths as subpaths ON (duf_paths.id=subpaths.parentid) " */
        " WHERE duf_paths.parentid='%llu' "
        /* " GROUP BY duf_paths.id " */
        ,
};
