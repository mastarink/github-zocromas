#ifndef MAS_DUF_LEVINFO_REF_DEF_H
# define MAS_DUF_LEVINFO_REF_DEF_H

# define DUF_LEVINFO_F_PLUS(_typ, _name, _ref1, _ref2) _typ duf_levinfo_ ## _name(  duf_depthinfo_t * pdi ) \
	{ \
	  _typ _ref2 result = (0); \
	  assert( pdi ); \
	  /* if ( duf_pdi_opendir(pdi) ) */ \
	  result = duf_levinfo_ ## _name ## _d( pdi, pdi->pathinfo.depth ); \
	  return result; \
	}

# define DUF_LEVINFO_F_UP_PLUS(_typ, _name, _ref1, _ref2) \
  	_typ _ref2 \
	duf_levinfo_ ## _name ## _up(  duf_depthinfo_t * pdi ) \
	{ \
	  assert( pdi ); \
	  return pdi->pathinfo.depth > 0 ? duf_levinfo_ ## _name ## _d( pdi, pdi->pathinfo.depth - 1 ) : 0; \
	}

# define DUF_LEVINFO_F(_typ, _name) DUF_LEVINFO_F_PLUS(_typ, _name, , )
# define DUF_LEVINFO_F_UP(_typ, _name) DUF_LEVINFO_F_UP_PLUS(_typ, _name, , )

/*********************** in FC - 'C' means const ***************************************/

# define DUF_LEVINFO_FC_D_PLUS(_typ, _name, _fld, _cond, _ref1, _ref2, _suffix) \
  _typ _ref2 \
  duf_levinfo_ ## _name ## _d( const duf_depthinfo_t * pdi, int d ) \
  { \
    duf_levinfo_t *pli; \
    pli=duf_levinfo_ptr_d( pdi, d ); \
    _typ  _ref2 __val; \
    __val=(pli && (_cond)?(_ref1 pli->_fld):((_typ _ref2) 0)); \
    return __val _suffix; \
  }

# define DUF_LEVINFO_FC_PLUS(_typ, _name, _ref1, _ref2) \
  	_typ _ref2 \
	duf_levinfo_ ## _name( const duf_depthinfo_t * pdi ) \
	{ \
	  _typ _ref2 result = (0); \
	  assert( pdi ); \
	  /* if ( duf_pdi_opendir(pdi) ) */ \
	  result = duf_levinfo_ ## _name ## _d( pdi, pdi->pathinfo.depth ); \
	  return result; \
	}

# define DUF_LEVINFO_FC(_typ, _name) DUF_LEVINFO_FC_PLUS(_typ, _name, , )
# define DUF_LEVINFO_FC_UP(_typ, _name) DUF_LEVINFO_FC_UP_PLUS(_typ, _name, , )
# define DUF_LEVINFO_FC_TOP(_typ, _name) DUF_LEVINFO_FC_TOP_PLUS(_typ, _name, , )

# define DUF_LEVINFO_FC_REF(_typ, _name) DUF_LEVINFO_FC_PLUS(_typ, _name, &, *)
# define DUF_LEVINFO_FC_UP_REF(_typ, _name) DUF_LEVINFO_FC_UP_PLUS(_typ, _name, &, *)
# define DUF_LEVINFO_FC_TOP_REF(_typ, _name) DUF_LEVINFO_FC_TOP_PLUS(_typ, _name, &, *)

# define DUF_LEVINFO_FC_UP_PLUS(_typ, _name, _ref1, _ref2) \
  	_typ _ref2 duf_levinfo_ ## _name ## _up( const duf_depthinfo_t * pdi ) \
	{ \
	  assert( pdi ); \
	  return pdi->pathinfo.depth > 0 ? duf_levinfo_ ## _name ## _d( pdi, pdi->pathinfo.depth - 1 ) : 0; \
	}

# define DUF_LEVINFO_FC_TOP_PLUS(_typ, _name, _ref1, _ref2) \
  	_typ _ref2 duf_levinfo_ ## _name ## _top( const duf_depthinfo_t * pdi ) \
	{ \
	  assert( pdi ); \
	  return pdi->pathinfo.depth > 0 ? duf_levinfo_ ## _name ## _d( pdi, pdi->pathinfo.topdepth ) : 0; \
	}

# define DUF_LEVINFO_FS_PLUS(_typ, _name, _ref1, _ref2) \
  	void duf_levinfo_set_ ## _name( duf_depthinfo_t * pdi, _typ setarg ) \
	{ \
	  assert( pdi ); \
	  /* if ( duf_pdi_opendir(pdi) ) */ \
	  duf_levinfo_set_ ## _name ## _d( pdi, setarg, pdi->pathinfo.depth ); \
	}

# define DUF_LEVINFO_FS_UP_PLUS(_typ, _name, _ref1, _ref2) \
  	void duf_levinfo_set_ ## _name ## _up( duf_depthinfo_t * pdi, _typ setarg ) \
	{ \
	  assert( pdi ); \
	  if ( pdi->pathinfo.depth > 0 ) \
	    duf_levinfo_set_ ## _name ## _d( pdi, setarg, pdi->pathinfo.depth - 1 ); \
	}

# define DUF_LEVINFO_FS(_typ, _name) DUF_LEVINFO_FS_PLUS(_typ, _name, , )
# define DUF_LEVINFO_FS_UP(_typ, _name) DUF_LEVINFO_FS_UP_PLUS(_typ, _name, , )

# define DUF_LEVINFO_FC_REF(_typ, _name) DUF_LEVINFO_FC_PLUS(_typ, _name, &, *)
# define DUF_LEVINFO_FC_UP_REF(_typ, _name) DUF_LEVINFO_FC_UP_PLUS(_typ, _name, &, *)

