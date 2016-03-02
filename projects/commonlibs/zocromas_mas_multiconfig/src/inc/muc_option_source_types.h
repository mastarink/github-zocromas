#ifndef MAS_MUC_OPTION_SOURCE_TYPES_H
# define MAS_MUC_OPTION_SOURCE_TYPES_H

typedef enum
{
  MUC_OPTION_SOURCE_NONE = 0,
  MUC_OPTION_SOURCE_MIN,
  MUC_OPTION_SOURCE_ENV = MUC_OPTION_SOURCE_MIN,
  MUC_OPTION_SOURCE_STDIN,
  MUC_OPTION_SOURCE_STREAM,
  MUC_OPTION_SOURCE_DUFFILE,
  MUC_OPTION_SOURCE_CFG,
  MUC_OPTION_SOURCE_CFGLAST,
  MUC_OPTION_SOURCE_CFGSTG,
  MUC_OPTION_SOURCE_CLI,
  MUC_OPTION_SOURCE_INTERACTIVE,
  MUC_OPTION_SOURCE_INTERAC = MUC_OPTION_SOURCE_INTERACTIVE,
  MUC_OPTION_SOURCE_IA = MUC_OPTION_SOURCE_INTERACTIVE,
  MUC_OPTION_SOURCE_MAX = MUC_OPTION_SOURCE_INTERAC,
} muc_option_source_code_t;
typedef struct
{
  muc_option_source_code_t sourcecode;
  int labelled;
  const char *label;
} muc_option_source_t;

# define  MUC_OPTION_SOURCE_LABELLED(_code, _label) ((muc_option_source_t){.sourcecode = MUC_OPTION_SOURCE_ ## _code, .label=_label, .labelled=1 })
# define  MUC_OPTION_SOURCE(_code ) ((muc_option_source_t){.sourcecode = MUC_OPTION_SOURCE_ ## _code })
# define MUC_OPTION_SOURCE_DEFAULT(_source, _code) ((muc_option_source_t){ \
    		.sourcecode = (_source.sourcecode==MUC_OPTION_SOURCE_NONE?MUC_OPTION_SOURCE_ ## _code:_source.sourcecode), \
    		.label=_source.label, \
    		.labelled=_source.labelled \
    	})
# define MUC_IS_SOURCE(_source, _code) (_source.sourcecode==MUC_OPTION_SOURCE_ ## _code)

#endif
