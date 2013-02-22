#include <mastar/wrap/mas_std_def.h>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib.h>
#include <mastar/tools/mas_arg_tools.h>

#include "mas_unidata.h"

/*
this:
  mas_unidata.c
related:
  mas_fileinfo.c
  mas_fileinfo_object.c
*/

char *
mas_unidata_data( mas_unidata_t * ud )
{
  return ud ? ud->data : NULL;
}

size_t
mas_unidata_data_size( mas_unidata_t * ud )
{
  return ud ? ud->size : 0;
}

mas_content_type_t
mas_udata_icontent_type( mas_unidata_t * ud )
{
  return ud ? ud->icontent_type : MAS_CONTENT_BAD;
}

void
mas_udata_set_icontent_type( mas_unidata_t * ud, mas_content_type_t ict )
{
  if ( ud )
  {
    ud->icontent_type = ict;
  }
}

mas_unidata_t *
mas_unidata_create( void )
{
  mas_unidata_t *ud;

  ud = mas_malloc( sizeof( mas_unidata_t ) );
  memset( ud, 0, sizeof( mas_unidata_t ) );
  return ud;
}

void
mas_unidata_delete( mas_unidata_t * ud )
{
  if ( ud )
  {
    if ( ud->data )
      mas_free( ud->data );
    ud->data = NULL;
    mas_free( ud );
  }
}

void
mas_unidata_link_data( mas_unidata_t * udata, char *data, size_t size )
{
  udata->size = size;
  udata->data = data;
}

content_type_details_t content_types[] = {
  {MAS_CONTENT_HTML, "text", "html"}
  ,
  {MAS_CONTENT_TEXT, "text", "plain"}
  ,
  {MAS_CONTENT_GIF, "image", "gif"}
  ,
  {MAS_CONTENT_JPEG, "image", "jpeg"}
};

char *
mas_unidata_content_type_string( mas_unidata_t * ud )
{
  char *s = NULL;

  if ( ud )
  {
    for ( int it = 0; it < sizeof( content_types ) / sizeof( content_types[0] ); it++ )
    {
      /* MAS_LOG( "CT: %d", mas_udata_icontent_type( ud ) ); */
      if ( mas_udata_icontent_type( ud ) == content_types[it].ctype )
      {
        s = mas_strdup( content_types[it].mtype );
        s = mas_strcat_x( s, "/" );
        s = mas_strcat_x( s, content_types[it].dtype );
      }
    }
  }
  return s;
}