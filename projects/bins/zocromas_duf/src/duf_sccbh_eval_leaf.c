/* #undef MAS_TRACING */
#include <assert.h>
#include <string.h>
#include <stddef.h>

#include "duf_tracen_defs_preset.h"                                  /* MAST_TRACE_CONFIG; etc. ✗ */
#include "duf_errorn_defs_preset.h"                                  /* MAST_ERRORS_FILE; etc. ✗ */

#include <mastar/trace/mas_trace.h>
#include <mastar/error/mas_error_defs_ctrl.h>
#include <mastar/error/mas_error_defs_make.h>
#include <mastar/error/mas_error_defs.h>

#include "duf_se_only.h"                                             /* Only DR; SR; ER; CR; QSTR; QERRIND; QERRNAME etc. ✗ */

/* #include "duf_config.h"                                              (* duf_get_config ✗ *) */
#include "duf_config_util.h"                                         /* duf_get_trace_config (for MAST_TRACE_CONFIG at duf_tracen_defs_preset) ✗ */

#include "duf_pdi_filters.h"                                         /* duf_pdi_pu; etc. ✗ */

#include "duf_levinfo_ref.h"                                         /* duf_levinfo_*; etc. ✗ */
#include "duf_levinfo_openclose.h"
#include "duf_levinfo_stat.h"

#include "duf_nodetype.h"                                            /* duf_nodetype_name ✗ */

/* #include "duf_sccb.h" */
#include "duf_sccb_ref.h"
#include "duf_sccb_structs.h"
#include "duf_sccb_scanstage.h"                                      /* duf_scanstage_name; duf_scanstage_scanner; ✗ */
#include "duf_sccb_row.h"                                            /* datarow_* ✗ */

#include "duf_sccbh_ref.h"
#include "duf_sccbh_structs.h"                                       /* duf_sccb_handle_s (from duf_sccbh_types: duf_sccb_handle_t; duf_sccbh_fun_t; duf_rsccbh_fun_t) ✗ */
#include "duf_sccbh_shortcuts.h"                                     /* H_SCCB; H_PDI; H_* ... ✗ */
#include "duf_sccbh_scanner.h"

/* #include "duf_sccbh_eval.h" */
/* ###################################################################### */
#include "duf_sccbh_eval_leaf.h"                                     /* duf_sccbh_eval_db_leaf_str_cb; duf_sccbh_eval_db_leaf_fd_str_cb; ✗ */
/* ###################################################################### */

static
SR( SCCBH, sccbh_eval_db_leaf_qfd, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_unused  MAS_UNUSED, */ duf_scanstage_t scanstage,
    unsigned to_open )
{
#if 0
  H_PDI->items.total++;
  H_PDI->items.files++;
#endif
  if ( to_open )
  {
    ERRLOWER( FS_DISABLED );
    CR( levinfo_if_openat_dh, H_PDI );
    if ( QISERR_N( OPENAT_ENOENT ) )
    {
      ERRCLEAR( OPENAT_ENOENT );
      ERRMAKE_M( OPENAT_ENOENT, "Can't open '%s'; deleted:%d", duf_levinfo_itemshowname( H_PDI ), CRX( levinfo_deleted, H_PDI ) );
      assert( 0 );
    }
    else if ( !CRX( levinfo_deleted, H_PDI ) )
    {
      ERRLOWER( STATAT_ENOENT );
      CR( levinfo_if_statat_dh, H_PDI );
      ERRUPPER( STATAT_ENOENT );
    }
    ERRUPPER( FS_DISABLED );
/* DOR_LOWERE( r, CRX(levinfo_if_openat_dh, H_PDI ), DUF_ERROR_FS_DISABLED );                          */
/* DOR_LOWERE( r, CRX(levinfo_if_statat_dh, H_PDI ), DUF_ERROR_FS_DISABLED, DUF_ERROR_STATAT_ENOENT ); */
#if 1
    if ( CRX( levinfo_deleted, H_PDI ) )
    {
      ERRCLEAR( OPENAT_ENOENT );
      ERRCLEAR( STATAT_ENOENT );
    }
#endif
  }

  for ( H_SCCBIv = 0; H_SCCB; H_SCCBIv++ )
  {
    duf_scanner_fun_t scanner = NULL;

    scanner = CRX( sccb_scanstage_scanner, H_SCCB, scanstage, CRX( levinfo_deleted, H_PDI ), DUF_NODE_LEAF );
  /* assert( !CRX( levinfo_deleted, H_PDI ) ); */
    ERRLOWER( FS_DISABLED );
    {
      CR( sccbh_call_scanner, sccbh, /* pstmt_unused , */ scanstage, scanner, DUF_NODE_LEAF );

    /* assert( sccbh->assert__current_node_type == DUF_NODE_LEAF ); */
    }
    ERRUPPER( FS_DISABLED );
  }
  H_SCCBIv = 0;
  ER( SCCBH, sccbh_eval_db_leaf_qfd, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_unused , */ duf_scanstage_t scanstage, unsigned to_open );
}

