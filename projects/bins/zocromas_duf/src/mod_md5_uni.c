#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <openssl/md5.h>




#include "duf_maintenance.h"

#include "duf_config_ref.h"


#include "duf_pdi_ref.h"
#include "duf_pdi_stmt.h"

#include "duf_levinfo_ref.h"


#include "duf_sql_stmt_defs.h"

#include "duf_sql_defs.h"
#include "duf_sql_field.h"

#include "duf_sql_bind.h"
#include "duf_sql_prepared.h"

/* #include "duf_dbg.h" */

#include "sql_beginning_tables.h"
/* ########################################################################################## */
static int md5_dirent_content2( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi );

/* ########################################################################################## */
static duf_sql_sequence_t final_sql = /* */
{
  .name = "final @ md5",
  .done = 0,
  .sql = {
          "UPDATE " DUF_SQL_TABLES_MD5_FULL " SET dup5cnt=(SELECT COUNT(*) " /* */
          " FROM " DUF_SQL_TABLES_MD5_FULL " AS md " /* */
          " JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fd.md5id=md." DUF_SQL_IDNAME ") " /* */
          " WHERE " DUF_SQL_TABLES_MD5_FULL "." DUF_SQL_IDNAME "=md." DUF_SQL_IDNAME ")" /* */
          /* " WHERE " DUF_SQL_TABLES_MD5_FULL ".md5sum1=md.md5sum1 AND " DUF_SQL_TABLES_MD5_FULL ".md5sum2=md.md5sum2)" (* *) */
          ,
#if 0
          "INSERT OR IGNORE INTO " DUF_DBPREF "pathtot_dirs (Pathid, numdirs) " /* */
          "SELECT parents." DUF_SQL_IDNAME " AS Pathid, COUNT(*) AS numdirs " /* */
          " FROM " DUF_SQL_TABLES_PATHS_FULL " " /* */
          " JOIN " DUF_SQL_TABLES_PATHS_FULL " AS parents ON (parents." DUF_SQL_IDNAME "=" DUF_SQL_TABLES_PATHS_FULL ".parentid) " /* */
          " GROUP BY parents." DUF_SQL_IDNAME "" /* */
          ,
#endif
          "DELETE FROM path_pairs" /* */
          ,
          "INSERT OR IGNORE INTO path_pairs (samefiles, Pathid1, Pathid2) SELECT count(*), fna.Pathid AS Pathid1, fnb.Pathid  AS Pathid2" /* */
          " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fna" /* */
          "   JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fda ON (fna.dataid=fda.rowid)" /* */
          "   JOIN " DUF_SQL_TABLES_MD5_FULL " AS mda ON (fda.md5id=mda.rowid)" /* */
          "   JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fdb ON (fdb.md5id=mda.rowid)" /* */
          "   JOIN " DUF_SQL_TABLES_FILENAMES_FULL " AS fnb ON (fdb.rowid=fnb.dataid)" /* */
          " WHERE Pathid1 < Pathid2 AND fna.name=fnb.name" /* */
          " GROUP BY Pathid1, Pathid2" /* */
          ,


          NULL,
          }
};

/* ########################################################################################## */

