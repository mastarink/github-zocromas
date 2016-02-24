#undef MAS_TRACING
#include "duf_maintenance.h"

#include "duf_config.h"

#include "evsql_begfin.h"

#include "duf_maindb.h"
#include "duf_status_ref.h"
#include "sql_selected_defs.h"

/* ###################################################################### */
#include "duf_pdi_attach.h"
/* ###################################################################### */

/* 20150912.094647 */
#ifdef DUF_ATTACH_SELECTED_PATTERN
int
duf_pdi_attach_selected( duf_depthinfo_t * pdi )
{
  DUF_STARTR( r );
  if ( !pdi->attached_copy )
  {
    DORF( r, duf_main_db_open, pdi );
    DUF_TRACE( pdi, 0, "@@@@ opened:%s; db_attached_selected:%s", global_status.db_opened_name, pdi->db_attached_selected );
    DUF_TRACE( pdi, 0, "@@@@ opened:%s; db_attached_selected:%s", global_status.db_opened_name, pdi->db_attached_selected );
    /* assert( global_status.db_attached_selected == NULL ); */
    if ( !pdi->db_attached_selected )
    {
      static const char *sql = "ATTACH DATABASE '" DUF_ATTACH_SELECTED_PATTERN "' AS " DUF_DBSELECTEDALIAS;
      static const char *sql1 = "DROP  TABLE IF EXISTS " DUF_DBSELECTEDALIAS "." DUF_SQL_SELECTED_TMP_FILENAMES;
      int changes = 0;

      T( "(%d) to attach (%p:%s)", r, pdi, pdi->pdi_name );
      pdi->db_attached_selected = mas_strdup( pdi->pdi_name );
      /* DUF_TRACE( sql, 0, "%p ATTACH %s : %s", pdi, pdi->db_attached_selected, sql ); */
      DUF_TRACE( db, 0, "%p ATTACH %s : %s", pdi, pdi->db_attached_selected, sql );

      DOR( r, duf_eval_sql_one( sql, ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL, pdi->db_attached_selected, &changes ) );
      DOR( r, duf_eval_sql_one( sql1, ( duf_ufilter_t * ) NULL, ( duf_yfilter_t * ) NULL, pdi->db_attached_selected, &changes ) );
    }
    DUF_TRACE( sql, 0, "%p post ATTACH %s", pdi, pdi->db_attached_selected );
  }
  DUF_ENDR( r );
}
#endif

/* 20150904.090641 */
int
duf_pdi_detach_selected( duf_depthinfo_t * pdi )
{
  DUF_STARTR( r );
  static const char *sql1 = "DETACH DATABASE 'duf${PDI_NAME}'";

  int changes = 0;


  DOR( r, duf_eval_sql_one( sql1, pdi->pup, ( duf_yfilter_t * ) NULL, pdi->pdi_name, &changes ) );
  T( "(%d) DETACH changes:%d", r, changes );

  DUF_ENDR( r );
}
