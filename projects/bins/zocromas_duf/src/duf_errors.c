#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <fnmatch.h>
/* #include <unistd.h> */
#include <sys/time.h>

#include <mastar/sqlite/mas_sqlite_const.h>

#include "duf_maintenance.h"


/* #include "duf_sql_const.h" */

/* ###################################################################### */
#include "duf_errors.h"
/* ###################################################################### */

/* #define DUF_NOTIMING */
static int noreport_error[DUF_ERROR_COUNT] = { 0 };
static int count_error[DUF_ERROR_COUNT] = { 0 };
static int max_show_count_error[DUF_ERROR_COUNT] = { 0 };

static long _made_errors = 0;
static int force_ereport = 0;

#define MAX_ERRORS 1000
typedef struct
{
  long made_id;
  duf_error_code_t err;
  const char *funcid;
  int linid;
  int shown;
} error_desc_t;

static error_desc_t *global_error_list = NULL;

__attribute__ ( ( destructor( 65535 ) ) )
     static void destructor_errorlist( void )
{
  mas_free( global_error_list );
  global_error_list = NULL;
}

void
duf_force_ereport( int count )
{
  force_ereport = count;
}

duf_error_code_t
duf_make_error( duf_error_code_t err, const char *funcid, int linid )
{
  if ( !global_error_list )
  {
    global_error_list = mas_malloc( sizeof( error_desc_t ) * MAX_ERRORS );
    memset( global_error_list, 0, sizeof( error_desc_t ) * MAX_ERRORS );
  }
  if ( _made_errors < MAX_ERRORS )
  {
    global_error_list[_made_errors].err = err;
    global_error_list[_made_errors].funcid = funcid;
    global_error_list[_made_errors].linid = linid;
  }
  _made_errors++;
  return err;
}

long
duf_made_errors( duf_error_code_t err )
{
  return _made_errors;
}

int
duf_errindex( duf_error_code_t rtest )
{
  return ( rtest - DUF_ERROR_ERROR_BASE );
}

int
duf_errindex_sql( duf_error_code_t rtest )
{
  return ( rtest - MAS_SQLITE_ERROR_BASE );
}

static void
_duf_set_ereport( int once, int enable, int abs, duf_error_code_t rtest, int maxerr )
{
  if ( rtest < 0 )
  {
    int errindex = duf_errindex( rtest );

    if ( errindex >= 0 && errindex < maxerr )
    {
      /* int b;                        */
      /*                               */
      /* b = noreport_error[errindex]; */
      if ( abs )
        noreport_error[errindex] = enable;
      else
        noreport_error[errindex] += enable /* < 0 ? -1 : 1 */ ;
      /* if ( rtest == DUF_ERROR_TOO_DEEP )                                                                                         */
      /* {                                                                                                                          */
      /*   if ( b < noreport_error[errindex] )                                                                                      */
      /*     fprintf( stderr, "$$$$$$$$$$$$ %d+%d=%d (abs:%d)\n", b, noreport_error[errindex] - b, noreport_error[errindex], abs ); */
      /*   else if ( b > noreport_error[errindex] )                                                                                 */
      /*     fprintf( stderr, "$$$$$$$$$$$$ %d-%d=%d (abs:%d)\n", b, b - noreport_error[errindex], noreport_error[errindex], abs ); */
      /*   else                                                                                                                     */
      /*     fprintf( stderr, "$$$$$$$$$$$$ ==%d (abs:%d)\n", b, abs );                                                             */
      /* }                                                                                                                          */
    }
  }
}

void
duf_set_ereport( int once, int enable, int abs, duf_error_code_t rtest )
{
  _duf_set_ereport( once, enable, abs, rtest, DUF_ERROR_COUNT );
}

void
duf_vset_mereport( int once, int enable, int abs, va_list args )
{
  duf_error_code_t rtest = 0;

  do
  {
    rtest = va_arg( args, int );

    if ( rtest )
      duf_set_ereport( once, enable, abs, rtest );
  }
  while ( rtest );
}

void
duf_set_mereport( int once, int enable, int abs, ... )
{
  va_list args;

  va_start( args, abs );
  duf_vset_mereport( once, enable, abs, args );
  va_end( args );
}

