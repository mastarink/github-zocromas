#ifndef MAS_MAST_TRACE_DEFS_PRESET_H
#  define MAS_MAST_TRACE_DEFS_PRESET_H


/* #define MAST_OUTPUT_LEVEL mas_output_level() */
#  ifndef MAST_OUTPUT_LEVEL
#    define MAST_OUTPUT_LEVEL 0
#  endif

#  ifndef MAST_OUTPUT_FILE
#    define MAST_OUTPUT_FILE  stdout
#  endif


#define MAST_FL __func__,__LINE__

#endif
