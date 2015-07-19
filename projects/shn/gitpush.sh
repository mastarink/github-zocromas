#!/bin/sh
echo " gmas ----------------------------------------"
git push gmas master
echo " github (ghn) ----------------------------------------"
git push ghn master
echo " github (gh) ----------------------------------------"
git push gh master
echo " gitlab (gl) ----------------------------------------"
git push gl master
echo " bitbucket (bb) ----------------------------------------"
git push bb master
# echo " xp-dev ----------------------------------------"
# git push xp-dev master

# # remote: 
# # remote: ============================== WARNING ==============================
# # remote: 
# # remote: Gitorious.org is entering read-only mode on May 15th. If you still
# # remote: need push capabilities please migrate your repository to GitLab.com
# # remote: 
# # remote: ============================== WARNING ==============================
# # remote: 
# echo " gitorius-zoc ----------------------------------------"
# git push gitorius-zoc master
