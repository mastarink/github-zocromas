#include "mas_basic_def.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include "mas_common.h"
#include "log/inc/mas_log.h"

#include "mas_modules_load_module.h"

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

  /* mMSG( "D '%s'; '%s'; '%s'; '%s';", MAS_MODULES_DIR, MAS_LIB_DIR, LT_OBJDIR, ctrl.launcherv[0] ); */
  mMSG( "to load %s", fullname );
  /* ( ( mas_cmd_fun_t * ) ( &mas_server_cmd_top ) ) = */
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

static void *
_mas_load_module( const char *libname, const char *path, int noerr )
{
  void *module_handle;
  char *fullname = NULL;

  MAS_LOG( "make path %s", path );
  fullname = mas_strdup( path );
  fullname = mas_strcat_x( fullname, libname );
  fullname = mas_strcat_x( fullname, ".so" );
  module_handle = __mas_modules_load_module( fullname, noerr );
  tMSG( "module %s loaded before:%p", libname, module_handle );
  mas_free( fullname );
  return module_handle;
}

void *
mas_modules_load_module( const char *libname )
{
  void *module_handle;
  const char *dir1 = MAS_MODULES_DIR "/" LT_OBJDIR;
  const char *dir2 = MAS_MODULESDIR "/";

  /* MAS_MODULES_DIR LT_OBJDIR */
  /* MAS_MODULESDIR */
  MAS_LOG( "load module from %s", dir1 );
  /* module_handle = _mas_load_module( libname, "src/cmdmods/.libs/", 1 ); */
  module_handle = _mas_load_module( libname, dir1, 1 );
  if ( !module_handle )
  {
    MAS_LOG( "load module from %s", dir2 );
    /* module_handle = _mas_load_module( libname, "/usr/local/lib/" PACKAGE "/", 0 ); */
    module_handle = _mas_load_module( libname, dir2, 0 );
  }
  return module_handle;
}

mas_cmd_fun_t
mas_modules_load_func( const char *libname, const char *funname )
{
  mas_cmd_fun_t cmd_fun = NULL;
  void *module_handle;

  module_handle = mas_modules_load_module( libname );
  tMSG( "module:%p", module_handle );
  if ( module_handle )
  {
    tMSG( "dlsym %s; %lx", funname, ( unsigned long ) cmd_fun );
    MAS_LOG( "dlsym %s;", funname );
    cmd_fun = ( mas_cmd_fun_t ) ( unsigned long ) dlsym( module_handle, funname );
    if ( !cmd_fun )
    {
      /* EMSG( "%s", dlerror(  ) ); */
      MAS_LOG( "NOT loaded %s : %s", funname, dlerror(  ) );
    }
    tMSG( "dlsym %s; %lx", funname, ( unsigned long ) cmd_fun );
  }
  MAS_LOG( "load func from %s => %p (mod:%p)", libname, ( void * ) ( unsigned long ) cmd_fun, ( void * ) module_handle );
  return cmd_fun;
}

mas_cmd_t *
mas_modules_load_subtable( const char *libname )
{
  mas_cmd_t *cmd_tab = NULL;
  void *module_handle;

  module_handle = mas_modules_load_module( libname );
  tMSG( "module:%p", module_handle );
  if ( module_handle )
  {
    cmd_tab = ( mas_cmd_t * ) ( unsigned long ) dlsym( module_handle, "subcmdtable" );
    tMSG( "dlsym subcmdtable (%s); %lx", libname, ( unsigned long ) cmd_tab );
  }
  MAS_LOG( "load subtable from %s => %p", libname, ( void * ) cmd_tab );
  return cmd_tab;
}
