#define R_GOOD(_r) ((_r)>=0)
#include "mulconfnt_defs.h"
#include <string.h>
#include <math.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_argvc_tools.h>

#include <mastar/minierr/minierr.h>

#include "mulconfnt_structs.h"

/* #include "mulconfnt_error.h" */

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
mucs_config_option_nvalue_from_ptr( mucs_option_t * opt )
{
  if ( opt->argptr )
  {
    switch ( opt->restype & ~MUCS_RTYP_FLAG_ALL )
    {
    case MUCS_RTYP_NONE:
    case MUCS_RTYP_STRING:
    case MUCS_RTYP_TARG:
      break;
    case MUCS_RTYP_CHAR:
      opt->nvalue.v_char = *( ( char * ) opt->argptr );
//        v_long_long = ( long long ) opt->nvalue.v_char;
      break;
    case MUCS_RTYP_UCHAR:
      opt->nvalue.v_uchar = *( ( unsigned char * ) opt->argptr );
//        v_ulong_long = ( unsigned long long ) opt->nvalue.v_uchar;
      break;
    case MUCS_RTYP_SHORT:
      opt->nvalue.v_short = *( ( short * ) opt->argptr );
//        v_long_long = ( long long ) opt->nvalue.v_short;
      break;
    case MUCS_RTYP_USHORT:
      opt->nvalue.v_ushort = *( ( unsigned short * ) opt->argptr );
//        v_ulong_long = ( unsigned long long ) opt->nvalue.v_ushort;
      break;
    case MUCS_RTYP_INT:
      opt->nvalue.v_int = *( ( int * ) opt->argptr );
//        v_long_long = ( long long ) opt->nvalue.v_int;
      break;
    case MUCS_RTYP_UINT:
      opt->nvalue.v_uint = *( ( unsigned int * ) opt->argptr );
//        v_ulong_long = ( unsigned long long ) opt->nvalue.v_uint;
      break;
    case MUCS_RTYP_LONG:
      opt->nvalue.v_long = *( ( long * ) opt->argptr );
//        v_long_long = ( long long ) opt->nvalue.v_long;
      break;
    case MUCS_RTYP_ULONG:
      opt->nvalue.v_ulong = *( ( unsigned long * ) opt->argptr );
//        v_ulong_long = ( unsigned long long ) opt->nvalue.v_ulong;
      break;
    case MUCS_RTYP_LONG_LONG:
      opt->nvalue.v_long_long = *( ( long long * ) opt->argptr );
//        v_long_long = opt->nvalue.v_long_long;
      break;
    case MUCS_RTYP_ULONG_LONG:
      opt->nvalue.v_ulong_long = *( ( unsigned long long * ) opt->argptr );
//        v_ulong_long = opt->nvalue.v_ulong_long;
      break;
    case MUCS_RTYP_DOUBLE:
      opt->nvalue.v_double = *( ( unsigned long long * ) opt->argptr );
//        v_double = opt->nvalue.v_double;
      break;
    case MUCS_RTYP_LDOUBLE:
      opt->nvalue.v_ldouble = *( ( unsigned long long * ) opt->argptr );
//        v_ldouble = opt->nvalue.v_ldouble;
      break;
    }
  }
}

