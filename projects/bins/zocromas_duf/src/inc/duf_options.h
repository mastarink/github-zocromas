#ifndef MAS_DUF_OPTIONS_H
#  define MAS_DUF_OPTIONS_H

#  include "duf_option_types.h"
#  include "duf_options_def.h"

int duf_show_options( const char *a0 );

int duf_all_options( int argc, char *argv[] );



#  define DUF_OPTION(_lo)          duf_config->_lo
#  define DUF_OPTION_F(_lo)        DUF_OPTION(_lo)
#  define DUF_OPTION_N(_lo)        DUF_OPTION(_lo)
#  define DUF_OPTION_A(_lo, _a)    DUF_OPTION(_lo)._a


#  define DUF_OPTION_FLAGG(_lo, _pref, _fls) DUF_OPTION_F(_pref _fls._lo)
#  define DUF_OPTION_FLAG(_lo, _pref) DUF_OPTION_FLAGG(_lo, _pref, .v.flag)

#  define DUF_CLI_FLAG(_lo) DUF_OPTION_FLAG(_lo, cli)
#  define DUF_ACT_FLAG(_lo) DUF_OPTION_FLAG(_lo, cli.act)
#  define DUF_U_FLAG(_lo) DUF_OPTION_FLAG(_lo, u)

#  define DUF_CLI_NUM( _lo, n ) DUF_OPTION_NUM( _lo, cli.n )
#  define DUF_ACT_NUM(_lo)      DUF_CLI_NUM( _lo, act )
#  define DUF_U_NUM(_lo)        DUF_OPTION_NUM( _lo, u )
#  define DUF_TRACE_NUM(_lo)    DUF_CLI_NUM( _lo, trace )

#  define DUF_OPTION_NUM(_lo, _pref) DUF_OPTION_N(_pref._lo)


/*  ACQUIRE  */
#  define DUF_OPTION_ACQUIRE_FSET(_lo)    DUF_OPTION_F(_lo) = 1
#  define DUF_OPTION_ACQUIRE_FTEST(_lo)   DUF_OPTION_F(_lo) ? 1 : 0

#  define DUF_OPTION_ACQUIRE_FLAGG(_lo, _pref, _fls ) DUF_OPTION_ACQUIRE_FSET(_pref _fls._lo)
#  define DUF_OPTION_ACQUIRE_FLAG(_lo, _pref ) DUF_OPTION_ACQUIRE_FLAGG(_lo, _pref, .v.flag)

#  define DUF_OPTION_CASE_ACQUIRE_FLAGG(up, _lo, _pref, _fls) \
    case DUF_OPTION_FLAG_##up: \
       DUF_OPTION_ACQUIRE_FLAGG( _lo, _pref, _fls.flag ); \
    break
#  define DUF_OPTION_CASE_ACQUIRE_FLAG(up, _lo, _pref) DUF_OPTION_CASE_ACQUIRE_FLAGG(up, _lo, _pref, .v)


#  define DUF_OPTION_CASE_ACQUIRE_ACT_FLAG(up, _lo) DUF_OPTION_CASE_ACQUIRE_FLAG(up, _lo, cli.act)
#  define DUF_OPTION_CASE_ACQUIRE_U_FLAG(up, _lo) DUF_OPTION_CASE_ACQUIRE_FLAG(up, _lo, u)


/* # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # */
#  define DUF_OPTION_ACQUIRE_NSET(_lo, val)  \
   DUF_OPTION_N(_lo) = val

#  define DUF_OPTION_ACQUIRE_NUM(_lo)  \
      if ( optarg && *optarg ) \
        DUF_OPTION_ACQUIRE_NSET(_lo, duf_strtol_suff( optarg, &r) )



#  define DUF_OPTION_ACQUIRE_NUM_PLUS(_lo)  \
      if ( optarg && *optarg ) \
        DUF_OPTION_ACQUIRE_NSET(_lo, duf_strtol_suff( optarg, &r) ); \
      else \
        DUF_OPTION_N(_lo)++

#  define  DUF_OPTION_CASE_ACQUIRE_NUM( up, _lo, _pref ) \
    case DUF_OPTION_ ## up: \
      DUF_OPTION_ACQUIRE_NUM( _pref._lo ); \
    break

#  define  DUF_OPTION_CASE_ACQUIRE_NUM_PLUS( up, _lo, _pref ) \
    case DUF_OPTION_ ## up: \
      DUF_OPTION_ACQUIRE_NUM_PLUS( _pref._lo ); \
    break

#  define DUF_OPTION_CASE_ACQUIRE_ACT_NUM(up, _lo)      DUF_OPTION_CASE_ACQUIRE_NUM(up, _lo, cli.act)
#  define DUF_OPTION_CASE_ACQUIRE_ACT_NUM_PLUS(up, _lo) DUF_OPTION_CASE_ACQUIRE_NUM_PLUS(up, _lo, cli.act)
#  define DUF_OPTION_CASE_ACQUIRE_U_NUM(up, _lo)        DUF_OPTION_CASE_ACQUIRE_NUM(up, _lo, u)
#  define DUF_OPTION_CASE_ACQUIRE_U_NUM_PLUS(up, _lo)   DUF_OPTION_CASE_ACQUIRE_NUM_PLUS(up, _lo, u)

/* # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # */
#  define DUF_OPTION_ACQUIRE_TRACE( _lo )  \
    DUF_OPTION_ACQUIRE_NUM_PLUS( cli.trace._lo )

#  define DUF_OPTION_CASE_ACQUIRE_TRACE(up, _lo) \
    case DUF_OPTION_## up ##_TRACE: \
      DUF_OPTION_ACQUIRE_TRACE( _lo ); \
    break

/* # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # */
#  define DUF_OPTION_ACQUIRE_STR(_lo)  \
      if ( optarg ) \
      { \
        mas_free( DUF_OPTION_N(_lo) ); \
        DUF_OPTION_N(_lo) = mas_strdup( optarg ); \
      }
#  define DUF_OPTION_RESTORE_STR( ocode, ptr, up, _lo, _pref, maxlen) \
    DUF_OPTION_RESTORET( ocode, ptr, s, up, _lo, _pref, maxlen)

#  define  DUF_OPTION_CASE_ACQUIRE_STR( up, _lo, _pref ) \
    case DUF_OPTION_ ## up: \
      DUF_OPTION_ACQUIRE_STR( _pref._lo ); \
    break


/* # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # */


#  define DUF_OPTION_ACQUIRE_ARG(_lo)  \
      if ( optarg ) DUF_OPTION_A(_lo,argc)=mas_add_argv_arg(DUF_OPTION_A(_lo,argc), &DUF_OPTION_A(_lo,argv), optarg)

#  define  DUF_OPTION_CASE_ACQUIRE_ARG( up, _lo, _pref ) \
    case DUF_OPTION_ ## up: \
      DUF_OPTION_ACQUIRE_ARG( _pref._lo ); \
    break

#  define DUF_OPTION_CASE_ACQUIRE_U_ARG(up, _lo) DUF_OPTION_CASE_ACQUIRE_ARG(up, _lo, u)


#endif