static
SR( SCCBH, sccbh_eval_db_leaf_qfd_new, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_unused MAS_UNUSED, */ duf_scanstage_t scanstage )
{
  for ( H_SCCBIv = 0; H_SCCB; H_SCCBIv++ )
  {
    int n = 0;
    int ifpack = 0;
    int packdone = 0;

    ifpack = /* sccbh->scanner_set_flags_special & DUF_SCANNER_SET_FLAG_PACK || */ CRX( sccbh_eval_new_pack, sccbh );
    for ( const duf_scanner_set_t * scanner_set = duf_sccb_scanners( H_SCCB ) /* H_SCCB->scanners */ ; scanner_set && scanner_set->fun;
          scanner_set++ )
    {
      duf_scanner_set_flags_set_t flags;

      flags = scanner_set->flags /* & ~sccbh->scanner_set_flags_mask_off */ ;
#if 0
      QT( "@flags:%llx (%d:%d:%d:%d) %s-%s; %s", flags, ( !sccbh->scanner_set_flags_mask_on || sccbh->scanner_set_flags_mask_on & flags ) ? 1 : 0,
          flags & DUF_SCANNER_SET_FLAG_DB ? 1 : 0, flags & DUF_SCANNER_SET_FLAG_PACK ? 1 : 0, ifpack, duf_scanstage_name( scanner_set->scanstage ),
          duf_scanstage_name( scanstage ), duf_nodetype_name( scanner_set->type ) );
#endif
    /* QT( "@a %s : %d : %s : %s", H_SCCB->name, n, scanner_set->name, duf_scanstage_name( scanstage ) ); */
#if 0
      QT( "@%d:%d:%d:%d:%d:%d",
          ( !sccbh->scanner_set_flags_mask_on || ( sccbh->scanner_set_flags_mask_on & flags ) ) ? 1 : 0,
          ( flags & DUF_SCANNER_SET_FLAG_DB ) ? 1 : 0,
          !( flags & DUF_SCANNER_SET_FLAG_DISABLED ) ? 1 : 0,
          ( ( scanner_set->scanstage & scanstage ) || scanner_set->scanstage == DUF_SCANSTAGE_NONE ) ? 1 : 0,
          ( ( scanner_set->type & DUF_NODE_LEAF ) || scanner_set->type == DUF_NODE_NONE ) ? 1 : 0,
          ( !( flags & DUF_SCANNER_SET_FLAG_PACK ) || ifpack ) ? 1 : 0 );
#endif
      if ( ( flags & DUF_SCANNER_SET_FLAG_DB )
           && !( flags & DUF_SCANNER_SET_FLAG_DISABLED )
           && ( ( scanner_set->scanstage & scanstage ) || scanner_set->scanstage == DUF_SCANSTAGE_NONE )
           && ( ( scanner_set->type & DUF_NODE_LEAF ) || scanner_set->type == DUF_NODE_NONE )
           && ( ifpack /*                    */  || !( flags & DUF_SCANNER_SET_FLAG_PACK ) )
           && ( !sccbh->scanner_set_flags_mask_on || ( flags & sccbh->scanner_set_flags_mask_on ) ) )
      {
#if 0
        if ( sccbh->scanner_set_flags_special & DUF_SCANNER_SET_FLAG_PACK )
          QT( "@flags:%llx", flags );
#endif
        if ( flags & DUF_SCANNER_SET_FLAG_TO_OPEN )
        {
          ERRLOWER( FS_DISABLED );
          CR( levinfo_if_openat_dh, H_PDI );
          {
            ERRLOWER( STATAT_ENOENT );
            CR( levinfo_if_statat_dh, H_PDI );
            ERRUPPER( STATAT_ENOENT );
          }
          ERRUPPER( FS_DISABLED );
#if 1
          if ( CRX( levinfo_deleted, H_PDI ) )
          {
            ERRCLEAR( OPENAT_ENOENT );
            ERRCLEAR( STATAT_ENOENT );
          }
#endif
        }
      /* if ( flags & DUF_SCANNER_SET_FLAG_PACK ) */
      /* {                                                     */
      /*   assert( 0 );                                        */
      /* }                                                     */
        ERRLOWER( FS_DISABLED );
        CR( sccbh_call_scanner, sccbh, /* pstmt_unused , */ scanstage, scanner_set->fun, DUF_NODE_LEAF );
        if ( flags & DUF_SCANNER_SET_FLAG_PACK )
          packdone++;
        ERRUPPER( FS_DISABLED );
      /* sleep( 1 ); */
      }
      else
      {
#if 0
        QT( "@b %d :  %d %d %d(%s:%s) %d", n, ( scanner_set->flags & DUF_SCANNER_SET_FLAG_DB ) ? 1 : 0, /* */
            ( !( scanner_set->flags & DUF_SCANNER_SET_FLAG_DISABLED ) ) ? 1 : 0, /* */
            ( ( scanner_set->scanstage & scanstage ) || scanner_set->scanstage == DUF_SCANSTAGE_NONE ) ? 1 : 0, /* */
            duf_scanstage_name( scanner_set->scanstage ),            /* */
            duf_scanstage_name( scanstage ),                         /* */
            ( ( scanner_set->type & DUF_NODE_LEAF ) || scanner_set->type == DUF_NODE_NONE ) ? 1 : 0 );
#endif
      }
      n++;
#if 0
      if ( sccbh->scanner_set_flags_special & DUF_SCANNER_SET_FLAG_PACK )
        QT( "@flags:%llx", flags );
#endif
    }
    if ( ifpack && packdone && CRX( datarow_list_penult, CRX( sccbh_rowlist, sccbh ) ) /* sccbh->rowlist.last && sccbh->rowlist.last->prev */  )
    {
#if 0
      CRX( datarow_list_delete_f, sccbh->rowlist.last->prev, 0 );
      sccbh->rowlist.last->prev = NULL;
#else
      CRX( datarow_list_delete_f, CRX( sccbh_rowlist_p, sccbh ) /* &sccbh->rowlist */ , 1, 0 );
      assert( CRX( datarow_list_last, CRX( sccbh_rowlist, sccbh ) ) /* sccbh->rowlist.last */  );
      assert( !CRX( datarow_list_penult, CRX( sccbh_rowlist, sccbh ) ) /* sccbh->rowlist.last->prev */  );
#endif
    }
  }
/* (* 20160415.171104 FIXME to make last pack evaluated need NULL-row !!! *) XXX XXX */
  H_SCCBIv = 0;
  ER( SCCBH, sccbh_eval_db_leaf_qfd_new, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_unused, */ duf_scanstage_t scanstage );
}

