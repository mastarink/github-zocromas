#include <string.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_argvc_tools.h>

#include "mulconfnt_defs.h"
#include "mulconfnt_structs.h"

#include "mulconfnt_error.h"
#include "option.h"

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
      if ( do_fprintf )
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
      v_long_long = strtoll( string, &ep, 0 );
      if ( ep != string + strlen( string ) )
        mulconfnt_set_error( __LINE__, __func__ );                   /* non-numeric */
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
      if ( ( long long ) ( int ) v_long_long != v_long_long )
        mulconfnt_set_error( __LINE__, __func__ );                   /*unable to place number into int */
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
      if ( ( long long ) ( long ) v_long_long != v_long_long )
      {
      /*unable to place number into long */
        mulconfnt_set_error( __LINE__, __func__ );
      }
      if ( opt->restype & MULCONF_BITWISE_AND )
        opt->nvalue.v_long &= ( long ) v_long_long;
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
      if ( do_fprintf )
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
