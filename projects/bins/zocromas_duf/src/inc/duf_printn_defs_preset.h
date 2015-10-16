#ifndef MAS_DUF_PRINTN_DEFS_PRESET_H
#  define MAS_DUF_PRINTN_DEFS_PRESET_H

#  ifndef MAST_OUTPUT_LEVEL
#    define MAST_OUTPUT_LEVEL duf_output_level()
#  endif

#  ifndef MAST_OUTPUT_FILE
#    define MAST_OUTPUT_FILE  duf_output_file()
#  endif

#  define DUF_PRINT_COLOR_S( ... )  \
         mas_print_color_s( __VA_ARGS__ )
#  define DUF_SNCOLOR_S( ... ) \
         mas_sncolor_s( __VA_ARGS__ )

#  endif

