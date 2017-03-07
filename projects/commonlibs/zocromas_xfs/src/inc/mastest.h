#ifndef MASTEST_H
# define MASTEST_H

# include "masxfs_types.h"

#define TEST_FD_EXAMS (1L+4L*(long)lim.rlim_cur-3L*3L)
#define TEST_0_PATH_EXAMS(_d) (23+(_d>1)+5*_d)
#define TEST_0_EXAMS  0 + ( 10L + TEST_FD_EXAMS * 4L + 5L + 1L + TEST_0_PATH_EXAMS( 1L ) + TEST_0_PATH_EXAMS( 2L ) + TEST_0_PATH_EXAMS( 14 ) * 2L )

#endif
