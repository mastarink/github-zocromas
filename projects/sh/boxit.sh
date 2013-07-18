#!/bin/sh
if [[ -f "${MAS_PROJECTS_DIR:-.}/sh/setup.sh" ]] ; then
  . ${MAS_PROJECTS_DIR:-.}/sh/setup.sh

  echo "cp -a $savedir/*`datem`* boxes/box.com.zocromas/"
  echo "cp -a $savedir/*`datem`* boxes/dropbox.com.zocromas/"
fi
