/* #undef MAS_TRACING */

#include <assert.h>                                                  /* assert */
#include <string.h>
#include <stddef.h>                                                  /* NULL */

#include <mastar/wrap/mas_std_def.h>
/* #include <mastar/trace/mas_trace.h> */

/* ###################################################################### */
#include "duf_optimpl_class.h"
/* ###################################################################### */
static const char *oclass_titles[DUF_OPTIMPL_CLASS_MAX + 1] = {
  [DUF_OPTIMPL_CLASS_HELP] = "Help",
  [DUF_OPTIMPL_CLASS_NO_HELP] = "No help",
  [DUF_OPTIMPL_CLASS_SYSTEM] = "System",
  [DUF_OPTIMPL_CLASS_CONTROL] = "Control",
  [DUF_OPTIMPL_CLASS_DB] = "Database",
  [DUF_OPTIMPL_CLASS_FS] = "File system",
  [DUF_OPTIMPL_CLASS_SCCB] = "SCCB system",
  [DUF_OPTIMPL_CLASS_REFERENCE] = "Reference",
  [DUF_OPTIMPL_CLASS_COLLECT] = "Collect",
  [DUF_OPTIMPL_CLASS_SCAN] = "Scan",
  [DUF_OPTIMPL_CLASS_FILTER] = "Filter",
  [DUF_OPTIMPL_CLASS_UPDATE] = "Update",
  [DUF_OPTIMPL_CLASS_REQUEST] = "Request",
  [DUF_OPTIMPL_CLASS_PRINT] = "Print",
  [DUF_OPTIMPL_CLASS_TRACE] = "Trace",
  [DUF_OPTIMPL_CLASS_OBSOLETE] = "Obsolete",
  [DUF_OPTIMPL_CLASS_OTHER] = "Other",
  [DUF_OPTIMPL_CLASS_NONE] = "None",
  [DUF_OPTIMPL_CLASS_DEBUG] = "DEBUG",
  [DUF_OPTIMPL_CLASS_NODESC] = "No desc",
};

duf_optimpl_class_t
duf_optclass_title2code( const char *title )
{
  for ( duf_optimpl_class_t oclass = DUF_OPTIMPL_CLASS_NONE; oclass <= DUF_OPTIMPL_CLASS_MAX; oclass++ )
  {
    if ( title && oclass_titles[oclass] && 0 == strcmp( title, oclass_titles[oclass] ) )
      return oclass;
  }
  return DUF_OPTIMPL_CLASS_NONE;
}

const char *
duf_optclass_title( duf_optimpl_class_t oclass )
{
  const char *title = NULL;

  if ( oclass > DUF_OPTIMPL_CLASS_NONE && oclass <= DUF_OPTIMPL_CLASS_MAX && oclass_titles[oclass] && *oclass_titles[oclass] )
    title = oclass_titles[oclass];
  return title;
}

#define ENUM_WRAP(_name) [DUF_OPTIMPL_CLASS_ ## _name]= #_name,
#define ENUM_WRAP_V(_name, _val)
static char *oclass_names[] = {
#include "duf_optimpl_class_enum.def"
  NULL
};

#undef ENUM_WRAP
#undef ENUM_WRAP_V

duf_optimpl_class_t
duf_optclass_name2code( const char *name )
{
  for ( duf_optimpl_class_t oclass = DUF_OPTIMPL_CLASS_NONE; oclass <= DUF_OPTIMPL_CLASS_MAX; oclass++ )
  {
    if ( name && oclass_names[oclass] && 0 == strcmp( name, oclass_names[oclass] ) )
      return oclass;
  }

  return DUF_OPTIMPL_CLASS_NONE;
}

const char *
duf_optclass_name( duf_optimpl_class_t oclass )
{
  return ( oclass >= 0 && oclass < DUF_OPTIMPL_CLASS_MAX ) ? ( const char * ) /* mas_strtolower */ ( oclass_names[oclass] ) : ( const char * ) NULL;
}

/* FIXME: duplicate function!! duf_optclass_name and duf_optclass2string */
const char *
duf_optclass2string( duf_optimpl_class_t oclass )
{
  const char *rs = NULL;

  switch ( oclass )
  {
#define ENUM_WRAP(_n)       case DUF_OPTIMPL_CLASS_ ## _n: rs= #_n; break;
#define ENUM_WRAP_V(_n, _v) case DUF_OPTIMPL_CLASS_ ## _n: rs= #_n; break;
#include "duf_optimpl_class_enum.def"
#undef ENUM_WRAP
#undef ENUM_WRAP_V
  }
  return rs;
}
