#ifndef MAS_LONGOPTS_TABLE_OBJECT_H
#  define MAS_LONGOPTS_TABLE_OBJECT_H

#  include <getopt.h>

#  include <mastar/types/mas_opts_types.h>

struct option *mas_cli_longopts_table_create_old( const mas_optionx_t * table, size_t num );
void mas_cli_longopts_table_delete_old( struct option *longtab, size_t num );

struct option *mas_cli_longopts_table_create( const mas_optionx_t * table );
void mas_cli_longopts_table_delete( struct option *longtab );


/* #include <mastar/types/mas_common_cdefs.h> */
#endif
