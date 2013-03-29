#include <mastar/wrap/mas_std_def.h>

/* #include <stdio.h> */
#include <stdlib.h>
/* #include <unistd.h> */
#include <wait.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib0.h>
#include <mastar/tools/mas_arg_tools.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>

#ifdef MAS_USE_CURSES
#include <mastar/msg/mas_curses.h>
#endif

#include <mastar/modules/mas_modules_commands_eval.h>
#include <mastar/modules/mas_modules_commands.h>


/*
this:
  mas_cmdmod_root.c
related:
  mas_cmdmod_get.c
  mas_cmdmod_client.c
  mas_cmdmod_mem.c
  mas_cmdmod_do.c
  mas_cmdmod_ctrl.c
  mas_cmdmod_set.c
  mas_modules_commands.c
  mas_modules_commands_eval.c
more:
  mas_watcher.c
  mas_ticker.c
*/





char *
test_cmd( STD_CMD_ARGS )
{
  return mas_strdup( args );
}

char *
exec_cmd( STD_CMD_ARGS )
{
  pid_t pid;
  char *s = NULL;

  pid = mas_fork(  );
  if ( pid == 0 )
  {                             /* child:  "cat a"                      */
    int r;

    /* use_curses = 0; */
    RP_ERR( execlp( args, "", NULL ) );
    _exit( 0 );                 /* no flush                             */
  }
  else if ( pid > 0 )
  {                             /* parent:  "/usr/bin/wc"               */
    mas_waitpid( pid, NULL, WNOHANG );
  }
  else
  {
    P_ERR;
  }
  return s;
}

char *
system_cmd( STD_CMD_ARGS )
{
  int r = -2;
  int pfildes[2];
  pid_t pid;
  char *s = NULL;

  RP_ERR( mas_pipe( pfildes ) );

  pid = mas_fork(  );
  if ( pid == 0 )
  {                             /* child:  "cat a"                      */
    int argc = 0;
    char **argv = NULL;

    /* use_curses = 0; */
#ifdef MAS_USE_CURSES
    if ( !use_curses )
#endif
      cMSG( "system_cmd pid=%u", getpid(  ) );
    mas_close( pfildes[0] );    /* close read end of pipe               */
    dup2( pfildes[1], STDOUT_FILENO ); /* make 1 same as write-to end of pipe  */
    mas_close( pfildes[1] );    /* close excess fildes                  */
#ifdef MAS_USE_CURSES
    if ( !use_curses )
#endif
      cMSG( "Args:'%s'", args );

    /* argc = mas_make_argv( args, 0, &argv, 0 ); */
    argc = mas_add_argv_args( argc, &argv, args, 0 );

#ifdef MAS_USE_CURSES
    if ( !use_curses )
#endif
      cMSG( "argc:%d", argc );
    if ( argc && argv )
    {
#ifdef MAS_USE_CURSES
      if ( !use_curses )
#endif
        cMSG( "exec ARgs:'%s'", args );
      RP_ERR( execvp( argv[0], &argv[0] ) );
#ifdef MAS_USE_CURSES
      if ( !use_curses )
#endif
        cMSG( "after exec ARGs:'%s'", args );
    }
    else
    {
      EMSG( "argv" );
    }
    mas_del_argv( argc, argv, 0 );
    /* RP_ERR( execlp( args, "", NULL ) ); */

#ifdef MAS_USE_CURSES
    if ( !use_curses )
#endif
      cMSG( "execlp: %d", r );
    _exit( 0 );                 /* no flush                             */
  }
  else if ( pid > 0 )
  {                             /* parent:  "/usr/bin/wc"               */
    int nr;
    char *tbuf;
    size_t tbufsz;

    tbufsz = 1025;
    tbuf = mas_malloc( tbufsz );

    mas_close( pfildes[1] );    /* close write end of pipe              */
    dup2( pfildes[0], STDIN_FILENO ); /* make 0 same as read-from end of pipe */
    mas_close( pfildes[0] );    /* close excess fildes                  */
    while ( ( nr = read( STDIN_FILENO, tbuf, tbufsz - 1 ) ) > 0 )
      s = mas_strncat_x( s, tbuf, nr );
    mas_free( tbuf );

    mas_waitpid( pid, NULL, 0 );
  }
  else
  {
    P_ERR;
  }
  return s;
}

/*==========================================================================================*/
char *
getpwd_cmd( STD_CMD_ARGS )
{
  char *p = NULL;
  char *r = NULL;

  p = get_current_dir_name(  );
  r = mas_strdup( p );
  mas_other_free( p );
  return r;
}

char *
getcd_cmd( STD_CMD_ARGS )
{
  char *p = NULL;
  char *r = NULL;

  /* cMSG( "ARGS:%s", args ); */
  if ( chdir( args ) >= 0 )
  {
    p = get_current_dir_name(  );
    r = mas_strdup( p );
    mas_other_free( p );
  }
  else
  {
    if ( prcontrol )
      prcontrol->qbin = MSG_BIN_ERROR_IN_COMMAND;
  }
  return r;
}

/*==========================================================================================*/

mas_cmd_t subcmdtable[] = {
  {0, NULL, list_commands_cmd, NULL} /*  */
  ,
/*==========================================================================================*/
  {.name = "test",.function = NULL,.libname = "test"} /* server ... */
  ,
  {.name = "server",.function = NULL,.libname = "server"} /* server ... */
  ,
  {.name = "server_old",.function = NULL,.libname = "server_old"} /* server_old ... */
  ,
  {.name = "tcmd",.function = test_cmd,.libname = NULL} /* test ... */
  ,
/*==========================================================================================*/
  {3,.name = "system",.function = system_cmd,.libname = NULL} /* system ... */
  ,
  {4,.name = "!",.function = system_cmd,.libname = NULL} /* ! ... */
  ,
  {5,.name = "exec",.function = exec_cmd,.libname = NULL} /* exec ... */
  ,
  {6,.name = "do",.function = NULL,.libname = "do",.only_level = 0} /* do ... */
  ,
  {7,.name = "check",.function = NULL,.libname = "check"} /* check ... */
  ,
/*==========================================================================================*/
  {8,.name = "pwd",.function = getpwd_cmd,.libname = NULL} /* pwd */
  ,
  {9,.name = "cd",.function = getcd_cmd,.libname = NULL} /* cd */
  ,
/*==========================================================================================*/
  {10,.name = "get",.function = NULL,.libname = "get",.only_level = 0} /* get ... */
  ,
  {11,.name = "set",.function = NULL,.libname = "set"} /* set ... */
  ,
  {12,.name = "show",.function = NULL,.libname = "show"} /* show */
  ,
  {13,.name = "list",.function = NULL,.libname = "list"} /* list */
  ,
  {14,.name = "notify",.function = NULL,.libname = "notify"} /* notify */
  ,
  {15,.name = "ctrl",.function = NULL,.libname = "ctrl"} /* ctrl */
  ,
  {16,.name = "nof",.function = NULL,.libname = NULL} /* nof */
  ,
/*==========================================================================================*/
  {999, NULL, NULL, NULL}
};
