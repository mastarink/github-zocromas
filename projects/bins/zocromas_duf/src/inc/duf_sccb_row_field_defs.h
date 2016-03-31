#ifndef MAS_DUF_SCCB_ROW_FIELD_DEFS_H
# define MAS_DUF_SCCB_ROW_FIELD_DEFS_H

#include "duf_sql_field_defs.h"
# define DUF_GET_SFIELD2Q(_name, _opt)			DUF_GET_STMT_SFIELD2Q( pstmt, _name, _opt)
# define DUF_GET_UFIELD2Q(_name, _opt)			DUF_GET_STMT_UFIELD2Q( pstmt, _name, _opt)

# define DUF_GET_SFIELD2(_name)			DUF_GET_SFIELD2Q(_name, 0)
# define DUF_GET_UFIELD2(_name)			DUF_GET_UFIELD2Q(_name, 0)

# define DUF_GET_SFIELD2OPT(_name)			DUF_GET_SFIELD2Q(_name, 1)
# define DUF_GET_UFIELD2OPT(_name)			DUF_GET_UFIELD2Q(_name, 1)

# define DUF_SET_SFIELD2(_name)			_name =	DUF_GET_SFIELD2(_name)
# define DUF_SET_UFIELD2(_name)			_name = DUF_GET_UFIELD2(_name)

# define DUF_SFIELD2(_name)				const char*         DUF_SET_SFIELD2(_name)
# define DUF_UFIELD2(_name)				unsigned long long  DUF_SET_UFIELD2(_name)

# define DUF_SET_SFIELD2OPT(_name)			_name = DUF_GET_SFIELD2OPT(_name)
# define DUF_SET_UFIELD2OPT(_name)			_name = DUF_GET_UFIELD2OPT(_name)

# define DUF_SFIELD2OPT(_name)				const char*         DUF_SET_SFIELD2OPT(_name)
# define DUF_UFIELD2OPT(_name)				unsigned long long  DUF_SET_UFIELD2OPT(_name)

#endif

