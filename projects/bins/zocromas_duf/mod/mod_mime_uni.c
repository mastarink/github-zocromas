/* #define DUF_GET_FIELD_FROM_ROW */
/* #undef MAS_TRACING */
#include <assert.h>                                                  /* assert */
#include <stddef.h>                                                  /* NULL */
#include <string.h>
#include <errno.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>                                  /* mas_(malloc|free|strdup); etc. ▤ */
#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

/* TODO: see media-libs/libextractor */
/* man libmagic : LIBMAGIC(3)              Gentoo Library Functions Manual            LIBMAGIC(3) */
#include <magic.h>                                                   /* man libmagic */

#include "duf_config.h"                                              /* duf_get_config ✗ */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_pdi_context.h"
#include "duf_pdi_ref.h"
#include "duf_pdi_stmt.h"                                            /* duf_pdi_find_statement_by_id; etc. ✗ */

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */

#include "duf_sql_se_stmt_defs.h"                                    /* DUF_SQL_SE_BIND_S_OPT etc. ✗ */

#include "duf_sccb_structs.h"
#include "duf_sccb_row_field_defs.h"                                 /* DUF_*FIELD2* ✗ */
#include "duf_sccb_row.h"                                            /* datarow_*; duf_sccbh_row_get_*; sccbh_rows_eval ✗ */

#include "duf_sccbh_ref.h"
#include "duf_sccbh_shortcuts.h"                                     /* H_SCCB; H_PDI; H_* ... ✗ */

#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ✗ */
#include "duf_sql_field.h"                                           /* __duf_sql_str_by_name2 for DUF_GET_QUFIELD2 etc. ✗ */

#include "duf_sql_bind.h"                                            /* duf_sql_... for DUF_SQL_BIND_... etc. ✗ */
#include "duf_sql_prepared.h"                                        /* duf_sql_prepare; duf_sql_step; duf_sql_finalize; ✗ */

#include "sql_beginning_tables.h"                                    /* DUF_SQL_TABLES... etc. ✗ */

#include "duf_pdi_structs.h"
#include "duf_mod_types.h"
/* ########################################################################################## */
/* static int duf_dirent_content2( duf_depthinfo_t * H_PDI, duf_sccb_handle_t * sccbh MAS_UNUSED ); */
static DR( MOD, dirent_content2, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh );

/* ########################################################################################## */
#define FILTER_DATA  "fd.mimeid IS NULL"
static duf_sql_sequence_t final_sql =                                /* */
{
  .name = "final-mime",
  .done = 0,
  .sql = {
          "UPDATE " DUF_SQL_TABLES_MIME_FULL " SET dupmimecnt=(SELECT COUNT(*) " /* */
          " FROM  " DUF_SQL_TABLES_MIME_FULL "      AS mi "          /* */
          " JOIN  " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fd.mimeid=mi." DUF_SQL_IDFIELD ") " /* */
          " WHERE " DUF_SQL_TABLES_MIME_FULL "." DUF_SQL_IDFIELD "=mi." DUF_SQL_IDFIELD ")" /* */
        /* " WHERE " DUF_SQL_TABLES_MIME_FULL ".mime=mi.mime)" (* *) */
          ,

          NULL,
          }
};

/* ########################################################################################## */
#define Q_J " LEFT JOIN "
#define Q_FROM( _t, _a ) " FROM " DUF_DBPREF # _t " AS " # _a
#define Q_JOIN_ID( _up, _t, _as, _o )  " LEFT JOIN " DUF_DBPREF # _t " AS " # _as " ON ( " # _up "." # _o " = " # _as "." DUF_SQL_IDFIELD " ) "
#define Q_JOIN_SYN( _up, _t, _as, _o ) " LEFT JOIN " DUF_DBPREF # _t " AS " # _as " ON ( " # _up "." # _o " = " # _as "." # _o ")"

static duf_scan_callbacks_t duf_sccb_dispatch;

