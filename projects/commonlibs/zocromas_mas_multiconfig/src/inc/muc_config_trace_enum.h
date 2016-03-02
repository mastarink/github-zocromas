#ifndef MAS_MUC_CONFIG_TRACE_ENUM_H
# define MAS_MUC_CONFIG_TRACE_ENUM_H

typedef enum
{
# define ENUM_WRAPP(_n, _rf, _rf2)    MUC_TRACE_LEVEL_ ## _rf2,
# include "muc_optimpl_enum_trace.def"
# undef ENUM_WRAPP
} muc_config_trace_enum_t;

#endif

/*
vi: ft=c
*/
