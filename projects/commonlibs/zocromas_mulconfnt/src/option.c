#define R_GOOD(_r) ((_r)>=0)
#include "mulconfnt_defs.h"
#include <string.h>
#include <math.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/tools/mas_argvc_tools.h>

#include <mastar/minierr/minierr.h>

#include "mulconfnt_structs.h"

#include "global.h"

/* #include "tools.h" */
/* #include "source.h" */

#include "option_ref.h"

/* #include "option_base.h" */
#include "option_flags.h"
#include "option_value.h"

#include "option.h"

/*
 *
 * mucs_config_option_...
 *
 * */

/*
 * if match
 *   returns end-pointer of name at arg_nopref
 * else
 *   returns NULL
 * */
static const char *
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

/*
none_value	option must be without value, value is error
no_value	use default value, if flag set(use_def_string,use_def_nvalue), otherwise errancy depends on optional flag (opt_val)
 */

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
#if 0
          if ( opt_val && optscan->nextarg )
          {
          /* use_def_string && opt_val && next_arg */
            optscan->has_value = 2;
            optscan->string_value = optscan->nextarg;
          }
          else
#endif
          {
          /* ok, no value, value is optional, but use default value */
            optscan->has_value = 1;
            optscan->string_value = topt->s.def_string_value;
          }
        }
        else if ( use_def_nvalue )
        {
          optscan->has_value = 1;                                    /* ??? ???????? */
        }
        else if ( optscan->nextarg )
        {
        /* !use_def_string && next_arg */
          optscan->has_value = 2;
          optscan->string_value = optscan->nextarg;
        }
        else if ( !opt_val )
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
mucs_config_option_lookup_options( const mucs_option_t * options, unsigned optcount, const char *arg_nopref, const char *eq,
                                   mucs_optscanner_t * optscan )
{
  rDECLBAD;

  if ( options )
  {
    rSETGOOD;
    for ( const mucs_option_t * topt = options; rGOOD /* && topt->s.name */  && ( ( unsigned ) ( topt - options ) ) < optcount; topt++ )
    {
      if ( topt->s.name                                              /* && ( topt->pass < 0 || topt->pass == optscan->pass ) */
           && ( !optscan->found_topt
                || ( optscan->variantid == MUCS_VARIANT_LONG && strlen( topt->s.name ) > strlen( optscan->found_topt->s.name ) ) ) )
      {
        const char *ep = mucs_config_option_match_name( topt, optscan, arg_nopref, eq );

        if ( ep )
        {
          optscan->at_arg = NULL;
          optscan->found_topt = topt;
        /* if ( optscan->found_topts && optscan->found_num < optscan->found_max ) */
        /*   optscan->found_topts[optscan->found_num++] = topt;                   */
          rC( mucs_config_option_validate( ep, optscan ) );
        }
      }
    }
  }
  rRET;
}

int
mucs_config_option_evaluate( mucs_option_t * opt, mucs_optscanner_t * optscan, void *userdata )
{
  rDECLGOOD;
  if ( optscan->has_value )
  {
    mucs_config_option_ptr_to_nvalue( opt );

    rC( mucs_config_option_set_value( opt, optscan->string_value, &optscan->errors ) );
    opt->d.has_value = optscan->has_value;
  }
  else
  /* if ( mucs_config_option_flag( opt, MUCS_FLAG_OPTIONAL_VALUE ) ) */
  {
    rSETGOOD;
  }

  if ( rGOOD )
  {
    if ( ( !mucs_global_flag( MUCS_FLAG_USE_CBPASS ) || opt->s.cb_pass < 0 || opt->s.cb_pass == optscan->pass ) )
    {
      if ( opt->s.callback /* && !opt->s.cust_ptr */  )
      {
        rC( opt->s.callback( opt, userdata, opt->s.extra_data ) );
        opt->d.extra_cb.callback_called++;
      }
      if ( opt->s.callback_s )
      {
        rC( opt->s.callback_s( mucs_config_option_string_value( opt ), mucs_config_option_npos( opt ), userdata, opt->s.extra_data ) );
        opt->d.extra_cb.callback_called++;
      }
    }

    if ( rGOOD && opt->d.has_value )
      mucs_config_option_nvalue_to_ptr( opt );
  }

  rRET;
}
