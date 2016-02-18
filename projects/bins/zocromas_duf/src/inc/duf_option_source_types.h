#ifndef MAS_DUF_OPTION_SOURCE_TYPES_H
# define MAS_DUF_OPTION_SOURCE_TYPES_H

typedef enum
{
  DUF_OPTION_SOURCE_NONE = 0,
  DUF_OPTION_SOURCE_MIN,
  DUF_OPTION_SOURCE_ENV = DUF_OPTION_SOURCE_MIN,
  DUF_OPTION_SOURCE_STDIN,
  DUF_OPTION_SOURCE_STREAM,
  DUF_OPTION_SOURCE_DUFFILE,
  DUF_OPTION_SOURCE_CFG,
  DUF_OPTION_SOURCE_CFGLAST,
  DUF_OPTION_SOURCE_CFGSTG,
  DUF_OPTION_SOURCE_CLI,
  DUF_OPTION_SOURCE_INTERACTIVE,
  DUF_OPTION_SOURCE_INTERAC = DUF_OPTION_SOURCE_INTERACTIVE,
  DUF_OPTION_SOURCE_IA = DUF_OPTION_SOURCE_INTERACTIVE,
  DUF_OPTION_SOURCE_MAX = DUF_OPTION_SOURCE_INTERAC,
} duf_option_source_code_t;
typedef struct
{
  duf_option_source_code_t sourcecode;
  int labelled;
  const char *label;
} duf_option_source_t;

# define  DUF_OPTION_SOURCE_LABELLED(_code, _label) ((duf_option_source_t){.sourcecode = DUF_OPTION_SOURCE_ ## _code, .label=_label, .labelled=1 })
# define  DUF_OPTION_SOURCE(_code ) ((duf_option_source_t){.sourcecode = DUF_OPTION_SOURCE_ ## _code })
# define DUF_OPTION_SOURCE_DEFAULT(_source, _code) ((duf_option_source_t){ \
    		.sourcecode = (_source.sourcecode==DUF_OPTION_SOURCE_NONE?DUF_OPTION_SOURCE_ ## _code:_source.sourcecode), \
    		.label=_source.label, \
    		.labelled=_source.labelled \
    	})
# define DUF_IS_SOURCE(_source, _code) (_source.sourcecode==DUF_OPTION_SOURCE_ ## _code)

#endif
