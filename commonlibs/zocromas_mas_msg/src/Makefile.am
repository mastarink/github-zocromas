SUBDIRS= inc
lib_LTLIBRARIES = libzocromas_mas_msg.la libzocromas_mas_msg_nooc.la


libzocromas_mas_msg_la_SOURCES = mas_msg_tools.c mas_curses.c
libzocromas_mas_msg_la_LDFLAGS=@AM_LDFLAGS@  @DEPSX_LIBS@ 

libzocromas_mas_msg_nooc_la_SOURCES = mas_msg_tools.c mas_curses.c
libzocromas_mas_msg_nooc_la_CFLAGS=@AM_CFLAGS@  -DMAS_NO_CTRL -DMAS_NO_OPTS -DMAS_NO_THTOOLS
libzocromas_mas_msg_nooc_la_LDFLAGS=@AM_LDFLAGS@
