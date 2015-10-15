#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "mas_arg_tools.h"
#include "mas_argvc_tools.h"


int
mas_add_argvc_args( mas_argvc_t * targ, const char *args, int ia_offset )
{
  return ( targ->argc = mas_add_argv_args( targ->argc, &targ->argv, args, ia_offset ) );
}

int
mas_add_argvc_argvc( mas_argvc_t * targ, mas_argvc_t * arg, int ia_offset )
{
  return ( targ->argc = mas_add_argv_argv( targ->argc, &targ->argv, arg->argc, arg->argv, ia_offset ) );
}

int
mas_add_argvc_arg_nodup( mas_argvc_t * targ, char *arg )
{
  targ->argc = mas_add_argv_arg_nodup( targ->argc, &targ->argv, arg );
  return targ->argc;
}

int
mas_add_argvc_arg( mas_argvc_t * targ, const char *arg )
{
  if ( arg )
    targ->argc = mas_add_argvc_arg_nodup( targ, mas_strdup( arg ) );
  return targ->argc;
}

int
mas_add_argvc_cmdline( mas_argvc_t * targ, const char *cmdline, size_t len )
{
  return ( targ->argc = mas_add_argv_cmdline( targ->argc, &targ->argv, cmdline, len ) );
}

int
mas_del_argvc( mas_argvc_t * targ, int from_a )
{
  targ->argc = mas_del_argv( targ->argc, targ->argv, from_a );
  targ->argv = NULL;
  return targ->argc;
}

int
mas_argvc_delete( mas_argvc_t * targ )
{
  return mas_del_argvc( targ, 0 );
}

int
mas_argvc_clone( mas_argvc_t * darg, mas_argvc_t * sarg )
{
  return ( darg->argc = mas_argv_clone( &darg->argv, sarg->argc, sarg->argv ) );
}

char *
mas_argvc_join( mas_argvc_t * targ, int from_a, char delim )
{
  return mas_argv_join( targ->argc, targ->argv, from_a, delim );
}

char *
mas_argvc_string( mas_argvc_t * targ, int from_a )
{
  return mas_argvc_join( targ, from_a, ' ' );
}
