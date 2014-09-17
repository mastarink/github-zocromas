#ifndef MAS_DUF_SQLITE_H
#  define MAS_DUF_SQLITE_H

#  include <sqlite3.h>


int duf_sqlite_error_code( int r3 );

int duf_sqlite_open( const char *dbpath );
int duf_sqlite_close( void );


/* int duf_sqlite_execcb_e( const char *sql, duf_sqexe_cb_t sqexe_cb, void *sqexe_data, int *pchanges ); */
/* int duf_sqlite_execcb( const char *sql, duf_sqexe_cb_t sqexe_cb, void *sqexe_data, int *pchanges, char **pemsg ); */
/* int duf_sqlite_exec( const char *sql, int *pchanges, char **pemsg ); */
int duf_sqlite_exec_c( const char *sql, int constraint_ignore, int *pchanges );
/* int duf_sqlite_exec_e( const char *sql, int *pchanges ); */

int duf_vsqlite_c( const char *sqlfmt, int constraint_ignore, int *pchanges, va_list args );
/* int duf_vsqlite_e( const char *fmt, int *pchanges, va_list args ); */

int duf_sqlite_vselect( duf_sel_cb_t sel_cb, void *sel_cb_udata, duf_str_cb_t str_cb, void *str_cb_udata,
                        duf_depthinfo_t * pdi, duf_scan_callbacks_t * sccb, const char *sqlfmt, va_list args );

unsigned long long duf_sqlite_last_insert_rowid( void );

int duf_sqlite_prepare( const char *sql, duf_sqlite_stmt_t ** pstmt );
int duf_sqlite_step( duf_sqlite_stmt_t * stmt );
int duf_sqlite_finalize( duf_sqlite_stmt_t * stmt );
int duf_sqlite_reset( duf_sqlite_stmt_t * stmt );

int duf_sqlite_bind_parameter_index( duf_sqlite_stmt_t * stmt, const char *name );

int duf_sqlite_bind_long_long( duf_sqlite_stmt_t * stmt, int num, long long val );
int duf_sqlite_bind_int( duf_sqlite_stmt_t * stmt, int num, int val );
int duf_sqlite_bind_null( duf_sqlite_stmt_t * stmt, int num );
int duf_sqlite_bind_double( duf_sqlite_stmt_t * stmt, int num, double val );
int duf_sqlite_bind_string( duf_sqlite_stmt_t * stmt, int num, const char *val );

int duf_sqlite_changes( void );

long long duf_sqlite_column_long_long( duf_sqlite_stmt_t * stmt, int icol );
int duf_sqlite_column_int( duf_sqlite_stmt_t * stmt, int icol );
const char *duf_sqlite_column_string( duf_sqlite_stmt_t * stmt, int icol );





const char *duf_sqlite_column_name( duf_sqlite_stmt_t * stmt, int index );
int duf_sqlite_column_count( duf_sqlite_stmt_t * stmt );



char *duf_sqlite_vmprintf( const char *fmt, va_list args );
/* char *duf_sqlite_mprintf( const char *fmt, ... ); */

/* void duf_sqlite_free( char *s ); */
void duf_sqlite_clear_bindings( duf_sqlite_stmt_t * stmt );




