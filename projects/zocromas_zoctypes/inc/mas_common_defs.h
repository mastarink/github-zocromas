#ifndef MAS_COMMON_DEFS_H
#  define MAS_COMMON_DEFS_H


/* #define MAS_NOPASS_OPTS */

/* #  ifdef MAS_NOPASS_OPTS                                                                     */
/* #    define  MAS_PASS_OPTS_DECLARE mas_options_t * _popts,                                   */
/* #    define  MAS_PASS_OPTS_DECLARE1 mas_options_t * _popts                                   */
/* #    define MAS_PASS_OPTS_DCLOFF 1                                                           */
/* #    define MAS_PASS_OPTS_PASS NULL,                                                         */
/* #    define MAS_PASS_OPTS_PASS1 NULL                                                         */
/* #    define MAS_PASS_OPTS_DECL_PREF extern mas_options_t gopts __attribute__ ( ( unused ) )  */
/* #    define MAS_PASS_OPTS_DECL_GPREF extern mas_options_t gopts __attribute__ ( ( unused ) ) */
/* #    define MAS_PASS_OPTS_REF &gopts                                                         */
/* #    define MAS_PASS_OPTS_PREF gopts.                                                        */
/* #    define MAS_PASS_OPTS_GPREF gopts.                                                       */
/* #    define MAS_PASS_OPTS_DECL_GREF                                                          */
/* #    define MAS_PASS_OPTS_GREF NULL,                                                         */
/* #    define MAS_PASS_OPTS_GREF1 NULL                                                         */
/* #  else                                                                                      */
/* #    define  MAS_PASS_OPTS_DECLARE mas_options_t * popts,                                    */
/* #    define  MAS_PASS_OPTS_DECLARE1 mas_options_t * popts                                    */
/* #    define MAS_PASS_OPTS_DCLOFF 1                                                           */
/* #    define MAS_PASS_OPTS_PASS popts,                                                        */
/* #    define MAS_PASS_OPTS_PASS1 popts                                                        */
/* #    define MAS_PASS_OPTS_DECL_PREF                                                          */
/* #    define MAS_PASS_OPTS_DECL_GPREF extern mas_options_t gopts __attribute__ ( ( unused ) ) */
/* #    define MAS_PASS_OPTS_REF popts                                                          */
/* #    define MAS_PASS_OPTS_PREF popts->                                                       */
/* #    define MAS_PASS_OPTS_GPREF gopts.                                                       */
/* #    define MAS_PASS_OPTS_DECL_GREF extern mas_options_t gopts __attribute__ ( ( unused ) )  */
/* #    define MAS_PASS_OPTS_GREF &gopts,                                                       */
/* #    define MAS_PASS_OPTS_GREF1 &gopts                                                       */
/* #  endif                                                                                     */

#  define CTRL_PREPARE extern mas_control_t ctrl __attribute__ ( ( weak ) )
#  define EVAL_PREPARE CTRL_PREPARE

#  define IEVAL_STD(_rv,_code) ( _rv = ( _rv >=0 ? ( _code ) : _rv ) )

#  ifdef MAS_NO_CTRL
#    define MAS_CTRL_IS_CLIENT 1
#    define MAS_CTRL_IS_SERVER 0
#    define MAS_CTRL_IN_CLIENT 1
#    define MAS_CTRL_IN_SERVER 0
#    define MAS_CTRL_MESSAGES  1
#    define MAS_CTRL_STATUS    0
#    define IEVALME(_rv, _code, pserrno, fmt, msg) IEVAL_STD(_rv, _code)
#    define IEVALM(_rv, _code, fmt, msg) IEVALME(_rv, _code, NULL, fmt, msg)
#    define YEVALM(_rv, _code, fmt, msg) IEVAL_STD(_rv, _code)
#    if 0
#      define EEVALM(_rv, _code, fmt, msg) IEVAL_STD(_rv, _code)
#    endif

#  else
#    define MAS_CTRL_IS_SERVER &ctrl && ctrl.is_server
#    define MAS_CTRL_IS_CLIENT &ctrl && ctrl.is_client
#    define MAS_CTRL_IN_SERVER &ctrl && ctrl.in_server
#    define MAS_CTRL_IN_CLIENT &ctrl && ctrl.in_client
#    define MAS_CTRL_MESSAGES (&ctrl && ( (ctrl.messages || \
      	(ctrl.messages_parent && ctrl.is_parent) || \
        (ctrl.messages_child && ctrl.is_child)) || \
         !ctrl.messages_set ))
#    define MAS_CTRL_STATUS &ctrl && ctrl.status

#    define _ERRHAN(sys,_rv,_rvr,merrno,perrno,pserrno,fmt,msg) \
              ( _rvr = (&ctrl ? (ctrl.error_handler)(FL, sys, _rv, merrno, perrno, pserrno, fmt, msg) : -1) )

#    define ERRREG(fmt,msg) (&ctrl ? (ctrl.error_handler)(FL, 0, -1, 0, NULL,NULL, fmt, msg) : -1)
#    define ERRRG(msg) (&ctrl ? (ctrl.error_handler)(FL, 0, -1, 0, NULL,NULL, "%s", msg) : -1)

#    define IEVALM(_rv, _code, fmt, msg) IEVALME(_rv, _code, NULL, fmt, msg)

#      define YEVALM(_rv, _code, fmt, msg) \
  (IEVAL_STD(_rv, _code), ( _rv < 0 ? _ERRHAN(1,_rv,_rv,errno,&errno,NULL,fmt,msg) : _rv) )
#      define IEVALME(_rv, _code, pserrno, fmt, msg) \
  (IEVAL_STD(_rv, _code), ( _rv < 0 ? _ERRHAN(0,_rv,_rv,errno,&errno,pserrno,fmt,msg) : _rv) )
#    if 0
#      define EEVALM(_rv, _tmp, _code, fmt, msg) \
    ( (_tmp = ( _code ) ) && ( _rv = ( _tmp ? _ERRHAN(0,-1,_tmp,_rv,NULL,NULL,fmt,msg) : _tmp ) ) )
#    endif


#  endif


#  if 0
#    define EEVAL(_rv, _code) EEVALM(_rv, _code, NULL, NULL)
#  endif

#  define IEVAL_OPT(_rv, _code)  IEVAL_STD(_rv, _code)
#  define IEVAL(_rv, _code) IEVALM(_rv, _code, #_code/*fmt*/, NULL /*msg*/)
#  define IEVALE(_rv, _code, _pserrno) IEVALME(_rv, _code, _pserrno, #_code/*fmt*/, NULL/*fmt*/)

#  define YEVAL_OPT(_rv, _code) IEVAL_STD(_rv, _code)
#  define YEVAL(_rv, _code) YEVALM(_rv, _code, #_code/*fmt*/, NULL/*fmt*/)




#endif
