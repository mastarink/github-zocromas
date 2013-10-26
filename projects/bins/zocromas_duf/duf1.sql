CREATE TABLE files (pathindex NUMERIC, name TEXT, inode INTEGER PRIMARY KEY, mdindex INTEGER);
CREATE INDEX "filename" on files (name ASC);
CREATE TABLE paths (name TEXT, inode INTEGER PRIMARY KEY, parentindex NUMERIC);
CREATE INDEX "dirname" on paths (name ASC);
