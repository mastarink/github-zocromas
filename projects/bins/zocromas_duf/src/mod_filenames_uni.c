#define DUF_SQL_PDI_STMT

#include <assert.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_hook_types.h"
#include "duf_maintenance.h"


#include "duf_dbg.h"
#include "duf_config_ref.h"

#include "duf_pdi.h"
#include "duf_levinfo.h"
#include "duf_levinfo_ref.h"

#include "duf_sql_defs.h"
#include "duf_sql_field.h"
#include "duf_sql.h"
#include "duf_sql2.h"

#include "duf_filedata.h"


/* ###################################################################### */
/* #include "duf_filenames_uni.h" */
/* ###################################################################### */


static int
filenames_insert_filename_uni( duf_depthinfo_t * pdi, const char *fname, /* unsigned long long dirid_unused, */ unsigned long long dataid )
{
  DEBUG_STARTR( r );
  if ( fname && duf_levinfo_dirid_up( pdi ) )
  {
    int changes = 0;

    const char *sql = "INSERT OR IGNORE INTO " DUF_DBPREF "filenames (Pathid, name, dataid) VALUES (:pathID, :Name, :dataID)";

    DUF_SQL_START_STMT( pdi, insert_filename, sql, r, pstmt );
    DUF_TRACE( insert, 0, "S:%s", sql );
    DUF_SQL_BIND_LL( pathID, duf_levinfo_dirid_up( pdi ), r, pstmt );
    DUF_SQL_BIND_S( Name, fname, r, pstmt );
    DUF_SQL_BIND_LL( dataID, dataid, r, pstmt );
    DUF_SQL_STEP( r, pstmt );
    DUF_SQL_CHANGES( changes, r, pstmt );
    DUF_SQL_END_STMT( insert_filename, r, pstmt );
  }
  else
  {
    DUF_SHOW_ERROR( "Wrong data (fname:%s; dirid:%llu)", fname, duf_levinfo_dirid_up( pdi ) );
    r = DUF_ERROR_DATA;
    DUF_TEST_R( r );
  }
  /* DUF_TRACE( current, 0, "%llu : %s @ %llu", dirid, fname, dirid ); */
  DEBUG_ENDR( r );
}





/* static int                                                                                                                      */
/* filenames_entry_reg( const char *fname, const struct stat *pst_file, unsigned long long dirid, duf_depthinfo_t * pdi,           */
/*                      duf_record_t * precord )                                                                                   */
/* {                                                                                                                               */
/*   DEBUG_STARTR( r );                                                                                                            */
/*   DUF_UNUSED unsigned long long dataid = 0;                                                                                     */
/*                                                                                                                                 */
/*   (* unsigned long long fnid = 0; *)                                                                                            */
/*                                                                                                                                 */
/*   DEBUG_START(  );                                                                                                              */
/*                                                                                                                                 */
/*   (* if ( pst_file && pst_file->st_size >= pdi->u.size.min && ( !pdi->u.size.max || pst_file->st_size <= pdi->u.size.max ) ) *) */
/*   if ( pst_file && dufOFF_lim_matchll( pdi->u.size, pst_file->st_size ) )                                                          */
/*   {                                                                                                                             */
/*     dataid = duf_file_dataid_by_stat( pdi, pst_file, &r );                                                                      */
/*     r = filenames_insert_filename_uni( pdi, fname, dirID, dataid );                                                             */
/*   }                                                                                                                             */
/*   DEBUG_ENDR( r );                                                                                                              */
/* }                                                                                                                               */

static int
filenames_entry_reg2(  /* duf_sqlite_stmt_t * pstmt, */ const char *fname, const struct stat *pst_file, /*unsigned long long dirid_unused, */
                      duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );

  if ( pst_file /* && !duf_config->cli.disable.insert */  )
  {
    unsigned long long dataid = 0;

    DOPR( r, dataid = duf_file_dataid_by_stat( pdi, pst_file, &r ) );

    DOR( r, filenames_insert_filename_uni( pdi, fname, /* dirid_unused, */ dataid ) );
  }
  DEBUG_ENDR( r );
}






