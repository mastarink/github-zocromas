#include "masxfs_levinfo_defs.h"
#include <string.h>
/* #include <unistd.h> */
/* #include <sys/types.h> */
/* #include <sys/stat.h> */
/* #include <unistd.h> */

/* #include <fcntl.h>                                                   (* Definition of AT_* constants *) */
#include <sys/stat.h>

#include <mastar/wrap/mas_memory.h>
#include <mastar/minierr/minierr.h>

#include "masxfs_levinfo_structs.h"

#include "masxfs_levinfo_tools.h"

static const char *std_sdetypes[] = {
  [MASXFS_ENTRY_BLK_NUM] = "BLK",
  [MASXFS_ENTRY_CHR_NUM] = "CHR",
  [MASXFS_ENTRY_DIR_NUM] = "DIR",
  [MASXFS_ENTRY_REG_NUM] = "REG",
  [MASXFS_ENTRY_FIFO_NUM] = "FIFO",
  [MASXFS_ENTRY_SOCK_NUM] = "SOCK",
  [MASXFS_ENTRY_LNK_NUM] = "LNK"
};

const char *
masxfs_levinfo_detype_name( masxfs_levinfo_t * li )
{
  const char *s = NULL;

  if ( li )
  {
    masxfs_entry_type_t detype = li->detype;

    switch ( detype )
    {
    case MASXFS_ENTRY_NONE_NUM:
      s = "NONE";
      break;
    case MASXFS_ENTRY_UNKNOWN_NUM:
      s = "UNKNOWN";
      break;
#if 1
    default:
      s = masxfs_levinfo_detype2s( detype );
      break;
#else
    case MASXFS_ENTRY_BLK_NUM:
      s = "BLK";
      break;
    case MASXFS_ENTRY_CHR_NUM:
      s = "CHR";
      break;
    case MASXFS_ENTRY_DIR_NUM:
      s = "DIR";
      break;
    case MASXFS_ENTRY_FIFO_NUM:
      s = "FIFO";
      break;
    case MASXFS_ENTRY_LNK_NUM:
      s = "LNK";
      break;
    case MASXFS_ENTRY_REG_NUM:
      s = "REG";
      break;
    case MASXFS_ENTRY_SOCK_NUM:
      s = "SOCK";
      break;
#endif
    }
  }
  return s;
}

masxfs_entry_type_t
masxfs_levinfo_s2detype( const char *sdetype )
{
  for ( size_t i = 0; i < sizeof( std_sdetypes ) / sizeof( std_sdetypes[0] ); i++ )
  {
    if ( std_sdetypes[i] && 0 == strcmp( sdetype, std_sdetypes[i] ) )
      return i;
  }
  return 0;
}

const char *
masxfs_levinfo_detype2s( masxfs_entry_type_t detype )
{
  return std_sdetypes[detype];
}

masxfs_entry_type_t
masxfs_levinfo_de2entry( unsigned char d_type )
{
  masxfs_entry_type_t c = MASXFS_ENTRY_UNKNOWN_NUM;

  switch ( d_type )
  {
  case DT_BLK:
    c = MASXFS_ENTRY_BLK_NUM;
    break;
  case DT_CHR:
    c = MASXFS_ENTRY_CHR_NUM;
    break;
  case DT_DIR:
    c = MASXFS_ENTRY_DIR_NUM;
    break;
  case DT_FIFO:
    c = MASXFS_ENTRY_FIFO_NUM;
    break;
  case DT_LNK:
    c = MASXFS_ENTRY_LNK_NUM;
    break;
  case DT_REG:
    c = MASXFS_ENTRY_REG_NUM;
    break;
  case DT_SOCK:
    c = MASXFS_ENTRY_SOCK_NUM;
    break;
  case DT_UNKNOWN:
    c = MASXFS_ENTRY_UNKNOWN_NUM;
    break;
  }
  return c;
}

unsigned char
masxfs_levinfo_entry2de( masxfs_entry_type_t detype )
{
  unsigned char c = 0;

  switch ( detype )
  {
  case MASXFS_ENTRY_BLK_NUM:
    c = DT_BLK;
    break;
  case MASXFS_ENTRY_CHR_NUM:
    c = DT_CHR;
    break;
  case MASXFS_ENTRY_DIR_NUM:
    c = DT_DIR;
    break;
  case MASXFS_ENTRY_FIFO_NUM:
    c = DT_FIFO;
    break;
  case MASXFS_ENTRY_LNK_NUM:
    c = DT_LNK;
    break;
  case MASXFS_ENTRY_REG_NUM:
    c = DT_REG;
    break;
  case MASXFS_ENTRY_SOCK_NUM:
    c = DT_SOCK;
    break;
  case MASXFS_ENTRY_UNKNOWN_NUM:
    c = DT_UNKNOWN;
    break;
  case MASXFS_ENTRY_NONE_NUM:
    c = 0;
    break;
  }
  return c;
}

masxfs_entry_type_t
masxfs_levinfo_statmode2entry( mode_t m )
{
  masxfs_entry_type_t c = MASXFS_ENTRY_UNKNOWN_NUM;

/* mode_t m; */
/* m = stat->st_mode; */
  if ( S_ISREG( m ) )                                                // is it a regular file?
    c = MASXFS_ENTRY_REG_NUM;
  else if ( S_ISDIR( m ) )                                           // directory?
    c = MASXFS_ENTRY_DIR_NUM;
  else if ( S_ISCHR( m ) )                                           // character device?
    c = MASXFS_ENTRY_CHR_NUM;
  else if ( S_ISBLK( m ) )                                           // block device?
    c = MASXFS_ENTRY_BLK_NUM;
  else if ( S_ISFIFO( m ) )                                          // FIFO (named pipe)?
    c = MASXFS_ENTRY_FIFO_NUM;
  else if ( S_ISLNK( m ) )                                           // symbolic link?  (Not in POSIX.1-1996.)
    c = MASXFS_ENTRY_LNK_NUM;
  else if ( S_ISSOCK( m ) )                                          // socket? (Not in POSIX.1-1996.)
    c = MASXFS_ENTRY_SOCK_NUM;
  else
    c = MASXFS_ENTRY_UNKNOWN_NUM;
  return c;
}

masxfs_entry_type_t
masxfs_levinfo_stat2entry( masxfs_stat_t * stat )
{
  masxfs_entry_type_t c = MASXFS_ENTRY_UNKNOWN_NUM;

  if ( stat )
    c = masxfs_levinfo_statmode2entry( stat->st_mode );
  return c;
}
