/* #undef MAS_TRACING */
#include <assert.h>                                                  /* assert */
#include <stddef.h>                                                  /* NULL */

#include "duf_tracen_defs_preset.h"

#include <mastar/wrap/mas_std_def.h>
#include <mastar/trace/mas_trace.h>

/* #include "duf_tracen_defs.h"                                         (* T; TT; TR ♠ *) */
/* #include "duf_errorn_defs.h"                                         (* DUF_NOERROR; DUF_CLEAR_ERROR; DUF_E_(LOWER|UPPER); DUF_TEST_R ... ♠ *) */

/* #include "duf_start_end.h"                                           (* DUF_STARTR ; DUF_ENDR ♠ *) */
/* #include "duf_dodefs.h"                                              (* DOR ♠ *) */

#include "duf_sql_defs.h"                                            /* DUF_SQL_IDFIELD etc. ♠ */
#include "duf_sql_set_types.h"                                       /* duf_fieldset_t */

#include "duf_mod_defs.h"

#include "sql_beginning_tables.h"                                    /* DUF_SQL_TABLES... etc. ♠ */

#include "duf_scan_types.h"                                          /* duf_node_type_t; duf_scanstage_t ♠ */
/* ###################################################################### */
#include "std_fieldsets.h"
/* ###################################################################### */

static duf_fieldset_t _all_fieldsets[] = {
  {
   .name = "template",
   .type = DUF_NODE_LEAF,
   .set =
 /* "'tmpl-leaf' AS fieldset_id, " (* *) */
   " fn.Pathid AS dirid "                                            /* */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
   ", 0 AS ndirs, 0 AS nfiles"                                       /* */
#endif
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_RNUMS )
   ", 0 AS rndirs, 0 AS rnfiles"                                     /* */
#endif
   ", fn." DUF_SQL_FILENAMEFIELD " AS fname, fn." DUF_SQL_FILENAMEFIELD " AS dfname, fd.size AS filesize " /* */
   ", fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, fd.rdev, fd.blksize, fd.blocks " /* */
   ", STRFTIME( '%s', fd.mtim ) AS mtime "                           /* */
   ", fd.mode " /*    */ " AS filemode "                             /* */
   ", fn." DUF_SQL_IDFIELD " AS filenameid "                         /* */
   ", fn." DUF_SQL_IDFIELD " AS nameid "                             /* */
   ", fd.md5id " /*    */ " AS md5id "                               /* */
 /* ", md." DUF_SQL_IDFIELD " AS md5id " (* *) */
   ", md.md5sum1, md.md5sum2 "                                       /* */
   ", fd." DUF_SQL_IDFIELD " AS filedataid "                         /* */
   ", fd." DUF_SQL_IDFIELD " AS dataid "                             /* */
 /* */
   ", md.dup5cnt " /*    */ " AS nsame "                             /* */
 /* ", md.dup5cnt            AS nsame_md5 " (* *)  */
 /* ", sh.dupsha1cnt         AS nsame_sha1 " (* *) */
 /* ", x.dupexifcnt          AS nsame_exif " (* *) */
   ", md.dup5cnt " /*    */ " AS dup5cnt "                           /* */
   ", sz.dupzcnt " /*    */ " AS dupzcnt "                           /* */
   ", fd.exifid AS exifid "                                          /* */
   ", fd.mimeid AS mimeid "                                          /* */
   ", mi.mime AS mime "                                              /* */
   ", STRFTIME( '%s', x.date_time ) AS exifdt "                      /* */
   ", xm.model AS camera "                                           /* */
   }
  ,
  {
   .name = "basic",
   .type = DUF_NODE_LEAF,
   .set =
 /* "'basic-leaf' AS fieldset_id, " (* *) */
   " fn.Pathid AS dirid "                                            /* */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
   ", 0 AS ndirs, 0 AS nfiles"                                       /* */
#endif
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_RNUMS )
   ", 0 AS rndirs, 0 AS rnfiles"                                     /* */
