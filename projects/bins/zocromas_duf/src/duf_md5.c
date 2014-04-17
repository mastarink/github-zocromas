#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

/* #define MD5_BSD */
#ifdef MD5_BSD
#  include <sys/types.h>
#  include <bsd/md5.h>
#else
#  include <openssl/md5.h>
#endif

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"

#include "duf_sql.h"
#include "duf_path.h"
/* #include "duf_keydata.h" */
/* #include "duf_file_pathid.h" */
#include "duf_file_md5id.h"
#include "duf_insert.h"


/* ###################################################################### */
#include "duf_md5.h"
/* ###################################################################### */




/* static unsigned long long                                                                                       */
/* duf_update_md5_file( const char *fpath, unsigned long long filedataid, size_t fsize )                           */
/* {                                                                                                               */
/*   unsigned long long resmd = 0;                                                                                 */
/*                                                                                                                 */
/*   if ( fpath )                                                                                                  */
/*   {                                                                                                             */
/* #ifdef MD5_BSD                                                                                                  */
/*     {                                                                                                           */
/*       char md[MD5_DIGEST_LENGTH];                                                                               */
/*                                                                                                                 */
/*       memset( &md, 0, sizeof( md ) );                                                                           */
/*       MD5File( fpath, md );                                                                                     */
/*       {                                                                                                         */
/*         resmd = duf_insert_md5( ( ( unsigned long long * ) &md[0] ), fsize );                                   */
/*         if ( 1 )                                                                                                */
/*         {                                                                                                       */
/*           fprintf( stderr, "resmd: %llu: %s\x1b[K\r", resmd, fpath );                                           */
/*         }                                                                                                       */
/*         if ( resmd )                                                                                            */
/*         {                                                                                                       */
/*           duf_sql( "UPDATE duf_filedatas SET md5id='%llu', ucnt=ucnt+1 WHERE id='%lld'", resmd, filedataid );   */
/*         }                                                                                                       */
/*       }                                                                                                         */
/*     }                                                                                                           */
/* #else                                                                                                           */
/*     {                                                                                                           */
/*       FILE *f;                                                                                                  */
/*                                                                                                                 */
/*       f = fopen( fpath, "r" );                                                                                  */
/*       if ( f )                                                                                                  */
/*       {                                                                                                         */
/*         MD5_CTX ctx;                                                                                            */
/*         unsigned char md[MD5_DIGEST_LENGTH];                                                                    */
/*         char *buffer;                                                                                           */
/*         size_t bufsz = 1024 * 1024 * 10;                                                                        */
/*                                                                                                                 */
/*         memset( &md, 0, sizeof( md ) );                                                                         */
/*         buffer = mas_malloc( bufsz );                                                                           */
/*         MD5_Init( &ctx );                                                                                       */
/*         while ( !feof( f ) )                                                                                    */
/*         {                                                                                                       */
/*           int r;                                                                                                */
/*                                                                                                                 */
/*           r = fread( buffer, 1, bufsz, f );                                                                     */
/*           MD5_Update( &ctx, buffer, r );                                                                        */
/*         }                                                                                                       */
/*         fclose( f );                                                                                            */
/*         mas_free( buffer );                                                                                     */
/*         MD5_Final( md, &ctx );                                                                                  */
/*         {                                                                                                       */
/*           resmd = duf_insert_md5( ( ( unsigned long long * ) &md[0] ), fsize );                                 */
/*           if ( 1 )                                                                                              */
/*           {                                                                                                     */
/*             fprintf( stderr, "resmd: %llu: %s\x1b[K\r", resmd, fpath );                                         */
/*           }                                                                                                     */
/*           if ( resmd )                                                                                          */
/*           {                                                                                                     */
/*             duf_sql( "UPDATE duf_filedatas SET md5id='%llu', ucnt=ucnt+1 WHERE id='%lld'", resmd, filedataid ); */
/*           }                                                                                                     */
/*         }                                                                                                       */
/*       }                                                                                                         */
/*       else                                                                                                      */
/*       {                                                                                                         */
/*         (* TODO : file deleted ... *)                                                                           */
/*         fprintf( stderr, "ERROR open file : %s\n", fpath );                                                     */
/*       }                                                                                                         */
/*     }                                                                                                           */
/* #endif                                                                                                          */
/*   }                                                                                                             */
/*   else                                                                                                          */
/*   {                                                                                                             */
/*     fprintf( stderr, "ERROR file not set\n" );                                                                  */
/*   }                                                                                                             */
/*   (* fprintf( stderr, "\n" ); *)                                                                                */
/*   return resmd;                                                                                                 */
/* }                                                                                                               */

