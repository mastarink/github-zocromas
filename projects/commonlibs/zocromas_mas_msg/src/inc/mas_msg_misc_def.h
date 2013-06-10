#ifndef MAS_MSG_MISC_DEF_H
#  define MAS_MSG_MISC_DEF_H
#ifdef MAS_NO_OPTS
#define MAS_MSG_BIT(bit) 1
#else
#include <mastar/types/mas_opts_types.h>
extern mas_msg_options_t *pmsg_opts;

#define MAS_MSG_BIT(testbit) pmsg_opts->bit.testbit
#endif

#  define memMSG(...) if (MAS_MSG_BIT(msg_mem)){MFP("%s:%u:", FL);MFP(__VA_ARGS__);MFP("\n");}
// *INDENT-OFF*
// #    define memMSG(...) MAS_MSGFL( ( pmsg_opts->bit.msg_mem ),               1, "<|    %5s   |>", "MEM",       NULL, __VA_ARGS__)
//						allow				details		pref_fmt	pref	suff	fmt,...
#define MSG(...)	MAS_MSGFL( MAS_MSG_NOTICE,  ( MAS_MSG_BIT(msg_notice	)),	1, "%-15s"      , "?"PACKAGE,  NULL,		__VA_ARGS__)
#define HMSG(...)	MAS_MSGFL( MAS_MSG_NOTICE,  ( MAS_MSG_BIT(msg_notice	)),	0, "%-15s"      , "?"PACKAGE,  "          ",	__VA_ARGS__)
#define FMSG(...)	MAS_MSGFL( MAS_MSG_NOTICE,   1,					0, "%-15s"      , "?"PACKAGE,  "          ",	__VA_ARGS__)
#define WMSG(...)	MAS_MSGFL( MAS_MSG_WATCH,   ( MAS_MSG_BIT(msg_watch	)),	0, "%-15s"      , "?"PACKAGE,  "          ",	__VA_ARGS__)
#define tMSG(...)	MAS_MSGFL( MAS_MSG_DEBUG,   ( MAS_MSG_BIT(msg_trace	)),	1, "<|   %-8s|>", "TRACE",	NULL,		__VA_ARGS__)
#define thMSG(...)	MAS_MSGFL( MAS_MSG_WARNING, ( MAS_MSG_BIT(msg_thread	)),	1, "<|   %-8s|>", "TH",		NULL,		__VA_ARGS__)
#define sigMSG(...)	MAS_MSGFL( MAS_MSG_NOTICE,  ( MAS_MSG_BIT(msg_signal	)),	1, "<|   %-8s|>", "SG",		NULL,		__VA_ARGS__)
#define ioMSG(...)	MAS_MSGFL( MAS_MSG_WARNING, ( MAS_MSG_BIT(msg_io	)),	1, "<|   %-8s|>", "IO",		NULL,		__VA_ARGS__)
#define mMSG(...)	MAS_MSGFL( MAS_MSG_WARNING, ( MAS_MSG_BIT(msg_main	)),	1, "<|   %-8s|>", "M",		NULL,		__VA_ARGS__)
#define rMSG(...)	MAS_MSGFL( MAS_MSG_WARNING, ( MAS_MSG_BIT(msg_transaction)),	1, "<|   %-8s|>", "R",		NULL,		__VA_ARGS__)
#define lMSG(...)	MAS_MSGFL( MAS_MSG_WARNING, ( MAS_MSG_BIT(msg_listen	)),	1, "<|   %-8s|>", "L",		NULL,		__VA_ARGS__)
#define wMSG(...)	MAS_MSGFL( MAS_MSG_WARNING, ( MAS_MSG_BIT(msg_wait	)),	1, "<|   %-8s|>", "WAIT",	NULL,		__VA_ARGS__)
#define GMSG(...)	MAS_MSGFL( MAS_MSG_WARNING,  1,					0, "     %-8s  ", "G",		NULL,		__VA_ARGS__)
#define cMSG(...)	MAS_MSGFL( MAS_MSG_WARNING, ( MAS_MSG_BIT(msg_cmd	)),	1, "<|   %-8s|>", "CMD",	NULL,		__VA_ARGS__)
#define IMSG(...)	MAS_MSGFL( MAS_MSG_TITLE,    1,					0, NULL         , "[*] ",	NULL,		__VA_ARGS__)
#define OMSG(...)	MAS_MSGFL( MAS_MSG_SUBTITLE, 1,					0, NULL         , NULL,		NULL,		__VA_ARGS__)
#define GDMSG(...)	MAS_MSGFL( MAS_MSG_WARNING,  1,					1, NULL         , NULL,		NULL,		__VA_ARGS__)
// *INDENT-ON*


#endif
