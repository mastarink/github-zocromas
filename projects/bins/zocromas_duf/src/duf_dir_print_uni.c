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
/* #include "duf_dir_print_uni.h" */
/* ###################################################################### */


static int
duf_sql_print_tree_prefix_uni( duf_dirinfo_t * pdi, int is_file )
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

        /* if ( !pdi->levinfo[i].eod )                                                      */
        /*   draw = 1;                                                                      */
        /* if ( pdi->levinfo[i].ndirs == 0 )                                                */
        /*   angle = !is_file;                                                              */
        /* if ( 1 )                                                                         */
        /* {                                                                                */
        /*   printf( "(%2d:%3lld:%2u)   ", i, pdi->levinfo[i].ndirs, pdi->levinfo[i].eod ); */
        /* }                                                                                */
        /* else                                                                             */
        /* {                                                                                */
        /*   if ( draw )                                                                    */
        /*   {                                                                              */
        /*     if ( angle )                                                                 */
        /*     {                                                                            */
        /*       printf( "└── " );                                                    */
        /*     }                                                                            */
        /*     else                                                                         */
        /*     {                                                                            */
        /*       if ( is_file )                                                             */
        /*       {                                                                          */
        /*         printf( "│ ◇ " );                                                    */
        /*       }                                                                          */
        /*       else                                                                       */
        /*       {                                                                          */
        /*         printf( "│   " );                                                      */
        /*       }                                                                          */
        /*     }                                                                            */
        /*   }                                                                              */
        /*   else                                                                           */
        /*   {                                                                              */
        /*     if ( is_file )                                                               */
        /*     {                                                                            */
        /*       printf( "  ◇ " );                                                        */
        /*     }                                                                            */
        /*     else                                                                         */
        /*     {                                                                            */
        /*       printf( "    " );                                                          */
        /*     }                                                                            */
        /*   }                                                                              */
        /* }                                                                                */
        /* (* printf( "%u▢  ", pdi->depth ); *)                                   */
        /* if ( is_file && i >= pdi->depth && pdi->levinfo[i] >= 0 )                */
        /* {                                                                        */
        /*   printf( "%u:%2d│ ◇ ", i, pdi->levinfo[i] );                        */
        /* }                                                                        */
        /* else if ( is_file && i >= pdi->depth && !pdi->levinfo[i] )               */
        /* {                                                                        */
        /*   printf( "%u:%2d  ◇ ", i, pdi->levinfo[i] );                          */
        /* }                                                                        */
        /* else if ( !pdi->levinfo[i] && i == pdi->depth )                          */
        /* {                                                                        */
        /*   (* printf( "└─ %3d%3d%3d ", i, pdi->depth, pdi->levinfo[i] ); *)   */
        /*   printf( "%u:%2d└── ", i, pdi->levinfo[i] );                      */
        /* }                                                                        */
        /* else if ( pdi->levinfo[i] > 0 && i == pdi->depth )                       */
        /* {                                                                        */
        /*   (* printf( "├─ %3d%3d%3d ", i, pdi->depth, pdi->levinfo[i] ); *)   */
        /*   printf( "%u:%2d├── ", i, pdi->levinfo[i] );                      */
        /* }                                                                        */
        /* (* else if ( is_file && !pdi->levinfo[i] )                            *) */
        /* (* {                                                                  *) */
        /* (*   (* printf( "   %3d%3d%3d ", i, pdi->depth, pdi->levinfo[i] ); *) *) */
        /* (*   printf( "%u:%2d    ", i, pdi->levinfo[i] );                      *) */
        /* (* }                                                                  *) */
        /* else if ( pdi->levinfo[i] && pdi->levinfo[i] > 0 )                       */
        /* {                                                                        */
        /*   (* printf( "│  %3d%3d%3d ", i, pdi->depth, pdi->levinfo[i] ); *)     */
        /*   printf( "%u:%2d│   ", i, pdi->levinfo[i] );                          */
        /* }                                                                        */
        /* else                                                                     */
        /* {                                                                        */
        /*   printf( "%u:%2d    ", i, pdi->levinfo[i] );                            */
        /* }                                                                        */
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
duf_file_scan_print_plain_uni( void *str_cb_udata, duf_dirinfo_t * pdi, duf_record_t * precord )
{
  int r = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  /* SQL at duf_scan_files_by_dirid */
  DUF_SFIELD( filename );
  /* const char *filename = duf_sql_str_by_name( "filename", precord, 0 ); */
  DUF_UFIELD( filesize );
  /* unsigned long long filesize = duf_sql_ull_by_name( "filesize", precord, 0 ); */
  DUF_UFIELD( filemode );
  /* unsigned long long filemode = duf_sql_ull_by_name( "filemode", precord, 0 ); */
  DUF_UFIELD( md5sum1 );
  /* unsigned long long md5a = duf_sql_ull_by_name( "md5sum1", precord, 0 ); */
  DUF_UFIELD( md5sum2 );
  /* unsigned long long md5b = duf_sql_ull_by_name( "md5sum2", precord, 0 ); */
  DUF_UFIELD( mtime );
  DUF_UFIELD( uid );
  DUF_UFIELD( gid );
  DUF_UFIELD( nlink );
  DUF_UFIELD( inode );
  /* DUF_SFIELD( mtimef ); */
  /* DUF_SFIELD( dowmtime ); */
  /* DUF_SFIELD( monthmtime ); */

  struct stat st;
  char modebuf[] = "----------";
  char *pmode = modebuf;
  time_t mtimet;
  char mtimes[128];
  struct tm mtimetm, *pmtimetm;

  mtimet = ( time_t ) mtime;
  pmtimetm = localtime_r( &mtimet, &mtimetm );
  strftime( mtimes, sizeof( mtimes ), "%b %d %Y %H:%M:%S", pmtimetm );
  st.st_mode = ( mode_t ) filemode;
  pmode++;
  if ( S_IRUSR & st.st_mode )
  {
    *pmode = 'r';
  }
  pmode++;
  if ( S_IWUSR & st.st_mode )
  {
    *pmode = 'w';
  }
  pmode++;
  if ( S_IXUSR & st.st_mode )
  {
    *pmode = 'x';
  }

  pmode++;
  if ( S_IRGRP & st.st_mode )
  {
    *pmode = 'r';
  }
  pmode++;
  if ( S_IWGRP & st.st_mode )
  {
    *pmode = 'w';
  }
  pmode++;
  if ( S_IXGRP & st.st_mode )
  {
    *pmode = 'x';
  }

  pmode++;
  if ( S_IROTH & st.st_mode )
  {
    *pmode = 'r';
  }
  pmode++;
  if ( S_IWOTH & st.st_mode )
  {
    *pmode = 'w';
  }
  pmode++;
  if ( S_IXOTH & st.st_mode )
  {
    *pmode = 'x';
  }

  /* printf( "> %s\n", duf_sql_str_by_name( "filename", precord ) ); */
  /* printf( "-rw-------  1 mastar mastar-firefox 106580068 Jan 27 2014 12:35:27 sample_video_hd.zip\n" ); */
  /* duf_sql_print_tree_prefix_uni( pdi, 1 ); */
  printf( "%8llu %9llu %s %llu%6llu%6llu  %7llu %s %-30s :: %016llx%016llx\n", pdi->seq, inode, modebuf, nlink, uid, gid,
          filesize, mtimes, filename, md5sum1, md5sum2 );

  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}

