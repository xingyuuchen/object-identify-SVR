#!/bin/sh

isRun=false
isBuild=false

while [ $# != 0 ]
do
  if [ "$1" == "-r" ]; then
    isRun=true
  elif [ "$1" == "-b" ]; then
    isBuild=true
  fi
  shift
done

if ${isBuild}; then
  echo "building..."
  if [ ! -d "./build" ]; then
    mkdir build
  fi

  cd build
  cmake ..
  make
fi

if ${isRun}; then
  ./oi_svr
fi