/* 
 * sql must select pathid, filenameid, filename, md5id, size
 * duf_sql_select_cb_t:
 * */
/* static int                                                                                                                                 */
/* duf_sql_update_md5( duf_record_t * precord, va_list args, void *sel_cb_udata,                                                              */
/*                     duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb )                */
/* {                                                                                                                                          */
/*   unsigned long long pathid = -1, old_pathid = -1, filenameid = -1;                                                                        */
/*   char **ppath;                                                                                                                            */
/*   const char *fname;                                                                                                                       */
/*   unsigned long long filedataid;                                                                                                           */
/*   size_t fsize = 0;                                                                                                                        */
/*   unsigned long long resmd = -1;                                                                                                           */
/*   ino_t inode;                                                                                                                             */
/*                                                                                                                                            */
/*   ppath = ( char ** ) sel_cb_udata;                                                                                                        */
/*   (* fprintf(stderr, "@@@@@@@@@@@ %u [[%s]] [%s]\x1b[K\n", column, presult[ 7], presult[ 8]); *)                                           */
/*   filedataid = strtoll( precord->presult[0], NULL, 10 );                                                                                   */
/*   inode = strtoll( precord->presult[2], NULL, 10 );                                                                                        */
/*   if ( precord->presult[3] )                                                                                                               */
/*     resmd = strtoll( precord->presult[3], NULL, 10 );                                                                                      */
/*   if ( precord->presult[4] )                                                                                                               */
/*     fsize = strtoll( precord->presult[4], NULL, 10 );                                                                                      */
/*   if ( precord->presult[5] )                                                                                                               */
/*     filenameid = strtoll( precord->presult[5], NULL, 10 );                                                                                 */
/*   else                                                                                                                                     */
/*   {                                                                                                                                        */
/*     fprintf( stderr, "duf_filenames record absent for duf_filedatas.id=duf_filenames.dataid=%lld\n", filedataid );                         */
/*   }                                                                                                                                        */
/*   if ( precord->presult[6] )                                                                                                               */
/*     pathid = strtoll( precord->presult[6], NULL, 10 );                                                                                     */
/*   else                                                                                                                                     */
/*   {                                                                                                                                        */
/*     fprintf( stderr, "%s;%s;%s;%s;%s;%s;%s;\n", precord->presult[0], precord->presult[1], precord->presult[2], precord->presult[3],        */
/*              precord->presult[4], precord->presult[5], precord->presult[6] );                                                              */
/*   }                                                                                                                                        */
/*   fname = precord->presult[7];                                                                                                             */
/*   if ( fname )                                                                                                                             */
/*   {                                                                                                                                        */
/*     const char *snow;                                                                                                                      */
/*     double dnow = 0;                                                                                                                       */
/*     time_t now = 0;                                                                                                                        */
/*                                                                                                                                            */
/*     snow = precord->presult[8];                                                                                                            */
/*     if ( snow )                                                                                                                            */
/*     {                                                                                                                                      */
/*       sscanf( snow, "%lg", &dnow );                                                                                                        */
/*       now = ( time_t ) dnow;                                                                                                               */
/*     }                                                                                                                                      */
/*     if ( old_pathid != pathid || !*ppath )                                                                                                 */
/*     {                                                                                                                                      */
/*       if ( *ppath )                                                                                                                        */
/*         mas_free( *ppath );                                                                                                                */
/*       *ppath = duf_pathid_to_path( pathid );                                                                                               */
/*     }                                                                                                                                      */
/*     if ( *ppath && fsize )                                                                                                                 */
/*     {                                                                                                                                      */
/*       int rs;                                                                                                                              */
/*       char *fpath;                                                                                                                         */
/*       struct stat st;                                                                                                                      */
/*                                                                                                                                            */
/*       fpath = duf_join_path( *ppath, fname );                                                                                              */
/*       rs = stat( fpath, &st );                                                                                                             */
/*       if ( 0 && now )                                                                                                                      */
/*       {                                                                                                                                    */
/*         fprintf( stderr, "%d. [%lu;%lu;now:%lu - %lu] (%lu) %s\x1b[K\n", rs, st.st_atime, st.st_mtime, st.st_ctime, now, fsize, fpath );   */
/*       }                                                                                                                                    */
/*       if ( fsize > 0 && ( !resmd || rs || !now || now < st.st_mtime || now < st.st_ctime ) )                                               */
/*       {                                                                                                                                    */
/*         if ( 0 )                                                                                                                           */
/*         {                                                                                                                                  */
/*           fprintf( stderr, "fsize>0:%d; !resmd:%d(%lld:%s); rs:%d; t1:%d; t2:%d all:%d  now:%lu; mtime:%lu %s\x1b[K\n", fsize > 0, !resmd, */
/*                    resmd, precord->presult[3], rs, now < st.st_mtime, now < st.st_ctime,                                                   */
/*                    ( !resmd || rs || ( now < st.st_mtime || now < st.st_ctime ) ), now, st.st_mtime, fpath );                              */
/*         }                                                                                                                                  */
/*         resmd = duf_update_md5_file( fpath, filedataid, fsize );                                                                           */
/*         if ( pathid && filenameid && inode && resmd )                                                                                      */
/*           duf_insert_keydata( pathid, filenameid, inode, resmd );                                                                          */
/*       }                                                                                                                                    */
/*       mas_free( fpath );                                                                                                                   */
/*     }                                                                                                                                      */
/*     old_pathid = pathid;                                                                                                                   */
/*   }                                                                                                                                        */
/*   return 0;                                                                                                                                */
/* }                                                                                                                                          */

