#include <string.h>

#include "duf_maintenance.h"


#include "duf_options_enum.h"

/* man getopt_long */


/* ###################################################################### */
#include "duf_xtended_table.h"
/* ###################################################################### */

static const duf_longval_extended_t _lo_extended1[] = {
/* #include "duf_optable.cdef"                   */
/* #include "duf_optable_str.cdef"               */
/* #include "duf_optable_db.cdef"                */
/* #include "duf_optable_sflag.cdef"             */
/* #include "duf_optable_uplus.cdef"             */
/* #include "duf_optable_help.cdef"              */
/* #include "duf_optable_trace.cdef"             */
/* #include "duf_optable_filter.cdef"            */
/* #include "duf_optable_debug.cdef"             */
/* #include "duf_optable_redo_and_obsolete.cdef" */
  {0}
};

/* static const duf_longval_extended_t _lo_extended2[] = { */
/* (* #include "duf_optable_flag.cdef" *)                  */
/*   {0}                                                   */
/* };                                                      */
extern const duf_longval_extended_table_t optable_main;
extern const duf_longval_extended_table_t optable_io;
extern const duf_longval_extended_table_t optable_experimental;
extern const duf_longval_extended_table_t optable_fs;
extern const duf_longval_extended_table_t optable_interactive;
extern const duf_longval_extended_table_t optable_str;
extern const duf_longval_extended_table_t optable_db;
extern const duf_longval_extended_table_t optable_sccb;
extern const duf_longval_extended_table_t optable_info;
extern const duf_longval_extended_table_t optable_flag;
extern const duf_longval_extended_table_t optable_sflag;
extern const duf_longval_extended_table_t optable_uplus;
extern const duf_longval_extended_table_t optable_help;

#ifdef MAS_TRACING_OPTIONS
extern const duf_longval_extended_table_t optable_trace;
#endif
extern const duf_longval_extended_table_t optable_format;
extern const duf_longval_extended_table_t optable_filter;
extern const duf_longval_extended_table_t optable_test;
extern const duf_longval_extended_table_t optable_debug;
extern const duf_longval_extended_table_t optable_redo_and_obsolete;

static const duf_longval_extended_table_t *const _lo_extended_table_multi[] = {
  &optable_main,
  &optable_experimental,
  &optable_fs,
  &optable_io,
  &optable_interactive,
  &optable_str,
  &optable_db,
  &optable_sccb,
  &optable_info,
  &optable_sflag,
  &optable_flag,
  &optable_uplus,
#ifdef MAS_TRACING_OPTIONS
  &optable_trace,
#endif
  &optable_filter,
  &optable_test,
  &optable_debug,
  &optable_help,
  &optable_format,
  &optable_redo_and_obsolete,

  NULL
};

static const duf_longval_extended_table_t **lo_extended_table_multi = NULL;
duf_longval_extended_vtable_t **lo_extended_vtable_multi = NULL;

static void
_duf_extended_table_multi_init( void )
{
  unsigned numtabs = 0;

#if 0
  const duf_longval_extended_table_t **src = NULL;

  src = _lo_extended_table_multi;
  while ( src && *src )
  {
    numtabs++;
    src++;
  }
  numtabs++;
#else
  for ( numtabs = 0; _lo_extended_table_multi[numtabs] && _lo_extended_table_multi[numtabs]->xlist; numtabs++ );
#endif
  lo_extended_table_multi = mas_malloc( sizeof( duf_longval_extended_table_t ** ) * ( numtabs + 1 ) ); /* +1 to allocate for terminating NULL */
  memset( lo_extended_table_multi, 0, sizeof( duf_longval_extended_table_t ** ) * ( numtabs + 1 ) ); /* +1 to allocate for terminating NULL */
  {
#if 0
    size_t itab = 0;
    const duf_longval_extended_table_t **dst = NULL;

    dst = lo_extended_table_multi;

    for ( const duf_longval_extended_table_t ** src = _lo_extended_table_multi; src && *src; src++, dst++ )
    {
      *dst = *src;
      T( "@tab.name: '%s' : %lu [%p:%p:%p]", ( *src )->name, sizeof( *src ), ( *src )->xlist, ( *dst )->xlist,
         _lo_extended_table_multi[itab]->xlist );
      itab++;
    }
    *dst++ = NULL;
#else
    for ( size_t itab = 0; itab < numtabs; itab++ )
    {
      lo_extended_table_multi[itab] = _lo_extended_table_multi[itab];
      /* T( "@@tab.name: '%s' : [%p:%p]", _lo_extended_table_multi[itab]->name, _lo_extended_table_multi[itab], _lo_extended_table_multi[itab]->xlist ); */
      for ( const duf_longval_extended_t * extended = _lo_extended_table_multi[itab]->xlist; extended && extended->o.name; extended++ )
      {
      }
    }
#endif
  }
}

