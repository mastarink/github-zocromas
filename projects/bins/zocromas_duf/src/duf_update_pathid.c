#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
/* #include <unistd.h> */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>

#include "duf_types.h"
#include "duf_config.h"

#include "duf_sql.h"
#include "duf_path.h"
/* #include "duf_insert.h" */

/* #include "duf_update_realpath.h" */
/* #include "duf_update_path.h" */
#include "duf_update_pathentries.h"

#include "duf_dbg.h"

/* ###################################################################### */
#include "duf_update_pathid.h"
/* ###################################################################### */




/* unsigned long long                                                                                               */
/* duf_update_pathid_down_filter( unsigned long long parentid, duf_filter_t * pfilter, unsigned long long pathid )  */
/* {                                                                                                                */
/*   unsigned long items = 0;                                                                                       */
/*                                                                                                                  */
/*   duf_dbgfunc( DBG_START, __func__, __LINE__ );                                                                  */
/*   items = duf_update_realpath_entries_filter( pathid, pfilter );                                                 */
/*   if ( items >= 0 )                                                                                              */
/*   {                                                                                                              */
/*     duf_sql( "UPDATE duf_paths " " SET items='%u', last_updated=datetime() " " WHERE id='%lu'", items, pathid ); */
/*     (* TODO group is really TAG *)                                                                               */
/*     duf_pathid_group( "updated", pathid, +1 );                                                                   */
/*   }                                                                                                              */
/*   duf_dbgfunc( DBG_ENDULL, __func__, __LINE__, pathid );                                                         */
/*   return pathid;                                                                                                 */
/* }                                                                                                                */