/* static int                                                                                                                         */
/* duf_update_md5_pathid( unsigned long long pathid, duf_ufilter_t u )                                                                */
/* {                                                                                                                                  */
/*   int r = 0;                                                                                                                       */
/*   char *path = NULL;                                                                                                               */
/*                                                                                                                                    */
/*   fprintf( stderr, "pathid=%lld Calc md5\x1b[K\n", pathid );                                                                       */
/* #define MD5_SQL "SELECT duf_filedatas.id as dataid, duf_filedatas.dev, duf_filedatas.inode, duf_filedatas.md5id," \                */
/*         " duf_filedatas.size as filesize, duf_filenames.id as filenameid, duf_filenames.pathid, duf_filenames.name as filename," \ */
/*         " strftime('%%s',  duf_md5.now) as seconds, " " duf_md5.now " \                                                            */
/*         " FROM duf_filedatas " \                                                                                                   */
/*         " LEFT JOIN duf_filenames ON (duf_filedatas.id=duf_filenames.dataid) " \                                                   */
/*         " LEFT JOIN duf_md5 ON (duf_filedatas.md5id=duf_md5.id) "                                                                  */
/*                                                                                                                                    */
/* (*                                                                                                                                 */
/*  *  duf_sql_update_md5 -- callback called for each row with:                                                                       */
/*  *    &path          passed                                                                                                        */
/*  *    STR_CB_DEF + STR_CB_UDATA_DEF  passed to be called there                                                                     */
/*  *  fmt + ... - sql                                                                                                                */
/*  * *)                                                                                                                              */
/*   if ( pathid )                                                                                                                    */
/*     r = duf_sql_select( duf_sql_update_md5, &path, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_dirinfo_t * ) NULL,                         */
/*                         ( duf_scan_callbacks_t * ) NULL (*  sccb *) , MD5_SQL " WHERE duf_filenames.pathid='%lld'", pathid );      */
/*   else                                                                                                                             */
/*     r = duf_sql_select( duf_sql_update_md5, &path, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_dirinfo_t * ) NULL,                         */
/*                         ( duf_scan_callbacks_t * ) NULL (*  sccb *) , MD5_SQL );                                                   */
/*   if ( path )                                                                                                                      */
/*     mas_free( path );                                                                                                              */
/*   fprintf( stderr, "End calc md5\x1b[K\n" );                                                                                       */
/*   return r;                                                                                                                        */
/* }                                                                                                                                  */

/* int                                                              */
/* duf_update_md5_path( const char *path, duf_ufilter_t u )         */
/* {                                                                */
/*   return duf_update_md5_pathid( duf_path_to_pathid( path ), u ); */
/* }                                                                */

