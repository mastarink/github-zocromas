#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_argvc_tools.h>

#include "mulconfnt_types.h"

#define MAST_TRACE(...) fprintf(stderr, "%s\n", __VA_ARGS__)

static int
source_check_env( int count __attribute__ ( ( unused ) ), void *data_ptr __attribute__ ( ( unused ) ), const char *delims
                  __attribute__ ( ( unused ) ), const char *eq __attribute__ ( ( unused ) ), const char *lpref
                  __attribute__ ( ( unused ) ), const char *pref __attribute__ ( ( unused ) ) )
{
  return data_ptr && getenv( ( char * ) data_ptr ) ? 1 : 0;
}

static int
source_check_direct( int count __attribute__ ( ( unused ) ), void *data_ptr __attribute__ ( ( unused ) ), const char *delims
                     __attribute__ ( ( unused ) ), const char *eq __attribute__ ( ( unused ) ), const char *lpref
                     __attribute__ ( ( unused ) ), const char *pref __attribute__ ( ( unused ) ) )
{
  return 1;
}

static char *
source_load_string_direct( config_source_desc_t * desc __attribute__ ( ( unused ) ) )
{
  return desc->data_ptr ? mas_strdup( desc->data_ptr ) : NULL;
}

static mas_argvc_t
source_load_targ_direct( config_source_desc_t * desc, mas_argvc_t targ )
{
  if ( desc->data_ptr )
    mas_add_argvc_args_d( &targ, ( char * ) desc->data_ptr, 0, desc->delims );
  return targ;
}

static char *
source_load_string_env( config_source_desc_t * desc __attribute__ ( ( unused ) ) )
{
  return desc->data_ptr ? mas_strdup( getenv( ( char * ) desc->data_ptr ) ) : NULL;
}

static mas_argvc_t
source_load_targ_env( config_source_desc_t * desc, mas_argvc_t targ )
{
  if ( desc->data_ptr )
    mas_add_argvc_args_d( &targ, getenv( ( char * ) desc->data_ptr ), 0, desc->delims );
  return targ;
}

static config_source_desc_t default_sources[] __attribute__ ( ( unused ) ) = /* */
{                                                                    /* */
  [CONFIG_SOURCE_DIRECT] =
  {                                                                  /* */
    .type = CONFIG_SOURCE_DIRECT,                                    /* */
            .count = 0,                                              /* */
            .data_ptr = NULL,                                        /* */
            .delims = ":",                                           /* */
            .eq = "=",                                               /* */
            .check_fun = source_check_direct,                        /* */
            .open_fun = NULL,                                        /* */
            .close_fun = NULL,                                       /* */
            .load_string_fun = source_load_string_direct,            /* */
            .load_targ_fun = source_load_targ_direct,                /* */
  },                                                                 /* */
          [CONFIG_SOURCE_ENV] =
  {                                                                  /* */
    .type = CONFIG_SOURCE_ENV,                                       /* */
            .count = 0,                                              /* */
            .data_ptr = "MAS_TEST_ENV",                              /* */
            .delims = ":",                                           /* */
            .eq = "=",                                               /* */
            .check_fun = source_check_env,                           /* */
            .open_fun = NULL,                                        /* */
            .close_fun = NULL,                                       /* */
            .load_string_fun = source_load_string_env,               /* */
            .load_targ_fun = source_load_targ_env,                   /* */
  },                                                                 /* */
          [CONFIG_SOURCE_ARGV] =
  {                                                                  /* */
    .type = CONFIG_SOURCE_ARGV,                                      /* */
            .count = 0,                                              /* */
            .data_ptr = NULL,                                        /* */
            .delims = 0,                                             /* */
            .eq = "=",                                               /* */
  },                                                                 /* */
          [CONFIG_SOURCE_STREAM] =
  {                                                                  /* */
    .type = CONFIG_SOURCE_STREAM,                                    /* */
            .count = 0,                                              /* */
            .data_ptr = NULL,                                        /* */
            .delims = ";\n",                                         /* */
            .eq = "=",                                               /* */
  },                                                                 /* */
};                                                                   /* */

