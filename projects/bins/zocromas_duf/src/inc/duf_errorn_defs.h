#ifndef MAS_DUF_ERRORN_DEFS_H
# define MAS_DUF_ERRORN_DEFS_H
# include <mastar/error/mas_error_defs_ctrl.h>
# include <mastar/error/mas_error_defs.h>                            /* MASE_TEST_R; MASE_TEST_R_LOWERE; ... */
# include <mastar/error/mas_error_defs_make.h>                       /* MASE_MAKE_ERROR; MASE_MAKE_ERRORFL; MASE_MAKE_ERRORM  ... */

#include "duf_errorn_defs_preset.h"
#include "duf_tracen_defs_preset.h"


# define MASE_STD_VERBOSE_LEVEL mas_verbose?mas_verbose():0 /* TODO 20160226.163713: => duf_errorn_defs_preset.h*/

# define DUF_CLEARED_ERROR(...) MASE_CLEARED_ERROR( __VA_ARGS__ )
# define DUF_CLEAR_ERROR(...) MASE_CLEAR_ERROR( __VA_ARGS__ )
# define DUF_E_LOWER(...) MASE_E_LOWER( __VA_ARGS__ )
# define DUF_E_LOWER_N(...) MASE_E_LOWER_N( __VA_ARGS__ )
# define DUF_E_MAX(...) MASE_E_MAX( __VA_ARGS__ )
# define DUF_ERRSYSE(...) MASE_ERRSYSE( __VA_ARGS__ )
# define DUF_ERRSYS(...) MASE_ERRSYS( __VA_ARGS__ )
# define DUF_E_UPPER(...) MASE_E_UPPER( __VA_ARGS__ )
# define DUF_E_UPPER_N(...) MASE_E_UPPER_N( __VA_ARGS__ )
# define DUF_IS_ERROR(...) MASE_IS_ERROR( __VA_ARGS__ )
# define DUF_IS_ERROR_N(...) MASE_IS_ERROR_N( __VA_ARGS__ )
# define DUF_MAKE_ERRORFL(...) MASE_MAKE_ERRORFL( __VA_ARGS__ )
# define DUF_MAKE_ERROR(...) MASE_MAKE_ERROR( __VA_ARGS__ )
# define DUF_MAKE_ERRORM(...) MASE_MAKE_ERRORM( __VA_ARGS__ )
# define DUF_NOERROR(_r) MASE_NOERROR(_r)
# define DUF_SHOW_ERRORO(...) MASE_SHOW_ERRORO( __VA_ARGS__ )
# define DUF_TEST_R_LOWERE(...) MASE_TEST_R_LOWERE( __VA_ARGS__ )
# define DUF_TEST_R(_r) MASE_TEST_R(_r)

#endif