/* 20151027.144543 */
/*
 * this is callback of type: duf_str_cb_t (first range; str_cb)
 *
 * duf_str_cb_scan_file_fd is just a wrapper for sccb->leaf_scan_fd
 *
 *  - pstmt_x - for 1 leaf data, obtained from db
 *  - pdi
 *  - sccb
 * */

SR( SCCBH, sccbh_eval_db_leaf_fd_str_cb, duf_sccb_handle_t * sccbh,  /* duf_stmnt_t * pstmt_unused MAS_UNUSED, */
    duf_scanstage_t scanstage )
{
#if 0
# if 0
  H_PDI->items.total++;
  H_PDI->items.files++;
# endif
  {
    ERRLOWER( FS_DISABLED );
    CR( levinfo_if_openat_dh, H_PDI );
    {
      ERRLOWER( STATAT_ENOENT );
      CR( levinfo_if_statat_dh, H_PDI );
      ERRUPPER( STATAT_ENOENT );
    }
    ERRUPPER( FS_DISABLED );
  }
/* DOR_LOWERE( r, CRX(levinfo_if_openat_dh, H_PDI ), DUF_ERROR_FS_DISABLED );                          */
/* DOR_LOWERE( r, CRX(levinfo_if_statat_dh, H_PDI ), DUF_ERROR_FS_DISABLED, DUF_ERROR_STATAT_ENOENT ); */
# if 1
  if ( CRX( levinfo_deleted, H_PDI ) )
  {
    ERRCLEAR( OPENAT_ENOENT );
    ERRCLEAR( STATAT_ENOENT );
  }
# endif
  for ( H_SCCBIv = 0; H_SCCB; H_SCCBIv++ )
  {
    duf_scanner_fun_t scanner = NULL;

    scanner = CRX( sccb_scanstage_scanner, H_SCCB, scanstage, CRX( levinfo_deleted, H_PDI ), DUF_NODE_LEAF );
    {
      ERRLOWER( FS_DISABLED );
      CR( sccbh_call_scanner, sccbh, /* pstmt_unused, */ scanstage, scanner, DUF_NODE_LEAF );
      assert( sccbh->assert__current_node_type == DUF_NODE_LEAF );
      ERRUPPER( FS_DISABLED );
    }
  }
  H_SCCBIv = 0;
#else
  CR( sccbh_eval_db_leaf_qfd, sccbh, /* pstmt_unused, */ scanstage, 1 );
#endif
  ER( SCCBH, sccbh_eval_db_leaf_fd_str_cb, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_unused, */
      duf_scanstage_t scanstage );
}