/* 
 * sql must select pathid, filenameid, filename, md5id, size
 * duf_sql_select_cb_t:
 * */

  /* currently used for --same-md5 */
/* int                                                                                                                              */
/* duf_sel_cb_scan_md5( duf_record_t * precord, va_list args, void *sel_cb_udata,                                                   */
/*                   duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb )        */
/* {                                                                                                                                */
/*   int r = 0;                                                                                                                     */
/*                                                                                                                                  */
/*   if ( str_cb )                                                                                                                  */
/*   {                                                                                                                              */
/*     char *path;                                                                                                                  */
/*     md5_std_data_t data;                                                                                                         */
/*                                                                                                                                  */
/*     const char *filename = duf_sql_str_by_name( "filename", precord, 0 );                                                        */
/*     unsigned long long pathid = duf_sql_ull_by_name( "pathid", precord, 0 );                                                     */
/*     unsigned long long filenameid = duf_sql_ull_by_name( "filenameid", precord, 0 );                                             */
/*                                                                                                                                  */
/*     (* unsigned long long filesize = duf_sql_ull_by_name( "filesize", precord, 0 ); *)                                           */
/*                                                                                                                                  */
/*                                                                                                                                  */
/*                                                                                                                                  */
/*     (* pathid = strtoll( precord->presult[0], NULL, 10 ); *)                                                                     */
/*     (* filenameid = strtoll( precord->presult[1], NULL, 10 ); *)                                                                 */
/*     (* filename = precord->presult[2]; *)                                                                                        */
/*     (* data.md5id = strtoll( precord->presult[3], NULL, 10 ); *)                                                                 */
/*     data.md5id = duf_sql_ull_by_name( "md5id", precord, 0 );                                                                     */
/*     (* data.size = strtoll( precord->presult[4], NULL, 10 ); *)                                                                  */
/*     data.size = duf_sql_ull_by_name( "md5size", precord, 0 );                                                                    */
/*     (* data.dupcnt = precord->presult[5] ? strtoll( precord->presult[5], NULL, 10 ) : 0; *)                                      */
/*     data.dupcnt = duf_sql_ull_by_name( "dupcnt", precord, 0 );                                                                   */
/*     path = duf_pathid_to_path( pathid );                                                                                         */
/*                                                                                                                                  */
/*                                                                                                                                  */
/* (* duf_scan_callback_file_t :                                                                                                    */
/*  *           int fun( pathid, path, filenameid, filename, stat,         str_cb_udata, precord ); *)                              */
/*     r = ( *str_cb ) ( pathid, filenameid, filename, &data, ( duf_dirinfo_t * ) NULL, ( duf_scan_callbacks_t * ) NULL, precord ); */
/*     mas_free( path );                                                                                                            */
/*   }                                                                                                                              */
/*   return r;                                                                                                                      */
/* }                                                                                                                                */

/* int                                                                                                                  */
/* duf_scan_vmd5_sql( duf_scan_callback_file_t str_cb, const char *sql, va_list args )                                  */
/* {                                                                                                                    */
/*   return duf_sql_vselect( duf_sel_cb_scan_md5, SEL_CB_UDATA_DEF, str_cb, STR_CB_UDATA_DEF, ( duf_dirinfo_t * ) NULL, */
/*                           ( duf_scan_callbacks_t * ) NULL (*  sccb *) , sql, args );                                 */
/* }                                                                                                                    */

/* int                                                                       */
/* duf_scan_md5_sql( duf_scan_callback_file_t str_cb, const char *sql, ... ) */
/* {                                                                         */
/*   int r = 0;                                                              */
/*   va_list args;                                                           */
/*                                                                           */
/*   va_start( args, sql );                                                  */
/*   r = duf_scan_vmd5_sql( str_cb, sql, args );                             */
/*   va_end( args );                                                         */
/*   return r;                                                               */
/* }                                                                         */

/*
 * sql must select pathid, filenameid, filename, md5id, size
 * */