static void
mucs_config_option_nvalue_to_ptr( mucs_option_t * opt )
{
  if ( opt->argptr )
  {
    switch ( opt->restype & ~MUCS_RTYP_FLAG_ALL )
    {
    case MUCS_RTYP_NONE:
      break;
    case MUCS_RTYP_STRING:
      *( ( char ** ) opt->argptr ) = mas_strdup( opt->string_value );
      break;
    case MUCS_RTYP_TARG:
      {
        mas_argvc_t *targ = ( mas_argvc_t * ) opt->argptr;

        mas_add_argvc_arg( targ, opt->string_value );
      }
      break;
    case MUCS_RTYP_CHAR:
      *( ( char * ) opt->argptr ) = opt->nvalue.v_char;
      break;
    case MUCS_RTYP_UCHAR:
      *( ( unsigned char * ) opt->argptr ) = opt->nvalue.v_char;
      break;
    case MUCS_RTYP_SHORT:
      *( ( short * ) opt->argptr ) = opt->nvalue.v_short;
      break;
    case MUCS_RTYP_USHORT:
      *( ( unsigned short * ) opt->argptr ) = opt->nvalue.v_short;
      break;
    case MUCS_RTYP_INT:
      *( ( int * ) opt->argptr ) = opt->nvalue.v_int;
      break;
    case MUCS_RTYP_UINT:
      *( ( unsigned int * ) opt->argptr ) = opt->nvalue.v_uint;
      break;
    case MUCS_RTYP_LONG:
      *( ( long * ) opt->argptr ) = opt->nvalue.v_long;
      break;
    case MUCS_RTYP_ULONG:
      *( ( unsigned long * ) opt->argptr ) = opt->nvalue.v_ulong;
      break;
    case MUCS_RTYP_LONG_LONG:
      *( ( long long * ) opt->argptr ) = opt->nvalue.v_long_long;
      break;
    case MUCS_RTYP_ULONG_LONG:
      *( ( unsigned long long * ) opt->argptr ) = opt->nvalue.v_ulong_long;
      break;
    case MUCS_RTYP_DOUBLE:
      *( ( double * ) opt->argptr ) = opt->nvalue.v_double;
      break;
    case MUCS_RTYP_LDOUBLE:
      *( ( long double * ) opt->argptr ) = opt->nvalue.v_ldouble;
      break;
    }
    opt->value_is_set++;
  }
}

static nvalue_t
mucs_config_option_string_to_nvalue( mucs_option_t * opt, int *perr )
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
      *perr = ( ep != string + strlen( string ) );
      break;
    }
  }
  return v_x;
}

static void
mucs_config_option_set_nvalue( mucs_option_t * opt )
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
      QRGOPTM( opt, -1, "wrong value '%s'", opt->string_value );
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
      {
        QRGOPTM( opt, -1, "wrong value '%s'", opt->string_value );
      }
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
      {
        QRGOPTM( opt, -1, "wrong value '%s'", opt->string_value );
      }
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
      {
        QRGOPTM( opt, -1, "wrong value '%s'", opt->string_value );
      }
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
      {
        QRGOPTM( opt, -1, "wrong value '%s'", opt->string_value );
      }
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
      {
        QRGOPTM( opt, -1, "wrong value '%s'", opt->string_value );
      }
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
      {
        QRGOPTM( opt, -1, "wrong value '%s'", opt->string_value );
      }
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
        QRGOPTM( opt, -1, "wrong value '%s'", opt->string_value );
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
        QRGOPTM( opt, -1, "wrong value '%s'", opt->string_value );
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
mucs_config_option_set_value( mucs_option_t * opt, const char *string_value )
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
    mucs_config_option_set_nvalue( opt );
  }
}

/*
 * if match
 *   returns end-pointer of name at arg
 * else
 *   returns NULL
 * */
const char *
mucs_config_option_match_name( const mucs_option_t * topt, mucs_optscanner_t * optscan, const char *arg, const char *eq )
{
  unsigned larg = strlen( arg );
  unsigned lname = 0;
  const char *argp = NULL;

  if ( optscan->variantid == MUCS_VARIANT_SHORT )
  {
    lname = 1;
    argp = arg + lname;
  /* WARN( "{%s} => '%s' %d:%d:%d", arg, argp, ( larg == lname ), ( *arg == topt->shortn ), ( !*argp || strchr( " \t", *argp ) ) ); */
    if ( !( ( *arg == topt->shortn ) && ( larg == lname || strchr( " \t", *argp ) ) ) )
    {
      argp = NULL;
    }
  }
  else if ( optscan->variantid == MUCS_VARIANT_LONG )
  {
    lname = strlen( topt->name );
    if ( 0 == strncmp( topt->name, arg, lname ) )
    {
      argp = arg + lname;
      if ( *argp )
      {
        unsigned leq = strlen( eq );
        int with_eq = 0;

        with_eq = ( eq && 0 == strncmp( argp, eq, leq ) );
        if ( with_eq )
          argp += leq;
        else if ( mucs_config_option_flag( topt, MUCS_FLAG_NEED_EQ ) )
          argp = NULL;
      }
      else if ( mucs_config_option_flag( topt, MUCS_FLAG_NEED_EQ ) )
        argp = NULL;
    }
  }
  return argp;
}

