#ifndef MAS_DUF_PRINT_DEFS_PRESET_H
#  define MAS_DUF_PRINT_DEFS_PRESET_H

#  ifndef DUF_OUTPUT_LEVEL
#    define DUF_OUTPUT_LEVEL duf_output_level()
#  endif

#  ifndef DUF_OUTPUT_FILE
#    define DUF_OUTPUT_FILE  duf_output_file()
#  endif

#  define DUF_PRINT_COLOR_S( ... )  \
         duf_print_color_s( __VA_ARGS__ )
/* #  define DUF_PRINT_COLOR_S( ... )  \     */
/*          mas_print_color_s( __VA_ARGS__ ) */
#  define DUF_SNCOLOR_S( ... ) \
        duf_sncolor_s( __VA_ARGS__ )


#  endif
