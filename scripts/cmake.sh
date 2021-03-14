#!/bin/sh

if [ `uname` != "Darwin" ]; then
  export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/usr/local/lib
fi

isRun=false
isDaemon=false

cd ..

while [ $# != 0 ]
do
  if [ "$1" == "-r" ]; then
    isRun=true
  elif [ "$1" == "-d" ]; then
    isDaemon=true
  fi
  shift
done

echo "building..."
if [ ! -d "./build" ]; then
  mkdir build
fi

cd build
if ${isRun}; then
  if ${isDaemon}; then
    cmake -DDAEMON=On .. && make
    if [ $? -eq 0 ]; then
#      nohup ./oi-svr > ../logs/log.txt 2>&1 &    # nohup is deprecated, replaced by daemon.
      ./oi-svr
    fi
    echo
    echo "starting an oi-svr daemon."
    echo
  else
    cmake -DDAEMON=Off .. && make && ./oi-svr
  fi
else
  cmake .. && make
fi

