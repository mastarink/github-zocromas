#ifndef MAS_DUF_OPTIONS_H
#  define MAS_DUF_OPTIONS_H

#  include "duf_option_types.h"
#  include "duf_options_def.h"

int duf_show_options( const char *a0 );

char *duf_restore_some_options( const char *a0 );

int duf_cli_option_by_string( const char *string );

int duf_env_options_at_var( int argc, char *argv[], const char *envvarname );
int duf_env_options( int argc, char *argv[] );

int duf_infile_options_at_file( int argc, char *argv[], const char *filename );
int duf_infile_options( int argc, char *argv[] );

int duf_all_options( int argc, char *argv[] );

int duf_find_long( duf_option_code_t code );

/* const duf_longval_extended_t *_duf_find_longval_extended( duf_option_code_t code ); */
const duf_longval_extended_t *duf_longindex_extended( int longindex );
const char *duf_longindex_extended_name( int longindex );
duf_option_code_t duf_longindex_extended_codeval( int longindex );

const char *duf_find_longval_help( duf_option_code_t code );

char *duf_cli_option_shorts( void );

const char *duf_option_cnames_tmp( int index, duf_option_code_t code );
char *duf_option_names_d( duf_option_code_t code, const char *delim );

char *duf_option_names( duf_option_code_t code );
char *duf_option_names_d( duf_option_code_t code, const char *delim );


char *duf_option_description( int longindex, const duf_longval_extended_t * extended );
char *duf_option_description_d( int longindex, const char *delimh, const char *delim );

#  define DUF_OPT_NAME(n) duf_option_cnames_tmp( -1, DUF_OPTION_ ## n )
#  define DUF_OPT_FLAG_NAME(n) duf_option_cnames_tmp( -1, DUF_OPTION_FLAG_ ## n )
#  define DUF_OPT_NAME1(n) duf_option_cnames_tmp( 1, DUF_OPTION_ ## n )
#  define DUF_OPT_FLAG_NAME1(n) duf_option_cnames_tmp( 1, DUF_OPTION_FLAG_ ## n )
#  define DUF_OPT_NAME2(n) duf_option_cnames_tmp( 2, DUF_OPTION_ ## n )
#  define DUF_OPT_FLAG_NAME2(n) duf_option_cnames_tmp( 2, DUF_OPTION_FLAG_ ## n )

#  define DUF_OPTION_NAME(n)  duf_option_cnames_tmp(-1,n)
#  define DUF_OPTION_LINDEX_NAME(n)  duf_option_cnames_tmp( -1, duf_longindex_extended_codeval( n ) )

#  define DUF_OPTION(lo)  \
    duf_config->lo
#  define DUF_OPTION_F(lo)  \
    DUF_OPTION(lo)
#  define DUF_OPTION_N(lo)  \
    DUF_OPTION(lo)
#  define DUF_OPTION_A(lo, a)  \
    DUF_OPTION(lo).a


