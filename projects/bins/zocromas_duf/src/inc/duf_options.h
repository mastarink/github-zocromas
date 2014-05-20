#ifndef MAS_DUF_OPTIONS_H
#  define MAS_DUF_OPTIONS_H

#include "duf_opt_types.h"

int duf_show_options( const char *a0 );

char *duf_restore_options( const char *a0 );

int duf_cli_option_by_string( const char *string );

int duf_infile_options( int argc, char *argv[] );

const duf_longval_extended_t *duf_find_longval_extended( duf_option_code_t code );
const char *duf_find_longval_help( duf_option_code_t code );

char *duf_cli_option_shorts( void );
char *duf_option_names( duf_option_code_t code );
char *duf_option_description( duf_option_code_t code );
void duf_option_smart_help( void );

#  define DUF_OPTION(code)  \
    duf_config->code
#  define DUF_OPTION_F(code)  \
    DUF_OPTION(code)
#  define DUF_OPTION_N(code)  \
    DUF_OPTION(code)
#  define DUF_OPTION_A(code, a)  \
    DUF_OPTION(code).a


#  define DUF_OPTION_RESTORETV(ptr, typ, up, lo, pref, value) \
  if ( code==DUF_OPTION_ ## up && value ) \
  { \
    _duf_restore_option_ ## typ(ptr, code, value ); \
  }
#  define DUF_OPTION_RESTOREV_B(ptr,  up, lo, pref, value) \
  DUF_OPTION_RESTORETV(ptr, b, up, lo, pref, value)

#  define DUF_OPTION_RESTORET(ptr, typ, up, lo, pref) \
  DUF_OPTION_RESTORETV(ptr, typ, up, lo, pref, DUF_OPTION(pref.lo))



/* # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # */
#  define DUF_OPTION_RESTORE_FLAGG(ptr, up, lo, pref, fls) \
    DUF_OPTION_RESTORET(ptr, b, FLAG_ ## up, lo, pref fls)
#  define DUF_OPTION_RESTORE_FLAG(ptr, up, lo, pref) \
    DUF_OPTION_RESTORE_FLAGG(ptr, up, lo, pref, .flag)


#  define DUF_OPTION_ACQUIRE_FSET(code)  \
   DUF_OPTION_F(code) = 1

#  define DUF_OPTION_ACQUIRE_FLAG(code, pref ) DUF_OPTION_ACQUIRE_FSET(pref.flag.code)
#  define DUF_OPTION_FLAG(code, pref) DUF_OPTION_F(pref.flag.code)

#  define DUF_OPTION_CASE_ACQUIRE_FLAGG(up, lo, pref, fls) \
    case DUF_OPTION_FLAG_##up: \
      DUF_OPTION_ACQUIRE_FSET( pref fls.lo ); \
    break

#  define DUF_OPTION_CASE_ACQUIRE_FLAG(up, lo, pref) DUF_OPTION_CASE_ACQUIRE_FLAGG(up, lo, pref, .flag)

#  define DUF_OPTION_CASE_ACQUIRE_ACT_FLAG(up, lo) DUF_OPTION_CASE_ACQUIRE_FLAG(up, lo, cli.act)
#  define DUF_OPTION_CASE_ACQUIRE_U_FLAG(up, lo) DUF_OPTION_CASE_ACQUIRE_FLAG(up, lo, u)

#  define DUF_ACT_FLAG(lo) DUF_OPTION_FLAG(lo, cli.act)
#  define DUF_U_FLAG(lo) DUF_OPTION_FLAG(lo, u)

/* # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # */
#  define DUF_OPTION_ACQUIRE_NSET(code, val)  \
   DUF_OPTION_N(code) = val

#  define DUF_OPTION_ACQUIRE_NUM(code)  \
      if ( optarg && *optarg ) \
        DUF_OPTION_ACQUIRE_NSET(code, duf_strtol( optarg, &r) )

#  define DUF_OPTION_NUM(code, pref) DUF_OPTION_N(pref.code)


#  define DUF_OPTION_ACQUIRE_NUM_PLUS(code)  \
      if ( optarg && *optarg ) \
        DUF_OPTION_ACQUIRE_NSET(code, duf_strtol( optarg, &r) ); \
      else \
        DUF_OPTION_N(code)++

#  define DUF_OPTION_RESTORE_NUM(ptr, up, lo, pref) \
    DUF_OPTION_RESTORET(ptr, i, up, lo, pref)

#  define  DUF_OPTION_CASE_ACQUIRE_NUM( up, lo, pref ) \
    case DUF_OPTION_ ## up: \
      DUF_OPTION_ACQUIRE_NUM( pref.lo ); \
    break

#  define DUF_OPTION_CASE_ACQUIRE_ACT_NUM(up, lo) DUF_OPTION_CASE_ACQUIRE_NUM(up, lo, cli.act)
#  define DUF_OPTION_CASE_ACQUIRE_U_NUM(up, lo) DUF_OPTION_CASE_ACQUIRE_NUM(up, lo, u)

#  define DUF_ACT_NUM(lo) DUF_OPTION_NUM(lo, cli.act)
#  define DUF_U_NUM(lo) DUF_OPTION_NUM(lo, u)

/* # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # */
#  define DUF_OPTION_ACQUIRE_TRACE(code)  \
    DUF_OPTION_ACQUIRE_NUM_PLUS(code)

#  define DUF_OPTION_RESTORE_TRACE(ptr, up, lo) \
  DUF_OPTION_RESTORE_NUM(ptr,  up ## _TRACE, lo, cli.trace)

#  define DUF_OPTION_CASE_ACQUIRE_TRACE(up, lo) \
    case DUF_OPTION_## up ##_TRACE: \
      DUF_OPTION_ACQUIRE_TRACE( cli.trace.lo ); \
    break

/* # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # */
#  define DUF_OPTION_ACQUIRE_STR(code)  \
      if ( optarg ) \
      { \
        mas_free( DUF_OPTION_N(code) ); \
        DUF_OPTION_N(code) = mas_strdup( optarg ); \
      }
#  define DUF_OPTION_RESTORE_STR(ptr, up, lo, pref) \
    DUF_OPTION_RESTORET(ptr, s, up, lo, pref)

#  define  DUF_OPTION_CASE_ACQUIRE_STR( up, lo, pref ) \
    case DUF_OPTION_ ## up: \
      DUF_OPTION_ACQUIRE_STR( pref.lo ); \
    break


/* # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # */


#  define DUF_OPTION_ACQUIRE_ARG(code)  \
      if ( optarg ) DUF_OPTION_A(code,argc)=mas_add_argv_arg(DUF_OPTION_A(code,argc), &DUF_OPTION_A(code,argv), optarg)

#  define  DUF_OPTION_CASE_ACQUIRE_ARG( up, lo, pref ) \
    case DUF_OPTION_ ## up: \
      DUF_OPTION_ACQUIRE_ARG( pref.lo ); \
    break

#  define DUF_OPTION_CASE_ACQUIRE_U_ARG(up, lo) DUF_OPTION_CASE_ACQUIRE_ARG(up, lo, u)


#endif
