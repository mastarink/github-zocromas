#ifndef MAS_DUF_CONFIG_TRACE_ENUM_H
# define MAS_DUF_CONFIG_TRACE_ENUM_H

typedef enum
{
# define ENUM_WRAPP(_n, _rf, _rf2)    DUF_TRACE_LEVEL_ ## _rf2,
# include "duf_optimpl_enum_trace.def"
# undef ENUM_WRAPP
} duf_config_trace_enum_t;

#endif

/*
vi: ft=c
*/
