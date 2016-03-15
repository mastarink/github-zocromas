#ifndef MAS_DUF_OPTIMPL_CLASS_H
# define MAS_DUF_OPTIMPL_CLASS_H

# include "duf_optimpl_class_types.h"                                /* duf_optimpl_class_t */

duf_optimpl_class_t duf_optclass_name2code( const char *name );
const char *duf_optclass_name( duf_optimpl_class_t oclass );
const char *duf_optclass2string( duf_optimpl_class_t oclass );
duf_optimpl_class_t duf_optclass_title2code( const char *title );
const char *duf_optclass_title( duf_optimpl_class_t oclass );

#endif
