#ifndef MAS_DUF_OPTIONS_DEF_H
#  define MAS_DUF_OPTIONS_DEF_H


#define DUF_OPTSRCI( _xr, _name, _istage, _cb_do_interactive, _cb_prompt_interactive, _paod, _sourcecode )  \
  { \
    DUF_TRACE( options, 10, "@@@to do %s options; stage:%s(%d)", #_name, duf_optstage_name( _istage ), _istage ); \
    if ( QNOERR ) \
    { \
      CR( source_ ## _name ## _parse, _istage, _cb_do_interactive, _cb_prompt_interactive, _paod,  DUF_OPTION_SOURCE_ ## _sourcecode ); \
      _xr = QERRIND; \
    } \
    DUF_TRACE( options, 10, "@@@@@done %s options; %s:%d [%c]  (%d:%s)", \
	#_name, #_xr, QERRIND, QERRIND > ' ' && QERRIND < 'z' ? QERRIND : '-', QERRIND, QERRNAME ); \
  }
#define DUF_OPTSRC( _xr, _name, _istage, _paod, _sourcecode )  \
  DUF_OPTSRCI( _xr, _name, _istage, NULL, NULL, _paod, _sourcecode )


#endif