static const char *final_sql[] = {
  "INSERT OR IGNORE INTO " DUF_DBPREF "pathtot_files (Pathid, numfiles, minsize, maxsize) " /* */
        "SELECT fn.Pathid AS Pathid, COUNT(*) AS numfiles, min(size) AS minsize, max(size) AS maxsize " /* */
        " FROM " DUF_DBPREF "filenames AS fn " /* */
        " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
        " GROUP BY fn.Pathid"   /* */
        ,
  "UPDATE " DUF_DBPREF "pathtot_files SET " /* */
        " minsize=(SELECT min(size) AS minsize " /* */
        " FROM " DUF_DBPREF "filenames AS fn JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
        " WHERE " DUF_DBPREF "pathtot_files.Pathid=fn.Pathid) " /* */
        ", maxsize=(SELECT max(size) AS maxsize " /* */
        " FROM " DUF_DBPREF "filenames AS fn JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
        " WHERE " DUF_DBPREF "pathtot_files.Pathid=fn.Pathid) " /* */
        ", numfiles=(SELECT COUNT(*) AS numfiles " /* */
        " FROM " DUF_DBPREF "filenames AS fn JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
        " WHERE " DUF_DBPREF "pathtot_files.Pathid=fn.Pathid)" /* */
        ,

  /* "DELETE FROM " DUF_DBPREF "sizes",                                 */
  /* "INSERT OR IGNORE INTO " DUF_DBPREF "sizes (size, dupzcnt) " (* *) */
  /*       "SELECT size, COUNT(*) " (* *)                               */
  /*       " FROM " DUF_DBPREF "filedatas AS fd GROUP BY fd.size" (* *) */
  /*       ,                                                            */

  /* "INSERT OR IGNORE INTO " DUF_DBPREF "pathtot_dirs (Pathid, numdirs) " (* *)                        */
  /*       "SELECT parents." DUF_SQL_IDNAME " AS Pathid, COUNT(*) AS numdirs " (* *)                    */
  /*       " FROM " DUF_DBPREF "paths " (* *)                                                           */
  /*       " JOIN " DUF_DBPREF "paths AS parents ON (parents." DUF_SQL_IDNAME "=paths.parentid) " (* *) */
  /*       " GROUP BY parents." DUF_SQL_IDNAME "" (* *)                                                 */
  /*       ,                                                                                            */
  /* "UPDATE " DUF_DBPREF "pathtot_dirs SET " (* *)                                                     */
  /*       " numdirs=(SELECT COUNT(*) AS numdirs " (* *)                                                */
  /*       " FROM " DUF_DBPREF "paths AS p " (* *)                                                      */
  /*       " WHERE p.parentid=" DUF_DBPREF "pathtot_dirs.Pathid )" (* *)                                */
  /*       ,                                                                                            */



  /* "DELETE FROM " DUF_DBPREF "keydata", */
  /* "INSERT OR REPLACE INTO " DUF_DBPREF "keydata (md5id, filenameid, dataid, Pathid) " (* *)  */
  /*       "SELECT md." DUF_SQL_IDNAME " AS md5id, fn." DUF_SQL_IDNAME " AS filenameid, fd." DUF_SQL_IDNAME " AS dataid, p." DUF_SQL_IDNAME " AS Pathid " (* *) */
  /*       " FROM " DUF_DBPREF "filenames AS fn " (* *)                                         */
  /*       " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ")" (* *)                */
  /*       " JOIN " DUF_DBPREF "paths AS p ON (fn.Pathid=p." DUF_SQL_IDNAME ")" (* *)                           */
  /*       " JOIN " DUF_DBPREF "md5 AS md ON (fd.md5id=md." DUF_SQL_IDNAME ")",                                 */


  NULL,
};