/* 20160212.131010 */
static void
duf_extended_vtable_multi_init_table( const duf_longval_extended_table_t * const *xtable_multi )
{
  unsigned numtabs = 0;
  duf_option_code_t maxcodeval = 0;

  for ( numtabs = 0; xtable_multi[numtabs] && xtable_multi[numtabs]->xlist; numtabs++ );
  assert( lo_extended_vtable_multi == NULL );
  lo_extended_vtable_multi = mas_malloc( sizeof( duf_longval_extended_vtable_t ** ) * ( numtabs + 1 ) ); /* +1 to allocate for terminating NULL */
  memset( lo_extended_vtable_multi, 0, sizeof( duf_longval_extended_vtable_t ** ) * ( numtabs + 1 ) ); /* +1 to allocate for terminating NULL */
  {
    for ( size_t itab = 0; itab < numtabs; itab++ )
    {
      const duf_longval_extended_t *xlist;

      xlist = xtable_multi[itab]->xlist;
      for ( const duf_longval_extended_t * x = xlist; x->o.name; x++ )
      {
        size_t xn = x - xlist;

        if ( xlist[xn].o.val && xlist[xn].o.val > maxcodeval )
          maxcodeval = xlist[xn].o.val;
      }
    }
    maxcodeval += 100;
    maxcodeval /= 100;
    maxcodeval *= 100;
  }
  for ( size_t itab = 0; itab < numtabs; itab++ )
  {
    duf_longval_extended_vtable_t *vtable;

    vtable = mas_malloc( sizeof( duf_longval_extended_t ) );
    memset( vtable, 0, sizeof( duf_longval_extended_t ) );
#if 0
    memcpy( &vtable, &xtable_multi[itab], sizeof( vtable ) );
#else
    vtable->name = xtable_multi[itab]->name;
    vtable->id = xtable_multi[itab]->id;
    vtable->stage_opts = xtable_multi[itab]->stage_opts;
#endif
    /* T( "@@%lu. tab.name: '%s' : [%p:%p]", itab, vtable->name, vtable, vtable->xlist ); */
    {
      size_t xcnt = 0;

      for ( const duf_longval_extended_t * x = xtable_multi[itab]->xlist; x->o.name; x++ )
        xcnt++;
      vtable->xlist = mas_malloc( sizeof( duf_longval_extended_t ) * ( xcnt + 1 ) );
      for ( size_t xn = 0; xn < xcnt; xn++ )
      {
        vtable->xlist[xn] = xtable_multi[itab]->xlist[xn];
      }
      for ( size_t xn = 0; xn < xcnt; xn++ )
      {
        if ( !vtable->xlist[xn].o.val )
        {
          T( "@\"%s\" no codeval; setting automatically to %d", vtable->xlist[xn].o.name, maxcodeval );
          vtable->xlist[xn].o.val = maxcodeval++;
        }
        else
        {
          /* T( "@@%s --", vtable->xlist[xn].o.name ); */
        }
      }
    }
    lo_extended_vtable_multi[itab] = vtable;
  }
}

static void
duf_extended_table_multi_init( void )
{
  static unsigned inited = 0;

  if ( !inited )
  {
    _duf_extended_table_multi_init(  );
    inited = 1;
  }
}

