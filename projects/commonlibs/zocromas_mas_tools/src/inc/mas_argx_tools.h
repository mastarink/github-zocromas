#ifndef MAS_ARGX_TOOLS_H
#  define MAS_ARGX_TOOLS_H

void mas_add_argx_arg( mas_string_setv_t*ptargx, const char *arg );
void mas_add_argx_arg_nodup( mas_string_setv_t * ptargx, char *arg );
void mas_del_argx( mas_string_setv_t * argx, int from_a );

#endif
