#ifndef MAS_VARVEC_HEADTAIL_H
#  define MAS_VARVEC_HEADTAIL_H


void _mas_varvec_add_tail( mas_varvec_t * vclass, const char *tail );
mas_varvec_t *mas_varvec_add_tail( mas_varvec_t * vclass, const char *vclass_name, const char *tail );

void _mas_varvec_set_head( mas_varvec_t * vclass, const char *head );
mas_varvec_t *mas_varvec_set_head( mas_varvec_t * vclass, const char *vclass_name, const char *head );
mas_varvec_t *mas_varvec_set_head_va( mas_varvec_t * vclass, const char *vclass_name, mas_xvsnprintf_t func,
                                                   const char *fmt, va_list args );
mas_varvec_t *mas_varvec_set_headf( mas_varvec_t * vclass, const char *vclass_name, mas_xvsnprintf_t func,
                                                 const char *fmt, ... );



#endif

