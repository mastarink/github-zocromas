#ifndef DUF_H
#  define DUF_H


/* cd /home/mastar/.mas/src/sqlite-3.8.4.3/work/sqlite-src-3080403/                                  */
/* ./configure  --enable-debug  --disable-tcl  --disable-amalgamation \                              */
/*     --prefix=$MSH_SHN_PRJTOP_DIR/admin/install/default/ ; make ; make install */



/* 20140420
 SEGFAULT:
A. -good
run    --db-name=test$$.db  --uni-scan -Z
run    --db-name=test$$.db  --uni-scan -OP /home/mastar/a/down/
run    --db-name=test$$.db  --uni-scan -Rid /home/mastar/a/down/

B. -segfault
run    --db-name=test$$.db  --uni-scan -Z
run    --db-name=test$$.db  --uni-scan -OPRid /home/mastar/a/down/

*********************
sqlite with uflag: debug :::


mastar@mastar .../bins/zocromas_duf $ run    --db-name=test$$.db  --uni-scan -OPRid5 /home/mastar/a/down/ 
. ○ make ok ○ . ○ installed ○ ------ to run 'zocromas_duf --db-name=test19639.db ...' -------
------------------------------------20140420.102643----

qargs: '--db-name=test19639.db' '--uni-scan' '-OPRid5' '/home/mastar/a/down/'
zocromas_duf: sqlite3.c:44875: sqlite3PagerAcquire: Assertion `(*ppPage)->pPager==pPager || (*ppPage)->pPager==0' failed.
Aborted (core dumped)

real	0m0.083s
user	0m0.014s
sys	0m0.006s
returned 134

◀  ●  ▶
#50  ▷[2430s]◁ /bin/bash;/bin/bash #19639 
₍Sun₎ ◆ 53.335s ▷ 20140420.102550 ▷ 20140420.102500 ▷ 20140417.145442
Apr 20 2014 ◆ 10:26:44 $19639 [xterm/25]  L1 /bind:roxterm@devel.0 (1/2/2/1)
mastar@mastar .../bins/zocromas_duf $



*********************
 */









/* 20140412                                                                                            */
/* reset:                                                                                              */
/*    run  --db-name=test20140412  --drop-tables --create-tables  --add-path /home/mastar/a/down/      */
/* test:                                                                                               */
/*    sqlite3 $MSH_SHN_PRJTOP_DIR/duf_db/test20140412 .tables                      */
/*    sqlite3 $MSH_SHN_PRJTOP_DIR/duf_db/test20140412 'select * from dufs.paths'    */
/* fill:                                                                                               */
/*    run  --db-name=test20140412  --uni-scan /mnt/new_media/media/down/  -R  --fill -v --trace-fill=1 */


/**
 20140409: 
run --recursive  --update-path /mnt/old_home/mastar/.mas --max-items=40 --max-depth=3 --min-size=10000000 -vvv

run  --uni-scan /home/mastar/a/down/ -R --max-depth=4  --tree --files
run  --uni-scan /home/mastar/a/down/ --max-depth=4  --max-items=70 --files --tree
run  --uni-scan /home/mastar/a/down/ --max-depth=4  --max-items=70 -R --tree


 **/


/* error codes */
/* - /usr/include/errno.h                  */
/* + /usr/include/bits/errno.h             */
/*   /usr/include/features.h               */
/* - /usr/include/linux/errno.h            */
/* - /usr/include/asm/errno.h              */
/* + /usr/include/asm-generic/errno.h      */
/* + /usr/include/asm-generic/errno-base.h */



/*
 * /mnt/new_media/media/Photo/
 * /mnt/new_media/media/Pictures.R.20120207.164339/
 * /mnt/miscn/video/Pictures/ /home/mastar/a/to-attach/
 * /home/mastar/a/zaurus/sd/Documents/image/
 * /home/mastar/.local/share/Trash/
 * /mnt/old_home/mastar/.mas/HTC_Legend/
 * /mnt/tall/htclegend/
 * /mnt/tall/atalk/backup/
 *
 * /mnt/new_media/media/down/
 * */

/* run tables drop tables check path add $MSH_SHN_PRJTOP_DIR/projects/bins/zocromas_duf/
 * sqlite3 -header -column $MSH_SHN_PRJTOP_DIR/duf_db/duf-photo.sqlite3  \
 * 				'SELECT ID, dirname, parentID  FROM dufs.paths order by ID limit 20'
 * */