#endif
   ", fn." DUF_SQL_FILENAMEFIELD " AS fname, fn." DUF_SQL_FILENAMEFIELD " AS dfname, fd.size AS filesize " /* */
   }
  ,
  {
   .name = "plus",
   .type = DUF_NODE_LEAF,
   .set =
 /* "'plus-leaf' AS fieldset_id, " (* *) */
   " fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, fd.rdev, fd.blksize, fd.blocks " /* */
   ", STRFTIME( '%s', fd.mtim ) AS mtime "                           /* */
   ", fd.mode " /*    */ " AS filemode "                             /* */
   ", fn." DUF_SQL_IDFIELD " AS filenameid "                         /* */
   ", fn." DUF_SQL_IDFIELD " AS nameid "                             /* */
   ", fd.md5id " /*    */ " AS md5id "                               /* */
   ", fd." DUF_SQL_IDFIELD " AS filedataid "                         /* */
   ", fd." DUF_SQL_IDFIELD " AS dataid "                             /* */
   }
  ,
  {
   .name = "md5x",
   .type = DUF_NODE_LEAF,
   .set =
 /* "'md5x-leaf' AS fieldset_id, " (* *) */
 /* */
   " md.dup5cnt " /*    */ " AS nsame "                              /* */
 /* ", md.dup5cnt            AS nsame_md5 " (* *)  */
 /* ", sh.dupsha1cnt         AS nsame_sha1 " (* *) */
 /* ", x.dupexifcnt          AS nsame_exif " (* *) */
   ", md.dup5cnt " /*    */ " AS dup5cnt "                           /* */
   ", sz.dupzcnt " /*    */ " AS dupzcnt "                           /* */
   }
  ,
  {
   .name = "md5",
   .type = DUF_NODE_LEAF,
   .set =
 /* "'md5-leaf' AS fieldset_id, " (* *) */
   " fn.Pathid AS dirid "                                            /* */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
   ", 0 AS ndirs, 0 AS nfiles"                                       /* */
#endif
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_RNUMS )
   ", 0 AS rndirs, 0 AS rnfiles"                                     /* */
#endif
   ", fn." DUF_SQL_FILENAMEFIELD " AS fname, fn." DUF_SQL_FILENAMEFIELD " AS dfname, fd.size AS filesize " /* */
   ", fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, fd.rdev, fd.blksize, fd.blocks " /* */
   ", STRFTIME( '%s', fd.mtim ) AS mtime "                           /* */
   ", fd.mode " /*    */ " AS filemode "                             /* */
   ", fn." DUF_SQL_IDFIELD " AS filenameid "                         /* */
   ", fn." DUF_SQL_IDFIELD " AS nameid "                             /* */
   ", fd.md5id " /*    */ " AS md5id "                               /* */
 /* ", md." DUF_SQL_IDFIELD " AS md5id " (* *) */
   ", md.md5sum1, md.md5sum2 "                                       /* */
   ", fd." DUF_SQL_IDFIELD " AS filedataid "                         /* */
   ", fd." DUF_SQL_IDFIELD " AS dataid "                             /* */
 /* */
   ", md.dup5cnt " /*    */ " AS nsame "                             /* */
 /* ", md.dup5cnt            AS nsame_md5 " (* *)  */
 /* ", sh.dupsha1cnt         AS nsame_sha1 " (* *) */
 /* ", x.dupexifcnt          AS nsame_exif " (* *) */
   ", md.dup5cnt " /*    */ " AS dup5cnt "                           /* */
   ", sz.dupzcnt " /*    */ " AS dupzcnt "                           /* */
   }
  ,
  {
   .name = "sha1x",
   .type = DUF_NODE_LEAF,
   .set =
 /* "'sha1x-leaf' AS fieldset_id, " (* *) */
 /* */
   " sh.dupsha1cnt " /*    */ " AS nsame "                           /* */
 /* ", md.dup5cnt            AS nsame_md5 " (* *)  */
 /* ", sh.dupsha1cnt         AS nsame_sha1 " (* *) */
 /* ", x.dupexifcnt          AS nsame_exif " (* *) */
   ", sz.dupzcnt " /*    */ " AS dupzcnt "                           /* */
   }
  ,
  {
   .name = "sha1",
   .type = DUF_NODE_LEAF,
   .set =
 /* "'sha1-leaf' AS fieldset_id, " (* *) */
   " fn.Pathid AS dirid "                                            /* */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
   ", 0 AS ndirs, 0 AS nfiles"                                       /* */
#endif
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_RNUMS )
   ", 0 AS rndirs, 0 AS rnfiles"                                     /* */