config_source_desc_t *
mulconfnt__open( config_source_desc_t ** plist, config_source_t source_type
                 __attribute__ ( ( unused ) ), int count, void *data_ptr, const char *delims, const char *eq, const char *lpref, const char *pref )
{
  config_source_desc_t *optr = NULL;

  if ( source_type >= 0 && source_type < sizeof( default_sources ) / sizeof( default_sources[0] ) )
  {
    config_source_desc_t *defsrc = &default_sources[source_type];

    if ( defsrc && source_type == defsrc->type )
    {
      int check = !defsrc->check_fun || defsrc->check_fun( count, data_ptr, delims, eq, lpref, pref );

      fprintf( stderr, "CHECK (%s): %d\n", ( char * ) data_ptr, check );
      if ( check )
      {
        optr = mas_malloc( sizeof( config_source_desc_t ) );
        memset( optr, 0, sizeof( config_source_desc_t ) );

        optr->type = source_type;
        optr->count = count;

        if ( delims )
          optr->delims = mas_strdup( delims );
        else if ( defsrc->delims )
          optr->delims = mas_strdup( defsrc->delims );

        if ( eq )
          optr->eq = mas_strdup( eq );
        else if ( defsrc->eq )
          optr->eq = mas_strdup( defsrc->eq );

        if ( lpref )
          optr->lpref = mas_strdup( lpref );
        else if ( defsrc->lpref )
          optr->lpref = mas_strdup( defsrc->lpref );

        if ( pref )
          optr->pref = mas_strdup( pref );
        else if ( defsrc->pref )
          optr->pref = mas_strdup( defsrc->pref );

        optr->data_ptr = data_ptr ? data_ptr : defsrc->data_ptr;
        optr->check_fun = defsrc->check_fun;
        optr->open_fun = defsrc->open_fun;
        optr->close_fun = defsrc->close_fun;
        optr->load_string_fun = defsrc->load_string_fun;
        optr->load_targ_fun = defsrc->load_targ_fun;
      }
    }
  }
  if ( plist )
  {
    if ( *plist )
    {
      config_source_desc_t *list = *plist;

      while ( list && list->next )
        list = list->next;
      list->next = optr;
    }
    else
      *plist = optr;
  }
  if ( optr )
    optr->next = NULL;
  return optr;
}

char *
mulconfnt_load_string( config_source_desc_t * optr )
{
  if ( optr )
  {
    if ( optr->string )
      mas_free( optr->string );
    optr->string = NULL;
    if ( optr && optr->load_string_fun )
      optr->string = optr->load_string_fun( optr );
  }
  return optr ? optr->string : NULL;
}

mas_argvc_t *
mulconfnt_load_targ( config_source_desc_t * optr )
{
  if ( optr )
  {
    mas_argvc_delete( &optr->targ );
    if ( optr && optr->load_targ_fun )
      optr->targ = optr->load_targ_fun( optr, optr->targ );
  }
  return optr ? &optr->targ : NULL;
}

int
mulconfnt_list_targ( mas_argvc_t * targ, config_source_desc_t * list )
{
  for ( config_source_desc_t * conf = list; conf; conf = conf->next )
  {
    mas_add_argvc_argvc( targ, &conf->targ, 0 );
  }
  return 0;
}

int
mulconfnt_close( config_source_desc_t * conf )
{
  if ( conf )
  {
    if ( conf->delims )
      mas_free( conf->delims );
    if ( conf->eq )
      mas_free( conf->eq );
    if ( conf->lpref )
      mas_free( conf->lpref );
    if ( conf->pref )
      mas_free( conf->pref );
    if ( conf->string )
      mas_free( conf->string );
    mas_argvc_delete( &conf->targ );
    if ( conf->next )
      mulconfnt_close( conf->next );
    mas_free( conf );
  }
  return 0;
}

int
mulconfnt__close( config_source_desc_t ** pconf )
{
  int r = 0;

  if ( pconf )
  {
    r = mulconfnt_close( *pconf );
    *pconf = NULL;
  }
  return r;
}

