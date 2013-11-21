#ifndef MAS_UNIDATA_H
#  define MAS_UNIDATA_H

#  include <mastar/types/mas_unidata_types.h>
#  include <mastar/types/mas_modules_types.h>

/*
this:
  mas_unidata.h
related:
  mas_fileinfo.h
  mas_unidata.c
  mas_fileinfo.c
  mas_fileinfo_object.c
*/
mas_evaluated_t *mas_evaluated_create( void );
void mas_evaluated_delete( mas_evaluated_t * answer );

mas_evaluated_t *mas_evaluated_wrap_pchar( const char *sanswer );
mas_evaluated_t *mas_evaluated_wrap_typed( void *sanswer, mas_cmdtype_t cmdtype );


mas_unidata_t *mas_unidata_create( void );
void mas_unidata_delete( mas_unidata_t * ud );
void mas_unidata_link_data( mas_unidata_t * udata, mas_evaluated_t * data, size_t size );

mas_evaluated_t *mas_unidata_data( mas_unidata_t * ud );

size_t mas_unidata_data_size( mas_unidata_t * ud );


mas_content_type_t mas_udata_icontent_type( mas_unidata_t * ud );
mas_content_type_t mas_unidata_parse_content_type( const char *ct );
void mas_udata_set_icontent_type( mas_unidata_t * ud, mas_content_type_t ict );
void mas_udata_set_icontent_size( mas_unidata_t * ud, size_t size );

char *mas_unidata_content_type_string( mas_unidata_t * ud );



/* #include <mastar/types/mas_common_cdefs.h> */
#endif
