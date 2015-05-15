#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_maintenance.h"

#include "duf_service.h"
#include "duf_config_ref.h"

#include "duf_pdi.h"
#include "duf_levinfo.h"
#include "duf_levinfo_ref.h"

#include "duf_sql_defs.h"

#include "duf_sql_field.h"
/* #include "duf_sql_field1.h" */

#include "duf_dbg.h"

#include "sql_beginning_selected.h"
#include "sql_beginning_tables.h"


/* ###################################################################### */
/* #include "duf_sample_uni.h" */
/* ###################################################################### */



/* callback of type duf_scan_hook_file_t */
/* static int                                                                                                       */
/* sample_scan_leaf( duf_depthinfo_t * pdi, duf_record_t * precord )                                                */
/* {                                                                                                                */
  /* DEBUG_STARTR( r ); */
/*                                                                                                                  */
/*   DUF_SFIELD( filename );                                                                                        */
/*                                                                                                                  */
/*   DEBUG_START(  );                                                                                               */
/* (* stat *)                                                                                                       */
/*                                                                                                                  */
/*   (* SQL at duf_file_pathid.c : duf_scan_fil_by_pi *)                                                            */
/*                                                                                                                  */
/*   (*                                                                                                             */
/*    * --uni-scan   -R   --sample   --files   -FF                                                                  */
/*    *                   ^^^^^^^^   ^^^^^^^                                                                        */
/*    * *)                                                                                                          */
/*                                                                                                                  */
/*                                                                                                                  */
/*                                                                                                                  */
/*                                                                                                                  */
/*   DUF_TRACE( sample, 2, "sample" );                                                                              */
/*                                                                                                                  */
/*   if ( pdi->depth <= 0 )                                                                                         */
/*   {                                                                                                              */
/*     DUF_SHOW_ERROR( "depth should not be %d at this point", pdi->depth );                                             */
/*     assert( pdi->depth > 0 );                                                                                    */
/*   }                                                                                                              */
/*   {                                                                                                              */
/*     const char *fpath;                                                                                           */
/*                                                                                                                  */
/*     DUF_SFIELD( filename );                                                                                      */
/*                                                                                                                  */
/*     fpath = duf_levinfo_path( pdi );                                                                             */
/*     DUF_TRACE( sample, 0, "@@@@@@@@@ (F%d) '%s'/'%s'", pdi->depth, fpath, filename );                            */
/*     DUF_PRINTF( 1, "#%4llu: L%2u%12s%s%s", pdi->seq, duf_pdi_depth( pdi ), "", fpath ? fpath : "?/", filename ); */
/*   }                                                                                                              */
/*                                                                                                                  */
/*   DUF_TRACE( sample, 2, "filename=%s", filename );                                                               */
/*   DEBUG_ENDR( r );                                                                                               */
/* }                                                                                                                */

static int
sample_scan_leaf2( duf_sqlite_stmt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_SFIELD2( filename );

  assert( 0 == strcmp( filename, duf_levinfo_itemname( pdi ) ) );
/* stat */

  /* SQL at duf_file_pathid.c : duf_scan_fil_by_pi */

  /* 
   * --uni-scan   -R   --sample   --files   -FF
   *                   ^^^^^^^^   ^^^^^^^
   * */




  DUF_TRACE( sample, 2, "sample" );

  if ( pdi->depth <= 0 )
  {
    DUF_SHOW_ERROR( "depth should not be %d at this point", pdi->depth );
    assert( pdi->depth > 0 );
  }
  {
    const char *fpath;

    DUF_SFIELD2( filename );

    fpath = duf_levinfo_path( pdi );
    DUF_TRACE( sample, 0, "@@@@@@@@@ (F%d) '%s'/'%s'", pdi->depth, fpath, filename );
    DUF_PRINTF( 1, "#%4llu: L%2u%12s%s%s", pdi->seq, duf_pdi_depth( pdi ), "", fpath ? fpath : "?/", filename );
  }

  DUF_TRACE( sample, 2, "filename=%s", filename );
  DEBUG_ENDR( r );
}

/* 
 * this is callback of type: duf_scan_hook_dir_t
 * */
/* will be static! */
/* int                                                                                                          */
/* sample_scan_node_before( unsigned long long pathid_unused, duf_depthinfo_t * pdi, duf_record_t * precord )   */
/* {                                                                                                            */
  /* DEBUG_STARTR( r ); */
