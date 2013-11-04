#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <openssl/md5.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_sql.h"
#include "duf_path.h"
/* #include "duf_file_pathid.h" */
#include "duf_file.h"
#include "duf_file_md5id.h"
#include "duf_insert.h"


/* ###################################################################### */
#include "duf_filedata.h"
/* ###################################################################### */

/* run --create-tables --zero-filedata   */
/* run --create-tables --update-filedata */


int
duf_update_filedatas_filedataid( unsigned long long filedataid, unsigned long long filenameid, const char *filename, size_t size,
                                 const char *filetype )
{
  int r = 0;
  int filestatus = 9999;
  char *filepath = NULL;

  /* size_t filepathlen = 0; */

  filepath = filenameid_to_filepath( filenameid );
  /* filepathlen = strlen( filepath ); */
  /* if ( 0 == strcmp( filepath + filepathlen - 4, ".jpg" ) || 0 == strcmp( filepath + filepathlen - 4, ".jpeg" )      */
  /*      || 0 == strcmp( filepath + filepathlen - 4, ".JPG" ) || 0 == strcmp( filepath + filepathlen - 4, ".JPEG" ) ) */
  /* {                                                                                                                 */
  /*   filetype = 1;                                                                                                   */
  /* }                                                                                                                 */
  {
    struct stat st;

    r = stat( filepath, &st );
    filestatus = r >= 0 ? 1 : -1;
  }
  /* fprintf( stderr, "UPDATE duf_filedatas SET filetype='%s', filestatus='%u' WHERE id='%llu'\n", filetype, filestatus, filedataid ); */

  fprintf( stderr, "%lld  : %s\x1b[K\r", filenameid, filepath );

  duf_sql( DUF_TRACE_NO,"UPDATE duf_filedatas SET filetype='%s', filestatus='%u' WHERE id='%llu'", filetype, filestatus, filedataid );
  mas_free( filepath );
  return r;
}

/* 
 * sql (must) select pathid, filenameid, filename, md5id, size
 * */
static int
duf_sql_update_filedatas( int nrow, int nrows, const char *const *presult, va_list args, void *sel_cb_udata, duf_str_cb_t str_cb,
                          void *str_cb_udata )
{
  unsigned long long filedataid;
  unsigned long long filenameid;
  size_t size = 0;
  const char *filename;
  const char *filetype = "NONE";

  filedataid = strtoll( presult[0], NULL, 10 );
  filenameid = strtoll( presult[1], NULL, 10 );
  filename = presult[2];
  size = strtoll( presult[4], NULL, 10 );
  filetype = presult[5];
  /* fprintf( stderr, "duf_sql_update_filedatas %d\n", nrow ); */
  duf_update_filedatas_filedataid( filedataid, filenameid, filename, size, filetype );
  return 0;
}

typedef struct
{
  const char *match;
  const char *filetype;
} duf_filetype_t;

duf_filetype_t ft_ext[] = {
  {"jpeg", "jpeg"},
  {"jpg", "jpeg"},

  {"gif", "pic"},
  {"png", "pic"},
  {"pbm", "pic"},
  {"xbm", "pic"},
  {"xcf", "pic"},
  {"pic", "pic"},
  {"tiff", "pic"},
  {"pnm", "pic"},
  {"xpm", "pic"},
  {"psd", "pic"},               /* Adobe Photoshop Image */

  {"fb2", "book"},
  {"epub", "book"},
  {"fb2.zip", "book"},
  {"fb2.gz", "book"},

  {"3gp", "video"},
  {"3gpp", "video"},
  {"mp4", "video"},
  {"mov", "video"},
  {"avi", "video"},
  {"mpg", "video"},

  {"ogg", "sound"},
  {"mp3", "sound"},
  {"m3u", "sound"},
  {"wav", "sound"},
  {"amr", "sound"},             /* Adaptive Multi-Rate Codec */

  {"vcf", "vCard"},

  {"swp", "temp"},
  {"tmp", "temp"},

  {"apk", "pkg"},
  {"pkg", "pkg"},
  {"jad", "pkg"},
  {"msi", "pkg"},
  {"jar", "pkg"},

  {"sh", "shell"},
  {"desktop", "program"},
  {"js", "program"},

  {"txt", "doc"},
  {"pdf", "doc"},
  {"htm", "doc"},
  {"html", "doc"},
  {"xml", "doc"},
  {"css", "doc"},
  {"odt", "doc"},
  {"doc", "doc"},

  {"cache", "cache"},

  {"java", "java"},
  {"java.svn-base", "java"},
  {"svn-base", "svn"},

  {"tar.gz", "archive"},
  {"gz", "archive"},
  {"tar.bz2", "archive"},
  {"bz2", "archive"},
  {"zip", "archive"},
  {"rar", "archive"},
  {"tgz", "archive"},

  {"skn", "other"},
  {"pskn", "other"},
  {"log", "other"},
  {"face", "other"},
  {"xface", "other"},
  {"db", "other"},
  {"kml", "other"},
};

