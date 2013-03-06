struct args
{
  int joined;
  pthread_t td;
  pthread_mutex_t mtx;
  pthread_cond_t cond;
  void **res;
};

static void *
waiter( void *ap )
{
  struct args *args = ap;

  pthread_join( args->td, args->res );
  pthread_mutex_lock( &args->mtx );
  pthread_mutex_unlock( &args->mtx );
  args->joined = 1;
  pthread_cond_signal( &args->cond );
  return 0;
}

int
pthread_timedjoin_np( pthread_t td, void **res, struct timespec *ts )
{
  pthread_t tmp;
  int ret;
  struct args args = {.td = td,.res = res };

  pthread_mutex_init( &args.mtx, 0 );
  pthread_cond_init( &args.cond, 0 );
  pthread_mutex_lock( &args.mtx );

  ret = pthread_create( &tmp, 0, waiter, &args );
  if ( ret )
    goto done;

  do
    ret = pthread_cond_timedwait( &args.cond, &args.mtx, ts );
  while ( !args.joined && ret != ETIMEDOUT );

  pthread_cancel( tmp );
  pthread_join( tmp, 0 );

  pthread_cond_destroy( &args.cond );
  pthread_mutex_destroy( &args.mtx );

  return args.joined ? 0 : ret;
}
