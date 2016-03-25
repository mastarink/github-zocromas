#ifndef MAS_DUF_CONFIG_DEFS_H
# define MAS_DUF_CONFIG_DEFS_H

# include <mastar/tools/mas_expandable_types.h>
# include "duf_config_structs.h"                                     /* bad here 20160323.173534 */

# define DUF_CONFIG_DIR_FROM_ENV "MSHDIRS_CONF_DIR"

# define DUF_CFGX(_cfg, _lo)  _cfg->_lo
# ifdef MAS_TRACING
#  define DUF_CFGG(_cfg, _lo)          (MAST_TRACE(flags, 0, "FLAG %s", # _lo), DUF_CFGX(_cfg, _lo))
# else
#  define DUF_CFGG(_cfg, _lo)          DUF_CFGX(_cfg, _lo)
# endif
# define DUF_CFGG_F(_cfg, _lo)        DUF_CFGG(_cfg, _lo)
# define DUF_CFGG_N(_cfg, _lo)        DUF_CFGG(_cfg, _lo)
# define DUF_CFGG_A(_cfg, _lo, _a)    DUF_CFGG(_cfg, _lo)._a

# define DUF_CFGA(_cfg, _lo)          &DUF_CFGX(_cfg, _lo)
# define DUF_CFGA_F(_cfg, _lo)        DUF_CFGA(_cfg, _lo)
# define DUF_CFGA_N(_cfg, _lo)        DUF_CFGA(_cfg, _lo)
# define DUF_CFGA_A(_cfg, _lo, _a)    DUF_CFGA(_cfg, _lo)._a

# define DUF_CFGW(_cfg, _lo)          DUF_CFGX(_cfg, _lo)
# define DUF_CFGW_F(_cfg, _lo)        DUF_CFGW(_cfg, _lo)
# define DUF_CFGW_N(_cfg, _lo)        DUF_CFGW(_cfg, _lo)
# define DUF_CFGW_A(_cfg, _lo, _a)    DUF_CFGW(_cfg, _lo)._a

# define DUF_CFGWN(_cfg, _lo, _val) DUF_CFGW(_cfg, _lo)=(_val)
# define DUF_CFGWS(_cfg, _lo, _val) DUF_CFGW(_cfg, _lo)=(_val)
# define DUF_CFGWP(_cfg, _lo, _val) DUF_CFGW(_cfg, _lo)=(_val)
# define DUF_CFGWSP(_cfg, _lo, _val) DUF_CFGW(_cfg, _lo.value)=(_val)

# define DUF_CFGGS(_cfg, _lo) DUF_CFGG(_cfg, _lo)
# define DUF_CFGGSP(_cfg, _lo) mas_string_expanded(&_cfg->_lo)

# define DUF_CONFIGX(_lo)  DUF_CFGX(duf_get_config(), _lo)
# ifdef MAS_TRACING
#  define DUF_CONFIGG(_lo)       (MAST_TRACE(flags, 0, "FLAG %s", # _lo), DUF_CONFIGX(_lo))
# else
#  define DUF_CONFIGG(_lo)        DUF_CONFIGX(_lo)
# endif
# define DUF_CONFIGG_F(_lo)        DUF_CONFIGG(_lo)
# define DUF_CONFIGG_N(_lo)        DUF_CONFIGG(_lo)
# define DUF_CONFIGG_A(_lo, _a)    DUF_CONFIGG(_lo)._a

# define DUF_CONFIGA(_lo)          &DUF_CONFIGX(_lo)
# define DUF_CONFIGA_F(_lo)        DUF_CONFIGA(_lo)
# define DUF_CONFIGA_N(_lo)        DUF_CONFIGA(_lo)
# define DUF_CONFIGA_A(_lo, _a)    DUF_CONFIGA(_lo)._a

# define DUF_CONFIGW(_lo)          DUF_CONFIGX(_lo)
# define DUF_CONFIGW_F(_lo)        DUF_CONFIGW(_lo)
# define DUF_CONFIGW_N(_lo)        DUF_CONFIGW(_lo)
# define DUF_CONFIGW_A(_lo, _a)    DUF_CONFIGW(_lo)._a

# define DUF_CONFIGWN(_lo, _val)   DUF_CONFIGW(_lo)=(_val)
# define DUF_CONFIGWS(_lo, _val)   DUF_CONFIGW(_lo)=(_val)
# define DUF_CONFIGWP(_lo, _val)   DUF_CONFIGW(_lo)=(_val)
# define DUF_CONFIGWSP(_lo, _val)  DUF_CFGWSP(_lo)=(_val)

# define DUF_CONFIGGS(_lo)         DUF_CONFIGG(_lo)
# define DUF_CONFIGGSP(_lo)        DUF_CFGGSP(duf_get_config(), _lo)

#endif
