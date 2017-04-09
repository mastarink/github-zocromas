#ifndef MUCS_TYPES_H
# define MUCS_TYPES_H

# define MULCONFNT_ETC_CONFIG MAS_SYSCONFDIR "/" PACKAGE_NAME ".conf"
# include <mastar/tools/mas_argvc_tools.h>

typedef union nvalue_u nvalue_t;

typedef unsigned long mucs_flags_t;

typedef enum mucs_source_type_e mucs_source_type_t;

typedef enum mucs_source_mode_e mucs_source_mode_t;

typedef struct mucs_source_list_s mucs_source_list_t;
typedef struct mucs_source_extra_cb_s mucs_source_extra_cb_t;
typedef struct mucs_source_s mucs_source_t;
typedef struct mucs_prefix_encoder_s mucs_prefix_encoder_t;
typedef int ( *mucs_source_check_fun_t ) ( int count, const void *data_ptr, const char *delims, const char *eq,
                                           const mucs_prefix_encoder_t * pref_ids );
typedef int ( *mucs_source_open_fun_t ) ( int count, const void *data_ptr, const char *delims, const char *eq,
                                          const mucs_prefix_encoder_t * pref_ids );
typedef int ( *mucs_source_close_fun_t ) ( int count, const void *data_ptr, const char *delims, const char *eq,
                                           const mucs_prefix_encoder_t * pref_ids );
typedef char *( *mucs_source_load_string_fun_t ) ( mucs_source_t * han );
typedef mas_argvc_t( *mucs_source_load_targ_fun_t ) ( mucs_source_t * han, mas_argvc_t targ );

typedef enum mucs_variant_e mucs_variant_t;
typedef enum mucs_restype_e mucs_restype_t;
typedef enum mucs_bitwise_e mucs_bitwise_t;
typedef struct mucs_option_s mucs_option_t;
typedef int ( *mucs_option_callback_t ) ( mucs_option_t * opt, void *userdata );
typedef struct mucs_option_table_list_s mucs_option_table_list_t;

/* typedef struct mucs_error_s mucs_error_t; */

typedef struct mucs_optscanner_s mucs_optscanner_t;
typedef struct mucs_option_interface_s mucs_option_interface_t;

#endif