duf_filetype_t ft_exact[] = {
  {"Picasa.ini", "picasa"},
  {"TRANS.TBL", "iso9660"},
  {".DS_Store", "apple"},
  {"._.DS_Store", "apple"},
  {"._.Trashes", "apple"},
  {"digikam4.db", "digikam"},
  {"digikam3.db", "digikam"},
  {"Thumbs.db", "thumbs"},
  {"thumbnails-digikam.db", "digikam"},
  {"DDISCVRY.DPS", "kodak"},
  {"LABELCTB.LCB", "kodak"},
  {"DCEMAIL.ABK", "kodak"},
  {".cvsignore", "cvs"},
  {"PhotafPanoramaConfig.cfg", "PhotafPcfg"},
  {"iconres.ezx", "ezx"},
  {".profile", "shell"},
  {".bashrc", "shell"},
  {".nomedia", "other"},
};

duf_filetype_t ft_started[] = {
  {"0.cache", "cache"},
  {"com.", "java"},
};

int
duf_update_filedatas( void )
{
  int r;

  fprintf( stderr, "Start duf_update_filedatas\n" );
  for ( int it = 0; it < sizeof( ft_ext ) / sizeof( ft_ext[0] ); it++ )
  {
    r = duf_sql_select( duf_sql_update_filedatas, SEL_CB_UDATA_DEF, STR_CB_DEF, STR_CB_UDATA_DEF, DUF_TRACE_NO,
                        "SELECT duf_filedatas.id, duf_filenames.id, duf_filenames.name, 0, duf_filedatas.size, '%s'" " FROM duf_filedatas "
                        " LEFT JOIN duf_filenames ON (duf_filenames.dataid=duf_filedatas.id) "
                        " WHERE duf_filedatas.filetype IS NULL " " AND ( duf_filenames.name LIKE '%%.%s' )" " ORDER BY duf_filedatas.id",
                        ft_ext[it].filetype, ft_ext[it].match );
  }
  for ( int it = 0; it < sizeof( ft_exact ) / sizeof( ft_exact[0] ); it++ )
  {
    r = duf_sql_select( duf_sql_update_filedatas, SEL_CB_UDATA_DEF, STR_CB_DEF, STR_CB_UDATA_DEF, DUF_TRACE_NO,
                        "SELECT duf_filedatas.id, duf_filenames.id, duf_filenames.name, 0, duf_filedatas.size, '%s'" " FROM duf_filedatas "
                        " LEFT JOIN duf_filenames ON (duf_filenames.dataid=duf_filedatas.id) "
                        " WHERE duf_filedatas.filetype IS NULL " " AND ( duf_filenames.name=='%s' )" " ORDER BY duf_filedatas.id",
                        ft_exact[it].filetype, ft_exact[it].match );
  }
  for ( int it = 0; it < sizeof( ft_started ) / sizeof( ft_started[0] ); it++ )
  {
    r = duf_sql_select( duf_sql_update_filedatas, SEL_CB_UDATA_DEF, STR_CB_DEF, STR_CB_UDATA_DEF, DUF_TRACE_NO,
                        "SELECT duf_filedatas.id, duf_filenames.id, duf_filenames.name, 0, duf_filedatas.size, '%s'" " FROM duf_filedatas "
                        " LEFT JOIN duf_filenames ON (duf_filenames.dataid=duf_filedatas.id) "
                        " WHERE duf_filedatas.filetype IS NULL " " AND ( duf_filenames.name LIKE '%s%%' )" " ORDER BY duf_filedatas.id",
                        ft_started[it].filetype, ft_started[it].match );
  }

  fprintf( stderr, "End duf_update_filedatas\n" );
  return r;
}

int
duf_zero_filedatas( void )
{
  duf_sql( DUF_TRACE_NO,"UPDATE duf_filedatas SET filetype=NULL, filestatus='99999'" );
  return 0;
}

/*
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 * */
static int
duf_sql_insert_filedata( int nrow, int nrows, const char *const *presult, va_list args, void *sel_cb_udata, duf_str_cb_t str_cb,
                         void *str_cb_udata )
{
  unsigned long long *presd;

  presd = ( unsigned long long * ) sel_cb_udata;
  *presd = strtol( presult[0], NULL, 10 );
  return 0;
}

unsigned long long
duf_insert_filedata( unsigned long long file_inode, const struct stat *pst_dir, const struct stat *pst_file )
{
  int r;
  unsigned long long resd;

  r = duf_sql_c( DUF_TRACE_NO,"INSERT INTO duf_filedatas (dev,inode,size,md5id,ucnt,now) values ('%lu','%lu','%lu','%lu',0,datetime())",
                 DUF_CONSTRAINT_IGNORE_YES, pst_dir->st_dev, file_inode, ( pst_file ? pst_file->st_size : 0 ) /* size */ , 0 /* md5id */  );
  if ( r == duf_constraint )
    r = duf_sql_select( duf_sql_insert_filedata, &resd, STR_CB_DEF, STR_CB_UDATA_DEF, DUF_TRACE_NO,
                        "SELECT id FROM duf_filedatas WHERE dev='%lu' and inode='%lu'", pst_dir->st_dev, file_inode );
  else if ( !r /* assume SQLITE_OK */  )
    resd = duf_last_insert_rowid(  );
  else
    fprintf( stderr, "error duf_insert_filedata %d\n", r );
  return resd;
}