duf_scan_callbacks_t duf_collect_openat_md5_callbacks = {
  .title = "collect md5",
  .name = "md5",
  .init_scan = NULL,
  .def_opendir = 1,
  /* .dirent_dir_scan_before = NULL, */
  /* .dirent_file_scan_before = NULL, */
  /* .node_scan_before = collect_openat_md5_node_before, */
  /*  .leaf_scan =  collect_openat_md5_leaf, */
  /* .leaf_scan_fd = duf_dirent_md5_content, */
  .leaf_scan_fd2 = md5_dirent_content2,

/* TODO : exp;ain values of use_std_leaf and use_std_node TODO */
  .use_std_leaf = 0,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node = 0,            /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .leaf = {
           .name = "md leaf",
           .type = DUF_NODE_LEAF,
           .fieldset =          /* */
#if 0
           /* "'md5-leaf' AS fieldset_id, " (* *) */
           " fn.Pathid AS dirid " /* */
           ", 0 AS ndirs, 0 AS nfiles" /* */
           " , fd." DUF_SQL_IDNAME " AS filedataid " /* */
           " , fd." DUF_SQL_IDNAME " AS dataid " /* */
           " , fd.inode AS inode " /* */
           " , fn.name AS filename, fn.name AS dfname, fd.size AS filesize " /* */
           " , fd.dev, fd.uid, fd.gid, fd.nlink, STRFTIME('%s',fd.mtim) AS mtime, fd.rdev, fd.blksize, fd.blocks " /* */
           " , md.dup5cnt AS nsame " /* */
           ", md.dup5cnt            AS dup5cnt " /* */
           ", sz.dupzcnt            AS dupzcnt " /* */
           " , fn." DUF_SQL_IDNAME " AS filenameid " /* */
           " , fn." DUF_SQL_IDNAME " AS nameid " /* */
           " , fd.mode AS filemode, md.md5sum1, md.md5sum2 " /* */
           " , fd.md5id AS md5id" /* */
#else
           "#md5"
#endif
           ,
           .fieldsets = {
                         "#basic",
                         "#plus",
                         "#md5x",
                         NULL}
           ,
           .selector2 =         /* */
#if 0
           " FROM " DUF_SQL_TABLES_FILENAMES_FULL /*        */ " AS fn " /* */
           " LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL /*   */ " AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_SQL_TABLES_SIZES_FULL /*       */ " AS sz ON (sz.size=fd.size)" /* */
           " LEFT JOIN " DUF_SQL_TABLES_MD5_FULL /*         */ " AS md ON (md." DUF_SQL_IDNAME "=fd.md5id)" /* */
           " LEFT JOIN " DUF_SQL_TABLES_SD5_FULL /*         */ " AS sd ON (sd." DUF_SQL_IDNAME "=fd.sd5id)" /* */
#else
           "#md5-leaf"
#endif
           ,
           .matcher = " fn.Pathid=:parentdirID " /* */
           ,                    /* */
           .filter =            /* */
           " ( fd.md5id   IS NULL OR md." DUF_SQL_IDNAME " IS NULL ) " /*                */ " AND " /* */
           " sz.size > 0 " /*                                                            */ " AND " /* */
           "(  :fFast IS NULL OR sz.size IS NULL OR sz.dupzcnt > 1 ) " /*                */ " AND " /* */
           "(  :fFast IS NULL OR sd." DUF_SQL_IDNAME " IS NULL OR sd.dup2cnt > 1 ) " /*  */ " AND " /* */
           "(  :fFast IS NULL OR sh." DUF_SQL_IDNAME " IS NULL OR sh.dupsha1cnt > 1 ) " /**/ " AND " /* */
           " 1 "                /* */
           ,
           .count_aggregate = "DISTINCT fd." DUF_SQL_IDNAME
#if 0
           ,
           .selector_total2 =   /* */
           " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn " /* */
           " LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fn.dataid=fd." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_SQL_TABLES_SIZES_FULL " AS sz ON (sz.size=fd.size)" /* */
           " LEFT JOIN " DUF_SQL_TABLES_MD5_FULL " AS md ON (md." DUF_SQL_IDNAME "=fd.md5id)" /* */
           " LEFT JOIN " DUF_SQL_TABLES_SD5_FULL " AS sd ON (sd." DUF_SQL_IDNAME "=fd.sd5id)" /* */
           ,                    /* */
#endif
           }
  ,                             /* */
  .node = {                     /* */
           .name = "md node",
           .type = DUF_NODE_NODE,
           .expand_sql = 1,     /* */
           .fieldset =          /* */
           /* "'md5-node' AS fieldset_id, " (* *) */
           " pt." DUF_SQL_IDNAME " AS dirid" /* */
           ", pt." DUF_SQL_IDNAME " AS nameid " /* */
           ", pt.dirname, pt.dirname AS dfname,  pt.ParentId " /* */
           ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize" /* */
           ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks, STRFTIME( '%s', pt.mtim ) AS mtime " /* */
           ,
           .selector2 =         /* */
           /* "SELECT     pt." DUF_SQL_IDNAME " AS dirid, pt.dirname, pt.dirname AS dfname,  pt.ParentId "                  */
           /* ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " */
           " FROM " DUF_SQL_TABLES_PATHS_FULL " AS pt " /* */
           " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_DIRS_FULL "  AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_SQL_TABLES_TMP_PATHTOT_FILES_FULL " AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
#if 0
           " LEFT JOIN " DUF_DBPREF "pathtot_dirs AS td ON (td.Pathid=pt." DUF_SQL_IDNAME ") " /* */
           " LEFT JOIN " DUF_DBPREF "pathtot_files AS tf ON (tf.Pathid=pt." DUF_SQL_IDNAME ") " /* */
#endif
           ,
           .matcher = " pt.ParentId=:parentdirID AND ( :dirName IS NULL OR dirname=:dirName )" /* */
           ,
           .filter = NULL       /* */
           },
  .final_sql_seq = &final_sql,
};

