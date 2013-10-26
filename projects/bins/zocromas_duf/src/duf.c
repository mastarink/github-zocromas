#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#include <sqlite3.h>

#include <mastar/wrap/mas_memory.h>


#include "duf_def.h"
#include "duf_create.h"
#include "duf_utils.h"
#include "duf_md5.h"
#include "duf_finddup.h"
#include "duf_update.h"
#include "duf_exif.h"

#include "duf.h"


/* error codes */
/* - /usr/include/errno.h                  */
/* + /usr/include/bits/errno.h             */
/*   /usr/include/features.h               */
/* - /usr/include/linux/errno.h            */
/* - /usr/include/asm/errno.h              */
/* + /usr/include/asm-generic/errno.h      */
/* + /usr/include/asm-generic/errno-base.h */



/*
./sh/run_duf.sh /mnt/new_media/media/Photo/ /mnt/new_media/media/Pictures.R.20120207.164339/ /mnt/miscn/video/Pictures/ /home/mastar/a/to-attach/ /home/mastar/a/zaurus/sd/Documents/image/ /home/mastar/.local/share/Trash/ /mnt/old_home/mastar/.mas/HTC_Legend/ /mnt/tall/htclegend/ /mnt/tall/atalk/backup/

./sh/run_duf.sh /mnt/new_media/media/down/

*/

/*
 * mkdir m4
 * autoreconf --install
 * ./configure 
 */


/* file:///usr/share/doc/sqlite-3.7.15.2/html/docs.html */
int
main( int argc, char **argv )
{
  SQL_ERR( sqlite3_initialize(  ) );
  SQL_ERR( sqlite3_open( "db/duf-photo.sqlite3", &pDb ) );

  SQL_EXECC( "PRAGMA synchronous = OFF" );
  check_tables(  );

  fprintf( stderr, "Scan\n" );
  for ( int ia = 1; ia < argc; ia++ )
    update_path( argv[ia], -1, 1, 1 );

  update_md5(  );
  update_exif(  );
  find_duplicates(  );

  update_mdline(  );


  /* print_hardlinks(  ); */

  copy_jpeg_by_date(  );
  /* print_duplicate_dirs(  ); */
  /* print_duplicates(  ); */
  SQL_ERR( sqlite3_close( pDb ) );

  SQL_ERR( sqlite3_shutdown(  ) );
  return 0;
}