/* static int                                                                                                                               */
/* duf_scan_md5id( duf_scan_callback_file_t str_cb, unsigned long long dupcnt_min, unsigned long long limit )                               */
/* {                                                                                                                                        */
/*   int r = 0;                                                                                                                             */
/*                                                                                                                                          */
/*   if ( limit )                                                                                                                           */
/*   {                                                                                                                                      */
/*     if ( dupcnt_min )                                                                                                                    */
/*       r = duf_scan_md5_sql( str_cb,                                                                                                      */
/*                             "SELECT duf_paths.id as pathid, duf_filenames.id as filenameid, duf_filenames.name as filename, "            */
/*                             " duf_md5.id as md5id, size as md5size, dupcnt " " FROM duf_md5 "                                            */
/*                             " LEFT JOIN duf_keydata ON (duf_keydata.md5id=duf_md5.id) "                                                  */
/*                             " LEFT JOIN duf_filenames ON (duf_filenames.id=duf_keydata.filenameid)"                                      */
/*                             " LEFT JOIN duf_paths ON (duf_filenames.pathid=duf_paths.id)"                                                */
/*                             " WHERE dupcnt>=%llu ORDER BY size DESC LIMIT %llu", dupcnt_min, limit );                                    */
/*     else                                                                                                                                 */
/*       r = duf_scan_md5_sql( str_cb,                                                                                                      */
/*                             "SELECT duf_paths.id as pathid, duf_filenames.id as filenameid, duf_filenames.name as filename, "            */
/*                             " duf_md5.id as md5id, size as md5size, dupcnt " " FROM duf_md5 "                                            */
/*                             " LEFT JOIN duf_keydata ON (duf_keydata.md5id=duf_md5.id) "                                                  */
/*                             " LEFT JOIN duf_filenames ON (duf_filenames.id=duf_keydata.filenameid)"                                      */
/*                             " LEFT JOIN duf_paths ON (duf_filenames.pathid=duf_paths.id)" " ORDER BY size DESC LIMIT %llu", limit );     */
/*   }                                                                                                                                      */
/*   else                                                                                                                                   */
/*   {                                                                                                                                      */
/*     if ( dupcnt_min )                                                                                                                    */
/*       r = duf_scan_md5_sql( str_cb,                                                                                                      */
/*                             "SELECT duf_paths.id as pathid, duf_filenames.id as filenameid, duf_filenames.name as filename, "            */
/*                             " duf_md5.id as md5id, size as md5size, dupcnt " " FROM duf_md5 "                                            */
/*                             " LEFT JOIN duf_keydata ON (duf_keydata.md5id=duf_md5.id) "                                                  */
/*                             " LEFT JOIN duf_filenames ON (duf_filenames.id=duf_keydata.filenameid)"                                      */
/*                             " LEFT JOIN duf_paths ON (duf_filenames.pathid=duf_paths.id)" " WHERE dupcnt>=%llu " " ORDER BY size DESC",  */
/*                             dupcnt_min );                                                                                                */
/*     else                                                                                                                                 */
/*       r = duf_scan_md5_sql( str_cb,                                                                                                      */
/*                             "SELECT duf_paths.id as pathid, duf_filenames.id as filenameid, duf_filenames.name as filename, "            */
/*                             " duf_md5.id as md5id, size as md5size, dupcnt " " FROM duf_md5 "                                            */
/*                             " LEFT JOIN duf_keydata ON (duf_keydata.md5id=duf_md5.id) "                                                  */
/*                             " LEFT JOIN duf_paths ON (duf_filenames.pathid=duf_paths.id)"                                                */
/*                             " LEFT JOIN duf_filenames ON (duf_filenames.id=duf_keydata.filenameid)" " ORDER BY size DESC", dupcnt_min ); */
/*   }                                                                                                                                      */
/*   fprintf( stderr, "@@@@@@@@@@ %d\n", r );                                                                                               */
/*   return r;                                                                                                                              */
/* }                                                                                                                                        */

/* 
 * duf_scan_callback_file_t:
 * */
/* static int                                                                                                            */
/* duf_sql_scan_print_md5( unsigned long long pathid, const char *path, unsigned long long filenameid, const char *name, */
/*                         void *str_cb_udata, duf_record_t * precord )                                                  */
/* {                                                                                                                     */
/*   md5_std_data_t *mdata;                                                                                              */
/*                                                                                                                       */
/*   mdata = ( md5_std_data_t * ) str_cb_udata;                                                                          */
/*   printf( "-- %7llu - %lld\n", mdata->md5id, mdata->size );                                                           */
/*   (* printf( "%s ", name ); *)                                                                                        */
/*   return 0;                                                                                                           */
/* }                                                                                                                     */

