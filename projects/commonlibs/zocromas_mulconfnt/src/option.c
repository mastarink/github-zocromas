#define R_GOOD(_r) ((_r)>=0)
#include "mulconfnt_defs.h"
#include <string.h>
#include <math.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_argvc_tools.h>

#include <mastar/minierr/minierr.h>

#include "mulconfnt_structs.h"

#include "tools.h"
#include "source.h"

#include "option_base.h"
#include "option.h"

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
      *( ( char ** ) opt->s.cust_ptr ) = mas_strdup( opt->d.string_value );
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

static int _uUu_
mucs_config_option_combine_value( mucs_option_t * opt, nvalue_t v_x, mucs_optscanner_t * optscan )
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
      optscan->errors |= MUCS_ERROR_WRONG_VALUE;
      QRGOPTM( opt, rCODE, "wrong value '%s'", opt->d.string_value );
      v_x.v_long_long = 0;
    }
    if ( opt->s.restype & MUCS_RTYP_BW_AND )
      opt->d.nvalue.v_char &= ( char ) v_x.v_long_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_OR )
      opt->d.nvalue.v_char |= ( char ) v_x.v_long_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_XOR )
      opt->d.nvalue.v_char ^= ( char ) v_x.v_long_long;
    else
      opt->d.nvalue.v_char = ( char ) v_x.v_long_long;
    break;
  case MUCS_RTYP_UCHAR:
    if ( ( unsigned long long ) ( unsigned char ) v_x.v_ulong_long != v_x.v_ulong_long )
    {
      rSETBAD;
      optscan->errors |= MUCS_ERROR_WRONG_VALUE;
      QRGOPTM( opt, rCODE, "wrong value '%s'", opt->d.string_value );
      v_x.v_ulong_long = 0;
    }
    if ( opt->s.restype & MUCS_RTYP_BW_AND )
      opt->d.nvalue.v_uchar &= ( char ) v_x.v_ulong_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_OR )
      opt->d.nvalue.v_uchar |= ( char ) v_x.v_ulong_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_XOR )
      opt->d.nvalue.v_uchar ^= ( char ) v_x.v_ulong_long;
    else
      opt->d.nvalue.v_uchar = ( char ) v_x.v_ulong_long;
    break;
  case MUCS_RTYP_SHORT:
    if ( ( long long ) ( short ) v_x.v_long_long != v_x.v_long_long )
    {
      rSETBAD;
      optscan->errors |= MUCS_ERROR_WRONG_VALUE;
      QRGOPTM( opt, rCODE, "wrong value '%s'", opt->d.string_value );
      v_x.v_long_long = 0;
    }
    if ( opt->s.restype & MUCS_RTYP_BW_AND )
      opt->d.nvalue.v_short &= ( short ) v_x.v_long_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_OR )
      opt->d.nvalue.v_short |= ( short ) v_x.v_long_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_XOR )
      opt->d.nvalue.v_short ^= ( short ) v_x.v_long_long;
    else
      opt->d.nvalue.v_short = ( short ) v_x.v_long_long;
    break;
  case MUCS_RTYP_USHORT:
    if ( ( unsigned long long ) ( unsigned short ) v_x.v_ulong_long != v_x.v_ulong_long )
    {
      rSETBAD;
      optscan->errors |= MUCS_ERROR_WRONG_VALUE;
      QRGOPTM( opt, rCODE, "wrong value '%s'", opt->d.string_value );
      v_x.v_ulong_long = 0;
    }
    if ( opt->s.restype & MUCS_RTYP_BW_AND )
      opt->d.nvalue.v_ushort &= ( short ) v_x.v_ulong_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_OR )
      opt->d.nvalue.v_ushort |= ( short ) v_x.v_ulong_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_XOR )
      opt->d.nvalue.v_ushort ^= ( short ) v_x.v_ulong_long;
    else
      opt->d.nvalue.v_ushort = ( short ) v_x.v_ulong_long;
    break;
  case MUCS_RTYP_INT:
    if ( ( long long ) ( int ) v_x.v_long_long != v_x.v_long_long )
    {
      rSETBAD;
      optscan->errors |= MUCS_ERROR_WRONG_VALUE;
      QRGOPTM( opt, rCODE, "wrong value '%s'", opt->d.string_value );
      v_x.v_long_long = 0;
    }
    if ( opt->s.restype & MUCS_RTYP_BW_AND )
      opt->d.nvalue.v_int &= ( int ) v_x.v_long_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_OR )
      opt->d.nvalue.v_int |= ( int ) v_x.v_long_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_XOR )
      opt->d.nvalue.v_int ^= ( int ) v_x.v_long_long;
    else
      opt->d.nvalue.v_int = ( int ) v_x.v_long_long;
    break;
  case MUCS_RTYP_UINT:
    if ( ( unsigned long long ) ( unsigned int ) v_x.v_ulong_long != v_x.v_ulong_long )
    {
      rSETBAD;
      optscan->errors |= MUCS_ERROR_WRONG_VALUE;
      QRGOPTM( opt, rCODE, "wrong value '%s'", opt->d.string_value );
      v_x.v_ulong_long = 0;
    }
    if ( opt->s.restype & MUCS_RTYP_BW_AND )
      opt->d.nvalue.v_uint &= ( int ) v_x.v_ulong_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_OR )
      opt->d.nvalue.v_uint |= ( int ) v_x.v_ulong_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_XOR )
      opt->d.nvalue.v_uint ^= ( int ) v_x.v_ulong_long;
    else
      opt->d.nvalue.v_uint = ( int ) v_x.v_ulong_long;
    break;
  case MUCS_RTYP_LONG:
    if ( ( long long ) ( long ) v_x.v_long_long != v_x.v_long_long )
    {
    /*unable to place number into long */
      rSETBAD;
      optscan->errors |= MUCS_ERROR_WRONG_VALUE;
      QRGOPTM( opt, rCODE, "wrong value '%s'", opt->d.string_value );
      v_x.v_long_long = 0;
    }
    if ( opt->s.restype & MUCS_RTYP_BW_AND )
      opt->d.nvalue.v_long &= ( long ) v_x.v_long_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_OR )
      opt->d.nvalue.v_long |= ( long ) v_x.v_long_long;
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
      optscan->errors |= MUCS_ERROR_WRONG_VALUE;
      QRGOPTM( opt, rCODE, "wrong value '%s'", opt->d.string_value );
      v_x.v_ulong_long = 0;
    }
    if ( opt->s.restype & MUCS_RTYP_BW_AND )
      opt->d.nvalue.v_ulong &= ( unsigned long ) v_x.v_ulong_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_OR )
      opt->d.nvalue.v_ulong |= ( unsigned long ) v_x.v_ulong_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_XOR )
      opt->d.nvalue.v_ulong ^= ( unsigned long ) v_x.v_ulong_long;
    else
      opt->d.nvalue.v_ulong = ( unsigned long ) v_x.v_ulong_long;
    break;
  case MUCS_RTYP_LONG_LONG:
    if ( opt->s.restype & MUCS_RTYP_BW_AND )
      opt->d.nvalue.v_long_long &= v_x.v_long_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_OR )
      opt->d.nvalue.v_long_long |= v_x.v_long_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_XOR )
      opt->d.nvalue.v_long_long ^= v_x.v_long_long;
    else
      opt->d.nvalue.v_long_long = v_x.v_long_long;
    break;
  case MUCS_RTYP_ULONG_LONG:
    if ( opt->s.restype & MUCS_RTYP_BW_AND )
      opt->d.nvalue.v_ulong_long &= v_x.v_ulong_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_OR )
      opt->d.nvalue.v_ulong_long |= v_x.v_ulong_long;
    else if ( opt->s.restype & MUCS_RTYP_BW_XOR )
      opt->d.nvalue.v_ulong_long ^= v_x.v_ulong_long;
    else
      opt->d.nvalue.v_ulong_long = v_x.v_ulong_long;
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
mucs_config_option_set_nvalue( mucs_option_t * opt, mucs_optscanner_t * optscan )
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
        optscan->errors |= MUCS_ERROR_WRONG_VALUE;
        QRGOPTM( opt, rCODE, "wrong value '%s'", opt->d.string_value );
      }
    }
    if ( rGOOD )
    {
      mucs_config_option_combine_value( opt, v_x, optscan );
    }
  }
  rRET;
}