static int
_duf_get_ereport_n( duf_error_code_t rtest, int maxerr )
{
  int re = 0;

  if ( rtest < 0 )
  {
    int errindex = duf_errindex( rtest );

    if ( errindex >= 0 && errindex < maxerr )
      re = noreport_error[errindex];
  }
  return re;
}

/* >0 -- report it */
static int
_duf_get_ereport( duf_error_code_t rtest, int maxerr )
{
  int re = 0;

  int errindex = duf_errindex( rtest );

  /* if ( rtest < 0 ) */
  /*   return 1;      */
  if ( rtest < 0 )
  {
    if ( errindex < 0 )
    {
      re = 0;                   /* sql ? ? ? */
    }
    else if ( errindex >= 0 && errindex < maxerr
              && ( max_show_count_error[errindex] <= 0 || count_error[errindex] < max_show_count_error[errindex] - 1 ) )
      re = noreport_error[errindex] + 1;
  }
  return re;
}

int
duf_get_ereport_n( duf_error_code_t rtest )
{
  int re = 0;

  DOCF( re, _duf_get_ereport_n, rtest, DUF_ERROR_COUNT );
  return re;
}

int
duf_get_ereport( duf_error_code_t rtest )
{
  int re = 0;

  if ( rtest < 0 )
  {
    if ( force_ereport > 0 )
    {
      re = force_ereport--;
    }
    else
    {
      DOCF( re, _duf_get_ereport, rtest, DUF_ERROR_COUNT );
    }
  }  
  return re;
}

static void
_duf_set_emax_count( int maxcount, duf_error_code_t rtest, int maxerr )
{
  if ( rtest < 0 )
  {
    int errindex = duf_errindex( rtest );

    if ( errindex >= 0 && errindex < maxerr )
      max_show_count_error[errindex] = maxcount + 1;
  }
}

void
duf_set_emax_count( int maxcount, duf_error_code_t rtest )
{
  _duf_set_emax_count( maxcount, rtest, DUF_ERROR_COUNT );
}

void
duf_vset_emax_count( int maxcount, va_list args )
{
  duf_error_code_t rtest = 0;

  do
  {
    rtest = va_arg( args, int );

    if ( rtest )
      duf_set_emax_count( maxcount, rtest );
  }
  while ( rtest );
}

void
duf_set_memax_count( int maxcount, ... )
{
  va_list args;

  va_start( args, maxcount );
  duf_vset_emax_count( maxcount, args );
  va_end( args );
}

int
_duf_ecount( duf_error_code_t rtest, int maxerr )
{
  int re = 0;

  if ( rtest < 0 )
  {
    int errindex = duf_errindex( rtest );

    if ( errindex >= 0 && errindex < maxerr )
      re = count_error[errindex]++;
  }
  return re;
}

int
duf_ecount( duf_error_code_t rtest )
{
  int re = 0;

  DOCF( re, _duf_ecount, rtest, DUF_ERROR_COUNT );
  return re;
}

duf_error_code_t
duf_vclear_error( duf_error_code_t re, va_list args )
{
  duf_error_code_t e = 0;

  do
  {
    e = va_arg( args, int );

    if ( re == e )
    {
      re = 0;
      break;
    }
  }
  while ( e );
  return re;
}

duf_error_code_t
duf_clear_error( duf_error_code_t re, ... )
{
  va_list args;

  va_start( args, re );
  re = duf_vclear_error( re, args );
  va_end( args );
  return re;
}


typedef struct
{
  const char *name;
  duf_error_code_t code;
} duf_errdesc_t;

