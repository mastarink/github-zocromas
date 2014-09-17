#ifndef DUF_LEVINFO_REF_DEF_H
#  define DUF_LEVINFO_REF_DEF_H


#define DUF_LEVINFO_F_PLUS(_typ, _name, _ref1, _ref2) _typ duf_levinfo_ ## _name(  duf_depthinfo_t * pdi ) \
	{ \
	  _typ _ref2 result = (0); \
	  assert( pdi ); \
	  /* if ( pdi->opendir ) */ \
	  result = duf_levinfo_ ## _name ## _d( pdi, pdi->depth ); \
	  return result; \
	}

#define DUF_LEVINFO_F_UP_PLUS(_typ, _name, _ref1, _ref2) \
  	_typ _ref2 \
	duf_levinfo_ ## _name ## _up(  duf_depthinfo_t * pdi ) \
	{ \
	  assert( pdi ); \
	  return pdi->depth > 0 ? duf_levinfo_ ## _name ## _d( pdi, pdi->depth - 1 ) : 0; \
	}

#define DUF_LEVINFO_F(_typ, _name) DUF_LEVINFO_F_PLUS(_typ, _name, , )
#define DUF_LEVINFO_F_UP(_typ, _name) DUF_LEVINFO_F_UP_PLUS(_typ, _name, , )


#define DUF_LEVINFO_FC_PLUS(_typ, _name, _ref1, _ref2) \
  	_typ _ref2 \
	duf_levinfo_ ## _name( const duf_depthinfo_t * pdi ) \
	{ \
	  _typ _ref2 result = (0); \
	  assert( pdi ); \
	  /* if ( pdi->opendir ) */ \
	  result = duf_levinfo_ ## _name ## _d( pdi, pdi->depth ); \
	  return result; \
	}
#define DUF_LEVINFO_FC(_typ, _name) DUF_LEVINFO_FC_PLUS(_typ, _name, , )
#define DUF_LEVINFO_FC_UP(_typ, _name) DUF_LEVINFO_FC_UP_PLUS(_typ, _name, , )

#define DUF_LEVINFO_FC_REF(_typ, _name) DUF_LEVINFO_FC_PLUS(_typ, _name, &, *)
#define DUF_LEVINFO_FC_UP_REF(_typ, _name) DUF_LEVINFO_FC_UP_PLUS(_typ, _name, &, *)

#define DUF_LEVINFO_FC_UP_PLUS(_typ, _name, _ref1, _ref2) \
  	_typ _ref2 duf_levinfo_ ## _name ## _up( const duf_depthinfo_t * pdi ) \
	{ \
	  assert( pdi ); \
	  return pdi->depth > 0 ? duf_levinfo_ ## _name ## _d( pdi, pdi->depth - 1 ) : 0; \
	}


#define DUF_LEVINFO_FS_PLUS(_typ, _name, _ref1, _ref2) \
  	void duf_levinfo_set_ ## _name( duf_depthinfo_t * pdi, _typ setarg ) \
	{ \
	  assert( pdi ); \
	  /* if ( pdi->opendir ) */ \
	  duf_levinfo_set_ ## _name ## _d( pdi, setarg, pdi->depth ); \
	}

#define DUF_LEVINFO_FS_UP_PLUS(_typ, _name, _ref1, _ref2) \
  	void duf_levinfo_set_ ## _name ## _up( duf_depthinfo_t * pdi, _typ setarg ) \
	{ \
	  assert( pdi ); \
	  if ( pdi->depth > 0 ) \
	    duf_levinfo_set_ ## _name ## _d( pdi, setarg, pdi->depth - 1 ); \
	}

#define DUF_LEVINFO_FS(_typ, _name) DUF_LEVINFO_FS_PLUS(_typ, _name, , )
#define DUF_LEVINFO_FS_UP(_typ, _name) DUF_LEVINFO_FS_UP_PLUS(_typ, _name, , )

#define DUF_LEVINFO_FC_REF(_typ, _name) DUF_LEVINFO_FC_PLUS(_typ, _name, &, *)
#define DUF_LEVINFO_FC_UP_REF(_typ, _name) DUF_LEVINFO_FC_UP_PLUS(_typ, _name, &, *)


#define DUF_LEVINFO_3GET_PLUS(_typ, _name, _fld, _cond, _ref1, _ref2, _suffix) \
  _typ _ref2 \
  duf_levinfo_ ## _name ## _d( const duf_depthinfo_t * pdi, int d ) \
  { \
    _typ  _ref2 __val; \
    __val=((_cond)?(_ref1 duf_levinfo_ptr_d( pdi, d )->_fld):((_typ _ref2) 0)); \
    return __val _suffix; \
  } \
   \
  DUF_LEVINFO_FC_PLUS( _typ, _name, _ref1, _ref2 ) \
  DUF_LEVINFO_FC_UP_PLUS( _typ, _name, _ref1, _ref2 )

#define DUF_LEVINFO_3GET(_typ, _name, _fld) DUF_LEVINFO_3GET_PLUS(_typ, _name, _fld, 1, , , )
#define DUF_LEVINFO_3GET_REF(_typ, _name, _fld) DUF_LEVINFO_3GET_PLUS(_typ, _name, _fld, 1, &, *, )
#define DUF_LEVINFO_3GET_BOOL(_typ, _name, _fld) DUF_LEVINFO_3GET_PLUS(_typ, _name, _fld, 1, , , ?1:0)

#define DUF_LEVINFO_3SET_PLUS(_typ, _name, _fld, _ref1, _ref2, _suffix) \
  void \
  duf_levinfo_set_ ## _name ## _d( duf_depthinfo_t * pdi, _typ __newval, int d ) \
  { \
    duf_levinfo_ptr_d( pdi, d )->_fld = __newval _suffix; \
  } \
   \
  DUF_LEVINFO_FS_PLUS( _typ, _name, _ref1, _ref2 ) \
  DUF_LEVINFO_FS_UP_PLUS( _typ, _name, _ref1, _ref2 )

#define DUF_LEVINFO_3SET(_typ, _name, _fld) DUF_LEVINFO_3SET_PLUS(_typ, _name, _fld, , , )
#define DUF_LEVINFO_3SET_BOOL(_typ, _name, _fld) DUF_LEVINFO_3SET_PLUS(_typ, _name, _fld, , , ?1:0)





#endif