int
mucs_config_option_set_value( mucs_option_t * opt, mucs_optscanner_t * optscan /* const char *string_value */  )
{
  rDECLBAD;

  if ( opt )
  {
    const char *string_value = optscan->string_value;

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
    rC( mucs_config_option_set_nvalue( opt, optscan ) );
  }
  rRET;
}

/*
 * if match
 *   returns end-pointer of name at arg_nopref
 * else
 *   returns NULL
 * */
const char *
mucs_config_option_match_name( const mucs_option_t * topt, mucs_optscanner_t * optscan, const char *arg_nopref, const char *eq )
{
  rDECLGOOD;
  const char *argp = NULL;

  switch ( optscan->variantid )
  {
  case MUCS_VARIANT_SHORT:
    {
      unsigned lname = 0;

      lname = 1;
      argp = arg_nopref + lname;
      if ( *arg_nopref != topt->s.shortn )
        argp = NULL;
    }
    break;
  case MUCS_VARIANT_LONG:
    {
      unsigned lname = 0;

      lname = strlen( topt->s.name );
      if ( 0 == strncmp( topt->s.name, arg_nopref, lname ) )
      {
        argp = arg_nopref + lname;
        if ( *argp )
        {
          unsigned leq = strlen( eq );
          int with_eq = 0;

          with_eq = ( eq && 0 == strncmp( argp, eq, leq ) );
          if ( with_eq )
            argp += leq;
          else if ( mucs_config_option_flag( topt, MUCS_FLAG_NEED_EQ ) || !strchr( " \t", *argp ) )
            argp = NULL;
        }
        else if ( mucs_config_option_flag( topt, MUCS_FLAG_NEED_EQ ) )
          argp = NULL;
      }
    }
    break;
  case MUCS_VARIANT_NONOPT:
    if ( 0 == strcmp( topt->s.name, MUCS_NONOPT_NAME ) )
      argp = arg_nopref;
    break;
  case MUCS_VARIANT_BAD:
  case MUCS_VARIANT_IGNORE:
  case MUCS_VARIANTS:
    rSETBAD;
    optscan->errors |= MUCS_ERROR_UNKNOWN;
    QRGOPTM( topt, rCODE, "unknown error \"%s\" at \"%s\"", optscan->at_arg, optscan->arg );
    break;
  }
  return argp;
}