/* ########################################################################################## */
unsigned long long
duf_pdistat2file_md5id_existed( duf_depthinfo_t * pdi, unsigned long md5sum1, unsigned long md5sum2, int *pr )
{
  int rpr = 0;
  unsigned long long md5id = 0;
  const char *sql = "SELECT " DUF_SQL_IDNAME " AS md5id FROM " DUF_SQL_TABLES_MD5_FULL " WHERE md5sum1=:md5Sum1 AND md5sum2=:md5Sum2"
        /* " INDEXED BY " DUF_SQL_TABLES_SD5 "_uniq WHERE  md5sum1=:md5Sum1 AND md5sum2=:md5Sum2 */
        ;

  DEBUG_START(  );

  DUF_SQL_START_STMT( pdi, select_md5, sql, rpr, pstmt );
  DUF_TRACE( select, 3, "S:%s", sql );
  DUF_SQL_BIND_LL( md5Sum1, md5sum1, rpr, pstmt );
  DUF_SQL_BIND_LL( md5Sum2, md5sum2, rpr, pstmt );
  DUF_SQL_STEP( rpr, pstmt );
  if ( rpr == MAS_SQL_ROW )
  {
    DUF_TRACE( select, 10, "<selected>" );
    /* md5id = duf_sql_column_long_long( pstmt, 0 ); */
    md5id = DUF_GET_UFIELD2( md5id );
    rpr = 0;
  }
  else
  {
    /* DUF_TEST_R( rpr ); */
    DUF_TRACE( select, 10, "<NOT selected> (%d)", rpr );
  }
  /* DUF_TEST_R( rpr ); */
  DUF_SQL_END_STMT( pdi, select_md5, rpr, pstmt );
  if ( pr )
    *pr = rpr;
  DEBUG_ENDULL( md5id );
}

static unsigned long long
duf_insert_md5_uni( duf_depthinfo_t * pdi, unsigned long long *md64, const char *msg, int need_id, int *pr )
{
  unsigned long long md5id = -1;
  int lr = 0;
  int changes = 0;

#ifdef MAS_TRACING
  const char *real_path = duf_levinfo_path( pdi );
#endif

  DEBUG_START(  );

  assert( sizeof( unsigned long long ) == 8 );
  assert( MD5_DIGEST_LENGTH == 2 * sizeof( unsigned long long ) );
  assert( MD5_DIGEST_LENGTH == 2 * 64 / 8 );
  if ( md64 && md64[1] && md64[0] )
  {
    if ( !DUF_CONFIGG( cli.disable.flag.insert ) )
    {
      static const char *sql = "INSERT OR IGNORE INTO " DUF_SQL_TABLES_MD5_FULL " ( md5sum1, md5sum2 ) VALUES ( :md5sum1, :md5sum2 )";

      DUF_TRACE( md5, 0, "%016llx%016llx %s%s", md64[1], md64[0], real_path, msg );
      DUF_SQL_START_STMT( pdi, insert_md5, sql, lr, pstmt );
      DUF_TRACE( insert, 0, "S:%s", sql );
      DUF_SQL_BIND_LL( md5sum1, md64[1], lr, pstmt );
      DUF_SQL_BIND_LL( md5sum2, md64[0], lr, pstmt );
      DUF_SQL_STEP( lr, pstmt );
      DUF_SQL_CHANGES( changes, lr, pstmt );
      DUF_SQL_END_STMT( pdi, insert_md5, lr, pstmt );
    }
    duf_pdi_reg_changes( pdi, changes );
    if ( ( lr == MAS_SQL_CONSTRAINT || !lr ) && !changes )
    {
      if ( need_id )
        md5id = duf_pdistat2file_md5id_existed( pdi, md64[1], md64[0], &lr );
    }
    else if ( !lr /* assume SQLITE_OK */  )
    {
      if ( need_id && changes )
      {
        md5id = duf_sql_last_insert_rowid(  );
      }
    }
    else
    {
      DUF_SHOW_ERROR( "insert md5 %d", lr );
    }
  }
  else
  {
    DUF_SHOW_ERROR( "Wrong data" );
    DUF_MAKE_ERROR( lr, DUF_ERROR_DATA );
    DUF_TEST_R( lr );
  }

  if ( pr )
    *pr = lr;

  DEBUG_ENDULL( md5id );
  return md5id;
}

