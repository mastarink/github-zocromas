#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>



#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_tools.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#include <mastar/types/mas_opts_types.h>


#ifdef MAS_OLD_VARIABLES_HTTP
#  include <mastar/variables/mas_variables.h>
#else
#  include <mastar/types/mas_varset_types.h>
#  include <mastar/varset/mas_varset_vclass.h>
#  include <mastar/varset/mas_varset_vclass_search.h>
#endif

#include "mas_unidata.h"
#include "mas_fileinfo_object.h"
#include "mas_fileinfo.h"


static mas_evaluated_t *mas_fileinfo_load_data( const mas_options_t * popts, mas_fileinfo_t * fileinfo );


/*
this:
  mas_fileinfo.c
related:
  mas_fileinfo_object.c
  mas_unidata.c
more:
  mas_fileinfo.h
  mas_fileinfo_object.h
  mas_fileinfo_types.h
  mas_unidata.h
  mas_unidata_types.h

*/


mas_evaluated_t *
_mas_fileinfo_data( const mas_options_t * popts, mas_fileinfo_t * fileinfo )
{
  return fileinfo ? mas_unidata_data( fileinfo->udata ) : NULL;
}

size_t
mas_fileinfo_data_size( mas_fileinfo_t * fileinfo )
{
  return fileinfo ? mas_unidata_data_size( fileinfo->udata ) : 0;
}

void
mas_fileinfo_set_icontent_type( mas_fileinfo_t * fileinfo, mas_content_type_t ict )
{
  if ( fileinfo )
  {
    if ( !fileinfo->udata )
      fileinfo->udata = mas_unidata_create(  );
    mas_udata_set_icontent_type( fileinfo->udata, ict );
    /* MAS_LOG( "ICT:%d", ict ); */
  }
}

void
mas_fileinfo_set_icontent_size( mas_fileinfo_t * fileinfo, size_t size )
{
  if ( fileinfo )
  {
    if ( !fileinfo->udata )
      fileinfo->udata = mas_unidata_create(  );
    mas_udata_set_icontent_size( fileinfo->udata, size );
  }
}

void
mas_fileinfo_set_content_size( mas_fileinfo_t * fileinfo, const char *ssize )
{
  unsigned long size = 0;

  size = atol( ssize );
  mas_fileinfo_set_icontent_size( fileinfo, size );
}

mas_content_type_t
mas_fileinfo_icontent_type( mas_fileinfo_t * fileinfo )
{
  mas_content_type_t ict = MAS_CONTENT_BAD;

  if ( fileinfo )
  {
    if ( !fileinfo->udata )
      fileinfo->udata = mas_unidata_create(  );
    ict = mas_udata_icontent_type( fileinfo->udata );
  }
  return ict;
}

mas_unidata_t *
mas_fileinfo_unidata( mas_fileinfo_t * fileinfo )
{
  return fileinfo ? fileinfo->udata : NULL;
}

mas_evaluated_t *
mas_fileinfo_data( const mas_options_t * popts, mas_fileinfo_t * fileinfo )
{
  return fileinfo ? mas_fileinfo_load_data( popts, fileinfo ) : NULL;
}

char *
mas_fileinfo_content_type_string( mas_fileinfo_t * fileinfo )
{
  return fileinfo ? mas_unidata_content_type_string( fileinfo->udata ) : NULL;
}

/********************************************************************************************/
static mas_evaluated_t *
_mas_fileinfo_link_data( const mas_options_t * popts, mas_fileinfo_t * fileinfo, mas_evaluated_t * data, size_t size )
{
  if ( fileinfo )
  {
    if ( !fileinfo->udata )
      fileinfo->udata = mas_unidata_create(  );
    if ( !fileinfo->filesize )
      fileinfo->filesize = mas_unidata_data_size( fileinfo->udata );
    mas_unidata_link_data( fileinfo->udata, data, size );
  }
  return _mas_fileinfo_data( popts, fileinfo );
}

mas_evaluated_t *
_mas_fileinfo_link_dataz( const mas_options_t * popts, mas_fileinfo_t * fileinfo, mas_evaluated_t * data )
{
  return _mas_fileinfo_link_data( popts, fileinfo, data, strlen( ( char * ) data->data ) );
}

