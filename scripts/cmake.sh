#!/bin/sh

if [ `uname` != "Darwin" ]; then
  export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/usr/local/lib
fi

isRun=false
isBuild=false
isNohup=false

cd ..

while [ $# != 0 ]
do
  if [ "$1" == "-r" ]; then
    isRun=true
  elif [ "$1" == "-b" ]; then
    isBuild=true
  elif [ "$1" == "-n" ]; then
    isNohup=true
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
    if ${isNohup}; then
      cmake .. && make
      if [ $? -eq 0 ]; then
        nohup ./oi_svr > ../logs/log.txt 2>&1 &
      fi
      echo
      echo "oi_svr running..."
      echo
    else
      cmake .. && make && ./oi_svr
    fi
  else
    cmake .. && make
  fi
fi

