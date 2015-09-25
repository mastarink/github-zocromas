#ifndef MAS_DUF_CONFIG_REF_H
#  define MAS_DUF_CONFIG_REF_H

#  include "duf_config_types.h"
extern duf_config_t *duf_config;

#  define DUF_CONFIGX(_lo)  duf_config->_lo

#  define DUF_CONFIGG(_lo)          (DUF_TRACE(flags, 0, "FLAG %s", # _lo), DUF_CONFIGX(_lo))
#  define DUF_CONFIGG_F(_lo)        DUF_CONFIGG(_lo)
#  define DUF_CONFIGG_N(_lo)        DUF_CONFIGG(_lo)
#  define DUF_CONFIGG_A(_lo, _a)    DUF_CONFIGG(_lo)._a

#  define DUF_CONFIGA(_lo)          &duf_config->_lo
#  define DUF_CONFIGA_F(_lo)        DUF_CONFIGA(_lo)
#  define DUF_CONFIGA_N(_lo)        DUF_CONFIGA(_lo)
#  define DUF_CONFIGA_A(_lo, _a)    DUF_CONFIGA(_lo)._a

#  define DUF_CONFIGW(_lo)          duf_config->_lo
#  define DUF_CONFIGW_F(_lo)        DUF_CONFIGW(_lo)
#  define DUF_CONFIGW_N(_lo)        DUF_CONFIGW(_lo)
#  define DUF_CONFIGW_A(_lo, _a)    DUF_CONFIGW(_lo)._a

#endif