static mas_evaluated_t *
mas_fileinfo_load_data( const mas_options_t * popts, mas_fileinfo_t * fileinfo )
{
  mas_evaluated_t *data = NULL;

  if ( !_mas_fileinfo_data( popts, fileinfo ) )
  {
    size_t size = 0;

    /* MAS_LOG( "to load data" ); */
    size = fileinfo->filesize;
    data = ( fileinfo->data_loader ) ( popts, fileinfo->root, fileinfo->tail, size, &fileinfo->filesize, &fileinfo->inode,
                                       &fileinfo->filetime, fileinfo->userdata );
    /* EMSG( "loader passed %lu %lu %lu", fileinfo->filesize, fileinfo->inode, fileinfo->filetime ); */
    _mas_fileinfo_link_data( popts, fileinfo, data, fileinfo->filesize );

    if ( fileinfo->filesize && fileinfo->root && mas_fileinfo_icontent_type( fileinfo ) <= 0 )
    {
      mas_content_type_t ict;

      ict = mas_fileinfo_content_type_by_ext( fileinfo );
      if ( ict > 0 )
        mas_fileinfo_set_icontent_type( fileinfo, ict );
    }

    /* MAS_LOG( "loaded data [%lu]", fileinfo->filesize ); */
  }
  return _mas_fileinfo_data( popts, fileinfo );
}


/*******************************************************************************/
/* static char *                                                               */
/* mas_fileinfo_link_data( mas_fileinfo_t * fileinfo, char *data, size_t size ) */
/* {                                                                           */
/*   char *ndata = NULL;                                                       */
/*                                                                             */
/*   if ( size )                                                               */
/*     ndata = mas_malloc( size );                                             */
/*   if ( size && data )                                                       */
/*   {                                                                         */
/*     memcpy( ndata, data, size );                                            */
/*   }                                                                         */
/*   else                                                                      */
/*   {                                                                         */
/*     memset( ndata, 0, size );                                               */
/*   }                                                                         */
/*   return _mas_fileinfo_link_data( fileinfo, ndata, size );                   */
/* }                                                                           */
/* static char *                                                               */
/* mas_fileinfo_link_dataz( mas_fileinfo_t * fileinfo, char *data )             */
/* {                                                                           */
/*   return mas_fileinfo_link_data( fileinfo, data, strlen( data ) + 1 );       */
/* }                                                                           */
/*******************************************************************************/


int
mas_fileinfo_make_etag( mas_fileinfo_t * fileinfo )
{
  int r = -1;

  if ( fileinfo )
  {
    if ( fileinfo->etag )
      mas_free( fileinfo->etag );
    fileinfo->etag = NULL;
    if ( fileinfo->inode && fileinfo->filetime && fileinfo->filesize )
    {
      char etag_buf[256];

      snprintf( etag_buf, sizeof( etag_buf ), "%lx-%lx-%lx", fileinfo->inode, fileinfo->filesize, fileinfo->filetime * 1000000 );

      fileinfo->etag = mas_strdup( "\"" );
      fileinfo->etag = mas_strcat_x( fileinfo->etag, etag_buf );
      fileinfo->etag = mas_strcat_x( fileinfo->etag, "\"" );
      r = 0;
    }
    else
    {
      EMSG( "ETAG error %d %d %d", fileinfo->inode ? 1 : 0, fileinfo->filetime ? 1 : 0, fileinfo->filesize ? 1 : 0 );
    }
  }
  return r;
}

