#include "mulconfnt_defs.h"
#include <string.h>
#include <math.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_argvc_tools.h>

#include "mulconfnt_structs.h"

#include "mulconfnt_error.h"

#include "tools.h"
#include "source.h"

#include "option_base.h"
#include "option.h"

/*
 *
 * mucs_config_option_...
 *
 * */

static void
mucs_config_option_nvalue_from_ptr( mucs_option_han_t * opt )
{
  if ( opt->ptr )
  {
    if ( do_fprintf )
      fprintf( stderr, "PTR: %p %lx\n", opt->ptr, *( ( long * ) opt->ptr ) );
    switch ( opt->restype & ~MUCS_RTYP_FLAG_ALL )
    {
    case MUCS_RTYP_NONE:
    case MUCS_RTYP_STRING:
    case MUCS_RTYP_TARG:
      break;
    case MUCS_RTYP_CHAR:
      opt->nvalue.v_char = *( ( char * ) opt->ptr );
//        v_long_long = ( long long ) opt->nvalue.v_char;
      break;
    case MUCS_RTYP_UCHAR:
      opt->nvalue.v_uchar = *( ( unsigned char * ) opt->ptr );
//        v_ulong_long = ( unsigned long long ) opt->nvalue.v_uchar;
      break;
    case MUCS_RTYP_SHORT:
      opt->nvalue.v_short = *( ( short * ) opt->ptr );
//        v_long_long = ( long long ) opt->nvalue.v_short;
      break;
    case MUCS_RTYP_USHORT:
      opt->nvalue.v_ushort = *( ( unsigned short * ) opt->ptr );
//        v_ulong_long = ( unsigned long long ) opt->nvalue.v_ushort;
      break;
    case MUCS_RTYP_INT:
      opt->nvalue.v_int = *( ( int * ) opt->ptr );
//        v_long_long = ( long long ) opt->nvalue.v_int;
      break;
    case MUCS_RTYP_UINT:
      opt->nvalue.v_uint = *( ( unsigned int * ) opt->ptr );
//        v_ulong_long = ( unsigned long long ) opt->nvalue.v_uint;
      break;
    case MUCS_RTYP_LONG:
      opt->nvalue.v_long = *( ( long * ) opt->ptr );
//        v_long_long = ( long long ) opt->nvalue.v_long;
      break;
    case MUCS_RTYP_ULONG:
      opt->nvalue.v_ulong = *( ( unsigned long * ) opt->ptr );
//        v_ulong_long = ( unsigned long long ) opt->nvalue.v_ulong;
      break;
    case MUCS_RTYP_LONG_LONG:
      opt->nvalue.v_long_long = *( ( long long * ) opt->ptr );
//        v_long_long = opt->nvalue.v_long_long;
      break;
    case MUCS_RTYP_ULONG_LONG:
      opt->nvalue.v_ulong_long = *( ( unsigned long long * ) opt->ptr );
//        v_ulong_long = opt->nvalue.v_ulong_long;
      break;
    case MUCS_RTYP_DOUBLE:
      opt->nvalue.v_double = *( ( unsigned long long * ) opt->ptr );
//        v_double = opt->nvalue.v_double;
      break;
    case MUCS_RTYP_LDOUBLE:
      opt->nvalue.v_ldouble = *( ( unsigned long long * ) opt->ptr );
//        v_ldouble = opt->nvalue.v_ldouble;
      break;
    }
  }
}

