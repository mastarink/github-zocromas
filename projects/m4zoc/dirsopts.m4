AC_ARG_WITH([base-dir], [ AC_HELP_STRING([--with-base-dir=DIR], [base dir])  ],
  [with_base_dir=$withval],[with_base_dir="/tmp"])
AC_SUBST([with_base_dir])
AC_DEFINE_UNQUOTED(MAS_BASE_DIR, "${with_base_dir}",
	       [Define base_dir])
AC_SUBST([MAS_BASE_DIR])

AC_ARG_WITH([pids-dir], [ AC_HELP_STRING([--with-pids-dir=DIR], [pids dir])  ],
  [with_pids_dir=$withval],[with_pids_dir="zocromas/pid"])
AC_SUBST([with_pids_dir])
AC_DEFINE_UNQUOTED(MAS_PIDS_DIR, "${with_pids_dir}",
	       [Define pids_dir])
AC_SUBST([MAS_PIDS_DIR])

AC_DEFINE_UNQUOTED(MAS_PREFIX, "${prefix}",
	       [Define prefix])
AC_SUBST([MAS_PREFIX])

AC_ARG_WITH([proto-dir], [ AC_HELP_STRING([--with-proto-dir=DIR], [proto dir])  ],
  [with_proto_dir=$withval],[with_proto_dir="zocromas_zocprotos"])
AC_SUBST([with_proto_dir])
AC_DEFINE_UNQUOTED(MAS_PROTO_DIR, "${with_proto_dir}",
	       [Define proto_dir])
AC_SUBST([MAS_PROTO_DIR])

AC_ARG_WITH([mods-dir], [ AC_HELP_STRING([--with-mods-dir=DIR], [mods dir])  ],
  [with_mods_dir=$withval],[with_mods_dir="zocmds"])
AC_SUBST([with_mods_dir])
AC_DEFINE_UNQUOTED(MAS_MODS_DIR, "${with_mods_dir}",
	       [Define mods_dir])
AC_SUBST([MAS_MODS_DIR])

AC_ARG_WITH([log-dir], [ AC_HELP_STRING([--with-log-dir=DIR], [log dir])  ],
  [with_log_dir=$withval],[with_log_dir="log"])
AC_SUBST([with_log_dir])
AC_DEFINE_UNQUOTED(MAS_LOG_DIR, "${with_log_dir}",
	       [Define log_dir])
AC_SUBST([MAS_LOG_DIR])

AC_ARG_WITH([server], [ AC_HELP_STRING([--with-server], [server])  ],
  [with_server=$withval],[with_server="0.0.0.0"])
AC_SUBST([with_server])
AC_DEFINE_UNQUOTED(MAS_SERVER_STRING, "${with_server}",
	       [Define server_string])
AC_SUBST([MAS_SERVER_STRING])

AC_ARG_WITH([default-port], [ AC_HELP_STRING([--with-default-port=PORT], [default port])  ],
  [with_default_port=$withval],[with_default_port=50005])
AC_SUBST([with_default_port])
AC_DEFINE_UNQUOTED(MAS_DEFAULT_PORT, ${with_default_port},
	       [Define default port])
AC_SUBST([MAS_DEFAULT_PORT])

AC_ARG_WITH([def-proto], [ AC_HELP_STRING([--with-def-proto], [def-proto])  ],
  [with_def_proto=$withval],[with_def_proto="xcromas"])
AC_SUBST([with_def_proto])
AC_DEFINE_UNQUOTED(MAS_SERVER_DEF_PROTO, "${with_def_proto}",
	       [Define server_def_proto])
AC_SUBST([MAS_SERVER_DEF_PROTO])



