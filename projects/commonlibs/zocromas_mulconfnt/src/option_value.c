#define R_GOOD(_r) ((_r)>=0)
#include "mulconfnt_defs.h"
#include <string.h>
#include <math.h>

#include <mastar/wrap/mas_memory.h>
/* #include <mastar/tools/mas_argvc_tools.h> */

#include <mastar/minierr/minierr.h>

#include "mulconfnt_structs.h"

/* #include "global.h" */

#include "tools.h"
/* #include "source.h" */

/* #include "option_ref.h" */

/* #include "option_base.h" */
#include "option_flags.h"
/* #include "option.h" */

#include "option_value.h"

/*
 *
 * mucs_config_option_...
 *
 * */

 void
mucs_config_option_ptr_to_nvalue( mucs_option_t * opt )
{
  memset( &opt->d.nvalue, 0, sizeof( opt->d.nvalue ) );
  if ( opt->s.cust_ptr )
  {
    switch ( opt->s.restype & ~MUCS_RTYP_FLAG_ALL )
    {
    case MUCS_RTYP_NONE:
    case MUCS_RTYP_STRING:
    case MUCS_RTYP_TARG:
      break;
    case MUCS_RTYP_CHAR:
      opt->d.nvalue.v_char = *( ( char * ) opt->s.cust_ptr );
//        v_long_long = ( long long ) opt->d.nvalue.v_char;
      break;
    case MUCS_RTYP_UCHAR:
      opt->d.nvalue.v_uchar = *( ( unsigned char * ) opt->s.cust_ptr );
//        v_ulong_long = ( unsigned long long ) opt->d.nvalue.v_uchar;
      break;
    case MUCS_RTYP_SHORT:
      opt->d.nvalue.v_short = *( ( short * ) opt->s.cust_ptr );
//        v_long_long = ( long long ) opt->d.nvalue.v_short;
      break;
    case MUCS_RTYP_USHORT:
      opt->d.nvalue.v_ushort = *( ( unsigned short * ) opt->s.cust_ptr );
//        v_ulong_long = ( unsigned long long ) opt->d.nvalue.v_ushort;
      break;
    case MUCS_RTYP_INT:
      opt->d.nvalue.v_int = *( ( int * ) opt->s.cust_ptr );
//        v_long_long = ( long long ) opt->d.nvalue.v_int;
      break;
    case MUCS_RTYP_UINT:
      opt->d.nvalue.v_uint = *( ( unsigned int * ) opt->s.cust_ptr );
//        v_ulong_long = ( unsigned long long ) opt->d.nvalue.v_uint;
      break;
    case MUCS_RTYP_LONG:
      opt->d.nvalue.v_long = *( ( long * ) opt->s.cust_ptr );
//        v_long_long = ( long long ) opt->d.nvalue.v_long;
      break;
    case MUCS_RTYP_ULONG:
      opt->d.nvalue.v_ulong = *( ( unsigned long * ) opt->s.cust_ptr );
//        v_ulong_long = ( unsigned long long ) opt->d.nvalue.v_ulong;
      break;
    case MUCS_RTYP_LONG_LONG:
      opt->d.nvalue.v_long_long = *( ( long long * ) opt->s.cust_ptr );
//        v_long_long = opt->d.nvalue.v_long_long;
      break;
    case MUCS_RTYP_ULONG_LONG:
      opt->d.nvalue.v_ulong_long = *( ( unsigned long long * ) opt->s.cust_ptr );
//        v_ulong_long = opt->d.nvalue.v_ulong_long;
      break;
    case MUCS_RTYP_DOUBLE:
      opt->d.nvalue.v_double = *( ( unsigned long long * ) opt->s.cust_ptr );
//        v_double = opt->d.nvalue.v_double;
      break;
    case MUCS_RTYP_LDOUBLE:
      opt->d.nvalue.v_ldouble = *( ( unsigned long long * ) opt->s.cust_ptr );
//        v_ldouble = opt->d.nvalue.v_ldouble;
      break;
    }
  }
}

 void
