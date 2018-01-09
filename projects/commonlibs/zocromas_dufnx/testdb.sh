#!/bin/sh
mysql -umasdufnt -pi2xV9KrTA54HRpj4e masdufntdb -e 'SELECT name, size, ishd.sum_id, COUNT(*) AS nsame, fp.atim, fp.ctim, fp.mtim  FROM qstd_sha1dref  AS ishd   LEFT JOIN qstd_sha1  AS isha ON (isha.id=ishd.sum_id) LEFT JOIN qstd_filedatas AS fd ON (data_id=fd.id) LEFT JOIN qstd_filenames AS fn ON (fn.data_id=fd.id) LEFT JOIN qstd_fileprops AS fp ON (fp.data_id=fd.id) GROUP BY ishd.sum_id HAVING nsame>3'