#  define DUF_OPTION_RESTORETV(ocode, ptr, typ, up, lo, pref, value, maxlen) \
  if ( ocode==DUF_OPTION_ ## up && value ) \
  { \
    _duf_restore_option_ ## typ(ptr, ocode, value, maxlen ); \
  }

#  define DUF_OPTION_RESTOREV_B(ocode, ptr,  up, lo, pref, value, maxlen) \
  DUF_OPTION_RESTORETV(ocode, ptr, b, up, lo, pref, value, maxlen)

#  define DUF_OPTION_RESTORET( ocode, ptr, typ, up, lo, pref, maxlen) \
  DUF_OPTION_RESTORETV( ocode, ptr, typ, up, lo, pref, DUF_OPTION(pref.lo), maxlen)



/* # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # */
#  define DUF_OPTION_RESTORE_FLAGG(ocode, ptr, up, lo, pref, fls, maxlen) \
    DUF_OPTION_RESTORET( ocode, ptr, b, FLAG_ ## up, lo, pref fls.flag, maxlen)
#  define DUF_OPTION_RESTORE_FLAG(ocode, ptr, up, lo, pref, maxlen) \
    DUF_OPTION_RESTORE_FLAGG(ocode, ptr, up, lo, pref, .v, maxlen)

/*  ACQUIRE  */
#  define DUF_OPTION_ACQUIRE_FSET(lo)  \
   DUF_OPTION_F(lo) = 1

#  define DUF_OPTION_ACQUIRE_FLAGG(lo, pref, fls ) DUF_OPTION_ACQUIRE_FSET(pref fls.lo)
#  define DUF_OPTION_ACQUIRE_FLAG(lo, pref ) DUF_OPTION_ACQUIRE_FLAGG(lo, pref, .v.flag)

#  define DUF_OPTION_FLAGG(lo, pref, fls) DUF_OPTION_F(pref fls.lo)
#  define DUF_OPTION_FLAG(lo, pref) DUF_OPTION_FLAGG(lo, pref, .v.flag)

#  define DUF_OPTION_CASE_ACQUIRE_FLAGG(up, lo, pref, fls) \
    case DUF_OPTION_FLAG_##up: \
       DUF_OPTION_ACQUIRE_FLAGG( lo, pref, fls.flag ); \
    break
#  define DUF_OPTION_CASE_ACQUIRE_FLAG(up, lo, pref) DUF_OPTION_CASE_ACQUIRE_FLAGG(up, lo, pref, .v)


#  define DUF_OPTION_CASE_ACQUIRE_ACT_FLAG(up, lo) DUF_OPTION_CASE_ACQUIRE_FLAG(up, lo, cli.act)
#  define DUF_OPTION_CASE_ACQUIRE_U_FLAG(up, lo) DUF_OPTION_CASE_ACQUIRE_FLAG(up, lo, u)

#  define DUF_CLI_FLAG(lo) DUF_OPTION_FLAG(lo, cli)
#  define DUF_ACT_FLAG(lo) DUF_OPTION_FLAG(lo, cli.act)
#  define DUF_U_FLAG(lo) DUF_OPTION_FLAG(lo, u)

/* # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # */
#  define DUF_OPTION_ACQUIRE_NSET(lo, val)  \
   DUF_OPTION_N(lo) = val

#  define DUF_OPTION_ACQUIRE_NUM(lo)  \
      if ( optarg && *optarg ) \
        DUF_OPTION_ACQUIRE_NSET(lo, duf_strtol( optarg, &r) )

#  define DUF_OPTION_NUM(lo, pref) DUF_OPTION_N(pref.lo)


#  define DUF_OPTION_ACQUIRE_NUM_PLUS(lo)  \
      if ( optarg && *optarg ) \
        DUF_OPTION_ACQUIRE_NSET(lo, duf_strtol( optarg, &r) ); \
      else \
        DUF_OPTION_N(lo)++

#  define DUF_OPTION_RESTORE_NUM( ocode, ptr, up, lo, pref, maxlen) \
    DUF_OPTION_RESTORET( ocode, ptr, i, up, lo, pref, maxlen)

#  define  DUF_OPTION_CASE_ACQUIRE_NUM( up, lo, pref ) \
    case DUF_OPTION_ ## up: \
      DUF_OPTION_ACQUIRE_NUM( pref.lo ); \
    break

#  define  DUF_OPTION_CASE_ACQUIRE_NUM_PLUS( up, lo, pref ) \
    case DUF_OPTION_ ## up: \
      DUF_OPTION_ACQUIRE_NUM_PLUS( pref.lo ); \
    break

#  define DUF_OPTION_CASE_ACQUIRE_ACT_NUM(up, lo)      DUF_OPTION_CASE_ACQUIRE_NUM(up, lo, cli.act)
#  define DUF_OPTION_CASE_ACQUIRE_ACT_NUM_PLUS(up, lo) DUF_OPTION_CASE_ACQUIRE_NUM_PLUS(up, lo, cli.act)
#  define DUF_OPTION_CASE_ACQUIRE_U_NUM(up, lo)        DUF_OPTION_CASE_ACQUIRE_NUM(up, lo, u)
#  define DUF_OPTION_CASE_ACQUIRE_U_NUM_PLUS(up, lo)   DUF_OPTION_CASE_ACQUIRE_NUM_PLUS(up, lo, u)

#  define DUF_CLI_NUM( lo, n ) DUF_OPTION_NUM( lo, cli.n )
#  define DUF_ACT_NUM(lo)      DUF_CLI_NUM( lo, act )
#  define DUF_U_NUM(lo)        DUF_OPTION_NUM( lo, u )
#  define DUF_TRACE_NUM(lo)    DUF_CLI_NUM( lo, trace )

/* # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # */
#  define DUF_OPTION_RESTORE_TRACE(ocode, ptr, up, lo, maxlen) \
  DUF_OPTION_RESTORE_NUM(ocode, ptr,  up ## _TRACE, lo, cli.trace, maxlen)

#  define DUF_OPTION_ACQUIRE_TRACE( lo )  \
    DUF_OPTION_ACQUIRE_NUM_PLUS( cli.trace.lo )

#  define DUF_OPTION_CASE_ACQUIRE_TRACE(up, lo) \
    case DUF_OPTION_## up ##_TRACE: \
      DUF_OPTION_ACQUIRE_TRACE( lo ); \
    break

/* # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # */
#  define DUF_OPTION_ACQUIRE_STR(lo)  \
      if ( optarg ) \
      { \
        mas_free( DUF_OPTION_N(lo) ); \
        DUF_OPTION_N(lo) = mas_strdup( optarg ); \
      }
#  define DUF_OPTION_RESTORE_STR( ocode, ptr, up, lo, pref, maxlen) \
    DUF_OPTION_RESTORET( ocode, ptr, s, up, lo, pref, maxlen)

#  define  DUF_OPTION_CASE_ACQUIRE_STR( up, lo, pref ) \
    case DUF_OPTION_ ## up: \
      DUF_OPTION_ACQUIRE_STR( pref.lo ); \
    break


/* # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # */


#  define DUF_OPTION_ACQUIRE_ARG(lo)  \
      if ( optarg ) DUF_OPTION_A(lo,argc)=mas_add_argv_arg(DUF_OPTION_A(lo,argc), &DUF_OPTION_A(lo,argv), optarg)

#  define  DUF_OPTION_CASE_ACQUIRE_ARG( up, lo, pref ) \
    case DUF_OPTION_ ## up: \
      DUF_OPTION_ACQUIRE_ARG( pref.lo ); \
    break

#  define DUF_OPTION_CASE_ACQUIRE_U_ARG(up, lo) DUF_OPTION_CASE_ACQUIRE_ARG(up, lo, u)

duf_option_class_t duf_help_option2class( duf_option_code_t code );

void duf_option_smart_help( duf_option_class_t oclass );
void duf_option_help( int argc, char **argv );
void duf_option_examples( int argc, char **argv );
void duf_option_version( int argc, char **argv );

#endif
