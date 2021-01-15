#!/bin/sh

if [ `uname` != "Darwin1" ]; then
  export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/usr/local/lib
fi

isRun=false
isBuild=false

cd ..

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
  if ${isRun}; then
    cmake .. && make && ./oi_svr
  else
    cmake .. && make
  fi
fi