int
mulconfnt_parse( int argc __attribute__ ( ( unused ) ), char *argv[] __attribute__ ( ( unused ) ) )
{
  char *envname = "ENVVAR";
  config_source_desc_t *list = NULL;
  config_source_desc_t *conf = NULL;

  fprintf( stderr, "%p:%s\n", envname, envname );

  conf = mulconfnt__open( &list, CONFIG_SOURCE_DIRECT, 0, "first=45 another=37:second=49;third=23", ":", "=", 0, 0 );
  fprintf( stderr, "string >>>> %s\n", mulconfnt_load_string( conf ) );

  mas_argvc_t *targ1 = mulconfnt_load_targ( conf );

  fprintf( stderr, "TARG:%d\n", conf->targ.argc );

  if ( targ1->argc )
  {
    for ( int i = 0; i < targ1->argc; i++ )
    {
      fprintf( stderr, "targ1 >>>> %d : %s;\n", i, targ1->argv[i] );
    }
  }
//  mulconfnt__close( &conf );

  fprintf( stderr, "TARG:%d\n", conf->targ.argc );
  conf = mulconfnt__open( &list, CONFIG_SOURCE_ENV, 0, "MAS_TEST_ENV", ":", "=", 0, 0 );
  fprintf( stderr, "string >>>> %s\n", mulconfnt_load_string( conf ) );
  fprintf( stderr, "TARG:%d\n", conf->targ.argc );
  mas_argvc_t *targ2 = mulconfnt_load_targ( conf );

  if ( targ2->argc )
  {
    for ( int i = 0; i < targ2->argc; i++ )
    {
      fprintf( stderr, "targ2 >>>> %d : %s;\n", i, targ2->argv[i] );
    }
  }
  fprintf( stderr, "-1--------------------------------------------------------------------\n" );
  if ( targ1->argc && targ2->argc )
  {
    if ( targ1->argc )
    {
      for ( int i = 0; i < targ1->argc; i++ )
      {
        fprintf( stderr, "targ1 >>>> %d : %s;\n", i, targ1->argv[i] );
      }
    }
    fprintf( stderr, "-2--------------------------------------------------------------------\n" );
    if ( targ2->argc )
    {
      for ( int i = 0; i < targ2->argc; i++ )
      {
        fprintf( stderr, "targ2 >>>> %d : %s;\n", i, targ2->argv[i] );
      }
    }
    mas_argvc_t targ = { 0 };
    mulconfnt_list_targ( &targ, list );

    fprintf( stderr, "-3--------------------------------------------------------------------\n" );
    if ( targ.argc )
    {
      for ( int i = 0; i < targ.argc; i++ )
      {
        fprintf( stderr, "targ >>>> %d : %s;\n", i, targ.argv[i] );
      }
    }
    mas_argvc_delete( &targ );
  }
  mulconfnt__close( &list );

#if 0
  {
    conf = mulconfnt_open( CONFIG_SOURCE_DIRECT, 0, "first=45:next=49", ":", '=', 0, 0 );
    mulconfnt_open( CONFIG_SOURCE_ENV, 0, envname, ":" );
    mulconfnt_open( CONFIG_SOURCE_ARGV, argc, argv, 0 );
    mulconfnt_open( CONFIG_SOURCE_CONFIG, 0, NULL, ";\n" );
    mulconfnt_open( CONFIG_SOURCE_FILE, 0, "mas.cfg", ";\n" );
    mulconfnt_open( CONFIG_SOURCE_STREAM, 0, stdin, ";\n" );
  }
#endif
/*
 * sources:
 *   1. argv nextarg: iarg++; getarg: argv[iarg]; delim:0
 *   2a. hard-coded config file: open, 
 *   2b. config file: nextarg: fgets; getarg:...; delim: '\n'
 *   3. * FILE
 *   4. env
 *   5. stdin
 *   6. interactive / readline
 * */
  fprintf( stderr, "%s : %d:%p\n", __func__, argc, argv );
  {
    struct stat st_stdin;

    if ( fstat( STDIN_FILENO, &st_stdin ) >= 0 )
    {
      switch ( st_stdin.st_mode & S_IFMT )
      {
      case S_IFBLK:
        MAST_TRACE( "block device" );
        break;
      case S_IFCHR:
        MAST_TRACE( "character device" );                            /* Y */
        break;
      case S_IFDIR:
        MAST_TRACE( "directory" );
        break;
      case S_IFIFO:
        MAST_TRACE( "FIFO/pipe" );                                   /* cmd | this */
        break;
      case S_IFLNK:
        MAST_TRACE( "symlink" );
        break;
      case S_IFREG:
        MAST_TRACE( "regular file" );                                /* this < file */
        break;
      case S_IFSOCK:
        MAST_TRACE( "socket" );
        break;
      default:
        MAST_TRACE( "unknown?" );
        break;
      }
    }
  }
  return 0;
}