/* int                                                                     */
/* duf_print_md5( unsigned long long limit )                               */
/* {                                                                       */
/*   int r = 0;                                                            */
/*                                                                         */
/*   duf_scan_md5id( duf_sql_scan_print_md5, 0 (* dupcnt_min *) , limit ); */
/*   return r;                                                             */
/* }                                                                       */

/* 
 * duf_scan_callback_file_t:
 * */
/* int                                                                                                                  */
/* duf_sql_scan_print_md5_same( unsigned long long pathid, unsigned long long filenameid,                               */
/*                              const char *name, void *str_cb_udata, duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb, */
/*                              duf_record_t * precord )                                                                */
/* {                                                                                                                    */
/*   md5_std_data_t *mdata;                                                                                             */
/*                                                                                                                      */
/*   mdata = ( md5_std_data_t * ) str_cb_udata;                                                                         */
/*   printf( "-- size=%lld; %lld files (id:%llu) ----------\n", mdata->size, mdata->dupcnt, mdata->md5id );             */
/*   duf_print_files_by_md5id( mdata->md5id );                                                                          */
/*   return 0;                                                                                                          */
/* }                                                                                                                    */

/* int                                                                           */
/* duf_print_md5_same( unsigned long long dupcnt_min, unsigned long long limit ) */
/* {                                                                             */
/*   int r = 0;                                                                  */
/*                                                                               */
/*   duf_scan_md5id( duf_sql_scan_print_md5_same, dupcnt_min, limit );           */
/*   return r;                                                                   */
/* }                                                                             */

/*
 * sql must select pathid, filenameid, filename(, md5id, size, dupcnt)
 * duf_sql_select_cb_t: 
 * */
/* static int                                                                                                                  */
/* duf_sql_insert_md5( duf_record_t * precord, va_list args, void *sel_cb_udata,                                               */
/*                     duf_scan_callback_file_t str_cb, void *str_cb_udata, duf_dirinfo_t * pdi, duf_scan_callbacks_t * sccb ) */
/* {                                                                                                                           */
/*   unsigned long long *presmd;                                                                                               */
/*                                                                                                                             */
/*   presmd = ( unsigned long long * ) sel_cb_udata;                                                                           */
/*   *presmd = strtoll( precord->presult[0], NULL, 10 );                                                                       */
/*   duf_sql( "UPDATE duf_md5 SET ucnt=ucnt+1, now=datetime() WHERE id='%lld'", *presmd );                                     */
/*   return 0;                                                                                                                 */
/* }                                                                                                                           */

/* unsigned long long                                                                                                     */
/* duf_insert_md5( unsigned long long *md64, size_t fsize )                                                               */
/* {                                                                                                                      */
/*   unsigned long long resmd = -1;                                                                                       */
/*   int r = 0;                                                                                                           */
/*                                                                                                                        */
/*   r = duf_sql_c( "INSERT INTO duf_md5 (md5sum1,md5sum2,size,ucnt,now) values ('%lld','%lld','%llu',0,datetime())",     */
/*                  DUF_CONSTRAINT_IGNORE_YES, md64[1], md64[0], ( unsigned long long ) fsize );                          */
/*   if ( r == duf_constraint )                                                                                           */
/*   {                                                                                                                    */
/*     r = duf_sql_select( duf_sql_insert_md5, &resmd, STR_CB_DEF, STR_CB_UDATA_DEF, ( duf_dirinfo_t * ) NULL,            */
/*                         ( duf_scan_callbacks_t * ) NULL (*  sccb *) ,                                                  */
/*                         "SELECT id as md5id FROM duf_md5 WHERE md5sum1='%lld' and md5sum2='%lld'", md64[1], md64[0] ); */
/*   }                                                                                                                    */
/*   else if ( !r (* assume SQLITE_OK *)  )                                                                               */
/*     resmd = duf_last_insert_rowid(  );                                                                                 */
/*   else                                                                                                                 */
/*     fprintf( stderr, "error duf_insert_md5 %d\n", r );                                                                 */
/*                                                                                                                        */
/*   return resmd;                                                                                                        */
/* }                                                                                                                      */