/*   unsigned long long dirid = duf_levinfo_dirid( pdi );                                                       */
/*                                                                                                              */
/*   DEBUG_START(  );                                                                                           */
/*                                                                                                              */
/*                                                                                                              */
/*   {                                                                                                          */
/*     const char *path = duf_levinfo_path( pdi );                                                              */
/*                                                                                                              */
/*     DUF_PRINTF( 2, "#%4llu: " DUF_DEPTH_PFMT "+ " DUF_SQL_IDNAME "%-7llu %s", pdi->seq, duf_pdi_depth( pdi ), dirid, path ); */
/*   }                                                                                                          */
/*   {                                                                                                          */
/*     void *ctx = duf_levinfo_context( pdi );                                                                  */
/*                                                                                                              */
/*     if ( !ctx )                                                                                              */
/*     {                                                                                                        */
/*       char *test = mas_strdup( "BEFORE" );                                                                   */
/*                                                                                                              */
/*       duf_levinfo_set_context( pdi, test );                                                                  */
/*     }                                                                                                        */
/*   }                                                                                                          */
/*   DEBUG_ENDR( r );                                                                                           */
/* }                                                                                                            */

static int
sample_scan_node_before2( duf_sqlite_stmt_t * pstmt_unused, /* unsigned long long pathid_unused, */ duf_depthinfo_t * pdi )
{
  unsigned long long dirid = duf_levinfo_dirid( pdi );

  DEBUG_STARTR( r );


  {
    const char *path = duf_levinfo_path( pdi );

    DUF_PRINTF( 2, "#%4llu: " DUF_DEPTH_PFMT "+ " DUF_SQL_IDNAME "%-7llu %s", pdi->seq, duf_pdi_depth( pdi ), dirid, path );
  }
  {
    void *ctx = duf_levinfo_context( pdi );

    if ( !ctx )
    {
      char *test = mas_strdup( "BEFORE" );

      duf_levinfo_set_context( pdi, test );
    }
  }
  DEBUG_ENDR( r );
}


/* static int                                                                                                   */
/* sample_scan_node_after( unsigned long long pathid_unused, duf_depthinfo_t * pdi, duf_record_t * precord )    */
/* {                                                                                                            */
  /* DEBUG_STARTR( r ); */
/*                                                                                                              */
/*                                                                                                              */
/*   {                                                                                                          */
/*     unsigned long long dirid = duf_levinfo_dirid( pdi );                                                     */
/*     const char *path = duf_levinfo_path( pdi );                                                              */
/*                                                                                                              */
/*     DUF_PRINTF( 4, "#%4llu: " DUF_DEPTH_PFMT "- " DUF_SQL_IDNAME "%-7llu %s", pdi->seq, duf_pdi_depth( pdi ), dirid, path ); */
/*   }                                                                                                          */
/*   {                                                                                                          */
/*     void *ctx = duf_levinfo_context( pdi );                                                                  */
/*                                                                                                              */
/*     if ( !ctx || 0 != strcmp( ( char * ) ctx, "MIDDLE" ) )                                                   */
/*       DUF_SHOW_ERROR( "sample context %s", ( char * ) ctx );                                                      */
/*     DUF_TRACE( sample, 0, "(%p) context=%p", ( void * ) pdi, ctx );                                          */
/*     duf_levinfo_set_context( pdi, NULL );                                                                    */
/*   }                                                                                                          */
/*   DEBUG_ENDR( r );                                                                                           */
/* }                                                                                                            */

static int
sample_scan_node_after2( duf_sqlite_stmt_t * pstmt_unused, /* unsigned long long pathid_unused, */ duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  {
    unsigned long long dirid = duf_levinfo_dirid( pdi );
    const char *path = duf_levinfo_path( pdi );

    DUF_PRINTF( 4, "#%4llu: " DUF_DEPTH_PFMT "- " DUF_SQL_IDNAME "%-7llu %s", pdi->seq, duf_pdi_depth( pdi ), dirid, path );
  }
  {
    void *ctx = duf_levinfo_context( pdi );

    if ( !ctx || 0 != strcmp( ( char * ) ctx, "MIDDLE" ) )
      DUF_SHOW_ERROR( "sample context %s", ( char * ) ctx );
    DUF_TRACE( sample, 0, "(%p) context=%p", ( void * ) pdi, ctx );
    duf_levinfo_set_context( pdi, NULL );
  }
  DEBUG_ENDR( r );
}

/* static int                                                                                                 */
/* sample_scan_node_middle( unsigned long long pathid_unused, duf_depthinfo_t * pdi, duf_record_t * precord ) */
/* {                                                                                                          */
  /* DEBUG_STARTR( r ); */
