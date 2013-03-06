#!/bin/sh
if [[ -f "sh/setup.sh" ]] ; then
  . sh/setup.sh

  echo $projectsdir
fi
