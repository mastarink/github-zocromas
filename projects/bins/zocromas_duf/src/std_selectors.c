#include "duf_maintenance.h"

#include "duf_sql_defs.h"


#include "duf_mod_defs.h"

/* #include "sql_beginning_selected.h" */
#include "sql_beginning_tables.h"

#include "duf_hook_types.h"

#include "sql_selected_defs.h"
#include "duf_scan_types.h"
/* ###################################################################### */
#include "std_selectors.h"
/* ###################################################################### */


static duf_selector_t _all_selectors[] = {
  {
   .name = "std-leaf",
   .type = DUF_NODE_LEAF,
   .selector2 = " FROM " DUF_SQL_SELECTED_TMP_FILENAMES_FULL /* */ " AS fns " /* */
   " JOIN " DUF_SQL_TABLES_FILENAMES_FULL /*                    */ " AS fn ON (fns.nameid=fn." DUF_SQL_IDFIELD ") " /* */
   " LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL /*               */ " AS fd ON (fn.dataid=fd." DUF_SQL_IDFIELD ") " /* */
   " LEFT JOIN " DUF_SQL_TABLES_SIZES_FULL /*                   */ " AS sz ON (sz.size=fd.size)" /* */
   " LEFT JOIN " DUF_SQL_TABLES_MD5_FULL /*                     */ " AS md ON (md." DUF_SQL_IDFIELD "=fd.md5id) " /* */
   " LEFT JOIN " DUF_SQL_TABLES_SD5_FULL /*                     */ " AS sd ON (sd." DUF_SQL_IDFIELD "=fd.sd5id)" /* */
   " LEFT JOIN " DUF_SQL_TABLES_EXIF_FULL /*                    */ " AS x  ON (x." DUF_SQL_IDFIELD "=fd.exifid) " /* */
   " LEFT JOIN " DUF_SQL_TABLES_EXIF_MODEL_FULL /*              */ " AS xm ON (x.modelid=xm." DUF_SQL_IDFIELD ") " /* */
   " LEFT JOIN " DUF_SQL_TABLES_MIME_FULL /*                    */ " AS mi ON (mi." DUF_SQL_IDFIELD "=fd.mimeid) " /* */
   " LEFT JOIN " DUF_SQL_TABLES_SHA1_FULL /*                    */ " AS sh ON (sh." DUF_SQL_IDFIELD "=fd.sha1id) " /* */
   " LEFT JOIN " DUF_SQL_TABLES_CRC32_FULL /*                   */ " AS crc ON (crc." DUF_SQL_IDFIELD "=fd.crc32id)" /* */
   },
  {
   .name = "std-ns-leaf",       /* ns: without selected table(s) */
   .type = DUF_NODE_LEAF,
   .selector2 = " FROM " DUF_SQL_TABLES_FILENAMES_FULL /*       */ " AS fn " /* */
   /* " JOIN " DUF_SQL_TABLES_FILENAMES_FULL " AS fn ON (fns." DUF_SQL_IDFIELD "=fn." DUF_SQL_IDFIELD ") " (* *) */
   " LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL /*               */ " AS fd ON (fn.dataid=fd." DUF_SQL_IDFIELD ") " /* */
   " LEFT JOIN " DUF_SQL_TABLES_SIZES_FULL /*                   */ " AS sz ON (sz.size=fd.size)" /* */
   " LEFT JOIN " DUF_SQL_TABLES_MD5_FULL /*                     */ " AS md ON (md." DUF_SQL_IDFIELD "=fd.md5id) " /* */
   " LEFT JOIN " DUF_SQL_TABLES_SD5_FULL /*                     */ " AS sd ON (sd." DUF_SQL_IDFIELD "=fd.sd5id)" /* */
   " LEFT JOIN " DUF_SQL_TABLES_EXIF_FULL /*                    */ " AS x  ON (x." DUF_SQL_IDFIELD "=fd.exifid) " /* */
   " LEFT JOIN " DUF_SQL_TABLES_EXIF_MODEL_FULL /*              */ " AS xm ON (x.modelid=xm." DUF_SQL_IDFIELD ") " /* */
   " LEFT JOIN " DUF_SQL_TABLES_MIME_FULL /*                    */ " AS mi ON (mi." DUF_SQL_IDFIELD "=fd.mimeid) " /* */
   " LEFT JOIN " DUF_SQL_TABLES_SHA1_FULL /*                    */ " AS sh ON (sh." DUF_SQL_IDFIELD "=fd.sha1id) " /* */
   " LEFT JOIN " DUF_SQL_TABLES_CRC32_FULL /*                   */ " AS crc ON (crc." DUF_SQL_IDFIELD "=fd.crc32id)" /* */
   },
  {
   .name = "md5-leaf",          /* ns: without selected table(s) */
   .type = DUF_NODE_LEAF,
   .selector2 = " FROM " DUF_SQL_TABLES_FILENAMES_FULL /*       */ " AS fn " /* */
   /* " JOIN " DUF_SQL_TABLES_FILENAMES_FULL " AS fn ON (fns." DUF_SQL_IDFIELD "=fn." DUF_SQL_IDFIELD ") " (* *) */
   " LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL /*               */ " AS fd ON (fn.dataid=fd." DUF_SQL_IDFIELD ") " /* */
   " LEFT JOIN " DUF_SQL_TABLES_SIZES_FULL /*                   */ " AS sz ON (sz.size=fd.size)" /* */
   " LEFT JOIN " DUF_SQL_TABLES_MD5_FULL /*                     */ " AS md ON (md." DUF_SQL_IDFIELD "=fd.md5id) " /* */
   " LEFT JOIN " DUF_SQL_TABLES_SD5_FULL /*                     */ " AS sd ON (sd." DUF_SQL_IDFIELD "=fd.sd5id)" /* */
   " LEFT JOIN " DUF_SQL_TABLES_SHA1_FULL /*                     */ " AS sh ON (sh." DUF_SQL_IDFIELD "=fd.sha1id) " /* */
   },
  {
   .name = "sha1-leaf",         /* ns: without selected table(s) */
   .type = DUF_NODE_LEAF,
   .selector2 = " FROM " DUF_SQL_TABLES_FILENAMES_FULL /*       */ " AS fn " /* */
   /* " JOIN " DUF_SQL_TABLES_FILENAMES_FULL " AS fn ON (fns." DUF_SQL_IDFIELD "=fn." DUF_SQL_IDFIELD ") " (* *) */
   " LEFT JOIN " DUF_SQL_TABLES_FILEDATAS_FULL /*               */ " AS fd ON (fn.dataid=fd." DUF_SQL_IDFIELD ") " /* */
   " LEFT JOIN " DUF_SQL_TABLES_SIZES_FULL /*                   */ " AS sz ON (sz.size=fd.size)" /* */
   " LEFT JOIN " DUF_SQL_TABLES_SHA1_FULL /*                     */ " AS sh ON (sh." DUF_SQL_IDFIELD "=fd.sha1id) " /* */
   " LEFT JOIN " DUF_SQL_TABLES_MD5_FULL /*                     */ " AS md ON (md." DUF_SQL_IDFIELD "=fd.md5id) " /* */
   " LEFT JOIN " DUF_SQL_TABLES_SD5_FULL /*                     */ " AS sd ON (sd." DUF_SQL_IDFIELD "=fd.sd5id)" /* */
   },
  {.name = NULL,.selector2 = NULL}
};

duf_selector_t *all_selectors = _all_selectors;
