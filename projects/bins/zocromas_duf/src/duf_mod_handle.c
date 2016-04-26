#include <assert.h>
#include <string.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <dirent.h>
#include <fnmatch.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/tools/mas_arg_tools.h>                              /* mas_strcat_x; etc. ▤ */
#include <mastar/tools/mas_utils_path.h>                             /* mas_normalize_path; mas_pathdepth; mas_realpath etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_mod_handle.h"

/* 20160422.160951 */
/* ###################################################################### */
#include "duf_mod_handle.h"
/* ###################################################################### */

static char *libpath = NULL;
SRX( MOD, const char *, lp, NULL, libpath, void )
{
  if ( !libpath )
    libpath = mas_normalize_path_plus( MAS_LIBDIR, "dufmod", NULL );
/* return libpath; */
  lp = libpath;
  ERX( MOD, const char *, lp, NULL, libpath, void );
}

SRX( MOD, char **, lib_list, NULL, liblist, const char *pat, int *psize )
{
/* char **lib_list = NULL; */
  DIR *dh = NULL;

  dh = opendir( CRX( libpath ) );
  if ( dh )
  {
    int cnt = 0;
    struct dirent *de;

    while ( ( de = readdir( dh ) ) )
    {
      if ( ( !pat && 0 == fnmatch( "*.so", de->d_name, 0 ) ) || ( pat && 0 == fnmatch( pat, de->d_name, 0 ) ) )
      {
        cnt++;
      }
    }
    if ( cnt > 0 )
    {
      char **ll;

      rewinddir( dh );
      lib_list = mas_malloc( sizeof( char * ) * ( cnt + 1 ) );
      memset( lib_list, 0, sizeof( char * ) * ( cnt + 1 ) );
      ll = lib_list;
      cnt = 0;
      while ( ( de = readdir( dh ) ) )
      {
        if ( ( 0 == fnmatch( "*.so", de->d_name, 0 ) ) && ( !pat || 0 == fnmatch( pat, de->d_name, 0 ) ) )
        {
          *ll++ = mas_strndup( de->d_name, strlen( de->d_name ) - 3 );
        /* QT( "@@%d matched %s", cnt, de->d_name ); */
          cnt++;
        }
      }
      if ( psize )
        *psize = cnt;
    }
    closedir( dh );
  }
/* return lib_list; */
  ERX( MOD, char **, lib_list, NULL, liblist, const char *pat, int *psize );
}

static
SRN( MOD, void, delete_libpath, void )
{
  mas_free( libpath );
  libpath = NULL;
  ERN( MOD, void, delete_libpath, void );
}

SRN( MOD, void, delete_liblist, char **lib_list )
{
  if ( lib_list )
  {
    for ( char **lp = lib_list; lp && *lp; lp++ )
    {
      mas_free( *lp );
    }
    mas_free( lib_list );
  }
  ERN( MOD, void, delete_liblist, char **lib_list );
}

static void mod_handle_destructor( void ) __attribute__ ( ( destructor( 101 ) ) );
static void
mod_handle_destructor( void )
{
  duf_delete_libpath(  );
}

char *
duf_libname2sopath( const char *libname )
{
  char *sopath = NULL;

  sopath = mas_strdup( duf_libpath(  ) );
  sopath = mas_strcat_x( sopath, libname );
  sopath = mas_strcat_x( sopath, ".so" );
  return sopath;
}

SRP( MOD, void *, psym, NULL, load_symbol, const char *libname, const char *symbol, void **plibhan )
{
/* void *psym = NULL; */
  void *han = NULL;
  char *sopath = NULL;
  char *es = NULL;

#if 0
/* sopath = mas_normalize_path_plus( MAS_LIBDIR, "dufmod", NULL ); */
  sopath = mas_strdup( duf_libpath(  ) );
  sopath = mas_strcat_x( sopath, libname );
  sopath = mas_strcat_x( sopath, ".so" );
#else
  sopath = duf_libname2sopath( libname );
#endif
  han = dlopen( sopath, RTLD_NOLOAD | RTLD_LAZY );

  es = dlerror(  );
  if ( es )
    ERRMAKE_M( MOD, "load error 1 (%s)", es );
  if ( QNOERR && !han )
  {
    han = dlopen( sopath, RTLD_LOCAL | RTLD_LAZY );

    es = dlerror(  );
    if ( es )
      ERRMAKE_M( MOD, "load error 2 (%s)", es );
  }
  if ( QNOERR && han )
  {
    psym = ( void * ) dlsym( han, symbol );

    es = dlerror(  );
    if ( es )
      ERRMAKE_M( MOD, "load error 3 (%s)", es );
    /* QT( "@%s : %s", symbol, es ); */
  }
  /* QT( "@%s", QERRNAME ); */
  MAST_TRACE( sccb, 0, "[han:%p] %s : %p", han, symbol, psym );
  mas_free( sopath );
  if ( plibhan )
    *plibhan = han;
/* return psym; */
  ERP( MOD, void *, psym, NULL, load_symbol, const char *libname, const char *symbol, void **plibhan );
}

/* const duf_mod_handler_t *                                          */
/* duf_load_mod_handler_symbol( const char *libname, void **plibhan ) */
SRP( MOD, const duf_mod_handler_t *, mhan, NULL, load_mod_handler_symbol, const char *libname, void **plibhan )
{
/* const duf_mod_handler_t *mhan = NULL; */

  MAST_TRACE( mod, 0, "@@@@@@to load lib: %s", libname );
  mhan = ( const duf_mod_handler_t * ) CRP( load_symbol, libname, "duf_mod_handler_uni", plibhan );
  MAST_TRACE( mod, 0, "%s", mhan ? mhan->name : NULL );
/* return mhan; */
  ERP( MOD, const duf_mod_handler_t *, mhan, NULL, load_mod_handler_symbol, const char *libname, void **plibhan );
}

/* void *                                                                            */
/* duf_load_mod_handler_symbol_find( const char *libname, const char *masmodsymbol ) */
SRP( MOD, void *, ptr, NULL, load_mod_handler_symbol_find, const char *libname, const char *masmodsymbol )
{
/* void *ptr = NULL; */
  void *libhan = NULL;

  for ( const duf_mod_handler_t * mhan = CRP( load_mod_handler_symbol, libname, &libhan ); QNOERR && !ptr && mhan && mhan->name; mhan++ )
  {
    if ( 0 == strcmp( mhan->name, masmodsymbol ) )
      ptr = mhan->handler;
  }
/*
   if ( unload_not_needed && !ptr && libhan )
   {
   fprintf(stderr, "closing %s\n", libname);
   dlclose( libhan );
   }
 */
/* QT( "@%s::%s ~ %p", libname, masmodsymbol, ptr ); */
/* return ptr; */
  ERP( OTHER, void *, ptr, NULL, load_mod_handler_symbol_find, const char *libname, const char *masmodsymbol );
}
