#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"
#include "duf_config.h"
#include "duf_utils.h"
#include "duf_service.h"


#include "duf_sql_def.h"
#include "duf_sql.h"
#include "duf_sql_field.h"

#include "duf_file.h"

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_filedata_expire.h"
/* ###################################################################### */

/* run --create-tables --zero-filedata   */
/* run --create-tables --update-filedata */


int
duf_update_filedatas_filedataid( unsigned long long filedataid, unsigned long long filenameid, const char *filename, size_t size,
                                 const char *filetype )
{
  int r = 0;
  int filestatus = DUF_ERROR_STAT;
  char *filepath = NULL;

  /* size_t filepathlen = 0; */

  filepath = filenameid_to_filepath( filenameid, NULL, &r );
  /* filepathlen = strlen( filepath ); */
  /* if ( 0 == strcmp( filepath + filepathlen - 4, ".jpg" ) || 0 == strcmp( filepath + filepathlen - 4, ".jpeg" )      */
  /*      || 0 == strcmp( filepath + filepathlen - 4, ".JPG" ) || 0 == strcmp( filepath + filepathlen - 4, ".JPEG" ) ) */
  /* {                                                                                                                 */
  /*   filetype = 1;                                                                                                   */
  /* }                                                                                                                 */
  {
    struct stat st;

    r = stat( filepath, &st );
    filestatus = r >= 0 ? 1 : r;
  }
  fprintf( stderr, "UPDATE duf_filedatas SET filetype='%s', filestatus='%u' WHERE id='%llu'\n", filetype, filestatus, filedataid );

  fprintf( stderr, "%lld  : %s\x1b[K\r", filenameid, filepath );

  duf_sql( "UPDATE duf_filedatas SET filetype='%s', filestatus='%u' WHERE id='%llu'", ( int * ) NULL, filetype, filestatus, filedataid );
  mas_free( filepath );
  return r;
}

/* 
 * sql (must) select pathid, filenameid, filename, md5id, size
 * */
static int
duf_sel_cb_update_filedatas( duf_record_t * precord, va_list args, void *sel_cb_udata, duf_scan_callback_file_t str_cb, void *str_cb_udata,
                             duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, const duf_dirhandle_t * pdhu )
{
  unsigned long long filedataid;
  unsigned long long filenameid;
  size_t size = 0;
  const char *filename;
  const char *filetype = "NONE";

  filedataid = strtoll( precord->presult[0], NULL, 10 );
  filenameid = strtoll( precord->presult[1], NULL, 10 );
  filename = precord->presult[2];
  size = strtoll( precord->presult[4], NULL, 10 );
  filetype = precord->presult[5];
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

  if ( duf_config->cli.dbg.verbose )
    fprintf( stderr, "Start duf_update_filedatas\n" );
  for ( int it = 0; it < sizeof( ft_ext ) / sizeof( ft_ext[0] ); it++ )
  {
    r = duf_sql_select( duf_sel_cb_update_filedatas, SEL_CB_UDATA_DEF, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_depthinfo_t * ) NULL,
                        ( duf_scan_callbacks_t * ) NULL /*  sccb */ , ( const duf_dirhandle_t * ) NULL,
                        "SELECT duf_filedatas.id AS dataid, duf_filenames.id AS filenameid, duf_filenames.name AS filename, "
                        " 0, duf_filedatas.size AS filesize, '%s'" " FROM duf_filedatas "
                        " LEFT JOIN duf_filenames ON (duf_filenames.dataid=duf_filedatas.id) " " WHERE duf_filedatas.filetype IS NULL "
                        " AND ( duf_filenames.name LIKE '%%.%s' )" " ORDER BY duf_filedatas.id", ft_ext[it].filetype, ft_ext[it].match );
  }
  for ( int it = 0; it < sizeof( ft_exact ) / sizeof( ft_exact[0] ); it++ )
  {
    r = duf_sql_select( duf_sel_cb_update_filedatas, SEL_CB_UDATA_DEF, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_depthinfo_t * ) NULL,
                        ( duf_scan_callbacks_t * ) NULL /*  sccb */ , ( const duf_dirhandle_t * ) NULL,
                        "SELECT duf_filedatas.id AS dataid, duf_filenames.id AS filenameid, duf_filenames.name AS filename, "
                        " 0, duf_filedatas.size AS filesize, '%s'" " FROM duf_filedatas "
                        " LEFT JOIN duf_filenames ON (duf_filenames.dataid=duf_filedatas.id) " " WHERE duf_filedatas.filetype IS NULL "
                        " AND ( duf_filenames.name=='%s' )" " ORDER BY duf_filedatas.id", ft_exact[it].filetype, ft_exact[it].match );
  }
  for ( int it = 0; it < sizeof( ft_started ) / sizeof( ft_started[0] ); it++ )
  {
    r = duf_sql_select( duf_sel_cb_update_filedatas, SEL_CB_UDATA_DEF, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_depthinfo_t * ) NULL,
                        ( duf_scan_callbacks_t * ) NULL /*  sccb */ , ( const duf_dirhandle_t * ) NULL,
                        "SELECT duf_filedatas.id AS dataid, duf_filenames.id AS filenameid, duf_filenames.name AS filename, "
                        " 0, duf_filedatas.size AS filesize, '%s'" " FROM duf_filedatas "
                        " LEFT JOIN duf_filenames ON (duf_filenames.dataid=duf_filedatas.id) " " WHERE duf_filedatas.filetype IS NULL "
                        " AND ( duf_filenames.name LIKE '%s%%' )" " ORDER BY duf_filedatas.id", ft_started[it].filetype,
                        ft_started[it].match );
  }

  if ( duf_config->cli.dbg.verbose )
    fprintf( stderr, "End duf_update_filedatas\n" );
  return r;
}

int
duf_zero_filedatas( void )
{
  duf_sql( "UPDATE duf_filedatas SET filetype=NULL, filestatus='99999'", ( int * ) NULL );
  return 0;
}