/*   unsigned long long dirid = duf_levinfo_dirid( pdi );                                                     */
/*                                                                                                            */
/*   DEBUG_START(  );                                                                                         */
/*                                                                                                            */
/*   DUF_TRACE( sample, 2, "T2 dirid=%llu", dirid );                                                          */
/*   {                                                                                                        */
/*     duf_config->cli.trace.sql--;                                                                           */
/*     {                                                                                                      */
/*       const char *real_path = NULL;                                                                        */
/*                                                                                                            */
/*       if ( !real_path )                                                                                    */
/*         real_path = duf_levinfo_path( pdi );                                                               */
/*       (* char *path = duf_pathid_to_path_s( dirid, pdi, &r ); *)                                           */
/*                                                                                                            */
/*       DUF_PRINTF( 5, "#%4llu: sample MIDDLE dPATH %s", pdi->seq, real_path );                              */
/*       DUF_TRACE( sample, 1, "path=%s", real_path );                                                        */
/*       (* mas_free( path ); *)                                                                              */
/*     }                                                                                                      */
/*     duf_config->cli.trace.sql++;                                                                           */
/*   }                                                                                                        */
/*   DUF_TRACE( sample, 2, "T2 dirid=%llu", dirid );                                                          */
/*                                                                                                            */
/*   {                                                                                                        */
/*     void *ctx = duf_levinfo_context( pdi );                                                                */
/*                                                                                                            */
/*     if ( !ctx || 0 != strcmp( ( char * ) ctx, "BEFORE" ) )                                                 */
/*       DUF_SHOW_ERROR( "sample context %s", ( char * ) ctx );                                                    */
/*                                                                                                            */
/*     duf_levinfo_set_context( pdi, NULL );                                                                  */
/*     ctx = duf_levinfo_context( pdi );                                                                      */
/*     if ( !ctx )                                                                                            */
/*     {                                                                                                      */
/*       char *test = mas_strdup( "MIDDLE" );                                                                 */
/*                                                                                                            */
/*       duf_levinfo_set_context( pdi, test );                                                                */
/*     }                                                                                                      */
/*   }                                                                                                        */
/*                                                                                                            */
/*   DEBUG_ENDR( r );                                                                                         */
/* }                                                                                                          */

static int
sample_scan_node_middle2( duf_sqlite_stmt_t * pstmt_unused, /* unsigned long long pathid_unused, */ duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  unsigned long long dirid = duf_levinfo_dirid( pdi );

  DUF_TRACE( sample, 2, "T2 dirid=%llu", dirid );
  {
    duf_config->cli.trace.sql--;
    {
      const char *real_path = NULL;

      if ( !real_path )
        real_path = duf_levinfo_path( pdi );
      /* char *path = duf_pathid_to_path_s( dirid, pdi, &r ); */

      DUF_PRINTF( 5, "#%4llu: sample MIDDLE dPATH %s", pdi->seq, real_path );
      DUF_TRACE( sample, 1, "path=%s", real_path );
      /* mas_free( path ); */
    }
    duf_config->cli.trace.sql++;
  }
  DUF_TRACE( sample, 2, "T2 dirid=%llu", dirid );

  {
    void *ctx = duf_levinfo_context( pdi );

    if ( !ctx || 0 != strcmp( ( char * ) ctx, "BEFORE" ) )
      DUF_SHOW_ERROR( "sample context %s", ( char * ) ctx );

    duf_levinfo_set_context( pdi, NULL );
    ctx = duf_levinfo_context( pdi );
    if ( !ctx )
    {
      char *test = mas_strdup( "MIDDLE" );

      duf_levinfo_set_context( pdi, test );
    }
  }

  DEBUG_ENDR( r );
}

static int
sample_entry_dir2(  /* duf_sqlite_stmt_t * pstmt_unused, */ const char *fname, const struct stat *pstat, /* unsigned long long dirid, */
                   duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  DUF_TRACE( scan, 0, "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ IN scan entry dir2 by %s", fname );

  DEBUG_ENDR( r );
}