duf_scan_callbacks_t duf_filenames_callbacks = {
  .title = "filenames",
  .init_scan = NULL,
  .def_opendir = 1,

  /* .dirent_file_scan_before = filenames_entry_reg, */
  .dirent_file_scan_before2 = filenames_entry_reg2,

  .leaf = {.fieldset = "fn.Pathid AS dirid, fn.name AS filename, fd.size AS filesize " /* */
           ", uid, gid, nlink, inode, strftime('%s',mtim) AS mtime " /* */
           ", fd.mode AS filemode " /* */
           ", fn." DUF_SQL_IDNAME " AS filenameid " /* */
           ", md.dup5cnt AS nsame" /* */
           ", md.md5sum1, md.md5sum2 " ", fd.md5id AS md5id" /* */
           ,
           /* .selector = "SELECT %s " (* *)                                                          */
           /*       " FROM " DUF_DBPREF "filenames AS fn " (* *)                                           */
           /*       " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON ( fn.dataid = fd." DUF_SQL_IDNAME " ) " (* *)             */
           /*       " LEFT JOIN " DUF_DBPREF "md5 AS md ON ( md." DUF_SQL_IDNAME " = fd.md5id ) " (* *)                    */
           /*       " WHERE "               (* *)                                                          */
           /*       (* " fd.size >= %llu AND fd.size < %llu "                      *)                      */
           /*       (* " AND( md.dup5cnt IS NULL OR( md.dup5cnt >= %llu AND md.dup5cnt < %llu ) ) AND " *) */
           /*       " fn.Pathid = '%llu' ",                                                                */
           .selector2 =         /* */
           /* "SELECT %s " */
           " FROM " DUF_DBPREF "filenames AS fn " /* */
           " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON ( fn.dataid = fd." DUF_SQL_IDNAME " ) " /* */
           " LEFT JOIN " DUF_DBPREF "md5 AS md ON ( md." DUF_SQL_IDNAME " = fd.md5id ) " /* */
           " WHERE "            /* */
           " fn.Pathid = :parentdirID " /* */
           ,
           .selector_total2 =   /* */
           " FROM " DUF_DBPREF "filenames AS fn " /* */
           " LEFT JOIN " DUF_DBPREF "filedatas AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "md5 AS md ON (md." DUF_SQL_IDNAME "=fd.md5id)" /* */
           },
  .node = {.fieldset = "pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname,  pt.parentid " /* */
           ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize" /* */
           ,
           /* .selector = "SELECT     pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname,  pt.parentid " (* *)                                         */
           /*       ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " (* *)                                   */
           /*       (* ", ( SELECT count( * ) FROM " DUF_DBPREF "paths AS subpaths WHERE subpaths.parentid = pt." DUF_SQL_IDNAME " ) AS ndirs "        *)                 */
           /*       (* ", ( SELECT count( * ) FROM " DUF_DBPREF "filenames AS sfn "                                                           *)          */
           /*       (* " LEFT JOIN " DUF_DBPREF "filedatas AS sfd ON( sfn.dataid = sfd." DUF_SQL_IDNAME " ) "                                                     *)      */
           /*       (* " JOIN " DUF_DBPREF "md5 AS smd ON( sfd.md5id = smd." DUF_SQL_IDNAME " ) "                                                            *)           */
           /*       (* " WHERE sfn.Pathid = pt." DUF_SQL_IDNAME " "                                                                         *)                            */
           /*       (* " AND sfd.size >= %llu AND sfd.size < %llu "                                                                *)                     */
           /*       (* " AND( smd.dup5cnt IS NULL OR( smd.dup5cnt >= %llu AND smd.dup5cnt < %llu ) ) "                                *)                  */
           /*       (* " ) AS nfiles "                                                                                             *)                     */
           /*       (* ", ( SELECT min( sfd.size ) FROM " DUF_DBPREF "filedatas AS sfd JOIN " DUF_DBPREF "filenames AS sfn ON( sfn.dataid = sfd." DUF_SQL_IDNAME " ) " *) */
           /*       (* " WHERE sfn.Pathid = pt." DUF_SQL_IDNAME " ) AS minsize "                                                            *)                            */
           /*       (* ", ( SELECT max( sfd.size ) FROM " DUF_DBPREF "filedatas AS sfd JOIN " DUF_DBPREF "filenames AS sfn ON( sfn.dataid = sfd." DUF_SQL_IDNAME " ) " *) */
           /*       (* " WHERE sfn.Pathid = pt." DUF_SQL_IDNAME " ) AS maxsize "                                                            *)                            */
           /*       " FROM " DUF_DBPREF "paths AS pt " (* *)                                                                                              */
           /*       " LEFT JOIN " DUF_DBPREF "pathtot_dirs AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " (* *)                                                             */
           /*       " LEFT JOIN " DUF_DBPREF "pathtot_files AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " (* *)                                                            */
           /*       " WHERE pt.parentid = '%llu' ",                                                                                                       */
           .selector2 =         /* */
           /* "SELECT     pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname,  pt.parentid "                  */
           /* ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " */
           " FROM " DUF_DBPREF "paths AS pt " /* */
           " LEFT JOIN " DUF_DBPREF "pathtot_dirs AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "pathtot_files AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " WHERE pt.ParentId = :parentdirID  AND ( :dirName IS NULL OR dirname=:dirName ) " /* */
  },
  .final_sql_argv = final_sql,
};