/***
 * run --db-directory=$MSH_SHN_PRJTOP_DIR/duf_dab/ -N aaaaa a b c
 * run --db-directory=$MSH_SHN_PRJTOP_DIR/duf_dab/ -N aaaaa --drop-tables a b c
 * run --db-directory=$MSH_SHN_PRJTOP_DIR/duf_dab/ -N temp.sqlite3 --drop-tables --create-tables
 * run --db-directory=$MSH_SHN_PRJTOP_DIR/duf_db/ -N aaaaa a b c
 * run --db-directory=$MSH_SHN_PRJTOP_DIR/duf_db/ -N temp.sqlite3 --drop-tables 
 * run --db-directory=$MSH_SHN_PRJTOP_DIR/duf_db/ -N temp.sqlite3 --drop-tables --create-tables
 * run --db-directory=$MSH_SHN_PRJTOP_DIR/duf_db/ -N temp.sqlite3 --drop-tables --create-tables --add-path .
 * run --db-directory=$MSH_SHN_PRJTOP_DIR/duf_db/ -N temp.sqlite3 --drop-tables --create-tables --add-path 
 * 				/mnt/tall/htclegend/ /mnt/old_home/mastar/.mas/HTC_Legend/ /mnt/new_media/media/photo/Pictures
 * 				/mnt/new_media/media/photo/Pictures.R.20120207.164339 
 * run --db-directory=$MSH_SHN_PRJTOP_DIR/duf_db/ -N temp.sqlite3 --drop-tables --create-tables --add-path 
 * 				/mnt/tall/htclegend/ /mnt/old_home/mastar/.mas/HTC_Legend/ /mnt/new_media/media/photo/Pictures
 * 				/mnt/new_media/media/photo/Pictures.R.20120207.164339
 * run --db-directory=$MSH_SHN_PRJTOP_DIR/duf_db/ -N temp.sqlite3 --drop-tables --create-tables --add-path --update-path
 * 				/mnt/tall/htclegend/ /mnt/old_home/mastar/.mas/HTC_Legend/ /mnt/new_media/media/photo/Pictures
 * 				/mnt/new_media/media/photo/Pictures.R.20120207.164339 
 * run --db-directory=$MSH_SHN_PRJTOP_DIR/duf_db/ -N temp.sqlite3 --drop-tables --create-tables --add-path --update-path
 * 				--print-dirs 
 * run --db-directory=$MSH_SHN_PRJTOP_DIR/duf_db/ -N temp.sqlite3 --drop-tables --create-tables --add-path --update-path
 * 				--print-dirs --recursive .
 * run --db-directory=$MSH_SHN_PRJTOP_DIR/duf_db/ -N temp.sqlite3 --drop-tables --create-tables --add-path --update-path
 * 				--print-files --print-dirs --recursive .
 * run --db-directory=$MSH_SHN_PRJTOP_DIR/duf_db/ -N temp.sqlite3 --drop-tables --create-tables --add-path --update-path 
 * 				--print-files --recursive .
 * run --db-directory=$MSH_SHN_PRJTOP_DIR/duf_db/ -N temp.sqlite3 --drop-tables --create-tables --add-path --update-path 
 * 				--print-paths .
 * run --db-directory=$MSH_SHN_PRJTOP_DIR/duf_db/ -N temp.sqlite3 --drop-tables --create-tables --add-path --update-path 
 * 				--print-paths . /mnt/new_media/media/photo/Pictures.R.20120207.164339 
 * run --db-directory=$MSH_SHN_PRJTOP_DIR/duf_db/ -N temp.sqlite3 --drop-tables --create-tables --add-path --update-path 
 * 				--print-paths /mnt/tall/htclegend/ /mnt/old_home/mastar/.mas/HTC_Legend/ /mnt/new_media/media/photo/Pictures 
 * 				/mnt/new_media/media/photo/Pictures.R.20120207.164339 
 * run -D $MSH_SHN_PRJTOP_DIR/duf_db/
 * run -D $MSH_SHN_PRJTOP_DIR/duf_db/duf-photo
 * run -D $MSH_SHN_PRJTOP_DIR/duf_db/ -N 
 * run -D $MSH_SHN_PRJTOP_DIR/duf_db/ -N aaaaa
 * run -D $MSH_SHN_PRJTOP_DIR/duf_db/ -N aaaaa 
 * run -D $MSH_SHN_PRJTOP_DIR/duf_db/ -N aaaaa a b c
 * run -N temp.sqlite3 --add-path --update-path --print-dirs --recursive .
 * run -N temp.sqlite3 --add-to-group --group=aaaaaa mased/ ; . shsq/libsql.bash ; sqf_group temp
 * run -N temp.sqlite3 --add-to-group --group=bbbbbb mased/ ; . shsq/libsql.bash ; sqf_group temp
 * run -N temp.sqlite3 --group=aaaaaa mased/
 * run -N temp.sqlite3 --group aaaaaa mased/
 * run -N temp.sqlite3 --group=aaaaaa mased/ ; . shsq/libsql.bash ; sqf_group temp
 * run -N temp.sqlite3 --group=bbbbbb mased/ ; . shsq/libsql.bash ; sqf_group temp
 * run -N temp.sqlite3 --print-dirs --recursive .
 * run -N temp.sqlite3 --remove-from-group --group=aaaaaa mased/ ; . shsq/libsql.bash ; sqf_group temp
 * run -N temp.sqlite3 --remove-from-group --group=bbbbbb mased/ ; . shsq/libsql.bash ; sqf_group temp
 * run -N temp.sqlite3 --update-mad5 --print-dirs --recursive .
 * run -N temp.sqlite3 --update-mad5 --print-dirs --recursive a
 * run -N temp.sqlite3 --update-md5 --print-dirs --recuarsive a
 * run -N temp.sqlite3 --update-md5 --print-dirs --recursive .
 * run -N temp.sqlite3 --update-md5 --print-dirs --recursive a
 * run --limit=2 --same-md5  --recursive 
 * run --print-dirs --recursive
 * run --print-dirs --recursive 
 * run --print-dirs --recursive /mnt/new_media/media/photo/Pictures.R.20120207.164339
 * run --print-dirs --recursive /mnt/new_media/media/photo/Pictures.R.20120207.164339/Picasa
 * run --print-files  --recursive  --limit=10
 * run --print-files  --recursive  --limit=10 /mnt/new_media/media/photo/Pictures/
 * run --print-paths
 * run --same-files  --limit=10 /mnt/new_media/media/photo/
 * run --same-files  --limit=10 /mnt/new_media/media/photo/Pictures/all/mov/
 * run --same-files  --limit=10 /mnt/new_media/media/photo/Pictures/all/mov/bbbfc0b7c5e0b0547474cfac27fbb7b2.mov
 * run --same-files /mnt/new_media/media/photo/Pictures/all/mov/bbbfc0b7c5e0b0547474cfac27fbb7b2.mov
 * run --same-files --recursive 
 * run --same-files  --recursive  --limit=10 /mnt/new_media/media/photo/
 * run --same-files  --recursive  --limit=10 /mnt/new_media/media/photo/Pictures/
 * run --same-files --recursive  /mnt/new_media/media/photo
 * run --same-files --recursive  /mnt/new_media/media/photo/Pictures.R.20120207.164339
 * run --same-md5  --limit=10 
 * run --same-md5 --limit=5 --recursive 
 * run --same-md5 --recursive 
 * run --same-md5  --recursive --limit=10
 * 
 * run --update-exif
 * 
 * lshn && run --update-mdpath "'/mnt/new_media/media/photo/Pictures/unsorted/kodak/Kodak Pictures/08-14-2007'"
 * 
 * 
 * run --drop-tables --create-tables --add-path --update-path --update-md5 --recursive /mnt/tall/htclegend/ /mnt/old_home/mastar/.mas/HTC_Legend/ 
 * 
 * run --drop-tables --create-tables --add-path --update-path --update-md5 --update-duplicates --update-mdpath --update-filedata --update-exif
 * 		--recursive /mnt/tall/htclegend/ /mnt/old_home/mastar/.mas/HTC_Legend/ 
 * 		/mnt/new_media/media/photo/Pictures /mnt/new_media/media/photo/Pictures.R.20120207.164339/
 * 
 * run
 * */


/*
 *
 * $ run --db-directory=$MSH_SHN_PRJTOP_DIR/duf_db/ -N temp.sqlite3 --drop-tables --create-tables --add-path --update-path --print-dirs --recursive .
 * $ run --print-dirs --recursive
 *
 * */

/* file://usr/share/doc/sqlite-3.7.15.2/html/docs.html */
/* file://usr/share/doc/sqlite-3.8.4.3/html/docs.html */



#endif
