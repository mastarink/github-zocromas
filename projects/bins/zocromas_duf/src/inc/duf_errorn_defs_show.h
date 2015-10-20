#ifndef MAS_DUF_ERRORN_DEFS_SHOW_H
#  define MAS_DUF_ERRORN_DEFS_SHOW_H





#  define DUF_SHOW_ERRORO_WP( _prefix, ... )		DUF_TRACE_WP( _prefix, errors  /* opt.trace.errors */, 0, __VA_ARGS__ )
#  define DUF_SHOW_ERRORO( ... )				DUF_SHOW_ERRORO_WP( "@@  ERRor", __VA_ARGS__)

#  define DUF_SHOW_ERRORN( _ri, _verb ) 		duf_error_report_i(_ri, 0, MAST_TRACE_FILE, _verb);
#  define DUF_SHOW_ERRORN_TEST( _ri, _verb ) 		duf_error_report_i(_ri, 1, MAST_TRACE_FILE, _verb);

/* #  define DUF_SHOW_ERRORiV( v )                         DUF_SHOW_ERRORO( #v ":%d" , v ) */
/* ###################################################################### */

/* error message if arg is not 0 */
/* #  define DUF_TEST_RX(_rval)         if (_rval) DUF_SHOW_ERRORO_TEST( "[%s] (#%d)", _rval<0?duf_error_name_i(_rval):"+", _rval ) */


#  if 0
#    define DUF_SHOW_ERRORO_TEST( _fmt, ... )             DUF_SHOW_ERRORO_WP( "@@ [TEst] ", "@@@@@@@@@"_fmt,  __VA_ARGS__ )
#    define DUF_SHOW_ERRORO_TEST_WP_STD(_rval, _prefix) DUF_SHOW_ERRORO_WP( _prefix, "[%s] (ri:%d) {en:%d} lsz:%ld rep:%u:%u", \
					  (_rval)<0?duf_error_name_i(_rval):"+", _rval, duf_enabled_ereport_n_i(_rval), \
    						duf_error_list_size(), duf_ecount_reported_i(_rval), duf_icount_reported_i(_rval) )
/* DUF_SHOW_ERRORO_TEST_STD : takes duf_error_index_t! */
#    define DUF_SHOW_ERRORO_TEST_STD(_rval)           DUF_SHOW_ERRORO_TEST(          "[%s] (ri:%d) {en:%d} lsz:%ld rep:%u:%u", \
					  (_rval)<0?duf_error_name_i(_rval):"+", _rval, duf_enabled_ereport_n_i(_rval), \
    						duf_error_list_size(), duf_ecount_reported_i(_rval), duf_icount_reported_i(_rval) )
#  else
#    define DUF_SHOW_ERRORN_TEST_STD(_rval, _verb)  duf_error_report_i(_rval, 1, MAST_TRACE_FILE, _verb)
#  endif



#endif

/*
vi: ft=c
*/
