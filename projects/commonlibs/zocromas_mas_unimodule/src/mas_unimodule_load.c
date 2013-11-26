#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>


#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
#include <mastar/log/mas_log.h>


#include "mas_unimodule_ctrl.h"

#include "mas_unimodule_load.h"


/* #define MAS_MODULES_DIR "/tmp/zoc" */

/*
this:
  mas_unimodule_load_module.c
related:
  mas_modules_commands.c
  mas_modules_commands.h
  mas_modules_commands_eval.c
  mas_modules_commands_eval.h
  mas_unimodule_load_module.h
  mas_unimodule_types.h
more:
  mas_control.c
  mas_opts.c

*/


static void *
_mas_unimodule_load_fullmodule( const char *fullname, const char *name, int noerr )
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
      if ( dler )
        dler = mas_strdup( dler );
      if ( !noerr )
      {
        EMSG( "%s", dler );
      }
      MAS_LOG( "ERROR: module not loaded: '%s' (%s)", fullname, dler );
      if ( dler )
        mas_free( dler );
    }
    WMSG( "_LOAD MOD %s %s", fullname, module_handle ? "OK" : "FAIL" );
    MAS_LOG( "module load: '%s' (%p)", fullname, ( void * ) module_handle );
    if ( module_handle )
      mas_unimodule_register_module( name, module_handle );
  }
  return module_handle;
}

void *
mas_unimodule_load_module_from( const char *libname, const char *modpath, int noerr )
{
  void *module_handle;
  char *fullname = NULL;

  MAS_LOG( "make modpath %s", modpath );
  fullname = mas_strdup( modpath );
  fullname = mas_strcat_x( fullname, "/" );
  fullname = mas_strcat_x( fullname, libname );
  fullname = mas_strcat_x( fullname, ".so" );
  module_handle = _mas_unimodule_load_fullmodule( fullname, libname, noerr );
  HMSG( "LOAD MOD %s %s", libname, module_handle ? "OK" : "FAIL" );
  tMSG( "load module %s %s", libname, module_handle ? "OK" : "FAIL" );
  MAS_LOG( "load module %s %s", libname, module_handle ? "OK" : "FAIL" );
  mas_free( fullname );
  return module_handle;
}

mas_any_fun_t
mas_unimodule_load_func_from( const char *libname, const char *funname, const char *modpath )
{
  mas_any_fun_t any_fun = NULL;

  /* any_fun = ( mas_any_fun_t ) ( unsigned long ) dlsym( module_handle, funname ); */
  any_fun = ( mas_any_fun_t ) ( unsigned long long ) mas_unimodule_load_symbol_from( libname, funname, modpath );
  if ( !any_fun )
  {
    char *dler;

    dler = dlerror(  );
    if ( dler )
      dler = mas_strdup( dler );
    MAS_LOG( "NOT loaded %s : %s", funname, dler );
    EMSG( "NOT loaded %s : %s", funname, dler );
    if ( dler )
      mas_free( dler );
  }

  /* else                                                             */
  /* {                                                                */
  /*   EMSG( "NO module_handle passed for %s.%s", libname, funname ); */
  /* }                                                                */
  tMSG( "load transaction func %s %s", funname, any_fun ? "OK" : "FAIL" );
  MAS_LOG( "load transaction func %s %s", funname, any_fun ? "OK" : "FAIL" );
  HMSG( "LOAD FUNC %s %s %s", libname, funname, any_fun ? "OK" : "FAIL" );
  return any_fun;
}

void *
mas_unimodule_load_symbol_from( const char *libname, const char *symname, const char *modpath )
{
  void *msymb = NULL;
  void *module_handle;

  /* module_handle = _mas_unimodule_load_module( libname ); */
  module_handle = mas_unimodule_load_module_from( libname, modpath, 0 );
  if ( module_handle )
  {
    msymb = ( void * ) ( unsigned long ) dlsym( module_handle, symname );
    if ( !msymb )
    {
      char *dler;

      dler = dlerror(  );
      if ( dler )
        dler = mas_strdup( dler );
      EMSG( "NOT loaded symbol %s; %s", symname, dler );
      if ( dler )
        mas_free( dler );
    }
  }
  MAS_LOG( "load subtable from %s => %p", libname, ( void * ) msymb );
  HMSG( "LOAD SYM %s %s %s", libname, symname, msymb ? "OK" : "FAIL" );
  return msymb;
}
