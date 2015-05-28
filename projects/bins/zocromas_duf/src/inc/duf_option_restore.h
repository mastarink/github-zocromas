#ifndef MAS_DUF_OPTION_RESTORE_H
#  define MAS_DUF_OPTION_RESTORE_H



char *duf_restore_some_options( const char *a0 );
void duf_restore_some_option( char *ptr, duf_option_code_t codeval, size_t maxlen );


#  define DUF_OPTION_RESTORETV(ocode, ptr, typ, up, lo, pref, value, maxlen) \
  if ( ocode==DUF_OPTION_VAL_ ## up && value ) \
  { \
    _duf_restore_option_ ## typ(ptr, ocode, value, maxlen ); \
  }

#  define DUF_OPTION_RESTOREV_B(ocode, ptr,  up, lo, pref, value, maxlen) \
  DUF_OPTION_RESTORETV(ocode, ptr, b, up, lo, pref, value, maxlen)

#  define DUF_OPTION_RESTORET( ocode, ptr, typ, up, lo, _ref, pref, maxlen) \
  DUF_OPTION_RESTORETV( ocode, ptr, typ, up, lo, pref, DUF_OPTION(pref _ref lo), maxlen)



/* # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # */
#  define DUF_OPTION_RESTORE_FLAGG(ocode, ptr, up, lo, pref, fls, maxlen) \
    DUF_OPTION_RESTORET( ocode, ptr, b, FLAG_ ## up, lo, ., pref fls.flag, maxlen)
#  define DUF_OPTION_RESTORE_FLAG(ocode, ptr, up, lo, pref, maxlen) \
    DUF_OPTION_RESTORE_FLAGG(ocode, ptr, up, lo, pref, .v, maxlen)
#  define DUF_OPTION_RESTORE_FLAGP(ocode, ptr, up, lo, pref, maxlen) \
    DUF_OPTION_RESTORE_FLAGG(ocode, ptr, up, lo, pref, ->v, maxlen)

#  define DUF_OPTION_RESTORE_NUM( ocode, ptr, up, lo, pref, maxlen) \
    DUF_OPTION_RESTORET( ocode, ptr, i, up, lo, ., pref, maxlen)
#  define DUF_OPTION_RESTORE_NUMP( ocode, ptr, up, lo, pref, maxlen) \
    DUF_OPTION_RESTORET( ocode, ptr, i, up, lo, ->, pref, maxlen)

#  define DUF_OPTION_RESTORE_TRACE(ocode, ptr, up, lo, maxlen) \
  DUF_OPTION_RESTORE_NUM(ocode, ptr,  up ## _TRACE, lo, cli.trace, maxlen)

#  define DUF_OPTION_RESTORE_STR( ocode, ptr, up, _lo, _pref, maxlen) \
    DUF_OPTION_RESTORET( ocode, ptr, s, up, _lo, ., _pref, maxlen)
#  define DUF_OPTION_RESTORE_STRP( ocode, ptr, up, _lo, _pref, maxlen) \
    DUF_OPTION_RESTORET( ocode, ptr, s, up, _lo, ->, _pref, maxlen)



#endif
