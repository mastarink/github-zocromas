#ifndef MAS_FILEINFO_OBJECT_H
#  define MAS_FILEINFO_OBJECT_H

#  include "mas_fileinfo_types.h"

mas_fileinfo_t *mas_fileinfo_create( void );
void mas_fileinfo_delete( mas_fileinfo_t * fileinfo );
mas_fileinfo_t *mas_fileinfo_init( mas_fileinfo_t * fileinfo, const char *root, const char *path, data_loader_t loader,
                                   const void *userdata );

void mas_fileinfo_delete_data( mas_fileinfo_t * fileinfo );

#endif