#endif
   ", fn." DUF_SQL_FILENAMEFIELD " AS fname, fn." DUF_SQL_FILENAMEFIELD " AS dfname, fd.size AS filesize " /* */
   ", fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, fd.rdev, fd.blksize, fd.blocks " /* */
   ", STRFTIME( '%s', fd.mtim ) AS mtime "                           /* */
   ", fd.mode " /*    */ " AS filemode "                             /* */
   ", fn." DUF_SQL_IDFIELD " AS filenameid "                         /* */
   ", fn." DUF_SQL_IDFIELD " AS nameid "                             /* */
   ", fd.sha1id " /*    */ " AS sha1id "                             /* */
   ", sh.sha1sum1, sh.sha1sum2, sh.sha1sum3 "                        /* */
   ", fd." DUF_SQL_IDFIELD " AS filedataid "                         /* */
   ", fd." DUF_SQL_IDFIELD " AS dataid "                             /* */
 /* */
   ", sh.dupsha1cnt " /*    */ " AS nsame "                          /* */
 /* ", md.dup5cnt            AS nsame_md5 " (* *)  */
 /* ", sh.dupsha1cnt         AS nsame_sha1 " (* *) */
 /* ", x.dupexifcnt          AS nsame_exif " (* *) */
   ", sz.dupzcnt " /*    */ " AS dupzcnt "                           /* */
   }
  ,
  {
   .name = "sd5",
   .type = DUF_NODE_LEAF,
   .set =
 /* "'sd5-leaf' AS fieldset_id, " (* *) */
   " fn.Pathid AS dirid "                                            /* */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
   ", 0 AS ndirs, 0 AS nfiles"                                       /* */
#endif
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_RNUMS )
   ", 0 AS rndirs, 0 AS rnfiles"                                     /* */
