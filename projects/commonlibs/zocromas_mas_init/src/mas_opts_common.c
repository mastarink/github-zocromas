#include <mastar/wrap/mas_std_def.h>
#include <mastar/types/mas_common_defs.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/wrap/mas_lib.h>

#include <mastar/tools/mas_arg_tools.h>

#include <mastar/msg/mas_msg_def.h>
#include <mastar/msg/mas_msg_tools.h>
/* #include <mastar/log/mas_log.h> */

#include "mas_opts_common.h"


/*
this:
  mas_opts_common.c
related:
  mas_opts.c
  mas_opts_types.h
  mas_opts_data.c
  mas_msg_tools.c
  mas_control.c

*/

int
mas_opts_restore_nosection( void *popts, const char *section, const char *sectvalue, const char *s, mas_option_parse_t * opt_table,
                            size_t opt_table_size, void *arg )
{
  int r = -1;

  for ( int io = 0; io < opt_table_size; io++ )
  {
    char *ptr = NULL;
    char **pptr = NULL;

    /* char *sptr; */

    if ( ( !( opt_table[io].section || section )
           || ( opt_table[io].section && section && 0 == strncmp( section, opt_table[io].section, strlen( opt_table[io].section ) ) ) )
         && ( opt_table[io].name && s && 0 == strncmp( s, opt_table[io].name, strlen( opt_table[io].name ) ) ) )
    {
      const char *se;

      r = 0;
      HMSG( "%d. %s(%s).%s (%lu:%lu) : %s", io, opt_table[io].section, section, opt_table[io].name, ( unsigned long ) opt_table[io].type,
            ( unsigned long ) opt_table[io].offset, s );
      se = mas_find_eq_value( s );
      switch ( opt_table[io].type )
      {
      case MAS_OPT_TYPE_NONE:
        break;
      case MAS_OPT_TYPE_INT:
        break;
      case MAS_OPT_TYPE_DOUBLE:
        if ( se )
        {
          double *pdoub;

          pdoub = ( double * ) ( ( ( char * ) popts ) + opt_table[io].offset );
          sscanf( se, "%lg", pdoub );
        }
        break;
      case MAS_OPT_TYPE_STR:
        ptr = ( ( ( char * ) popts ) + opt_table[io].offset );
        if ( se && ( strlen( se ) < opt_table[io].size ) )
          strcpy( ptr, se );
        break;
      case MAS_OPT_TYPE_PSTR:
        pptr = ( ( char ** ) ( ( ( char * ) popts ) + opt_table[io].offset ) );
        if ( se )
        {
          /* HMSG( "to set '%s' < '%s'", *pptr, se ); */
          if ( *pptr )
            mas_free( *pptr );
          *pptr = se && ( 0 != strcmp( se, "(null)" ) ) ? mas_strdup( se ) : NULL;
        }
        ptr = *pptr;
        break;
      case MAS_OPT_TYPE_FUNC:
        {
          mas_opts_func_t func;

          func = opt_table[io].func;
          if ( func )
          {
            func( arg, section, sectvalue, opt_table[io].name, se );
          }
          if ( r < 0 )
          {
            IEVAL( r, -1 );
          }
        }
        break;
      }
      /* HMSG( "%d. %s (%lu:%lu) : %s ? %s", io, opt_table[io].name, ( unsigned long ) opt_table[io].type, */
      /*       ( unsigned long ) opt_table[io].offset, ptr, se );                                          */
    }
  }
  return r;
}

int
_mas_opts_restore_relative( const char *filename, void *popts, mas_option_parse_t * opt_table, size_t opt_table_size, void *arg,
                            mas_new_section_func_t new_section_func, mas_at_section_func_t at_section_func,
                            mas_unknown_opt_func_t unknown_opt_func )
{
  int r = 0;
  char *fpath = NULL;

  /* for .dir.config only! */
  extern mas_options_t opts;

  fpath = mas_strdup( opts.dir.config );
  fpath = mas_strcat_x( fpath, "/" );
  fpath = mas_strcat_x( fpath, filename );
  r = _mas_opts_restore_path( fpath, popts, opt_table, opt_table_size, arg, new_section_func, at_section_func, unknown_opt_func );
  mas_free( fpath );
  return r;
}

int
_mas_opts_restore_path( const char *fpath, void *popts, mas_option_parse_t * opt_table, size_t opt_table_size, void *arg,
                        mas_new_section_func_t new_section_func, mas_at_section_func_t at_section_func,
                        mas_unknown_opt_func_t unknown_opt_func )
{
  int r = 0;

  if ( fpath )
  {
    FILE *f;

    /* HMSG( "offsetof : %lu", ( unsigned long ) offsetof( mas_options_t, ticker_mode ) ); */
    HMSG( "OPTS from %s", fpath );
    f = mas_fopen( fpath, "r" );
    if ( f )
    {
      r = 0;
      char buf[4096];
      char *section = NULL;
      char *sectvalue = NULL;

      /* section=mas_strdup("DEFAULT"); */
      while ( !feof( f ) )
      {
        char *s, *rbp, *spp;
        size_t len;

        s = fgets( buf, sizeof( buf ), f );
        while ( s && *s && *s <= ' ' )
          s++;
        if ( !s || !*s || *s == '#' )
          continue;
        s = mas_chomp( s );
        /* HMSG( "OPTS read :'%s'", s ); */
        if ( s && *s )
          r++;
        rbp = strchr( s, ']' );
        spp = strchr( s, ' ' );
        len = strlen( s );

        if ( s[0] == '[' && rbp && ( rbp - s == len - 1 ) )
        {
          if ( section )
            mas_free( section );
          section = NULL;
          if ( sectvalue )
            mas_free( sectvalue );
          sectvalue = NULL;
          if ( spp )
          {
            section = mas_strndup( s + 1, spp - s - 1 );
            sectvalue = mas_strndup( spp + 1, rbp - spp - 1 );
          }
          else if ( rbp )
            section = mas_strndup( s + 1, rbp - s - 1 );
          /* mMSG( "SECTION :'%s'", section ); */
          if ( new_section_func )
          {
            IEVAL( r, new_section_func( section ) );
            /* r=mas_opts_restore_new_section( section ); */
          }
          continue;
        }
        if ( section && at_section_func )
        {
          IEVAL( r, at_section_func( section, s ) );
        }
        else
        {
          r = mas_opts_restore_nosection( popts, section, sectvalue, s, opt_table, opt_table_size, arg );
          if ( r < 0 )
          {
            if ( unknown_opt_func )
            {
              r = unknown_opt_func( s );
            }
            else
            {
              IEVAL( r, -1 );
            }
          }
        }
      }
      if ( section )
        mas_free( section );
      if ( sectvalue )
        mas_free( sectvalue );
      mas_fclose( f );
    }
    else
    {
      IEVALM( r, -1, "(%d)opts file:'%s'", fpath );
    }
  }
  else
  {
    IEVAL( r, -1 );
  }
  return r;
}
