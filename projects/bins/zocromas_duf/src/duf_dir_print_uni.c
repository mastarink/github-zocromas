#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */
#include <sys/stat.h>

#include <openssl/md5.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"
#include "duf_config.h"

/* #include "duf_sql.h" */
/* #include "duf_utils.h" */
#include "duf_path.h"
/* #include "duf_file.h" */
/* #include "duf_file_pathid.h" */
#include "duf_file_scan.h"

#include "duf_sql.h"

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_dir_print_uni.h"
/* ###################################################################### */


static int
duf_sql_print_tree_prefix_uni( duf_dirinfo_t * pdi, int file )
{
  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  if ( pdi )
    for ( int i = 1; i <= pdi->level; i++ )
    {
      if ( pdi->levinfo )
      {
        /* printf( "%u▢  ", pdi->level ); */
        if ( i == 1 )
          printf( "%3d.", pdi->seq );
        if ( file && i >= pdi->level )
        {
          printf( " ◇ " );
        }
        else if ( pdi->levinfo[i] == 1 && i == pdi->level )
        {
          /* printf( "└─ %3d%3d%3d ", i, pdi->level, pdi->levinfo[i] ); */
          printf( "└── " );
        }
        else if ( pdi->levinfo[i] && i == pdi->level )
        {
          /* printf( "├─ %3d%3d%3d ", i, pdi->level, pdi->levinfo[i] ); */
          printf( "├── " );
        }
        else if ( file && pdi->levinfo[i] == 1 )
        {
          /* printf( "   %3d%3d%3d ", i, pdi->level, pdi->levinfo[i] ); */
          printf( "    " );
        }
        else if ( pdi->levinfo[i] && pdi->levinfo[i] != 1 )
        {
          /* printf( "│  %3d%3d%3d ", i, pdi->level, pdi->levinfo[i] ); */
          printf( "│   " );
        }
        else
        {
          printf( "    " );
        }
      }
      else
      {
        printf( "??? " );
      }
    }
  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}

/* callback of  duf_scan_callback_file_t */
static int
duf_sql_uni_scan_print_files_plain_uni( unsigned long long pathid, unsigned long long filenameid, const char *name, void *str_cb_udata,
                                        duf_dirinfo_t * pdi, struct duf_scan_callbacks_s *sccb, duf_record_t * precord )
{
  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  /* duf_sql_print_tree_prefix_uni( pdi, 1 ); */

  /* SQL at duf_scan_files_by_pathid */
  unsigned long long filesize = duf_sql_ull_by_name( "filesize", precord, 0 );
  unsigned long long filemode = duf_sql_ull_by_name( "filemode", precord, 0 );
  struct stat st;

  unsigned long long md5a;
  unsigned long long md5b;


  char modebuf[] = "----------";
  char *pmode = modebuf;

  md5a = duf_sql_ull_by_name( "md5sum1", precord, 0 );
  md5b = duf_sql_ull_by_name( "md5sum2", precord, 0 );


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
  printf( "%s> %9llu %016llx%016llx %s\n", modebuf, filesize, md5a, md5b, name );

  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}

/* callback of  duf_scan_callback_file_t */
static int
duf_sql_uni_scan_print_files_tree_uni( unsigned long long pathid, unsigned long long filenameid, const char *name, void *str_cb_udata,
                                       duf_dirinfo_t * pdi, struct duf_scan_callbacks_s *sccb, duf_record_t * precord )
{
  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  duf_sql_print_tree_prefix_uni( pdi, 1 );

  /* SQL at duf_scan_files_by_pathid */

  /* printf( "# %s\n", duf_sql_str_by_name( "filename", precord ) ); */
  printf( "%s\n", name );

  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}


static int
duf_sql_scan_print_dir_uni( unsigned long long pathid, const char *name, unsigned long long items,
                            duf_dirinfo_t * pdi, struct duf_scan_callbacks_s *sccb )
{
  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  if ( pdi && ( !pdi->u.maxseq || pdi->seq < pdi->u.maxseq ) )
  {
    char *path = duf_pathid_to_path( pathid );

    printf( "%s\n", path );
    mas_free( path );
  }

  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}

/*
 * this is callback of type: duf_scan_callback_dir_t (second range):
 * */
static int
duf_sql_scan_print_tree_uni( unsigned long long pathid, const char *name, unsigned long long items,
                             duf_dirinfo_t * pdi, struct duf_scan_callbacks_s *sccb )
{
  duf_dbgfunc( DBG_START, __func__, __LINE__ );

  if ( pdi && ( !pdi->u.maxseq || pdi->seq < pdi->u.maxseq ) )
  {
    /* if ( pdi->levinfo && !nrow1 )                                             */
    /*   pdi->levinfo[pdi->level] = nrows1;                                      */
    /* pdi->seq++;                                                              */
    /* (* printf( "%4llu: (%7s) :", pathid, precord->presult[2] ? precord->presult[2] : "-" ); *) */
    duf_sql_print_tree_prefix_uni( pdi, 0 );
    printf( "%s\n", name );
  }

  duf_dbgfunc( DBG_END, __func__, __LINE__ );
  return 0;
}

duf_scan_callbacks_t duf_print_tree_callbacks = {
  .fieldset = "duf_filenames.pathid, duf_filenames.name as filename",
  .init_scan = NULL,
  .directory_scan = duf_sql_scan_print_tree_uni,
  .file_scan = duf_sql_uni_scan_print_files_tree_uni,
};

duf_scan_callbacks_t duf_print_dir_callbacks = {
  .fieldset =
        "duf_filenames.pathid, duf_filenames.name as filename, duf_filedatas.size as filesize, "
        "duf_filedatas.mode as filemode, duf_md5.md5sum1, duf_md5.md5sum2",
  .init_scan = NULL,
  .directory_scan = duf_sql_scan_print_dir_uni,
  .file_scan = duf_sql_uni_scan_print_files_plain_uni,
};