/* As for sqlite 3.8.4.3 */
/* #define SQLITE_OK           0   (* Successful result *)                          */
/* (* beginning-of-error-codes *)                                                   */
/* #define SQLITE_ERROR        1   (* SQL error or missing database *)              */
/* #define SQLITE_INTERNAL     2   (* Internal logic error in SQLite *)             */
/* #define SQLITE_PERM         3   (* Access permission denied *)                   */
/* #define SQLITE_ABORT        4   (* Callback routine requested an abort *)        */
/* #define SQLITE_BUSY         5   (* The database file is locked *)                */
/* #define SQLITE_LOCKED       6   (* A table in the database is locked *)          */
/* #define SQLITE_NOMEM        7   (* A malloc() failed *)                          */
/* #define SQLITE_READONLY     8   (* Attempt to write a readonly database *)       */
/* #define SQLITE_INTERRUPT    9   (* Operation terminated by sqlite3_interrupt()*) */
/* #define SQLITE_IOERR       10   (* Some kind of disk I/O error occurred *)       */
/* #define SQLITE_CORRUPT     11   (* The database disk image is malformed *)       */
/* #define SQLITE_NOTFOUND    12   (* Unknown opcode in sqlite3_file_control() *)   */
/* #define SQLITE_FULL        13   (* Insertion failed because database is full *)  */
/* #define SQLITE_CANTOPEN    14   (* Unable to open the database file *)           */
/* #define SQLITE_PROTOCOL    15   (* Database lock protocol error *)               */
/* #define SQLITE_EMPTY       16   (* Database is empty *)                          */
/* #define SQLITE_SCHEMA      17   (* The database schema changed *)                */
/* #define SQLITE_TOOBIG      18   (* String or BLOB exceeds size limit *)          */
/* #define SQLITE_CONSTRAINT  19   (* Abort due to constraint violation *)          */
/* #define SQLITE_MISMATCH    20   (* Data type mismatch *)                         */
/* #define SQLITE_MISUSE      21   (* Library used incorrectly *)                   */
/* #define SQLITE_NOLFS       22   (* Uses OS features not supported on host *)     */
/* #define SQLITE_AUTH        23   (* Authorization denied *)                       */
/* #define SQLITE_FORMAT      24   (* Auxiliary database format error *)            */
/* #define SQLITE_RANGE       25   (* 2nd parameter to sqlite3_bind out of range *) */
/* #define SQLITE_NOTADB      26   (* File opened that is not a database file *)    */
/* #define SQLITE_NOTICE      27   (* Notifications from sqlite3_log() *)           */
/* #define SQLITE_WARNING     28   (* Warnings from sqlite3_log() *)                */
/* #define SQLITE_ROW         100  (* sqlite3_step() has another row ready *)       */
/* #define SQLITE_DONE        101  (* sqlite3_step() has finished executing *)      */

