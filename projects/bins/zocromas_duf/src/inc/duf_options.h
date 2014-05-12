#ifndef MAS_DUF_OPTIONS_H
#  define MAS_DUF_OPTIONS_H


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

#  define DUF_OPTION_RESTORETV(ptr, typ, up, lo, pref, value) \
  if ( code==DUF_OPTION_ ## up && value ) \
  { \
    _duf_restore_option_ ## typ(ptr, code, value ); \
  }
#  define DUF_OPTION_RESTOREV_B(ptr,  up, lo, pref, value) \
  DUF_OPTION_RESTORETV(ptr, b, up, lo, pref, value)

#  define DUF_OPTION_RESTORET(ptr, typ, up, lo, pref) \
  DUF_OPTION_RESTORETV(ptr, typ, up, lo, pref, duf_config->pref.lo)



/* # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # */
#  define DUF_OPTION_ACQUIRE_FLAG(code)  \
    duf_config->code = 1;

#  define DUF_OPTION_RESTORE_FLAG(ptr, up, lo, pref) \
    DUF_OPTION_RESTORET(ptr, b, FLAG_ ## up, lo, pref)

#define DUF_OPTION_CASE_ACQUIRE_FLAG(up, lo, pref) \
    case DUF_OPTION_FLAG_##up: \
      DUF_OPTION_ACQUIRE_FLAG( pref.lo ); \
    break

/* # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # */
#  define DUF_OPTION_ACQUIRE_NUM(code)  \
      if ( optarg && *optarg ) \
        duf_config->code = duf_strtol( optarg, &r); \

#  define DUF_OPTION_ACQUIRE_NUM_PLUS(code)  \
      if ( optarg && *optarg ) \
        duf_config->code = duf_strtol( optarg, &r); \
      else \
        duf_config->code++;

#  define DUF_OPTION_RESTORE_NUM(ptr, up, lo, pref) \
    DUF_OPTION_RESTORET(ptr, i, up, lo, pref)

#define  DUF_OPTION_CASE_ACQUIRE_NUM( up, lo, pref ) \
    case DUF_OPTION_ ## up: \
      DUF_OPTION_ACQUIRE_NUM( pref.lo ); \
    break


/* # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # */
#  define DUF_OPTION_ACQUIRE_TRACE(code)  \
    DUF_OPTION_ACQUIRE_NUM_PLUS(code)

#  define DUF_OPTION_RESTORE_TRACE(ptr, up, lo) \
  DUF_OPTION_RESTORE_NUM(ptr,  up ## _TRACE, lo, cli.trace)

#define DUF_OPTION_CASE_ACQUIRE_TRACE(up, lo) \
    case DUF_OPTION_## up ##_TRACE: \
      DUF_OPTION_ACQUIRE_TRACE( cli.trace.lo ); \
      break

/* # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # */
#  define DUF_OPTION_ACQUIRE_STR(code)  \
      if ( optarg ) \
      { \
        mas_free( duf_config->code ); \
        duf_config->code = mas_strdup( optarg ); \
      }
#  define DUF_OPTION_RESTORE_STR(ptr, up, lo, pref) \
    DUF_OPTION_RESTORET(ptr, s, up, lo, pref)

#define  DUF_OPTION_CASE_ACQUIRE_STR( up, lo, pref ) \
    case DUF_OPTION_ ## up: \
      DUF_OPTION_ACQUIRE_STR( pref.lo ); \
    break


/* # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # */


#  define DUF_OPTION_ACQUIRE_ARG(code)  \
      if ( optarg ) duf_config->code.argc=mas_add_argv_arg(duf_config->code.argc, &duf_config->code.argv, optarg)

#define  DUF_OPTION_CASE_ACQUIRE_ARG( up, lo, pref ) \
    case DUF_OPTION_ ## up: \
      DUF_OPTION_ACQUIRE_ARG( pref.lo ); \
    break



#endif
