#ifndef MAS_DUF_OPTION_CLARIFY_STRING_H
#  define MAS_DUF_OPTION_CLARIFY_STRING_H

/* naming convention:
 * <gen.prefix>_<subject prefix><entity>_<action-result>
 *   *** see duf_option_clarify.h
 * 1. gen.prefix: duf
 * 2. subject prefix: depends on main argument(s) type, see duf_option_clarify.h
 * 3. entity: option
 * 4. action: ...
 * */

/* TODO not used with duf_option_clarify_new.c */
int duf_soption_xclarify_at_stdx( const char *string, char vseparator, duf_option_stage_t istage, int all_matched, duf_option_source_t source )
      __attribute__ ( ( warn_unused_result ) );



#endif