/*20150820.085324
 * this is callback of type: duf_str_cb_t (first range; str_cb)
 *
 * duf_str_cb_leaf_scan is just a wrapper for sccb->leaf_scan
 *
 *  - pstmt_x - for 1 leaf data, obtained from db
 *  - pdi
 *  - sccb
 * */
SR( SCCBH, sccbh_eval_db_leaf_str_cb, duf_sccb_handle_t * sccbh,     /* duf_stmnt_t * pstmt_unused MAS_UNUSED, */
    duf_scanstage_t scanstage )
{
#if 0
# if 0
  H_PDI->items.total++;
  H_PDI->items.files++;
# endif
  for ( H_SCCBIv = 0; H_SCCB; H_SCCBIv++ )
  {
    duf_scanner_fun_t scanner = NULL;

    scanner = CRX( sccb_scanstage_scanner, H_SCCB, scanstage, CRX( levinfo_deleted, H_PDI ), DUF_NODE_LEAF );
    {
      ERRLOWER( FS_DISABLED );
      CR( sccbh_call_scanner, sccbh, /* pstmt_unused, */ scanstage, scanner, DUF_NODE_LEAF );
      ERRUPPER( FS_DISABLED );
    }
  }
  H_SCCBIv = 0;
#else
  CR( sccbh_eval_db_leaf_qfd, sccbh, /* pstmt_unused, */ scanstage, 0 );
#endif
  ER( SCCBH, sccbh_eval_db_leaf_str_cb, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_unused, */ duf_scanstage_t scanstage );
}

SR( SCCBH, sccbh_eval_db_leaf_str_cb_new, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_unused MAS_UNUSED, */
    duf_scanstage_t scanstage )
{
  CR( sccbh_eval_db_leaf_qfd_new, sccbh, /* pstmt_unused, */ scanstage );
  ER( SCCBH, sccbh_eval_db_leaf_str_cb_new, duf_sccb_handle_t * sccbh, /* duf_stmnt_t * pstmt_unused, */
      duf_scanstage_t scanstage );
}

