#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <fnmatch.h>
/* #include <unistd.h> */
#include <sys/time.h>

#include <mastar/wrap/mas_std_def.h>
#include <mastar/wrap/mas_memory.h>


#include "duf_sql_const.h"

/* ###################################################################### */
#include "duf_errors.h"
/* ###################################################################### */

/* #define DUF_NOTIMING */
static short int noreport_error[DUF_ERROR_COUNT] = { 0 };
static short int noreport_error_once[DUF_ERROR_COUNT] = { 0 };

int
duf_errindex( duf_error_code_t rtest )
{
  return ( rtest - DUF_ERROR_ERROR_BASE );
}

void
duf_set_ereport( int once, int doreport, duf_error_code_t rtest )
{
  if ( rtest < 0 )
  {
    int errindex = duf_errindex( rtest );

    if ( errindex >= 0 && errindex < DUF_ERROR_COUNT )
    {
      if ( once )
        noreport_error_once[errindex] = doreport ? 0 : 1;
      else
        noreport_error[errindex] = doreport ? 0 : 1;
    }
  }
}

void
duf_vset_ereport( int once, int doreport, va_list args )
{
  duf_error_code_t rtest = 0;

  do
  {
    rtest = va_arg( args, int );
    duf_set_ereport(once, doreport, rtest);
  }
  while ( rtest );
}

void
duf_set_mereport( int once, int doreport, ... )
{
  va_list args;

  va_start( args, doreport );
  duf_vset_ereport( once, doreport, args );
  va_end( args );
}

/* !=0 -- report it */
int
duf_get_ereport( duf_error_code_t rtest )
{
  int r = 0;

  if ( rtest < 0 )
  {
    int errindex = duf_errindex( rtest );

    if ( errindex >= 0 && errindex < DUF_ERROR_COUNT && ( !( noreport_error[errindex] || noreport_error_once[errindex] ) ) )
      r = -1;
  }
  memset( noreport_error_once, 0, sizeof( noreport_error_once ) );
  return r;
}

duf_error_code_t
duf_vclear_error( duf_error_code_t r, va_list args )
{
  duf_error_code_t e = 0;

  /* fprintf( stderr, ">>>>>>>>>>>>>> %d\n", r ); */
  do
  {
    e = va_arg( args, int );

    /* fprintf( stderr, ">>>>>>>>>>>>>> %d ? %d\n", r, e ); */

    if ( r == e )
    {
      r = 0;
      break;
    }
  }
  while ( e );
  /* fprintf( stderr, "!!! %d\n", r ); */
  return r;
}

