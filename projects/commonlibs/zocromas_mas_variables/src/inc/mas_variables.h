#ifndef MAS_VARIABLES_H
#  define MAS_VARIABLES_H

#  include  <stdio.h>
#  include  <mastar/types/mas_channel_types.h>
#  include  <mastar/types/mas_variables_types.h>

mas_variables_list_head_t *mas_variable_vcreate_x( mas_variables_list_head_t * variables, /* th_type_t thtype, */ const char *vclass,
                                                   const char *name, mas_xvsnprintf_t func, const char *fmt, va_list args,
                                                   int nomem );


mas_variables_list_head_t *mas_variable_create_x( mas_variables_list_head_t * variables, /* th_type_t thtype, */ const char *vclass,
                                                  const char *name, mas_xvsnprintf_t func, const char *fmt, ... );
mas_variables_list_head_t *mas_variable_create_typed( mas_variables_list_head_t * variables, /* th_type_t thtype, */ const char *vclass,
                                                      const char *name, mas_vartype_t vtype, const void *data, size_t datasize,
                                                      int nomem );
mas_variables_list_head_t *mas_variable_create_text( mas_variables_list_head_t * variables, /* th_type_t thtype, */ const char *vclass,
                                                     const char *name, const void *txt, int nomem );
mas_variables_list_head_t *mas_variable_create_binary( mas_variables_list_head_t * variables, /* th_type_t thtype, */
                                                       const char *vclass, const char *name, const void *data, size_t datasize );
mas_variables_list_head_t *mas_variable_set_text( mas_variables_list_head_t * variables, /* th_type_t thtype, */ const char *vclass,
                                                  const char *name, const void *txt );


int mas_variables_delete( mas_variables_list_head_t * variables );

int mas_variables_writef( mas_variables_list_head_t * variables, const char *vclass, const char *fmt, int wfd );
int _mas_variables_writef( mas_variables_list_head_t * variables, const char *vclass, int fname, int fvalue, const char *fmt,
                           int wfd );

int
_mas_variables_fwritef( mas_variables_list_head_t * variables, const char *vclass, int fname, int fvalue, const char *fmt,
                        FILE * f );
int mas_variables_fwritef( mas_variables_list_head_t * variables, const char *vclass, const char *fmt, FILE * f );

int
_mas_variables_channel_writef( mas_variables_list_head_t * variables, const char *vclass, int fname, int fvalue, const char *fmt,
                               const mas_channel_t * pchannel );
int
mas_variables_chwritef( mas_variables_list_head_t * variables, const char *vclass, const char *fmt,
                        const mas_channel_t * pchannel );
int mas_variables_chwrite_pairs( mas_variables_list_head_t * variables, const char *vclass, const mas_channel_t * pchannel );
int mas_variables_memory_write_pairs( mas_variables_list_head_t * variables, const char *vclass, char *buffer, size_t max );

int
mas_variables_chwrite_bin( mas_variables_list_head_t * variables, const char *vclass, int fname, int fvalue,
                           const mas_channel_t * pchannel );

mas_variable_t *mas_variables_find( mas_variables_list_head_t * variables, const char *vclass, const char *name );

mas_variable_t *mas_variable_matching( mas_variables_list_head_t * variables, mas_variable_t * var, const char *vclass,
                                       const char *name );
mas_variable_t *mas_variable_first( mas_variables_list_head_t * variables, const char *vclass, const char *name );
mas_variable_t *mas_variable_next( mas_variable_t * var, const char *vclass, const char *name );


int mas_variables_log_pairs( mas_variables_list_head_t * variables, const char *vclass );


#endif
