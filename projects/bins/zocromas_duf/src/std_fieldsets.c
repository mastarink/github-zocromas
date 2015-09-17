#include "duf_maintenance.h"

#include "duf_sql_defs.h"


#include "duf_mod_defs.h"

#include "sql_beginning_tables.h"

#include "duf_hook_types.h"

#include "duf_scan_types.h"
/* ###################################################################### */
#include "std_fieldsets.h"
/* ###################################################################### */

static duf_fieldset_t _all_fieldsets[] = {
  {
   .name = "template",
   .type = DUF_NODE_LEAF,
   .set =
   /* "'tmpl-leaf' AS fieldset_id, " (* *) */
   " fn.Pathid AS dirid "       /* */
   ", 0 AS ndirs, 0 AS nfiles"  /* */
   ", fn.name AS filename, fn.name AS dfname, fd.size AS filesize " /* */
   ", fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, fd.rdev, fd.blksize, fd.blocks " /* */
   ", STRFTIME( '%s', fd.mtim ) AS mtime " /* */
   ", fd.mode               AS filemode " /* */
   ", fn." DUF_SQL_IDNAME " AS filenameid " /* */
   ", fn." DUF_SQL_IDNAME " AS nameid " /* */
   ", fd.md5id              AS md5id " /* */
   /* ", md." DUF_SQL_IDNAME " AS md5id " (* *) */
   ", md.md5sum1, md.md5sum2 "  /* */
   ", fd." DUF_SQL_IDNAME " AS filedataid " /* */
   ", fd." DUF_SQL_IDNAME " AS dataid " /* */
   /* */
   ", md.dup5cnt            AS nsame " /* */
   ", md.dup5cnt            AS dup5cnt " /* */
   ", sz.dupzcnt            AS dupzcnt " /* */
   ", fd.exifid AS exifid "     /* */
   ", fd.mimeid AS mimeid "     /* */
   ", mi.mime AS mime "         /* */
   ", STRFTIME( '%s', x.date_time ) AS exifdt " /* */
   ", xm.model AS camera "      /* */
   }
  ,
  {
   .name = "basic",
   .type = DUF_NODE_LEAF,
   .set =
   /* "'basic-leaf' AS fieldset_id, " (* *) */
   " fn.Pathid AS dirid "       /* */
   ", 0 AS ndirs, 0 AS nfiles"  /* */
   ", fn.name AS filename, fn.name AS dfname, fd.size AS filesize " /* */
   }
  ,
  {
   .name = "plus",
   .type = DUF_NODE_LEAF,
   .set =
   /* "'plus-leaf' AS fieldset_id, " (* *) */
   " fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, fd.rdev, fd.blksize, fd.blocks " /* */
   ", STRFTIME( '%s', fd.mtim ) AS mtime " /* */
   ", fd.mode               AS filemode " /* */
   ", fn." DUF_SQL_IDNAME " AS filenameid " /* */
   ", fn." DUF_SQL_IDNAME " AS nameid " /* */
   ", fd.md5id              AS md5id " /* */
   ", fd." DUF_SQL_IDNAME " AS filedataid " /* */
   ", fd." DUF_SQL_IDNAME " AS dataid " /* */
   }
  ,
  {
   .name = "md5x",
   .type = DUF_NODE_LEAF,
   .set =
   /* "'md5x-leaf' AS fieldset_id, " (* *) */
   /* */
   " md.dup5cnt            AS nsame " /* */
   ", md.dup5cnt            AS dup5cnt " /* */
   ", sz.dupzcnt            AS dupzcnt " /* */
   }
  ,
  {
   .name = "md5",
   .type = DUF_NODE_LEAF,
   .set =
   /* "'md5-leaf' AS fieldset_id, " (* *) */
   " fn.Pathid AS dirid "       /* */
   ", 0 AS ndirs, 0 AS nfiles"  /* */
   ", fn.name AS filename, fn.name AS dfname, fd.size AS filesize " /* */
   ", fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, fd.rdev, fd.blksize, fd.blocks " /* */
   ", STRFTIME( '%s', fd.mtim ) AS mtime " /* */
   ", fd.mode               AS filemode " /* */
   ", fn." DUF_SQL_IDNAME " AS filenameid " /* */
   ", fn." DUF_SQL_IDNAME " AS nameid " /* */
   ", fd.md5id              AS md5id " /* */
   /* ", md." DUF_SQL_IDNAME " AS md5id " (* *) */
   ", md.md5sum1, md.md5sum2 "  /* */
   ", fd." DUF_SQL_IDNAME " AS filedataid " /* */
   ", fd." DUF_SQL_IDNAME " AS dataid " /* */
   /* */
   ", md.dup5cnt            AS nsame " /* */
   ", md.dup5cnt            AS dup5cnt " /* */
   ", sz.dupzcnt            AS dupzcnt " /* */
   }
  ,
  {
   .name = "sd5",
   .type = DUF_NODE_LEAF,
   .set =
   /* "'sd5-leaf' AS fieldset_id, " (* *) */
   " fn.Pathid AS dirid "       /* */
   ", 0 AS ndirs, 0 AS nfiles"  /* */
   ", fn.name AS filename, fn.name AS dfname, fd.size AS filesize " /* */
   ", fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, fd.rdev, fd.blksize, fd.blocks " /* */
   ", STRFTIME( '%s', fd.mtim ) AS mtime " /* */
   ", fd.mode               AS filemode " /* */
   ", fn." DUF_SQL_IDNAME " AS filenameid " /* */
   ", fn." DUF_SQL_IDNAME " AS nameid " /* */
   ", fd.md5id              AS md5id " /* */
   /* ", md." DUF_SQL_IDNAME " AS md5id " (* *) */
   ", md.md5sum1, md.md5sum2 "  /* */
   ", fd." DUF_SQL_IDNAME " AS filedataid " /* */
   ", fd." DUF_SQL_IDNAME " AS dataid " /* */
   /* */
   ", sd.dup2cnt            AS nsame " /* */
   ", md.dup5cnt            AS dup5cnt " /* */
   ", sz.dupzcnt            AS dupzcnt " /* */
   }
  ,
  {
   .name = "crc32",
   .type = DUF_NODE_LEAF,
   .set =
   /* "'crc32-leaf' AS fieldset_id, " (* *) */
   " fn.Pathid AS dirid "       /* */
   ", 0 AS ndirs, 0 AS nfiles"  /* */
   ", fn.name AS filename, fn.name AS dfname, fd.size AS filesize " /* */
   ", fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, fd.rdev, fd.blksize, fd.blocks " /* */
   ", STRFTIME( '%s', fd.mtim ) AS mtime " /* */
   ", fd.mode               AS filemode " /* */
   ", fn." DUF_SQL_IDNAME " AS filenameid " /* */
   ", fn." DUF_SQL_IDNAME " AS nameid " /* */
   ", fd.md5id              AS md5id " /* */
   /* ", md." DUF_SQL_IDNAME " AS md5id " (* *) */
   /* ", md.md5sum1, md.md5sum2 "  (* *) */
   ", fd." DUF_SQL_IDNAME " AS filedataid " /* */
   ", fd." DUF_SQL_IDNAME " AS dataid " /* */
   /* */
   ", crc.dup32cnt          AS nsame " /* */
   /* ", md.dup5cnt            AS dup5cnt " (* *) */
   ", sz.dupzcnt            AS dupzcnt " /* */
   ", fd.crc32id            AS crc32id" /* */
   ", crc.crc32sum "            /* */
   }
  ,
  {
   .name = "mime",
   .type = DUF_NODE_LEAF,
   .set =
   /* "'mime-leaf' AS fieldset_id, " (* *) */
   " fn.Pathid AS dirid "       /* */
   ", 0 AS ndirs, 0 AS nfiles"  /* */
   ", fn.name AS filename, fn.name AS dfname, fd.size AS filesize  " /* */
   ", fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, fd.rdev, fd.blksize, fd.blocks " /* */
   ", STRFTIME('%s',fd.mtim)   AS mtime " /* */
   ", fd.mode               AS filemode " /* */
   ", fn." DUF_SQL_IDNAME " AS filenameid " /* */
   ", fn." DUF_SQL_IDNAME " AS nameid " /* */
   ", fd.md5id              AS md5id " /* */
   /* ", md." DUF_SQL_IDNAME " AS md5id " (* *) */
   /* ", md.md5sum1, md.md5sum2 "  (* *) */
   ", fd." DUF_SQL_IDNAME " AS filedataid " /* */
   ", fd." DUF_SQL_IDNAME " AS dataid " /* */
   /* */
   ", mi.dupmimecnt         AS nsame " /* */
   /* ", md.dup5cnt            AS dup5cnt " (* *) */
   ", sz.dupzcnt            AS dupzcnt " /* */
   }
  ,
  {
   .name = "exif",
   .type = DUF_NODE_LEAF,
   .set =
   /* "'exif-leaf' AS fieldset_id, " (* *) */
   " fn.Pathid AS dirid "       /* */
   ", 0 AS ndirs, 0 AS nfiles"  /* */
   ", fn.name AS filename, fn.name AS dfname, fd.size AS filesize " /* */
   ", fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, fd.rdev, fd.blksize, fd.blocks " /* */
   ", STRFTIME( '%s', fd.mtim ) AS mtime " /* */
   ", fd.mode               AS filemode " /* */
   ", fn." DUF_SQL_IDNAME " AS filenameid " /* */
   ", fn." DUF_SQL_IDNAME " AS nameid " /* */
   ", fd.md5id              AS md5id " /* */
   /* ", md." DUF_SQL_IDNAME " AS md5id " (* *) */
   ", md.md5sum1, md.md5sum2 "  /* */
   ", fd." DUF_SQL_IDNAME " AS filedataid " /* */
   ", fd." DUF_SQL_IDNAME " AS dataid " /* */
   /* */
   ", md.dup5cnt            AS nsame " /* */
   ", md.dup5cnt            AS dup5cnt " /* */
   ", sz.dupzcnt            AS dupzcnt " /* */
   ", fd.exifid AS exifid "     /* */
   ", fd.mimeid AS mimeid "     /* */
   ", mi.mime AS mime "         /* */
   ", STRFTIME( '%s', x.date_time ) AS exifdt " /* */
   ", xm.model AS camera "      /* */
   }
  ,
  {
   .name = "std-leaf",
   .type = DUF_NODE_LEAF,
   .set =
   /* " 'std-nons-leaf' AS fieldset_id, " (* *) */
   " fn.Pathid AS dirid "       /* */
   ", 0 AS ndirs, 0 AS nfiles"  /* */
   ", fn.name AS filename, fn.name AS dfname, fd.size AS filesize " /* */
   ", fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, fd.rdev, fd.blksize, fd.blocks " /* */
   ", STRFTIME( '%s', fd.mtim ) AS mtime " /* */
   ", fd.mode               AS filemode " /* */
   ", fn." DUF_SQL_IDNAME " AS filenameid " /* */
   ", fn." DUF_SQL_IDNAME " AS nameid " /* */
   ", fd.md5id              AS md5id " /* */
   /* ", md." DUF_SQL_IDNAME " AS md5id " (* *) */
   ", md.md5sum1, md.md5sum2 "  /* */
   ", fd." DUF_SQL_IDNAME " AS filedataid " /* */
   ", fd." DUF_SQL_IDNAME " AS dataid " /* */
   /* */
   ", md.dup5cnt            AS nsame " /* */
   ", md.dup5cnt            AS dup5cnt " /* */
   ", sz.dupzcnt            AS dupzcnt " /* */
   ", fd.exifid AS exifid "     /* */
   ", fd.mimeid AS mimeid "     /* */
   ", mi.mime AS mime "         /* */
   ", STRFTIME( '%s', x.date_time ) AS exifdt " /* */
   ", xm.model AS camera "      /* */
   ,
   }
  ,
  {
   .name = "std-ns-leaf",       /* same as std-leaf */
   .type = DUF_NODE_LEAF,
   .set =
   /* " 'std-ns-leaf' AS fieldset_id, " (* *) */
   " fn.Pathid AS dirid "       /* */
   ", 0 AS ndirs, 0 AS nfiles"  /* */
   ", fn.name AS filename, fn.name AS dfname, fd.size AS filesize " /* */
   ", fd.dev, fd.uid, fd.gid, fd.nlink, fd.inode, fd.rdev, fd.blksize, fd.blocks " /* */
   ", STRFTIME( '%s', fd.mtim ) AS mtime " /* */
   ", fd.mode               AS filemode " /* */
   ", fn." DUF_SQL_IDNAME " AS filenameid " /* */
   ", fn." DUF_SQL_IDNAME " AS nameid " /* */
   ", fd.md5id              AS md5id " /* */
   /* ", md." DUF_SQL_IDNAME " AS md5id " (* *) */
   ", md.md5sum1, md.md5sum2 "  /* */
   ", fd." DUF_SQL_IDNAME " AS filedataid " /* */
   ", fd." DUF_SQL_IDNAME " AS dataid " /* */
   ", sz.dupzcnt            AS dupzcnt " /* */
   /* */
   ", md.dup5cnt            AS nsame " /* */
   ", md.dup5cnt            AS dup5cnt " /* */
   ", sz.dupzcnt            AS dupzcnt " /* */
   ", fd.exifid AS exifid "     /* */
   ", fd.mimeid AS mimeid "     /* */
   ", mi.mime AS mime "         /* */
   ", STRFTIME( '%s', x.date_time ) AS exifdt " /* */
   ", xm.model AS camera "      /* */
   ,
   }
  ,
  {
   .name = "std-node",
   .type = DUF_NODE_NODE,
   .set =
   /* "'std-node' AS fieldset_id, " (* *) */
   " pt." DUF_SQL_IDNAME " AS dirid" /* */
   ", pt." DUF_SQL_IDNAME " AS nameid " /* */
   ", pt.dirname, pt.dirname AS dfname, pt.parentid " /* */
   ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
   ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks " /* */
   ", STRFTIME( '%s', pt.mtim ) AS mtime " /* */
   }
  ,
  {
   .name = "std-ns-node",
   .type = DUF_NODE_NODE,
   .set =
   /* "'std-ns-node' AS fieldset_id, " (* *) */
   " pt." DUF_SQL_IDNAME " AS dirid" /* */
   ", pt." DUF_SQL_IDNAME " AS nameid " /* */
   ", pt.dirname, pt.dirname AS dfname, pt.parentid " /* */
   ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
   ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks " /* */
   ", STRFTIME( '%s', pt.mtim ) AS mtime " /* */
   }
  ,
  {
   .name = "template",
   .type = DUF_NODE_NODE,
   .set =
   /* "'tmpl-node' AS fieldset_id, " (* *) */
   " pt." DUF_SQL_IDNAME " AS dirid" /* */
   ", pt." DUF_SQL_IDNAME " AS nameid " /* */
   ", pt.dirname, pt.dirname AS dfname, pt.parentid " /* */
   ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
   ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks " /* */
   ", STRFTIME( '%s', pt.mtim ) AS mtime " /* */
   }
  ,
  {
   .name = "filenames",
   .type = DUF_NODE_NODE,
   .set =
   /* "'filenames-node' AS fieldset_id, " (* *) */
   " pt." DUF_SQL_IDNAME " AS dirid" /* */
   ", pt." DUF_SQL_IDNAME " AS nameid " /* */
   ", pt.dirname, pt.dirname AS dfname, pt.parentid " /* */
   ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
   ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks " /* */
   ", STRFTIME( '%s', pt.mtim ) AS mtime " /* */
   }
  ,
  {
   .name = "filedata",
   .type = DUF_NODE_NODE,
   .set =
   /* "'filedata-node' AS fieldset_id, " (* *) */
   " pt." DUF_SQL_IDNAME " AS dirid" /* */
   ", pt." DUF_SQL_IDNAME " AS nameid " /* */
   ", pt.dirname, pt.dirname AS dfname, pt.parentid " /* */
   ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
   ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks " /* */
   ", STRFTIME( '%s', pt.mtim ) AS mtime " /* */
   }
  ,
  {
   .name = "dirs",
   .type = DUF_NODE_NODE,
   .set =
   /* "'dirs-node' AS fieldset_id, " (* *) */
   " pt." DUF_SQL_IDNAME " AS dirid " /* */
   ", pt." DUF_SQL_IDNAME " AS nameid " /* */
   ", pt.dirname, pt.dirname AS dfname, pt.parentid " /* */
   ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
   ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks " /* */
   ", STRFTIME( '%s', pt.mtim ) AS mtime " /* */
   }
  ,
  {
   .name = "",
   .type = DUF_NODE_NODE,
   .set =
   /* "'md5-node' AS fieldset_id, " (* *) */
   " pt." DUF_SQL_IDNAME " AS dirid" /* */
   ", pt." DUF_SQL_IDNAME " AS nameid " /* */
   ", pt.dirname, pt.dirname AS dfname, pt.parentid " /* */
   ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
   ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks " /* */
   ", STRFTIME( '%s', pt.mtim ) AS mtime " /* */
   }
  ,
  {
   .name = "",
   .type = DUF_NODE_NODE,
   .set =
   /* "'sd5-node' AS fieldset_id, " (* *) */
   " pt." DUF_SQL_IDNAME " AS dirid" /* */
   ", pt." DUF_SQL_IDNAME " AS nameid " /* */
   ", pt.dirname, pt.dirname AS dfname, pt.parentid " /* */
   ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
   ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks " /* */
   ", STRFTIME( '%s', pt.mtim ) AS mtime " /* */
   }
  ,
  {
   .name = "",
   .type = DUF_NODE_NODE,
   .set =
   /* "'crc32-node' AS fieldset_id, " (* *) */
   " pt." DUF_SQL_IDNAME " AS dirid" /* */
   ", pt." DUF_SQL_IDNAME " AS nameid " /* */
   ", pt.dirname, pt.dirname AS dfname, pt.parentid " /* */
   ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
   ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks " /* */
   ", STRFTIME( '%s', pt.mtim ) AS mtime " /* */
   }
  ,
  {
   .name = "",
   .type = DUF_NODE_NODE,
   .set =
   /* "'mime-node' AS fieldset_id, " (* *) */
   " pt." DUF_SQL_IDNAME " AS dirid" /* */
   ", pt." DUF_SQL_IDNAME " AS nameid " /* */
   ", pt.dirname, pt.dirname AS dfname, pt.parentid " /* */
   ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
   ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks " /* */
   ", STRFTIME( '%s', pt.mtim ) AS mtime " /* */
   }
  ,
  {
   .name = "",
   .type = DUF_NODE_NODE,
   .set =
   /* "'exif-node' AS fieldset_id, " (* *) */
   " pt." DUF_SQL_IDNAME " AS dirid" /* */
   ", pt." DUF_SQL_IDNAME " AS nameid " /* */
   ", pt.dirname, pt.dirname AS dfname, pt.parentid " /* */
   ", tf.numfiles AS nfiles, td.numdirs AS ndirs, tf.maxsize AS maxsize, tf.minsize AS minsize " /* */
   ", pt.size AS filesize, pt.mode AS filemode, pt.dev, pt.uid, pt.gid, pt.nlink, pt.inode, pt.rdev, pt.blksize, pt.blocks " /* */
   ", STRFTIME( '%s', pt.mtim ) AS mtime " /* */
   }
  ,
  {.name = NULL,.set = NULL}
};

duf_fieldset_t *all_fieldsets = _all_fieldsets;