duf_error_code_t
duf_clear_error( duf_error_code_t r, ... )
{
  va_list args;

  va_start( args, r );
  r = duf_vclear_error( r, args );
  va_end( args );
  return r;
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
    DUF_ERROR_NAME( DUF_SQL_ERROR ),
    DUF_ERROR_NAME( DUF_SQL_INTERNAL ),
    DUF_ERROR_NAME( DUF_SQL_PERM ),
    DUF_ERROR_NAME( DUF_SQL_ABORT ),
    DUF_ERROR_NAME( DUF_SQL_BUSY ),
    DUF_ERROR_NAME( DUF_SQL_LOCKED ),
    DUF_ERROR_NAME( DUF_SQL_NOMEM ),
    DUF_ERROR_NAME( DUF_SQL_READONLY ),
    DUF_ERROR_NAME( DUF_SQL_INTERRUPT ),
    DUF_ERROR_NAME( DUF_SQL_IOERR ),
    DUF_ERROR_NAME( DUF_SQL_CORRUPT ),
    DUF_ERROR_NAME( DUF_SQL_NOTFOUND ),
    DUF_ERROR_NAME( DUF_SQL_FULL ),
    DUF_ERROR_NAME( DUF_SQL_CANTOPEN ),
    DUF_ERROR_NAME( DUF_SQL_PROTOCOL ),
    DUF_ERROR_NAME( DUF_SQL_EMPTY ),
    DUF_ERROR_NAME( DUF_SQL_SCHEMA ),
    DUF_ERROR_NAME( DUF_SQL_TOOBIG ),
    DUF_ERROR_NAME( DUF_SQL_CONSTRAINT ),
    DUF_ERROR_NAME( DUF_SQL_MISMATCH ),
    DUF_ERROR_NAME( DUF_SQL_MISUSE ),
    DUF_ERROR_NAME( DUF_SQL_NOLFS ),
    DUF_ERROR_NAME( DUF_SQL_AUTH ),
    DUF_ERROR_NAME( DUF_SQL_FORMAT ),
    DUF_ERROR_NAME( DUF_SQL_RANGE ),
    DUF_ERROR_NAME( DUF_SQL_NOTADB ),
    DUF_ERROR_NAME( DUF_SQL_NOTICE ),
    DUF_ERROR_NAME( DUF_SQL_WARNING ),
    DUF_ERROR_NAME( DUF_SQL_ROW ),
    DUF_ERROR_NAME( DUF_SQL_DONE ),

/* extended */
    DUF_ERROR_NAME( DUF_SQL_IOERR_READ ),
    DUF_ERROR_NAME( DUF_SQL_IOERR_SHORT_READ ),
    DUF_ERROR_NAME( DUF_SQL_IOERR_WRITE ),
    DUF_ERROR_NAME( DUF_SQL_IOERR_FSYNC ),
    DUF_ERROR_NAME( DUF_SQL_IOERR_DIR_FSYNC ),
    DUF_ERROR_NAME( DUF_SQL_IOERR_TRUNCATE ),
    DUF_ERROR_NAME( DUF_SQL_IOERR_FSTAT ),
    DUF_ERROR_NAME( DUF_SQL_IOERR_UNLOCK ),
    DUF_ERROR_NAME( DUF_SQL_IOERR_RDLOCK ),
    DUF_ERROR_NAME( DUF_SQL_IOERR_DELETE ),
    DUF_ERROR_NAME( DUF_SQL_IOERR_BLOCKED ),
    DUF_ERROR_NAME( DUF_SQL_IOERR_NOMEM ),
    DUF_ERROR_NAME( DUF_SQL_IOERR_ACCESS ),
    DUF_ERROR_NAME( DUF_SQL_IOERR_CHECKRESERVEDLOCK ),
    DUF_ERROR_NAME( DUF_SQL_IOERR_LOCK ),
    DUF_ERROR_NAME( DUF_SQL_IOERR_CLOSE ),
    DUF_ERROR_NAME( DUF_SQL_IOERR_DIR_CLOSE ),
    DUF_ERROR_NAME( DUF_SQL_IOERR_SHMOPEN ),
    DUF_ERROR_NAME( DUF_SQL_IOERR_SHMSIZE ),
    DUF_ERROR_NAME( DUF_SQL_IOERR_SHMLOCK ),
    DUF_ERROR_NAME( DUF_SQL_IOERR_SHMMAP ),
    DUF_ERROR_NAME( DUF_SQL_IOERR_SEEK ),
    DUF_ERROR_NAME( DUF_SQL_IOERR_DELETE_NOENT ),
    DUF_ERROR_NAME( DUF_SQL_IOERR_MMAP ),
    DUF_ERROR_NAME( DUF_SQL_IOERR_GETTEMPPATH ),
    DUF_ERROR_NAME( DUF_SQL_IOERR_CONVPATH ),
    DUF_ERROR_NAME( DUF_SQL_LOCKED_SHAREDCACHE ),
    DUF_ERROR_NAME( DUF_SQL_BUSY_RECOVERY ),
    DUF_ERROR_NAME( DUF_SQL_BUSY_SNAPSHOT ),
    DUF_ERROR_NAME( DUF_SQL_CANTOPEN_NOTEMPDIR ),
    DUF_ERROR_NAME( DUF_SQL_CANTOPEN_ISDIR ),
    DUF_ERROR_NAME( DUF_SQL_CANTOPEN_FULLPATH ),
    DUF_ERROR_NAME( DUF_SQL_CANTOPEN_CONVPATH ),
    DUF_ERROR_NAME( DUF_SQL_CORRUPT_VTAB ),
    DUF_ERROR_NAME( DUF_SQL_READONLY_RECOVERY ),
    DUF_ERROR_NAME( DUF_SQL_READONLY_CANTLOCK ),
    DUF_ERROR_NAME( DUF_SQL_READONLY_ROLLBACK ),
    DUF_ERROR_NAME( DUF_SQL_READONLY_DBMOVED ),
    DUF_ERROR_NAME( DUF_SQL_ABORT_ROLLBACK ),
    DUF_ERROR_NAME( DUF_SQL_CONSTRAINT_CHECK ),
    DUF_ERROR_NAME( DUF_SQL_CONSTRAINT_COMMITHOOK ),
    DUF_ERROR_NAME( DUF_SQL_CONSTRAINT_FOREIGNKEY ),
    DUF_ERROR_NAME( DUF_SQL_CONSTRAINT_FUNCTION ),
    DUF_ERROR_NAME( DUF_SQL_CONSTRAINT_NOTNULL ),
    DUF_ERROR_NAME( DUF_SQL_CONSTRAINT_PRIMARYKEY ),
    DUF_ERROR_NAME( DUF_SQL_CONSTRAINT_TRIGGER ),
    DUF_ERROR_NAME( DUF_SQL_CONSTRAINT_UNIQUE ),
    DUF_ERROR_NAME( DUF_SQL_CONSTRAINT_VTAB ),
    DUF_ERROR_NAME( DUF_SQL_CONSTRAINT_ROWID ),
    DUF_ERROR_NAME( DUF_SQL_NOTICE_RECOVER_WAL ),
    DUF_ERROR_NAME( DUF_SQL_NOTICE_RECOVER_ROLLBACK ),
    DUF_ERROR_NAME( DUF_SQL_WARNING_AUTOINDEX ),

/*========================================*/
    DUF_ERROR_NAME( DUF_OK ),
    DUF_ERROR_NAME( DUF_ERROR_UNKNOWN ),
    DUF_ERROR_NAME( DUF_ERROR_UNKNOWN_NODE ),
    DUF_ERROR_NAME( DUF_ERROR_MAIN ),
    DUF_ERROR_NAME( DUF_ERROR_NO_ACTIONS ),
    DUF_ERROR_NAME( DUF_ERROR_PTR ),
    DUF_ERROR_NAME( DUF_ERROR_DATA ),
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
    DUF_ERROR_NAME( DUF_ERROR_READ ),
    DUF_ERROR_NAME( DUF_ERROR_CLOSE ),
    DUF_ERROR_NAME( DUF_ERROR_UNLINK ),
    DUF_ERROR_NAME( DUF_ERROR_OPTION ),
    DUF_ERROR_NAME( DUF_ERROR_OPTION_MULTIPLE ),
    DUF_ERROR_NAME( DUF_ERROR_OPTION_NOT_PARSED ),
    DUF_ERROR_NAME( DUF_ERROR_OPTION_VALUE ),
    DUF_ERROR_NAME( DUF_ERROR_SUBOPTION ),
    DUF_ERROR_NAME( DUF_ERROR_SCANDIR ),
    DUF_ERROR_NAME( DUF_ERROR_CHECK_TABLES ),
    DUF_ERROR_NAME( DUF_ERROR_CLEAR_TABLES ),
    DUF_ERROR_NAME( DUF_ERROR_NO_FILE_SELECTOR ),
    DUF_ERROR_NAME( DUF_ERROR_DB_NO_PATH ),
    DUF_ERROR_NAME( DUF_ERROR_NO_STR_CB ),
    DUF_ERROR_NAME( DUF_ERROR_BIND_NAME ),
    DUF_ERROR_NAME( DUF_ERROR_MAX_DEPTH ),
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
    DUF_ERROR_NAME( DUF_ERROR_SQL_NO_FIELDSET ),
    DUF_ERROR_NAME( DUF_ERROR_SQL_NO_TABLE ),
    DUF_ERROR_NAME( DUF_ERROR_MEMORY ),
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
