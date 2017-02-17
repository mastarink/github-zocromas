#include <stdio.h>
#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "masxfs_defs.h"
#include "masxfs_error.h"

#include "masxfs_structs.h"

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
masxfs_levinfo_init( masxfs_levinfo_t * li, const char *name )
{
  if ( li->name )
    mas_free( li->name );
  if ( li )
    li->name = mas_strdup( name );
}

void
masxfs_levinfo_reset( masxfs_levinfo_t * li )
{
  if ( li && li->name )
    mas_free( li->name );
  li->name = NULL;
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