int
mucs_config_option_lookup_option_table( const mucs_option_t * option_table, const char *arg, const char *eq, mucs_optscanner_t * optscan )
{
  rDECLBAD;
  int found = 0;

  if ( option_table )
  {
    rSETGOOD;
    for ( const mucs_option_t * topt = option_table; rGOOD && topt->name; topt++ )
    {
      int without_value = ( ( topt->restype & ~MUCS_RTYP_FLAG_ALL ) == MUCS_RTYP_NONE );
      const char *ep = mucs_config_option_match_name( topt, optscan, arg, eq );

      if ( ep )
      {
        found = 1;
        if ( optscan->found_topt && strlen( topt->name ) <= strlen( optscan->found_topt->name ) )
          continue;
        optscan->found_topt = topt;
        if ( optscan->force_value )                                  /* use force_value for alias inheritance (only!?) */
        {
        /* B. "--opt" ( + forced value )  */
          optscan->has_value = 1;
          optscan->string_value = optscan->force_value;
        }
        else
        {
        /* WARN( "[%s] '%s'", arg, ep ); */
          while ( *ep && strchr( " \t", *ep ) )
            ep++;
          if ( without_value )
          {
            if ( *ep )
            {
            /* TODO bad: value */
              QRGOPTM( opt, -1, "unexpected value given for \"-%c\" option (short)", topt->shortn );
              optscan->has_value = 1;
              optscan->string_value = ep;
              found = -found;
              rSETBAD;
            }
            else
            {
            /* TODO good: no value */
            }
          }
          else                                                       /* need value */
          {
          /* WARN( "[%s] '%s'", arg, ep ); */
            if ( *ep )
            {
            /* TODO good: possible value, here */
              optscan->has_value = 1;
              optscan->string_value = ep;
            }
            else
            {
              if ( optscan->nextarg )
              {
                optscan->has_value = 2;
                optscan->string_value = optscan->nextarg;
              }
              else if ( !mucs_config_option_flag( topt, MUCS_FLAG_OPTIONAL_VALUE ) )
              {
                if ( optscan->variantid == MUCS_VARIANT_SHORT )
                {
                  QRGOPTM( opt, -1, "no value given for \"-%c\" option (short)", topt->shortn );
                  rSETBAD;
                }
                else if ( optscan->variantid == MUCS_VARIANT_LONG )
                {
                  QRGOPTM( opt, -1, "no value given for \"--%s\" option", topt->name ); /* !without_value && !has_value */
                  rSETBAD;
                }
                found = -found;
              }
            }
          }
        }
      }
    }
  }
/* return optscan->found_topt; */
  rRET;
}

unsigned long
mucs_config_option_flags( const mucs_option_t * opt )
{
  unsigned long osrcflag = opt && opt->source ? mucs_source_flags( opt->source ) : 0L;

  return opt ? ( osrcflag | opt->flags ) : 0L;
}

int
mucs_config_option_flag( const mucs_option_t * opt, unsigned long mask )
{
  unsigned long osrcflag = opt->source ? mucs_source_flag( opt->source, mask ) : 0L;

/* return opt ? ( osrcflag | ( opt->flags & mask ) ? 1 : 0 ) : 0; */
  return opt ? ( osrcflag || ( opt->flags & mask ) ) : 0;
}

#if 0
int
mucs_option_set_source( mucs_option_t * opt, mucs_source_t * osrc )
{
  rDECLBAD;
  if ( opt && osrc )
  {
    rSETGOOD;
    opt->source = osrc;
  }
  rRET;
}
#endif
