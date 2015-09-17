#ifndef DUF_PATHINFO_REF_DEF_H
#  define DUF_PATHINFO_REF_DEF_H


#  define DUF_PATHINFO_F_PLUS(_typ, _name, _ref1, _ref2) _typ duf_pi_ ## _name(  duf_pathinfo_t * pi ) \
	{ \
	  _typ _ref2 result = (0); \
	  assert( pi ); \
	  result = duf_pi_ ## _name ## _d( pi, pi->depth ); \
	  return result; \
	}

#  define DUF_PATHINFO_F_UP_PLUS(_typ, _name, _ref1, _ref2) \
  	_typ _ref2 \
	duf_pi_ ## _name ## _up(  duf_pathinfo_t * pi ) \
	{ \
	  assert( pi ); \
	  return pi->depth > 0 ? duf_pi_ ## _name ## _d( pi, pi->depth - 1 ) : 0; \
	}

#  define DUF_PATHINFO_F(_typ, _name) DUF_PATHINFO_F_PLUS(_typ, _name, , )
#  define DUF_PATHINFO_F_UP(_typ, _name) DUF_PATHINFO_F_UP_PLUS(_typ, _name, , )

/*********************** in FC - 'C' means const ***************************************/

#  define DUF_PATHINFO_FC_D_PLUS(_typ, _name, _fld, _cond, _ref1, _ref2, _suffix) \
  _typ _ref2 \
  duf_pi_ ## _name ## _d( const duf_pathinfo_t * pi, int d ) \
  { \
    _typ  _ref2 __val; \
    __val=((_cond)?(_ref1 duf_pi_ptr_d( pi, d )->_fld):((_typ _ref2) 0)); \
    return __val _suffix; \
  }



#  define DUF_PATHINFO_FC_PLUS(_typ, _name, _ref1, _ref2) \
  	_typ _ref2 \
	duf_pi_ ## _name( const duf_pathinfo_t * pi ) \
	{ \
	  _typ _ref2 result = (0); \
	  assert( pi ); \
	  result = duf_pi_ ## _name ## _d( pi, pi->depth ); \
	  return result; \
	}

#  define DUF_PATHINFO_FC(_typ, _name) DUF_PATHINFO_FC_PLUS(_typ, _name, , )
#  define DUF_PATHINFO_FC_UP(_typ, _name) DUF_PATHINFO_FC_UP_PLUS(_typ, _name, , )
#  define DUF_PATHINFO_FC_TOP(_typ, _name) DUF_PATHINFO_FC_TOP_PLUS(_typ, _name, , )

#  define DUF_PATHINFO_FC_REF(_typ, _name) DUF_PATHINFO_FC_PLUS(_typ, _name, &, *)
#  define DUF_PATHINFO_FC_UP_REF(_typ, _name) DUF_PATHINFO_FC_UP_PLUS(_typ, _name, &, *)
#  define DUF_PATHINFO_FC_TOP_REF(_typ, _name) DUF_PATHINFO_FC_TOP_PLUS(_typ, _name, &, *)

#  define DUF_PATHINFO_FC_UP_PLUS(_typ, _name, _ref1, _ref2) \
  	_typ _ref2 duf_pi_ ## _name ## _up( const duf_pathinfo_t * pi ) \
	{ \
	  assert( pi ); \
	  return pi->depth > 0 ? duf_pi_ ## _name ## _d( pi, pi->depth - 1 ) : 0; \
	}

#  define DUF_PATHINFO_FC_TOP_PLUS(_typ, _name, _ref1, _ref2) \
  	_typ _ref2 duf_pi_ ## _name ## _top( const duf_pathinfo_t * pi ) \
	{ \
	  assert( pi ); \
	  return pi->depth > 0 ? duf_pi_ ## _name ## _d( pi, pi->topdepth ) : 0; \
	}

#  define DUF_PATHINFO_FS_PLUS(_typ, _name, _ref1, _ref2) \
  	void duf_pathinfo_set_ ## _name( duf_pathinfo_t * pi, _typ setarg ) \
	{ \
	  assert( pi ); \
	  duf_pathinfo_set_ ## _name ## _d( pi, setarg, pi->depth ); \
	}

#  define DUF_PATHINFO_FS_UP_PLUS(_typ, _name, _ref1, _ref2) \
  	void duf_pathinfo_set_ ## _name ## _up( duf_pathinfo_t * pi, _typ setarg ) \
	{ \
	  assert( pi ); \
	  if ( pi->depth > 0 ) \
	    duf_pathinfo_set_ ## _name ## _d( pi, setarg, pi->depth - 1 ); \
	}

#  define DUF_PATHINFO_FS(_typ, _name) DUF_PATHINFO_FS_PLUS(_typ, _name, , )
#  define DUF_PATHINFO_FS_UP(_typ, _name) DUF_PATHINFO_FS_UP_PLUS(_typ, _name, , )

#  define DUF_PATHINFO_FC_REF(_typ, _name) DUF_PATHINFO_FC_PLUS(_typ, _name, &, *)
#  define DUF_PATHINFO_FC_UP_REF(_typ, _name) DUF_PATHINFO_FC_UP_PLUS(_typ, _name, &, *)


