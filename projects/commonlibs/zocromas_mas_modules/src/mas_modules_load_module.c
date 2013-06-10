#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/log/mas_log.h>

#include <mastar/types/mas_common_defs.h>

#include "mas_modules_ctrl_module.h"

#include "mas_modules_load_module.h"

/* #define MAS_MODULES_DIR "/tmp/zoc" */

/*
this:
  mas_modules_load_module.c
related:
  mas_modules_commands.c
  mas_modules_commands.h
  mas_modules_commands_eval.c
  mas_modules_commands_eval.h
  mas_modules_load_module.h
  mas_modules_types.h
more:
  mas_control.c
  mas_opts.c

*/
__attribute__ ( ( constructor ) )
     static void master_constructor( void )
{
  /* fprintf( stderr, "******************** CONSTRUCTOR %s e%d\n", __FILE__, errno ); */
}

static void *
_mas_modules_load_fullmodule( const char *fullname, const char *name, int noerr )
{
  void *module_handle;

  module_handle = dlopen( fullname, RTLD_NOLOAD | RTLD_LAZY | RTLD_LOCAL );
  if ( module_handle )
  {
    MAS_LOG( "module already loaded: '%s' (%p)", fullname, ( void * ) module_handle );
  }
  else
  {
    module_handle = dlopen( fullname, RTLD_LAZY | RTLD_LOCAL );
    WMSG( "_LOAD MOD %s %s", fullname, module_handle ? "OK" : "FAIL" );
    if ( !module_handle )
    {
      char *dler;

      dler = dlerror(  );
      dler = mas_strdup( dler );
      if ( !noerr )
      {
        EMSG( "%s", dler );
      }
      MAS_LOG( "ERROR: module not loaded: '%s' (%s)", fullname, dler );
      if ( dler )
        mas_free( dler );
    }
    MAS_LOG( "module load: '%s' (%p)", fullname, ( void * ) module_handle );
    mas_modules_register_module( name, module_handle );
  }
  return module_handle;
}

void *
mas_modules_load_module_from( const char *libname, const char *path, int noerr )
{
  void *module_handle;
  char *fullname = NULL;

  MAS_LOG( "make path %s", path );
  fullname = mas_strdup( path );
  fullname = mas_strcat_x( fullname, "/" );
  fullname = mas_strcat_x( fullname, libname );
  fullname = mas_strcat_x( fullname, ".so" );
  module_handle = _mas_modules_load_fullmodule( fullname, libname, noerr );
  WMSG( "LOAD MOD %s %s", libname, module_handle ? "OK" : "FAIL" );
  tMSG( "load module %s %s", libname, module_handle ? "OK" : "FAIL" );
  MAS_LOG( "load module %s %s", libname, module_handle ? "OK" : "FAIL" );
  mas_free( fullname );
  return module_handle;
}

/* static void *                                                                 */
/* _mas_modules_load_module( const char *libname )                               */
/* {                                                                             */
/*   void *module_handle = NULL;                                                 */
/*                                                                               */
/*   MAS_LOG( "load module %s @ %s", libname, opts.dir.mods );                    */
/*   if ( opts.dir.mods )                                                         */
/*     module_handle = mas_modules_load_module_from( libname, opts.dir.mods, 1 ); */
/*   return module_handle;                                                       */
/* }                                                                             */

/* void *                                                                                    */
/* mas_modules_load_proto( const char *libname )                                             */
/* {                                                                                         */
/*   void *module_handle = NULL;                                                             */
/*                                                                                           */
/*   MAS_LOG( "load proto %s @ %s", libname, opts.dir.proto );                                */
/*   if ( opts.dir.proto )                                                                    */
/*     module_handle = mas_modules_load_module_from( libname, opts.dir.proto, 1 );            */
/*   WMSG( "PROTO LOAD %s @ %s %s", libname, opts.dir.proto, module_handle ? "OK" : "FAIL" ); */
/*   return module_handle;                                                                   */
/* }                                                                                         */

mas_any_fun_t
mas_modules_load_func_from( const char *libname, const char *funname, const char *path )
{
  mas_any_fun_t any_fun = NULL;

  /* void *module_handle; */

  /* module_handle = mas_modules_load_module_from( libname, path, 1 ); */
  /* if ( module_handle ) */
  {
    /* any_fun = ( mas_any_fun_t ) ( unsigned long ) dlsym( module_handle, funname ); */
    any_fun = ( mas_any_fun_t ) ( unsigned long long ) mas_modules_load_symbol_from( libname, funname, path );
    if ( !any_fun )
    {
      EMSG( "%s", dlerror(  ) );
      MAS_LOG( "NOT loaded %s : %s", funname, dlerror(  ) );
    }
  }
  /* else                                                             */
  /* {                                                                */
  /*   EMSG( "NO module_handle passed for %s.%s", libname, funname ); */
  /* }                                                                */
  tMSG( "load transaction func %s %s", funname, any_fun ? "OK" : "FAIL" );
  MAS_LOG( "load transaction func %s %s", funname, any_fun ? "OK" : "FAIL" );
  return any_fun;
}

void *
mas_modules_load_symbol_from( const char *libname, const char *symname, const char *path )
{
  void *msymb = NULL;
  void *module_handle;

  /* module_handle = _mas_modules_load_module( libname ); */
  module_handle = mas_modules_load_module_from( libname, path, 1 );
  if ( module_handle )
    msymb = ( void * ) ( unsigned long ) dlsym( module_handle, symname );
  MAS_LOG( "load subtable from %s => %p", libname, ( void * ) msymb );
  return msymb;
}
