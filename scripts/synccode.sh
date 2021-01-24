#!/bin/sh

# This script is to sync local code with server,
# which makes it easy to debug on server.


isBlockMonitor=false

cd ..

while [ $# != 0 ]
do
  if [ "$1" == "-b" ]; then
    isBlockMonitor=true
  fi
  shift
done

function sync_project() {
  rsync -av --exclude='.*' --exclude-from='.gitignore' \
      ./ root@49.235.29.121:/root/cxy/object-identify-SVR
}

if ${isBlockMonitor} ; then
  fswatch . | while read file ; do
    echo ${file} has changed.
    sync_project
  done

else
  echo "start sync..."
  echo
  sync_project
fi