static int
mucs_config_option_validate( const char *ep, mucs_optscanner_t * optscan )
{
  rDECLGOOD;
  const mucs_option_t *topt = optscan->found_topt;

  if ( optscan->force_value )                                        /* use force_value for alias inheritance (only!?) */
  {
  /* B. "--opt" ( + forced value )  */
    optscan->has_value = 1;
    optscan->string_value = optscan->force_value;
  }
  else
  {
    int none_value = ( ( ( topt->s.restype & ~MUCS_RTYP_FLAG_ALL ) == MUCS_RTYP_NONE ) );
    int no_value = mucs_config_option_flag( topt, MUCS_FLAG_NO_VALUE );
    int opt_val = mucs_config_option_flag( topt, MUCS_FLAG_OPTIONAL_VALUE );
    int use_def_string = mucs_config_option_flag( topt, MUCS_FLAG_USE_DEF_SVALUE );
    int use_def_nvalue = mucs_config_option_flag( topt, MUCS_FLAG_USE_DEF_NVALUE );
    int word _uUu_ = strchr( " \t", *ep ) ? 1 : 0;

    if ( *ep )
    {
      if ( none_value )
      {
        rSETBAD;
      /* TODO bad: value */
        optscan->errors |= MUCS_ERROR_UNEXPECTED_VALUE;
        QRGOPTM( topt, rCODE, "unexpected value given for \"-%c\" option (short)", topt->s.shortn );
        optscan->has_value = 1;
        optscan->string_value = ep;
        while ( *optscan->string_value && strchr( " \t", *optscan->string_value ) )
          optscan->string_value++;
      }
      else
      {
        if ( no_value )
        {
          if ( use_def_string )
          {
          /* ok, no value, value is optional, but use default value */
            optscan->has_value = 1;
            optscan->string_value = topt->s.def_string_value;
          }
          else if ( use_def_nvalue )
          {
            optscan->has_value = 1;                                  /* ??? */
            if ( optscan->variantid == MUCS_VARIANT_SHORT || ( optscan->variantid == MUCS_VARIANT_LONG && word ) )
            {
              optscan->at_arg = ep;
            /* WARN( "** New arg: %s", optscan->at_arg ); */
            }
          }
        }
        else
        {
        /* TODO good: possible value, here */
          optscan->has_value = 1;
          optscan->string_value = ep;
        }
      }
    }
    else
    {
      if ( none_value )
      {
      /* ok, no value and no need value */
      }
      else
      {
        if ( use_def_string )
        {
          if ( opt_val && optscan->nextarg )
          {
          /* use_def_string && opt_val && next_arg */
            optscan->has_value = 2;
            optscan->string_value = optscan->nextarg;
          }
          else
          {
          /* ok, no value, value is optional, but use default value */
            optscan->has_value = 1;
            optscan->string_value = topt->s.def_string_value;
          }
        }
        else if ( use_def_nvalue )
        {
          optscan->has_value = 1;                                    /* ??? */
        }
        else if ( optscan->nextarg )
        {
        /* !use_def_string && next_arg */
          optscan->has_value = 2;
          optscan->string_value = optscan->nextarg;
        }
        else
        {
          rSETBAD;
          if ( optscan->variantid == MUCS_VARIANT_SHORT )
          {
            optscan->errors |= MUCS_ERROR_NO_VALUE;
            QRGOPTM( optscan->found_topt, rCODE, "no value given for \"-%c\" option (short)", topt->s.shortn );
          }
          else if ( optscan->variantid == MUCS_VARIANT_LONG )
          {
            optscan->errors |= MUCS_ERROR_NO_VALUE;
            QRGOPTM( optscan->found_topt, rCODE, "no value given for \"--%s\" option", topt->s.name ); /* !none_value && !has_value */
          }
        }
      }
    }
  }
  rRET;
}

