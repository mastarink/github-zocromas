#ifndef MAS_SIG_H
#define MAS_SIG_H

/* void sigabrt_han( int s ); */
void sigterm_han( int s );
void sigint_han( int s );
void sigquit_han( int s );
void sigusr1_han( int s );
void sigusr2_han( int s );
void sigpoll_han( int s );
void sighup_han( int s );
void sigpipe_han( int s );
void sigwinch_han( int s );

int mas_init_sig( mas_options_t * popts, const char **message );
void mas_dfl_sig( void );

#endif