#endif
   ", fn." DUF_SQL_FILENAMEFIELD " AS fname, fn." DUF_SQL_FILENAMEFIELD " AS dfname, fd.size AS filesize " /* */
   ", fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, fd.rdev, fd.blksize, fd.blocks " /* */
   ", STRFTIME( '%s', fd.mtim ) AS mtime "                           /* */
   ", fd.mode " /*    */ " AS filemode "                             /* */
   ", fn." DUF_SQL_IDFIELD " AS filenameid "                         /* */
   ", fn." DUF_SQL_IDFIELD " AS nameid "                             /* */
   ", fd.md5id " /*    */ " AS md5id "                               /* */
 /* ", md." DUF_SQL_IDFIELD " AS md5id " (* *) */
   ", md.md5sum1, md.md5sum2 "                                       /* */
   ", fd." DUF_SQL_IDFIELD " AS filedataid "                         /* */
   ", fd." DUF_SQL_IDFIELD " AS dataid "                             /* */
 /* */
   ", sd.dup2cnt " /*    */ " AS nsame "                             /* */
 /* ", md.dup5cnt            AS nsame_md5 " (* *)  */
 /* ", sh.dupsha1cnt         AS nsame_sha1 " (* *) */
 /* ", x.dupexifcnt          AS nsame_exif " (* *) */
   ", md.dup5cnt " /*    */ " AS dup5cnt "                           /* */
   ", sz.dupzcnt " /*    */ " AS dupzcnt "                           /* */
   }
  ,
  {
   .name = "crc32x",
   .type = DUF_NODE_LEAF,
   .set =
 /* "'crc32x-leaf' AS fieldset_id, " (* *) */
   " crc.dup32cnt " /*    */ " AS nsame "                            /* */
 /* ", md.dup5cnt            AS nsame_md5 " (* *)  */
 /* ", sh.dupsha1cnt         AS nsame_sha1 " (* *) */
 /* ", x.dupexifcnt          AS nsame_exif " (* *) */
   ", sz.dupzcnt " /*    */ " AS dupzcnt "                           /* */
   ", fd.crc32id " /*    */ " AS crc32id"                            /* */
   ", crc.crc32sum "                                                 /* */
   }
  ,
  {
   .name = "crc32",
   .type = DUF_NODE_LEAF,
   .set =
 /* "'crc32-leaf' AS fieldset_id, " (* *) */
   " fn.Pathid AS dirid "                                            /* */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
   ", 0 AS ndirs, 0 AS nfiles"                                       /* */
#endif
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_RNUMS )
   ", 0 AS rndirs, 0 AS rnfiles"                                     /* */
#endif
   ", fn." DUF_SQL_FILENAMEFIELD " AS fname, fn." DUF_SQL_FILENAMEFIELD " AS dfname, fd.size AS filesize " /* */
   ", fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, fd.rdev, fd.blksize, fd.blocks " /* */
   ", STRFTIME( '%s', fd.mtim ) AS mtime "                           /* */
   ", fd.mode " /*    */ " AS filemode "                             /* */
   ", fn." DUF_SQL_IDFIELD " AS filenameid "                         /* */
   ", fn." DUF_SQL_IDFIELD " AS nameid "                             /* */
   ", fd.md5id " /*    */ " AS md5id "                               /* */
 /* ", md." DUF_SQL_IDFIELD " AS md5id " (* *) */
 /* ", md.md5sum1, md.md5sum2 "  (* *) */
   ", fd." DUF_SQL_IDFIELD " AS filedataid "                         /* */
   ", fd." DUF_SQL_IDFIELD " AS dataid "                             /* */
 /* */
   ", crc.dup32cnt " /*    */ " AS nsame "                           /* */
 /* ", md.dup5cnt            AS nsame_md5 " (* *)  */
 /* ", sh.dupsha1cnt         AS nsame_sha1 " (* *) */
 /* ", x.dupexifcnt          AS nsame_exif " (* *) */
 /* ", md.dup5cnt            AS dup5cnt " (* *) */
   ", sz.dupzcnt " /*    */ " AS dupzcnt "                           /* */
   ", fd.crc32id " /*    */ " AS crc32id"                            /* */
   ", crc.crc32sum "                                                 /* */
   }
  ,
  {
   .name = "mime",
   .type = DUF_NODE_LEAF,
   .set =
 /* "'mime-leaf' AS fieldset_id, " (* *) */
   " fn.Pathid AS dirid "                                            /* */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
   ", 0 AS ndirs, 0 AS nfiles"                                       /* */
#endif
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_RNUMS )
   ", 0 AS rndirs, 0 AS rnfiles"                                     /* */
