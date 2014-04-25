#ifndef MAS_DUF_SQL_DEF_H
#  define MAS_DUF_SQL_DEF_H

#  include <sqlite3.h>

typedef enum
{

  DUF_SQLITE_ERROR_BASE = -59999,
  DUF_SQL_ERROR = DUF_SQLITE_ERROR_BASE + SQLITE_ERROR, /* #  define SQLITE_ERROR        1 (* SQL error or missing database *)               */
  DUF_SQL_INTERNAL = DUF_SQLITE_ERROR_BASE + SQLITE_INTERNAL, /* #  define SQLITE_INTERNAL     2 (* Internal logic error in SQLite *)              */
  DUF_SQL_PERM = DUF_SQLITE_ERROR_BASE + SQLITE_PERM, /* #  define SQLITE_PERM         3 (* Access permission denied *)                    */
  DUF_SQL_ABORT = DUF_SQLITE_ERROR_BASE + SQLITE_ABORT, /* #  define SQLITE_ABORT        4 (* Callback routine requested an abort *)         */
  DUF_SQL_BUSY = DUF_SQLITE_ERROR_BASE + SQLITE_BUSY, /* #  define SQLITE_BUSY         5 (* The database file is locked *)                 */
  DUF_SQL_LOCKED = DUF_SQLITE_ERROR_BASE + SQLITE_LOCKED, /* #  define SQLITE_LOCKED       6 (* A table in the database is locked *)           */
  DUF_SQL_NOMEM = DUF_SQLITE_ERROR_BASE + SQLITE_NOMEM, /* #  define SQLITE_NOMEM        7 (* A malloc() failed *)                           */
  DUF_SQL_READONLY = DUF_SQLITE_ERROR_BASE + SQLITE_READONLY, /* #  define SQLITE_READONLY     8 (* Attempt to write a readonly database *)        */
  DUF_SQL_INTERRUPT = DUF_SQLITE_ERROR_BASE + SQLITE_INTERRUPT, /* #  define SQLITE_INTERRUPT    9 (* Operation terminated by sqlite3_interrupt() *) */
  DUF_SQL_IOERR = DUF_SQLITE_ERROR_BASE + SQLITE_IOERR, /* #  define SQLITE_IOERR       10 (* Some kind of disk I/O error occurred *)        */
  DUF_SQL_CORRUPT = DUF_SQLITE_ERROR_BASE + SQLITE_CORRUPT, /* #  define SQLITE_CORRUPT     11 (* The database disk image is malformed *)        */
  DUF_SQL_NOTFOUND = DUF_SQLITE_ERROR_BASE + SQLITE_NOTFOUND, /* #  define SQLITE_NOTFOUND    12 (* Unknown opcode in sqlite3_file_control() *)    */
  DUF_SQL_FULL = DUF_SQLITE_ERROR_BASE + SQLITE_FULL, /* #  define SQLITE_FULL        13 (* Insertion failed because database is full *)   */
  DUF_SQL_CANTOPEN = DUF_SQLITE_ERROR_BASE + SQLITE_CANTOPEN, /* #  define SQLITE_CANTOPEN    14 (* Unable to open the database file *)            */
  DUF_SQL_PROTOCOL = DUF_SQLITE_ERROR_BASE + SQLITE_PROTOCOL, /* #  define SQLITE_PROTOCOL    15 (* Database lock protocol error *)                */
  DUF_SQL_EMPTY = DUF_SQLITE_ERROR_BASE + SQLITE_EMPTY, /* #  define SQLITE_EMPTY       16 (* Database is empty *)                           */
  DUF_SQL_SCHEMA = DUF_SQLITE_ERROR_BASE + SQLITE_SCHEMA, /* #  define SQLITE_SCHEMA      17 (* The database schema changed *)                 */
  DUF_SQL_TOOBIG = DUF_SQLITE_ERROR_BASE + SQLITE_TOOBIG, /* #  define SQLITE_TOOBIG      18 (* String or BLOB exceeds size limit *)           */
  DUF_SQL_CONSTRAINT = DUF_SQLITE_ERROR_BASE + SQLITE_CONSTRAINT, /* #  define SQLITE_CONSTRAINT  19 (* Abort due to constraint violation *)           */
  DUF_SQL_MISMATCH = DUF_SQLITE_ERROR_BASE + SQLITE_MISMATCH, /* #  define SQLITE_MISMATCH    20 (* Data type mismatch *)                          */
  DUF_SQL_MISUSE = DUF_SQLITE_ERROR_BASE + SQLITE_MISUSE, /* #  define SQLITE_MISUSE      21 (* Library used incorrectly *)                    */
  DUF_SQL_NOLFS = DUF_SQLITE_ERROR_BASE + SQLITE_NOLFS, /* #  define SQLITE_NOLFS       22 (* Uses OS features not supported on host *)      */
  DUF_SQL_AUTH = DUF_SQLITE_ERROR_BASE + SQLITE_AUTH, /* #  define SQLITE_AUTH        23 (* Authorization denied *)                        */
  DUF_SQL_FORMAT = DUF_SQLITE_ERROR_BASE + SQLITE_FORMAT, /* #  define SQLITE_FORMAT      24 (* Auxiliary database format error *)             */
  DUF_SQL_RANGE = DUF_SQLITE_ERROR_BASE + SQLITE_RANGE, /* #  define SQLITE_RANGE       25 (* 2nd parameter to sqlite3_bind out of range *)  */
  DUF_SQL_NOTADB = DUF_SQLITE_ERROR_BASE + SQLITE_NOTADB, /* #  define SQLITE_NOTADB      26 (* File opened that is not a database file *)     */
  DUF_SQL_NOTICE = DUF_SQLITE_ERROR_BASE + SQLITE_NOTICE, /* #  define SQLITE_NOTICE      27 (* Notifications from sqlite3_log() *)            */
  DUF_SQL_WARNING = DUF_SQLITE_ERROR_BASE + SQLITE_WARNING, /* #  define SQLITE_WARNING     28 (* Warnings from sqlite3_log() *)                 */
  DUF_SQL_ROW = DUF_SQLITE_ERROR_BASE + SQLITE_ROW, /* #  define SQLITE_ROW         100(* sqlite3_step() has another row ready *)        */
  DUF_SQL_DONE = DUF_SQLITE_ERROR_BASE + SQLITE_DONE, /* #  define SQLITE_DONE        101(* sqlite3_step() has finished executing *)       */
} duf_sqlite_code_t;


/* #  define SQLITE_OK           0 (* Successful result *)                           */
/* (* beginning-of-error-codes *)                                                    */
































#endif
