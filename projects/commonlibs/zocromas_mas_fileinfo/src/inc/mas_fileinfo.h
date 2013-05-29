#ifndef MAS_FILEINFO_H
#  define MAS_FILEINFO_H

#  include <mastar/types/mas_fileinfo_types.h>

/*
this:
  mas_fileinfo.h
related:
  mas_unidata.c
  mas_unidata.h
  mas_fileinfo.c
  mas_fileinfo_object.c
*/

#ifdef MAS_OLD_VARIABLES_HTTP
mas_variables_list_head_t *mas_fileinfo_make_headers( mas_variables_list_head_t * outdata, mas_fileinfo_t * fileinfo );
mas_variables_list_head_t *mas_fileinfo_make_body( mas_variables_list_head_t * outdata, mas_fileinfo_t * fileinfo );
#else
mas_varset_t *mas_fileinfo_make_headers( mas_varset_t * outdata, mas_fileinfo_t * fileinfo );
mas_varset_t *mas_fileinfo_make_body( mas_varset_t * outdata, mas_fileinfo_t * fileinfo );
#endif

int mas_fileinfo_make_etag( mas_fileinfo_t * fileinfo );
int mas_fileinfo_stat( mas_fileinfo_t * fileinfo );
char *mas_fileinfo_content_type_string( mas_fileinfo_t * fileinfo );

/* char *mas_fileinfo_load_data( mas_fileinfo_t * fileinfo ); */
char *_mas_fileinfo_data( mas_fileinfo_t * fileinfo );

char *mas_fileinfo_data( mas_fileinfo_t * fileinfo );

/* char *_mas_fileinfo_link_data( mas_fileinfo_t * fileinfo, char *data, size_t size ); */
char *_mas_fileinfo_link_dataz( mas_fileinfo_t * fileinfo, char *data );

/* char *mas_fileinfo_set_dataz( mas_fileinfo_t * fileinfo, char *data ); */
/* char *mas_fileinfo_set_data( mas_fileinfo_t * fileinfo, char *data, size_t size ); */


size_t mas_fileinfo_data_size( mas_fileinfo_t * fileinfo );

mas_content_type_t mas_fileinfo_content_type_by_ext( mas_fileinfo_t * fileinfo );
mas_content_type_t mas_fileinfo_icontent_type( mas_fileinfo_t * fileinfo );
void mas_fileinfo_set_icontent_type( mas_fileinfo_t * fileinfo, mas_content_type_t ict );
void mas_fileinfo_set_icontent_size( mas_fileinfo_t * fileinfo, size_t size );

unsigned long mas_fileinfo_content_size( mas_fileinfo_t * fileinfo );
void mas_fileinfo_set_content_size( mas_fileinfo_t * fileinfo, const char *ssize );

mas_unidata_t *mas_fileinfo_unidata( mas_fileinfo_t * fileinfo );

#endif