#endif
   ", fn." DUF_SQL_FILENAMEFIELD " AS fname, fn." DUF_SQL_FILENAMEFIELD " AS dfname, fd.size AS filesize  " /* */
   ", fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, fd.rdev, fd.blksize, fd.blocks " /* */
   ", STRFTIME('%s',fd.mtim)   AS mtime "                            /* */
   ", fd.mode " /*    */ " AS filemode "                             /* */
   ", fn." DUF_SQL_IDFIELD " AS filenameid "                         /* */
   ", fn." DUF_SQL_IDFIELD " AS nameid "                             /* */
   ", fd.md5id " /*    */ " AS md5id "                               /* */
 /* ", md." DUF_SQL_IDFIELD " AS md5id " (* *) */
 /* ", md.md5sum1, md.md5sum2 "  (* *) */
   ", fd." DUF_SQL_IDFIELD " AS filedataid "                         /* */
   ", fd." DUF_SQL_IDFIELD " AS dataid "                             /* */
 /* */
   ", mi.dupmimecnt " /*    */ " AS nsame "                          /* */
 /* ", md.dup5cnt            AS nsame_md5 " (* *)  */
 /* ", sh.dupsha1cnt         AS nsame_sha1 " (* *) */
 /* ", x.dupexifcnt          AS nsame_exif " (* *) */
 /* ", md.dup5cnt            AS dup5cnt " (* *) */
   ", sz.dupzcnt " /*    */ " AS dupzcnt "                           /* */
   }
  ,
  {
   .name = "exif",
   .type = DUF_NODE_LEAF,
   .set =
 /* "'exif-leaf' AS fieldset_id, " (* *) */
   " fn.Pathid AS dirid "                                            /* */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
   ", 0 AS ndirs, 0 AS nfiles"                                       /* */
#endif
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_RNUMS )
   ", 0 AS rndirs, 0 AS rnfiles"                                     /* */
#endif
   ", fn." DUF_SQL_FILENAMEFIELD " AS fname, fn." DUF_SQL_FILENAMEFIELD " AS dfname, fd.size AS filesize " /* */
   ", fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, fd.rdev, fd.blksize, fd.blocks " /* */
   ", STRFTIME( '%s', fd.mtim ) AS mtime "                           /* */
   ", fd.mode " /*    */ " AS filemode "                             /* */
   ", fn." DUF_SQL_IDFIELD " AS filenameid "                         /* */
   ", fn." DUF_SQL_IDFIELD " AS nameid "                             /* */
   ", fd.md5id " /*    */ " AS md5id "                               /* */
 /* ", md." DUF_SQL_IDFIELD " AS md5id " (* *) */
   ", md.md5sum1, md.md5sum2 "                                       /* */
   ", fd." DUF_SQL_IDFIELD " AS filedataid "                         /* */
   ", fd." DUF_SQL_IDFIELD " AS dataid "                             /* */
 /* */
   ", md.dup5cnt " /*    */ " AS nsame "                             /* */
 /* ", md.dup5cnt            AS nsame_md5 " (* *)  */
 /* ", sh.dupsha1cnt         AS nsame_sha1 " (* *) */
 /* ", x.dupexifcnt          AS nsame_exif " (* *) */
   ", md.dup5cnt " /*    */ " AS dup5cnt "                           /* */
   ", sz.dupzcnt " /*    */ " AS dupzcnt "                           /* */
   ", fd.exifid AS exifid "                                          /* */
   ", fd.mimeid AS mimeid "                                          /* */
   ", mi.mime AS mime "                                              /* */
   ", STRFTIME( '%s', x.date_time ) AS exifdt "                      /* */
   ", xm.model AS camera "                                           /* */
   }
  ,
  {
   .name = "std-leaf",
   .type = DUF_NODE_LEAF,
   .set =
 /* " 'std-nons-leaf' AS fieldset_id, " (* *) */
   " fn.Pathid AS dirid "                                            /* */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
   ", 0 AS ndirs, 0 AS nfiles"                                       /* */
#endif
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_RNUMS )
   ", 0 AS rndirs, 0 AS rnfiles"                                     /* */