#ifdef MAS_OLD_VARIABLES_HTTP
mas_variables_list_head_t *
mas_fileinfo_make_headers( mas_variables_list_head_t * outdata, mas_fileinfo_t * fileinfo )
#elif defined(MAS_VARSET_VARIABLES_HTTP)
mas_varset_t *
mas_fileinfo_make_headers( mas_varset_t * outdata, mas_fileinfo_t * fileinfo )
#else
mas_varset_class_t *
mas_fileinfo_make_headers( mas_varset_class_t * outdata, mas_fileinfo_t * fileinfo )
#endif
{
  size_t dsz;

  dsz = mas_fileinfo_data_size( fileinfo );
  if ( !dsz )
  {
    dsz = fileinfo->filesize;
  }
  /* EMSG( "? error [%lu] %d %d %d", dsz, fileinfo->inode ? 1 : 0, fileinfo->filetime ? 1 : 0, fileinfo->filesize ? 1 : 0 ); */
  if ( 1 || dsz )
  {
#ifdef MAS_OLD_VARIABLES_HTTP
    outdata = mas_variable_create_x( outdata, /* MAS_THREAD_TRANSACTION, */ "header", "Content-Length", NULL, "%d", dsz );
#elif defined(MAS_VARSET_VARIABLES_HTTP)
    outdata = mas_varset_search_variablef( outdata, "header", "Content-Length", NULL, "%d", dsz );
#else
    outdata = mas_varset_vclass_search_variablef( outdata, NULL, "Content-Length", NULL, "%d", dsz );
    if ( !dsz )
    {
      EMSG( "Length error %d %d %d", fileinfo->inode ? 1 : 0, fileinfo->filetime ? 1 : 0, fileinfo->filesize ? 1 : 0 );
    }
#endif
  }
  /* if ( mas_udata_icontent_type( fileinfo->udata ) ) */
  {
    char *content_type;

    content_type = mas_fileinfo_content_type_string( fileinfo );
    if ( content_type )
    {
#ifdef MAS_OLD_VARIABLES_HTTP
      outdata = mas_variable_create_text( outdata, /* MAS_THREAD_TRANSACTION, */ "header", "Content-Type", content_type, 0 );
#elif defined(MAS_VARSET_VARIABLES_HTTP)
      outdata = mas_varset_search_variable( outdata, "header", "Content-Type", content_type );
#else
      outdata = mas_varset_vclass_search_variable( outdata, NULL, "Content-Type", content_type );
#endif
      mas_free( content_type );
    }
  }
  mas_fileinfo_make_etag( fileinfo );
  if ( fileinfo && fileinfo->etag )
  {
#ifdef MAS_OLD_VARIABLES_HTTP
    outdata = mas_variable_create_text( outdata, /* MAS_THREAD_TRANSACTION, */ "header", "ETag", fileinfo->etag, 0 );
#elif defined(MAS_VARSET_VARIABLES_HTTP)
    outdata = mas_varset_search_variable( outdata, "header", "ETag", fileinfo->etag );
#else
    outdata = mas_varset_vclass_search_variable( outdata, NULL, "ETag", fileinfo->etag );
#endif
  }
  if ( fileinfo && fileinfo->filetime )
  {
#ifdef MAS_OLD_VARIABLES_HTTP
    outdata = mas_variable_create_x( outdata, /* MAS_THREAD_TRANSACTION, */ "header", "Last-Modified", mas_xvstrftime_time,
                                     "%a, %d %b %Y %T GMT", fileinfo->filetime );
#elif defined(MAS_VARSET_VARIABLES_HTTP)
    outdata =
          mas_varset_search_variablef( outdata, "header", "Last-Modified", mas_xvstrftime_time, "%a, %d %b %Y %T GMT", fileinfo->filetime );
#else
    outdata =
          mas_varset_vclass_search_variablef( outdata, NULL, "Last-Modified", mas_xvstrftime_time, "%a, %d %b %Y %T GMT",
                                              fileinfo->filetime );
#endif
  }
  return outdata;
}

/* mas_variables_list_head_t *                                                                                                        */
/* mas_fileinfo_make_body( mas_variables_list_head_t * outdata, mas_fileinfo_t * fileinfo )                                           */
/* {                                                                                                                                  */
/*   if ( fileinfo )                                                                                                                  */
/*   {                                                                                                                                */
/*     char *data;                                                                                                                    */
/*                                                                                                                                    */
/*     data = mas_fileinfo_data( fileinfo );                                                                                          */
/*     MAS_LOG( "to make binary %lu => %lu", fileinfo->filesize, mas_fileinfo_data_size( fileinfo ) );                                */
/*     outdata = mas_variable_create_binary( outdata, MAS_THREAD_TRANSACTION, "body", "body", data, fileinfo->filesize );             */
/*   }                                                                                                                                */
/*   else                                                                                                                             */
/*   {                                                                                                                                */
/*     outdata =                                                                                                                      */
/*           mas_variable_create_x( outdata, MAS_THREAD_TRANSACTION, "body", "body", NULL,                                            */
/*                                  "<html><body><h1>%s</h1><div>%s</div><div>" "<a href='http://mastarink.net:5001/'>5001</a>"       */
/*                                  ":<a href='http://mastarink.net:5002/'>5002</a>" ":<a href='http://mastarink.net:5003/'>5003</a>" */
/*                                  "</div></body></html>\r\n", "Hallo, world", "A small number" );                                   */
/*   }                                                                                                                                */
/*   return outdata;                                                                                                                  */
/* }                                                                                                                                  */

