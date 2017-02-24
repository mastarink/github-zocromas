#include <string.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include "masxfs_defs.h"

#include "masxfs_structs.h"

#include "masxfs_levinfo_base.h"

#include "masxfs_levinfo_path.h"

char *
masxfs_levinfo_normalize_path( const char *path, const char *name )
{
  char *npath = NULL;

  if ( path )
  {
    size_t plen = 0, nlen = 0, len = 0;

    plen = strlen( path );
    if ( name )
      nlen = strlen( name );
    len = plen + nlen;
    if ( len > 0 )
    {
      npath = mas_malloc( len + 2 /* for 0 and possible '/'  */  );
      {
        const char *p = path;
        char *np = npath;

        while ( p && *p )
        {
          char c;

          *np++ = c = *p++;
          while ( c == '/' && *p == '/' )
            p++;
        }
        *np = 0;
        if ( name )
        {
          if ( np[-1] != '/' )
            *np++ = '/';
          *np = 0;
          strcat( npath, name );
        }
      }
    }
  }
  return npath;
}

masxfs_levinfo_t *
masxfs_levinfo_path2lia( const char *path, masxfs_depth_t depth_limit, masxfs_depth_t * psz )
{
  masxfs_levinfo_t *levinfo = NULL;

  if ( path && *path )
  {
    masxfs_depth_t levinfo_depth = 0;

    levinfo = masxfs_levinfo_create_array_setup( depth_limit );

#if 1
    {
      const char *ptok = path;

      while ( ptok )
      {
        char *ep = strchrnul( ptok, '/' );
        size_t len = ep - ptok;

      /* masxfs_entry_type_t de_type = *ep == '/' ? MASXFS_ENTRY_DIR_NUM : MASXFS_ENTRY_UNKNOWN_NUM; */
        masxfs_entry_type_t de_type = *ep == MASXFS_ENTRY_UNKNOWN_NUM;

        masxfs_levinfo_n_init( levinfo + levinfo_depth, levinfo_depth, ptok, len, de_type, 0 );
	levinfo_depth++;
        while ( *ep == '/' )
          ep++;
        ptok = *ep ? ep : NULL;
      }
    }
#elif 0
    {
      char *spath = masxfs_levinfo_normalize_path( path, NULL );
      char *stok = "", *ep;

      for ( char *nstok = strtok_r( spath, "/", &ep ); stok; nstok = strtok_r( NULL, "/", &ep ) )
      {
        masxfs_levinfo_init( levinfo + levinfo_depth++, stok, nstok ? MASXFS_ENTRY_DIR_NUM : MASXFS_ENTRY_UNKNOWN );
        stok = nstok;
      }
      mas_free( spath );
    }
#elif 0
    {
      char *spath = masxfs_levinfo_normalize_path( path, NULL );
      char *spatht = spath;

      for ( char *stok = strsep( &spatht, "/" ); stok && ( !levinfo_depth || *stok ); stok = strsep( &spatht, "/" ) )
      {
      /* fprintf( stderr, "%ld stok:%s (%ld)\n", levinfo_depth, stok, strlen( stok ) ); */
        masxfs_levinfo_init( levinfo + levinfo_depth++, stok, MASXFS_ENTRY_UNKNOWN );
      }
      mas_free( spath );
    }
#endif
  /* if ( !path[1] )                  */
  /* DIE( "L:%ld", levinfo_depth ); */
    if ( psz )
      *psz = levinfo_depth;
  }
  return levinfo;
}

char *
masxfs_levinfo_lia2path( masxfs_levinfo_t * lia, masxfs_depth_t pidepth, char tail )
{
  size_t len = 0;
  char *path = NULL;

  if ( lia )
  {
    if ( pidepth )
    {
      for ( masxfs_depth_t i = 0; i < pidepth; i++ )
      {
        len += strlen( lia[i].name );
        len++;                                                       /* '/' */
      }
      len += 3;
      path = mas_malloc( len );
      {
        char *p = path;

        for ( masxfs_depth_t i = 0; i < pidepth; i++ )
        {
          if ( i != 1 )
            *p++ = '/';
          strcpy( p, lia[i].name );
          while ( p && *p )
            p++;
        }
        if ( tail && p[-1] != tail )
          *p++ = tail;
        *p++ = 0;
      }
    }
    else
    {
    }
  }
  return path;
}

char *
masxfs_levinfo_li2path_up( masxfs_levinfo_t * li, char tail )
{
  return masxfs_levinfo_lia2path( li - li->lidepth, li->lidepth + 1, tail );
}

char *
masxfs_levinfo_li2path( masxfs_levinfo_t * li )
{
  li -= li->lidepth;
  while ( li->name )
    li++;
  return masxfs_levinfo_li2path_up( li - 1, 0 );
}