static void
mucs_config_option_nvalue_to_ptr( mucs_option_han_t * opt )
{
  if ( opt->ptr )
  {
    if ( do_fprintf )
      fprintf( stderr, "PTR: %p\n", opt->ptr );
    switch ( opt->restype & ~MUCS_RTYP_FLAG_ALL )
    {
    case MUCS_RTYP_NONE:
      break;
    case MUCS_RTYP_STRING:
      if ( do_fprintf )
        fprintf( stderr, "STRING_VALUE: %s/%p => %p\n", opt->string_value, opt->string_value, ( ( char ** ) opt->ptr ) );
      *( ( char ** ) opt->ptr ) = mas_strdup( opt->string_value );
      break;
    case MUCS_RTYP_TARG:
      {
        mas_argvc_t *targ = ( mas_argvc_t * ) opt->ptr;

        mas_add_argvc_arg( targ, opt->string_value );
      }
      break;
    case MUCS_RTYP_CHAR:
      *( ( char * ) opt->ptr ) = opt->nvalue.v_char;
      break;
    case MUCS_RTYP_UCHAR:
      *( ( unsigned char * ) opt->ptr ) = opt->nvalue.v_char;
      break;
    case MUCS_RTYP_SHORT:
      *( ( short * ) opt->ptr ) = opt->nvalue.v_short;
      break;
    case MUCS_RTYP_USHORT:
      *( ( unsigned short * ) opt->ptr ) = opt->nvalue.v_short;
      break;
    case MUCS_RTYP_INT:
      *( ( int * ) opt->ptr ) = opt->nvalue.v_int;
      break;
    case MUCS_RTYP_UINT:
      *( ( unsigned int * ) opt->ptr ) = opt->nvalue.v_uint;
      break;
    case MUCS_RTYP_LONG:
      *( ( long * ) opt->ptr ) = opt->nvalue.v_long;
      break;
    case MUCS_RTYP_ULONG:
      *( ( unsigned long * ) opt->ptr ) = opt->nvalue.v_ulong;
      break;
    case MUCS_RTYP_LONG_LONG:
      *( ( long long * ) opt->ptr ) = opt->nvalue.v_long_long;
      break;
    case MUCS_RTYP_ULONG_LONG:
      *( ( unsigned long long * ) opt->ptr ) = opt->nvalue.v_ulong_long;
      break;
    case MUCS_RTYP_DOUBLE:
      *( ( double * ) opt->ptr ) = opt->nvalue.v_double;
      break;
    case MUCS_RTYP_LDOUBLE:
      *( ( long double * ) opt->ptr ) = opt->nvalue.v_ldouble;
      break;
    }
    opt->worked++;
  }
}

static nvalue_t
mucs_config_option_string_to_nvalue( mucs_option_han_t * opt, int *perr )
{
  nvalue_t v_x = { 0 };
  if ( opt )
  {
    char *string = opt->string_value;
    char *ep = NULL;

    v_x = opt->nvalue;

    switch ( opt->restype & ~MUCS_RTYP_FLAG_ALL )
    {
    case MUCS_RTYP_NONE:
    case MUCS_RTYP_STRING:
    case MUCS_RTYP_TARG:
      break;
    case MUCS_RTYP_CHAR:
    case MUCS_RTYP_SHORT:
    case MUCS_RTYP_INT:
    case MUCS_RTYP_LONG:
    case MUCS_RTYP_LONG_LONG:
      v_x.v_long_long = strtoll( string, &ep, 0 );
      *perr = ( ep != string + strlen( string ) );
      break;
    case MUCS_RTYP_UCHAR:
    case MUCS_RTYP_USHORT:
    case MUCS_RTYP_UINT:
    case MUCS_RTYP_ULONG:
    case MUCS_RTYP_ULONG_LONG:
      v_x.v_ulong_long = strtoull( string, &ep, 0 );
      *perr = ( ep != string + strlen( string ) );
      break;
    case MUCS_RTYP_DOUBLE:
      v_x.v_double = strtod( string, &ep );
      *perr = ( ep != string + strlen( string ) );
      break;
    case MUCS_RTYP_LDOUBLE:
      v_x.v_ldouble = strtold( string, &ep );
      if ( do_fprintf )
        fprintf( stderr, "LONG DOUBLE:\t%s\n\t\t%s\n\t\t%2.45Lf\n\t\t%2.45Lf\n",
                 "3.141592653589793238462643383279502884197169399375105820974944592", string, v_x.v_ldouble, atanl( 1 ) * 4.L );
      *perr = ( ep != string + strlen( string ) );
      break;
    }
  }
  return v_x;
}