/* callback of  duf_scan_callback_file_t */
static int
duf_file_scan_print_tree_uni( void *str_cb_udata, duf_dirinfo_t * pdi, duf_record_t * precord )
{
  int r = 0;

  DUF_SFIELD( filename );
  /* const char *filename = duf_sql_str_by_name( "filename", precord, 0 ); */

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  printf( "%8llu [%8llu] ", pdi->seq, pdi->levinfo[pdi->depth].dirid );
  r = duf_sql_print_tree_prefix_uni( pdi, 1 );

  /* SQL at duf_scan_files_by_dirid */

  printf( "%s\n", filename );

  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return r;
}


static int
duf_directory_scan_print_plain_uni( unsigned long long pathid, unsigned long long items, duf_dirinfo_t * pdi, duf_record_t * precord )
{
  int r = 0;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  {
    char *path = duf_pathid_to_path( pathid );

    {
      /* optimizing makes puts, segfault by NULL, therefore printf( "%s\n", path  ); is not good */
      printf( "%8llu %s\n", pdi->seq, path ? path : "-=No path=-" );
    }
    mas_free( path );
  }

  duf_dbgfunc( DBG_END, __func__, __LINE__ );

  return r;
}

/*
 * this is callback of type: duf_scan_callback_dir_t (second range;):
 * */