mucs_config_option_nvalue_to_ptr( mucs_option_t * opt )
{
  if ( opt->s.cust_ptr )
  {
    switch ( opt->s.restype & ~MUCS_RTYP_FLAG_ALL )
    {
    case MUCS_RTYP_NONE:
      break;
    case MUCS_RTYP_STRING:
      {
        char **dst = ( ( char ** ) opt->s.cust_ptr );

        if ( *dst )
          mas_free( *dst );
        *dst = mas_strdup( opt->d.string_value );
      }
      break;
    case MUCS_RTYP_TARG:
      {
        mas_argvc_t *targ = ( mas_argvc_t * ) opt->s.cust_ptr;

        mas_add_argvc_arg( targ, opt->d.string_value );
      }
      break;
    case MUCS_RTYP_CHAR:
      *( ( char * ) opt->s.cust_ptr ) = opt->d.nvalue.v_char;
      break;
    case MUCS_RTYP_UCHAR:
      *( ( unsigned char * ) opt->s.cust_ptr ) = opt->d.nvalue.v_char;
      break;
    case MUCS_RTYP_SHORT:
      *( ( short * ) opt->s.cust_ptr ) = opt->d.nvalue.v_short;
      break;
    case MUCS_RTYP_USHORT:
      *( ( unsigned short * ) opt->s.cust_ptr ) = opt->d.nvalue.v_short;
      break;
    case MUCS_RTYP_INT:
      *( ( int * ) opt->s.cust_ptr ) = opt->d.nvalue.v_int;
      break;
    case MUCS_RTYP_UINT:
      *( ( unsigned int * ) opt->s.cust_ptr ) = opt->d.nvalue.v_uint;
      break;
    case MUCS_RTYP_LONG:
      *( ( long * ) opt->s.cust_ptr ) = opt->d.nvalue.v_long;
      break;
    case MUCS_RTYP_ULONG:
      *( ( unsigned long * ) opt->s.cust_ptr ) = opt->d.nvalue.v_ulong;
      break;
    case MUCS_RTYP_LONG_LONG:
      *( ( long long * ) opt->s.cust_ptr ) = opt->d.nvalue.v_long_long;
      break;
    case MUCS_RTYP_ULONG_LONG:
      *( ( unsigned long long * ) opt->s.cust_ptr ) = opt->d.nvalue.v_ulong_long;
      break;
    case MUCS_RTYP_DOUBLE:
      *( ( double * ) opt->s.cust_ptr ) = opt->d.nvalue.v_double;
      break;
    case MUCS_RTYP_LDOUBLE:
      *( ( long double * ) opt->s.cust_ptr ) = opt->d.nvalue.v_ldouble;
      break;
    }
    opt->d.value_is_set++;
  }
}

static nvalue_t
mucs_config_option_string_to_nvalue( mucs_option_t * opt )
{
  nvalue_t v_x = { 0 };
  if ( opt )
  {
    char *string = opt->d.string_value;
    char *ep = NULL;

    errno = 0;
    v_x = opt->d.nvalue;

    switch ( opt->s.restype & ~MUCS_RTYP_FLAG_ALL )
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
      break;
    case MUCS_RTYP_UCHAR:
    case MUCS_RTYP_USHORT:
    case MUCS_RTYP_UINT:
    case MUCS_RTYP_ULONG:
    case MUCS_RTYP_ULONG_LONG:
      v_x.v_ulong_long = strtoull( string, &ep, 0 );
      break;
    case MUCS_RTYP_DOUBLE:
      v_x.v_double = strtod( string, &ep );
      break;
    case MUCS_RTYP_LDOUBLE:
      v_x.v_ldouble = strtold( string, &ep );
      break;
    }
  }
  return v_x;
}