static void
mucs_config_option_set_nvalue( mucs_option_han_t * opt, unsigned long flags )
{
  if ( opt && opt->string_value )
  {
    int err = 0;

/**   long long v_long_long _uUu_ = 0;
      unsigned long long v_ulong_long _uUu_ = 0;
      double v_double _uUu_ = 0.0;
      long double v_ldouble _uUu_ = 0.0; **/
    nvalue_t v_x = { 0 };

    mucs_config_option_nvalue_from_ptr( opt );
    v_x = mucs_config_option_string_to_nvalue( opt, &err );
    if ( err )
    {
      fprintf( stderr, ">>>>>> '%s'\n", opt->string_value );
      mucs_error_set_at_option( opt, __LINE__, __func__, __FILE__, flags, "Wrong value '%s'", opt->string_value ); /* non-numeric */
    }

    if ( opt->restype & MUCS_RTYP_BW_NOT )
      v_x.v_ulong_long = ~v_x.v_ulong_long;
    switch ( opt->restype & ~MUCS_RTYP_FLAG_ALL )
    {
    case MUCS_RTYP_NONE:
    case MUCS_RTYP_STRING:
    case MUCS_RTYP_TARG:
      break;
    case MUCS_RTYP_CHAR:
      if ( ( long long ) ( char ) v_x.v_long_long != v_x.v_long_long )
        mucs_error_set_at_option( opt, __LINE__, __func__, __FILE__, flags, "Wrong value '%s'", opt->string_value ); /*unable to place number into char */
      if ( opt->restype & MUCS_RTYP_BW_AND )
        opt->nvalue.v_char &= ( char ) v_x.v_long_long;
      else if ( opt->restype & MUCS_RTYP_BW_OR )
        opt->nvalue.v_char |= ( char ) v_x.v_long_long;
      else if ( opt->restype & MUCS_RTYP_BW_XOR )
        opt->nvalue.v_char ^= ( char ) v_x.v_long_long;
      else
        opt->nvalue.v_char = ( char ) v_x.v_long_long;
      break;
    case MUCS_RTYP_UCHAR:
      if ( ( unsigned long long ) ( unsigned char ) v_x.v_ulong_long != v_x.v_ulong_long )
        mucs_error_set_at_option( opt, __LINE__, __func__, __FILE__, flags, "Wrong value '%s'", opt->string_value ); /*unable to place number into char */
      if ( opt->restype & MUCS_RTYP_BW_AND )
        opt->nvalue.v_uchar &= ( char ) v_x.v_ulong_long;
      else if ( opt->restype & MUCS_RTYP_BW_OR )
        opt->nvalue.v_uchar |= ( char ) v_x.v_ulong_long;
      else if ( opt->restype & MUCS_RTYP_BW_XOR )
        opt->nvalue.v_uchar ^= ( char ) v_x.v_ulong_long;
      else
        opt->nvalue.v_uchar = ( char ) v_x.v_ulong_long;
      break;
    case MUCS_RTYP_SHORT:
      if ( ( long long ) ( short ) v_x.v_long_long != v_x.v_long_long )
        mucs_error_set_at_option( opt, __LINE__, __func__, __FILE__, flags, "Wrong value '%s'", opt->string_value ); /*unable to place number into short */
      if ( opt->restype & MUCS_RTYP_BW_AND )
        opt->nvalue.v_short &= ( short ) v_x.v_long_long;
      else if ( opt->restype & MUCS_RTYP_BW_OR )
        opt->nvalue.v_short |= ( short ) v_x.v_long_long;
      else if ( opt->restype & MUCS_RTYP_BW_XOR )
        opt->nvalue.v_short ^= ( short ) v_x.v_long_long;
      else
        opt->nvalue.v_short = ( short ) v_x.v_long_long;
      break;
    case MUCS_RTYP_USHORT:
      if ( ( unsigned long long ) ( unsigned short ) v_x.v_ulong_long != v_x.v_ulong_long )
        mucs_error_set_at_option( opt, __LINE__, __func__, __FILE__, flags, "Wrong value '%s'", opt->string_value ); /*unable to place number into short */
      if ( opt->restype & MUCS_RTYP_BW_AND )
        opt->nvalue.v_ushort &= ( short ) v_x.v_ulong_long;
      else if ( opt->restype & MUCS_RTYP_BW_OR )
        opt->nvalue.v_ushort |= ( short ) v_x.v_ulong_long;
      else if ( opt->restype & MUCS_RTYP_BW_XOR )
        opt->nvalue.v_ushort ^= ( short ) v_x.v_ulong_long;
      else
        opt->nvalue.v_ushort = ( short ) v_x.v_ulong_long;
      break;
    case MUCS_RTYP_INT:
      if ( ( long long ) ( int ) v_x.v_long_long != v_x.v_long_long )
        mucs_error_set_at_option( opt, __LINE__, __func__, __FILE__, flags, "Wrong value '%s'", opt->string_value ); /*unable to place number into int */
      if ( opt->restype & MUCS_RTYP_BW_AND )
        opt->nvalue.v_int &= ( int ) v_x.v_long_long;
      else if ( opt->restype & MUCS_RTYP_BW_OR )
        opt->nvalue.v_int |= ( int ) v_x.v_long_long;
      else if ( opt->restype & MUCS_RTYP_BW_XOR )
        opt->nvalue.v_int ^= ( int ) v_x.v_long_long;
      else
        opt->nvalue.v_int = ( int ) v_x.v_long_long;
      break;
    case MUCS_RTYP_UINT:
      if ( ( unsigned long long ) ( unsigned int ) v_x.v_ulong_long != v_x.v_ulong_long )
        mucs_error_set_at_option( opt, __LINE__, __func__, __FILE__, flags, "Wrong value '%s'", opt->string_value ); /*unable to place number into int */
      if ( opt->restype & MUCS_RTYP_BW_AND )
        opt->nvalue.v_uint &= ( int ) v_x.v_ulong_long;
      else if ( opt->restype & MUCS_RTYP_BW_OR )
        opt->nvalue.v_uint |= ( int ) v_x.v_ulong_long;
      else if ( opt->restype & MUCS_RTYP_BW_XOR )
        opt->nvalue.v_uint ^= ( int ) v_x.v_ulong_long;
      else
        opt->nvalue.v_uint = ( int ) v_x.v_ulong_long;
      break;
    case MUCS_RTYP_LONG:
      if ( ( long long ) ( long ) v_x.v_long_long != v_x.v_long_long )
      {
      /*unable to place number into long */
        mucs_error_set_at_option( opt, __LINE__, __func__, __FILE__, flags, "Wrong value '%s'", opt->string_value );
      }
      if ( opt->restype & MUCS_RTYP_BW_AND )
        opt->nvalue.v_long &= ( long ) v_x.v_long_long;
      else if ( opt->restype & MUCS_RTYP_BW_OR )
        opt->nvalue.v_long |= ( long ) v_x.v_long_long;
      else if ( opt->restype & MUCS_RTYP_BW_XOR )
        opt->nvalue.v_long ^= ( long ) v_x.v_long_long;
      else
        opt->nvalue.v_long = ( long ) v_x.v_long_long;
      break;
    case MUCS_RTYP_ULONG:
      if ( ( unsigned long long ) ( unsigned long ) v_x.v_ulong_long != v_x.v_ulong_long )
      {
      /*unable to place number into long */
        mucs_error_set_at_option( opt, __LINE__, __func__, __FILE__, flags, "Wrong value '%s'", opt->string_value );
      }
      if ( opt->restype & MUCS_RTYP_BW_AND )
        opt->nvalue.v_ulong &= ( long ) v_x.v_ulong_long;
      else if ( opt->restype & MUCS_RTYP_BW_OR )
        opt->nvalue.v_ulong |= ( long ) v_x.v_ulong_long;
      else if ( opt->restype & MUCS_RTYP_BW_XOR )
        opt->nvalue.v_ulong ^= ( long ) v_x.v_ulong_long;
      else
        opt->nvalue.v_ulong = ( long ) v_x.v_ulong_long;
      break;
    case MUCS_RTYP_LONG_LONG:
      if ( opt->restype & MUCS_RTYP_BW_AND )
        opt->nvalue.v_long_long &= v_x.v_long_long;
      else if ( opt->restype & MUCS_RTYP_BW_OR )
        opt->nvalue.v_long_long |= v_x.v_long_long;
      else if ( opt->restype & MUCS_RTYP_BW_XOR )
        opt->nvalue.v_long_long ^= v_x.v_long_long;
      else
        opt->nvalue.v_long_long = v_x.v_long_long;
      break;
    case MUCS_RTYP_ULONG_LONG:
      if ( opt->restype & MUCS_RTYP_BW_AND )
        opt->nvalue.v_ulong_long &= v_x.v_ulong_long;
      else if ( opt->restype & MUCS_RTYP_BW_OR )
        opt->nvalue.v_ulong_long |= v_x.v_ulong_long;
      else if ( opt->restype & MUCS_RTYP_BW_XOR )
        opt->nvalue.v_ulong_long ^= v_x.v_ulong_long;
      else
        opt->nvalue.v_ulong_long = v_x.v_ulong_long;
      break;
    case MUCS_RTYP_DOUBLE:
      opt->nvalue.v_double = v_x.v_double;
      break;
    case MUCS_RTYP_LDOUBLE:
      opt->nvalue.v_ldouble = v_x.v_ldouble;
      break;
    }

    mucs_config_option_nvalue_to_ptr( opt );
  }
}

