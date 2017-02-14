#ifndef MULCONF_TYPES_H
# define MULCONF_TYPES_H

# include <mastar/tools/mas_argvc_tools.h>

typedef union nvalue_u nvalue_t;
typedef enum config_source_e config_source_t;

typedef enum config_source_mode_e config_source_mode_t;

typedef struct config_source_list_s config_source_list_t;
typedef struct config_source_desc_s config_source_desc_t;
typedef struct config_prefix_encoder_s config_prefix_encoder_t;
typedef int ( *source_check_fun_t ) ( int count, const void *data_ptr, const char *delims, const char *eq, const config_prefix_encoder_t * pref_ids );
typedef int ( *source_open_fun_t ) ( int count, const void *data_ptr, const char *delims, const char *eq, const config_prefix_encoder_t * pref_ids );
typedef int ( *source_close_fun_t ) ( int count, const void *data_ptr, const char *delims, const char *eq, const config_prefix_encoder_t * pref_ids );
typedef char *( *source_load_string_fun_t ) ( config_source_desc_t * descr );
typedef mas_argvc_t( *source_load_targ_fun_t ) ( config_source_desc_t * descr, mas_argvc_t targ );

typedef enum config_variant_e config_variant_t;
typedef enum config_restype_e config_restype_t;
typedef enum config_bitwise_e config_bitwise_t;
typedef struct config_option_s config_option_t;
typedef int ( *option_callback_t ) ( config_option_t *opt );
typedef struct config_option_table_list_s config_option_table_list_t;
typedef struct config_error_s config_error_t;

#endif