static int
mucs_config_option_combine_value( mucs_option_t * opt, nvalue_t v_x, mucs_errors_t * perrors )
{
  rDECLGOOD;
  if ( opt->s.restype & MUCS_RTYP_BW_NOT )
    v_x.v_ulong_long = ~v_x.v_ulong_long;
  switch ( opt->s.restype & ~MUCS_RTYP_FLAG_ALL )
  {
  case MUCS_RTYP_NONE:
  case MUCS_RTYP_STRING:
  case MUCS_RTYP_TARG:
    break;
  case MUCS_RTYP_CHAR:
    if ( ( long long ) ( char ) v_x.v_long_long != v_x.v_long_long )
    {
      rSETBAD;
      *perrors |= MUCS_ERROR_WRONG_VALUE;
      QRGOPTM( opt, rCODE, "wrong value '%s'", opt->d.string_value );
      v_x.v_long_long = 0;
    }
    if ( opt->s.restype & MUCS_RTYP_BW_AND )
      opt->d.nvalue.v_char &= ( char ) ( v_x.v_long_long & 0xff );
    else if ( opt->s.restype & MUCS_RTYP_BW_OR )                     // set bits
      opt->d.nvalue.v_char |= ( char ) ( v_x.v_long_long & 0xff );
    else if ( opt->s.restype & MUCS_RTYP_BW_NOR )                    // reset bits
      opt->d.nvalue.v_char |= ( char ) ( ~v_x.v_long_long & 0xff );
    else if ( opt->s.restype & MUCS_RTYP_BW_XOR )                    // invert bits
      opt->d.nvalue.v_char ^= ( char ) ( v_x.v_long_long & 0xff );
    else
      opt->d.nvalue.v_char = ( char ) ( v_x.v_long_long & 0xff );
    break;
  case MUCS_RTYP_UCHAR:
    if ( ( unsigned long long ) ( unsigned char ) v_x.v_ulong_long != v_x.v_ulong_long )
    {
      rSETBAD;
      *perrors |= MUCS_ERROR_WRONG_VALUE;
      QRGOPTM( opt, rCODE, "wrong value '%s'", opt->d.string_value );
      v_x.v_ulong_long = 0;
    }
    if ( opt->s.restype & MUCS_RTYP_BW_AND )
      opt->d.nvalue.v_uchar &= ( char ) ( v_x.v_ulong_long & 0xff );
    else if ( opt->s.restype & MUCS_RTYP_BW_OR )                     // set bits
      opt->d.nvalue.v_uchar |= ( char ) ( v_x.v_ulong_long & 0xff );
    else if ( opt->s.restype & MUCS_RTYP_BW_NOR )                    // reset bits
      opt->d.nvalue.v_uchar &= ( char ) ( ( ~v_x.v_ulong_long ) & 0xff );
    else if ( opt->s.restype & MUCS_RTYP_BW_XOR )                    // invert bits
      opt->d.nvalue.v_uchar ^= ( char ) ( v_x.v_ulong_long & 0xff );
    else
      opt->d.nvalue.v_uchar = ( char ) ( v_x.v_ulong_long & 0xff );
    break;
  case MUCS_RTYP_SHORT:
    if ( ( long long ) ( short ) v_x.v_long_long != v_x.v_long_long )
    {
      rSETBAD;
      *perrors |= MUCS_ERROR_WRONG_VALUE;
      QRGOPTM( opt, rCODE, "wrong value '%s'", opt->d.string_value );
      v_x.v_long_long = 0;
    }
    if ( opt->s.restype & MUCS_RTYP_BW_AND )
      opt->d.nvalue.v_short &= ( short ) v_x.v_long_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_OR )                     // set bits
      opt->d.nvalue.v_short |= ( short ) v_x.v_long_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_NOR )                    // reset bits
      opt->d.nvalue.v_short &= ( short ) ( ~v_x.v_long_long );
    else if ( opt->s.restype & MUCS_RTYP_BW_XOR )
      opt->d.nvalue.v_short ^= ( short ) v_x.v_long_long;
    else
      opt->d.nvalue.v_short = ( short ) v_x.v_long_long;
    break;
  case MUCS_RTYP_USHORT:
    if ( ( unsigned long long ) ( unsigned short ) v_x.v_ulong_long != v_x.v_ulong_long )
    {
      rSETBAD;
      *perrors |= MUCS_ERROR_WRONG_VALUE;
      QRGOPTM( opt, rCODE, "wrong value '%s'", opt->d.string_value );
      v_x.v_ulong_long = 0;
    }
    if ( opt->s.restype & MUCS_RTYP_BW_AND )
      opt->d.nvalue.v_ushort &= ( short ) v_x.v_ulong_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_OR )                     // set bits
      opt->d.nvalue.v_ushort |= ( short ) v_x.v_ulong_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_NOR )                    // reset bits
      opt->d.nvalue.v_ushort &= ( short ) ( ~v_x.v_ulong_long );
    else if ( opt->s.restype & MUCS_RTYP_BW_XOR )
      opt->d.nvalue.v_ushort ^= ( short ) v_x.v_ulong_long;
    else
      opt->d.nvalue.v_ushort = ( short ) v_x.v_ulong_long;
    break;
  case MUCS_RTYP_INT:
    if ( ( long long ) ( int ) v_x.v_long_long != v_x.v_long_long )
    {
      rSETBAD;
      *perrors |= MUCS_ERROR_WRONG_VALUE;
      QRGOPTM( opt, rCODE, "wrong value '%s'", opt->d.string_value );
      v_x.v_long_long = 0;
    }
    if ( opt->s.restype & MUCS_RTYP_BW_AND )
      opt->d.nvalue.v_int &= ( int ) v_x.v_long_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_OR )                     // set bits
      opt->d.nvalue.v_int |= ( int ) v_x.v_long_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_NOR )                    // reset bits
      opt->d.nvalue.v_int &= ( int ) ( ~v_x.v_long_long );
    else if ( opt->s.restype & MUCS_RTYP_BW_XOR )
      opt->d.nvalue.v_int ^= ( int ) v_x.v_long_long;
    else
      opt->d.nvalue.v_int = ( int ) v_x.v_long_long;
    break;
  case MUCS_RTYP_UINT:
    if ( ( unsigned long long ) ( unsigned int ) v_x.v_ulong_long != v_x.v_ulong_long )
    {
      rSETBAD;
      *perrors |= MUCS_ERROR_WRONG_VALUE;
      QRGOPTM( opt, rCODE, "wrong value '%s'", opt->d.string_value );
      v_x.v_ulong_long = 0;
    }
    if ( opt->s.restype & MUCS_RTYP_BW_AND )
      opt->d.nvalue.v_uint &= ( unsigned int ) v_x.v_ulong_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_OR )                     // set bits
      opt->d.nvalue.v_uint |= ( unsigned int ) v_x.v_ulong_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_NOR )                    // reset bits
      opt->d.nvalue.v_uint &= ( unsigned int ) ( ~v_x.v_long_long );
    else if ( opt->s.restype & MUCS_RTYP_BW_XOR )
      opt->d.nvalue.v_uint ^= ( unsigned int ) v_x.v_ulong_long;
    else
      opt->d.nvalue.v_uint = ( unsigned int ) v_x.v_ulong_long;
    break;
  case MUCS_RTYP_LONG:
    if ( ( long long ) ( long ) v_x.v_long_long != v_x.v_long_long )
    {
    /*unable to place number into long */
      rSETBAD;
      *perrors |= MUCS_ERROR_WRONG_VALUE;
      QRGOPTM( opt, rCODE, "wrong value '%s'", opt->d.string_value );
      v_x.v_long_long = 0;
    }
    if ( opt->s.restype & MUCS_RTYP_BW_AND )
      opt->d.nvalue.v_long &= ( long ) v_x.v_long_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_OR )                     // set bits
      opt->d.nvalue.v_long |= ( long ) v_x.v_long_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_NOR )                    // reset bits
      opt->d.nvalue.v_long &= ( long ) ( ~v_x.v_long_long );
    else if ( opt->s.restype & MUCS_RTYP_BW_XOR )
      opt->d.nvalue.v_long ^= ( long ) v_x.v_long_long;
    else
      opt->d.nvalue.v_long = ( long ) v_x.v_long_long;
    break;
  case MUCS_RTYP_ULONG:
    if ( ( unsigned long long ) ( unsigned long ) v_x.v_ulong_long != v_x.v_ulong_long )
    {
    /*unable to place number into long */
      rSETBAD;
      *perrors |= MUCS_ERROR_WRONG_VALUE;
      QRGOPTM( opt, rCODE, "wrong value '%s'", opt->d.string_value );
      v_x.v_ulong_long = 0;
    }
    if ( opt->s.restype & MUCS_RTYP_BW_AND )
      opt->d.nvalue.v_ulong &= ( unsigned long ) v_x.v_ulong_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_OR )                     // set bits
      opt->d.nvalue.v_ulong |= ( unsigned long ) v_x.v_ulong_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_NOR )                    // reset bits
      opt->d.nvalue.v_ulong &= ( unsigned long ) ( ~v_x.v_long_long );
    else if ( opt->s.restype & MUCS_RTYP_BW_XOR )
      opt->d.nvalue.v_ulong ^= ( unsigned long ) v_x.v_ulong_long;
    else
      opt->d.nvalue.v_ulong = ( unsigned long ) v_x.v_ulong_long;
    break;
  case MUCS_RTYP_LONG_LONG:
    if ( opt->s.restype & MUCS_RTYP_BW_AND )
      opt->d.nvalue.v_long_long &= v_x.v_long_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_OR )                     // set bits
      opt->d.nvalue.v_long_long |= v_x.v_long_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_NOR )                    // reset bits
      opt->d.nvalue.v_long_long &= ~v_x.v_long_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_XOR )
      opt->d.nvalue.v_long_long ^= v_x.v_long_long;
    else
      opt->d.nvalue.v_long_long = v_x.v_long_long;
    break;
  case MUCS_RTYP_ULONG_LONG:
    if ( opt->s.restype & MUCS_RTYP_BW_AND )
      opt->d.nvalue.v_ulong_long &= ( unsigned long long ) v_x.v_ulong_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_OR )                     // set bits
      opt->d.nvalue.v_ulong_long |= ( unsigned long long ) v_x.v_ulong_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_NOR )                    // reset bits
      opt->d.nvalue.v_ulong_long &= ( unsigned long long ) ~v_x.v_long_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_XOR )
      opt->d.nvalue.v_ulong_long ^= ( unsigned long long ) v_x.v_ulong_long;
    else
      opt->d.nvalue.v_ulong_long = ( unsigned long long ) v_x.v_ulong_long;
    break;
  case MUCS_RTYP_DOUBLE:
    opt->d.nvalue.v_double = v_x.v_double;
    break;
  case MUCS_RTYP_LDOUBLE:
    opt->d.nvalue.v_ldouble = v_x.v_ldouble;
    break;
  }

  rRET;
}

