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

#  define DUF_RESTORE_OPTIONTV(ptr, typ, up, lo, pref, value) \
  if ( code==DUF_OPTION_ ## up && value ) \
  { \
    _duf_restore_option_ ## typ(ptr, code, value ); \
  }
#  define DUF_RESTORE_OPTIONV_B(ptr,  up, lo, pref, value) \
  DUF_RESTORE_OPTIONTV(ptr, b, up, lo, pref, value)
#  define DUF_RESTORE_OPTIONT(ptr, typ, up, lo, pref) \
  DUF_RESTORE_OPTIONTV(ptr, typ, up, lo, pref, duf_config->pref.lo)

#  define DUF_RESTORE_OPTION(ptr, up, lo, pref) \
    DUF_RESTORE_OPTIONT(ptr, i, up, lo, pref)
#  define DUF_RESTORE_OPTION_S(ptr, up, lo, pref) \
    DUF_RESTORE_OPTIONT(ptr, s, up, lo, pref)
#  define DUF_RESTORE_OPTION_B(ptr, up, lo, pref) \
    DUF_RESTORE_OPTIONT(ptr, b, up, lo, pref)

#  define DUF_RESTORE_OPTION_TRACE(ptr, up, lo) \
  DUF_RESTORE_OPTION(ptr,  up ## _TRACE, lo, cli.trace)



#endif
