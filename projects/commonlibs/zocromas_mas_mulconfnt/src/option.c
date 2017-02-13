#include <string.h>
#include <math.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_argvc_tools.h>

#include "mulconfnt_defs.h"
#include "mulconfnt_structs.h"

#include "mulconfnt_error.h"

#include "source.h"

#include "option_base.h"
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
    unsigned long long v_ulong_long = 0;
    double v_double = 0.0;
    long double v_ldouble = 0.0;

    if ( opt->ptr )
    {
      if ( do_fprintf )
        fprintf( stderr, "PTR: %p %lx\n", opt->ptr, *( ( long * ) opt->ptr ) );
      switch ( opt->restype & ~MULCONF_BITWISE_ALL )
      {
      case MULCONF_RESTYPE_NONE:
      case MULCONF_RESTYPE_STRING:
        break;
      case MULCONF_RESTYPE_INT:
        opt->nvalue.v_int = *( ( int * ) opt->ptr );
        v_long_long = ( long long ) opt->nvalue.v_int;
        break;
      case MULCONF_RESTYPE_UINT:
        opt->nvalue.v_uint = *( ( unsigned int * ) opt->ptr );
        v_ulong_long = ( unsigned long long ) opt->nvalue.v_uint;
        break;
      case MULCONF_RESTYPE_LONG:
        opt->nvalue.v_long = *( ( long * ) opt->ptr );
        v_long_long = ( long long ) opt->nvalue.v_long;
        break;
      case MULCONF_RESTYPE_ULONG:
        opt->nvalue.v_ulong = *( ( unsigned long * ) opt->ptr );
        v_ulong_long = ( unsigned long long ) opt->nvalue.v_ulong;
        break;
      case MULCONF_RESTYPE_LONG_LONG:
        opt->nvalue.v_long_long = *( ( long long * ) opt->ptr );
        v_ulong_long = opt->nvalue.v_long_long;
        break;
      case MULCONF_RESTYPE_ULONG_LONG:
        opt->nvalue.v_ulong_long = *( ( unsigned long long * ) opt->ptr );
        v_ulong_long = opt->nvalue.v_ulong_long;
        break;
      case MULCONF_RESTYPE_DOUBLE:
        opt->nvalue.v_double = *( ( unsigned long long * ) opt->ptr );
        v_double = opt->nvalue.v_double;
        break;
      case MULCONF_RESTYPE_LDOUBLE:
        opt->nvalue.v_ldouble = *( ( unsigned long long * ) opt->ptr );
        v_ldouble = opt->nvalue.v_ldouble;
        break;
      }
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
        mulconfnt_error_set_at_option( opt, __LINE__, __func__, __FILE__, "Wrong value '%s'", string ); /* non-numeric */
      break;
    case MULCONF_RESTYPE_UINT:
    case MULCONF_RESTYPE_ULONG:
    case MULCONF_RESTYPE_ULONG_LONG:
      v_ulong_long = strtoull( string, &ep, 0 );
      if ( ep != string + strlen( string ) )
      {
        fprintf( stderr, ">>>>>> '%s'\n", string );
        mulconfnt_error_set_at_option( opt, __LINE__, __func__, __FILE__, "Wrong value '%s'", string ); /* non-numeric */
      }
      break;
    case MULCONF_RESTYPE_DOUBLE:
      v_double = strtod( string, &ep );
      if ( ep != string + strlen( string ) )
      {
        fprintf( stderr, ">>>>>> '%s'\n", string );
        mulconfnt_error_set_at_option( opt, __LINE__, __func__, __FILE__, "Wrong value '%s'", string ); /* non-numeric */
      }
      break;
    case MULCONF_RESTYPE_LDOUBLE:
      v_ldouble = strtold( string, &ep );
      if ( do_fprintf )
        fprintf( stderr, "LONG DOUBLE:\t%s\n\t\t%s\n\t\t%2.45Lf\n\t\t%2.45Lf\n", "3.141592653589793238462643383279502884197169399375105820974944592",
                 string, v_ldouble, atanl( 1 ) * 4.L );
      if ( ep != string + strlen( string ) )
      {
        fprintf( stderr, ">>>>>> '%s'\n", string );
        mulconfnt_error_set_at_option( opt, __LINE__, __func__, __FILE__, "Wrong value '%s'", string ); /* non-numeric */
      }
      break;
    }

    if ( opt->restype & MULCONF_BITWISE_NOT )
    {
      v_long_long = ~v_long_long;
      v_ulong_long = ~v_ulong_long;
    }
    switch ( opt->restype & ~MULCONF_BITWISE_ALL )
    {
    case MULCONF_RESTYPE_NONE:
    case MULCONF_RESTYPE_STRING:
      break;
    case MULCONF_RESTYPE_INT:
      if ( ( long long ) ( int ) v_long_long != v_long_long )
        mulconfnt_error_set_at_option( opt, __LINE__, __func__, __FILE__, "Wrong value '%s'", string ); /*unable to place number into int */
      if ( opt->restype & MULCONF_BITWISE_AND )
        opt->nvalue.v_int &= ( int ) v_long_long;
      else if ( opt->restype & MULCONF_BITWISE_OR )
        opt->nvalue.v_int |= ( int ) v_long_long;
      else if ( opt->restype & MULCONF_BITWISE_XOR )
        opt->nvalue.v_int ^= ( int ) v_long_long;
      else
        opt->nvalue.v_int = ( int ) v_long_long;
      break;
    case MULCONF_RESTYPE_UINT:
      if ( ( unsigned long long ) ( unsigned int ) v_ulong_long != v_ulong_long )
        mulconfnt_error_set_at_option( opt, __LINE__, __func__, __FILE__, "Wrong value '%s'", string ); /*unable to place number into int */
      if ( opt->restype & MULCONF_BITWISE_AND )
        opt->nvalue.v_uint &= ( int ) v_ulong_long;
      else if ( opt->restype & MULCONF_BITWISE_OR )
        opt->nvalue.v_uint |= ( int ) v_ulong_long;
      else if ( opt->restype & MULCONF_BITWISE_XOR )
        opt->nvalue.v_uint ^= ( int ) v_ulong_long;
      else
        opt->nvalue.v_uint = ( int ) v_ulong_long;
      break;
    case MULCONF_RESTYPE_LONG:
      if ( ( long long ) ( long ) v_long_long != v_long_long )
      {
      /*unable to place number into long */
        mulconfnt_error_set_at_option( opt, __LINE__, __func__, __FILE__, "Wrong value '%s'", string );
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
    case MULCONF_RESTYPE_ULONG:
      if ( ( unsigned long long ) ( unsigned long ) v_ulong_long != v_ulong_long )
      {
      /*unable to place number into long */
        mulconfnt_error_set_at_option( opt, __LINE__, __func__, __FILE__, "Wrong value '%s'", string );
      }
      if ( opt->restype & MULCONF_BITWISE_AND )
        opt->nvalue.v_ulong &= ( long ) v_ulong_long;
      else if ( opt->restype & MULCONF_BITWISE_OR )
        opt->nvalue.v_ulong |= ( long ) v_ulong_long;
      else if ( opt->restype & MULCONF_BITWISE_XOR )
        opt->nvalue.v_ulong ^= ( long ) v_ulong_long;
      else
        opt->nvalue.v_ulong = ( long ) v_ulong_long;
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
    case MULCONF_RESTYPE_ULONG_LONG:
      if ( opt->restype & MULCONF_BITWISE_AND )
        opt->nvalue.v_ulong_long &= v_ulong_long;
      else if ( opt->restype & MULCONF_BITWISE_OR )
        opt->nvalue.v_ulong_long |= v_ulong_long;
      else if ( opt->restype & MULCONF_BITWISE_XOR )
        opt->nvalue.v_ulong_long ^= v_ulong_long;
      else
        opt->nvalue.v_ulong_long = v_ulong_long;
      break;
    case MULCONF_RESTYPE_DOUBLE:
      opt->nvalue.v_double = v_double;
      break;
    case MULCONF_RESTYPE_LDOUBLE:
      opt->nvalue.v_ldouble = v_ldouble;
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
        if ( do_fprintf )
          fprintf( stderr, "STRING_VALUE: %s/%p => %p\n", opt->string_value, opt->string_value, ( ( char ** ) opt->ptr ) );
        *( ( char ** ) opt->ptr ) = mas_strdup( opt->string_value );
        break;
      case MULCONF_RESTYPE_INT:
        *( ( int * ) opt->ptr ) = opt->nvalue.v_int;
        break;
      case MULCONF_RESTYPE_UINT:
        *( ( unsigned int * ) opt->ptr ) = opt->nvalue.v_uint;
        break;
      case MULCONF_RESTYPE_LONG:
        *( ( long * ) opt->ptr ) = opt->nvalue.v_long;
        break;
      case MULCONF_RESTYPE_ULONG:
        *( ( unsigned long * ) opt->ptr ) = opt->nvalue.v_ulong;
        break;
      case MULCONF_RESTYPE_LONG_LONG:
        *( ( long long * ) opt->ptr ) = opt->nvalue.v_long_long;
        break;
      case MULCONF_RESTYPE_ULONG_LONG:
        *( ( unsigned long long * ) opt->ptr ) = opt->nvalue.v_ulong_long;
        break;
      case MULCONF_RESTYPE_DOUBLE:
        *( ( double * ) opt->ptr ) = opt->nvalue.v_double;
        break;
      case MULCONF_RESTYPE_LDOUBLE:
        *( ( long double * ) opt->ptr ) = opt->nvalue.v_ldouble;
        break;
      }
    }
  }
}

