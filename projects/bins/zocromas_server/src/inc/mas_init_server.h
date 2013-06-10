#ifndef MAS_INIT_SERVER_H
#  define MAS_INIT_SERVER_H

int mas_init_daemon( MAS_PASS_OPTS_DECLARE1 );
int mas_init_pids( MAS_PASS_OPTS_DECLARE1 );
int mas_init_load_protos( MAS_PASS_OPTS_DECLARE1 );

void mas_destroy_server( MAS_PASS_OPTS_DECLARE1 );

#endif
