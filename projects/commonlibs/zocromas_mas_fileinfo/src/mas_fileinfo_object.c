#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include <mastar/wrap/mas_memory.h>

#include <mastar/types/mas_varset_types.h>
#include <mastar/varset/mas_varset.h>

#include <mastar/types/mas_opts_types.h>

#include "mas_unidata.h"
#include "mas_fileinfo.h"
#include "mas_fileinfo_object.h"

/*
this:
  mas_fileinfo_object.c
related:
  mas_fileinfo.c
*/


mas_fileinfo_t *
mas_fileinfo_create( void )
{
  mas_fileinfo_t *fileinfo;

  fileinfo = mas_malloc( sizeof( mas_fileinfo_t ) );
  memset( fileinfo, 0, sizeof( mas_fileinfo_t ) );
  return fileinfo;
}

void
mas_fileinfo_delete( mas_fileinfo_t * fileinfo )
{
  if ( fileinfo )
  {
    if ( fileinfo->etag )
      mas_free( fileinfo->etag );
    if ( fileinfo->root )
      mas_free( fileinfo->root );
    if ( fileinfo->tail )
      mas_free( fileinfo->tail );
    mas_fileinfo_delete_data( fileinfo );
    /* if ( fileinfo->content_type )         */
    /*   mas_free( fileinfo->content_type ); */
    mas_free( fileinfo );
  }
}

mas_fileinfo_t *
mas_fileinfo_init( mas_fileinfo_t * fileinfo, const char *root, const char *path, data_loader_t loader, const void *userdata )
{
  if ( root )
  {
    if ( fileinfo->root )
      mas_free( fileinfo->root );
    if ( fileinfo->tail )
      mas_free( fileinfo->tail );

    fileinfo->userdata = userdata;
    fileinfo->root = NULL;
    fileinfo->tail = NULL;
    fileinfo->root = mas_strdup( root );
    fileinfo->tail = mas_strdup( path );
    fileinfo->data_loader = loader;
    /* OFF 20130605.151602
     * mas_fileinfo_stat( fileinfo );
     * */
    /* if ( fileinfo->content_type )         */
    /*   mas_free( fileinfo->content_type ); */
  }
  else
  {
    /* MAS_LOG( "no root set" ); */
  }
  return fileinfo;
}

void
mas_fileinfo_delete_data( mas_fileinfo_t * fileinfo )
{
  if ( fileinfo && fileinfo->udata )
    mas_unidata_delete( fileinfo->udata );
  fileinfo->udata = NULL;
}