const duf_mod_handler_t duf_mod_handler_uni[] = {
  {"sccb", &duf_sccb_dispatch},
  {NULL, NULL}
};

/* ########################################################################################## */
static duf_scanner_set_t scanners[] = {
  {
   .flags = DUF_SCANNER_SET_FLAG_TO_OPEN | DUF_SCANNER_SET_FLAG_DB,  /* */
   .type = DUF_NODE_LEAF,                                            /* */
   .scanstage = DUF_SCANSTAGE_DB_LEAVES,                             /* */
   .fun = F2ND( dirent_content2 ),                                   /* */
   },

  {.fun = NULL}
};

static duf_scan_callbacks_t duf_sccb_dispatch = {
  .title = "collect mime",
  .name = "mime",
  .def_opendir = 1,

#if 0
  .leaf_scan_fd2 = F2ND( dirent_content2 ),
#else
  .scanners = scanners,
#endif

/* TODO : explain values of use_std_leaf_set_num and use_std_node_set_num TODO */
  .use_std_leaf_set_num = -1,                                        /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node_set_num = 2,                                         /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .std_leaf_set_name = "std-leaf-no-sel-fd",
  .std_node_set_name = "std-node-two",
/* filename for debug only */
  .leaf = {
           .name = "mime-leaf",
           .type = DUF_NODE_LEAF,
           .fieldset =                                               /* */
           "#mime",
           .selector2 =                                              /* */
#if 1
           "#std-ns-fd-leaf"                                         /* */
#else
           " FROM      " DUF_SQL_TABLES_FILENAMES_FULL " AS fn "     /* */
         /* Q_FROM( filenames, fn ) (* *) */
           " LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON ( fn.dataid = fd." DUF_SQL_IDFIELD " ) " /* */
           " LEFT JOIN " DUF_SQL_TABLES_MIME_FULL "      AS mi ON ( fd.mimeid = mi." DUF_SQL_IDFIELD " ) " /* */
         /* Q_JOIN_ID( fd, mime, mi, mimeid) */
           " LEFT JOIN " DUF_SQL_TABLES_SIZES_FULL "     AS sz ON ( sz.size   = fd.size               ) " /* */
         /* Q_JOIN_SYN( fd, sizes, sz, size ) (* *) */
#endif
           ,
           .matcher = " fn.Pathid = :parentdirID "                   /* */
           ,                                                         /* */
           .filter =                                                 /* */
           "( " FILTER_DATA " OR mi.mime IS NULL ) " /* */ " AND "   /* */
           "( sz.size   IS NULL OR sz.size > 0 ) " /*     */ " AND " /* */
           " 1 "                                                     /* */
           ,
           .count_aggregate = "DISTINCT fd." DUF_SQL_IDFIELD},
  .node = {
           .name = "mime-node",
           .type = DUF_NODE_NODE,
           .expand_sql = 1,                                          /* */
           .fieldset =                                               /* */
         /* "'mime-node' AS fieldset_id, " (* *) */
           "pt." DUF_SQL_IDFIELD " AS dirid"                         /* */
           ", pt." DUF_SQL_IDFIELD " AS nameid "                     /* */
           ", pt." DUF_SQL_DIRNAMEFIELD " AS dname, pt." DUF_SQL_DIRNAMEFIELD " AS dfname, pt.parentid " /* */
           ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks, STRFTIME( '%s', pt.mtim ) AS mtime " /* */
           ,
           .selector2 =                                              /* */
           " FROM      " DUF_SQL_TABLES_PATHS_FULL "             AS pt " /* */
           ,
           .matcher = "pt.ParentId = :parentdirID  AND ( :dirName IS NULL OR dname=:dirName )" /* */
           ,                                                         /* */
           },
  .final_sql_seq = &final_sql,
};

/* ########################################################################################## */

