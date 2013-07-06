#ifndef MAS_AUTOOBJECT_H
#  define MAS_AUTOOBJECT_H

#  include <mastar/types/mas_autoobject_types.h>

int mas_autoobject_delete_data( mas_autoobject_t * obj );

int _mas_autoobject_compare( const void *a, const void *b );

int mas_autoobject_fd( mas_autoobject_t * obj );
FILE *mas_autoobject_file( mas_autoobject_t * obj );

int mas_autoobject_open( mas_autoobject_t * obj, const char *name );
int mas_autoobject_reopen( mas_autoobject_t * obj );
int mas_autoobject_qopen( mas_autoobject_t * obj, const char *name );

/* int mas_autoobject_qopen_data( mas_autoobject_t * obj ); */

int _mas_autoobject_close( mas_autoobject_t * obj );
int mas_autoobject_close( mas_autoobject_t * obj );

void mas_autoobject_set_name( mas_autoobject_t * obj, const char *name );

int mas_autoobject_opened( mas_autoobject_t * obj );

void _mas_autoobject_set_iaccess_type( mas_autoobject_t * obj, mas_iaccess_type_t t );
void mas_autoobject_set_iaccess_type( mas_autoobject_t * obj, mas_iaccess_type_t t );

int mas_autoobject_set_data( mas_autoobject_t * obj, void *ptr );
int mas_autoobject_load_data( mas_autoobject_t * obj, int use_new );

int mas_autoobject_rewind( mas_autoobject_t * obj );
int mas_autoobject_cat( int han, mas_autoobject_t * obj, int use_new );

#endif
