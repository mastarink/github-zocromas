#ifndef DUF_MAINDB_H
#  define DUF_MAINDB_H

int duf_main_db( int argc, char **argv ) __attribute__ ( ( warn_unused_result ) );
int duf_main_db_open( void ) __attribute__ ( ( warn_unused_result ) );
int duf_main_db_close( int ra ) __attribute__ ( ( warn_unused_result ) );



/* int duf_main_db_pre_action( void ); */

#endif