static int
duf_make_md5_uni( int fd, unsigned char *pmd )
{
  DEBUG_STARTR( r );
  size_t bufsz = 1024 * 1024 * 1;
  MD5_CTX ctx;

  memset( &ctx, 0, sizeof( ctx ) );
  DUF_TRACE( md5, 0, "make" );
  {
    char *buffer;

    buffer = mas_malloc( bufsz );
    if ( buffer )
    {
      if ( !DUF_CONFIGG( cli.disable.flag.calculate ) && ( MD5_Init( &ctx ) != 1 ) )
        DUF_MAKE_ERROR( r, DUF_ERROR_MD5 );
      DUF_TEST_R( r );
      {
        int maxcnt = 0;
        int cnt = 0;

        while ( DUF_NOERROR( r ) && ( maxcnt == 0 || cnt++ < maxcnt ) )
        {
          int ry;

          DUF_TRACE( md5, 10, "read fd:%u", fd );
          ry = read( fd, buffer, bufsz );
          DUF_TRACE( md5, 10, "read ry:%u", ry );
          if ( ry < 0 )
          {
            DUF_ERRSYS( "read file" );

            DUF_MAKE_ERROR( r, DUF_ERROR_READ );
            DUF_TEST_R( r );
            break;
          }
          if ( ry > 0 && !DUF_CONFIGG( cli.disable.flag.calculate ) )
          {
            if ( MD5_Update( &ctx, buffer, ry ) != 1 )
              DUF_MAKE_ERROR( r, DUF_ERROR_MD5 );
          }
          if ( ry <= 0 )
            break;
          DUF_TEST_R( r );
        }
      }
      mas_free( buffer );
    }
    else
    {
      DUF_MAKE_ERROR( r, DUF_ERROR_MEMORY );
    }
  }
  if ( !DUF_CONFIGG( cli.disable.flag.calculate ) && MD5_Final( pmd, &ctx ) != 1 )
    DUF_MAKE_ERROR( r, DUF_ERROR_MD5 );
  DEBUG_ENDR( r );
}

static int
duf_make_md5r_uni( duf_depthinfo_t * pdi, unsigned char *pmdr )
{
  DEBUG_STARTR( r );
  unsigned char amd5[MD5_DIGEST_LENGTH];
  int fd;

  memset( amd5, 0, sizeof( amd5 ) );
  fd = duf_levinfo_dfd( pdi );
  DOR( r, duf_make_md5_uni( fd, amd5 ) );
  /* reverse */
  for ( int i = 0; i < sizeof( amd5 ) / sizeof( amd5[0] ); i++ )
    pmdr[i] = amd5[sizeof( amd5 ) / sizeof( amd5[0] ) - i - 1];


  DEBUG_ENDR( r );
}

static int
md5_dirent_content2( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  unsigned char amd5r[MD5_DIGEST_LENGTH];


  DUF_SFIELD2( filename );
  DUF_TRACE( md5, 0, "+ %s", filename );

  if ( !DUF_CONFIGG( cli.disable.flag.calculate ) )
    DOR( r, duf_make_md5r_uni( pdi, amd5r ) );

  if ( DUF_NOERROR( r ) )
  {
    unsigned long long md5id = 0;
    unsigned long long *pmd = ( unsigned long long * ) &amd5r;

    DUF_TRACE( md5, 0, "insert %s", filename );


    md5id = duf_insert_md5_uni( pdi, pmd, filename /* for dbg message only */ , 1 /*need_id */ , &r );
    if ( md5id )
    {
      int changes = 0;

      pdi->cnts.dirent_content2++;
      if ( !DUF_CONFIGG( cli.disable.flag.update ) )
      {
        DUF_UFIELD2( filedataid );
#if 0
        DOR( r, duf_sql( "UPDATE " DUF_SQL_TABLES_FILEDATAS_FULL " SET md5id='%llu' WHERE " DUF_SQL_IDNAME "='%lld'", &changes, md5id, filedataid ) );
#else
        const char *sql = "UPDATE " DUF_SQL_TABLES_FILEDATAS_FULL " SET md5id=:md5Id WHERE " DUF_SQL_IDNAME " =:dataId ";

        DUF_SQL_START_STMT( pdi, update_md5id, sql, r, pstmt );
        DUF_TRACE( mod, 3, "S:%s", sql );
        DUF_SQL_BIND_LL( md5Id, md5id, r, pstmt );
        DUF_SQL_BIND_LL( dataId, filedataid, r, pstmt );
        DUF_SQL_STEP( r, pstmt );
        DUF_SQL_CHANGES( changes, r, pstmt );
        DUF_SQL_END_STMT( pdi, update_md5id, r, pstmt );
#endif
      }
      duf_pdi_reg_changes( pdi, changes );
      DUF_TEST_R( r );
    }
    DUF_TRACE( md5, 0, "%016llx%016llx : md5id: %llu", pmd[1], pmd[0], md5id );
    /* DUF_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan 5    * %016llx%016llx : %llu", duf_pdi_depth( pdi ), pmd[1], pmd[0], md5id ); */
  }
  DEBUG_ENDR( r );
}
