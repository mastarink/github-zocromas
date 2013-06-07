#ifndef MAS_VARSET_VCLASS_HEADTAIL_H
#  define MAS_VARSET_VCLASS_HEADTAIL_H


void _mas_varset_vclass_add_tail( mas_varset_class_t * vclass, const char *tail );
mas_varset_class_t *mas_varset_vclass_add_tail( mas_varset_class_t * vclass, const char *vclass_name, const char *tail );

void _mas_varset_vclass_set_head( mas_varset_class_t * vclass, const char *head );
mas_varset_class_t *mas_varset_vclass_set_head( mas_varset_class_t * vclass, const char *vclass_name, const char *head );
mas_varset_class_t *mas_varset_vclass_set_head_va( mas_varset_class_t * vclass, const char *vclass_name, mas_xvsnprintf_t func,
                                                   const char *fmt, va_list args );
mas_varset_class_t *mas_varset_vclass_set_headf( mas_varset_class_t * vclass, const char *vclass_name, mas_xvsnprintf_t func,
                                                 const char *fmt, ... );



#endif

