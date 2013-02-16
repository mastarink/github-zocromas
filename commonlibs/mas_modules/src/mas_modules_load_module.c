#include <mastar/wrap/mas_std_def.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/types/mas_control_types.h>
#include <mastar/types/mas_opts_types.h>
extern mas_control_t ctrl;
extern mas_options_t opts;


#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/log/mas_log.h>


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



static void *
__mas_modules_load_module( const char *fullname, const char *name, int noerr )
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
    if ( !module_handle )
    {
      char *dler;

      dler = dlerror(  );
      dler = mas_strdup( dler );
      if ( !noerr )
      {
        MAS_LOG( "ERROR: module not loaded: '%s' (%s)", fullname, dler );
        EMSG( "%s", dler );
      }
      if ( dler )
        mas_free( dler );
    }
    MAS_LOG( "module load: '%s' (%p)", fullname, ( void * ) module_handle );
    if ( module_handle )
    {
      unsigned cnt, size;
      void **array;

      pthread_rwlock_wrlock( &ctrl.thglob.modules_list_rwlock );
      array = ctrl.loaded_modules;
      /* ctrl.loaded_modules = NULL;  */
      /* ctrl.loaded_modules_cnt = 0; */
      cnt = ctrl.loaded_modules_cnt;
      size = cnt + 1;

      if ( array )
        array = mas_realloc( array, sizeof( void * ) * size );
      else
        array = mas_malloc( sizeof( void * ) * size );
      array[cnt] = module_handle;

      ctrl.loaded_modules = array;
      ctrl.loaded_modules_cnt = size;
      pthread_rwlock_unlock( &ctrl.thglob.modules_list_rwlock );
      HMSG( "REG.MODULE %u. %s", ctrl.loaded_modules_cnt, name );
    }
  }
  return module_handle;
}

void *
_mas_load_module( const char *libname, const char *path, int noerr )
{
  void *module_handle;
  char *fullname = NULL;

  MAS_LOG( "make path %s", path );
  fullname = mas_strdup( path );
  fullname = mas_strcat_x( fullname, libname );
  fullname = mas_strcat_x( fullname, ".so" );
  module_handle = __mas_modules_load_module( fullname, libname, noerr );
  HMSG( "LOAD MOD %s %s", libname, module_handle ? "OK" : "FAIL" );
  tMSG( "load module %s %s", libname, module_handle ? "OK" : "FAIL" );
  MAS_LOG( "load module %s %s", libname, module_handle ? "OK" : "FAIL" );
  mas_free( fullname );
  return module_handle;
}

void *
mas_modules_load_module( const char *libname )
{
  void *module_handle = NULL;

  if ( opts.modsdir )
    module_handle = _mas_load_module( libname, opts.modsdir, 1 );
  return module_handle;
}

void *
mas_modules_load_proto( const char *libname )
{
  void *module_handle = NULL;

  if ( opts.protodir )
    module_handle = _mas_load_module( libname, opts.protodir, 1 );
  HMSG( "PROTO LOAD %s %s", libname, module_handle ? "OK" : "FAIL" );
  return module_handle;
}

mas_cmd_fun_t
mas_modules_load_cmd_func( const char *libname, const char *funname )
{
  mas_cmd_fun_t cmd_fun = NULL;
  void *module_handle;

  module_handle = mas_modules_load_module( libname );
  if ( module_handle )
  {
    cmd_fun = ( mas_cmd_fun_t ) ( unsigned long ) dlsym( module_handle, funname );
    tMSG( "load cmd func %s %s", funname, cmd_fun ? "OK" : "FAIL" );
    EHMSG( cmd_fun, "LOAD CMD FUNC %s.%s %s", libname, funname, cmd_fun ? "OK" : "FAIL" );
    MAS_LOG( "load cmd func %s %s", funname, cmd_fun ? "OK" : "FAIL" );
    if ( !cmd_fun )
    {
      EMSG( "%s", dlerror(  ) );
      MAS_LOG( "NOT loaded %s : %s", funname, dlerror(  ) );
    }
  }
  return cmd_fun;
}

mas_transaction_fun_t
mas_modules_load_proto_func( const char *libname, const char *funname )
{
  mas_transaction_fun_t transaction_fun = NULL;
  void *module_handle;

  module_handle = mas_modules_load_proto( libname );
  if ( module_handle )
  {
    transaction_fun = ( mas_transaction_fun_t ) ( unsigned long ) dlsym( module_handle, funname );
  }
  tMSG( "load transaction func %s %s", funname, transaction_fun ? "OK" : "FAIL" );
  MAS_LOG( "load transaction func %s %s", funname, transaction_fun ? "OK" : "FAIL" );
  if ( !transaction_fun )
  {
    EMSG( "%s", dlerror(  ) );
    MAS_LOG( "NOT loaded %s : %s", funname, dlerror(  ) );
  }
  return transaction_fun;
}

mas_cmd_t *
mas_modules_load_subtable( const char *libname )
{
  mas_cmd_t *cmd_tab = NULL;
  void *module_handle;

  module_handle = mas_modules_load_module( libname );
  if ( module_handle )
    cmd_tab = ( mas_cmd_t * ) ( unsigned long ) dlsym( module_handle, "subcmdtable" );
  MAS_LOG( "load subtable from %s => %p", libname, ( void * ) cmd_tab );
  return cmd_tab;
}

void
mas_modules_destroy( void )
{
  if ( ctrl.loaded_modules_cnt && ctrl.loaded_modules )
  {
    for ( int im = 0; im < ctrl.loaded_modules_cnt; im++ )
    {
      HMSG( "MODULE DESTROY %u. %p", im, ( void * ) ctrl.loaded_modules[im] );
      dlclose( ctrl.loaded_modules[im] );
      ctrl.loaded_modules[im] = NULL;
    }
    ctrl.loaded_modules_cnt = 0;
    mas_free( ctrl.loaded_modules );
    ctrl.loaded_modules = NULL;
  }
}
