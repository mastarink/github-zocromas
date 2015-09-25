#ifndef MAS_DUF_OPTION_CMD_H
#  define MAS_DUF_OPTION_CMD_H


int duf_exec_cmd_long_xtables( const char *string, const duf_longval_extended_table_t ** xtables, char vseparator, duf_option_stage_t istage,
                               duf_option_source_t source ) __attribute__ ( ( warn_unused_result ) );
int duf_exec_cmd_long_xtables_std( const char *string, char vseparator, duf_option_stage_t istage, duf_option_source_t source )
      __attribute__ ( ( warn_unused_result ) );



#endif
