#ifndef MAS_MAST_TRACE_DEFS_PRESET_H
#  define MAS_MAST_TRACE_DEFS_PRESET_H

# ifndef MAST_TRACE_INDEX
#  define MAST_TRACE_INDEX(_name)  -1
# endif

/* #define MAST_OUTPUT_LEVEL mas_output_level() */
#  ifndef MAST_OUTPUT_LEVEL
#    define MAST_OUTPUT_LEVEL 0
#  endif
/*                                       */
/* #  ifndef MAST_OUTPUT_FORCE_COLOR     */
/* #    define MAST_OUTPUT_FORCE_COLOR 0 */
/* #  endif                              */
/*                                       */
/* #  ifndef MAST_OUTPUT_NOCOLOR         */
/* #    define MAST_OUTPUT_NOCOLOR 0     */
/* #  endif                              */
/*                                       */
/*                                       */
/* #  ifndef MAST_OUTPUT_FILE            */
/* #    define MAST_OUTPUT_FILE  stdout  */
/* #  endif                              */


#define MAST_FL __func__,__LINE__

#endif
