#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <zlib.h>




#include "duf_maintenance.h"

#include "duf_config.h"
#include "duf_config_trace.h"
#include "duf_config_util.h"
#include "duf_config_ref.h"
#include "duf_config_defs.h"


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
static int crc32_dirent_content2( duf_stmnt_t * pstmt, duf_depthinfo_t * pdi );

/* ########################################################################################## */
#define FILTER_DATA  "fd.crc32id IS NULL"

static duf_sql_sequence_t final_sql = /* */
{
  .name = "final @ ...",
  .done = 0,
  .sql = {
          "UPDATE " DUF_SQL_TABLES_CRC32_FULL " SET dup32cnt=(SELECT COUNT(*) " /* */
          " FROM " DUF_SQL_TABLES_CRC32_FULL " AS c32 " /* */
          " JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fd.crc32id=c32." DUF_SQL_IDFIELD ") " /* */
          " WHERE " DUF_SQL_TABLES_CRC32_FULL "." DUF_SQL_IDFIELD "=c32." DUF_SQL_IDFIELD ")" /* */
          /* " WHERE " DUF_SQL_TABLES_CRC32_FULL ".crc32sum=c32.crc32sum )" (* *) */
          ,
          NULL,
          }
};

/* ########################################################################################## */
duf_scan_callbacks_t duf_crc32_callbacks = {
  .title = "collect crc32",
  .name = "crc32",
  .init_scan = NULL,
  .def_opendir = 1,




  .leaf_scan_fd2 = crc32_dirent_content2,

/* TODO : explain values of use_std_leaf_set_num and use_std_node_set_num TODO */
  .use_std_leaf_set_num = 2, /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .use_std_node_set_num = 2, /* 1 : preliminary selection; 2 : direct (beginning_sql_seq=NULL recommended in many cases) */
  .std_leaf_set_name = "std-leaf-no-sel",
  .std_node_set_name = "std-node-two",
#define DUF_FILTER
  .leaf = {                     /* */
           .name = "crc32 leaf",
           .type = DUF_NODE_LEAF,
           .fieldset =          /* */
           "#crc32",
           .fieldsets = {
                         "#basic",
                         "#plus",
                         "#crc32x",
                         NULL}
           ,
           .selector2 =         /* */
           /* "SELECT %s " */
           " FROM " DUF_SQL_TABLES_FILENAMES_FULL " AS fn " /* */
           " LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL " AS fd ON (fn.dataid=fd." DUF_SQL_IDFIELD ") " /* */
           " LEFT JOIN " DUF_SQL_TABLES_SIZES_FULL " AS sz ON (sz.size=fd.size)" /* */
           " LEFT JOIN " DUF_SQL_TABLES_CRC32_FULL " AS crc ON (crc." DUF_SQL_IDFIELD "=fd.crc32id)" /* */
           ,
           .matcher = " fn.Pathid=:parentdirID " /* */
           ,                    /* */
#if 0
           .filter =            /* */
           "( " FILTER_DATA " OR crc." DUF_SQL_IDFIELD " IS NULL ) " /*                          */ " AND " /* */
           "( sz.size    IS NULL OR sz.size > 0 ) " /*                                             */ " AND " /* */
           "(  :fFast    IS NULL OR sz.size IS NULL OR sz.dupzcnt IS NULL OR sz.dupzcnt > 1 ) " /* */ " AND " /* */
           " 1 "                /* */
           ,                    /* */
#else
           .afilter = {"sz.size > 0"},
           .afilter_fresh = {FILTER_DATA " OR crc." DUF_SQL_IDFIELD " IS NULL", "sz.size  IS NULL OR sz.size > 0"},
           .afilter_fast = {"sz.size IS NULL OR sz.dupzcnt IS NULL OR sz.dupzcnt > 1"},
#endif
           /*, .group=" fd." DUF_SQL_IDFIELD */
           .count_aggregate = "DISTINCT fd." DUF_SQL_IDFIELD /* */
           }
  ,
  .node = {
           .name = "crc32 node",
           .type = DUF_NODE_NODE,
           .expand_sql = 1,     /* */
           .fieldset =          /* */
           /* "'crc32-node' AS fieldset_id, " (* *) */
           " pt." DUF_SQL_IDFIELD " AS dirid" /* */
           ", pt." DUF_SQL_IDFIELD " AS nameid " /* */
           ", pt." DUF_SQL_DIRNAMEFIELD " AS dname, pt." DUF_SQL_DIRNAMEFIELD " AS dfname,  pt.ParentId " /* */
#ifndef MAS_DUF_DEFS_H
#  error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
           ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize" /* */
#endif
#ifndef MAS_DUF_DEFS_H
#  error use #include "duf_defs.h"
#elif defined( DUF_DO_RNUMS )
           ", " DUF_SQL_RNUMDIRS( pt ) " AS rndirs " /* */
           ", (" DUF_SQL__RNUMFILES( pt ) " WHERE " FILTER_DATA " ) AS rnfiles " /* */
#endif
           ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks, STRFTIME( '%s', pt.mtim ) AS mtime " /* */
           ,
           .selector2 =         /* */
           " FROM " DUF_SQL_TABLES_PATHS_FULL " AS pt " /* */
#ifndef MAS_DUF_DEFS_H
#  error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
           " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_DIRS_FULL "  AS td ON (td.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
           " LEFT JOIN " DUF_SQL_TABLES_PSEUDO_PATHTOT_FILES_FULL " AS tf ON (tf.Pathid=pt." DUF_SQL_IDFIELD ") " /* */
#endif
           ,
           .matcher = "pt.ParentId=:parentdirID AND ( :dirName IS NULL OR dname=:dirName )" /* */
           ,                    /* */
#ifndef MAS_DUF_DEFS_H
#  error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
           .filter = " rnfiles > 0 " /* */
#endif
           },
  .final_sql_seq = &final_sql,
};


/* ########################################################################################## */
unsigned long long
duf_pdistat2file_crc32id_existed( duf_depthinfo_t * pdi, unsigned long crc32sum, int *pr )
{
  int rpr = 0;
  unsigned long long crc32id = 0;
  const char *sql = "SELECT " DUF_SQL_IDFIELD " AS crc32id FROM " DUF_SQL_TABLES_CRC32_FULL " WHERE crc32sum=:Crc32sum"
        /* " INDEXED BY " DUF_SQL_TABLES_CRC32 "_uniq WHERE  crc32sum=:Crc32sum" */
        ;

  DEBUG_START(  );

  DUF_SQL_START_STMT( pdi, select_crc32, sql, rpr, pstmt );
  DUF_TRACE( select, 3, "S:%s", sql );
  DUF_SQL_BIND_LL( Crc32sum, crc32sum, rpr, pstmt );
  DUF_SQL_STEP( rpr, pstmt );
  if ( DUF_IS_ERROR_N( rpr, DUF_SQL_ROW ) )
  {
    DUF_TRACE( select, 10, "<selected>" );
    /* crc32id = duf_sql_column_long_long( pstmt, 0 ); */
    crc32id = DUF_GET_UFIELD2( crc32id );
    /* rpr = 0; */
  }
  else
  {
    /* DUF_TEST_R( rpr ); */
    DUF_TRACE( select, 10, "<NOT selected> (%d)", rpr );
  }
  /* DUF_TEST_R( rpr ); */
  DUF_SQL_END_STMT( pdi, select_crc32, rpr, pstmt );
  if ( pr )
    *pr = rpr;
  DEBUG_ENDULL( crc32id );
}

static unsigned long long
duf_insert_crc32_uni( duf_depthinfo_t * pdi, unsigned long long crc32sum, const char *filename, int need_id, int *pr )
{
  unsigned long long crc32id = -1;
  int lr = 0;
  int changes = 0;

#ifdef MAS_TRACING
  const char *real_path = duf_levinfo_path( pdi );
#endif

  DEBUG_START(  );
  if ( crc32sum )
  {
    static unsigned long insert_cnt = 0;

    if ( !DUF_CONFIGG( opt.disable.flag.insert ) )
    {
      static const char *sql = "INSERT OR IGNORE INTO " DUF_SQL_TABLES_CRC32_FULL " (crc32sum) VALUES (:crc32sum)";

      DUF_TRACE( crc32, 10, "%0llx %s%s", crc32sum, real_path, filename );
      DUF_SQL_START_STMT( pdi, insert_crc32, sql, lr, pstmt );
      DUF_TRACE( insert, 0, "(%ld) S:%s", insert_cnt, sql );
      DUF_SQL_BIND_LL( crc32sum, crc32sum, lr, pstmt );
      DUF_SQL_STEPC( lr, pstmt );
      DUF_SQL_CHANGES( changes, lr, pstmt );
      DUF_SQL_END_STMT( pdi, insert_crc32, lr, pstmt );
      insert_cnt++;
    }
    duf_pdi_reg_changes( pdi, changes );
    if ( ( DUF_IS_ERROR_N( lr, DUF_SQL_CONSTRAINT ) || !lr ) && !changes )
    {
      if ( need_id )
        crc32id = duf_pdistat2file_crc32id_existed( pdi, crc32sum, &lr );
    }
    else if ( DUF_NOERROR( lr ) /* assume SQLITE_OK */  )
    {
      if ( need_id && changes )
      {
        crc32id = duf_sql_last_insert_rowid(  );
      }
    }
    /* else                                       */
    /* {                                          */
    /*   DUF_SHOW_ERROR( "insert crc32 %d", lr ); */
    /* }                                          */
  }
  else
  {
    /* DUF_SHOW_ERROR( "Wrong data" ); */
    DUF_MAKE_ERRORM( lr, DUF_ERROR_DATA, "no crc32 sum" );
    /* DUF_TEST_R( lr ); */
  }

  if ( pr )
    *pr = lr;

  DEBUG_ENDULL( crc32id );
  return crc32id;
}

static int
duf_make_crc32_uni( int fd, unsigned long long *pcrc32sum )
{
  DEBUG_STARTR( r );
  size_t bufsz = 512 * 8;
  unsigned long crc32sum = 0;
  unsigned char *buffer;

  if ( !DUF_CONFIGG( opt.disable.flag.calculate ) )
    crc32sum = crc32( 0L, Z_NULL, 0 );
  /* if ( !DUF_CONFIGG(opt.disable.flag.calculate )) */
  {
    size_t bytes = 0;

    buffer = mas_malloc( bufsz );
    if ( buffer )
    {
      int cnt = 0;
      int maxcnt = 1;

      /* lseek( fd, -bufsz * maxcnt, SEEK_END ); */
      while ( DUF_NOERROR( r ) && cnt++ < maxcnt )
      {
        int rr;

        rr = read( fd, buffer, bufsz );
        DUF_TRACE( crc32, 10, "read %d : crc32sum:%lx", rr, crc32sum );
        if ( rr < 0 )
        {
          DUF_ERRSYS( "read file" );
          DUF_MAKE_ERROR( r, DUF_ERROR_READ );
        }
        DUF_TEST_R( r );
        if ( rr > 0 && !DUF_CONFIGG( opt.disable.flag.calculate ) )
        {
          crc32sum = crc32( crc32sum, buffer, rr );
          bytes += rr;
        }
        DUF_TRACE( crc32, 10, "rr:%d; r:%d; crc32sum:%lx", rr, r, crc32sum );
        if ( rr <= 0 )
          break;

      }
      DUF_TRACE( crc32, 10, "last crc32sum:%lx", crc32sum );
      mas_free( buffer );
    }
    else
    {
      DUF_MAKE_ERROR( r, DUF_ERROR_MEMORY );
    }
    /* T( "@bytes:%lu", bytes ); */
  }
  if ( pcrc32sum )
    *pcrc32sum = crc32sum;
  DEBUG_ENDR( r );
}


static int
crc32_dirent_content2( duf_stmnt_t * pstmt, /* const struct stat *pst_file_needless, */ duf_depthinfo_t * pdi )
{
  DEBUG_STARTR( r );
  unsigned long long crc32sum = 0;
  static unsigned long content_cnt = 0;

  DUF_SFIELD2( fname );
  DUF_TRACE( crc32, 0, "+ %s", fname );

  /* assert( duf_levinfo_dbstat( pdi ) ); */
  assert( duf_levinfo_dfd( pdi ) );
  assert( duf_levinfo_stat( pdi ) );

  if ( DUF_CONFIGG( opt.disable.flag.calculate ) )
    crc32sum = ( unsigned long long ) duf_levinfo_dirid( pdi );
  else
    DOR( r, duf_make_crc32_uni( duf_levinfo_dfd( pdi ), &crc32sum ) );


  content_cnt++;
  if ( DUF_NOERROR( r ) )
  {
    unsigned long long crc32id = 0;

    DUF_TRACE( crc32, 10, "insert %s", fname );


    crc32id = duf_insert_crc32_uni( pdi, crc32sum, fname /* for dbg message only */ , 1 /*need_id */ , &r );
    if ( DUF_NOERROR( r ) && crc32id )
    {
      int changes = 0;

      pdi->cnts.dirent_content2++;
      if ( DUF_NOERROR( r ) && !DUF_CONFIGG( opt.disable.flag.update ) )
      {
        DUF_UFIELD2( filedataid );
#if 0
        DOR( r,
             duf_sql( "UPDATE " DUF_SQL_TABLES_FILEDATAS_FULL " SET crc32id='%llu' WHERE " DUF_SQL_IDFIELD "='%lld'", &changes, crc32id,
                      filedataid ) );
#else
        const char *sql = "UPDATE " DUF_SQL_TABLES_FILEDATAS_FULL " SET crc32id=:crc32Id WHERE " DUF_SQL_IDFIELD " =:dataId ";

        DUF_SQL_START_STMT( pdi, update_crc32id, sql, r, pstmt );
        DUF_TRACE( mod, 3, "S:%s", sql );
        DUF_SQL_BIND_LL( crc32Id, crc32id, r, pstmt );
        DUF_SQL_BIND_LL( dataId, filedataid, r, pstmt );
        DUF_SQL_STEPC( r, pstmt );
        DUF_SQL_CHANGES( changes, r, pstmt );
        DUF_SQL_END_STMT( pdi, update_crc32id, r, pstmt );
#endif
      }
      duf_pdi_reg_changes( pdi, changes );
      DUF_TEST_R( r );
    }
    DUF_TRACE( crc32, 0, "(%lu) %04llx : crc32id: %llu (sz:%lu) \"%s\"", content_cnt, crc32sum, crc32id, duf_levinfo_stat( pdi )->st_size, fname );
    /* DUF_TRACE( scan, 12, "  " DUF_DEPTH_PFMT ": scan 5    * %04lx : %llu", duf_pdi_depth( pdi ), crc32sum, crc32id ); */
  }
  DEBUG_ENDR( r );
}