const char *
duf_error_name( duf_error_code_t c )
{
  static char buf[512];
  int found = 0;

#define DUF_ERROR_NAME(ername) {.name=#ername, .code=ername}

  const duf_errdesc_t table[] = {
    DUF_ERROR_NAME( MAS_SQL_ERROR ),
    DUF_ERROR_NAME( MAS_SQL_INTERNAL ),
    DUF_ERROR_NAME( MAS_SQL_PERM ),
    DUF_ERROR_NAME( MAS_SQL_ABORT ),
    DUF_ERROR_NAME( MAS_SQL_BUSY ),
    DUF_ERROR_NAME( MAS_SQL_LOCKED ),
    DUF_ERROR_NAME( MAS_SQL_NOMEM ),
    DUF_ERROR_NAME( MAS_SQL_READONLY ),
    DUF_ERROR_NAME( MAS_SQL_INTERRUPT ),
    DUF_ERROR_NAME( MAS_SQL_IOERR ),
    DUF_ERROR_NAME( MAS_SQL_CORRUPT ),
    DUF_ERROR_NAME( MAS_SQL_NOTFOUND ),
    DUF_ERROR_NAME( MAS_SQL_FULL ),
    DUF_ERROR_NAME( MAS_SQL_CANTOPEN ),
    DUF_ERROR_NAME( MAS_SQL_PROTOCOL ),
    DUF_ERROR_NAME( MAS_SQL_EMPTY ),
    DUF_ERROR_NAME( MAS_SQL_SCHEMA ),
    DUF_ERROR_NAME( MAS_SQL_TOOBIG ),
    DUF_ERROR_NAME( MAS_SQL_CONSTRAINT ),
    DUF_ERROR_NAME( MAS_SQL_MISMATCH ),
    DUF_ERROR_NAME( MAS_SQL_MISUSE ),
    DUF_ERROR_NAME( MAS_SQL_NOLFS ),
    DUF_ERROR_NAME( MAS_SQL_AUTH ),
    DUF_ERROR_NAME( MAS_SQL_FORMAT ),
    DUF_ERROR_NAME( MAS_SQL_RANGE ),
    DUF_ERROR_NAME( MAS_SQL_NOTADB ),
    DUF_ERROR_NAME( MAS_SQL_NOTICE ),
    DUF_ERROR_NAME( MAS_SQL_WARNING ),
    DUF_ERROR_NAME( MAS_SQL_ROW ),
    DUF_ERROR_NAME( MAS_SQL_DONE ),
/* extended */
    DUF_ERROR_NAME( MAS_SQL_IOERR_READ ),
    DUF_ERROR_NAME( MAS_SQL_IOERR_SHORT_READ ),
    DUF_ERROR_NAME( MAS_SQL_IOERR_WRITE ),
    DUF_ERROR_NAME( MAS_SQL_IOERR_FSYNC ),
    DUF_ERROR_NAME( MAS_SQL_IOERR_DIR_FSYNC ),
    DUF_ERROR_NAME( MAS_SQL_IOERR_TRUNCATE ),
    DUF_ERROR_NAME( MAS_SQL_IOERR_FSTAT ),
    DUF_ERROR_NAME( MAS_SQL_IOERR_UNLOCK ),
    DUF_ERROR_NAME( MAS_SQL_IOERR_RDLOCK ),
    DUF_ERROR_NAME( MAS_SQL_IOERR_DELETE ),
    DUF_ERROR_NAME( MAS_SQL_IOERR_BLOCKED ),
    DUF_ERROR_NAME( MAS_SQL_IOERR_NOMEM ),
    DUF_ERROR_NAME( MAS_SQL_IOERR_ACCESS ),
    DUF_ERROR_NAME( MAS_SQL_IOERR_CHECKRESERVEDLOCK ),
    DUF_ERROR_NAME( MAS_SQL_IOERR_LOCK ),
    DUF_ERROR_NAME( MAS_SQL_IOERR_CLOSE ),
    DUF_ERROR_NAME( MAS_SQL_IOERR_DIR_CLOSE ),
    DUF_ERROR_NAME( MAS_SQL_IOERR_SHMOPEN ),
    DUF_ERROR_NAME( MAS_SQL_IOERR_SHMSIZE ),
    DUF_ERROR_NAME( MAS_SQL_IOERR_SHMLOCK ),
    DUF_ERROR_NAME( MAS_SQL_IOERR_SHMMAP ),
    DUF_ERROR_NAME( MAS_SQL_IOERR_SEEK ),
    DUF_ERROR_NAME( MAS_SQL_IOERR_DELETE_NOENT ),
    DUF_ERROR_NAME( MAS_SQL_IOERR_MMAP ),
    DUF_ERROR_NAME( MAS_SQL_IOERR_GETTEMPPATH ),
    DUF_ERROR_NAME( MAS_SQL_IOERR_CONVPATH ),
    DUF_ERROR_NAME( MAS_SQL_LOCKED_SHAREDCACHE ),
    DUF_ERROR_NAME( MAS_SQL_BUSY_RECOVERY ),
    DUF_ERROR_NAME( MAS_SQL_BUSY_SNAPSHOT ),
    DUF_ERROR_NAME( MAS_SQL_CANTOPEN_NOTEMPDIR ),
    DUF_ERROR_NAME( MAS_SQL_CANTOPEN_ISDIR ),
    DUF_ERROR_NAME( MAS_SQL_CANTOPEN_FULLPATH ),
    DUF_ERROR_NAME( MAS_SQL_CANTOPEN_CONVPATH ),
    DUF_ERROR_NAME( MAS_SQL_CORRUPT_VTAB ),
    DUF_ERROR_NAME( MAS_SQL_READONLY_RECOVERY ),
    DUF_ERROR_NAME( MAS_SQL_READONLY_CANTLOCK ),
    DUF_ERROR_NAME( MAS_SQL_READONLY_ROLLBACK ),
    DUF_ERROR_NAME( MAS_SQL_READONLY_DBMOVED ),
    DUF_ERROR_NAME( MAS_SQL_ABORT_ROLLBACK ),
    DUF_ERROR_NAME( MAS_SQL_CONSTRAINT_CHECK ),
    DUF_ERROR_NAME( MAS_SQL_CONSTRAINT_COMMITHOOK ),
    DUF_ERROR_NAME( MAS_SQL_CONSTRAINT_FOREIGNKEY ),
    DUF_ERROR_NAME( MAS_SQL_CONSTRAINT_FUNCTION ),
    DUF_ERROR_NAME( MAS_SQL_CONSTRAINT_NOTNULL ),
    DUF_ERROR_NAME( MAS_SQL_CONSTRAINT_PRIMARYKEY ),
    DUF_ERROR_NAME( MAS_SQL_CONSTRAINT_TRIGGER ),
    DUF_ERROR_NAME( MAS_SQL_CONSTRAINT_UNIQUE ),
    DUF_ERROR_NAME( MAS_SQL_CONSTRAINT_VTAB ),
    DUF_ERROR_NAME( MAS_SQL_CONSTRAINT_ROWID ),
    DUF_ERROR_NAME( MAS_SQL_NOTICE_RECOVER_WAL ),
    DUF_ERROR_NAME( MAS_SQL_NOTICE_RECOVER_ROLLBACK ),
    DUF_ERROR_NAME( MAS_SQL_WARNING_AUTOINDEX ),
/*========================================*/
    DUF_ERROR_NAME( DUF_OK ),
    DUF_ERROR_NAME( DUF_ERROR_UNKNOWN ),
    DUF_ERROR_NAME( DUF_ERROR_UNKNOWN_NODE ),
    DUF_ERROR_NAME( DUF_ERROR_MAIN ),
    DUF_ERROR_NAME( DUF_ERROR_NO_ACTIONS ),
    DUF_ERROR_NAME( DUF_ERROR_PTR ),
    DUF_ERROR_NAME( DUF_ERROR_DATA ),
    DUF_ERROR_NAME( DUF_ERROR_SCCB ),
    DUF_ERROR_NAME( DUF_ERROR_MD5 ),
    DUF_ERROR_NAME( DUF_ERROR_CRC32 ),
    DUF_ERROR_NAME( DUF_ERROR_EXIF_END ),
    DUF_ERROR_NAME( DUF_ERROR_EXIF_NO_MODEL ),
    DUF_ERROR_NAME( DUF_ERROR_EXIF_NO_DATE ),
    DUF_ERROR_NAME( DUF_ERROR_EXIF_BROKEN_DATE ),
    DUF_ERROR_NAME( DUF_ERROR_EXIF ),
    DUF_ERROR_NAME( DUF_ERROR_NOT_OPEN ),
    DUF_ERROR_NAME( DUF_ERROR_EOF ),
    DUF_ERROR_NAME( DUF_ERROR_PATH ),
    DUF_ERROR_NAME( DUF_ERROR_OPENAT ),
    DUF_ERROR_NAME( DUF_ERROR_OPENAT_ENOENT ),
    DUF_ERROR_NAME( DUF_ERROR_OPEN ),
    DUF_ERROR_NAME( DUF_ERROR_OPEN_ENOENT ),
    DUF_ERROR_NAME( DUF_ERROR_FS_DISABLED ),
    DUF_ERROR_NAME( DUF_ERROR_READ ),
    DUF_ERROR_NAME( DUF_ERROR_CLOSE ),
    DUF_ERROR_NAME( DUF_ERROR_UNLINK ),
    DUF_ERROR_NAME( DUF_ERROR_OPTION ),
    DUF_ERROR_NAME( DUF_ERROR_OPTION_MULTIPLE ),
    DUF_ERROR_NAME( DUF_ERROR_OPTION_NOT_FOUND ),
    DUF_ERROR_NAME( DUF_ERROR_OPTION_NOT_PARSED ),
    DUF_ERROR_NAME( DUF_ERROR_OPTION_NO_FUNC ),
    DUF_ERROR_NAME( DUF_ERROR_OPTION_VALUE ),
    DUF_ERROR_NAME( DUF_ERROR_SUBOPTION ),
    DUF_ERROR_NAME( DUF_ERROR_SCANDIR ),
    DUF_ERROR_NAME( DUF_ERROR_CHECK_TABLES ),
    DUF_ERROR_NAME( DUF_ERROR_CLEAR_TABLES ),
    DUF_ERROR_NAME( DUF_ERROR_NO_FILE_SELECTOR ),
    DUF_ERROR_NAME( DUF_ERROR_DB_NO_PATH ),
    DUF_ERROR_NAME( DUF_ERROR_NO_STR_CB ),
    DUF_ERROR_NAME( DUF_ERROR_BIND_NAME ),
    DUF_ERROR_NAME( DUF_ERROR_LEVINFO_SIZE ),
    DUF_ERROR_NAME( DUF_ERROR_TOO_DEEP ),
    DUF_ERROR_NAME( DUF_ERROR_MAX_REACHED ),
    DUF_ERROR_NAME( DUF_ERROR_MAX_SEQ_REACHED ),
    DUF_ERROR_NAME( DUF_ERROR_GET_FIELD ),
    DUF_ERROR_NAME( DUF_ERROR_NOT_IN_DB ),
    DUF_ERROR_NAME( DUF_ERROR_TOTALS ),
    DUF_ERROR_NAME( DUF_ERROR_NO_FIELD ),
    DUF_ERROR_NAME( DUF_ERROR_NO_FIELD_OPTIONAL ),
    DUF_ERROR_NAME( DUF_ERROR_INSERT_MDPATH ),
    DUF_ERROR_NAME( DUF_ERROR_NOT_FOUND ),
    DUF_ERROR_NAME( DUF_ERROR_STAT ),
    DUF_ERROR_NAME( DUF_ERROR_STAT_ENOENT ),
    DUF_ERROR_NAME( DUF_ERROR_STATAT ),
    DUF_ERROR_NAME( DUF_ERROR_STATAT_ENOENT ),
    DUF_ERROR_NAME( DUF_ERROR_PDI_SQL ),
    DUF_ERROR_NAME( DUF_ERROR_SQL_NO_FIELDSET ), /* */
    DUF_ERROR_NAME( DUF_ERROR_SQL_NO_TABLE ), /* */
    DUF_ERROR_NAME( DUF_ERROR_MEMORY ), /* */
    DUF_ERROR_NAME( DUF_ERROR_ARGUMENT ),
    DUF_ERROR_NAME( DUF_ERROR_FILE_EXISTS ),
/*========================================*/
    DUF_ERROR_NAME( DUF_ERROR_ERROR_MAX ),
  };

  for ( int i = 0; i < sizeof( table ) / sizeof( table[9] ); i++ )
  {
    if ( c == table[i].code )
    {
      snprintf( buf, sizeof( buf ), "%s", table[i].name + 4 );
      found = 1;
      break;
    }
  }
  if ( !found )
    snprintf( buf, sizeof( buf ), "Unknown errcode %d", c );
  return buf;
}