static int
duf_directory_scan_print_tree_uni( unsigned long long pathid, unsigned long long items, duf_dirinfo_t * pdi, duf_record_t * precord )
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
    printf( "%8llu [%8llu] ", pdi->seq, pdi->levinfo[pdi->depth].dirid );
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
  .fieldset = "duf_filenames.pathid as dirid, " " duf_filenames.name as filename, duf_filedatas.size as filesize ",
  .file_selector =
        "SELECT %s FROM duf_filenames " " LEFT JOIN duf_filedatas on (duf_filenames.dataid=duf_filedatas.id) "
        " LEFT JOIN duf_md5 on (duf_md5.id=duf_filedatas.md5id)" " WHERE duf_filenames.pathid='%llu'",
  .dir_selector =
        "SELECT duf_paths.id as dirid, duf_paths.dirname, duf_paths.items, duf_paths.parentid "
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

duf_scan_callbacks_t duf_print_dir_callbacks = {
  .title = __FILE__ ".dir",
  .init_scan = NULL,
  .directory_scan_before = duf_directory_scan_print_plain_uni,
  .file_scan = duf_file_scan_print_plain_uni,
  .fieldset =
        "duf_filenames.pathid as dirid, "
        " duf_filenames.name as filename, duf_filedatas.size as filesize " ", duf_filedatas.size as filesize "
        ", uid, gid, nlink, inode, mtim as mtime "
        /* " , datetime(mtim, 'unixepoch') as mtimef " */
        /* ", strftime('%Y-%m-%d %H:%M:%S',mtim,'unixepoch') as mtimef " */
        /* ", case cast (strftime('%w', mtim,'unixepoch') as integer) "                                                                   */
        /* " when 0 then 'Sun' when 1 then 'Mon' when 2 then 'Tue' when 3 then 'Wed' "                                                    */
        /* " when 4 then 'Thu' when 5 then 'Fri' else 'Sat' end as dowmtime, " "case cast (strftime('%m', mtim,'unixepoch') as integer) " */
        /* " when 1 then 'Jan' when 2 then 'Feb' when 3 then 'Mar' when 4 then 'Apr' when 5 then 'May' when 6 then "                      */
        /* " 'Jun' when 7 then 'Jul' when 8 then 'Aug' when 9 then 'Sep' when 10 then 'Oct' when 11 then 'Nov' when 12 then 'Dec' "       */
        /* " else 'Wow' end as monthmtime "                                                                                               */
        ", duf_filedatas.mode as filemode, duf_md5.md5sum1, duf_md5.md5sum2 ",
  .file_selector =
        "SELECT %s FROM duf_filenames " " LEFT JOIN duf_filedatas on (duf_filenames.dataid=duf_filedatas.id) "
        " LEFT JOIN duf_md5 on (duf_md5.id=duf_filedatas.md5id) " " WHERE duf_filenames.pathid='%llu' ",
  .dir_selector =
        "SELECT duf_paths.id as dirid, duf_paths.dirname, duf_paths.items, duf_paths.parentid "
        " ,(SELECT count(*) FROM duf_paths as subpaths WHERE subpaths.parentid=duf_paths.id) as ndirs "
        " ,(SELECT count(*) FROM duf_filenames as subfilenames WHERE subfilenames.pathid=duf_paths.id) as nfiles "
        " FROM duf_paths " " WHERE duf_paths.parentid='%llu' ",
};
