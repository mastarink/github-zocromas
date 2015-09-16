#ifndef DUF_SCCB_EVAL_STD_H
#  define DUF_SCCB_EVAL_STD_H

/* * see duf_sccb_eval.h for names explanation * */


int duf_ev_evnamed_list_std( const char *names ) __attribute__ ( ( warn_unused_result ) );
int duf_ev_evnamen_std( const char *name, size_t len ) __attribute__ ( ( warn_unused_result ) );
int duf_ev_evname_std( const char *name ) __attribute__ ( ( warn_unused_result ) );

int duf_ev_pdi_evnamen_std_pt( duf_depthinfo_t * pdi, const char *name, size_t namelen, duf_argvc_t * ptarg /*, duf_ufilter_t * pu */  )
      __attribute__ ( ( warn_unused_result ) );
int duf_ev_pdi_evname_std_pt( duf_depthinfo_t * pdi, const char *name, duf_argvc_t * ptarg /*, duf_ufilter_t * pu */  )
      __attribute__ ( ( warn_unused_result ) );

int duf_ev_pdi_evname_std_at( duf_depthinfo_t * pdi, const char *name, const char *arg /*, duf_ufilter_t * pu */  )
      __attribute__ ( ( warn_unused_result ) );
int duf_ev_pdi_evname_std( duf_depthinfo_t * pdi, const char *name /*, duf_ufilter_t * pu */  ) __attribute__ ( ( warn_unused_result ) );

#endif
