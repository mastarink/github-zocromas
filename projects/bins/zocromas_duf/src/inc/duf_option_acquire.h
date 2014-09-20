#ifndef MAS_DUF_OPTION_ACQUIRE_H
#  define MAS_DUF_OPTION_ACQUIRE_H



/*  ACQUIRE  */
#  define DUF_OPTION_ACQUIRE_FSET(_lo)    DUF_OPTION_F(_lo) = 1
#  define DUF_OPTION_ACQUIRE_FTEST(_lo)   DUF_OPTION_F(_lo) ? 1 : 0

#  define DUF_OPTION_ACQUIRE_FLAGG(_lo, _pref, _fls ) DUF_OPTION_ACQUIRE_FSET(_pref _fls._lo)
#  define DUF_OPTION_ACQUIRE_FLAG(_lo, _pref ) DUF_OPTION_ACQUIRE_FLAGG(_lo, _pref, .v.flag)

#  define DUF_OPTION_CASE_ACQUIRE_FLAGG(up, _lo, _pref, _fls) \
    case DUF_OPTION_FLAG_##up: \
       DUF_OPTION_ACQUIRE_FLAGG( _lo, _pref, _fls.flag ); \
    break

#  define DUF_OPTION_CASE_ACQUIRE_FLAG(up, _lo, _ref, _pref) DUF_OPTION_CASE_ACQUIRE_FLAGG(up, _lo, _pref, _ref, v)


#  define DUF_OPTION_CASE_ACQUIRE_ACT_FLAG(up, _lo) DUF_OPTION_CASE_ACQUIRE_FLAG(up, _lo, ., cli.act)
#  define DUF_OPTION_CASE_ACQUIRE_U_FLAG(up, _lo) DUF_OPTION_CASE_ACQUIRE_FLAG(up, _lo, ->, pu)


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

#  define  DUF_OPTION_CASE_ACQUIRE_NUM( up, _lo, _ref, _pref ) \
    case DUF_OPTION_ ## up: \
      DUF_OPTION_ACQUIRE_NUM( _pref _ref _lo ); \
    break

#  define  DUF_OPTION_CASE_ACQUIRE_NUM_PLUS( up, _lo, _ref, _pref ) \
    case DUF_OPTION_ ## up: \
      DUF_OPTION_ACQUIRE_NUM_PLUS( _pref _ref _lo ); \
    break

#  define DUF_OPTION_CASE_ACQUIRE_ACT_NUM(up, _lo)      DUF_OPTION_CASE_ACQUIRE_NUM(up, _lo, ., cli.act)
#  define DUF_OPTION_CASE_ACQUIRE_ACT_NUM_PLUS(up, _lo) DUF_OPTION_CASE_ACQUIRE_NUM_PLUS(up, _lo, ., cli.act)
#  define DUF_OPTION_CASE_ACQUIRE_U_NUM(up, _lo)        DUF_OPTION_CASE_ACQUIRE_NUM(up, _lo, ->, pu)
#  define DUF_OPTION_CASE_ACQUIRE_U_NUM_PLUS(up, _lo)   DUF_OPTION_CASE_ACQUIRE_NUM_PLUS(up, _lo, ->, pu)

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

#  define  DUF_OPTION_CASE_ACQUIRE_STR( up, _lo, _pref ) \
    case DUF_OPTION_ ## up: \
      DUF_OPTION_ACQUIRE_STR( _pref._lo ); \
    break


/* # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # */


#  define DUF_OPTION_ACQUIRE_ARG(_lo)  \
      if ( optarg ) DUF_OPTION_A(_lo,argc)=mas_add_argv_arg(DUF_OPTION_A(_lo,argc), &DUF_OPTION_A(_lo,argv), optarg)

#  define  DUF_OPTION_CASE_ACQUIRE_ARG( up, _lo, _ref, _pref ) \
    case DUF_OPTION_ ## up: \
      DUF_OPTION_ACQUIRE_ARG( _pref _ref _lo ); \
    break

#  define DUF_OPTION_CASE_ACQUIRE_U_ARG(up, _lo) DUF_OPTION_CASE_ACQUIRE_ARG(up, _lo, ->, pu)


#endif