/* 20160212.131034 */
static void
duf_extended_vtable_multi_init( void )
{
  static unsigned inited = 0;

  if ( !inited )
  {
    duf_extended_vtable_multi_init_table( _lo_extended_table_multi );
    inited = 1;
  }
}

static void extended_table_multi_init( void ) __attribute__ ( ( constructor( 101 ) ) );
static void
extended_table_multi_init( void )
{
  /* duf_extended_table_multi_init(  ); */
}

const duf_longval_extended_table_t **
duf_extended_table_multi( void ) /* obsolete/not used 20160208.130331 */
{
  duf_extended_table_multi_init(  );
  return lo_extended_table_multi;
}

static duf_longval_extended_vtable_t **
_duf_extended_vtable_multi( void )
{
  duf_extended_vtable_multi_init(  );
  return lo_extended_vtable_multi;
}

const duf_longval_extended_vtable_t *const *
duf_extended_vtable_multi( void )
{
  return ( const duf_longval_extended_vtable_t * const * ) _duf_extended_vtable_multi(  );
}




/* _lo_extended_table_multi; */
static void unbuild( void ) __attribute__ ( ( destructor( 101 ) ) );
static void
unbuild( void )
{
  mas_free( lo_extended_table_multi );
  {
    for ( size_t itab = 0; lo_extended_vtable_multi && lo_extended_vtable_multi[itab] /* && lo_extended_vtable_multi[itab]->name */ ; itab++ )
    {
      mas_free( lo_extended_vtable_multi[itab]->xlist );
      mas_free( lo_extended_vtable_multi[itab] );
      lo_extended_vtable_multi[itab] = NULL;
    }

    mas_free( lo_extended_vtable_multi );
    lo_extended_vtable_multi = NULL;
  }
}

/* unsigned lo_extended_count = sizeof( _lo_extended ) / sizeof( _lo_extended[0] ); */