static
SRP( MOD, unsigned long long, mimeid, 0, insert_mime_uni, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh, const char *mime,
     const char *chs MAS_UNUSED, const char *tail MAS_UNUSED, int need_id )
{
  if ( mime )
  {
    int changes = 0;

    if ( need_id )
    {
      const char *sql = "SELECT " DUF_SQL_IDFIELD " AS mimeId FROM " DUF_SQL_TABLES_MIME_FULL " WHERE mime = :Mime " /* " AND charset=:charSet" */ ;

      DUF_SQL_SE_START_STMT( H_PDI, select_mime, sql, pstmt_local );
      DUF_SQL_SE_BIND_S( Mime, mime, pstmt_local );
    /* DUF_SQL_SE_BIND_S( charSet, chs,  pstmt_local ); */
    /* DUF_SQL_SE_BIND_S( Tail, tail,  pstmt_local ); */
      DUF_SQL_SE_STEP( pstmt_local );
      if ( QISERR1_N( SQL_ROW ) )
      {
        ERRCLEAR1( SQL_ROW );
        MAST_TRACE( mod, 0, "<selected>" );
        mimeid = DUF_GET_QUFIELD3( pstmt_local, mimeId );
      }
      DUF_SQL_SE_END_STMT( H_PDI, select_mime, pstmt_local );          /* clears SQL_ROW / SQL_DONE */
    }

    if ( !mimeid && !duf_get_config_flag_disable_insert(  ) )
    {
      const char *sql = "INSERT OR IGNORE INTO " DUF_SQL_TABLES_MIME_FULL " ( mime ) VALUES ( :Mime )";

    /* "INSERT OR IGNORE INTO " DUF_SQL_TABLES_MIME_FULL " ( mime, charset, tail ) VALUES (:Mime, :charSet, :Tail )"; */

      DUF_SQL_SE_START_STMT( H_PDI, insert_mime, sql, pstmt_insert );
      MAST_TRACE( mod, 3, " S: %s ", sql );
      DUF_SQL_SE_BIND_S( Mime, mime, pstmt_insert );
    /* DUF_SQL_SE_BIND_S( charSet, chs,  pstmt_insert ); */
    /* DUF_SQL_SE_BIND_S( Tail, tail,  pstmt_insert ); */
      DUF_SQL_SE_STEPC( pstmt_insert );
      DUF_SQL_SE_CHANGES( H_PDI, changes, pstmt_insert );
    /* DUF_SHOW_ERROR( "changes:%d", changes ); */
      if ( need_id && changes )
      {
        mimeid = duf_sql_last_insert_rowid(  );
        MAST_TRACE( mime, 0, " inserted now( SQLITE_OK ) mimeid = %llu ", mimeid );
        assert( mimeid );
      }
      DUF_SQL_SE_END_STMT( H_PDI, insert_mime, pstmt_insert );         /* clears SQL_ROW / SQL_DONE */
    }
  }
  else
  {
  /* DUF_SHOW_ERROR( " Wrong data " ); */
    ERRMAKE( DATA );
  }
  ERP( MOD, unsigned long long, mimeid, 0, insert_mime_uni, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh, const char *mime,
       const char *chs MAS_UNUSED, const char *tail MAS_UNUSED, int need_id );
}

static void
mime_destructor( void *ctx )
{
  magic_t magic = ( magic_t ) ctx;

  magic_close( magic );
  MAST_TRACE( mime, 0, " closed mime " );
}