config_option_t *
mulconfnt_config_option_lookup_option_table( const config_option_t * option_table, config_variant_t variantid, const char *arg,
                                             const char *nextarg, const char *eq, const char *force_value )
{
  config_option_t *opt = NULL;
  const config_option_t *topt = option_table;

  while ( !opt && topt && topt->name )
  {
    int found = 0;
    int has_value = 0;
    const char *string_value = NULL;

    if ( do_fprintf > 10 )
      fprintf( stderr, "variantid: %d\n", variantid );
    if ( variantid == MULCONF_VARIANT_SHORT )
    {
      if ( do_fprintf > 10 )
        fprintf( stderr, "variantid: SHORT\n" );
      found = ( strlen( arg ) == 1 && *arg == topt->shortname ) ? 1 : 0;
      if ( found )
      {
        string_value = nextarg;
        has_value = 2;
      }
    }
    else if ( variantid == MULCONF_VARIANT_LONG )
    {
      unsigned l = strlen( topt->name );

      if ( do_fprintf > 10 )
        fprintf( stderr, "variantid: LONG %s ? %s -- %08x : %08x\n", arg, topt->name, ( topt->restype ), ( topt->restype & ~MULCONF_BITWISE_ALL ) );

      if ( strlen( arg ) >= l && 0 == strncmp( arg, topt->name, l ) )
      {
      /* found opt name */
        if ( ( topt->restype & ~MULCONF_BITWISE_ALL ) != MULCONF_RESTYPE_NONE && arg[l] && ( eq && 0 == strncmp( arg + l, eq, strlen( eq ) ) ) )
        {
          string_value = &arg[l] + strlen( eq );
          found = 2;
          has_value = 1;
          if ( do_fprintf )
            fprintf( stderr, "FOUND %d: string_value='%s'\n", found, string_value );
        }
        else if ( force_value )
        {
          string_value = force_value;
          found = 2;
          has_value = 1;
          if ( do_fprintf )
            fprintf( stderr, "FOUND %d: string_value='%s'\n", found, string_value );
        }
        else if ( arg[l] )
        {
          found = 0;
        }
        else if ( mulconfnt_config_option_flag( topt, MULCONF_OPTION_NEED_EQ ) )
        {
          found = 3;
        }
        else
        {
          found = 1;
          string_value = nextarg;
          has_value = 2;
          if ( do_fprintf )
            fprintf( stderr, "FOUND %d: string_value='%s'\n", found, string_value );
        }
      }
    }
    if ( found )
    {
      opt = mulconfnt_config_option_clone( topt );
      if ( do_fprintf )
        fprintf( stderr, "NEW OPT %s; has_value=%d\n", opt->name, has_value );
      if ( found > 2 )
      {
        mulconfnt_error_set_at_option( opt, __LINE__, __func__, __FILE__, "No value given for '%s' option", opt->name );
      }
      if ( has_value )
      {
        if ( do_fprintf )
          fprintf( stderr, "SET VALUE %s='%s'; has_value=%d\n", opt->name, string_value, has_value );
        mulconfnt_config_option_set_value( opt, string_value );
        opt->has_value = has_value;
      }
    }
    if ( opt )
      if ( do_fprintf )
        fprintf( stderr, "%s :: %s ? %c -- %s ='%s'\n", arg, option_table->name, option_table->shortname, found ? "OK" : "-", opt->string_value );
    option_table++;
    topt++;
  }
  return opt;
}

int
mulconfnt_config_option_flag( const config_option_t * opt, unsigned long mask )
{
  unsigned long osrcflag = opt->source ? mulconfnt_source_flag( opt->source, mask ) : 0L;

  return osrcflag | ( opt->flags & mask ) ? 1 : 0;
}

void
mulconfnt_option_set_source( config_option_t * opt, config_source_desc_t * osrc )
{
  if ( opt && osrc )
  {
    opt->source = osrc;
    mulconfnt_error_set_at_source_from_option( osrc, opt );
  }
}
