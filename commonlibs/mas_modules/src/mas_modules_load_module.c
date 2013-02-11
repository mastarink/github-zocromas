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
__mas_modules_load_module( const char *fullname, int noerr )
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
    if ( !module_handle && !noerr )
    {
      EMSG( "%s", dlerror(  ) );
      MAS_LOG( "ERROR: module not loaded: '%s'", fullname );
    }
    MAS_LOG( "module load: '%s' (%p)", fullname, ( void * ) module_handle );
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
  module_handle = __mas_modules_load_module( fullname, noerr );
  cMSG("load module %s %s", libname, module_handle?"OK":"FAIL");
  MAS_LOG("load module %s %s", libname, module_handle?"OK":"FAIL");
  mas_free( fullname );
  return module_handle;
}

void *
mas_modules_load_module( const char *libname )
{
  void *module_handle;

  if ( opts.modsdir )
    module_handle = _mas_load_module( libname, opts.modsdir, 1 );
  return module_handle;
}

void *
mas_modules_load_proto( const char *libname )
{
  void *module_handle;

  if ( opts.protodir )
    module_handle = _mas_load_module( libname, opts.protodir, 1 );
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
    cMSG( "load cmd func %s %s", funname, cmd_fun ? "OK" : "FAIL" );
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
    tMSG( "load transaction func %s %s", funname, transaction_fun ? "OK" : "FAIL" );
    MAS_LOG( "load transaction func %s %s", funname, transaction_fun ? "OK" : "FAIL" );
    if ( !transaction_fun )
    {
      EMSG( "%s", dlerror(  ) );
      MAS_LOG( "NOT loaded %s : %s", funname, dlerror(  ) );
    }
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