#  define DUF_PATHINFO_3GET_PLUS_OLD(_typ, _name, _fld, _cond, _ref1, _ref2, _suffix) \
  _typ _ref2 \
  duf_pi_ ## _name ## _d( const duf_pathinfo_t * pi, int d ) \
  { \
    _typ  _ref2 __val; \
    __val=((_cond)?(_ref1 duf_pi_ptr_d( pi, d )->_fld):((_typ _ref2) 0)); \
    return __val _suffix; \
  } \
   \
  DUF_PATHINFO_FC_PLUS( _typ, _name, _ref1, _ref2 ) \
  DUF_PATHINFO_FC_UP_PLUS( _typ, _name, _ref1, _ref2 )

#  define DUF_PATHINFO_3GET_PLUS(_typ, _name, _fld, _cond, _ref1, _ref2, _suffix) \
  DUF_PATHINFO_FC_D_PLUS(  _typ, _name, _fld, _cond, _ref1, _ref2, _suffix ) \
  DUF_PATHINFO_FC_PLUS(    _typ, _name,              _ref1, _ref2 ) \
  DUF_PATHINFO_FC_UP_PLUS( _typ, _name,              _ref1, _ref2 )

#  define DUF_PATHINFO_4GET_PLUS(_typ, _name, _fld, _cond, _ref1, _ref2, _suffix) \
  DUF_PATHINFO_3GET_PLUS( _typ, _name, _fld, _cond, _ref1, _ref2, _suffix ) \
  DUF_PATHINFO_FC_TOP_PLUS( _typ, _name, _ref1, _ref2 )


#  define DUF_PATHINFO_3GET(_typ, _name, _fld) DUF_PATHINFO_3GET_PLUS(_typ, _name, _fld, 1, , , )
#  define DUF_PATHINFO_3GET_REF(_typ, _name, _fld) DUF_PATHINFO_3GET_PLUS(_typ, _name, _fld, 1, &, *, )
#  define DUF_PATHINFO_3GET_BOOL(_typ, _name, _fld) DUF_PATHINFO_3GET_PLUS(_typ, _name, _fld, 1, , , ?1:0)

#  define DUF_PATHINFO_4GET(_typ, _name, _fld) DUF_PATHINFO_4GET_PLUS(_typ, _name, _fld, 1, , , )
#  define DUF_PATHINFO_4GET_REF(_typ, _name, _fld) DUF_PATHINFO_4GET_PLUS(_typ, _name, _fld, 1, &, *, )
#  define DUF_PATHINFO_4GET_BOOL(_typ, _name, _fld) DUF_PATHINFO_4GET_PLUS(_typ, _name, _fld, 1, , , ?1:0)

#  define DUF_PATHINFO_3SET_PLUS(_typ, _name, _fld, _ref1, _ref2, _suffix) \
  void \
  duf_pathinfo_set_ ## _name ## _d( duf_pathinfo_t * pi, _typ __newval, int d ) \
  { \
    duf_pi_ptr_d( pi, d )->_fld = __newval _suffix; \
  } \
   \
  DUF_PATHINFO_FS_PLUS( _typ, _name, _ref1, _ref2 ) \
  DUF_PATHINFO_FS_UP_PLUS( _typ, _name, _ref1, _ref2 )

#  define DUF_PATHINFO_3SET(_typ, _name, _fld) DUF_PATHINFO_3SET_PLUS(_typ, _name, _fld, , , )
#  define DUF_PATHINFO_3SET_BOOL(_typ, _name, _fld) DUF_PATHINFO_3SET_PLUS(_typ, _name, _fld, , , ?1:0)





#  define DUF_PATHINFO_ST_TYP_FLD_NAME(_typ, _fld, _name) \
  _typ ## _t \
  duf_pathinfo_stat_ ## _name( const duf_pathinfo_t * pi ) \
  { \
   struct stat *st; \
  \
   st = duf_pi_stat( pi ); \
   return st ? st->st_ ## _fld : 0; \
 }

#  define DUF_PATHINFO_ST_FLD_NAME(_fld, _name) DUF_PATHINFO_ST_TYP_FLD_NAME(_fld, _fld, _name)
#  define DUF_PATHINFO_ST_TYP_FLD(_typ, _fld) DUF_PATHINFO_ST_TYP_FLD_NAME(_typ, _fld, _fld)

#  define DUF_PATHINFO_ST_FLD(_fld) DUF_PATHINFO_ST_FLD_NAME(_fld, _fld)


#  define DUF_PATHINFO_DBST_TYP_FLD_NAME(_typ, _fld, _name) \
  _typ ## _t \
  duf_pathinfo_dbstat_ ## _name( const duf_pathinfo_t * pi ) \
  { \
   struct stat *st; \
  \
   st = duf_pi_dbstat( pi ); \
   return st ? st->st_ ## _fld : 0; \
 }

#  define DUF_PATHINFO_DBST_FLD_NAME(_fld, _name) DUF_PATHINFO_DBST_TYP_FLD_NAME(_fld, _fld, _name)
#  define DUF_PATHINFO_DBST_TYP_FLD(_typ, _fld) DUF_PATHINFO_DBST_TYP_FLD_NAME(_typ, _fld, _fld)

#  define DUF_PATHINFO_DBST_FLD(_fld) DUF_PATHINFO_DBST_FLD_NAME(_fld, _fld)





#endif
