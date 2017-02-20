#include <stdio.h>
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "masxfs_defs.h"
#include "masxfs_error.h"

#include "masxfs_structs.h"

#include "masxfs_levinfo_io.h"
#include "masxfs_levinfo_base.h"

masxfs_levinfo_t *
masxfs_levinfo_create_array( size_t sz )
{
#if 0
  size_t size = sz * sizeof( masxfs_levinfo_t );
  masxfs_levinfo_t *li = mas_malloc( size );

  memset( li, 0, size );
#else
  masxfs_levinfo_t *li = mas_calloc( sz, sizeof( masxfs_levinfo_t ) );
#endif
  return li;
}

masxfs_levinfo_t *
masxfs_levinfo_create_array_setup( size_t sz )
{
  masxfs_levinfo_t *lia = masxfs_levinfo_create_array( sz );

  for ( size_t i = 0; i < sz; i++ )
  {
  /* lia[i].pi = pi; */
    lia[i].lidepth = i;
  }
  return lia;
}

masxfs_levinfo_t *
masxfs_levinfo_create( void )
{
  return masxfs_levinfo_create_array( 0 );
}

void
masxfs_levinfo_n_init( masxfs_levinfo_t * li, const char *name, size_t len, masxfs_entry_type_t d_type )
{
  if ( li )
  {
    if ( li->name )
      mas_free( li->name );
    li->name = mas_strndup( name, len );
    li->detype = d_type;
  }
}

void
masxfs_levinfo_init( masxfs_levinfo_t * li, const char *name, masxfs_entry_type_t d_type )
{
  masxfs_levinfo_n_init( li, name, name ? strlen( name ) : 0, d_type );
}

void
masxfs_levinfo_reset( masxfs_levinfo_t * li )
{
  if ( li )
  {
    masxfs_levinfo_closedir( li );
    masxfs_levinfo_close( li );
    li->fd = 0;
    if ( li->name )
      mas_free( li->name );
    li->name = NULL;
    if ( li->stat )
      mas_free( li->stat );
    li->stat = NULL;
  }
}

void
masxfs_levinfo_reset_lia( masxfs_levinfo_t * lia, size_t sz )
{
  if ( lia )
  {
    for ( size_t il = 0; il < sz && ( lia + il )->name; il++ )
    {
      masxfs_levinfo_reset( lia + il );
    }
  }
}

void
masxfs_levinfo_delete_lia( masxfs_levinfo_t * li, size_t sz )
{
  if ( li )
  {
    masxfs_levinfo_reset_lia( li, sz );
    mas_free( li );
  }
}