/* int                                                        */
/* mas_fileinfo_stat( mas_fileinfo_t * fileinfo )             */
/* {                                                          */
/*   int r = -1;                                              */
/*   char *filepath;                                          */
/*                                                            */
/*   filepath = mas_strdup( fileinfo->root );                 */
/*   filepath = mas_strcat_x( filepath, fileinfo->tail );     */
/*                                                            */
/*   if ( filepath )                                          */
/*   {                                                        */
/* #ifdef MAS_TEST_HTTP_FILE                                  */
/*     fileinfo->filesize = 30942;                            */
/*     fileinfo->filetime = 1357909287;                       */
/*     fileinfo->inode = 2222222;                             */
/* #else                                                      */
/*     struct stat est;                                       */
/*                                                            */
/*     r = stat( filepath, &est );                            */
/*     if ( r >= 0 )                                          */
/*     {                                                      */
/*       (* inode number : st_ino                        *)   */
/*       (* size         : st_size                       *)   */
/*       (* time of last modification         : st_mtime *)   */
/*       fileinfo->filesize = est.st_size;                    */
/*       fileinfo->filetime = est.st_mtime;                   */
/*       fileinfo->inode = est.st_ino;                        */
/*     }                                                      */
/*     else                                                   */
/*     {                                                      */
/*       (* MAS_LOGERR( errno, "file %s stat", filepath ); *) */
/*       errno = 0;                                           */
/*     }                                                      */
/* #endif                                                     */
/*     mas_free( filepath );                                  */
/*   }                                                        */
/*   return r;                                                */
/* }                                                          */

content_type_ext_t content_exts[] = {
  {MAS_CONTENT_HTML, ":html:htm:"}
  ,
  {MAS_CONTENT_GIF, ":gif:GIF:"}
  ,
  {MAS_CONTENT_JPEG, ":jpg:JPG:jpeg:JPEG"}
  ,
  {MAS_CONTENT_TEXT, ":txt:"}
};

static mas_content_type_t
mas_content_type_by_ext( const char *filepath )
{
  mas_content_type_t t = MAS_CONTENT_BAD;
  const char *epath;
  const char *ext = NULL;
  char extbuf[256];

  epath = filepath + strlen( filepath );
  while ( epath > filepath )
  {
    epath--;
    if ( *epath == '.' )
      ext = epath + 1;
    else if ( *epath == '/' )
      break;
  }
  if ( ext )
  {
    int len;

    strcpy( extbuf + 1, ext );
    extbuf[0] = ':';
    len = strlen( ext );
    extbuf[0] = ':';
    extbuf[len + 1] = ':';
    extbuf[len + 2] = '\0';
  }

  /* rMSG( "CHECK EXT %s", extbuf ); */
  for ( int it = 0; it < sizeof( content_exts ) / sizeof( content_exts[0] ); it++ )
  {
    /* MAS_LOG( "CHECK EXT '%s' ? '%s'", content_exts[it].extset, extbuf ); */
    if ( strstr( content_exts[it].extset, extbuf ) )
    {
      t = content_exts[it].ctype;
      /* MAS_LOG( "CHECK EXT = %d", t ); */
      break;
    }
  }
  return t;
}

mas_content_type_t
mas_fileinfo_content_type_by_ext( mas_fileinfo_t * fileinfo )
{
  mas_content_type_t t = MAS_CONTENT_BAD;
  char *filepath;

  filepath = mas_strdup( fileinfo->root );
  filepath = mas_strcat_x( filepath, fileinfo->tail );

  if ( filepath )
  {
    t = mas_content_type_by_ext( filepath );
    mas_free( filepath );
  }
  return t;
}


mas_evaluated_t *
mas_load_filename_at_file( const mas_options_t * popts, const char *root, const char *tail, size_t size, size_t * ptruesize,
                           ino_t * ptrueinode, time_t * ptruefiletime, const void *arg )
{
  char *filedata = NULL;
  char *filepath;

  filepath = mas_strdup( root );
  filepath = mas_strcat_x( filepath, tail );
  filedata = mas_load_filename_file( filepath, size, ptruesize, ptrueinode, ptruefiletime, arg );
  if ( filepath )
    mas_free( filepath );
  return mas_evaluated_wrap_pchar( filedata );
}

mas_evaluated_t *
mas_load_filename_at_fd( const mas_options_t * popts, const char *root, const char *tail, size_t size, size_t * ptruesize,
                         ino_t * ptrueinode, time_t * ptruefiletime, const void *arg )
{
  char *filedata = NULL;
  char *filepath;

  filepath = mas_strdup( root );
  filepath = mas_strcat_x( filepath, tail );
  filedata = mas_load_filename_fd( filepath, size, ptruesize, ptrueinode, ptruefiletime, arg );
  if ( filepath )
    mas_free( filepath );
  return mas_evaluated_wrap_pchar( filedata );
}