#endif
   ", fn." DUF_SQL_FILENAMEFIELD " AS fname, fn." DUF_SQL_FILENAMEFIELD " AS dfname, fd.size AS filesize " /* */
   ", fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, fd.rdev, fd.blksize, fd.blocks " /* */
   ", STRFTIME( '%s', fd.mtim ) AS mtime "                           /* */
   ", fd.mode " /*    */ " AS filemode "                             /* */
   ", fn." DUF_SQL_IDFIELD " AS filenameid "                         /* */
   ", fn." DUF_SQL_IDFIELD " AS nameid "                             /* */
   ", fd.md5id " /*    */ " AS md5id "                               /* */
 /* ", md." DUF_SQL_IDFIELD " AS md5id " (* *) */
   ", fd.sha1id " /*    */ " AS sha1id "                             /* */
   ", md.md5sum1, md.md5sum2 "                                       /* */
   ", sh.sha1sum1, sh.sha1sum2, sh.sha1sum3 "                        /* */
   ", fd." DUF_SQL_IDFIELD " AS filedataid "                         /* */
   ", fd." DUF_SQL_IDFIELD " AS dataid "                             /* */
 /* */
   ", md.dup5cnt " /*    */ " AS nsame "                             /* */
   ", md.dup5cnt " /*    */ " AS nsame_md5 "                         /* */
   ", sh.dupsha1cnt " /*    */ " AS nsame_sha1 "                     /* */
   ", x.dupexifcnt " /*    */ " AS nsame_exif "                      /* */
   ", md.dup5cnt " /*    */ " AS dup5cnt "                           /* */
   ", sz.dupzcnt " /*    */ " AS dupzcnt "                           /* */
   ", fd.exifid AS exifid "                                          /* */
   ", fd.mimeid AS mimeid "                                          /* */
   ", mi.mime AS mime "                                              /* */
   ", STRFTIME( '%s', x.date_time ) AS exifdt "                      /* */
   ", xm.model AS camera "                                           /* */
   ,
   }
  ,
  {
   .name = "std-node",
   .type = DUF_NODE_NODE,
   .set =
 /* "'std-node' AS fieldset_id, " (* *) */
   " pt." DUF_SQL_IDFIELD " AS dirid"                                /* */
   ", pt." DUF_SQL_IDFIELD " AS nameid "                             /* */
   ", pt." DUF_SQL_DIRNAMEFIELD " AS dname, pt." DUF_SQL_DIRNAMEFIELD " AS dfname, pt.parentid " /* */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
   ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
#endif
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_RNUMS )
   ", " DUF_SQL_RNUMDIRS( pt ) " AS rndirs"                          /* */
   ", (" DUF_SQL__RNUMFILES( pt ) ") AS rnfiles"                     /* */
#endif
   ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks " /* */
   ", STRFTIME( '%s', pt.mtim ) AS mtime "                           /* */
   }
  ,
  {
   .name = "std-ns-node",
   .type = DUF_NODE_NODE,
   .set =
 /* "'std-ns-node' AS fieldset_id, " (* *) */
   " pt." DUF_SQL_IDFIELD " AS dirid"                                /* */
   ", pt." DUF_SQL_IDFIELD " AS nameid "                             /* */
   ", pt." DUF_SQL_DIRNAMEFIELD " AS dname, pt." DUF_SQL_DIRNAMEFIELD " AS dfname, pt.parentid " /* */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
   ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
#endif
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_RNUMS )
   ", " DUF_SQL_RNUMDIRS( pt ) " AS rndirs"                          /* */
   ", (" DUF_SQL__RNUMFILES( pt ) ") AS rnfiles"                     /* */
#endif
   ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks " /* */
   ", STRFTIME( '%s', pt.mtim ) AS mtime "                           /* */
   }
  ,
  {
   .name = "template",
   .type = DUF_NODE_NODE,
   .set =
 /* "'tmpl-node' AS fieldset_id, " (* *) */
   " pt." DUF_SQL_IDFIELD " AS dirid"                                /* */
   ", pt." DUF_SQL_IDFIELD " AS nameid "                             /* */
   ", pt." DUF_SQL_DIRNAMEFIELD " AS dname, pt." DUF_SQL_DIRNAMEFIELD " AS dfname, pt.parentid " /* */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
   ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
#endif
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_RNUMS )
   ", " DUF_SQL_RNUMDIRS( pt ) " AS rndirs"                          /* */
   ", (" DUF_SQL__RNUMFILES( pt ) ") AS rnfiles"                     /* */