void
mucs_config_option_set_value( mucs_option_han_t * opt, const char *string_value, unsigned long flags )
{
  if ( opt )
  {
    if ( opt->string_value )
      mas_free( opt->string_value );
    opt->string_value = NULL;
#if 0
    if ( string_value )
      opt->string_value = mas_strdup( string_value );
#else
    if ( string_value )
      opt->string_value = mucs_config_option_flag( opt, MUCS_FLAG_UNQUOTE ) ? mucs_unquote( string_value, "'\"" ) : mas_strdup( string_value );
#endif
    memset( &opt->nvalue, 0, sizeof( opt->nvalue ) );
    mucs_config_option_set_nvalue( opt, flags );
  }
}

mucs_option_han_t *
mucs_config_option_lookup_option_table( const mucs_option_han_t * option_table, mucs_variant_t variantid, const char *arg,
                                        const char *nextarg, const char *eq, const char *force_value, unsigned long flags )
{
  mucs_option_han_t *opt = NULL;
  const mucs_option_han_t *topt = option_table;

  while ( !opt && topt && topt->name )
  {
    int found = 0;
    int has_value = 0;
    const char *string_value = NULL;

    if ( do_fprintf > 10 )
      fprintf( stderr, "variantid: %d\n", variantid );
    if ( variantid == MUCS_VARIANT_SHORT )
    {
      if ( do_fprintf > 10 )
        fprintf( stderr, "variantid: SHORT\n" );
      found = ( strlen( arg ) == 1 && *arg == topt->shortname ) ? 1 : 0;
      if ( *arg == topt->shortname )
      {
        const char *argp = arg + 1;

        if ( *argp == 0 )
        {
          if ( do_fprintf )
            fprintf( stderr, "a FOUND %d: '%s' string_value='%s'\n", found, arg, string_value );
          string_value = nextarg;
          found = 1;
          has_value = 2;
        }
        else if ( strchr( " \t", *argp ) )
        {
          if ( do_fprintf )
            fprintf( stderr, "b FOUND %d: '%s' string_value='%s'\n", found, arg, string_value );
          found = 2;
          has_value = 1;
          string_value = argp;
          while ( strchr( " \t", *string_value ) )
            string_value++;
        }
        else
        {
          string_value = NULL;
        }
      }
    }
    else if ( variantid == MUCS_VARIANT_LONG )
    {
      unsigned l = strlen( topt->name );

      if ( do_fprintf > 10 )
        fprintf( stderr, "variantid: LONG %s ? %s -- %08x : %08x\n", arg, topt->name, ( topt->restype ), ( topt->restype & ~MUCS_RTYP_FLAG_ALL ) );

      if ( strlen( arg ) >= l && 0 == strncmp( arg, topt->name, l ) )
      {
      /* found opt name */
        if ( ( topt->restype & ~MUCS_RTYP_FLAG_ALL ) != MUCS_RTYP_NONE && arg[l] && ( eq && 0 == strncmp( arg + l, eq, strlen( eq ) ) ) )
        {
        /* A. --opt=val  */
          string_value = &arg[l] + strlen( eq );
          found = 2;
          has_value = 1;
          if ( do_fprintf )
            fprintf( stderr, "A FOUND %d: '%s' string_value='%s'\n", found, arg, string_value );
        }
        else if ( force_value )
        {
        /* B. --opt ( + forced value )  */
          string_value = force_value;
          found = 2;
          has_value = 1;
          if ( do_fprintf )
            fprintf( stderr, "B FOUND %d: '%s' string_value='%s'\n", found, arg, string_value );
        }
        else if ( arg[l] )
        {
        /* Not found */
          found = 0;
        }
        else if ( mucs_config_option_flag( topt, MUCS_FLAG_NEED_EQ ) )
        {
        /* Found, but no needed "=value" */
          found = 3;
        }
        else if ( ( topt->restype & ~MUCS_RTYP_FLAG_ALL ) == MUCS_RTYP_NONE )
        {
        /* Found, no-value */
          found = 1;
          has_value = 0;
          string_value = NULL;
          if ( do_fprintf )
            fprintf( stderr, "C FOUND %d: '%s' string_value='%s'\n", found, arg, string_value );
        }
        else
        {
        /* Found, value at next arg */
          found = 1;
          string_value = nextarg;
          has_value = 2;
          if ( do_fprintf )
            fprintf( stderr, "D FOUND %d: '%s' string_value='%s'\n", found, arg, string_value );
        }
      }
    }
    if ( found )
    {
      opt = mucs_config_option_clone( topt );
      if ( do_fprintf )
        fprintf( stderr, "NEW OPT %s; has_value=%d\n", opt->name, has_value );
      if ( found > 2 )
      {
        mucs_error_set_at_option( opt, __LINE__, __func__, __FILE__, flags, "No value given for '%s' option", opt->name );
      }
      if ( has_value )
      {
        if ( do_fprintf )
          fprintf( stderr, "SET VALUE %s='%s'; has_value=%d\n", opt->name, string_value, has_value );
        mucs_config_option_set_value( opt, string_value, flags );
        if ( opt->callback && !opt->ptr )
        {
          opt->callback( opt );
          opt->callback_called++;
        }
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

unsigned long
mucs_config_option_flags( const mucs_option_han_t * opt )
{
  unsigned long osrcflag = opt && opt->source ? mucs_source_flags( opt->source ) : 0L;

  return opt ? ( osrcflag | opt->flags ) : 0L;
}

int
mucs_config_option_flag( const mucs_option_han_t * opt, unsigned long mask )
{
  unsigned long osrcflag = opt->source ? mucs_source_flag( opt->source, mask ) : 0L;

  return opt ? ( osrcflag | ( opt->flags & mask ) ? 1 : 0 ) : 0;
}

void
mucs_option_set_source( mucs_option_han_t * opt, mucs_source_han_t * osrc )
{
  if ( opt && osrc )
  {
    opt->source = osrc;
    mucs_error_set_at_source_from_option( osrc, opt );
  }
}