static int
mucs_config_option_set_nvalue( mucs_option_t * opt, mucs_errors_t * perrors )
{
  rDECLGOOD;
  if ( opt )
  {
    nvalue_t v_x = { 0 };
  /* take old value from "user area" to the opt->d.nvalue (opt->d.string_value ?) to be combined with v_x */

    if ( mucs_config_option_flag( opt, MUCS_FLAG_USE_DEF_NVALUE ) )
    {
      v_x = opt->s.def_nvalue;
    }
    else if ( opt->d.string_value )
    {
      errno = 0;
      v_x = mucs_config_option_string_to_nvalue( opt );
      if ( errno /* || err */  )
      {
        rSETBAD;
        *perrors |= MUCS_ERROR_WRONG_VALUE;
        QRGOPTM( opt, rCODE, "wrong value '%s'", opt->d.string_value );
      }
    }
    if ( rGOOD )
    {
      mucs_config_option_combine_value( opt, v_x, perrors );
    }
  }
  rRET;
}

 int
mucs_config_option_set_value( mucs_option_t * opt, const char *string_value, mucs_errors_t * perrors )
{
  rDECLBAD;

  if ( opt )
  {
  /* const char *string_value = optscan->string_value; */

    if ( opt->d.string_value )
      mas_free( opt->d.string_value );
    opt->d.string_value = NULL;
#if 0
    if ( string_value )
      opt->d.string_value = mas_strdup( string_value );
#else
    if ( string_value )
      opt->d.string_value = mucs_config_option_flag( opt, MUCS_FLAG_UNQUOTE ) ? mucs_unquote( string_value, "'\"" ) : mas_strdup( string_value );
#endif
    rC( mucs_config_option_set_nvalue( opt, perrors ) );
  }
  rRET;
}