duf_scan_callbacks_t duf_sample_callbacks = {
  .title = "sample mod",
  .init_scan = NULL,
  /* .def_opendir = 1, */

  /* .dirent_dir_scan_before = directories_entry_dir, */
  .dirent_dir_scan_before2 = sample_entry_dir2,

  /* .node_scan_before = sample_scan_node_before, */
  .node_scan_before2 = sample_scan_node_before2,

  /* .node_scan_after = sample_scan_node_after, */
  .node_scan_after2 = sample_scan_node_after2,

  /* .node_scan_middle = sample_scan_node_middle, */
  .node_scan_middle2 = sample_scan_node_middle2,

  /* .leaf_scan = sample_scan_leaf, */
  .leaf_scan2 = sample_scan_leaf2,

  .leaf = {.fieldset = "fn.pathid AS dirid " /* */
           ", fn.name AS filename, fd.size AS filesize" /* */
           ", uid, gid, nlink, inode, strftime('%s',mtim) AS mtime " /* */
           ", dup5cnt AS nsame " /* */
           ", fd." DUF_SQL_IDNAME " AS filenameid" /* */
           ", fd.mode AS filemode" /* */
           ", md.md5sum1, md.md5sum2 " /* */
           ", fd.md5id AS md5id" /* */
           ,
           .selector = "SELECT %s FROM " DUF_DBPREF "filenames AS fn " /* */
           " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "md5 AS md ON (md." DUF_SQL_IDNAME "=fd.md5id)" /* */
           "    WHERE "         /* */
           /* "           fd.size >= %llu AND fd.size < %llu "             */
           /* "       AND (md.dup5cnt IS NULL OR (md.dup5cnt >= %llu AND md.dup5cnt < %llu))  AND " */
           " fn.pathid='%llu' ",
           .selector2 = " FROM " DUF_DBPREF "filenames AS fn " /* */
           " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "md5 AS md ON (md." DUF_SQL_IDNAME "=fd.md5id)" /* */
           "    WHERE "         /* */
           " fn.pathid = :parentdirID" /* */
           ,
           .selector_total2 =   /* */
           " FROM " DUF_DBPREF "filenames AS fn " /* */
           " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "md5 AS md ON (md." DUF_SQL_IDNAME "=fd.md5id)" /* */
           }
  ,
  .node = {.fieldset = "pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname,  pt.ParentId " /* */
           ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize" /* */
           ,
           .selector = "SELECT     pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname,  pt.ParentId " /* */
           ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
           /* " ,(SELECT count(*) FROM " DUF_DBPREF "paths AS subpaths WHERE subpaths.ParentId=pt." DUF_SQL_IDNAME ") AS ndirs "       */
           /* " ,(SELECT count(*) FROM " DUF_DBPREF "filenames AS sfn "                                                       */
           /* "          JOIN " DUF_DBPREF "filedatas AS sfd ON (sfn.dataid=sfd." DUF_SQL_IDNAME ") "                                         */
           /* "          JOIN " DUF_DBPREF "md5 AS smd ON (sfd.md5id=smd." DUF_SQL_IDNAME ") "                                                */
           /* "          WHERE sfn.Pathid=pt." DUF_SQL_IDNAME " "                                                            */
           /* "              AND   sfd.size >= %llu AND sfd.size < %llu "                                           */
           /* "              AND (smd.dup5cnt IS NULL OR (smd.dup5cnt >= %llu AND smd.dup5cnt < %llu)) "               */
           /* " ) AS nfiles "                                                                                       */
           /* " ,(SELECT min(sfd.size) FROM " DUF_DBPREF "filedatas AS sfd JOIN " DUF_DBPREF "filenames AS sfn ON (sfn.dataid=sfd." DUF_SQL_IDNAME ") " */
           /* "           WHERE sfn.Pathid=pt." DUF_SQL_IDNAME ") AS minsize "                                               */
           /* " ,(SELECT max(sfd.size) FROM " DUF_DBPREF "filedatas AS sfd JOIN " DUF_DBPREF "filenames AS sfn ON (sfn.dataid=sfd." DUF_SQL_IDNAME ") " */
           /* "           WHERE sfn.Pathid=pt." DUF_SQL_IDNAME ") AS maxsize "                                               */
           " FROM " DUF_DBPREF "paths AS pt " /* */
           " LEFT JOIN " DUF_DBPREF "pathtot_dirs AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "pathtot_files AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " WHERE pt.ParentId='%llu' AND 0 " /* */
           ,
           .selector2 =         /* */
           " FROM " DUF_DBPREF "paths AS pt " /* */
           " LEFT JOIN " DUF_SQL_TABLES_PATHTOT_DIRS_FULL "  AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_SQL_TABLES_PATHTOT_FILES_FULL " AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
#if 0
           " LEFT JOIN " DUF_DBPREF "pathtot_dirs AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "pathtot_files AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
#endif
           " WHERE pt.ParentId=:parentdirID  AND ( :dirName IS NULL OR dirname=:dirName )" /* */
           }
  /*, .final_sql_argv = &final_sql, */
};