int
mucs_config_option_lookup_option_table( const mucs_option_t * option_table, const char *arg_nopref, const char *eq, mucs_optscanner_t * optscan )
{
  rDECLBAD;

  if ( option_table )
  {
    rSETGOOD;
    for ( const mucs_option_t * topt = option_table; rGOOD && topt->s.name; topt++ )
    {
      if ( !optscan->found_topt || ( optscan->variantid == MUCS_VARIANT_LONG && strlen( topt->s.name ) > strlen( optscan->found_topt->s.name ) ) )
      {
        const char *ep = mucs_config_option_match_name( topt, optscan, arg_nopref, eq );

        if ( ep )
        {
          optscan->at_arg = NULL;
          optscan->found_topt = topt;
          rC( mucs_config_option_validate( ep, optscan ) );
        }
      }
    }
  }
  rRET;
}

mucs_flags_t
mucs_config_option_flags( const mucs_option_t * opt )
{
  mucs_flags_t osrcflags = opt && opt->d.source ? mucs_source_flags( opt->d.source ) : 0L;

  return opt ? ( osrcflags | opt->s.flags ) : 0L;
}

int
mucs_config_soption_flag( const mucs_option_static_t * sopt, mucs_flags_t mask )
{
  return sopt ? ( sopt->flags & mask ) : 0;
}

int
mucs_config_option_flag( const mucs_option_t * opt, mucs_flags_t mask )
{
  int osrcflag = opt->d.source ? mucs_source_flag( opt->d.source, mask ) : 0L;

  return opt ? ( osrcflag || mucs_config_soption_flag( &opt->s, mask ) ) : 0;
}

#if 0
int
mucs_option_set_source( mucs_option_t * opt, mucs_source_t * osrc )
{
  rDECLBAD;
  if ( opt && osrc )
  {
    rSETGOOD;
    opt->d.source = osrc;
  }
  rRET;
}
#endif
