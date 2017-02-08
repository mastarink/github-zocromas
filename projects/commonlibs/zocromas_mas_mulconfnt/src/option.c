#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_argvc_tools.h>

#include "mulconfnt_structs.h"

#include "option.h"

config_option_t *
mulconfnt_config_option_create( void )
{
  config_option_t *opt = mas_malloc( sizeof( config_option_t ) );

  memset( opt, 0, sizeof( config_option_t ) );
  return opt;
}

void
mulconfnt_config_option_delete( config_option_t * opt )
{
  if ( opt->name )
    mas_free( opt->name );
  if ( opt->desc )
    mas_free( opt->desc );
  if ( opt->argdesc )
    mas_free( opt->argdesc );
  if ( opt->string_value )
    mas_free( opt->string_value );
  mas_free( opt );
}

config_option_t *
mulconfnt_config_option_clone( const config_option_t * topt )
{
  config_option_t *opt = mulconfnt_config_option_create(  );

  *opt = *topt;
  opt->name = mas_strdup( opt->name );
  opt->desc = mas_strdup( opt->desc );
  opt->argdesc = mas_strdup( opt->argdesc );
  opt->string_value = mas_strdup( opt->string_value );
  return opt;
}

void
mulconfnt_config_option_set_value( config_option_t * opt, const char *string_value )
{
  if ( opt->string_value )
    mas_free( opt->string_value );
  opt->string_value = NULL;
  if ( string_value )
    opt->string_value = mas_strdup( string_value );
  memset( &opt->nvalue, 0, sizeof( opt->nvalue ) );
  if ( opt->string_value )
  {
    char *ep = NULL;
    char *string = opt->string_value;
    long long v_long_long = 0;

    if ( opt->ptr )
    {
      fprintf( stderr, "PTR: %p %lx\n", opt->ptr, *( ( long * ) opt->ptr ) );
      switch ( opt->restype & ~MULCONF_BITWISE_ALL )
      {
      case MULCONF_RESTYPE_NONE:
        break;
      case MULCONF_RESTYPE_STRING:
        break;
      case MULCONF_RESTYPE_INT:
        opt->nvalue.v_int = *( ( int * ) opt->ptr );
        break;
      case MULCONF_RESTYPE_LONG:
        opt->nvalue.v_long = *( ( long * ) opt->ptr );
        break;
      case MULCONF_RESTYPE_LONG_LONG:
        opt->nvalue.v_long_long = *( ( long long * ) opt->ptr );
        break;
      }
    }

    switch ( opt->restype & ~MULCONF_BITWISE_ALL )
    {
    case MULCONF_RESTYPE_NONE:
    case MULCONF_RESTYPE_STRING:
      break;
    case MULCONF_RESTYPE_INT:
      v_long_long = ( long long ) opt->nvalue.v_int;
      break;
    case MULCONF_RESTYPE_LONG:
      v_long_long = ( long long ) opt->nvalue.v_long;
      break;
    case MULCONF_RESTYPE_LONG_LONG:
      v_long_long = opt->nvalue.v_long_long;
      break;
    }

    switch ( opt->restype & ~MULCONF_BITWISE_ALL )
    {
    case MULCONF_RESTYPE_NONE:
    case MULCONF_RESTYPE_STRING:
      break;
    case MULCONF_RESTYPE_INT:
    case MULCONF_RESTYPE_LONG:
    case MULCONF_RESTYPE_LONG_LONG:
      if ( string && string[0] == '0' )
      {
        if ( string[1] == 'x' )
          v_long_long = strtoll( string, &ep, 16 );
        else
          v_long_long = strtoll( string, &ep, 8 );
      }
      else
        v_long_long = strtoll( string, &ep, 10 );
      break;
    }

    if ( opt->restype & MULCONF_BITWISE_NOT )
      v_long_long = ~v_long_long;
    switch ( opt->restype & ~MULCONF_BITWISE_ALL )
    {
    case MULCONF_RESTYPE_NONE:
    case MULCONF_RESTYPE_STRING:
      break;
    case MULCONF_RESTYPE_INT:
      if ( opt->restype & MULCONF_BITWISE_AND )
        opt->nvalue.v_int &= ( int ) v_long_long;
      else if ( opt->restype & MULCONF_BITWISE_OR )
        opt->nvalue.v_int |= ( int ) v_long_long;
      else if ( opt->restype & MULCONF_BITWISE_XOR )
        opt->nvalue.v_int ^= ( int ) v_long_long;
      else
        opt->nvalue.v_int = ( int ) v_long_long;
      break;
    case MULCONF_RESTYPE_LONG:
      if ( opt->restype & MULCONF_BITWISE_AND )
      {
        fprintf( stderr, "=============================== %lx %lx\n", opt->nvalue.v_long, ( long ) v_long_long );
        opt->nvalue.v_long &= ( long ) v_long_long;
      }
      else if ( opt->restype & MULCONF_BITWISE_OR )
        opt->nvalue.v_long |= ( long ) v_long_long;
      else if ( opt->restype & MULCONF_BITWISE_XOR )
        opt->nvalue.v_long ^= ( long ) v_long_long;
      else
        opt->nvalue.v_long = ( long ) v_long_long;
      break;
    case MULCONF_RESTYPE_LONG_LONG:
      if ( opt->restype & MULCONF_BITWISE_AND )
        opt->nvalue.v_long_long &= v_long_long;
      else if ( opt->restype & MULCONF_BITWISE_OR )
        opt->nvalue.v_long_long |= v_long_long;
      else if ( opt->restype & MULCONF_BITWISE_XOR )
        opt->nvalue.v_long_long ^= v_long_long;
      else
        opt->nvalue.v_long_long = v_long_long;
      break;
    }

    if ( opt->ptr )
    {
      fprintf( stderr, "PTR: %p\n", opt->ptr );
      switch ( opt->restype & ~MULCONF_BITWISE_ALL )
      {
      case MULCONF_RESTYPE_NONE:
        break;
      case MULCONF_RESTYPE_STRING:
        break;
      case MULCONF_RESTYPE_INT:
        *( ( int * ) opt->ptr ) = opt->nvalue.v_int;
        break;
      case MULCONF_RESTYPE_LONG:
        *( ( long * ) opt->ptr ) = opt->nvalue.v_long;
        break;
      case MULCONF_RESTYPE_LONG_LONG:
        *( ( long long * ) opt->ptr ) = opt->nvalue.v_long_long;
        break;
      }
    }
  }
}