/* #define SQLITE_IOERR_READ              (SQLITE_IOERR | (1<<8))      */
/* #define SQLITE_IOERR_SHORT_READ        (SQLITE_IOERR | (2<<8))      */
/* #define SQLITE_IOERR_WRITE             (SQLITE_IOERR | (3<<8))      */
/* #define SQLITE_IOERR_FSYNC             (SQLITE_IOERR | (4<<8))      */
/* #define SQLITE_IOERR_DIR_FSYNC         (SQLITE_IOERR | (5<<8))      */
/* #define SQLITE_IOERR_TRUNCATE          (SQLITE_IOERR | (6<<8))      */
/* #define SQLITE_IOERR_FSTAT             (SQLITE_IOERR | (7<<8))      */
/* #define SQLITE_IOERR_UNLOCK            (SQLITE_IOERR | (8<<8))      */
/* #define SQLITE_IOERR_RDLOCK            (SQLITE_IOERR | (9<<8))      */
/* #define SQLITE_IOERR_DELETE            (SQLITE_IOERR | (10<<8))     */
/* #define SQLITE_IOERR_BLOCKED           (SQLITE_IOERR | (11<<8))     */
/* #define SQLITE_IOERR_NOMEM             (SQLITE_IOERR | (12<<8))     */
/* #define SQLITE_IOERR_ACCESS            (SQLITE_IOERR | (13<<8))     */
/* #define SQLITE_IOERR_CHECKRESERVEDLOCK (SQLITE_IOERR | (14<<8))     */
/* #define SQLITE_IOERR_LOCK              (SQLITE_IOERR | (15<<8))     */
/* #define SQLITE_IOERR_CLOSE             (SQLITE_IOERR | (16<<8))     */
/* #define SQLITE_IOERR_DIR_CLOSE         (SQLITE_IOERR | (17<<8))     */
/* #define SQLITE_IOERR_SHMOPEN           (SQLITE_IOERR | (18<<8))     */
/* #define SQLITE_IOERR_SHMSIZE           (SQLITE_IOERR | (19<<8))     */
/* #define SQLITE_IOERR_SHMLOCK           (SQLITE_IOERR | (20<<8))     */
/* #define SQLITE_IOERR_SHMMAP            (SQLITE_IOERR | (21<<8))     */
/* #define SQLITE_IOERR_SEEK              (SQLITE_IOERR | (22<<8))     */
/* #define SQLITE_IOERR_DELETE_NOENT      (SQLITE_IOERR | (23<<8))     */
/* #define SQLITE_IOERR_MMAP              (SQLITE_IOERR | (24<<8))     */
/* #define SQLITE_IOERR_GETTEMPPATH       (SQLITE_IOERR | (25<<8))     */
/* #define SQLITE_IOERR_CONVPATH          (SQLITE_IOERR | (26<<8))     */
/* #define SQLITE_LOCKED_SHAREDCACHE      (SQLITE_LOCKED |  (1<<8))    */
/* #define SQLITE_BUSY_RECOVERY           (SQLITE_BUSY   |  (1<<8))    */
/* #define SQLITE_BUSY_SNAPSHOT           (SQLITE_BUSY   |  (2<<8))    */
/* #define SQLITE_CANTOPEN_NOTEMPDIR      (SQLITE_CANTOPEN | (1<<8))   */
/* #define SQLITE_CANTOPEN_ISDIR          (SQLITE_CANTOPEN | (2<<8))   */
/* #define SQLITE_CANTOPEN_FULLPATH       (SQLITE_CANTOPEN | (3<<8))   */
/* #define SQLITE_CANTOPEN_CONVPATH       (SQLITE_CANTOPEN | (4<<8))   */
/* #define SQLITE_CORRUPT_VTAB            (SQLITE_CORRUPT | (1<<8))    */
/* #define SQLITE_READONLY_RECOVERY       (SQLITE_READONLY | (1<<8))   */
/* #define SQLITE_READONLY_CANTLOCK       (SQLITE_READONLY | (2<<8))   */
/* #define SQLITE_READONLY_ROLLBACK       (SQLITE_READONLY | (3<<8))   */
/* #define SQLITE_READONLY_DBMOVED        (SQLITE_READONLY | (4<<8))   */
/* #define SQLITE_ABORT_ROLLBACK          (SQLITE_ABORT | (2<<8))      */
/* #define SQLITE_CONSTRAINT_CHECK        (SQLITE_CONSTRAINT | (1<<8)) */
/* #define SQLITE_CONSTRAINT_COMMITHOOK   (SQLITE_CONSTRAINT | (2<<8)) */
/* #define SQLITE_CONSTRAINT_FOREIGNKEY   (SQLITE_CONSTRAINT | (3<<8)) */
/* #define SQLITE_CONSTRAINT_FUNCTION     (SQLITE_CONSTRAINT | (4<<8)) */
/* #define SQLITE_CONSTRAINT_NOTNULL      (SQLITE_CONSTRAINT | (5<<8)) */
/* #define SQLITE_CONSTRAINT_PRIMARYKEY   (SQLITE_CONSTRAINT | (6<<8)) */
/* #define SQLITE_CONSTRAINT_TRIGGER      (SQLITE_CONSTRAINT | (7<<8)) */
/* #define SQLITE_CONSTRAINT_UNIQUE       (SQLITE_CONSTRAINT | (8<<8)) */
/* #define SQLITE_CONSTRAINT_VTAB         (SQLITE_CONSTRAINT | (9<<8)) */
/* #define SQLITE_CONSTRAINT_ROWID        (SQLITE_CONSTRAINT |(10<<8)) */
/* #define SQLITE_NOTICE_RECOVER_WAL      (SQLITE_NOTICE | (1<<8))     */
/* #define SQLITE_NOTICE_RECOVER_ROLLBACK (SQLITE_NOTICE | (2<<8))     */
/* #define SQLITE_WARNING_AUTOINDEX       (SQLITE_WARNING | (1<<8))    */






#  define DUF_SQLITE_CODE_CHAR(arr,name) arr[name]=#name


#endif