static
SR( MOD, dirent_content2, duf_depthinfo_t * pdi_unused MAS_UNUSED, duf_sccb_handle_t * sccbh MAS_UNUSED )
{
  unsigned long long mimeid = 0;

  MAST_TRACE( mod, 0, " mime" );

  if ( QNOERR )
  {
    char *mime = NULL;
    char *mime_plus = NULL;

    magic_t magic = NULL;

#if 0
    magic = ( magic_t ) duf_levinfo_context_up( H_PDI );
#else
    magic = ( magic_t ) duf_pdi_context( H_PDI );
#endif

    if ( !magic )
    {
      magic = magic_open(  /* MAGIC_MIME | */ MAGIC_PRESERVE_ATIME );
      MAST_TRACE( mime, 0, " opened mime %s ", magic ? " OK " : " FAIL " );
#if 0
      duf_levinfo_set_context_up( H_PDI, magic );
      duf_levinfo_set_context_up_destructor( H_PDI, mime_destructor );
#else
      duf_pdi_set_context( H_PDI, magic );
      duf_pdi_set_context_destructor( H_PDI, mime_destructor );
#endif
    }
    if ( QNOERR )
    /* ry = */ magic_load( magic, NULL );

    {
    /* magic_setflags(magic,MAGIC_MIME_TYPE ); */
    /* magic_setflags(magic, MAGIC_MIME ); */
    }

    magic_setflags( magic, MAGIC_PRESERVE_ATIME );
    mime_plus = mas_strdup( magic_descriptor( magic, duf_levinfo_dfd( H_PDI ) ) );
  /* MAST_TRACE( mime, 0, " opened mime %s : %s ---", magic ? " OK " : " FAIL ", mime_plus ); */

    magic_setflags( magic, MAGIC_MIME | MAGIC_PRESERVE_ATIME );
    mime = mas_strdup( magic_descriptor( magic, duf_levinfo_dfd( H_PDI ) ) );

    MAST_TRACE( mime, 0, " opened mime %s : %s :: %s ---%s/%s", magic ? " OK " : " FAIL ", mime, mime_plus, duf_levinfo_path( H_PDI ),
                DUF_GET_RSFIELD2( fname ) );

    if ( mime )
    {
      const char *p = strchr( mime, ';' );

      if ( p )
      {
        char *mimet = NULL;
        char *charset = NULL;
        char *tail = NULL;

        mimet = mas_strndup( mime, p - mime );
        p++;
        while ( p && *p && *p <= ' ' )
          p++;
        if ( 0 == strncmp( p, " charset = ", 8 ) )
          charset = mas_strdup( p + 8 );
        else
          tail = mas_strdup( p );
        mimeid = CRP( insert_mime_uni, H_PDI, sccbh, mimet, charset, tail, 1 /*need_id */  );

        if ( QNOERR && mimeid && !duf_get_config_flag_disable_update(  ) )
        {
          int changes = 0;

          DUF_RUFIELD2( filedataid );

          const char *sql = " UPDATE " DUF_SQL_TABLES_FILEDATAS_FULL " SET mimeid = :mimeID WHERE " DUF_SQL_IDFIELD " = :dataID ";

          DUF_SQL_SE_START_STMT( H_PDI, update_mime, sql, pstmt_update );
          MAST_TRACE( mod, 3, " S: %s ", sql );
          DUF_SQL_SE_BIND_LL( mimeID, mimeid, pstmt_update );
          DUF_SQL_SE_BIND_LL( dataID, filedataid, pstmt_update );
          DUF_SQL_SE_STEPC( pstmt_update );

          DUF_SQL_SE_CHANGES( H_PDI, changes, pstmt_update );
          DUF_SQL_SE_END_STMT( H_PDI, update_mime, pstmt_update );     /* clears SQL_ROW / SQL_DONE */

          duf_pdi_reg_changes( H_PDI, changes );

        }
        mas_free( mimet );
        mas_free( charset );
        mas_free( tail );
      }
    /* MAST_TRACE( mod, 12, " " DUF_DEPTH_PFMT ": scan 5: %llu ", duf_pdi_depth( H_PDI ), mimeid ); */
    }
    mas_free( mime );
    mas_free( mime_plus );
  }
  H_PDI->total_files++;

  ER( MOD, dirent_content2, duf_depthinfo_t * pdi_unused, duf_sccb_handle_t * sccbh );
}
