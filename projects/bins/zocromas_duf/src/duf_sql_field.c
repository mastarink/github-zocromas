#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"
#include "duf_utils.h"

#include "duf_dbg.h"

#include "duf_config.h"

#include "duf_sql_def.h"
#include "duf_uni_scan.h"
#include "duf_path.h"


/* ###################################################################### */
#include "duf_sql_field.h"
/* ###################################################################### */



int
__duf_sql_pos_by_name( const char *name, duf_record_t * precord, int *phave, int optional )
{
  int pos = DUF_ERROR_NO_FIELD;

  if ( precord )
  {
    for ( int i = 0; i < precord->ncolumns; i++ )
    {
      if ( 0 == strcmp( name, precord->pnames[i] ) )
      {
        pos = i;
        break;
      }
    }
    if ( phave )
    {
      *phave = pos;
    }
    else
    {
      if ( pos < 0 && !optional )
      {
        for ( int i = 0; i < precord->ncolumns; i++ )
        {
          fprintf( stderr, "  Have '%s'\n", precord->pnames[i] );
        }
        fprintf( stderr, "Error - no such field '%s' {%p} (%u)\n", name, ( void * ) precord, precord->ncolumns );
        {
          /* force SEGFAULT */
          char *p = NULL;
          char c __attribute__ ( ( unused ) ) = *p;
        }
        exit( 4 );
      }
    }
  }
  return pos;
}

const char *
__duf_sql_str_by_name( const char *name, duf_record_t * precord, int *phave, int optional )
{
  const char *ptr = NULL;
  int pos = __duf_sql_pos_by_name( name, precord, phave, optional );

  if ( pos >= 0 )
    ptr = precord->presult[pos];

  return ptr;
}

const char *
__duf_sql_val_by_name( const char *name, duf_record_t * precord, int *phave, int optional )
{
  const char *ptr = NULL;
  int pos = __duf_sql_pos_by_name( name, precord, phave, optional );

  if ( pos >= 0 )
    ptr = precord->presult[pos];
  return ptr;
}

unsigned long long
__duf_sql_ull_by_name( const char *name, duf_record_t * precord, int *phave, int optional )
{
  const char *ptr = NULL;

  ptr = __duf_sql_val_by_name( name, precord, phave, optional );
  return ptr ? strtoll( ptr, NULL, 10 ) : 0;
}

/* this is callback of type: duf_sql_select_cb_t (first range) */
int
duf_sel_cb_field_by_sccb( duf_record_t * precord, va_list args, void *sel_cb_udata, duf_scan_callback_file_t str_cb, void *str_cb_udata,
                          duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, const duf_dirhandle_t * pdhu )
{
  int r = DUF_ERROR_GET_FIELD;

  duf_dbgfunc( DBG_START, __func__, __LINE__ );
  if ( sccb )
  {
    unsigned long long *pvalue;

    pvalue = ( unsigned long long * ) sel_cb_udata;
    /* fprintf( stderr, "OGO %s :: %llu\n", __func__, duf_sql_ull_by_name( sccb->fieldset, precord, 0 ) ); */
    if ( pvalue && precord->nrow == 0 )
    {
      int have_pos = 0;

      r = DUF_ERROR_NO_FIELD_OPTIONAL;
      *pvalue = __duf_sql_ull_by_name( sccb->fieldset, precord, &have_pos, 0 );
      if ( have_pos >= 0 )
        r = precord->nrow;
      else
        DUF_ERROR( "r=%d; no field %s", r, sccb->fieldset );
    }
    else
      DUF_ERROR( "something is wrong" );
  }
  else
  {
    r = 0;
  }
  duf_dbgfunc( DBG_ENDR, __func__, __LINE__, r );
  return ( r );
}
