#ifndef MAS_MUC_OPTIONS_DEF_H
# define MAS_MUC_OPTIONS_DEF_H

# define MUC_OPTSRCI( _cli, _xr, _name, _istage, _cb_do_interactive, _cb_prompt_interactive, _paod, _sourcecode )  \
  { \
    MAST_TRACE( options, 10, "@@@to do %s options; stage:%s(%d)", #_name, muc_optstage_name( cli,_istage ), _istage ); \
    if ( QNOERR ) \
    { \
      muc_CR( source_ ## _name ## _parse, _cli, _istage, _cb_do_interactive, _cb_prompt_interactive, _paod,  MUC_OPTION_SOURCE_ ## _sourcecode ); \
      _xr = QERRIND; \
    } \
    MAST_TRACE( options, 10, "@@@@@done %s options; %s:%d [%c]  (%d:%s)", \
	#_name, #_xr, QERRIND, QERRIND > ' ' && QERRIND < 'z' ? QERRIND : '-', QERRIND, QERRNAME ); \
  }
# define MUC_OPTSRC( _cli, _xr, _name, _istage, _paod, _sourcecode )  \
  MUC_OPTSRCI( _cli, _xr, _name, _istage, NULL, NULL, _paod, _sourcecode )

#endif