#endif
   ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks " /* */
   ", STRFTIME( '%s', pt.mtim ) AS mtime "                           /* */
   }
  ,
  {
   .name = "filenames",
   .type = DUF_NODE_NODE,
   .set =
 /* "'filenames-node' AS fieldset_id, " (* *) */
   " pt." DUF_SQL_IDFIELD " AS dirid"                                /* */
   ", pt." DUF_SQL_IDFIELD " AS nameid "                             /* */
   ", pt." DUF_SQL_DIRNAMEFIELD " AS dname, pt." DUF_SQL_DIRNAMEFIELD " AS dfname, pt.parentid " /* */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
   ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
#endif
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_RNUMS )
   ", " DUF_SQL_RNUMDIRS( pt ) " AS rndirs"                          /* */
   ", (" DUF_SQL__RNUMFILES( pt ) ") AS rnfiles"                     /* */
#endif
   ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks " /* */
   ", STRFTIME( '%s', pt.mtim ) AS mtime "                           /* */
   }
  ,
  {
   .name = "filedata",
   .type = DUF_NODE_NODE,
   .set =
 /* "'filedata-node' AS fieldset_id, " (* *) */
   " pt." DUF_SQL_IDFIELD " AS dirid"                                /* */
   ", pt." DUF_SQL_IDFIELD " AS nameid "                             /* */
   ", pt." DUF_SQL_DIRNAMEFIELD " AS dname, pt." DUF_SQL_DIRNAMEFIELD " AS dfname, pt.parentid " /* */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
   ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
#endif
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_RNUMS )
   ", " DUF_SQL_RNUMDIRS( pt ) " AS rndirs"                          /* */
   ", (" DUF_SQL__RNUMFILES( pt ) ") AS rnfiles"                     /* */
#endif
   ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks " /* */
   ", STRFTIME( '%s', pt.mtim ) AS mtime "                           /* */
   }
  ,
  {
   .name = "dirs",
   .type = DUF_NODE_NODE,
   .set =
 /* "'dirs-node' AS fieldset_id, " (* *) */
   " pt." DUF_SQL_IDFIELD " AS dirid "                               /* */
   ", pt." DUF_SQL_IDFIELD " AS nameid "                             /* */
   ", pt." DUF_SQL_DIRNAMEFIELD " AS dname, pt." DUF_SQL_DIRNAMEFIELD " AS dfname, pt.parentid " /* */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
   ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
#endif
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_RNUMS )
   ", " DUF_SQL_RNUMDIRS( pt ) " AS rndirs"                          /* */
   ", (" DUF_SQL__RNUMFILES( pt ) ") AS rnfiles"                     /* */
#endif
   ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks " /* */
   ", STRFTIME( '%s', pt.mtim ) AS mtime "                           /* */
   }
  ,
  {
   .name = "",
   .type = DUF_NODE_NODE,
   .set =
 /* "'md5-node' AS fieldset_id, " (* *) */
   " pt." DUF_SQL_IDFIELD " AS dirid"                                /* */
   ", pt." DUF_SQL_IDFIELD " AS nameid "                             /* */
   ", pt." DUF_SQL_DIRNAMEFIELD " AS dname, pt." DUF_SQL_DIRNAMEFIELD " AS dfname, pt.parentid " /* */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
   ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
#endif
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_RNUMS )
   ", " DUF_SQL_RNUMDIRS( pt ) " AS rndirs"                          /* */
   ", (" DUF_SQL__RNUMFILES( pt ) ") AS rnfiles"                     /* */