# define DUF_LEVINFO_3GET_PLUS_OLD(_typ, _name, _fld, _cond, _ref1, _ref2, _suffix) \
  _typ _ref2 \
  duf_levinfo_ ## _name ## _d( const duf_depthinfo_t * pdi, int d ) \
  { \
    _typ  _ref2 __val; \
    duf_levinfo_t *pli; \
    pli=duf_levinfo_ptr_d( pdi, d ); \
    __val=(pli && (_cond)?(_ref1 pli->_fld):((_typ _ref2) 0)); \
    return __val _suffix; \
  } \
   \
  DUF_LEVINFO_FC_PLUS( _typ, _name, _ref1, _ref2 ) \
  DUF_LEVINFO_FC_UP_PLUS( _typ, _name, _ref1, _ref2 )

# define DUF_LEVINFO_3GET_PLUS(_typ, _name, _fld, _cond, _ref1, _ref2, _suffix) \
  DUF_LEVINFO_FC_D_PLUS(  _typ, _name, _fld, _cond, _ref1, _ref2, _suffix ) \
  DUF_LEVINFO_FC_PLUS(    _typ, _name,              _ref1, _ref2 ) \
  DUF_LEVINFO_FC_UP_PLUS( _typ, _name,              _ref1, _ref2 )

# define DUF_LEVINFO_4GET_PLUS(_typ, _name, _fld, _cond, _ref1, _ref2, _suffix) \
  DUF_LEVINFO_3GET_PLUS( _typ, _name, _fld, _cond, _ref1, _ref2, _suffix ) \
  DUF_LEVINFO_FC_TOP_PLUS( _typ, _name, _ref1, _ref2 )

# define DUF_LEVINFO_3GET(_typ, _name, _fld) DUF_LEVINFO_3GET_PLUS(_typ, _name, _fld, 1, , , )
# define DUF_LEVINFO_3GET_REF(_typ, _name, _fld) DUF_LEVINFO_3GET_PLUS(_typ, _name, _fld, 1, &, *, )
# define DUF_LEVINFO_3GET_BOOL(_typ, _name, _fld) DUF_LEVINFO_3GET_PLUS(_typ, _name, _fld, 1, , , ?1:0)

# define DUF_LEVINFO_4GET(_typ, _name, _fld) DUF_LEVINFO_4GET_PLUS(_typ, _name, _fld, 1, , , )
# define DUF_LEVINFO_4GET_REF(_typ, _name, _fld) DUF_LEVINFO_4GET_PLUS(_typ, _name, _fld, 1, &, *, )
# define DUF_LEVINFO_4GET_BOOL(_typ, _name, _fld) DUF_LEVINFO_4GET_PLUS(_typ, _name, _fld, 1, , , ?1:0)

# define DUF_LEVINFO_3SET_PLUS(_typ, _name, _fld, _ref1, _ref2, _suffix) \
  void \
  duf_levinfo_set_ ## _name ## _d( duf_depthinfo_t * pdi, _typ __newval, int d ) \
  { \
    duf_levinfo_t *pli; \
    pli=duf_levinfo_ptr_d( pdi, d ); \
    if (pli) pli->_fld = __newval _suffix; \
  } \
   \
  DUF_LEVINFO_FS_PLUS( _typ, _name, _ref1, _ref2 ) \
  DUF_LEVINFO_FS_UP_PLUS( _typ, _name, _ref1, _ref2 )

# define DUF_LEVINFO_3SET(_typ, _name, _fld) DUF_LEVINFO_3SET_PLUS(_typ, _name, _fld, , , )
# define DUF_LEVINFO_3SET_BOOL(_typ, _name, _fld) DUF_LEVINFO_3SET_PLUS(_typ, _name, _fld, , , ?1:0)

# define DUF_LEVINFO_ST_TYP_FLD_NAME(_typ, _fld, _name) \
  _typ ## _t \
  duf_levinfo_stat_ ## _name ## _d( const duf_depthinfo_t * pdi, int d ) \
  { \
   struct stat *st; \
  \
   st = duf_levinfo_stat_d( pdi, d  ); \
   return st ? st->st_ ## _fld : 0; \
 } \
 DUF_LEVINFO_FC( _typ ## _t, stat_ ## _name ) DUF_LEVINFO_FC_UP( _typ ## _t, stat_ ## _name )

# define DUF_LEVINFO_ST_FLD_NAME(_fld, _name) DUF_LEVINFO_ST_TYP_FLD_NAME(_fld, _fld, _name)
# define DUF_LEVINFO_ST_TYP_FLD(_typ, _fld) DUF_LEVINFO_ST_TYP_FLD_NAME(_typ, _fld, _fld)

# define DUF_LEVINFO_ST_FLD(_fld) DUF_LEVINFO_ST_FLD_NAME(_fld, _fld)

# define DUF_LEVINFO_DBST_TYP_FLD_NAME(_typ, _fld, _name) \
  _typ ## _t \
  duf_levinfo_dbstat_ ## _name( const duf_depthinfo_t * pdi ) \
  { \
   struct stat *st; \
  \
   st = duf_levinfo_dbstat( pdi ); \
   return st ? st->st_ ## _fld : 0; \
 }

# define DUF_LEVINFO_DBST_FLD_NAME(_fld, _name) DUF_LEVINFO_DBST_TYP_FLD_NAME(_fld, _fld, _name)
# define DUF_LEVINFO_DBST_TYP_FLD(_typ, _fld) DUF_LEVINFO_DBST_TYP_FLD_NAME(_typ, _fld, _fld)

# define DUF_LEVINFO_DBST_FLD(_fld) DUF_LEVINFO_DBST_FLD_NAME(_fld, _fld)

#endif
