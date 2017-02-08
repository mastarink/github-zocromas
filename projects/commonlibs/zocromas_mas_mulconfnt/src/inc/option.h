#ifndef MULCONF_OPTION_H
# define MULCONF_OPTION_H

config_option_t *mulconfnt_config_option_create( void );
config_option_t *mulconfnt_config_option_clone( const config_option_t * topt );
void mulconfnt_config_option_set_value( config_option_t * opt, const char *string_value );
void mulconfnt_config_option_delete( config_option_t * opt );

#endif
