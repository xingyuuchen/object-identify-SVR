#!/bin/sh

function kill_procs() {
  while [ $# != 0 ]
  do
    echo $1
#    kill -9 ${pid}
    shift
  done
}

#kill_procs $(ps -A | grep 'oi_svr$' | awk '{print $1}')
kill_procs $(ps -A | grep 'zsh' | awk '{print $1}')
#kill_procs `netstat -anp | grep 5002`