/* static const duf_option_t duf_longopts_table[] = {                                                                                             */
/* (*{.name = "add-to-group",                      .has_arg = no_argument,                         .val = DUF_OPTION_ADD_TO_GROUP}, *)            */
/*   {.name = "dry-run",.has_arg = no_argument,.val = DUF_OPTION_DRY_RUN_TRACE},                                                                  */
/*   {.name = "exclude",.has_arg = required_argument,.val = DUF_OPTION_GLOB_EXCLUDE_FILES},                                                       */
/* (*{.name = "group",                             .has_arg = required_argument,                           .val = DUF_OPTION_GROUP}, *)           */
/*   {.name = "include",.has_arg = required_argument,.val = DUF_OPTION_GLOB_INCLUDE_FILES},                                                       */
/* (*{.name = "limit",                             .has_arg = required_argument,                           .val = DUF_OPTION_LIMIT}, *)           */
/*   {.name = "max-dbg-lines",.has_arg = required_argument,.val = DUF_OPTION_MAX_DBGLINE},                                                        */
/*   {.name = "max-dirfiles",.has_arg = required_argument,.val = DUF_OPTION_MAXDIRFILES},                                                         */
/*   {.name = "max-dirs",.has_arg = required_argument,.val = DUF_OPTION_MAXITEMS_DIRS},                                                           */
/*   {.name = "max-files",.has_arg = required_argument,.val = DUF_OPTION_MAXITEMS_FILES},                                                         */
/*   {.name = "max-items",.has_arg = required_argument,.val = DUF_OPTION_MAXITEMS},                                                               */
/*   {.name = "max-same",.has_arg = required_argument,.val = DUF_OPTION_MAXSAME},                                                                 */
/*   {.name = "mdpath",.has_arg = no_argument,.val = DUF_OPTION_FLAG_MDPATH},                                                                     */
/*   {.name = "memusage",.has_arg = no_argument,.val = DUF_OPTION_MEMUSAGE},                                                                      */
/*   {.name = "min-dbg-lines",.has_arg = required_argument,.val = DUF_OPTION_MIN_DBGLINE},                                                        */
/*   {.name = "min-dirfiles",.has_arg = required_argument,.val = DUF_OPTION_MINDIRFILES},                                                         */
/*   {.name = "output-level",.has_arg = optional_argument,.val = DUF_OPTION_OUTPUT_LEVEL},                                                        */
/* (*{.name = "print-dirs",                        .has_arg = no_argument,                         .val = DUF_OPTION_PRINT_DIRS},   *)            */
/* (*{.name = "print-duplicates",                  .has_arg = no_argument,                         .val = DUF_OPTION_PRINT_DUPLICATES}, *)        */
/* (*{.name = "print-files",                       .has_arg = no_argument,                         .val = DUF_OPTION_PRINT_FILES}, *)             */
/* (*{.name = "print-paths",                       .has_arg = no_argument,                         .val = DUF_OPTION_PRINT_PATHS}, *)             */
/*   {.name = "remove-database",.has_arg = no_argument,.val = DUF_OPTION_FLAG_REMOVE_DATABASE},                                                   */
/* (*{.name = "remove-from-group",                 .has_arg = no_argument,                         .val = DUF_OPTION_REMOVE_FROM_GROUP}, *)       */
/* (*{.name = "same-exif",                         .has_arg = no_argument,                         .val = DUF_OPTION_SAME_EXIF},   *)             */
/* (*{.name = "same-files",                        .has_arg = no_argument,                         .val = DUF_OPTION_SAME_FILES}, *)              */
/*   {.name = "same",.has_arg = required_argument,.val = DUF_OPTION_SAME},                                                                        */
/* (*{.name = "same-md5",                          .has_arg = no_argument,                         .val = DUF_OPTION_SAME_MD5},     *)            */
/*   {.name = "sample",.has_arg = optional_argument,.val = DUF_OPTION_SAMPLE},                                                                    */
/*   {.name = "sampupd",.has_arg = optional_argument,.val = DUF_OPTION_SAMPUPD},                                                                  */
/*   {.name = "test-option",.has_arg = required_argument,.val = DUF_OPTION_TEST},                                                                 */
/*   {.name = "tree2db",.has_arg = no_argument,.val = DUF_OPTION_TREE_TO_DB},                                                                     */
/*   {.name = "tree",.has_arg = no_argument,.val = DUF_OPTION_FLAG_TREE},                                                                         */
/*   {.name = "tree-to-db",.has_arg = no_argument,.val = DUF_OPTION_TREE_TO_DB},                                                                  */
/*   {.name = "uni-scan",.has_arg = no_argument,.val = DUF_OPTION_FLAG_UNI_SCAN},                                                                 */
/* (*{.name = "update-exif",                       .has_arg = no_argument,                         .val = DUF_OPTION_UPDATE_EXIF}, *)             */
/* (*{.name = "update-mdpath",                     .has_arg = no_argument,                         .val = DUF_OPTION_UPDATE_MDPATH}, *)           */
/* (*{.name = "update-mdpath-selective",           .has_arg = no_argument,                         .val = DUF_OPTION_UPDATE_MDPATH_SELECTIVE}, *) */
/* (*{.name = "update-path",                       .has_arg = no_argument,                         .val = DUF_OPTION_UPDATE_PATH}, *)             */
/*   {.name = "vacuum",.has_arg = no_argument,.val = DUF_OPTION_FLAG_VACUUM},                                                                     */
/*   {.name = "zero-db",.has_arg = no_argument,.val = DUF_OPTION_FLAG_ZERO_DB},                                                                   */
/* (*{.name = "zero-duplicates",                   .has_arg = no_argument,                         .val = DUF_OPTION_ZERO_DUPLICATES}, *)         */
/*   {0},                                                                                                                                         */
/* };                                                                                                                                             */
/*                                                                                                                                                */
/* const duf_option_t *duf_longopts = &duf_longopts_table[0];                                                                                     */
/* unsigned duf_longopts_count = sizeof( duf_longopts_table ) / sizeof( duf_longopts_table[0] );                                                  */