SRX( SCCBH, int, np, 0, sccbh_eval_new_pack, duf_sccb_handle_t * sccbh )
{
  int eq = 0;
  duf_sccb_data_row_t *last_row MAS_UNUSED = NULL;
  duf_sccb_data_row_t *prenult_row MAS_UNUSED = NULL;

  last_row = CRX( datarow_list_last, CRX( sccbh_rowlist, sccbh ) );
  prenult_row = CRX( datarow_list_penult, CRX( sccbh_rowlist, sccbh ) );
  if ( prenult_row )
  {
    const char *pack_field;

    pack_field = CRX( pdi_pack_field, H_PDI );
    if ( QNOERR )
    {
      duf_sqltype_t t;

      t = CRP( datarow_get_type, last_row, pack_field );
      if ( QNOERR )
        switch ( t )
        {
        case DUF_SQLTYPE_NONE:
          eq = 0;
          break;
        case DUF_SQLTYPE_INTEGER:
          {
            unsigned long long number0;
            unsigned long long number1;

            number0 = CRP( datarow_get_number, last_row, pack_field );
            number1 = CRP( datarow_get_number, prenult_row, pack_field );
            eq = ( number0 == number1 );
            if ( !eq )
              MAST_TRACE( temp, 5, "@@---A %lld ? %lld : %p:%p", number0, number1, last_row, prenult_row );
          }
          break;
        case DUF_SQLTYPE_FLOAT:
          assert( 0 );
          break;
        case DUF_SQLTYPE_TEXT:
          {
            const char *str0;
            const char *str1;

            str0 = CRP( datarow_get_string, last_row, pack_field );
            str1 = CRP( datarow_get_string, prenult_row, pack_field );
            eq = ( ( !str0 && !str1 ) || ( str0 && str1 && 0 == strcmp( str0, str1 ) ) );
            if ( !eq )
              MAST_TRACE( temp, 5, "@@---A %s ? %s : %p:%p", str0, str1, last_row, prenult_row );
          }
          break;
        case DUF_SQLTYPE_BLOB:
          assert( 0 );
          break;
        case DUF_SQLTYPE_NULL:
          {
            int isnull0;
            int isnull1;

            isnull0 = CRP( datarow_get_null, last_row, pack_field );
            isnull1 = CRP( datarow_get_null, prenult_row, pack_field );
            eq = ( isnull0 && isnull1 );
            if ( !eq )
              MAST_TRACE( temp, 5, "@@---A %d ? %d : %p:%p", isnull0, isnull1, last_row, prenult_row );
          }
          break;
        }
    }
    ERRCLEAR( NO_FIELD );                                            /* possible absence is not error now */
  }
  else
  {
  /* assert( 0 ); */
  }
  np = !eq && prenult_row;
  ERX( SCCBH, int, np, 0, sccbh_eval_new_pack, duf_sccb_handle_t * sccbh );
}

#if 0
SR( SCCBH, sccbh_eval_db_leaf_qfd_pack_new, duf_sccb_handle_t * sccbh, duf_scanstage_t scanstage MAS_UNUSED )
{
/* assert(  !sccbh->rowlist.last->nfields ||  duf_levinfo_node_type( H_PDI ) == DUF_NODE_LEAF ); */
/* CRX( sccbh_preset_leaf_scanner, sccbh ); */
  if ( sccbh && ( CRX( sccbh_eval_new_pack, sccbh ) ) )
  {
    CR( sccbh_eval_db_leaf_qfd_new, sccbh, /* pstmt_unused, */ scanstage );
    assert( duf_sccbh_current_row( sccbh ) == NULL );
    if ( sccbh->rowlist.last && sccbh->rowlist.last->prev )
    {
      CRX( datarow_list_delete_f, sccbh->rowlist.last->prev, 0 );
      sccbh->rowlist.last->prev = NULL;
    }

  /* assert(0); */

  /* CRX( sccbh_rows_eval, sccbh ); */
  }
  ER( SCCBH, sccbh_eval_db_leaf_qfd_pack_new, duf_sccb_handle_t * sccbh, duf_scanstage_t scanstage );
}
#endif