#endif
   ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks " /* */
   ", STRFTIME( '%s', pt.mtim ) AS mtime "                           /* */
   }
  ,
  {
   .name = "",
   .type = DUF_NODE_NODE,
   .set =
 /* "'sd5-node' AS fieldset_id, " (* *) */
   " pt." DUF_SQL_IDFIELD " AS dirid"                                /* */
   ", pt." DUF_SQL_IDFIELD " AS nameid "                             /* */
   ", pt." DUF_SQL_DIRNAMEFIELD " AS dname, pt." DUF_SQL_DIRNAMEFIELD " AS dfname, pt.parentid " /* */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
   ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
#endif
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_RNUMS )
   ", " DUF_SQL_RNUMDIRS( pt ) " AS rndirs"                          /* */
   ", (" DUF_SQL__RNUMFILES( pt ) ") AS rnfiles"                     /* */
#endif
   ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks " /* */
   ", STRFTIME( '%s', pt.mtim ) AS mtime "                           /* */
   }
  ,
  {
   .name = "",
   .type = DUF_NODE_NODE,
   .set =
 /* "'crc32-node' AS fieldset_id, " (* *) */
   " pt." DUF_SQL_IDFIELD " AS dirid"                                /* */
   ", pt." DUF_SQL_IDFIELD " AS nameid "                             /* */
   ", pt." DUF_SQL_DIRNAMEFIELD " AS dname, pt." DUF_SQL_DIRNAMEFIELD " AS dfname, pt.parentid " /* */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
   ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
#endif
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_RNUMS )
   ", " DUF_SQL_RNUMDIRS( pt ) " AS rndirs"                          /* */
   ", (" DUF_SQL__RNUMFILES( pt ) ") AS rnfiles"                     /* */
#endif
   ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks " /* */
   ", STRFTIME( '%s', pt.mtim ) AS mtime "                           /* */
   }
  ,
  {
   .name = "",
   .type = DUF_NODE_NODE,
   .set =
 /* "'mime-node' AS fieldset_id, " (* *) */
   " pt." DUF_SQL_IDFIELD " AS dirid"                                /* */
   ", pt." DUF_SQL_IDFIELD " AS nameid "                             /* */
   ", pt." DUF_SQL_DIRNAMEFIELD " AS dname, pt." DUF_SQL_DIRNAMEFIELD " AS dfname, pt.parentid " /* */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
   ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
#endif
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_RNUMS )
   ", " DUF_SQL_RNUMDIRS( pt ) " AS rndirs"                          /* */
   ", (" DUF_SQL__RNUMFILES( pt ) ") AS rnfiles"                     /* */
#endif
   ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks " /* */
   ", STRFTIME( '%s', pt.mtim ) AS mtime "                           /* */
   }
  ,
  {
   .name = "",
   .type = DUF_NODE_NODE,
   .set =
 /* "'exif-node' AS fieldset_id, " (* *) */
   " pt." DUF_SQL_IDFIELD " AS dirid"                                /* */
   ", pt." DUF_SQL_IDFIELD " AS nameid "                             /* */
   ", pt." DUF_SQL_DIRNAMEFIELD " AS dname, pt." DUF_SQL_DIRNAMEFIELD " AS dfname, pt.parentid " /* */
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_NUMS )
   ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
#endif
#ifndef MAS_DUF_DEFS_H
# error use #include "duf_defs.h"
#elif defined( DUF_DO_RNUMS )
   ", " DUF_SQL_RNUMDIRS( pt ) " AS rndirs"                          /* */
   ", (" DUF_SQL__RNUMFILES( pt ) ") AS rnfiles"                     /* */
#endif
   ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks " /* */
   ", STRFTIME( '%s', pt.mtim ) AS mtime "                           /* */
   }
  ,
  {.name = NULL,.set = NULL}
};

duf_fieldset_t *all_fieldsets = _all_fieldsets;
